.\" $Header: XWindow.f,v 11.8 87/08/15 12:17:43 mento Exp $
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
