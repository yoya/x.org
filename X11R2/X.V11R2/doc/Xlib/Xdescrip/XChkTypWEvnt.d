The
.PN XCheckTypedWindowEvent
function searches first the event queue, then any events available  
on the server connection, for the first event that matches the specified
type and window.
When it finds a match,
.PN XCheckTypedWindowEvent
removes the event from the queue, copies it into the specified
.PN XEvent
structure and returns
.PN True .
