.\" $Header: XGetProperty.f,v 1.2 88/02/10 07:57:03 mento Exp $
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
      unsigned long *\fIbytes_after_return\fP\^;
.br
      unsigned char **\fIprop_return\fP\^;
