.\" $Header: XGetWMHnts.d,v 1.1 87/08/27 13:20:43 mento Exp $
The
.PN XGetWMHints
function reads the value of the window manager hints atom and 
returns either NULL if it fails (for example, if no WM_HINTS property was
set on window w) or a pointer to a XWMHints structure if it succeeds.
