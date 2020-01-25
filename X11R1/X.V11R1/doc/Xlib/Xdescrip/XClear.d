.\" $Header: XClear.d,v 1.3 87/09/09 15:07:05 mento Exp $
The
.PN XClearWindow
function clears the entire area in the specified window and is
equivalent to
.PN XClearArea
(display, w, 0, 0, 0, 0, 
.PN False ).
