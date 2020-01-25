.\" $Header: XClear.d,v 1.1 87/09/09 12:07:17 mento Exp $
The
.PN XClearWindow
function clears the entire area in the specified window and is
equivalent to
.PN XClearArea
(display, w, 0, 0, 0, 0, 
.PN False ).
