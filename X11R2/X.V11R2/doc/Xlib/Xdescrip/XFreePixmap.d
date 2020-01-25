.\" $Header: XFreePixmap.d,v 1.1 87/09/09 13:37:44 mento Exp $
The
.PN XFreePixmap
function first deletes the association between the pixmap ID and the pixmap.
Then, the X server frees the pixmap storage when no other resources reference 
it.
The pixmap should never be referenced again.
