.\" $Header: XUnmapSubs.d,v 1.1 87/08/28 05:19:03 mento Exp $
The
.PN XUnmapSubwindows
function unmaps all subwindows for the specified window in bottom to top
stacking order.
It causes the X server to generate an
.PN UnmapNotify
event on each subwindow and exposure events on formerly obscured windows.
.IN "UnmapNotify Event"
.IN "Exposure Event"
