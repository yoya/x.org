.\" $Header: XGetHardClr.d,v 1.4 88/02/24 09:00:46 mento Exp $
The
.PN XAllocColor
function allocates a read-only color map entry corresponding to the closest
red, green, and blue values supported by the hardware.
.PN XAllocColor
returns the pixel value of the color closest to the specified
RGB elements supported by the hardware
and returns the red, green, and blue values actually used.
