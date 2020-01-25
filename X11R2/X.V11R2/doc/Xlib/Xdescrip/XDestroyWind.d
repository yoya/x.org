.\" $Header: XDestroyWind.d,v 1.1 87/08/28 04:23:34 mento Exp $
The
.PN XDestroyWindow
function destroys the specified window as well as all of its subwindows and causes
the X server to generate a
.PN DestroyNotify
event for each window.
