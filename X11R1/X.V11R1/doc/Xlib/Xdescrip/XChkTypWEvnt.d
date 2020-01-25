The
.PN XCheckTypedWindowEvent
function flushes the output buffer and searches the event queue for  
the events associated with the specified window.
.PN XCheckTypedWindowEvent
returns the next event that matches the window and the specified event type
to the specified
.PN XEvent
structure.
