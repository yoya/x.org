.\" $Header: XRectangle.d,v 1.1 87/08/27 14:23:53 mento Exp $
The
.PN XDrawRectangle
and
.PN XDrawRectangles
functions draw the outlines of the specified rectangle or rectangles as
if a five-point PolyLine were specified for each rectangle:
.IP
[x,y,] [x+width,y] [x+width,y+height] [x,y+height] [x,y]
