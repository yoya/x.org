.\" $Header: XAllocPlanes.f,v 1.1 87/09/28 13:22:49 mento Exp $
Status XAllocColorPlanes\^(\^\fIdisplay\fP, \fIcmap\fP\^, \fIcontig\fP\^, \fIpixels_return\fP\^, \fIncolors\fP\^, \fInreds\fP\^, \fIngreens\fP\^, 
.br
                           \fInblues\fP\^, \fIrmask_return\fP\^, \fIgmask_return\fP\^, \fIbmask_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Colormap \fIcmap\fP\^;
.br
      Bool \fIcontig\fP\^;
.br
      unsigned long \fIpixels_return\fP[\^]\^;
.br
      int \fIncolors\fP\^;
.br
      int \fInreds\fP\^, \fIngreens\fP\^, \fInblues\fP\^;
.br
      unsigned long *\fIrmask_return\fP\^, *\fIgmask_return\fP\^, *\fIbmask_return\fP\^;
