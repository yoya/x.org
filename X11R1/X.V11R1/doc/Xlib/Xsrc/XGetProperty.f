.\" $Header: XGetProperty.f,v 11.23 87/08/15 12:13:38 mento Exp $
int XGetWindowProperty\^(\^\fIdisplay\fP, \fIw\fP\^, \fIproperty\fP\^, \fIlong_offset\fP\^, \fIlong_length\fP\^, \fIdelete\fP\^, \fIreq_type\fP\^, 
.br
                        \fIactual_type_return\fP\^, \fIactual_format_return\fP\^, \fInitems_return\fP\^, \fIbytes_after_return\fP\^, 
.br
                        \fIprop_return\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIw\fP\^;
.br
      Atom \fIproperty\fP\^;
.br
      long \fIlong_offset\fP\^, \fIlong_length\fP\^;
.br
      Bool \fIdelete\fP\^;
.br
      Atom \fIreq_type\fP\^; 
.br
      Atom *\fIactual_type_return\fP\^;
.br
      int *\fIactual_format_return\fP\^;
.br
      unsigned long *\fInitems_return\fP\^;
.br
      long *\fIbytes_after_return\fP\^;
.br
      unsigned char **\fIprop_return\fP\^;
