.\" $Header: XFontStruct.d,v 1.1 87/08/27 14:23:27 mento Exp $
The
.PN XLoadQueryFont
function provides the most common way for accessing a font.
That is,
.PN XLoadQueryFont
both opens (loads) the specified font and returns a pointer to the
appropriate
.PN XFontStruct
structure.
