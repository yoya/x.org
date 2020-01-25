.\" $Header: XIfEvent.d,v 1.2 87/09/09 19:41:05 mento Exp $
The
.PN XIfEvent
function
flushes the output buffer, checks the event queue for the specified
event, and, if the events match as determined by your predicate procedure, 
removes the event from the queue.
