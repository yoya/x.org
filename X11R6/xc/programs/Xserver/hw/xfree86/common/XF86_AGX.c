/* $XFree86: xc/programs/Xserver/hw/xfree86/common/XF86_AGX.c,v 3.2 1994/09/07 15:51:39 dawes Exp $ */

#include "X.h"
#include "os.h"

#define _NO_XF86_PROTOTYPES

#include "xf86.h"
#include "xf86_Config.h"

extern ScrnInfoRec agxInfoRec;

/*
 * This limit is currently set to 85MHz because this is the limit of many
 * ramdacs when running in 1:1 mode.  It will be increased when support
 * is added for using the ramdacs in 2:1 mode.  Increasing this limit
 * could result in damage to your hardware.
 */
#define MAX_AGX_CLOCK		85000

int agxMaxClock = MAX_AGX_CLOCK;

ScrnInfoPtr xf86Screens[] = 
{
  &agxInfoRec,
};

int  xf86MaxScreens = sizeof(xf86Screens) / sizeof(ScrnInfoPtr);

int xf86ScreenNames[] =
{
  ACCEL,
  -1
};

int agxValidTokens[] =
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
  IOBASE,
  DACBASE,
  COPBASE,
  POSBASE,
  INSTANCE,
  -1
};

#include "xf86ExtInit.h"

