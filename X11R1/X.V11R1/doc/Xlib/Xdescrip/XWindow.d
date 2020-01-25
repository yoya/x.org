.\" $Header: XWindow.d,v 1.1 87/08/27 14:22:19 mento Exp $
The
.PN XCreateWindow
function creates an unmapped subwindow for a specified parent window, returns the
window ID of the created window, and causes the X server to generate a
.PN CreateNotify
event.
The created window is placed on top in the stacking order with respect to siblings.
