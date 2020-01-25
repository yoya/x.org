.ds CF DRAFT
.de Cp
.QP
Convention:
.I
..
.de Ce
.R
..
.de Pp
.QP
Problem:
.I
..
.de Pe
.R
..
.de Ip
.IP \(bu 3
..
.de Ls
.DS L
..
.de Le
.DE
..
.TL
X Window System,  Version 11
.sp
Inter-Client Communication Conventions Manual
.sp
DRAFT 25\s-2\uth\d\s0 February 1988
.AU
David S. H. Rosenthal
.AI
Sun Microsystems
2550 Garcia Ave.
Mountain View CA 94043
.AB
.LP
It was an explicit design goal of the X Window System,  Version 11\s-2\u1\d\s0
.FS
1.  The X Window System is a Trademark of the Massachusetts Institute of
Technology.
.FE
to specify mechanism,  not policy.
As a result,  a client that converses with the server using the
protocol defined by the \fIX Window System Protocol,  Version 11\fP may
operate ``correctly'' in isolation,  but may not coexist properly
with others sharing the same server.  Conventions are proposed to
allow clients to cooperate in the areas of selections,  cut buffers,
window management, session management,  and resources.
.AE
.LP
.DS C
Copyright \(co 1987 by Sun Microsystems, Inc.\s-2\u2\d\s0
.DE
.FS
2.  Permission to use,  copy,  modify and distribute
this document for any purpose and without fee is hereby
granted,  provided that the above copyright notice and this permission
notice appear in all copies,  and that the name of Sun Microsystems,  Inc. not be
used in advertising or publicity pertaining to distribution of the software
without specific,  written prior permission.  Sun Microsystems,  Inc. makes no
representations about the suitability of the software described herein for
any purpose.  It is provided "as is" without express or implied warranty.
.FE
.sp 2
.QP
``If you smile at me,  you know that I will understand,  'cos
that is something everybody everywhere does in the same language.''
.br
.nf
           Jefferson Airplane,  \fIWooden Ships\fP.
.fi
.NH
Introduction
.LP
It was an explicit design goal of X11 to specify mechanism,  not policy.
As a result,  a client that converses with the server using the
protocol defined by the \fIX Window System Protocol,  Version 11\fP may
operate ``correctly'' in isolation,  but may not coexist properly
with others sharing the same server.
.LP
Being a good citizen in the X11 world involves adhering to
conventions governing inter-client communications in a number of areas:
.Ip
The selection mechanism.
.Ip
The cut-buffers.
.Ip
The window manager.
.Ip
The session manager.
.Ip
The resource database.
.Ip
The manipulation of shared resources.
.LP
In the following sections we propose suitable conventions for each area,
in so far as it is possible to do so \fIwithout\fP enforcing a particular
user interface.
.NH 2
Status of the Selection Conventions
.LP
Apart from the obvious gaps in the specification of the target and type
atom lists,  the selection conventions described above have caused
little controversy.  This may reflect the fact that the selection mechanism
is,  as yet,  little used.
.LP
It will obviously take some time for these conventions to obtain formal
endorsement,  and for the toolkits and clients to be changed to conform.
.NH
Peer-to-Peer Communication via Selections
.LP
The primary mechanism X11 defines for clients that want to
exchange information,
for example by cutting and pasting between windows,  are \fIselections\fP.
There can be an arbitrary number of selections,  each named by an atom,
and they are global to the server.
The choice of an atom to be used is discussed later.  Each selection is
owned by a client,  and is attached to a window.
.LP
Selections communicate between an \fIowner\fR and a \fIrequestor\fR.  The
owner has the data representing the value of its selection,  and the requestor
receives it.
A requestor wishing to obtain the value of a selection provides:
.Ip
the name of the selection
.Ip
the name of a property
.Ip
a window
.Ip
an atom representing the datatype required
.LP
If the selection is currently owned,  the owner receives an
event,  and is expected to:
.Ip
convert the contents of the selection to the requested datatype
.Ip
place this data in the named property on the named window
.Ip
send the requestor an event to let it know the property is available.
.LP
Clients are strongly encouraged to use this mechanism.  In particular,
displaying text in a permanent window without providing the ability to select it 
and convert it into a string is definitely anti-social.
.LP
Note that,  in the X11 environment,  \fIall\fP data transferred between
an owner and a requestor must normally go via the server.  An X11 client cannot
assume that another client can open the same files,  or even communicate
directly.  The other client may be talking to the server via a
completely different networking mechanism (for example,  one client might
be DECnet,  and the other TCP/IP).  Thus,  passing indirect references to data
such as file names,  hostnames & port numbers,  and so on is permitted only
if both clients specifically agree.
.NH 2
Acquiring Selection Ownership
.LP
A client wishing to acquire ownership of a particular selection
should call SetSelectionOwner:
.Ls
SetSelectionOwner
	selection:	ATOM
	owner:		WINDOW or None
	time:		TIMESTAMP or CurrentTime
.Le
.LP
The client should set "selection" to the Atom representing the selection,
set "owner" to some Window, that it created  and set "time" to some time
between the current last-change time of the selection concerned,
and the current server time.  This time value will
normally be obtained from the timestamp of the event triggering the
acquisition of the selection.  Clients should \fInot\fP set the time
value to CurrentTime,  since if they do so they have no way of finding
when they gained ownership of the selection.  Clients must use a window
they created,  since SendEvent will be used with an empty mask to
reply.
.Cp
Clients attempting to acquire a selection must set the time value of
the SetSelectionOwner request to the timestamp of the event triggering
the acquisition attempt,  not to CurrentTime.
A zero-length append to a property is a way to obtain a time-stamp for
this purpose,  the timestamp is in the corresponding PropertyNotify event.
.Ce
.LP
Note that if the time in the SetSelectionOwner request is in the future
relative to the server's current time,  or if it is in the past relative
to the last time the selection concerned changed hands,  the SetSelectionOwner
request appears to the client to succeed,  but ownership is \fInot\fP
actually transferred.
.Cp
Clients are normally expected to provide some visible confirmation
of selection onwership.
To make this feedback reliable,  a client
must perform the sequence:
.IP
<XXX - Xlib stuff>
.Ls
XSetSelectionOwner(dpy, seln, own, time);
if (XGetSelectionOwner(dpy, seln) != own) {
    /* We didn't get the selection */
.Le
.Ce
.LP
If the SetSelectionOwner request succeeds (not merely appears to suceed),
the client issuing it
is recorded by the server as being the owner of the selection for the
time period starting at "time".
Since clients cannot name other clients
directly,  they use the "owner" window to refer to the owning client
in the replies to GetSelectionOwner,  and in SelectionRequest and
SelectionClear events,
and possibly as a place to put properties describing the selection
in question.
.LP
To discover the owner of a particular selection,  a client should invoke:
.Ls
GetSelectionOwner
	selection:	ATOM
=>
	owner:		WINDOW or None
.Le
.Pp
There is no way for anyone to find out the last-change time of
a selection.  At the next protocol revision,  GetSelectionOwner
should be changed to return the last-change time as well as the owner.
.Pe
.NH 2
Responsibilities of the Selection Owner
.LP
When a requestor wants the value of a selection,  the owner receives
a SelectionRequest event:
.Ls
SelectionRequest
	owner:		WINDOW
	selection:	ATOM
	target:		ATOM
	property:	ATOM or None
	requestor:	WINDOW
	time:		TIMESTAMP or CurrentTime
.Le
.LP
The owner and the selection fields will be the values specified
in the SetSelectionOwner request.
The owner should compare the time stamp with the period it has owned
the selection and,  if the time is outside,
refuse the SelectionRequest by sending the requestor window
a SelectionNotify event with the ``property'' set to None,  using
SendEvent with an empty event-mask.
.LP
More advanced selection owners are free to maintain a history
of the value of the selection,  and to respond to requests for the
value of the selection during periods they owned it before the current
one.
.LP
Otherwise,  the owner should use the ``target'' field to decide the
form to convert the selection into,  and if the selection cannot be
converted into that form,  refuse the SelectionRequest similarly.
.LP
If the ``property'' field is not None,  the owner should place the
data resulting from converting the selection into the specified
property on the requestor window,  setting the property's type
to some appropriate value (which need not be the same as ``target'').
If the ``property'' field is None,  the owner should choose a suitable
property name and place the data as that property on the requestor
window,  setting the type as before.
.Cp
All properties used to reply to SelectionRequest events must be placed
on the requestor window.
.Ce
.LP
In either case,  if the data comprising the selection cannot be
stored on the requestor window (for example,  because the server cannot
provide sufficient memory),  the owner must refuse the SelectionRequest
as above.  See the section on ``Large Data Transfers'' below.
.LP
If the property is successfully stored,
the owner should acknowledge the successful conversion
by sending the requestor window a SelectionNotify event,
using SendEvent with an empty mask:
.Ls
SelectionNotify
	requestor:	WINDOW
	selection:	ATOM
	target:		ATOM
	property:	ATOM or None
	time:		TIMESTAMP or CurrentTime
.Le
.LP
The ``selection'',  ``target'' and ``property'' fields should be
set to the values received in the SelectionRequest event
(Setting the ``property'' field to None indicates that the conversion
requested could not be made).
.Cp
The ``selection'',  ``target'',  ``time'' and ``property'' fields should be
set to the values received in the SelectionRequest event.
.Ce
.LP
The data stored in the property must eventually be deleted.  A convention
is needed to assign the responsibility for doing so.
.Cp
Selection requestors are responsible for deleting properties whose
names they receive in SelectionNotify events (See section 2.4).
Owners are responsible for deleting all other properties involved
in communicating selections.
.Ce
.LP
A selection owner will often need confirmation that the data comprising the
selection has actually been transferred (for example,  if the operation has
side-effects on the owner's internal data-structures these should not
take place until the data has been successfully received).
They should express interest in PropertyNotify events for the ``requestor''
window and wait until the property in the SelectionNotify event has
been deleted before assuming that the selection data has been transferred.
.LP
When some other client acquires a selection,  the previous owner
receives a SelectionClear event:
.Ls
SelectionClear
	owner:		WINDOW
	selection:	ATOM
	time:		TIMESTAMP
.Le
.LP
The ``timestamp'' field is the time at which the ownership changed hands,
and the ``owner'' field is the window the new owner specified in its
SetSelectionOwner request.
.LP
If an owner loses ownership while it has a transfer in progress,
that is to say before it receives notification that the requestor
has received all the data,  it must continue to service the on-going
transfer until it is complete.
.NH 2
Giving Up Selection Ownership
.NH 3
Voluntarily
.LP
To relinquish ownership of a selection voluntarily,  a client should
execute a SetSelectionOwner request for that selection atom,
with owner specified as None,  and time specified as CurrentTime.
.LP
Alternatively,  the client may destroy the window used as the "owner"
value of the SetSelectionOwner request,  or it may terminate.  In both
cases the ownership of the selection involved will revert to None.
.NH 3
Forcibly
.LP
If a client gives up ownership of a selection,  or if some other
client executes a SetSelectionOwner for it,  the client will receive
a SelectionClear event:
.Ls
SelectionClear
	owner:		WINDOW
	selection:	ATOM
	time:		TIMESTAMP
.Le
.LP
The timestamp is the time the selection changed hands.
The owner argument is the
window that was specified by the current owner in its SetSelectionOwner
request.
.NH 2
Requesting a Selection
.LP
A client wishing to obtain the value of a selection in a particular
form issues a ConvertSelection request:
.Ls
ConvertSelection
	selection:	ATOM
	target:		ATOM
	property:	ATOM or None
	requestor:	WINDOW
	time:		TIMESTAMP or CurrentTime
.Le
.LP
The selection field specifies the particular selection involved,  and
the target specifies the form the information is required in.
The choice of suitable atoms to use is discussed below.
The requestor field should be set to a window the requestor created;
the owner will use it to place the reply property on.
The time field should be set to the timestamp on the event triggering
the request for the selection value,  clients should \fInot\fP use
CurrentTime for this field.
.Cp
Clients should not use CurrentTime for the time field of ConvertSelection
requests.
They should use the timestamp of the event that caused the request to
be made.
.Ce
.LP
The property field should be set to the name of a property that the owner
can use to report the value of the selection,
or to None (in which case the owner will chose a property name).
Note that the requestor of a selection needs to know neither the owner
of the selection,  nor the window it is attached to.
.Cp
Requestors should,  wherever practicable,  use None for the property
field of ConvertSelection requests.
By allowing the owner to choose the name of the reply property in this way,
the need to coin new atoms can be reduced.
.Ce
.LP
The result of the ConvertSelection request is that a SelectionNotify event
will be received:
.Ls
SelectionNotify
	requestor:	WINDOW
	selection:	ATOM
	target:		ATOM
	property:	ATOM or None
	time:		TIMESTAMP or CurrentTime
.Le
.LP
The ``requestor'',  ``selection'',  ``time'' and ``target'' fields will be the same
as those on the ConvertSelection request.
.LP
If the ``property'' field is None,  the conversion has been refused.
This can mean that there is no owner for the selection,  that the owner
does not support the conversion implied by ``target'',  or that the
server did not have sufficient space to accomodate the data.
.LP
If the ``property'' field is not None,  then that property will exist on the
``requestor'' window.  The value of the selection can be retrieved from this
property by using the GetProperty request:
.Ls
GetProperty
	window:		WINDOW
	property:	ATOM
	type:		ATOM or AnyPropertyType
	long-offset:	CARD32
	long-length:	CARD32
	delete:		BOOL
=>
	type:		ATOM or None
	format:		{0, 8, 16, 32}
	bytes-after:	CARD32
	value:		LISTofINT8 or LISTofINT16 or LISTofINT32
.Le
.LP
When using GetProperty to retrieve the value of a selection,  the ``property''
field should be set to the corresponding value in the SelectionNotify event.
The ``type'' field should be set to AnyPropertyType,  because the requestor
has no way of knowing beforehand what type the selection owner will use.
Several GetProperty requests may be needed to retrieve all the data in the
selection;  each should set the ``long-offset'' field to the amount
of data received so far,  and the
``size'' field to some reasonable buffer size (see the section on ``Large
Data Transfers'').
If the returned value of
``bytes-after'' is zero the whole property has been transferred.
.LP
Once all the data in the selection has been retrieved,  which may require
getting the values of several properties (see the section on ``Selection
Properties''),  the property in the SelectionNotify
should be deleted by invoking GetProperty
with the ``delete'' field set True.
As discussed above,  the owner has no way of knowing when the data has been
transferred to the requestor unless the property is removed.
.Cp
The requestor must delete the property named in the SelectionNotify
once all the data has been retrieved.
They should invoke either DeleteProperty,  or
GetWindowProperty(delete==TRUE) after they have sucessfully retrieved
all data comprising the selection.  See the section on ``Large Data Transfers''
below.
.Ce
.NH 2
Large Data Transfers
.LP
Selections can get large,  and this poses two problems:
.Ip
Transferring large amounts of data to the server is expensive,  and it
would be beneficial to be able to reuse the data once it has been sent
to answer further ConvertSelection requests.
.Ip
All servers will have limits on the amount of data that can be stored
in properties.  Exceeding this limit will result in a BadAlloc error
on the ChangeProperty request that the selection owner uses to store
the data.
.LP
To deal with the first problem,  we define the following structure:
.Ls
typedef struct {
	Window		window;
	Atom		property;
	unsigned long	start;
	unsigned long	length;
} XPropertyPart;
.Le
and establish the following conventions:
.Cp
Selection owners should store the data describing a largish selection
(where ``largish'' is defined flexibly,  but is definitely less than
the maximum-request-length in the connection handshake) in
a property on the owner window,  not on the requestor window.
They should reply to the SelectionRequest with a property of type
INDIRECT on the requestor window whose content is an array of
XPropertyPart structures describing the parts of some other properties
containing the selection.
Normally,  there will be a single entry in the array.
The properties so named is the owner's responsibility,  requestors should not
delete it.
.Ce
.LP
Since the actual selection data remains in properties on the selection owner
window,  it can be used to reply to futher SelectionRequest events.
However,  owners must be careful not to modify the data in these
properties between replying to the SelectionRequest and receiving the
corresponding PropertyNotify indicating that the requestor has the data.
.LP
The problem of limited server resources is addressed by the following
conventions:
.Cp
Selection owners should transfer the data describing an immense selection
(where ``immense'' is defined flexibly,  but is larger than large-ish)
using the INCREMENTAL property mechanism (see below).
.Ce
.Cp
Any client using SetSelectionOwner to acquire selection ownership should
arrange to process BadAlloc errors.  For clients using Xlib,  this
involves using XSetErrorHandler() to override the default handler.
.Ce
.Cp
A selection owner must confirm that no BadAlloc error ocurred while
storing the properties for a selection before replying with a
confirming SelectionNotify event.
.Ce
.Cp
When storing large amounts (relative to max-request-size) of data,
clients should use a sequence of ChangeProperty(mode==Append)
requests for reasonable quantities of data.
This is to avoid locking-up servers,  and to limit the waste of
data transfer caused by a BadAlloc error.
.Ce
.Cp
If a BadAlloc error occurs during storing the selection data,
all properties stored for this selection should be deleted,
and the ConvertSelection request refused by replying with a
SelectionNotify event with ``property'' set to None.
.Ce
.Cp
In order to avoid locking-up servers for inordinate lengths of time,
requestors retrieving large quantities of data from a property
should perform a series of GetProperty requests,  each asking for a
reasonable amount of data.
.Ce
.LP
Single-threaded servers should be changed to avoid locking-up during large
data transfers.
.NH 2
Usage of Selection Atoms
.LP
It is important to observe that defining a new atom consumes resources
in the server, and they are not released until the server re-initializes.
Thus,  it must be a goal to reduce the need for newly minted atoms.
.NH 3
Selection Atoms
.LP
There can be an arbitrary number of selections,  each named by an atom.
To conform with the inter-client conventions,  however,  clients need
deal with only these three selections:
.Ip
PRIMARY
.Ip
SECONDARY
.Ip
CLIPBOARD
.LP
Other selections may be used freely for private communciation among
related groups of clients.
.Pp
How does a client find out which selection atoms are valid?
.Pe
.NH 4
The PRIMARY Selection
.LP
The selection named by the atom PRIMARY is used for all commands
which take only a single argument.
It is the principal means of communication between clients
which use the selection mechanism.
.NH 4
The SECONDARY Selection
.LP
The selection named by the atom SECONDARY is used:
.Ip
As the second argument to commands taking two arguments,  for example
``exchange primary and secondary selections''.
.Ip
As a means of obtaining data when there is a primary selection,
and the user does not wish to disturb it.
.NH 4
The CLIPBOARD Selection
.LP
The selection named by the atom CLIPBOARD is used to hold deleted
data.
Clients deleting data should:
.Ip
Assert ownership of the CLIPBOARD.
.Ip
Be prepared to respond to a request for the contents of the CLIPBOARD
in the normal way,  returning the deleted data.
The request will be generated by the clipboard client described below.
.LP
Clients wishing to restore deleted data should request the contents of
the CLIPBOARD selection in the usual way.
.LP
Except while a client is actually deleting data,
the owner of the CLIPBOARD selection will be a single, special client
implemented for the purpose.
It should:
.Ip
Assert ownership of the CLIPBOARD selection.
.Ip
If it loses the selection (which will be because someone has some
newly deleted data):
.RS
.Ip
Obtain the contents of the selection from the new owner.
.Ip
Re-assert ownership of the CLIPBOARD selection
.RE
.Ip
Respond to requests for the CLIPBOARD contents in the normal way.
.NH 3
Target Atoms
.LP
The atom that a requestor supplies as the ``target'' of a ConvertSelection
request determines the form of the data supplied.
The set of such atoms is extensible,  but a generally accepted base
set of target atoms is needed.
As a starting point for this,  Table 1 contains those that have been suggested
so far.
.KF
.FS
*  PostScript is a Trademark of Adobe Systems Inc.
\(dg  InterPress is a Trademark of Xerox.
.FE
.TS
center, box;
c s
c c
l l.
Table 1 \- Target Atoms and their Meanings
_
Atom	Meaning
_
TARGETS	list of valid target atoms
INDIRECT	look in the ConvertSelection property
STRING	Un-interpreted ISO Latin 1 text
PIXMAP	Pixmap ID
ODIF	ISO Office Document Interchange Format
POSTSCRIPT	PostScript\s-2\u*\d\s0 program
INTERPRESS	InterPress\s-2\u\(dg\d\s0 program
OWNER_OS	operating system of owner
FILE_NAME	full path name of a file
HOST_NAME	see WM_CLIENT_MACHINE
IP_ADDRESS	DARPA net address/port for owner
DECNET_ADDRESS	DECnet address for owning host
CHARACTER_POSITION	start and end of selection in bytes
LINE_NUMBER	start and end line numbers
COLUMN_NUMBER
LENGTH	number of bytes in selection
USER	name of user running owner
PROCEDURE	name of selected procedure
MODULE	name of selected module
PROCESS	process ID of owner
TASK	task ID of owner
CLASS	class of owner - see WM_CLASS
NAME	name of owner - see WM_NAME

XXX	need to generate more & agree
.TE
.KE
.LP
Selection owners are required to support the following targets:
.Ip
TARGETS.
They should return a list of the targets they are prepared to
convert their selection into.
.Ip
INDIRECT.
The INDIRECT target atom is valid only when a property is specified
on the ConvertSelection request.
If the property field in the SelectionRequest event is None and the
target is INDIRECT,  it should be refused.
.IP
When a selection owner receives a SelectionRequest(target=INDIRECT)
request,  the contents of the property named in the request will be a
list of atom pairs,  the first atom naming a target,  and the second
naming a property (or None).
The effect should be as if the owner had received a sequence of
SelectionRequest events,  one for each atom pair,  except that;
.RS
.Ip
the owner should reply with a SelectionNotify only when all the requested
conversions have been performed.
.Ip
the owner should replace any property atoms it received as None with
the properties used to store the converted data.
.Ip
the owner should replace any property atoms for targets it failed to
convert with None.
.RE
.LP
All other targets are optional.
.NH 2
Usage of Selection Properties
.LP
The names of the properties used in selection data transfer are chosen by:
.Ip
The requestor,  if the ``property'' field is not None.
.Ip
The selection owner,  if the ``property'' field is None,  and also in the case
of all indirect properties.
.LP
The type of the properties involved is always chosen by the selection owner,
and they can involve some types with special semantics assigned by convention.
These special types are reviewed in the following sections.
.LP
Clients receiving properties of these types should ensure that they
have retreived all data they need from the other objects so named \fIbefore\fP
they delete the property named in the reply.
.NH 3
STRING Properties
.LP
Clients receiving properties of type STRING can assume that for the purposes
of displaying them the encoding is ISO Latin 1.
.LP
Character strings requiring other encodings are transmitted as properties
with other types.
Table 2 is a partial list of such types.
.KF
.TS
center, box;
c s
c c
l l.
Table 2 \- Property Types and Encodings
_
Type Atom	Meaning
_
STRING	ISO Latin 1

XXX	need more entries - who will generate?
.TE
.sp
.KE
.NH 3
INDIRECT Properties
.LP
Clients may receive properties of type INDIRECT.
The contents of these properties will be arrays of window/atom pairs.
The data in the selection consists of the data in each of the
properties so named in turn,  starting from the start of the array.
.LP
All selection requestors must be prepared to receive properties of
type INDIRECT.
.LP
It is an error if the types of the second and subsequent properties
in the list differ from the type of the first.
.LP
The properties named in the INDIRECT
property are the owner's responsibility,  requestors should not
delete them.
.NH 3
INCREMENTAL Properties
.LP
Clients may receive properties of type INCREMENTAL.
The contents of the property will be a window/atom pair,  which the client
and the selection owner will use to communicate using the following
method.
.LP
The selection owner:
.Ip
appends the data in suitable-size chunks to the
property.
The size should be less than the maximum-request-size in the connection
handshake.
Between each append,  the owner should wait for a PropertyNotify
(state==Deleted) event showing that the requestor has read the data.
The reason for doing this is to limit the consumption of space in the
server.
.Ip
when the entire data has been transferred to the server,
waits until a PropertyNotify (state==Deleted)
showing that the data has been read by the requestor,
and then writes zero-length data to the property.
.LP
The selection requestor:
.Ip
waits for the SelectionNotify event.
.Ip
loops:
.RS
.Ip
retrieving data using GetProperty with ``delete'' True
.Ip
waiting for a PropertyNotify with state==NewValue
.RE
.Ip
until a zero-length property is obtained
.Ip
deletes the type INCREMENTAL property
.NH
Peer-to-Peer Communication via Cut-Buffers
.LP
This section is missing;  Carnegie-Mellon has agreed to supply some
information for it.
.if 0 \{
.IP
<XXX - Andy Palay will supply>
\}
.NH
Client to Window-Manager Communciation
.LP
To permit window managers to perform their role of mediating the competing
demands for resources such as screen space,  the clients being managed must
adhere to certain conventions,  and must expect the window managers to
do likewise.
These conventions are covered here from the client's point of view,
and again from the window manager's point of view in the
.I
Window and Session Manager Conventions Manual.
.R
.LP
In general,  these conventions are somewhat complex,  and will
undoubtedly change through time as new window management paradigms
are developed.
There is thus a strong bias towards defining only those conventions
that are essential,  and which apply generally to all window management
paradigms.
Clients designed to run with a particular window manager can easily
define private protocols to add to these conventions,
but must be aware that their users may decide to run some other
window manager no matter how much the designers of the private protocol
are convinced that they have seen the ``one true light'' of user interfaces.
.LP
It is a principle of these conventions that a general client should
neither know nor care which window manager is running,  or indeed if
one is running at all.
Each window manager will implement a particular window management
policy;  the choice of an appropriate window management policy
for the user's circumstances is not one for an individual client to
make but will be made by the user or the user's system administrator.
This does not exclude the possibility of writing clients that
use a private protocol to restrict themselves to operating only
under a specific window manager,  it merely ensures that no 
claim of general utility is made for such programs.
.LP
For example,  the claim is often made ``the client I'm writing is
important,  and it needs to be on top''.  Well,  maybe it is
important when it is being run for real,  and it should then be run
under the control of a window manager that recognizes ``important''
windows through some private protocol and ensures that they are on top.
However,  imagine that the ``important'' client is being
debugged.  Then,  ensuring that it is always on top is no
longer the appropriate window management policy,  and it should be
run under a window manager that allows other windows (e.g. the
debugger) to appear on top.
.LP
For clients which need to scan the resource database for information
about the window manager's resources,  the res_class of the window manager
is ``Wm'',  and the res_name is the name of the window manager (e.g.
``uwm'').
Note that this differs from the normal convention,  for two reasons:
.Ip
Clients should be able to discover the resources the window manager is
using without being forced to know which window manager it is.
.Ip
At most one (top-level) window manager should be running,  even if the server is
driving several screens.
The code for ``wm'' shows how a window manager can deal with multiple screens.
.NH 2
Client's Actions
.LP
In general,  the object of the X11 design is that clients should as
far as possible do exactly what they would do in the absence of a
window manager,  except for:
.Ip
Hinting to the window manager about the resources they would like
to obtain.
.Ip
Cooperating with the window manager by accepting the resources they
are allocated,  even if they are not  those requested.
.Ip
Being prepared for resource allocations to change at any time.
.NH 3
Creating a Top-Level Window
.LP
A client would normally expect to create its top-level windows
as children of one or more of the root windows,  using some
boilerplate like:
.Ls
win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
			      xsh.x, xsh.y, xsh.width, xsh.height,
			      bw, bd, bg);
.Le
or,  if a particular one of the roots was required,  like:
.Ls
win = XCreateSimpleWindow(dpy, RootWindow(dpy, screen),
			      xsh.x, xsh.y, xsh.width, xsh.height,
			      bw, bd, bg);
.Le
.LP
Ideally,  it should be possible to override the choice of a root
window and allow clients (including window managers) to treat a
non-root window as a pseudo-root.
This would allow,  for example,  testing of window managers and the
use of application specific window managers to control the sub-windows
owned by the members of a related suite of clients.
.LP
To support this,  we define the following extension to the semantics of
XOpenDisplay() on 
.UX -based
systems (similar extensions are required
for other operating systems).
.LP
On
.UX -based
systems,
the display name or DISPLAY environment variable is a string
that has the format:
.LP
.IP "" 1i
\fIhostname\fP\^:\^\fInumber\fP\^.\^\fIscreen\fP\^.\^\fIprop\fP
.LP
If the prop component is present,  it should be interpreted
as the name of a property on the root window of each screen as returned
in the connection handshake.
If that property exists,  and has the type SCREEN,  its contents will
be a PseudoScreen structure containing information that,
from the client's point of view,  should replace the information in the
connection handshake describing this screen.
.Ls
typedef struct {
	Window root;		/* Root window id. */
	long width, height;	/* width and height of screen */
	long mwidth, mheight;	/* width and height of  in millimeters */
	VisualID root_visual;	/* root visual */
	Colormap cmap;		/* default color map */
	unsigned long white_pixel;
	unsigned long black_pixel;	/* White and Black pixel values */
	long max_maps, min_maps;	/* max and min color maps */
	long backing_store;	/* Never, WhenMapped, Always */
	Bool save_unders;	
	long root_input_mask;	/* initial root input mask */
	Atom depths;		/* list of allowable depths on the screen */
} PseudoScreen;
.Le
.LP
The ``depths'' field is either None,
in which case the handshake depths and visuals are valid,
or the name of a property of type DEPTHS on the same
window,  which contains an array of PseudoDepth structures.
The number of elements in this array controls the ``ndepths'' field
of the Display structure.
.Ls
typedef struct {
	long depth;		/* this depth (Z) of the depth */
	long nvisuals;		/* number of Visual types at this depth */
	Atom visuals;	/* list of visuals possible at this depth */
} PseudoDepth;
.Le
.LP
The depth of the Visual whose ID is ``root_visual'' controls the
``root_depth'' field of the Display structure.
The ``visuals'' field is either None,
in which case the handshake Visuals for this depth are
valid,  or the name of a property of type VISUALS on the
same window,  which contains an array of PseudoVisual structures:
.Ls
typedef struct {
	VisualID visualid;	/* visual id of this visual */
	long class;		/* class of screen (monochrome, etc.) */
	unsigned long red_mask, green_mask, blue_mask;	/* mask values */
	long bits_per_rgb;	/* log base 2 of distinct color values */
	long map_entries;	/* color map entries */
} PseudoVisual;
.Le
.NH 3
Client Properties
.LP
Once the client has one or more top-level windows,  it must place
properties on that window to inform the window manager of its desired
behaviour.
Some of these properties are mandatory,  and some are optional.
Properties written by the client will not be changed by the window manager.
.NH 4
WM_NAME
.LP
The WM_NAME property is an un-interpreted
string that the client wishes displayed
in association with the window (for example,  in a window headline bar).
.LP
The encoding used for this string (and all other un-interpreted string
properties) is implied by the type of the property.  Type STRING
implies ISO Latin 1 encoding;  for other types see the section on
``Usage of Selection Properties''.
.LP
Window managers are expected to make an effort to display this
information;  simply ignoring WM_NAME is not acceptable behaviour.
Clients can assume that at least the first part of this string
is visible to the user,  and that if the information is not
visible to the user it is because the user has taken an explicit
decision to make it invisible.
.LP
On the other hand,  there is no guarantee that the user can see
the WM_NAME string even if the window manager supports window
headlines.  The user may have placed the headline off-screen,
or have covered it by other windows.  WM_NAME should not be
used for application-critical information,  nor to announce
asynchronous changes of application state that require timely user response.
The expected uses are:
.Ip
to permit the user to identify one of a
number of instances of the same client
.Ip
To provide the user with non-critical state information.
.LP
Note that even window managers that support headline bars
will place some limit on the length of string that can be visible;
brevity here will pay dividends.
.NH 4
WM_ICON_NAME
.LP
The WM_ICON_NAME property is an un-interpreted
string that the client wishes displayed
in association with the window when it is iconified (for example,
in an icon label).
In other respects,  it is similar to WM_NAME.
Fewer characters will normally be visible in WM_ICON_NAME than WM_NAME,
for obvious geometric reasons.
.NH 4
WM_NORMAL_HINTS
.LP
The WM_NORMAL_HINTS property is a XSizeHints structure describing
the desired window geometry.
.Ls
typedef struct {
	long flags;
	int x, y;
	int width, height;
	int min_width, min_height;
	int max_width, max_height;
	int width_inc, height_inc;
	struct {
	       int x;  /* numerator */
	       int y;  /* denominator */
	} min_aspect, max_aspect;
	int base_width, base_height;
} XSizeHints;
.Le
.Pp
In this and other property structure definitions,  it is assumed that
sizeof (int) and sizeof (long) are 4.
.Pe
.LP
The definitions for the flags field are:
.LP
.DS 0
.TA .5i 2i 3i
.ta .5i 2i 3i
#define USPosition	(1L << 0)	/* user specified x, y */
#define USSize	(1L << 1)	/* user specified width, height */
#define PPosition	(1L << 2)	/* program specified position */
#define PSize	(1L << 3)	/* program specified size */
#define PMinSize	(1L << 4)	/* program specified minimum size */
#define PMaxSize	(1L << 5)	/* program specified maximum size */
#define PResizeInc	(1L << 6)	/* program specified resize increments */
#define PAspect	(1L << 7)	/* program specified min and max aspect ratios */
#define PBaseSize	(1L << 8)	/* progran specified base size */
.DE
The x, y, width, and height elements describe a desired position and size
for the window.
The coordinate system for x and y is the (pseudo-) root window,  irrespective
of any reparenting that may have occurred.
To indicate that this information was specified by the user, set the
.B USPosition
and
.B USSize
flags. 
To indicate that it was specified by the client without any user
involvement, set 
.B PPosition
and 
.B PSize .
This allows a window manager to know that the user specifically asked where
the window should be placed or how the window should be sized and that
the window manager does not have to rely the program's opinion.
.LP
The min_width and min_height elements specify the
minimum size that the window can be for the client to be useful.
The max_width and max_height elements specify the maximum size.
The base_width and base_height elements in conjunction with width_inc
and height_inc define an arithmetic progression of preferred window
widths and heights:
.Ls    
width = base_width + ( i * width_inc )
height = base_height + ( j * height_inc )
.Le
for non-negative integers i and j.  Window managers are encouraged to
use i and j instead of width and height in reporting window sizes to
users.  If a base size is not provided, the minimum size is to be
used in its place, and vice versa.
.LP
The min_aspect and max_aspect elements are expressed
as ratios of x and y, 
and they allow a client to specify the range of aspect
ratios it prefers.
.NH 4
WM_HINTS
.LP
.Ls
typedef struct {
	long flags;
	Bool input;
	int initial_state;
	Pixmap icon_pixmap;
	Window icon_window;
	int icon_x, icon_y;
	Pixmap icon_mask;
	XID window_group;
	unsigned int messages;
} XWMHints;
.Le
.LP
The definitions for the flags field are:
.sp
.DS 0
.TA .5i 2.5i
.ta .5i 2.5i
#define	InputHint	(1L << 0)
#define	StateHint	(1L << 1)
#define	IconPixmapHint	(1L << 2)
#define	IconWindowHint	(1L << 3)
#define	IconPositionHint	(1L << 4)
#define	IconMaskHint	(1L << 5)
#define	WindowGroupHint		(1L << 6)
#define	MessageHint	(1L << 7)
.DE
.LP
The input field is used to communicate to the window manager the input focus
model used by the client
There are four such models:
.Ip
No Input.
The client never expects keyboard input.
.IP
An example would be xload,  or another output-only client.
.Ip
Passive Input.
The client expects keyboard input but never explicitly sets the input focus.
.IP
An example would be a simple client with a no subwindows,  which will
accept input in PointerRoot mode,  or when the window manager sets
the input focus to its top-level window (in click-to-type mode).
.Ip
Locally Active Input.
The client expects keyboard input,  and explicitly sets the input focus,
but only does so when one of its windows already has the focus.
.IP
An example would be a client with sub-windows defining various data
entry fields,  that uses Next and Prev keys to move the input focus
between the fields,  once its top-level window has acquired the focus
in PointerRoot mode,  or when the window manager sets the input focus
to its top-level window (in click-to-type mode).
.Ip
Globally Active Input.
The client expects keyboard input,  and explicitly sets the input focus even when
it is in windows the client does not own.
.IP
An example would be a client with a scroll bar,  that wants to allow
users to scroll the window without disturbing the input focus even if
it is in some other window.
It wants to acquire the input focus when the user clicks in the scrolled
region,  but not when the user clicks in the scroll bar itself.  Thus,
it wants to prevent the window manager setting the input focus to any of
its windows.
.LP
Clients with the Globally Active and No Input models should set the
``input'' flag to
.B False .
Clients with the Passive and Locally Active should set the ``input''
flag to
.B True .
For more details,  see the FocusMessage bit below.
.LP
The definitions for the initial_state flag are:
.sp
.DS 0
.TA .5i 2.5i
.ta .5i 2.5i
#define	DontCareState	0
#define	NormalState	1
#define	ClientIconState	2
#define	IconicState	3
#define	InactiveState	4
#define	IgnoreState	5
.DE
.LP
The value of the initial_state flag determines the state the client
wishes to be in at the time the top-level window is mapped.
In addition,
clients may ask the window manager to switch between states by
setting the initial_state flag
(its name has become somewhat misleading).
The states are:
.Ip
DontCareState.
The client doesn't care what state it is in.
.Ip
NormalState.
The client wants its top-level window to be visible.
.Ip
ClientIconState.
The client wants its icon_window to be visible.
If icon_windows are not available,  it wants its top-level window visible.
.Ip
IconicState.
The client wants to be iconic,  whatever that means for this window manager.
It can assume that at least one of its icon_window (if any),
its icon_pixmap (if any),
or its WM_ICON_NAME will be visible.
.Ip
InactiveState.
The client wants to be inactive,  whatever that means for this window manager.
Inactive windows will normally be available from a pop-up menu,  or
some other means that doesn't involve permanently allocating screen real-estate.
.Ip
IgnoreState.
The client wants the window manager to ignore this window,  and in return
agrees that attempts to map it (or its icon_window) will be ignored.
.LP
The icon_pixmap field may specify a pixmap to be used as an icon.
This pixmap should be:
.Ip
One of the sizes specified in the WM_ICON_SIZES property on the (pseudo-)
root.
.Ip
1-bit deep.  The window manager will select,  through the defaults database,
suitable background (for the 0 bits)
and foreground (for the 1 bits) colors.  These defaults can,  of
course,  specify different colors for the icons of different clients.
.LP
The icon_mask specifies which pixels of the icon_pixmap should be used as the
icon,  allowing for non-rectangular icons.
.LP
The icon_window field is the ID of a window the client wants used as its
icon.  Not all window managers will support icon windows,  but those that
do will expect that:
.Ip
The icon window is a child of the (pseudo-) root.
.Ip
The icon window is one of the sizes specified in the WM_ICON_SIZES property
on the (pseudo-) root.
.Ip
The icon window uses the root visual & default colormap for the screen
in question.
.Ip
The client will neither map nor unmap the icon window.  If the client does
map the icon window,  it will be treated as if it had set the initial_state
flag to ClientIconState.  This treatment is not guaranteed to result in the
icon window actually being mapped.
.LP
Clients needing more capabilities from the icons than a simple two-color
bitmap should use icon windows.
.LP
The window_group lets the client specify that this window belongs to a group
of windows.
An example is a single client manipulating multiple 
children of the root window.
Window managers may provide facilities for manipulating the group as 
a whole.
.Cp
The window_group field should be set to the ID of the
group leader.
The window group leader may be a window which exists only for that
purpose,  and may never be mapped.
Its window_group field should contain its own ID.
.Ce
.Cp
The properties of the window group leader are those for the group as
a whole (for example,  the icon to be shown when the entire group
is iconified).
.Ce
.LP
The definitions for the ``messages'' flags are:
.DS L
#define	ConfigureDenied	(1L << 0)	/* WM_CONFIGURE_DENIED */
#define	WindowMoved	(1L << 1)	/* WM_WINDOW_MOVED */
#define	BangMessage	(1L << 2)	/* BANG! */
#define	FocusMessage	(1l << 3)	/* WM_TAKE_FOCUS */
.DE
.LP
The meanings of the bits are:
.Ip
The ConfigureDenied bit enables notification that the window manager has
decided not to change the size or position of the top-level 
window in response to a ConfigureWindow request.
.Ip
The WindowMoved bit enables notification that the window manager has moved
a top-level window without resizing it.
.Ip
The BangMessage bit enables notification that the session manager feels
that termination is likely.
.Ip
The FocusMessage bit announces that the client sets the input
focus to its windows explicitly,  and wants notification of when
the window manager thinks it should do so.
.LP
If the ``messages'' field is not defined,  it defaults to zeros.
.LP
The four input models and the corresponding values of the ``input'' and
FocusMessage bits are shown in Table 3.
.KF
.TS
center, box;
c s s
c c c
l l l.
Table 3 \- Input Models
_
Input Model	input field	FocusMessage
_
No Input	False	False
Passive	True	False
Locally Active	True	True
Globally Active	False	True
.TE
.KE
.NH 4
WM_CLASS
.LP
The WM_CLASS property is a string containing two null-separated elements,
``res_class'' and ``res_name'', that are meant to
be used by clients both as a means of permanent identification, and as
the handles by which which both the client and the window manager obtain
resources related to the window.  "res_class" is meant to identify the
client (e.g. emacs), while "res_name" is meant to more specifically
identify the particular instance.  Resources should be obtained using
"res_name", or if not found under "res_name", then using "res_class".
The fields should be set using the following rules (non-Unix systems will
differ):
.Ip
Set WM_CLASS.res_class to the name of the client (for example,  "emacs").
.Ip
Set WM_CLASS.res_name from the first of the following that applies:
.RS
.Ip
an optional command line argument (i.e. -rn name)
.Ip
a specific environment variable (i.e. RESOURCE_NAME)
.Ip
the trailing component of argv[0]
.RE
.LP
The WM_CLASS property is write-once and must be present when the window is
mapped;  window managers will ignore changes to it while the window is mapped.
If the window is unmapped,  and then re-mapped,  window managers
will normally re-read WM_CLASS.
However,  there should be no need for a client to change its class.
.NH 4
WM_TRANSIENT_FOR
.LP
The WM_TRANSIENT_FOR property is the ID of another top-level window.
The implication is that this window is a pop-up on behalf of the named
window,  and window managers may decide not to decorate transient windows,
or treat them differently in other ways.
Dialogue boxes,  for example,  are an example of windows that should have
WM_TRANSIENT_FOR set.
.LP
It is important not to confuse WM_TRANSIENT_FOR with override-redirect.
WM_TRANSIENT_FOR should be used in those cases where the pointer
is not grabbed while the window is grabbed;  in other words if other windows
are allowed to be active while the transient is up.
If other windows must be frozen,  use override-redirect and grab the
pointer while the window is mapped.
.NH 3
Window Manager Properties
.LP
The properties described above are those which the client is responsible
for maintaining on its top-level windows.
This section describes the properties that the window manager places on
clients top-level windows.
.NH 4
WM_STATE
.LP
The WM_STATE property is composed of two fields:
.Ls
struct {
	int state;
	Window icon;
};
.Le
.LP
The state field can take on some of the same values as the initial_state field 
of the WM_HINTS property.
In particular,  it can be:
.DS 0
.TA .5i 2.5i
.ta .5i 2.5i
#define	NormalState	1
#define	ClientIconState	2
#define	IconicState	3
#define	InactiveState	4
.DE
The icon field should contain the window
ID of the window which the window manager uses as the icon window for the
window on which this property is set,  if any,  otherwise None.
Note that this window may
not be the same as the icon window which the client may specify.
.LP
Clients should be prepared for this property not being set for the
standard reasons (no window manager, badly behaved window manager, broken
window manager).
The state field describes the window manager's idea of
the state the window is in,  which may not match
the client's idea as expressed in the initial_state field
of the WM_HINTS property (for example,  if the user has
asked the window manager to iconify the window).
If it is NormalState,  the window manager believes
the client should be animating its window;
if it is IconicState that it should animate its icon window.
Note that in either state clients should be prepared to handle
exposure events from either window.
.NH 3
Mapping and Unmapping the Window
.LP
Once the top-level window has been provided with suitable
properties,  the client is free to map it and unmap it as
required.
Mapping the window should be interpreted as a request to place
this window under the control of the window manager,  and
unmapping it as a request to remove it from the control of
the window manager.
Typically,  the window manager will intercept these
calls and ``do the right thing''.
.LP
Note that mapping or unmapping the window is a heavyweight
operation,  and that it is \fInot\fP the way to change state
from open to iconic,  and vice versa.
An unmapped window is not under the control of the window
manager,  and its icon will also be unmapped.
.LP
A client receiving UnmapNotify on its (top-level or icon) window
should regard that as notification from the window manager that
animating the window is no longer necessary.  They should cease computing
new states of the window,  and stop sending output to it.
.LP
A client can also select for VisibilityChange on their (top-level
or icon) windows.
They will the receive a VisibilityNotify(state=FullyObscured)
event when the window concerned becomes completely
obscured even though mapped (and thus perhaps a waste
of time to update),  and a VisibilityNotify(state!=FullyObscured)
when it becomes even partly viewable.
.LP
Clients should neither map nor unmap their icon windows.
.NH 3
Configuring the Window
.LP
There are two possible ways in which a client could resize 
or re-position its top-level windows:
.IP
<XXX - Xlib stuff>
.Ip
Use XConfigure().
.Ip
Use XSetNormalHints() to change the size and/or position fields
in the WM_NORMAL_HINTS property.
.LP
Clients wishing to re-size or re-position their top-level
windows should do \fIboth\fP of them,  as follows:
.IP
<XXX - Xlib stuff>
.Ip
Do an XConfigure() with the desired size and location.
.Ip
Do an XSetNormalHints() with the \fIsame\fP size and location.
.LP
The order is important.
The coordinate system in which the location is expressed is that
of the (pseudo-) root,  irrespective of any re-parenting that
may have occurred (in this way clients need not be aware that
they have been reparented).
.LP
Clients must be aware that there is no guarantee that the window manager
will allocate them the requested size or location,  and must be prepared to
deal with \fIany\fP size and location.
If the window manager decides to respond to a ConfigureRequest by:
.Ip
Not changing the size or location of the window at all,  a client
which has requested notification by setting the ConfigureDenied
bit in WM_HINTS
will receive a ClientMessage whose type field is the atom WM_CONFIGURE_DENIED,
and which carries no other data.
.Ip
Moving the window without resizing it,  a client which
has requested notification by setting the WindowMoved bit in
WM_HINTS will
receive a ClientMessage whose type field is the atom WM_WINDOW_MOVED,
and whose data field contains the new (pseudo-) root X and Y.
They will not normally receive a ConfigureNotify event describing this
change,  since the window manager will have re-parented their window.
.Ip
Moving and resizing the window,  a client which has selected for
StructureNotify will receive a ConfigureNotify event.  Note that
the coordinates in this event are relative to the parent,  which
may not be the root in the window has been reparented.
.NH 3
Input Focus
.LP
Clients can,  as described above,  deal with the input focus in four ways:
.Ip
No Input.
.Ip
Passive.
.Ip
Locally Active.
.Ip
Globally Active.
.LP
Passive and Locally Active clients set the ``input'' field of WM_HINTS 
.B True
to indicate that they require window manager assistance  in acquiring the
input focus.  No Input and Globally Active clients set the ``input'' field
.B False
to prevent the window manager setting the input focus to its top-level
window.
.LP
Clients using SetInputFocus must set the ``time'' field to
the timestamp of the event that caused them to make the
attempt.
Note that this cannot be a FocusIn event,  since they do not
have timestamps,  and that clients may acquire the focus
without a corresponding EnterNotify.
Clients must not use CurrentTime in the ``time'' field.
.LP
Clients using the Globally Active model can only use SetInputFocus
to acquire the input focus when they do not already have it on
receipt of one of the following events:
.Ip
ButtonPress
.Ip
ButtonRelease
.Ip
Passive-grabbed KeyPress
.Ip
Passive-grabbed KeyRelease
.LP
In general,  clients should avoid using pasive-grabbed Key events for
this purpose except when they are unavoidable (as for example a selection
tool that establishes a passive grab on the keys that cut,  copy,  or
paste).
.LP
The method by which the user commands the window manager to
set the focus to a window is up to the window manager.
For example,  clients cannot determine whether they
will see the click that transfers the focus.
.LP
Clients which set the FocusMessage bit in WM_HINTS may receive
a ClientMessage from the window manager whose ``type'' field
is the atom WM_TAKE_FOCUS and whose data field is a timestamp.
If they expect input,  they should respond with a SetInputFocus
request with its
``window'' field set to the window of theirs that last had the input
focus,  or to their ``default input window'',
and the ``time'' field set to the timestamp.
.LP
Clients will normally receive WM_TAKE_FOCUS when opening from an icon,
or when the user has clicked outside their window in an area that
indicates to the window manager that it should assign the focus (for example,
clicking in the headline bar can be used to asign the focus).
.LP
Clients that set the input focus need to decide a value for the
``revert-to'' field of the SetInputFocus request.
This determines the behaviour of the input focus if the window
the focus has been set to becomes not viewable.
It can be any of:
.Ip
Parent.  In general,  clients should use this value when assigning focus
to one of their subwindows.  Unmapping the subwindow will cause focus
to revert to the parent,  which is probably what you want.
.Ip
PointerRoot.  Using this value with a ClickToType-style window manager
leads to race conditions,  since the window becoming unviewable may
coincide with the window manager deciding to move the focus elsewhere.
.Ip
None.  Using this value causes problems if the window manager
reparents the window (most window managers will) and then crashes.
The input focus will be None,  and there will probably be no way
to change it.
.LP
There isn't a lot of experience to base a convention on,  but
the tentative convention is:
.Cp
Clients invoking SetInputFocus should set ``revert-to'' to Parent.
.Ce
.LP
A convention is also required for clients that want to give up the
input focus.
.Cp
When a client with the focus wants to give it up,  it should set it to None,
rather than to PointerRoot.
.Ce
.NH 3
Colormaps
.LP
Clients that use one colormap for each top-level window and its sub-windows
should set its ID in the colormap field of the window's attributes,
and depend on the window manager to install it at suitable times.
They should not set a WM_COLORMAPS property on the top-level window.
.LP
If they want to change the colormap,  they should change the window
attribute,  and the window manager will install the colormap for them.
Window managers are responsible for ensuring that top-level windows
colormaps are installed at appropriate times.
.LP
Clients that have sub-windows with different colormap requirements
from their top-level windows should install these (sub-window) colormaps
themselves.
The window manager remains responsible for installing the top-level colormap.
They should set the WM_COLORMAPS property on the top-level window
concerned to a list of the IDs of the colormaps they will be installing,
this informs the window manager that they will do their own
sub-window colormap installation,  and allows the window manager to uninstall
other maps at suitable times.
.LP
Clients,  especially those installing their own colormaps,  should be
aware of the min-installed-maps and max-installed-maps fields of the
connection startup information,  and the effect that the minimum value
has on the ``required list'':
.QP
``At any time, there is a subset of the installed maps, viewed as an
ordered list, called the "required list".  The length of the required
list is at most M, where M is the min-installed-maps specified for the
screen in the connection setup.  The required list is maintained as
follows.  When a colormap is an explicit argument to InstallColormap,
it is added to the head of the list, and the list is truncated at the
tail if necessary to keep the length of the list to at most M.  When a
colormap is an explicit argument to UninstallColormap and it is in the
required list, it is removed from the list.  A colormap is not added to
the required list when it is installed implicitly by the server, and
the server cannot implicitly uninstall a colormap that is in the
required list.''
.LP
In less precise words,  the min-installed-maps most recently installed maps
are guaranteed to be installed.  This number will often be one;  clients
needing multiple colormaps should beware.
.LP
The WM_COLORMAPS property is merely a hint to the window manager,
allowing it to uninstall suitable maps when preparing to install
a top-level window's map.
If it is inconvenient for a client to collect the complete set of
colormaps it will install,  the property can be incomplete (or even
empty).
The only result of an incomplete list is that window manager's
attempts to manage the set of installed maps will in some cases
be less than optimal.
.NH 3
Icons
.LP
A client can hint to the window manager about the desired appearance of its
icon in several ways:
.Ip
Set a string in WM_ICON_NAME.  All clients should do this,  as it provides
a fall-back for window managers whose ideas about icons differ widely from
those of the client.
.Ip
Set a Pixmap into the ``icon_pixmap'' field of the WM_HINTS property,
and possibly another into the ``icon_mask'' field.  The window manager
is expected to display the pixmap masked by the mask.
The pixmap should be one of the sizes found in the WM_ICON_SIZE property
on the root (or pseudo-root).
If this property is not found,  the window manager is unlikely to display
icon pixmaps.
Window managers will normally clip or tile pixmaps which do not match
WM_ICON_SIZE.
.Ip
Set a window into the ``icon_window''
field of the WM_HINTS property.
The window manager is expected to map that window whenever the client is
in IconState or ClientIconState.
If the icon window is not the window itself,  the window manager will
treat the window's WM_NORMAL_SIZE property as a hint of a suitable icon size.
In general,  the size of the icon window should be one of those specified
in WM_ICON_SIZE on the (pseudo-) root,  if it exists.
Window managers are free to resize icon windows.
.LP
.Cp
Clients may ask the window manager to change their state from normal
to iconic and vice versa by setting the ``initial_state'' flag
in the WM_HINTS property.
.Ce
.LP
Clients must not depend on being able to receive input events
via their icon windows.
Window managers will differ as to whether they support this.
.NH 3
Popup Windows
.LP
Clients wishing to pop-up a window can do one of three things:
.Ip
They can create and map another normal top-level window,  which will get
decorated and managed as normal by the window manager.
See the discussion of window groups below.
.Ip
If the window will be visible for a relatively short time,
and deserves a somewhat lighter treatment,
they can set the WM_TRANSIENT_FOR property.
They can expect less decoration,  but can set all the normal
window manager properties on the window.
An example would be a dialog box.
.Ip
If the window will be visible for a very short time, and
should not be decorated at all,  the client can set
override-redirect on the window.
In general,  this should be done only if the pointer is
grabbed while the window is mapped.
The window manager will never interfere with these windows,
which should be used with caution.
An example of an appropriate use is a pop-up menu.
.NH 3
Window Groups
.LP
A client with multiple persistent top-level windows constitutes
a window group,  and its top-level windows should be linked
together using the ``window_group'' field of the WM_HINTS
structure.
.LP
One of the windows (the one the others point to) will be the
group leader and will carry the group as opposed to the
individual properties.
Window managers may treat the group leader differently from
other windows in the group.
For example,  group leaders may have the full set of decorations,
and other group members a restricted set.
.LP
It is not necessary for the group leader ever to be mapped.
.NH 2
Client Responses to Window Manager Actions
.LP
The window manager performs a number of operations on client
resources,  primarily on their top-level windows.
Clients must not try to fight this,  but may elect to
receive notification of the window manager's operations.
.NH 3
Move
.LP
If the window manager moves a top-level window without changing its size,
the client can elect to receive notification by setting
the WindowMoved bit in the WM_HINTS structure.
Notification is via a ClientMessage event whose type field
is WM_WINDOW_MOVED and whose data field contains the new
(pseudo-) root X and Y.
.LP
Clients must not respond to being moved by attempting to move
themselves to a better location.
.NH 3
Resize
.LP
The client can elect to receive notification of being
resized by selecting for StructureNotify on its
top-level window(s).
A ConfigureNotify event on a top-level window implies that the
window's position on the root may have changed,  even though
its position in its parent is unchanged,  because the window
may have been re-parented.  And note that the coordinates in
the event will not,  in this case,  be meaningful.
.LP
The response of the client to being resized should be to accept
the size it has been given,  and to do its best with it.
Clients must not respond to being resized by attempting to resize
themselves to a better size.
If the size is impossible to work with,  clients are free to change
the ``initial_state'' field in their WM_HINTS structure and ask
to be iconified.
.NH 3
(De)Iconify
.LP
Clients can know their open/closed status by examining the WM_STATE
property on their top-level window.
The window manager will set this,  using the same bit definitions
as the ``initial_state'' field in WM_HINTS,  to indicate the state
it believes the window is currently in.
.LP
Clients needing to take action on changing state (other than painting
the window on the opening Expose event) can select for PropertyNotify
and wait for notification of changes to the WM_STATE property.
.LP
If the only reason for wanting to know whether the window is in open
or iconic state is to stop updating it when it is iconic,  the client
should select for StructureNotify on it,  and stop updating the display
when it receives UnmapNotify.
UnmapNotify does not imply that the window is in any particular state,
rather it implies that the window manager believes it is no longer
necessary for the client to update the window.
For example,  consider a window that is open on a pseudo-root that is
unmapped.
.NH 3
Colormap Change
.LP
Clients that wish to be notified of their colormaps being installed
or uninstalled should select for ColormapNotify on their top-level
windows.
They will receive ColormapNotify events with the ``new'' field FALSE
when the colormap for that window is installed or uninstalled.
Window managers will only ever install the colormaps for top-level
windows,  but a side-effect of them doing so (or of other clients
installing sub-window colormaps) may be that other maps are
uninstalled.
.LP
Clients that need to explicitly install colormaps for sub-windows
should do so only when their top-level window has its colormap installed.
.Pp
There is a race condition here;  the InstallColormap request doesn't
take a timestamp,  and it may be executed after the top-level
colormap has been uninstalled.
The next protocol revision should provide the timestamp.
.Pe
.LP
.Cp
Clients that need to install their own colormaps,  and which expect input,
should install them only when they have the input focus and their
top-level colormap is installed.
Clients that need to install their own colormaps,  and which never
expect input,  should install them only when their top-level window
has its colormap installed.
.Ce
.NH 3
Input Focus
.LP
Clients can request notification that they have the input focus
by selecting for FocusChange on their top-level windows;
they will receive FocusIn and FocusOut events.
Clients that need to set the input focus to one of their
sub-windows should not do so unless
they actually have the focus in (one of) their top-level windows.
Clients should not warp the pointer in an attempt to transfer the
focus,  they should set the focus and leave the pointer alone.
.LP
Once a client has the focus in one of its windows,  it may 
transfer it to another of its windows using:
.Ls
SetInputFocus
	focus:		WINDOW or PointerRoot or None
	revert-to:	{Parent, PointerRoot, None}
	time:		TIMESTAMP or CurrentTime
.Le
.LP
.Cp
Clients using SetInputFocus must set the ``time'' field to
the timestamp of the event that caused them to make the
attempt.
Note that this cannot be a FocusIn event,  since they do not
have timestamps,  and that clients may acquire the focus
without a corresponding EnterNotify.
Clients must not use CurrentTime in the ``time'' field.
.Ce
.NH 3
ClientMessages
.LP
Clients may receive the following messages from their
window manager.  All will be events of type ClientMessage,
distinguished by their ``type'' fields being the following
atoms:
.Ip
WM_CONFIGURE_DENIED.
These messages carry no data.
They are enabled by setting the ConfigureDenied bit of the ``messages''
field of the WM_HINTS structure.
.Ip
WM_WINDOW_MOVED.
These messages carry the new X and Y values in root window
coordinates.
They are enabled by setting the WindowMoved bit of the ``messages''
field of the WM_HINTS structure.
.Ip
WM_TAKE_FOCUS.
These messages carry a timestamp.
They are enabled by setting the FocusMessage bit of the WM_HINTS structure.
.NH 2
Status of the Window Manager Conventions
.LP
A number of areas in these conventions are still causing considerable
controversy.  Examples are:
.Ip
Input Focus handling.
.Ip
WM_WINDOW_MOVED.
.Ip
The XOpenDisplay() extension.
.Ip
WM_ICON_SIZES.
.Ip
WM_STATE.
.LP
It will obviously take some time for these conventions to obtain formal
endorsement,  and for the toolkits and clients to be changed to conform.
.NH
Client to Session Manager Communication
.LP
The role of the session manager is to manage a collection of clients.
It should be capable of:
.Ip
Starting a collection of clients as a group.
.Ip
Remembering the state of a collection of clients so that
they can be re-started in the same state.
.Ip
Stopping a collection of clients in a controlled way.
.LP
It may also provide a user interface to starting,  stopping
and re-starting groups of clients.
.NH 2
Client Actions
.LP
Clients need to cooperate with the session manager by providing
it with information it can use to restart them if it should become
necessary,  but need not take any other action to assist it.
.NH 3
Properties
.LP
The client communicates with the session manager using two properties
on its top-level window.
If the client has a group of top-level windows,  these properties
should be placed on the group leader window.
.NH 4
WM_COMMAND
.LP
The WM_COMMAND property is a string representing the command used
to (re-)start the client.
In
.UX
systems,  it will initially be set from \fIargv\fP.
Clients should ensure,  by re-setting this property,  that it
always reflects a command that will restart them in their current state.
.NH 4
WM_CLIENT_MACHINE
.LP
The WM_CLIENT_MACHINE property should be set to a string forming the
name of the machine running the client,  as seen from the machine
running the server.
.NH 3
Termination
.LP
Since they communicate via unreliable network connections,  X11 clients must be
prepared for their connection to the server to be terminated at
any time without warning.
They cannot depend on getting notification that termination is imminent,
nor on being able to use the server to negotiate with the user (for example,
using dialog boxes for confirmation) about their fate.
.LP
Equally,  clients may terminate at any time without notice to the
session manager.
.NH 2
Client Responses to Session Manager Actions
.LP
In general,  the only action that a client need take in response
to the actions of a session manager is to prepare for termination.
.NH 3
Termination
.LP
Clients that wish to be warned of impending termination should set the
BangMessage bit in the WM_HINTS structure.
They will receive a ClientMessage whose type field is BANG! whenever the
session manager believes that termination is likely.
.LP
Clients that do not set the BangMessage bit may be terminated by the
session manager at any time without warning.
.LP
Clients receiving BANG! should place themselves in a state from
which they can be restarted,  and should update WM_COMMAND to
be a command that will restart them in this state.
The session manager will be waiting for a PropertyNotify on WM_COMMAND
as a confirmation that the client has saved its state,  so that
WM_COMMAND should be updated (perhaps with a zero-length append)
even if its contents are correct.
.LP
Once it has received this confirmation,  the session manager will
feel free to terminate the client if that is what the user asked for.
Otherwise,  if the user merely asked for the session to be put to
sleep,  the session manager will ensure that the client does not
receive any mouse or keyboard events.
.LP
Clients should regard BANG! not as a command to terminate,  but rather
as a warning from the session manager that it believes termination
is likely;  as a warning shot rather than one to the heart.
There is no need to panic and commit suicide when the shot arrives,
but clients should be aware that the session manager has more
deadly weapons at its disposal,  and should ensure their last
will and testament is up-to-date,  take out life insurance,
and prepare for resurrection.
.LP
Nevertheless,  a client is always free to terminate without giving
either the session or window managers notice (and in particular
is free to terminate when it receives the BANG! message).
When a client terminates itself,  rather than being terminated by
the session manager,  it is viewed as having resigned from the session
in question,  and it will not be revived if the session is revived.
.LP
After receiving a BANG! and saving its state,  the client should not
change its state until it receives a mouse or keyboard event.
Once it does so,  it can assume that the danger is over.
.NH 2
Status of the Session Manager Conventions
.LP
The question of client termination is still somewhat controversial.
It will obviously take some time for these conventions to obtain formal
endorsement,  and for the toolkits and clients to be changed to conform.
.NH
Manipulation of Shared Resources
.LP
X11 permits clients to manipulate a number of shared resources,
among them the input focus,  the pointer,  and colormaps.
Conventions are required so that clients do so in an
orderly fashion.
.NH 2
The Input Focus
.LP
.Cp
Clients should set the input focus to one of their windows only
when it is already in one of their windows,  or when they receive
a WM_TAKE_FOCUS message.
They should set the ``input'' field of the WM_HINTS structure TRUE.
.Ce
.Cp
Clients should use the timestamp of the event that caused them
to attempt to set the input focus as the ``time'' field on
the SetInputFocus request,  not CurrentTime.
.Ce
.NH 2
The Pointer
.LP
In general, clients should not warp the pointer.  Window managers may
do so,  for example to maintain the invariant that the pointer is always
in the window with the input focus.
Other window managers may wish to preserve the illusion that the user
is in sole control of the pointer.
.Cp
Clients should not warp the pointer.
.Ce
.Cp
Clients which insist on warping the pointer should do so only
with the ``src-window'' field of the WarpPointer request set to one
of their windows.
.Ce
.NH 2
Colormaps
.LP
.Cp
Clients should install their own colormaps only if they have
sub-windows with colormaps that differ from their top-level
window.
.Ce
.Cp
Clients should warn the window manager that they will be installing
their own colormaps by placing a list of the colormaps they will use
in the WM_COLORMAPS property of the top-level window concerned.
.Ce
.Cp
Clients that need to install their own colormaps,  and which expect input,
should install them only when they have the input focus and their
top-level colormap is installed.
Clients that need to install their own colormaps,  and which never
expect input,  should install them only when their top-level window
has its colormap installed.
.Ce
.LP
Clients with DirectColor type applications should consult Section 9.2
of the Xlib manual for conventions connected with sharing standard
colormaps.
.NH
Resource Manager Conventions
.LP
This section has yet to be generated.
.if 0 \{
.IP
<XXX - I haven't started this part yet>
.IP
<XXX - xrdb>
\}
.NH
Conclusion
.SH
Acknowledgements
.LP
The author's role in preparing this document was limited to
asking questions and writing down the answers;  the ideas are
all due to others.
Sources for the questions were the Protocol and Xlib documents,
and the answers came from:
.Ip
For the Selection section,
Jerry Farrell,
Phil Karlton,
and Bob Scheifler.
.Ip
For the Window and Session Manager sections,
Todd Brunhoff,
Ellis Cohen,
Hania Gajewska,
Mark Manasse,
Bob Scheifler,
Raplh Swick,
and the participants in the Window Manager discussions
at the January 1988 X Conference at MIT.
