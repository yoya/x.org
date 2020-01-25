/* $XConsortium: ramdac.c,v 1.4 95/01/06 20:56:54 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/accel/et4000w32/w32/ramdac.c,v 3.4 1994/12/29 09:44:29 dawes Exp $ */
/*
 * Copyright 1990,91 by Thomas Roell, Dinkelscherben, Germany.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Thomas Roell not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Thomas Roell makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THOMAS ROELL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THOMAS ROELL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

#include "X.h"
#include "Xproto.h"
#include "windowstr.h"
#include "compiler.h"

#include "xf86.h"
#include "vga.h"
#include "w32.h"

#define NOMAPYET        (ColormapPtr) 0

static ColormapPtr InstalledMaps[MAXSCREENS];
				/* current colormap for each screen */

int
vgaListInstalledColormaps(pScreen, pmaps)
     ScreenPtr	pScreen;
     Colormap	*pmaps;
{
  /* By the time we are processing requests, we can guarantee that there
   * is always a colormap installed */
  
  *pmaps = InstalledMaps[pScreen->myNum]->mid;
  return(1);
}


static void
set_overscan(overscan)
    int overscan;
{
    inb(0x3da);
    GlennsIODelay();
    outb(0x3c0, 0x11);
    GlennsIODelay();
    outb(0x3c0, overscan);
    GlennsIODelay();
    inb(0x3da);
    GlennsIODelay();
    outb(0x3c0, 0x20);
    GlennsIODelay();
} 


#define RMR 0x3c6
#define READ 0x3c7
#define WRITE 0x3c8
#define RAM 0x3c9


static void
write_color(entry, cmap)
    int entry;
    unsigned char *cmap;
{
    outb(WRITE, entry);
    GlennsIODelay();
    outb(RAM, cmap[0]);
    GlennsIODelay();
    outb(RAM, cmap[1]);
    GlennsIODelay();
    outb(RAM, cmap[2]);
    GlennsIODelay();
} 


static void
read_color(entry, cmap)
    int entry;
    unsigned char *cmap;
{
    outb(READ, entry);
    GlennsIODelay();
    cmap[0] = inb(RAM);
    GlennsIODelay();
    cmap[1] = inb(RAM);
    GlennsIODelay();
    cmap[2] = inb(RAM);
    GlennsIODelay();
}


static W32Blanked = FALSE;

static unsigned char black_cmap[] = {0x0, 0x0, 0x0};
static unsigned char white_cmap[] = {0xff, 0xff, 0xff};

void
vgaStoreColors(pmap, ndef, pdefs)
     ColormapPtr	pmap;
     int		ndef;
     xColorItem	        *pdefs;
{
    int		i;
    unsigned char *cmap, *tmp;
    xColorItem	directDefs[256];
    Bool          new_overscan = FALSE;
    unsigned char overscan = ((vgaHWPtr)vgaNewVideoState)->Attribute[OVERSCAN];
    unsigned char old_overscan; 
    unsigned char tmp_overscan;
   
    extern RamdacShift;

    if (pmap != InstalledMaps[pmap->pScreen->myNum])
        return;

    /* GJA -- We don't want cfb code right now (in vga16 server) */
    if ((pmap->pVisual->class | DynamicClass) == DirectColor)
    {
        ndef = cfbExpandDirectColors (pmap, ndef, pdefs, directDefs);
        pdefs = directDefs;
    }

    for(i = 0; i < ndef; i++)
    {
        if (pdefs[i].pixel == overscan)
	{
	    old_overscan = overscan; 
	    new_overscan = TRUE;
	}
        cmap = &((vgaHWPtr)vgaNewVideoState)->DAC[pdefs[i].pixel*3];
        cmap[0] = pdefs[i].red   >> RamdacShift;
        cmap[1] = pdefs[i].green >> RamdacShift;
        cmap[2] = pdefs[i].blue  >> RamdacShift;

        if (xf86VTSema && (!W32Blanked || pdefs[i].pixel != overscan))
	{
	    outb(0x3C8, pdefs[i].pixel);
	    GlennsIODelay();
	    outb(0x3C9, cmap[0]);
	    GlennsIODelay();
	    outb(0x3C9, cmap[1]);
	    GlennsIODelay();
	    outb(0x3C9, cmap[2]);
	    GlennsIODelay();
	}
    }	
    if (new_overscan)
    {
	new_overscan = FALSE;
        for(i = 0; i < ndef; i++)
        {
            if (pdefs[i].pixel == overscan)
	    {
	        if ((pdefs[i].red != 0) || 
	            (pdefs[i].green != 0) || 
	            (pdefs[i].blue != 0))
	        {
	            new_overscan = TRUE;
		    tmp_overscan = overscan;
        	    tmp = &((vgaHWPtr)vgaNewVideoState)->DAC[pdefs[i].pixel*3];
	        }
	        break;
	    }
        }
        if (new_overscan)
        {
            /*
             * Find a black pixel, or the nearest match.
             */
            for (i=255; i >= 0; i--)
	    {
                cmap = &((vgaHWPtr)vgaNewVideoState)->DAC[i*3];
	        if ((cmap[0] == 0) && (cmap[1] == 0) && (cmap[2] == 0))
	        {
	            overscan = i;
	            break;
	        }
	        else
	        {
/* CHANGE to SUMMATION--GGLGGL */ 
	            if ((cmap[0] < tmp[0]) && 
		        (cmap[1] < tmp[1]) && (cmap[2] < tmp[2]))
	            {
		        tmp = cmap;
		        tmp_overscan = i;
	            }
	        }
	    }
	    if (i < 0)
	    {
	        overscan = tmp_overscan;
	    }
	    ((vgaHWPtr)vgaNewVideoState)->Attribute[OVERSCAN] = overscan;
            if (xf86VTSema)
	    {
		if (W32Blanked)
		    write_color(overscan, black_cmap);

	        (void)inb(vgaIOBase + 0x0A);
		GlennsIODelay();
	        outb(0x3C0, OVERSCAN);
		GlennsIODelay();
	        outb(0x3C0, overscan);
		GlennsIODelay();
	        (void)inb(vgaIOBase + 0x0A);
		GlennsIODelay();
	        outb(0x3C0, 0x20);
		GlennsIODelay();

		if (W32Blanked)
		{
		    cmap = &((vgaHWPtr)vgaNewVideoState)->DAC[old_overscan * 3];
		    write_color (old_overscan, cmap);
		}
	    }
        }
    }
}


