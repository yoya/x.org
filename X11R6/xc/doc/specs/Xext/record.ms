.\" $XConsortium: record.ms,v 1.4 94/04/17 20:06:11 rws Exp $
.\" Use tbl, -ms and macros.t
.\" -----------------------------------------------
.de Ip
.IP \(bu 3
..
.de Is
.IP \(sq 3
..
.de Cs
.IP
.nf
.ft CW
..
.de Ce
.ft P
.fi
..
.de Bu
.br
.ti +.5i
.ie \\n(.$ \\$1
.el \\(bu
..
.\"     These macros should select a typewriter font if you have one.
.de LS
.KS
.DS
.ps -2
.vs -2
.ft CW
.ta .25i .5i .75i 1.0i 1.25i 1.5i 1.75i 2.0i 2.25i 2.5i 2.75i 3.0i 
..
.de LE
.ft R
.ps +2
.ps +2
.DE
.KE 
..
.\"
.EH ''''
.OH ''''
.EF ''''
.OF ''''
.fi 
.ps 11
.nr PS 11
\&
.sp 8
.ce 50
\s+3\fBExtending X for Recording\fP\s-3
.sp
\fBVersion 1.3\fP
.sp
\fBDRAFT - not yet an X Consortium standard\fR
.sp 6
Martha Zimet
Network Computing Devices, Inc.
.ce 0
.bp
.br
\&
.sp 15
.ps 9
.nr PS 9
.fi 
.LP
Copyright \(co 1994 Network Computing Devices, Inc.
.LP
Permission to use, copy, modify, distribute, and sell this
documentation for any purpose is hereby granted without fee,
provided that the above copyright notice and this permission
notice appear in all copies.  Network Computing Devices, Inc.
makes no representations about the suitability for any purpose
of the information in this document.  This documentation is
provided ``as is'' without express or implied warranty.
.ps 11
.nr PS 11
.bp 1
.EH '\fBExtending X for Recording - Version 1.3\fP''\fBDRAFT\fP'
.OH '\fBExtending X for Recording\- Version 1.3\fP''\fBDRAFT\fP'
.OF ''\fB % \fP''
.EF ''\fB % \fP''
.hy 14
.nr LL 6.5i
.ll 6.5i 
.NH 1
Introduction
.XS
\*(SN Introduction
.XE
.LP
Several proposals have been written over the past few years that address some 
of the issues surrounding the recording and playback of user actions 
in the X Window System\s-4\u1\d\s0:
.Ip
\fISome Proposals for a Minimal X11 testing Extension\fP, 
Kieron Drake, UniSoft Ltd., April 1991
.Ip
\fIX11 Input Synthesis Extension Proposal\fP, Larry Woestman,
Hewlett Packard, November 1991
.Ip
\fIXTrap Architecture\fP, Dick Annicchiario, et al, Digital Equipment Corporation, 
July  1991
.Ip
\fIXTest Extension Recording Specification\fP, Yochanan Slonim, 
Mercury Interactive, December 1992
.FS
1. \fIX Window System\fP is a trademark of X Consortium, Inc.
.FE
.LP
This document both unifies and extends the previous diverse approaches to 
generate a proposal for an X extension that provides support for the recording 
of all core X protocol and arbitrary extension protocol. 
.LP
Input synthesis, or playback, has already been implemented in the XTest 
extension, an X Consortium standard, therefore, this extension 
is limited to recording.  This proposal addresses the issue of synchronization, 
since a recording solution would be incomplete without it.  The requirement to 
provide  synchronization of contextual information comes from the playback 
mechanism.  That is, there needs to be a way to interleave device or user 
events with their consequences.  Hence, contextual information must be 
captured during recording.  
.LP
Since it is impossible to predict in advance what synchronization information 
will be required by a particular application, the proposed extension takes 
a ``\fIbroad brush\fP'' approach, whereby facilities will exist to both record 
and filter core X protocol and arbitrary extension protocol.  As such, the 
proposal does not enforce a specific synchronization methodology (eg. window 
mapping/unmappping, cursor changes, text strings, etc.). 
.LP
The name of the extension, as seen by ListExtensions and QueryExtension, is ``RECORD.''  
.LP
Several people have provided input to this document:
.LP
.TS
box center tab(|);
l  l.

Robert Chesler | Absol-puter
Kieron Drake   | UniSoft Ltd. 
Marc Evans     | Synergytics
Jim Fulton     | NCD
Ken Miller     | DEC
Bob Scheifler  | X Consortium 

.TE
.bp 

.NH 2
Goals
.XS
\*(SN Goals
.XE 
.LP
.RS
.Ip
To implement an X Consortium standard for recording and synchronization,  
whereby both device events and contextual synchronization information in the 
form of device event consequences are recorded. 
.Ip 
To record contextual information used in synchronized playback 
without beforehand knowledge of the application which is being recorded.
.Ip
To provide the ability to work with an arbitrary set of X extension protocol.
.RE
.NH 2
Requirements
.XS
\*(SN Requirements
.XE 
.LP
The extension should:
.RS
.IP 1. 
not be dependent on other clients or extensions for its operation
.IP 2. 
not significantly impact performance
.IP 3. 
should support the recording of all device input (core 
devices and XInput devices)
.IP 4. 
should be extensible
.IP 5. 
should support the recording of contextual information for user events
.IP 6. 
follow an event-driven synchronization model
.IP 7. 
handle byte-order and provide word-size independence 
.IP 8. 
support multiple client connections
.RE 

.NH 1
Design
.XS
\*(SN Design 
.XE
.NH 2
Overview
.XS
\*(SN Overview
.XE
.LP
The mechanism used by this extension for recording is to intercept core X protocol and arbitrary X extension protocol entirely 
within the X server itself.  When the extension has been requested by one 
or more clients to intercept specific protocol, the protocol data is 
formatted and returned to the recording clients.  The extension provides 
a mechanism for capturing all events, including input device events 
that go to no clients, that is analogous to a client expressing  ``interest'' 
in all events, in all windows, including the root window.  Event filtering 
in the extension provides a mechanism for feeding device events to 
recording clients; it does not provide a mechanism for in-place, 
synchronous event substitution, modification, or withholding.  It is 
assumed that this functionality will be provided in another extension 
or in a later version of this extension.  
.LP
A 
.PN "Configuration"  
resource is added by the extension to the set of resources managed by 
the server.  Although any program with multiple paths open to the server 
is viewed as multiple clients by the X Window System protocol\s-4\u1\d\s0, 
the recommended communication model for a recording client is to open 
two connections to the server and use one for configuration control and 
the other for reading protocol data.   The ``control'' connection can 
execute requests to obtain information about the supported protocol version, create and destroy configurations, specify protocol types to be intercepted 
and clients to be recorded, query the current state of a 
configuration, and to stop interception and reporting of protocol data.   
The ``data'' connection can execute a request to start interception 
and reporting of specified protocol for a particular configuration.  
Intercepted protocol is sent back on the data connection, and may be 
sent by more than one reply packet.  Individual requests, replies, events 
or errors intercepted by the extension on behalf of recording clients 
may not be split across reply packets.   
.LP
The extension manages the intercepted protocol data through a record format 
that describes and extends the intercepted protocol data.  The data can be 
unpackaged by any client that follows the data formatting protocol of 
the extension.  This data format wraps the core X protocol.  
Aligned extension-specific information is at the front of the data. 
.FS
1. Scheifler, Robert W. `X Window System Protocol Version 11' 
.FE
.bp 

.NH 2 
Types
.XS
\*(SN Types
.XE
.sp 2 
.LP
The following new types are used in the request definitions in the 
following section.
.LP
.TS
tab(@);
l l.
CONFIGURATION:@XID     
.TE
.LP
The CONFIGURATION type defines a client-side handle for a server 
\fBConfiguration\fP.  


.LP
.TS
tab(@);
l l.
RANGE:@\s+2[\s0\fIfirst\fP, \fIlast\fP: CARD8\s+2]\s0
RANGE16:@\s+2[\s0\fIfirst\fP, \fIlast\fP: CARD16\s+2]\s0
EXTRANGE:@\s+2[\s0\fImajor\fP\^: CARD8, \fIminor\fP: LISTofRANGE16\s+2]\s0   
.TE
.LP
.TS
tab(@);
l l l.
RECORDFLAGS:@\s+2[\s0\fIcore-requests\fP:@LISTofRANGE
@\fIcore-replies\fP:@LISTofRANGE
@\fIext-requests\fP:@LISTofEXTRANGE
@\fIext-replies\fP:@LISTofEXTRANGE
@\fIevents\fP:@LISTofRANGE
@\fIunsent-events\fP:@LISTofRANGE
@\fIerrors\fP:@LISTofRANGE\s+2]\s0         
.TE
.LP
The  
.PN RECORDFLAGS 
structure contains the protocol values to be intercepted.  Typically, 
this structure is sent by recording clients over the control connection 
when creating or modifying a configuration.
.IP
.PN "core-requests"  
.br 
Specifies core X protocol requests.   
.IP 
.PN "core-replies"
.br 
Specifies core X protocol replies.  
.IP
.PN "ext-requests"  
.br 
Specifies arbitrary X extension requests.  
.IP 
.PN "ext-replies"
.br 
Specifies arbitrary X extension replies.  
.IP 
.PN "events"
.br 
Specifies core X protocol events and arbitrary extension events. 
.IP 
.PN "unsent-events"
.br 
Specifies core device events not delivered to any clients. 
.IP 
.PN "errors" 
.br
Specifies X protocol errors and arbitrary extension errors.  
.bp
  
.LP
.TS
tab (@) ;
l l l.
RECORDSTATE:@\s+2[\s0\fIenabled\fP:@BOOL
@\fIintercepted-clients\fP:@LISTofCARD32
@\fIintercepted-protocol\fP:@LISTofRECORDFLAGS\s+2]\s0
.TE 
.LP
This structure specifies the current state of a configuration 
environment.   
.PN "RECORDSTATE" 
is returned when the configuration state has been queried, typically by a 
recording client over the control connection.  
.LP
The enabled field specifies the state of data transfer between the extension 
and the recording client, and is either enabled (1) or disabled (0).  The 
default state is disabled.  When enabled, all core X protocol and 
extension protocol received from (xRequest) or sent to (xReply, xError, 
xEvent) a particular client, and requested by the recording client to 
be intercepted, will be reported to the recording client over the 
data connection.  
.LP
The intercepted-clients field defines the list of clients currently being 
recorded, where each recorded client is identified by its resource base.  
The intercepted-protocol field specifies the protocol to be intercepted 
for each recorded client.  

.NH 2
Errors
.LP
.IP
\fIConfiguration\fP\^:
.br
This error is returned if the value for a Configuration argument 
in a request does not name a valid CONFIGURATION.  
.bp 

.NH 1
Protocol
.XS
\*(SN Protocol
.XE
.NH 2
Requests 
.XS
\*(SN Requests
.XE
.sp 
.LP
.PN "QueryVersion"
.TA .75i
.ta .75i
.IP 
\fImajor-version\fP\, \fIminor-version\fP\^: CARD16  
.LP 
\(-> 
.IP
\fImajor-version\fP\, \fIminor-version\fP\^: CARD16
.br
.IP
Errors:  
.LP
This request specifies the RECORD extension protocol version the client 
would like to use.  When the specified protocol version is supported 
by the extension, the protocol version the server expects from the 
client is returned.  Clients should use this request before other RECORD 
extension requests.  If this request is not the first extension request 
executed, the protocol version used by the extension is implementation 
dependent.  
.LP
This request determines whether or not the RECORD extension protocol 
version specified by the client is supported by the extension.  If the 
extension supports the version specified by the client, this version number 
should be returned.  If the client has requested a higher version than is 
supported by the server, the server's highest version should be returned.  
Otherwise, if the client has requested a lower version than is supported
by the server, the server's lowest version should be returned.  This document 
defines major version one (1), minor version zero (0).  

.LP
.PN "CreateConfiguration"
.TA .75i
.ta .75i
.IP 
\fIid\fP\^: CONFIGURATION
.IP
\fIid-base\fP\^: CARD32
.IP
\fIflags\fP\^: RECORDFLAGS  
.br 
.IP
Errors:   
.PN Match , 
.PN Value , 
.PN Alloc  
.LP
This request creates a new 
.PN Configuration  
object within the server and assigns identifier id to it.  
Typically, this request is used by a recording client over the control 
connection to express interest in specific core X protocol and X 
extension protocol to be intercepted by the extension for a particular client. 
.LP   
The id-base argument is the resource identifier base sent to a client 
from the server in the connection setup reply, and hence, identifies the 
client to be recorded.  An id-base of zero (0) specifies the 
configuration values will apply to the set of clients that includes 
all current client connections and all future client connections.  
The flags argument specifies the values of the configuration environment, respective to the recorded client or clients.   
.LP
The error 
.PN "Match" 
is returned when the id-base argument does not identify a client connection.   
.PN "Value" 
is returned when a value specified falls outside of the accepted range.   
.PN "Alloc" 
is returned when the extension is unable to allocate the necessary 
resources. 

.LP
.PN "ChangeConfiguration"
.TA .75i
.ta  .75i
.IP
\fIconfiguration\fP\^: CONFIGURATION
.IP
\fIid-base\fP\^: CARD32
.IP
\fIflags\fP\^: RECORDFLAGS
.IP
\fIadd\fP\^:  BOOL   
.br 
.IP
Errors: 
.PN Match ,  
.PN Value , 
.PN Alloc , 
.PN Configuration  
.LP
Typically, this request is used by a recording client over the control connection to modify the specific protocol and to add or delete clients 
from the set of clients to be intercepted by the extension.  
.LP   
The id-base argument is the resource identifier base sent to a client 
from the server in the connection setup reply, and hence, identifies the 
client to be recorded.  An id-base of zero (0) specifies the 
configuration values will apply to the set of clients that includes 
all current client connections and all future client connections.  
The flags argument specifies the configuration values, respective 
to the recorded client or clients.   
.LP
The add argument specifies whether to add or delete the client, specified 
by id-base, from the configuration.  When add is ``True' and the client is 
not associated with the configuration, the configuration values are assigned 
to the client and the client is associated with the configuration.  When add 
is ``True' and the client is already associated with the configuration, the 
new configuration values specified by the flags argument are assigned to 
the client.   When add is ``False' the client is deleted from the set of 
clients associated with the configuration.  
.LP 
The error 
.PN "Match" 
is returned when the id-base argument does not identify a client connection. 
.PN "Value" 
is returned when a value specified falls outside of the accepted range.  
.PN "Alloc" 
is returned when the extension is unable to allocate the necessary 
resources.  When the configuration argument is not valid, a 
.PN Configuration 
error is returned.
  
.LP 
.PN "GetConfiguration"
.TA .75i
.ta  .75i
.IP
\fIconfiguration\fP\^: CONFIGURATION 
.LP 
\(->\(pl 
.IP
\fInum-clients\fP\^: CARD32
.IP
\fIstate\fP\^: RECORDSTATE
.br
.IP
Errors:
.PN Configuration 
.LP
This request queries the current state of a configuration and is typically used 
by a recording client over the control connection.  This request always 
returns the number of clients currently being recorded.  When the number of recorded clients is greater than zero (0), the current configuration 
environment is returned, consisting of the data transfer state, the list of 
recorded clients and the protocol values intercepted for each client.  
When the number of recorded clients is equal to zero (0), the current environment is returned, consisting only of the data transfer state.    
.LP
When the configuration argument is not valid, a 
.PN Configuration 
error is returned. 
 
.LP 
.PN "EnableConfiguration"
.TA .75i
.ta  .75i
.IP
\fIconfiguration\fP\^: CONFIGURATION
.LP 
\(->\(pl  
.IP
\fIreplies-following-hint\fP\^:  CARD32
.IP 
\fIdirection\fP\^:  {FromClient, FromServer} 
.IP
\fIclient-swapped\fP\^:  BOOL 
.IP
\fIid-base\fP\^:  CARD32 
.IP
\fIclient-seq\fP\^:  CARD32
.IP
\fIclient-req\fP\^:  CARD32
.IP 
\fIdata\fP\^: LISTofCARD8
.br 
.IP
Errors:  
.PN Match , 
.PN Alloc , 
.PN Configuration
.LP
This request specifies to enable data transfer between the recording client 
and the extension, and returns the protocol data the recording client 
has previously expressed interest in.  Typically, this request will be 
executed by the recording client over the data connection. 
.LP
Once the extension completes processing this request, it begins intercepting 
and reporting to the recording client all core and extension protocol 
received from or sent to clients associated with the configuration that the recording client has expressed interest in.  All intercepted protocol data 
is returned in the byte-order of the recorded client.  Therefore, 
recording clients are responsible for all byte swapping, if required.   
More than one recording client cannot enable data transfer on the 
same configuration at the same time.    
.LP
The replies-following-hint contains a positive value that specifies the 
number of replies that are likely, but not required, to follow.  When the 
disable request is made over the control connection, the reply is sent over 
the data connection with replies-following-hint set to zero, 
indicating the end of the reply sequence.  Direction is set to the direction 
of the protocol data, which is either from the client (xRequest), or from 
the server (xReply, xError, xEvent).  Client-swapped is ``True'' if the 
byte order of the client being recorded is swapped relative to the server; otherwise, client-swapped is ``False.'' Id-base is the resource identifier 
base sent to the client from the server in the connection setup reply, 
and hence, identifies the client being recorded.  Client-seq is set to 
the sequence number of the request just processed by the server on behalf 
of the recorded client and  client-req is set to the number of the request. 
.LP
The error 
.PN "Match" 
is returned when data transfer is already enabled.    
.PN "Alloc" 
is returned when the extension is unable to allocate the necessary 
resources.   When the configuration argument is not valid, a 
.PN Configuration 
error is returned. 
  
.LP 
.PN "DisableConfiguration"
.TA .75i
.ta  .75i
.IP
\fIconfiguration\fP\^: CONFIGURATION
.br 
.IP
Errors:  
.PN Value , 
.PN Alloc , 
.PN Configuration
.LP
This request is typically executed by the recording client over the 
control connection.  This request directs the extension to discontinue 
data transfer between the extension and the recording client and 
disable protocol reporting on the data connection that is currently 
enabled.   Once the extension completes processing this request, no 
protocol received from or sent to the client will be reported. 
.LP
The error   
.PN "Alloc" 
is returned when the extension is unable to allocate the necessary 
resources.   When the configuration argument is not valid, a 
.PN Configuration 
error is returned.
 
.LP
.PN "FreeConfiguration"
.TA .75i
.ta .75i
.IP 
\fIconfiguration\fP\^: CONFIGURATION
.br
.IP
Errors:  
.PN Configuration 
.LP 
This request deletes the association between the resource ID and the 
configuration and destroys the configuration.  If enabled for the 
configuration, data transfer between the extension and any recording 
clients will be discontinued.  All client connections that have enabled 
data transfer for the configuration will be released.  
.LP
A configuration is 
destroyed automatically when the connection to the creating client is 
closed down and the close-down mode is \fBDestroyAll\fP.  When the 
configuration argument is not valid, a 
.PN Configuration 
error is returned.
