.\" $Header: XGetGeometry.f,v 1.1 87/09/28 13:30:01 mento Exp $
Status XGetGeometry\^(\^\fIdisplay\fP, \fId\fP\^, \^\fIroot_return\fP\^, \fIx_return\fP\^, \fIy_return\fP\^, \fIwidth_return\fP\^, 
.br
                      \fIheight_return\fP\^, \fIborder_width_return\fP\^, \fIdepth_return\fP\^)
.br
        Display *\fIdisplay\fP\^;
.br
        Drawable \fId\fP\^;
.br
        Window *\fIroot_return\fP\^;
.br
        int *\fIx_return\fP\^, *\fIy_return\fP\^;
.br
        unsigned int *\fIwidth_return\fP\^, *\fIheight_return\fP\^;
.br
        unsigned int *\fIborder_width_return\fP\^;
.br
        unsigned int *\fIdepth_return\fP\^;
