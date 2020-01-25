.\" $Header: XChkWinEvnt.d,v 1.4 88/02/25 05:55:36 mento Exp $
The
.PN XCheckWindowEvent
function searches the event queue, then the events available 
on the server connection, for the first event that matches the specified window
and event mask.
When it finds a match,
.PN XCheckWindowEvent
removes that event, copies it into the specified
.PN XEvent
structure, and returns
.PN True .
