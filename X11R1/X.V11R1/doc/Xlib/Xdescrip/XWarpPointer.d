.\" $Header: XWarpPointer.d,v 1.2 87/09/10 17:46:32 mento Exp $
The
.PN XWarpPointer
function moves the pointer to the coordinates specified by the dest_x and dest_y
arguments, relative to the destination window's origin.
If the destination window is 
.PN None ,
the pointer is moved by offsets specified by the dest_x and dest_y 
coordinates.
