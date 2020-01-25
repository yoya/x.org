/*****************************************************************************
Copyright 1992 by Apple Computer, Inc, Cupertino, California
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

*****************************************************************************/

#include "macII.h"

#define CountScreens(res) *((short *)res)

static Resource screenRes = NULL;
static ResHandle systemFileHandle = NULL;
static screenT theScreen;

ResHandle
OpenSystemFile()
{
    if (systemFileHandle == NULL) {
	systemFileHandle = mropen("/mac/sys/System Folder/System", 0);
    if (systemFileHandle == NULL) {
	ErrorF("Can't open /mac/sys/System Folder/System for gamma correction\n");
	return(-1);
    }
    }
    return systemFileHandle;
}

int
GetGammaTableSize(gammaTable)
	Resource gammaTable;
{
    ResHandle fileHandle;
    short id;
    long type;
    ResReference resR;
    int size;
	
    fileHandle = OpenSystemFile();
    if (fileHandle < 0) return(-1);
    size = mrinfo(fileHandle, gammaTable, &id, &type, &resR);
    return size;
}

Resource
GetGamma(rsrcID)
{
    ResHandle fileHandle;
    Resource gammaResource;

    fileHandle = OpenSystemFile();
    if (fileHandle < 0) return(NULL);
    gammaResource = mrget(fileHandle, 'gama', rsrcID);
    if (gammaResource == NULL)
	ErrorF("Can't find 'gama' resource\n");
    return gammaResource;
}

SetGamma(slot, gammaTable)
    Resource gammaTable;
{
    struct VDGammaRecord gammaRecord;
    struct CntrlParam pb;
    struct strioctl ctl;
    int fd;
    extern int errno;
    int len = GetGammaTableSize(gammaTable);

    if (len < 0) return;

    fd = open("/dev/console", O_RDWR, 0);
    if (fd < 0) {
	ErrorF("Can't open /dev/console to set gamma\n");
	return;
    }

    ctl.ic_cmd = VIDEO_CONTROL;
    ctl.ic_timout = -1;
    ctl.ic_len = sizeof(pb);
    ctl.ic_dp = (char *) &pb;

    gammaRecord.csGTable = (Ptr) gammaTable;

    pb.qType = slot;
    pb.ioTrap = noQueueBit;
    pb.ioCmdAddr = (char *) -1;
    pb.csCode = cscSetGamma;
    *(char **) pb.csParam = (char *) &gammaRecord;
    
    if (ioctl(fd, I_STR, &ctl) == -1 || pb.qType != 0)
	ErrorF("SetGamma ioctl failed - qtype %d slot %d, errno %d\n",
	       pb.qType, slot, errno);

    close(fd);
}

Resource
LoadScreenResource()
{
    ResHandle fileHandle;
    Resource  screenResource;

    fileHandle = OpenSystemFile();
    if (fileHandle < 0) return(NULL);
    screenResource = mrget(fileHandle, 'scrn', 0);
    if (screenResource == NULL)
	ErrorF("Can't find 'scrn' resource\n");
    return screenResource;
}

screenT *
GetScreen(res, idx)
    char *res;
{
    screenT *scr;
    int rec;
    int ctl;

    if (idx >= CountScreens(res))
	scr = NULL;
    else {
	scr = &theScreen;
	res += sizeof(short); /* skip the count */
	for (rec = 0; rec <= idx; rec++) {
	    scr->sSlotHdwrId = *((short *)res)++;
	    scr->sSlot = *((short *)res)++;
	    scr->sDceBase = *((long *)res)++;
	    scr->sMode = *((short *)res)++;
	    scr->sMask = *((short *)res)++;
	    scr->sFlags = *((short *)res)++;
	    scr->sClut = *((short *)res)++;
	    scr->sGamma = *((short *)res)++;
	    scr->sRect[0] = *((short *)res)++;
	    scr->sRect[1] = *((short *)res)++;
	    scr->sRect[2] = *((short *)res)++;
	    scr->sRect[3] = *((short *)res)++;
	    scr->sCtlCalls = *((short *)res)++;

	    for (ctl = 0; ctl < scr->sCtlCalls; ctl++) {
		int cscode;
		int length;

		cscode = *((short *)res)++;
		length = *((short *)res)++;
		if (length)
		    res += length;
	    }
	}
    }
    return scr;
}

void
macIISetGamma()
{
    int nscreens;
    int screenix;
    Resource gammaTable;
    screenT *screen;

    if (screenRes == NULL) {
	screenRes = LoadScreenResource();
	if (screenRes == NULL)
	    return;
    }
    for (screenix = 0; screenix < CountScreens(screenRes); screenix++) {
	screen = GetScreen(screenRes, screenix);
	if (screen == NULL)
	    continue;
	if (screen->sGamma != -1) {
	    gammaTable = GetGamma(screen->sGamma);
	    if (gammaTable == NULL)
		break;
	    SetGamma(screen->sSlot, gammaTable);
	}
    }
}
