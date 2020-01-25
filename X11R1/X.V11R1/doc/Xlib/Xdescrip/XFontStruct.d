.\" $Header: XFontStruct.d,v 1.1 87/08/27 14:19:09 mento Exp $
The
.PN XLoadQueryFont
function provides the most common way for accessing a font.
That is,
.PN XLoadQueryFont
both opens (loads) the specified font and returns a pointer to the
appropriate
.PN XFontStruct
structure.
