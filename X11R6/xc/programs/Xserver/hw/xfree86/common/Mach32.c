/* $XConsortium: Mach32.c,v 1.1 95/01/26 15:26:42 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/common/Mach32.c,v 3.3 1994/09/17 13:46:26 dawes Exp $ */
#include "X.h"
#include "os.h"

#define _NO_XF86_PROTOTYPES

#include "xf86.h"
#include "xf86_Config.h"

extern ScrnInfoRec mach32InfoRec;

/*
 * This limit is currently set to 80MHz because this is the limit of many
 * ramdacs when running in 1:1 mode.  It will be increased when support
 * is added for using the ramdacs in 2:1 mode.  Increasing this limit
 * could result in damage to your hardware.
 */
#define MAX_MACH32_CLOCK		80000
#define MAX_MACH32_TLC34075_CLOCK	135000
#define MAX_MACH32_16BPP_CLOCK		67500

int mach32MaxClock = MAX_MACH32_CLOCK;
int mach32MaxTlc34075Clock = MAX_MACH32_TLC34075_CLOCK;
int mach32Max16bppClock = MAX_MACH32_16BPP_CLOCK;

ScrnInfoPtr xf86Screens[] = 
{
  &mach32InfoRec,
};

int  xf86MaxScreens = sizeof(xf86Screens) / sizeof(ScrnInfoPtr);

int xf86ScreenNames[] =
{
  ACCEL,
  -1
};

int mach32ValidTokens[] =
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

