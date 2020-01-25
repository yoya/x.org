.\" $Header: XFillRect.d,v 1.2 87/09/01 18:43:07 mento Exp $
The
.PN XFillRectangle
and
.PN XFillRectangles
functions fill the specified rectangle or rectangles
as if a four-point 
.PN XFillPolygon
were specified for each rectangle:
.IP
[x,y] [x+width,y] [x+width,y+height] [x,y+height]
