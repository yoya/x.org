.na
.de Ds
.nf
.\\$1D \\$2 \\$1
.ft 1
.ps \\n(PS
.if \\n(VS>=40 .vs \\n(VSu
.if \\n(VS<=39 .vs \\n(VSp
..
.de De
.ce 0
.if \\n(BD .DF
.nr BD 0
.in \\n(OIu
.if \\n(TM .ls 2
.sp \\n(DDu
.fi
..
.de FD
.LP
.KS
.TA .5i 3i
.ta .5i 3i
.nf
..
.de FN
.fi
.KE
.LP
..
.de IN		\" send an index entry to the stderr
.tm \\n%:\\$1:\\$2:\\$3
..
.de C{
.KS
.nf
.D
.\"
.\"	choose appropriate monospace font
.\"	the imagen conditional, 480,
.\"	may be changed to L if LB is too
.\"	heavy for your eyes...
.\"
.ie "\\*(.T"480" .ft L
.el .ie "\\*(.T"300" .ft L
.el .ie "\\*(.T"202" .ft PO
.el .ie "\\*(.T"aps" .ft CW
.el .ft R
.ps \\n(PS
.ie \\n(VS>40 .vs \\n(VSu
.el .vs \\n(VSp
..
.de C}
.DE
.R
..
.de Pn
.IN \\$2
.ie t \\$1\f(H\^\\$2\^\fR\\$3
.el \\$1\fI\^\\$2\^\fP\\$3
..
.de PN
.IN \\$1
.ie t \f(H\^\\$1\^\fR\\$2
.el \fI\^\\$1\^\fP\\$2
..
.de NT
.ne 7
.ds NO Note
.if \\n(.$>$1 .if !'\\$2'C' .ds NO \\$2
.if \\n(.$ .if !'\\$1'C' .ds NO \\$1
.ie n .sp
.el .sp 10p
.TB
.ce
\\*(NO
.ie n .sp
.el .sp 5p
.if '\\$1'C' .ce 99
.if '\\$2'C' .ce 99
.in +5n
.ll -5n
.R
..
.		\" Note End -- doug kraft 3/85
.de NE
.ce 0
.in -5n
.ll +5n
.ie n .sp
.el .sp 10p
..
.ny0
