The
.PN XCheckTypedEvent
function searches first the event queue, then any events available  
on the server connection, for the first event that matches the specified
type.
When it finds a match,
.PN XCheckTypedEvent
returns its associated event structure 
to the specified
.PN XEvent
structure and returns
.PN True .
