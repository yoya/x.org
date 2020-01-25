.\" $Header: XMaskEvent.d,v 1.3 88/02/24 15:32:50 mento Exp $
The
.PN XMaskEvent
function searches the event queue for the events associated with the 
specified mask.
When it finds a match,
.PN XMaskEvent
removes that and copies it into the specified
.PN XEvent
structure.
