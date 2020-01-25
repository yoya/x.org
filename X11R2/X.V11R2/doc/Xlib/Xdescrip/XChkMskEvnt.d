.\" $Header: XChkMskEvnt.d,v 1.3 88/02/24 15:40:33 mento Exp $
The
.PN XCheckMaskEvent
function searches first the event queue, then any events available on the
server connection, for the first event that matches the specified mask.
When it finds a match,
.PN XCheckMaskEvent
removes that event, copies it into the specified
.PN XEvent
structure, and returns
.PN True .
