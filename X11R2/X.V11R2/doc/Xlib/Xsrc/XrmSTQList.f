#define XrmStringToNameList(str, name)  XrmStringToQuarkList((str), (name))
#define XrmStringToClassList(str,class) XrmStringToQuarkList((str), (class))
.sp
void XrmStringToQuarkList\^(\^\fIstring\fP, \fIquarks_return\fP\^)
.br
     char *\fIstring\fP\^;
.br
     XrmQuarkList \fIquarks_return\fP\^;
