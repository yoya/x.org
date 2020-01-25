void XGetResources(\^\fIresources\fP, \fIres_count\fP, \fIvalues\fP, \fIvalue_count\fP, \fIdisplay\fP,
                   \fIpredecessor\fP, \fImy_name\fP, \fImy_class\fP, \fIfull_name\fP, \fIfull_class\fP\^)
.br
      ResourceList \fIresources\fP\^;
.br
      int \fIres_count\fP\^;
.br
      ArgList \fIvalues\fP\^;
.br
      int \fIvalue_count\fP\^;
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIpredecessor\fP\^;
.br
      String \fImy_name;\fP\^
.br
      String \fImy_class;\fP\^
.br
      XNameList *\fIfull_name\fP\^;	/* RETURN */
.br
      XClassList *\fIfull_class\fP\^;	/* RETURN */
