/* $XConsortium: Mach64.c,v 1.1 95/01/26 15:26:42 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/common/Mach64.c,v 3.0 1994/11/27 20:42:39 dawes Exp $ */
#include "X.h"
#include "os.h"

#define _NO_XF86_PROTOTYPES

#include "xf86.h"
#include "xf86_Config.h"

extern ScrnInfoRec mach64InfoRec;

/*
 * This limit is currently set to 80MHz because this is the limit of many
 * ramdacs when running in 1:1 mode.  It will be increased when support
 * is added for using the ramdacs in 2:1 mode.  Increasing this limit
 * could result in damage to your hardware.
 */
#define MAX_MACH64_CLOCK		80000
#define MAX_MACH64_ATI68860_CLOCK	135000

int mach64MaxClock = MAX_MACH64_CLOCK;
int mach64MaxATI68860Clock = MAX_MACH64_ATI68860_CLOCK;

ScrnInfoPtr xf86Screens[] = 
{
  &mach64InfoRec,
};

int  xf86MaxScreens = sizeof(xf86Screens) / sizeof(ScrnInfoPtr);

int xf86ScreenNames[] =
{
  ACCEL,
  -1
};

int mach64ValidTokens[] =
{
  STATICGRAY,
  GRAYSCALE,
  STATICCOLOR,
  PSEUDOCOLOR,
  TRUECOLOR,
  DIRECTCOLOR,
  CHIPSET,
  CLOCKS,
  MODES,
  OPTION,
  VIDEORAM,
  VIEWPORT,
  VIRTUAL,
  CLOCKPROG,
  BIOSBASE,
  MEMBASE,
  -1
};

#include "xf86ExtInit.h"
