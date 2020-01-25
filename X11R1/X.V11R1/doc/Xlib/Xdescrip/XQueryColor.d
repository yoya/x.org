.\" $Header: XQueryColor.d,v 1.4 87/09/01 14:42:42 mento Exp $
The
.PN XQueryColor
function returns the red, green, and blue color values stored in cmap for
the pixel value you passed to the pixel member of the
.PN XColor
structure.
The values returned for an unallocated entry are undefined.
The
.PN XQueryColors
function does the same thing, except that it operates on an array of
.PN XColor
structures.
They also set the flags member in the
.PN XColor
structure to all three colors.
