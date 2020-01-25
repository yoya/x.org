.\" $Header: XGetSImage.f,v 1.2 88/02/24 20:17:09 mento Exp $
XImage *XGetSubImage\^(\^\fIdisplay\fP, \fId\fP\^, \fIx\fP\^, \fIy\fP\^, \fIwidth\fP\^, \fIheight\fP\^, \fIplane_mask\fP, \fIformat\fP\^, \fIdest_image\fP\^, \fIdest_x\fP\^, 
.br
                     \fIdest_y\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Drawable \fId\fP\^;
.br
      int \fIx\fP\^, \fIy\fP\^;
.br
      unsigned int \fIwidth\fP\^, \fIheight\fP\^;
.br
      unsigned long \fIplane_mask\fP\^;
.br
      int \fIformat\fP\^;
.br
      XImage *\fIdest_image\fP\^;
.br
      int \fIdest_x\fP\^, \fIdest_y\fP\^;
