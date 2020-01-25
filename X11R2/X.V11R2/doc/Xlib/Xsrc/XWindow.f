.\" $Header: XWindow.f,v 1.1 87/09/28 13:45:21 mento Exp $
Window XCreateWindow\^(\^\fIdisplay\fP, \fIparent\fP\^, \fIx\fP\^, \fIy\fP\^, \fIwidth\fP\^, \fIheight\fP\^, \fIborder_width\fP\^, \fIdepth\fP\^, 
.br
                       \fIclass\fP\^, \fIvisual\fP\^, \fIvaluemask\fP\^, \fIattributes\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIparent\fP\^;
.br
      int \fIx\fP\^, \fIy\fP\^; 
.br
      unsigned int \fIwidth\fP\^, \fIheight\fP\^;
.br
      unsigned int \fIborder_width\fP\^;
.br
      int \fIdepth\fP\^;
.br
      unsigned int \fIclass\fP\^;
.br
      Visual *\fIvisual\fP\^
.br
      unsigned long \fIvaluemask\fP\^;
.br
      XSetWindowAttributes *\fIattributes\fP\^;
