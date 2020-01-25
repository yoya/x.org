.\" $Header: XTranCoords.d,v 1.4 87/09/11 14:50:41 mento Exp $
The
.PN XTranslateCoordinates
function takes the src_x and src_y coordinates within the source window relative
to the source window's origin and returns these coordinates to 
dest_x_return and dest_y_return
relative to the destination window's origin.
If
.PN XTranslateCoordinates
returns zero (0), src_w and
dest_w are on different screens,
and dest_x_return and dest_y_return are zero (0).
If the coordinates are contained in a mapped child of dest_w,
that child is returned to the child argument.
