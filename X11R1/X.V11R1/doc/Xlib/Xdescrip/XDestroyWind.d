.\" $Header: XDestroyWind.d,v 1.2 87/08/28 04:23:27 mento Exp $
The
.PN XDestroyWindow
function destroys the specified window as well as all of its subwindows and causes
the X server to generate a
.PN DestroyNotify
event for each window.
