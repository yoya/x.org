.\" $Header: XFontFree.d,v 1.1 87/08/27 14:19:07 mento Exp $
The
.PN XFreeFont
function deletes the association between the font resource ID and the specified font.
The font itself will be freed when no other resource references it.
The data and the font should not be referenced again.
