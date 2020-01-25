#include "copyright.h"

/* $Header: XGetPntMap.c,v 1.6 87/10/29 19:03:10 newman Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/

#define NEED_REPLIES
#include "Xlibint.h"

#ifdef MIN		/* some systems define this in <sys/param.h> */
#undef MIN
#endif
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int XGetPointerMapping (dpy, map, nmaps)
    register Display *dpy;
    unsigned char map[];
    int nmaps;

{
    unsigned char mapping[256];	/* known fixed size */
    long nbytes;
    xGetPointerMappingReply rep;
    register xReq *req;

    LockDisplay(dpy);
    GetEmptyReq(GetPointerMapping, req);
    (void) _XReply(dpy, (xReply *)&rep, 0, xFalse);

    nbytes = (long)rep.length << 2;
    _XRead (dpy, (char *)mapping, nbytes);
    /* don't return more data than the user asked for. */
    if (rep.nElts) {
	    bcopy ((char *) mapping, (char *) map, 
		MIN((int)rep.nElts, nmaps) );
	}
    UnlockDisplay(dpy);
    SyncHandle();
    return ((int) rep.nElts);
}

KeySym *XGetKeyboardMapping (dpy, first_keycode, count, keysyms_per_keycode)
    register Display *dpy;
    KeyCode first_keycode;
    int count;
     int *keysyms_per_keycode;		/* RETURN */
{
    long nbytes;
    register KeySym *mapping = NULL;
    xGetKeyboardMappingReply rep;
    register xGetKeyboardMappingReq *req;

    LockDisplay(dpy);
    GetReq(GetKeyboardMapping, req);
    req->firstKeyCode = first_keycode;
    req->count = count;
    (void) _XReply(dpy, (xReply *)&rep, 0, xFalse);

    if (rep.length > 0) {
        *keysyms_per_keycode = rep.keySymsPerKeyCode;
	nbytes = (long)rep.length << 2;
	mapping = (KeySym *) Xmalloc((unsigned) nbytes);

	_XRead (dpy, (char *)mapping, nbytes);
      }
    UnlockDisplay(dpy);
    SyncHandle();
    return (mapping);
}

