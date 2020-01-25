/**************************************************************** 
Copyright 1988-1992 by Apple Computer, Inc, Cupertino, California
			All Rights Reserved

Permission to use, copy, modify, and distribute this software
for any purpose and without fee is hereby granted, provided
that the above copyright notice appear in all copies.

APPLE MAKES NO WARRANTY OR REPRESENTATION, EITHER EXPRESS,
OR IMPLIED, WITH RESPECT TO THIS SOFTWARE, ITS QUALITY,
PERFORMANCE, MERCHANABILITY, OR FITNESS FOR A PARTICULAR
PURPOSE. AS A RESULT, THIS SOFTWARE IS PROVIDED "AS IS,"
AND YOU THE USER ARE ASSUMING THE ENTIRE RISK AS TO ITS
QUALITY AND PERFORMANCE. IN NO EVENT WILL APPLE BE LIABLE 
FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
DAMAGES RESULTING FROM ANY DEFECT IN THE SOFTWARE.

THE WARRANTY AND REMEDIES SET FORTH ABOVE ARE EXCLUSIVE
AND IN LIEU OF ALL OTHERS, ORAL OR WRITTEN, EXPRESS OR
IMPLIED.

****************************************************************/

/*
 * Under A/UX >=2.0 struct video has been "Macintized" and now incorporates
 * a structure called AuxDCE which is defined once and for all in
 * /usr/include/mac. Alas the definition for AuxDCE requires wheeling in
 * lots of Mac stuff including QuickDraw. This is a headache as there are
 * a variety of clashes between X and QuickDraw (they both do windows after
 * all). So we define just what we need here and avoid pulling in the Mac
 * includes. Of course if this ever changes ...
 */

#include <mac/osutils.h>
#include <mac/types.h>

#ifndef __DEVICES__
#ifndef __mac_devices_h
#include <mac/files.h>

struct DCtlEntry {
	char **dCtlDriver;
	short dCtlFlags;
	struct QHdr dCtlQHdr;
	long dCtlPosition;
	char **dCtlStorage;
	short dCtlRefNum;
	long dCtlCurTicks;
	char *dCtlWindow;
	short dCtlDelay;
	short dCtlEMask;
	short dCtlMenu;
	char dCtlSlot;
	char dCtlSlotId;
	long dCtlDevBase;
	long reserved;
	char dCtlExtDev;
	char fillByte;
};

struct AuxDCE {
	char **dCtlDriver;
	short dCtlFlags;
	struct QHdr dCtlQHdr;
	long dCtlPosition;
	char **dCtlStorage;
	short dCtlRefNum;
	long dCtlCurTicks;
	char *dCtlWindow;
	short dCtlDelay;
	short dCtlEMask;
	short dCtlMenu;
	char dCtlSlot;
	char dCtlSlotId;
	long dCtlDevBase;
	long reserved;
	char dCtlExtDev;
	char fillByte;
};

typedef struct AuxDCE AuxDCE;
typedef AuxDCE *AuxDCEPtr;

#define __mac_devices_h
#endif  __mac_devices_h
#define __DEVICES__
#endif

#ifndef __QUICKDRAW__
#ifndef __mac_quickdraw_h
struct RGBColor {
	unsigned short red;
	unsigned short green;
	unsigned short blue;
};

typedef struct RGBColor RGBColor;

struct ColorSpec {
	short value;
	RGBColor color;
};

typedef struct ColorSpec ColorSpec;
#define __mac_quickdraw_h
#endif
#define __QUICKDRAW__
#endif

#ifndef __TYPES__
#ifndef __mac_types_h
struct Rect {
	short top;
	short left;
	short bottom;
	short right;
};

typedef struct Rect Rect;
typedef char *Ptr;
typedef char Boolean;
#define __mac_types_h
#endif
#define __TYPES__
#endif

#ifndef SUPPORT_2_0
#include <mac/video.h>
#else
#define VDSetEntryRecord VDEntryRecord
#endif /* SUPPORT_2_0 */

#include <sys/stropts.h>
#include <sys/termio.h>
#include <sys/video.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <sys/signal.h>
#include <sys/ttychars.h>

