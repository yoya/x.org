.\" $Header: XQueryColor.d,v 1.4 88/02/24 10:22:33 mento Exp $
The
.PN XQueryColor
and
.PN XQueryColors
functions returns the red, green, and blue color values stored in the
specified color map for the pixel value you pass in the pixel member of the
.PN XColor
structure(s).
The values returned for an unallocated entry are undefined.
They also set the flags member in the
.PN XColor
structure to all three colors.
