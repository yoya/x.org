.\" $Header: XIfEvent.d,v 1.2 88/02/07 16:08:20 mento Exp $
The
.PN XIfEvent
function
checks the event queue for the specified
event.
If the events match as determined by your predicate procedure, 
.PN XIfEvent
removes the event from the queue and, when it returns, copies the structure
into the client-supplied
.PN XEvent
structure.
