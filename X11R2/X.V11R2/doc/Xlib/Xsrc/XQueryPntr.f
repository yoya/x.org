.\" $Header: XQueryPntr.f,v 1.1 87/09/28 13:37:47 mento Exp $
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
