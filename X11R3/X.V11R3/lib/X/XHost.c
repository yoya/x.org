#include "copyright.h"

/* $XConsortium: XHost.c,v 11.10 88/09/06 16:07:54 jim Exp $ */
/* Copyright    Massachusetts Institute of Technology    1986	*/
/* this might be rightly reguarded an os dependent file */

#include "Xlibint.h"

XAddHost (dpy, host)
    register Display *dpy;
    XHostAddress *host;
    {
    register xChangeHostsReq *req;
    register int length = (host->length + 3) & ~0x3;	/* round up */

    LockDisplay(dpy);
    GetReqExtra (ChangeHosts, length, req);
    req->mode = HostInsert;
    req->hostFamily = host->family;
    req->hostLength = host->length;
    bcopy (host->address, (char *) NEXTPTR(req,xChangeHostsReq), host->length);
    UnlockDisplay(dpy);
    SyncHandle();
    }

XRemoveHost (dpy, host)
    register Display *dpy;
    XHostAddress *host;
    {
    register xChangeHostsReq *req;
    register int length = (host->length + 3) & ~0x3;	/* round up */

    LockDisplay(dpy);
    GetReqExtra (ChangeHosts, length, req);
    req->mode = HostDelete;
    req->hostFamily = host->family;
    req->hostLength = host->length;
    bcopy (host->address, (char *) NEXTPTR(req,xChangeHostsReq), host->length);
    UnlockDisplay(dpy);
    SyncHandle();
    }


XAddHosts (dpy, hosts, n)
    register Display *dpy;
    XHostAddress *hosts;
    int n;
{
    register int i;
    for (i = 0; i < n; i++) {
	XAddHost(dpy, &hosts[i]);
      }
}

XRemoveHosts (dpy, hosts, n)
    register Display *dpy;
    XHostAddress *hosts;
    int n;
{
    register int i;
    for (i = 0; i < n; i++) {
	XRemoveHost(dpy, &hosts[i]);
      }
}
