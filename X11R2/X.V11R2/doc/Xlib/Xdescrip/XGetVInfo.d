.\" $Header: XGetVInfo.d,v 1.1 87/09/03 14:13:58 mento Exp $
The
.PN XGetVisualInfo
function returns a list of visual structures that match the attributes specified
by the template argument.
If no visual structures match the template using the specified vinfo_mask,
.PN XGetVisualInfo
returns a NULL.
