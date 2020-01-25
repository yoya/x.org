.\" $Header: XCrePmap.d,v 1.2 87/08/28 08:36:46 mento Exp $
The
.PN XCreatePixmap
function creates a pixmap of the width, height, and depth you specified.
It also assigns the pixmap ID to it.
It is valid to pass a window whose class
is
.PN InputOnly
to the drawable argument.
