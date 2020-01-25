.\" $Header: XDeleteProp.d,v 1.3 87/09/04 16:30:00 mento Exp $
The
.PN XDeleteProperty
function deletes the specified property only if the
property was defined on the specified window.
.PN XDeleteProperty
causes the X server to generate a
.PN PropertyNotify
event on the window,
unless the property does not exist.
