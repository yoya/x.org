.\" $Header: XWarpPointer.d,v 1.1 87/09/10 14:48:56 mento Exp $
The
.PN XWarpPointer
function moves the pointer to the coordinates specified by the dest_x and dest_y
arguments, relative to the destination window's origin.
If the destination window is 
.PN None ,
the pointer is moved by offsets specified by the dest_x and dest_y 
coordinates.
