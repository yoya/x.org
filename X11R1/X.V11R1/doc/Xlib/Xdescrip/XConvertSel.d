.\" $Header: XConvertSel.d,v 1.3 87/09/11 15:11:36 mento Exp $
.PN XConvertSelection
requests that the specified selection be converted to the specified target
type:
.IP \(bu 5
If the specified selection has an owner, the X server sends a
.PN SelectionRequest
event to that owner.
.IP \(bu 5
If no owner for the specified
selection exists, the X server generates a
.PN SelectionNotify
event to the
requestor with property
.PN None .
The arguments are passed on unchanged in either event.
