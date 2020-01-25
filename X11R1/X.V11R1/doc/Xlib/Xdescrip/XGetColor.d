.\" $Header: XGetColor.d,v 1.3 87/08/31 09:10:06 mento Exp $
The
.PN XAllocNamedColor
function determines the correct color (shade) for the given screen.
.PN XAllocNamedColor
returns zero (0) when it encounters an error or a nonzero when it succeeds.
Both the exact data base definition and
the closest color supported by the hardware are returned.
