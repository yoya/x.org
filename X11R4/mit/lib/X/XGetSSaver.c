#include "copyright.h"

/* $XConsortium: XGetSSaver.c,v 11.10 88/09/06 16:08:19 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

XGetScreenSaver(dpy, timeout, interval, prefer_blanking, allow_exp)
     register Display *dpy;
     /* the following are return only vars */
     int *timeout, *interval;
     int *prefer_blanking, *allow_exp;  /*boolean */
     
{       
    xGetScreenSaverReply rep;
    register xReq *req;
    LockDisplay(dpy);
    GetEmptyReq(GetScreenSaver, req);

    (void) _XReply (dpy, (xReply *)&rep, 0, xTrue);
    *timeout = rep.timeout;
    *interval = rep.interval;
    *prefer_blanking = rep.preferBlanking;
    *allow_exp = rep.allowExposures;
    UnlockDisplay(dpy);
    SyncHandle();
}

