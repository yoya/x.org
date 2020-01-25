.\" $Header: XOpenDisplay.d,v 1.3 87/09/10 11:48:51 mento Exp $
The
.PN XOpenDisplay
function returns a Display structure that serves as the
connection to the X server and that contains all the information
about that X server.
.PN XOpenDisplay
connects the specified hardware display to the server through TCP,
UNIX domain, or DECnet stream communications protocols.
.IN "Protocol" "TCP"
.IN "Protocol" "DECnet"
