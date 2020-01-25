.na
.de FD
.LP
.KS
.TA 1.0i 3i
.ta 1.0i 3i
.nf
..
.de FN
.fi
.KE
.LP
..
.de IN		\" send an index entry to the stderr
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
.de PN
.ie t \f(H\|\\$1\|\fR\\$2
.el \fI\|\\$1\|\fP\\$2
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
