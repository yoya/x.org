.\" $Header: XTranCoords.f,v 11.11 87/08/15 12:17:12 mento Exp $
int XTranslateCoordinates\^(\^\fIdisplay\fP, \fIsrc_w\fP\^, \fIdest_w\fP\^, \fIsrc_x\fP\^, \fIsrc_y\fP\^, \fIdest_x_return\fP\^, 
.br
                            \fIdest_y_return\fP\^, \fIchild_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIsrc_w\fP\^, \fIdest_w\fP\^;
.br
      int \fIsrc_x\fP\^, \fIsrc_y\fP\^;
.br
      int *\fIdest_x_return\fP\^, *\fIdest_y_return\fP\^;
.br
      Window *\fIchild_return\fP\^;
