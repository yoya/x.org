.\" $Header: XWindowEvent.d,v 1.3 88/02/24 15:20:42 mento Exp $
The
.PN XWindowEvent
function searches the event queue for an event that matches both the specified
window and event mask.
When it finds a match,
.PN XWindowEvent
removes that event from the queue and copies it into the specified
.PN XEvent
structure.
