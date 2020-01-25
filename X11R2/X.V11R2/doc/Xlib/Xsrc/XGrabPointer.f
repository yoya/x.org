.\" $Header: XGrabPointer.f,v 1.1 87/09/28 13:31:01 mento Exp $
int XGrabPointer\^(\^\fIdisplay\fP, \fIgrab_window\fP\^, \fIowner_events\fP\^, \fIevent_mask\fP\^, \fIpointer_mode\fP\^,
               \fIkeyboard_mode\fP\^, \fIconfine_to\fP\^, \fIcursor\fP\^, \fItime\fP\^)
.br
      Display *\fIdisplay\fP\^;
.br
      Window \fIgrab_window\fP\^;
.br
      Bool \fIowner_events\fP\^;
.br
      unsigned int \fIevent_mask\fP\^;	
.br
      int \fIpointer_mode\fP\^, \fIkeyboard_mode\fP\^; 
.br
      Window \fIconfine_to\fP\^; 
.br
      Cursor \fIcursor\fP\^; 
.br
      Time \fItime\fP\^; 
