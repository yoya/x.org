.\" $Header: XFreePixmap.d,v 1.4 87/09/09 16:37:31 mento Exp $
The
.PN XFreePixmap
function first deletes the association between the pixmap ID and the pixmap.
Then, the X server frees the pixmap storage when no other resources reference 
it.
The pixmap should never be referenced again.