static struct termio d_tio = {
	(BRKINT|IGNPAR|ICRNL|IXON)&(~ISTRIP)&(~IGNBRK)&(~PARMRK)&(~INPCK)&(~INLCR)&
	(~IGNCR)&(~IUCLC)&(~IXANY)&(~IXOFF),
	(OPOST|ONLCR)&(~OLCUC)&(~OCRNL)&(~ONOCR)&(~ONLRET)&(~OFILL)&(~OFDEL),
	(B9600|CS8|CREAD)&(~CSTOPB)&(~PARENB)&(~PARODD)&(~HUPCL)&(~CLOCAL)&(~LOBLK),
	(ISIG|ICANON|ECHO|ECHOE|ECHOK)&(~XCASE)&(~ECHONL)&(~NOFLSH),
	0,
	{CINTR, CQUIT, CERASE, CKILL, CEOF, CNUL, CNUL, CNUL}
};

static struct ltchars tc = { CSUSP, CDSUSP, CNUL, CNUL, CNUL, CNUL };

/*
 * Check to see that the server restored the "line" streams module
 * on /dev/console. If so, we'll presume all is well. If not, clear 
 * all stacked modules, push "line", and establish workable stty values.
 */
main()
{
	int fd; int line;
	int iarg;
	struct strioctl ctl;
	char buff[FMNAMESZ+1];
	int errors = 0;

	if ((fd = open("/dev/console", O_RDWR)) < 0) {
	    printf("Xrepair: can't open /dev/console\n");
	} else if (ioctl(fd, I_FIND, "line") == 0) {
            ctl.ic_len = 0;
            ctl.ic_cmd = CONS_UNDIRECT;
            if (ioctl(fd, I_STR, &ctl) < 0) {
                errors++;
                printf("Failed to ioctl I_STR CONS_UNDIRECT.\r\n");
            }
	    iarg = 0;
	    if (ioctl(fd, FIONBIO, &iarg) < 0) {
		errors++;
		printf("Could not ioctl FIONBIO. \r\n");
	    }
	    
	    iarg = 0;
	    if (ioctl(fd, FIOASYNC, &iarg) < 0) {
		errors++;
		printf("Could not ioctl FIOASYNC. \r\n");
	    }
	    
	    if (ioctl(fd, I_FLUSH, FLUSHRW) < 0) {
		errors++;
		printf("Failed to ioctl I_FLUSH FLUSHRW.\r\n");
	    }
	    
	    ctl.ic_len = 0;
	    ctl.ic_cmd = VIDEO_NOMOUSE;
	    if (ioctl(fd, I_STR, &ctl) < 0) {
		errors++;
		printf("Failed to ioctl I_STR VIDEO_NOMOUSE.\r\n");
	    }
	    
	    ctl.ic_len = 0;
	    ctl.ic_cmd = VIDEO_MAC;
	    if (ioctl(fd, I_STR, &ctl) < 0) {
	        ctl.ic_len = 0;
	        ctl.ic_cmd = VIDEO_ASCII; /* A/UX 1.* */
	        if (ioctl(fd, I_STR, &ctl) < 0) {
		    errors++;
		    printf("Failed to ioctl I_STR VIDEO_MAC VIDEO_ASCII.\r\n");
	        }
	    }

	    if(ioctl(fd, I_PUSH, "line") < 0) {
		errors++;
		printf("Failed to ioctl I_PUSH.\r\n");
	    }

	    (void) signal(SIGTTOU, SIG_IGN);
	    if (ioctl(fd, TCSETA, &d_tio) < 0) {
		errors++;
		printf("Failed to ioctl TCSETA.\r\n");
	    }

	    if (ioctl(fd, TIOCSLTC, &tc) < 0) {
		errors++;
		printf("Failed to ioctl TIOCSLTC.\r\n");
	    }

	    (void) signal(SIGTTOU, SIG_DFL);
	} else {
	    ctl.ic_len = 0;
	    ctl.ic_cmd = VIDEO_MAC; /* For A/UX 2.0 and later */
	    if (ioctl(fd, I_STR, &ctl) < 0) {
		errors++;
		printf("Failed to ioctl I_STR VIDEO_MAC.\r\n");
	    }
	}
    exit (errors);
}
