/* $XConsortium: gen_driver.c,v 1.3 95/01/16 13:17:59 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/vga2/drivers/generic/gen_driver.c,v 3.6 1995/01/10 10:29:23 dawes Exp $ */

/*
 * Generic VGA driver for mono operation.  This driver doesn't do much since
 * most of the generic stuff is done in vgaHW.c
 *
 * David Dawes August 1992
 */

#include "X.h"
#include "input.h"
#include "screenint.h"
#include "dix.h"

#include "compiler.h"

#include "xf86.h"
#include "xf86Priv.h"
#include "xf86_OSlib.h"
#include "xf86_HWlib.h"
#include "vga.h"

typedef struct {
  vgaHWRec std;               /* good old IBM VGA */
  } vgaGENERICRec, *vgaGENERICPtr;

static Bool     GENERICProbe();
static char *   GENERICIdent();
static Bool     GENERICClockSelect();
static void     GENERICEnterLeave();
static Bool     GENERICInit();
static Bool     GENERICValidMode();
static void *   GENERICSave();
static void     GENERICRestore();
static void     GENERICAdjust();

vgaVideoChipRec GENERIC = {
  GENERICProbe,
  GENERICIdent,
  GENERICEnterLeave,
  GENERICInit,
  GENERICValidMode,
  GENERICSave,
  GENERICRestore,
  GENERICAdjust,
  (void (*)())NoopDDA,
  (void (*)())NoopDDA,
  (void (*)())NoopDDA,
  (void (*)())NoopDDA,
  (void (*)())NoopDDA,
  (void (*)())NoopDDA,
  0x10000,
  0x10000,
  16,
  0xFFFF,
  0x00000, 0x10000,
  0x00000, 0x10000,
  TRUE,
  VGA_NO_DIVIDE_VERT,
  {0,},
  32,
  FALSE,
  0,
  0,
  FALSE,
  FALSE,
  NULL,
  1,
};

#define new ((vgaGENERICPtr)vgaNewVideoState)


/*
 * GENERICIdent
 */

static char *
GENERICIdent(n)
     int n;
{
  static char *chipsets[] = {"generic"};

  if (n + 1 > sizeof(chipsets) / sizeof(char *))
    return(NULL);
  else
    return(chipsets[n]);
}


/*
 * GENERICClockSelect --
 *      select one of the possible clocks ...
 */

static Bool
GENERICClockSelect(no)
     int no;
{
  static unsigned char save1;
  unsigned char temp;

  switch(no)
  {
    case CLK_REG_SAVE:
      save1 = inb(0x3CC);
      break;
    case CLK_REG_RESTORE:
      outb(0x3C2, save1);
      break;
    default:
      temp = inb(0x3CC);
      outb(0x3C2, (temp & 0xf3) | ((no << 2) & 0x0C));
  }
  return(TRUE);
}


/*
 * GENERICProbe --
 *      check up whether a VGA board is installed
 */

static Bool
GENERICProbe()
{

  /*
   * Set up I/O ports to be used by this card
   */
  xf86ClearIOPortList(vga2InfoRec.scrnIndex);
  xf86AddIOPorts(vga2InfoRec.scrnIndex, Num_VGA_IOPorts, VGA_IOPorts);

  if (vga2InfoRec.chipset)
    {
      if (StrCaseCmp(vga2InfoRec.chipset, GENERICIdent(0)))
	return (FALSE);
      else
	GENERICEnterLeave(ENTER);
    }
  else
    {
      unsigned char temp, origVal, newVal;

      GENERICEnterLeave(ENTER);

      /*
       * Check if there is a VGA.  VGA has one more attribute register
       * than EGA, so see if we can read/write it.
       */

      temp = inb(vgaIOBase + 0x0A); /* reset ATC flip-flop */
      outb(0x3C0, 0x14 | 0x20); origVal = inb(0x3C1);
      outb(0x3C0, origVal ^ 0x0F);
      outb(0x3C0, 0x14 | 0x20); newVal = inb(0x3C1);
      outb(0x3C0, origVal);
      if (newVal != (origVal ^ 0x0F))
	{
	  GENERICEnterLeave(LEAVE);
	  return(FALSE);
	}
    }

  /*
   * We only need the minimum 64k of memory, so don't try to check.
   * For vga16, we assume 256k.
   */
  if (!vga2InfoRec.videoRam)
    {
#ifdef XF86VGA16
      vga2InfoRec.videoRam = 256;
#else
      vga2InfoRec.videoRam = 64;
#endif
    }
  if (!vga2InfoRec.clocks)
    {

      /* some vga cards not explicitly supported may have 4 clocks available */
      vgaGetClocks(4, GENERICClockSelect);
    }

  vga2InfoRec.chipset = GENERICIdent(0);
  vga2InfoRec.bankedMono = FALSE;
  return(TRUE);
}


/*
 * GENERICEnterLeave --
 *      enable/disable io-mapping
 */

static void 
GENERICEnterLeave(enter)
     Bool enter;
{
  unsigned char temp;

  if (enter)
    {
      xf86EnableIOPorts(vga2InfoRec.scrnIndex);

      vgaIOBase = (inb(0x3CC) & 0x01) ? 0x3D0 : 0x3B0;
      /* Unprotect CRTC[0-7] */
      outb(vgaIOBase + 4, 0x11); temp = inb(vgaIOBase + 5);
      outb(vgaIOBase + 5, temp & 0x7F);
    }
  else
    {
      xf86DisableIOPorts(vga2InfoRec.scrnIndex);
    }
}



/*
 * GENERICRestore --
 *      restore a video mode
 */

static void 
GENERICRestore(restore)
  vgaGENERICPtr restore;
{
  vgaHWRestore((vgaHWPtr)restore);
}



/*
 * GENERICSave --
 *      save the current video mode
 */

static void *
GENERICSave(save)
     vgaGENERICPtr save;
{
  save = (vgaGENERICPtr)vgaHWSave((vgaHWPtr)save, sizeof(vgaGENERICRec));

  return ((void *) save);
}



/*
 * GENERICInit --
 *      Handle the initialization of the VGAs registers
 */

static Bool
GENERICInit(mode)
     DisplayModePtr mode;
{
  return(vgaHWInit(mode,sizeof(vgaGENERICRec)));
}



/*
 * GENERICAdjust --
 *      adjust the current video frame to display the mousecursor
 */

static void 
GENERICAdjust(x, y)
     int x, y;
{
  int Base = (y * vga2InfoRec.displayWidth + x + 3) >> 3;

  outw(vgaIOBase + 4, (Base & 0x00FF00) | 0x0C);
  outw(vgaIOBase + 4, ((Base & 0x00FF) << 8) | 0x0D);
  outw(vgaIOBase + 4, ((Base & 0x030000) >> 8) | 0x33);
}

/*
 * GENERICValidMode --
 *
 */
static Bool
GENERICValidMode(mode)
DisplayModePtr mode;
{
return TRUE;
}