void
vgaInstallColormap(pmap)
     ColormapPtr	pmap;
{
  ColormapPtr oldmap = InstalledMaps[pmap->pScreen->myNum];
  int         entries;
  Pixel *     ppix;
  xrgb *      prgb;
  xColorItem *defs;
  int         i;


  if (pmap == oldmap)
    return;

  if ((pmap->pVisual->class | DynamicClass) == DirectColor)
    entries = (pmap->pVisual->redMask |
	       pmap->pVisual->greenMask |
	       pmap->pVisual->blueMask) + 1;
  else
    entries = pmap->pVisual->ColormapEntries;

  ppix = (Pixel *)ALLOCATE_LOCAL( entries * sizeof(Pixel));
  prgb = (xrgb *)ALLOCATE_LOCAL( entries * sizeof(xrgb));
  defs = (xColorItem *)ALLOCATE_LOCAL(entries * sizeof(xColorItem));

  if ( oldmap != NOMAPYET)
    WalkTree( pmap->pScreen, TellLostMap, &oldmap->mid);

  InstalledMaps[pmap->pScreen->myNum] = pmap;

  for ( i=0; i<entries; i++) ppix[i] = i;

  QueryColors( pmap, entries, ppix, prgb);

  for ( i=0; i<entries; i++) /* convert xrgbs to xColorItems */
    {
      defs[i].pixel = ppix[i];
      defs[i].red = prgb[i].red;
      defs[i].green = prgb[i].green;
      defs[i].blue = prgb[i].blue;
      defs[i].flags =  DoRed|DoGreen|DoBlue;
    }

  vgaStoreColors( pmap, entries, defs);

  WalkTree(pmap->pScreen, TellGainedMap, &pmap->mid);
  
  DEALLOCATE_LOCAL(ppix);
  DEALLOCATE_LOCAL(prgb);
  DEALLOCATE_LOCAL(defs);
}


void
vgaUninstallColormap(pmap)
     ColormapPtr pmap;
{
  ColormapPtr defColormap;
  
  if ( pmap != InstalledMaps[pmap->pScreen->myNum] )
    return;

  defColormap = (ColormapPtr) LookupIDByType( pmap->pScreen->defColormap,
					      RT_COLORMAP);

  if (defColormap == InstalledMaps[pmap->pScreen->myNum])
    return;

  (*pmap->pScreen->InstallColormap) (defColormap);
}


/*
 *    The power saver is for w32p_rev_c and later only--GGL. 
 */
