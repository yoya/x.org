.\" $Header: XGrabServer.d,v 1.1 87/08/27 14:19:52 mento Exp $
The
.PN XGrabServer
function disables processing of requests and close-downs on all other connections
than the one this request arrived on.
It is recommended that you not grab the X server any more than is absolutely
necessary.
