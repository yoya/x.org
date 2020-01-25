.\" $Header: XQueryPntr.f,v 11.10 87/08/15 12:15:13 mento Exp $
Bool XQueryPointer\^(\^\fIdisplay\fP, \fIw\fP\^, \fIroot_return\fP\^, \fIchild_return\fP\^, \fIroot_x_return\fP\^, \fIroot_y_return\fP\^, 
.br
                     \fIwin_x_return\fP\^, \fIwin_y_return\fP\^, \fImask_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      Window *\fIroot_return\fP\^, *\fIchild_return\fP\^;
.br
      int *\fIroot_x_return\fP\^, *\fIroot_y_return\fP\^;
.br
      int *\fIwin_x_return\fP\^, *\fIwin_y_return\fP\^;
.br
      unsigned int *\fImask_return\fP\^;