Bool
W32SaveScreen (pScreen, on)
     ScreenPtr     pScreen;
     Bool          on;
{
    unsigned char state;
    unsigned char *cmap;
    unsigned char overscan; 
    extern vgaPowerSaver;

    if (on)
	SetTimeSinceLastInputEvent();
    if (xf86VTSema)
    {
	outb(vgaIOBase + 4, 0x34);
	state = inb(vgaIOBase + 5);
  
	if (on) {
	    state &= ~0x21;
	    W32Blanked = FALSE; 

	    overscan = ((vgaHWPtr)vgaNewVideoState)->Attribute[OVERSCAN];
	    cmap = &((vgaHWPtr)vgaNewVideoState)->DAC[overscan * 3];

	    inb(vgaIOBase + 0x0A);
	    GlennsIODelay();
	    outb(0x3C0, 0x20);         
	    GlennsIODelay();

	    write_color(overscan, cmap);

	    if (vgaPowerSaver && W32pCAndLater)
	    {
		outb(vgaIOBase + 4, 0x34);
		GlennsIODelay();
		outb(vgaIOBase + 5, state);
		GlennsIODelay();
	    }
	} else {
	    state |= 0x21;
	    W32Blanked = TRUE; 
    
	    overscan = ((vgaHWPtr)vgaNewVideoState)->Attribute[OVERSCAN];
	    write_color(overscan, black_cmap);

	    inb(vgaIOBase + 0x0A);
	    GlennsIODelay();
	    outb(0x3C0, 0x00);         
	    GlennsIODelay();

	    if (vgaPowerSaver && W32pCAndLater)
	    {
		outb(vgaIOBase + 4, 0x34);
		GlennsIODelay();
		outb(vgaIOBase + 5, state);
		GlennsIODelay();
	    }
	}
    }
    return(TRUE);
}


/*
 *  For a description of the following, see AT&T's data sheet for ATT20C490/491
 *  and ATT20C492/493--GGL
 */
static void write_cr(cr)
    int cr;
{
    inb(WRITE); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    outb(RMR, cr); GlennsIODelay();
    inb(WRITE); GlennsIODelay();
}


static read_cr()
{
    unsigned int cr;

    inb(WRITE); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    cr = inb(RMR); GlennsIODelay();
    inb(WRITE);
    return cr;
}


int RamdacShift;
extern int vgaRamdacMask;
static saved_cr;
static cr_saved;
static rmr;
static check_ramdac()
{
    unsigned char cmap[3], save_cmap[3];

    RamdacShift = 10;
    vgaRamdacMask = 0x3f;

    rmr = inb(RMR);
    saved_cr = read_cr();
    cr_saved = TRUE;

    outb(RMR, 0xff); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    inb(RMR); GlennsIODelay();
    outb(RMR, 0x1c); GlennsIODelay();

    if (inb(RMR) != 0xff)
    {
	cr_saved = FALSE;
	ErrorF("Ramdac:  ATT20C47xA or Sierra 1502[5-6]\n");
	return;
    }

    write_cr(0xe0);
    if ((read_cr() >> 5) != 0x7)
    {
	ErrorF("Ramdac:  ATT20C497\n");
	return;
    }

    write_cr(0x60);
    if ((read_cr() >> 5) == 0)
    {
	write_cr(0x2);	
	if ((read_cr() & 0x2) != 0)
	{
	    ErrorF("Ramdac:  ATT20C490\n");
	    RamdacShift = 8;
	    vgaRamdacMask = 0xff;
	}
	else
	    ErrorF("Ramdac:  ATT20C493\n");
    }
    else
    {
	write_cr(0x2);	
	outb(RMR, 0xff);
	read_color(0xff, save_cmap);

	write_color(0xff, white_cmap);
	read_color(0xff, cmap);

	if (cmap[0] == 0xff && cmap[1] == 0xff && cmap[2] == 0xff)
	{
	    ErrorF("Ramdac:  ATT20C491\n");
	    RamdacShift = 8;
	    vgaRamdacMask = 0xff;
	}
	else
	    ErrorF("Ramdac:  ATT20C492\n");

	write_color(0xff, save_cmap);
    }
}


void VGARamdac()
{
    if (cr_saved)
	write_cr(saved_cr);
    outb(RMR, rmr);
}


void XRamdac()
{
    if (RamdacShift == 8)
	write_cr(0x2);
    outb(RMR, 0xff);
}


void SetupRamdac()
{
    check_ramdac();
    if (cr_saved && RamdacShift == 10)
	write_cr(saved_cr);
    outb(RMR, 0xff);
}
