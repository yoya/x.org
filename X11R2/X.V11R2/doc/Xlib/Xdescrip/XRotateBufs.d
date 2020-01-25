.\" $Header: XRotateBufs.d,v 1.1 87/08/27 14:23:55 mento Exp $
The
.PN XRotateBuffers
function rotates the cut
buffers, such that buffer 0 becomes buffer n, buffer 1 becomes n+1 mod 8, and so on.
This cut buffer numbering is global to the display.
