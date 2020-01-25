.\" $Header: XDestroySubs.d,v 1.1 87/09/11 13:23:28 mento Exp $
The
.PN XDestroySubwindows
function destroys all inferior windows of the specified window, 
in bottom to top stacking order.
It causes the X server to generate a
.PN DestroyNotify
event for each window.
If any mapped
subwindows were actually destroyed,
.PN XDestroySubwindows
causes the X sever to generate exposure events on the specified window.
