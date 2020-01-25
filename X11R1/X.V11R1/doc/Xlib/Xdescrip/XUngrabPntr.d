.\" $Header: XUngrabPntr.d,v 1.2 87/09/07 16:13:20 jg Exp $
The
.PN XUngrabPointer
function releases the pointer and any queued events,
if this client has actively grabbed the pointer from
.PN XGrabPointer ,
.PN XGrabButton ,
or from a normal button press.
