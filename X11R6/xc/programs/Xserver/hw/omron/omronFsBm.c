/*
 * $XConsortium: omronFsBm.c,v 1.2 93/09/03 19:50:18 dpw Exp $
 *
 * Copyright 1991 by OMRON Corporation
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of OMRON not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  OMRON makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * OMRON DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL OMRON
 * BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#if defined(luna1) && defined(uniosu)

#include "omron.h" 

#include "omronFb.h" 

#define PALADR(p)		(p->reg.addr)
#define PALCONT(p)		(p->reg.control)
#define PALCDATA(p)		(p->reg.coldata)
#define PALOVCDATA(p)	(p->reg.ovcdata)

/*
**	color palette 
*/
struct pal{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

static struct pal paldata[256] = {
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0}
};

static struct pal palinit[256] = {
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0},
	{~0,~0,~0}, {0,0,0}
};

static struct pal palsave[256];

static int	  pagesize;

static Bool omronFsBmPalCreate();
static void omronFsBmPalInit();
static void omronFsBmSetPal();

Bool
omronFsBmCreate(omron_fb_info)
OmronFbInfoPtr omron_fb_info;
{
	FsBmMapPtr fs_bm_reg = (caddr_t)0xd0000000;	/* frame buffer i/o port */

	/* Set transparent Mapping. */
	if(sys9100(S91TPTRE, 0xd0, 0, 0) < 0) {
		return FALSE;
	}

	omron_fb_info->plane = (char *)0xd0200000;	

	omron_fb_info->fbmap = (char *)fs_bm_reg;
	omron_fb_info->refresh_reg = &(fs_bm_reg->refresh.reg);
	omron_fb_info->palmap      = (char *) &(fs_bm_map->palette);

	if (!omronFsBmPalCreate(omron_fb_info)) {
		return FALSE;
	}

	return TRUE;
}


static Bool
omronFsBmSaveScreen(pScreen, on)
ScreenPtr     pScreen;
Bool          on;
{
static int omronScreenIsSaved = FALSE;
	struct pal savepal[256]; 	
	OmronFbInfoPtr pFbInfo =
		(OmronFbInfoPtr)pScreen->devPrivates[omronScreenIndex].ptr;

	if (on != SCREEN_SAVER_ON) {
		omronSetLastEventTime();
		if(omronScreenIsSaved == TRUE) {
			omronFsBmSetPal(pFbInfo, palsave);
			omronScreenIsSaved = FALSE;
		}
		return TRUE;
	}

	bzero(savepal, sizeof(savepal));
	omronFsBmSetPal(pFbInfo, savepal);
	omronScreenIsSaved = TRUE;
	return TRUE;
}

Bool
omronFsBmInit(index, pScreen, argc, argv)
    int           index;
    ScreenPtr     pScreen;
    int           argc;
    char          **argv;
{
	OmronFbInfoPtr pFbInfo;
	extern miPointerScreenFuncRec  omronPointerScreenFuncs;
	
	pFbInfo = (OmronFbInfoPtr) pScreen->devPrivates[omronScreenIndex].ptr;

	omronFsBmPalInit(pFbInfo);

	if (!monitorResolution) {
		if (pFbInfo->fb_type == DT_PLASMA)
			monitorResolution = PLASMA_TV_RESOLUTION;
		else
			monitorResolution = MONO_TV_RESOLUTION; 
	}

	if(!mfbScreenInit(pScreen,(pointer)pFbInfo->plane,
			pFbInfo->scr_width,pFbInfo->scr_height,
			monitorResolution,monitorResolution,pFbInfo->fb_width)){
		ErrorF("mfbScreenInit Error.\n");
		return FALSE;
	}

   	pScreen->whitePixel = 0;
   	pScreen->blackPixel = 1;

	miDCInitialize (pScreen, &omronPointerScreenFuncs);

	mfbCreateDefColormap(pScreen);

	pScreen->SaveScreen = omronFsBmSaveScreen;

	omronFsBmSaveScreen(pScreen, SCREEN_SAVER_FORCER);

	return TRUE;

}

static void
omronFsBmPalClose(omron_fb_info)
OmronFbInfoPtr omron_fb_info;
{
	union fs_palette *fs_bm_pal;

	omronFsBmSetPal(omron_fb_info, paldata);

	fs_bm_pal = (union fs_palette *)(omron_fb_info->palmap);

	/* read mask register */
	PALADR(Fs_bm_pal)=    0x4L;
	PALCONT(fs_bm_pal) = 0L;

	/* blink mask register */
	PALADR(fs_bm_pal) = 0x5L;
	PALCONT(fs_bm_pal) = 0x0L;

	/* command register */
	PALADR(fs_bm_pal) = 0x6L;
	PALCONT(fs_bm_pal) = 0x2L;
}


static Bool
omronFsBmPalCreate(omron_fb_info)
OmronFbInfoPtr omron_fb_info;
{
	union fs_palette *fs_bm_pal;

	fs_bm_pal = (union fs_palette *)(omron_fb_info->palmap);

	/* read mask register */
	PALADR(fs_bm_pal)=    0x4L;
	PALCONT(fs_bm_pal) = (1 << (omron_fb_info->fb_depth) ) - 1;

	/* blink mask register */
	PALADR(fs_bm_pal) = 0x5L;
	PALCONT(fs_bm_pal) = 0x0L;

	/* command register */
	PALADR(fs_bm_pal) = 0x6L;
	PALCONT(fs_bm_pal) = 0x40L;

	return TRUE; 
}


static void
omronFsBmPalInit(omron_fb_info)
OmronFbInfoPtr omron_fb_info;
{
	memmove(palsave, palinit, sizeof(palinit));
	omronFsBmSetPal(omron_fb_info, palsave);
}


static void
omronFsBmSetPal(omron_fb_info, pal)
OmronFbInfoPtr omron_fb_info;
struct pal *pal;
{
	union fs_palette	   *fs_bm_pal;
	register int       i, j;
	unsigned char      *palp;

	fs_bm8_pal = (union palette_bm8 *)(omron_fb_info->palmap);
	for (i = 0; i < 256; i++) {
		palp = &pal++->red;
		PALADR(fs_bm_pal) = i;
		for (j = 0; j < 3; j++) {
			PALCDATA(fs_bm_pal) = *palp++;
		}
	}
}


static void
omronFsBmFbClear(omron_fb_info)
OmronFbInfoPtr omron_fb_info;
{
	register int *f;
	register int nw,height;
	int nwidth,nlwidth,nlwExtra;

	f = (int *)omron_fb_info->plane;
	nlwidth  = (omron_fb_info->fb_width)>>5;
	nwidth   = (omron_fb_info->scr_width)>>5;
	nlwExtra = nlwidth - nwidth;
	height =omron_fb_info->scr_height;

	while(height--) {
		nw = nwidth;
		while(nw--)
			*f++ = 0;
		f += nlwExtra;
	}
}


void
omronFsBmGiveUp(omron_fb_info)
OmronFbInfoPtr omron_fb_info;
{
	omronFsBmFbClear(omron_fb_info);
	omronFsBmPalClose(omron_fb_info);
}
#endif
