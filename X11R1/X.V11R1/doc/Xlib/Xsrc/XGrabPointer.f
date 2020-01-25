.\" $Header: XGrabPointer.f,v 11.10 87/08/15 12:13:51 mento Exp $
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
