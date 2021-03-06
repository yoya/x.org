.\" $XConsortium: encoding.ms,v 1.8 94/04/18 14:22:27 rws Exp $
\&
.sp 1
.XS
Appendix A \- Input Extension Protocol Encoding
.XE
.ce 2
.ps 11
.nr PS 11
.ps +2
\fBAppendix A\fP

\fBInput Extension Protocol Encoding\fP
.ps
.sp 2
.LP
.ps 9
.nr PS 9
.vs 10
.nr VS 10
.\"The sections in this appendix correspond to their number counterparts
.\"in the protocol document.
.ps +2
\fBSyntactic Conventions\fP
.ps -3
.LP
All numbers are in decimal, 
unless prefixed with #x, in which case they are in hexadecimal (base 16).
.LP
The general syntax used to describe requests, replies, errors, events, and
compound types is:
.LP
.DS I
\fBNameofThing\fP
  encode-form
  ...
  encode-form
.DE
Each encode-form describes a single component.
.LP
For components described in the protocol as:
.LP
.DS I
name: TYPE
.DE
the encode-form is:
.LP
.DS I
.TA 1i 1.5i 2.5i
.ta 1i 1.5i 2.5i
N	TYPE	name
.DE
N is the number of bytes occupied in the data stream, 
and TYPE is the interpretation of those bytes.
For example,
.LP
.DS I
.TA 1i 1.5i
.ta 1i 1.5i
depth: CARD8
.DE
becomes:
.LP
.DS I
.TA 1i 1.5i 2.5i 
.ta 1i 1.5i 2.5i
1	CARD8	depth
.DE
For components with a static numeric value the encode-form is:
.LP
.DS I
.TA 1i 1.5i 2.5i
.ta 1i 1.5i 2.5i
N	value	name
.DE
The value is always interpreted as an N-byte unsigned integer.
For example, 
the first two bytes of a Window error are always zero (indicating an
error in general) and three (indicating the Window error in particular):
.LP
.DS I
.TA 1i 1.5i 2.5i
.ta 1i 1.5i 2.5i
1	0	Error
1	3	code
.DE
For components described in the protocol as:
.LP
.DS I
name: \fB{Name1, ..., NameI}\fP
.DE
.LP
the encode-form is:
.LP
.DS I
.TA 1i 1.5i 2.5i
.ta 1i 1.5i 2.5i
N		name
	value1 Name1
	...
	valueI NameI
.DE
The value is always interpreted as an N-byte unsigned integer.
Note that the size of N is sometimes larger than that strictly required 
to encode the values.
For example:
.LP
.DS I
class: \fB{InputOutput, InputOnly, CopyFromParent}\fP
.DE
.LP
becomes:
.LP
.DS I
.TA 1i 1.5i 2.5i 3i
.ta 1i 1.5i 2.5i 3i
2			class
	0	CopyFromParent
	1	InputOutput
	2	InputOnly
.DE
For components described in the protocol as:
.LP
.DS I
NAME: TYPE or \fBAlternative1 ... or AlternativeI\fP
.DE
.LP
the encode-form is:
.LP
.DS I
.TA 1i 1.5i 2i 2.5i 3i
.ta 1i 1.5i 2i 2.5i 3i
N	TYPE			NAME
	value1	Alternative1
	...
	valueI	AlternativeI
.DE
The alternative values are guaranteed not to conflict with the encoding
of TYPE.  
For example:
.LP
.DS
destination: WINDOW or \fBPointerWindow\fP or \fBInputFocus\fP
.DE
.LP
becomes:
.LP
.DS I
.TA 1i 1.5i 2.5i
.ta 1i 1.5i 2.5i
4	WINDOW		destination
	0	PointerWindow
	1	InputFocus
.DE
For components described in the protocol as:
.LP
.DS I
.TA 1i 1.5i
.ta 1i 1.5i
value-mask: BITMASK
.DE
the encode-form is:
.LP
.DS I
.TA 1i 1.5i 2i 2.5i 
.ta 1i 1.5i 2i 2.5i
N	BITMASK			value-mask
	mask1	mask-name1
	...
	maskI	mask-nameI
.DE
The individual bits in the mask are specified and named, 
and N is 2 or 4.
The most-significant bit in a BITMASK is reserved for use in defining
chained (multiword) bitmasks, as extensions augment existing core requests.
The precise interpretation of this bit is not yet defined here, 
although a probable mechanism is that a 1-bit indicates that another N bytes 
of bitmask follows, with bits within the overall mask still interpreted 
from least-significant to most-significant with an N-byte unit, with N-byte units 
interpreted in stream order, and with the overall mask being byte-swapped 
in individual N-byte units.
.LP
For LISTofVALUE encodings, the request is followed by a section of the form:
.LP
.DS I
.TA 1i 1.5i
.ta 1i 1.5i
VALUEs
 encode-form
 ...
 encode-form
.DE
listing an encode-form for each VALUE.
The NAME in each encode-form keys to the corresponding BITMASK bit.
The encoding of a VALUE always occupies four bytes, 
but the number of bytes specified in the encoding-form indicates how
many of the least-significant bytes are actually used; 
the remaining bytes are unused and their values do not matter.
.LP
In various cases, the number of bytes occupied by a component will be specified
by a lowercase single-letter variable name instead of a specific numeric
value, and often some other component will have its value specified as a
simple numeric expression involving these variables.
Components specified with such expressions are always interpreted 
as unsigned integers.
The scope of such variables is always just the enclosing request, reply, 
error, event, or compound type structure.
For example:
.LP
.DS I
.TA 1i 1.5i 2i 2.5i
.ta 1i 1.5i 2i 2.5i
2	3+n			request length
4n	LISTofPOINT		points
.DE
For unused bytes (the values of the bytes are undefined and do not matter),
the encode-form is:
.LP
.DS I
.TA 1i 1.5i 2i 2.5i
.ta 1i 1.5i 2i 2.5i
N			unused
.DE
If the number of unused bytes is variable, the encode-form typically is:
.LP
.DS I
.TA 1i 1.5i 2i 2.5i
.ta 1i 1.5i 2i 2.5i
p			unused, p=pad(E)
.DE
where E is some expression,
and pad(E) is the number of bytes needed to round E up to a multiple of four.
.LP
.DS I
.TA 1i 1.5i 2i 2.5i
.ta 1i 1.5i 2i 2.5i
pad(E) = (4 - (E mod 4)) mod 4
.DE
.ps +2
\fBCommon Types\fP
.ps
.LP
LISTofFOO
.IP
In this document the LISTof notation strictly means some number of repetitions
of the FOO encoding; 
the actual length of the list is encoded elsewhere.
.LP
SETofFOO
.IP
A set is always represented by a bitmask, with a 1-bit indicating presence in
the set.
.LP
BITMASK: CARD32
.LP
WINDOW: CARD32
.LP
BYTE: 8-bit value
.LP
INT8: 8-bit signed integer
.LP
INT16: 16-bit signed integer
.LP
INT32: 32-bit signed integer
.LP
CARD8: 8-bit unsigned integer
.LP
CARD16: 16-bit unsigned integer
.LP
CARD32: 32-bit unsigned integer
.LP
TIMESTAMP: CARD32
.LP
EVENTCLASS: CARD32
.LP
.DS 0
.TA .75i 1.75i
.ta .75i 1.75i
INPUTCLASS
	0	KeyClass
	1	ButtonClass
	2	ValuatorClass
	3	FeedbackClass
	4	ProximityClass
	5	FocusClass
	6	OtherClass
.DE
.LP
.DS 0
.TA .75i 1.75i
.ta .75i 1.75i
INPUTCLASS
	0	KbdFeedbackClass
	1	PtrFeedbackClass
	2	StringFeedbackClass
	3	IntegerFeedbackClass
	4	LedFeedbackClass
	5	BellFeedbackClass
.DE
.LP
.DS 0
.TA .75i 1.75i
.ta .75i 1.75i
INPUTINFO
	0	KEYINFO
	1	BUTTONINFO
	2	VALUATORINFO
.DE
.LP
.DS 0
.TA .75i 1.75i
.ta .75i 1.75i
DEVICEMODE
	0	Relative
	1	Absolute
.DE
.LP
.DS 0
.TA .75i 1.75i
.ta .75i 1.75i
PROXIMITYSTATE
	0	InProximity
	1	OutOfProximity
.DE
.LP
.DS 0
.TA .75i 1.75i
.ta .75i 1.75i
BOOL
	0	False
	1	True
.DE
.LP
KEYSYM: CARD32
.LP
KEYCODE: CARD8
.LP
BUTTON: CARD8
.LP
.DS 0
.TA .75i 1.75i
.ta .75i 1.75i
SETofKEYBUTMASK
	#x0001	Shift
	#x0002	Lock
	#x0004	Control
	#x0008	Mod1
	#x0010	Mod2
	#x0020	Mod3
	#x0040	Mod4
	#x0080	Mod5
	#x0100	Button1
	#x0200	Button2
	#x0400	Button3
	#x0800	Button4
	#x1000	Button5
	#xe000	unused but must be zero
.DE
.LP
.DS 0
.TA .75i 1.75i
.ta .75i 1.75i
SETofKEYMASK
	encodings are the same as for SETofKEYBUTMASK, except with
	#xff00		unused but must be zero
.DE
.LP
STRING8: LISTofCARD8
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
STR
	1	n		length of name in bytes
	n	STRING8		name
.DE
.ps +2
\fBErrors\fP
.ps
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
Request
	1	0		Error
	1	1		code
	2	CARD16		sequence number
	4			unused
	2	CARD16		minor opcode
	1	CARD8		major opcode
	21			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
Value
	1	0		Error
	1	2		code
	2	CARD16		sequence number
	4	<32-bits>		bad value
	2	CARD16		minor opcode
	1	CARD8		major opcode
	21			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
Window
	1	0		Error
	1	3		code
	2	CARD16		sequence number
	4	CARD32		bad resource id
	2	CARD16		minor opcode
	1	CARD8		major opcode
	21			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
Match
	1	0		Error
	1	8		code
	2	CARD16		sequence number
	4			unused
	2	CARD16		minor opcode
	1	CARD8		major opcode
	21			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
Access
	1	0		Error
	1	10		code
	2	CARD16		sequence number
	4			unused
	2	CARD16		minor opcode
	1	CARD8		major opcode
	21			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
Alloc
	1	0		Error
	1	11		code
	2	CARD16		sequence number
	4			unused
	2	CARD16		minor opcode
	1	CARD8		major opcode
	21			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
Name
	1	0		Error
	1	15		code
	2	CARD16		sequence number
	4			unused
	2	CARD16		minor opcode
	1	CARD8		major opcode
	21			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
Device
	1	0		Error
	1	CARD8		code
	2	CARD16		sequence number
	4			unused
	2	CARD16		minor opcode
	1	CARD8		major opcode
	21			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
Event
	1	0		Error
	1	CARD8		code
	2	CARD16		sequence number
	4			unused
	2	CARD16		minor opcode
	1	CARD8		major opcode
	21			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
Mode
	1	0		Error
	1	CARD8		code
	2	CARD16		sequence number
	4			unused
	2	CARD16		minor opcode
	1	CARD8		major opcode
	21			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
Class
	1	0		Error
	1	CARD8		code
	2	CARD16		sequence number
	4			unused
	2	CARD16		minor opcode
	1	CARD8		major opcode
	21			unused
.DE
.ps +2
Keyboards
.ps
.LP
KEYCODE values are always greater than 7 (and less than 256).
.LP
KEYSYM values with the bit #x10000000 set are reserved as vendor-specific.
.LP
The names and encodings of the standard KEYSYM values are contained in 
.\"Appendix B, Keysym Encoding.
appendix F.
.LP
.ps +2
Pointers
.ps
.LP
BUTTON values are numbered starting with one.
.LP
.ps +2
Requests
.ps
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GetExtensionVersion
	1	CARD8		input extension opcode
	1	1		GetExtensionVersion opcode
	2	2+(n+p)/4		request length
	2	n		length of name
	2			unused
	n	STRING8		name
	p			unused, p=pad(n)
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1		Reply
	1	1		GetExtensionVersion opcode
	2	CARD16		sequence number
	4	0		reply length
	2	CARD16		major version
	2	CARD16		minor version
	1	BOOL		present
	19			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
ListInputDevices
	1	CARD8		input extension opcode
	1	2		ListInputDevices opcode
	2	1		request length
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1			Reply
	1	2			ListInputDevices opcode
	2	CARD16			sequence number
	4	(n+p)/4			reply length
	1	CARD8			number of input devices
	23				unused
	n	LISTofDEVICEINFO	info for each input device
	p				unused, p=pad(n)
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  DEVICEINFO
	4	CARD32		device type
	1	CARD8		device id
	1	CARD8		number of input classes this device reports
	1	CARD8		device use
			0	IsXPointer
			1	IsXKeyboard
			2	IsXExtensionDevice
	1			unused
	n	LISTofINPUTINFO 		input info for each input class
	m	STR			name
	p				unused, p=pad(m)
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  INPUTINFO	KEYINFO or BUTTONINFO or VALUATORINFO
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  KEYINFO
	1	0		class id
	1	8		length
	1	KEYCODE		minimum keycode
	1	KEYCODE		maximum keycode
	2	CARD16		number of keys
	2			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
BUTTONINFO
	1	1		class id
	1	4		length
	2	CARD16		number of buttons
.DE
.LP
.DS 0
.TA .2i .75i 2.0i 3.0i
.ta .2i .75i 2.0i 3.0i
VALUATORINFO
	1	2		class id
	1	8+12n		length
	1	n		number of axes
	1	SETofDEVICEMODE			mode
	4	CARD32		size of motion buffer
	12n	LISTofAXISINFO		valuator limits
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
AXISINFO
	4	CARD32		resolution
	4	CARD32		minimum value
	4	CARD32		maximum value
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
OpenDevice
	1	CARD8		input extension opcode
	1	3		OpenDevice opcode
	2	2		request length
	1	CARD8		device id
	3			unused
.DE
.DS 0
.TA .2i .5i 1.5i 3.5i
.ta .2i .5i 1.5i 3.5i
 =>
	1	1			Reply
	1	3			OpenDevice opcode
	2	CARD16			sequence number
	4	(n+p)/4			reply length
	1	CARD8			number of input classes
	23				unused
	n	LISTofINPUTCLASSINFO	input class information
	p				unused, p=pad(n)
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  INPUTCLASSINFO
	1	CARD8		input class id
		0	KEY
		1	BUTTON
		2	VALUATOR
		3	FEEDBACK
		4	PROXIMITY
		5	FOCUS
		6	OTHER
	1	CARD8		event type base code for this class
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
CloseDevice
	1	CARD8		input extension opcode
	1	4		CloseDevice opcode
	2	2		request length
	1	CARD8		device id
	3			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
SetDeviceMode
	1	CARD8		input extension opcode
	1	5		SetDeviceMode opcode
	2	2		request length
	1	CARD8		device id
	1	CARD8		mode
	2			unused
.DE
.DS 0
.TA .2i .5i 1.5i 3.5i
.ta .2i .5i 1.5i 3.5i
 =>
	1	1			Reply
	1	5			SetDeviceMode opcode
	2	CARD16			sequence number
	4	0			reply length
	1	CARD8			status
		0		Success
			1	AlreadyGrabbed
		3 + first_error	DeviceBusy
	23				unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
SelectExtensionEvent
	1	CARD8			input extension opcode
	1	6			SelectExtensionEvent opcode
	2	3+n				request length
	4	Window			event window
	2	CARD16			count
	2				unused
	4n	LISTofEVENTCLASS	desired events
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GetSelectedExtensionEvents
	1	CARD8		input extension opcode
	1	7		GetSelectedExtensionEvents opcode
	2	2		request length
	4	Window		event window
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1		Reply
	1	7		GetSelecteExtensionEvents opcode
	2	CARD16		sequence number
	4	n + m  		reply length
	2	n     		this client count
	2	m     		all clients count
	20			unused
	4n	LISTofEVENTCLASS	this client list
	4m	LISTofEVENTCLASS	all clients list
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
ChangeDeviceDontPropagateList
	1	CARD8			input extension opcode
	1	8			ChangeDeviceDontPropagateList opcode
	2	3+n				request length
	4	Window			event window
	2	n			count of events
	1	mode
		0	AddToList
		1	DeleteFromList
	1				unused
	4n	LISTofEVENTCLASS	desired events
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GetDeviceDontPropagateList
	1	CARD8		input extension opcode
	1	9		GetDeviceDontPropagateList opcode
	2	2		request length
	4	Window		event window
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1			Reply
	1	9			GetDeviceDontPropagateList opcode
	2	CARD16			sequence number
	4	n  			reply length
	2	n			count of events
	22				unused
	4n	LISTofEVENTCLASS	don't propagate list
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GetDeviceMotionEvents
	1	CARD8		input extension opcode
	1	10		GetDeviceMotionEvents opcode
	2	4		request length
	4	TIMESTAMP		start
		0	CurrentTime
	4	TIMESTAMP		stop
		0	CurrentTime
	1	CARD8		device id
	3			unused
.DE
.DS 0
.TA .2i .5i 1.5i 3.5i
.ta .2i .5i 1.5i 3.5i
 =>
	1		1		Reply
	1		10	GetDeviceMotionEvents opcode
	2		CARD16	sequence number
	4		(m+1)n	reply length
	4		n		number of DEVICETIMECOORDs in events
	1		m		number of valuators per event
	1		CARD8		mode of the device
			0	Absolute
			1	Relative
	18				unused
	(4m+4)n	LISTofDEVICETIMECOORD	events
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  DEVICETIMECOORD
	4	TIMESTAMP		time
	4m	LISTofINT32		valuators
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
ChangeKeyboardDevice
	1	CARD8		input extension opcode
	1	11		ChangeKeyboardDevice opcode
	2	2		request length
	1	CARD8		device id
	3			unused
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1		Reply
	1	11		ChangeKeyboardDevice opcode
	2	CARD16		sequence number
	4	0		reply length
	1			status
		0	Success
		1	AlreadyGrabbed
		2	DeviceFrozen
	23			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
ChangePointerDevice
	1	CARD8		input extension opcode
	1	12		ChangePointerDevice opcode
	2	2		request length
	1	CARD8		x-axis
	1	CARD8		y-axis
	1	CARD8		device id
	1			unused
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1		Reply
	1	12		ChangePointerDevice opcode
	2	CARD16		sequence number
	4	0		reply length
	1			status
		0	Success
		1	AlreadyGrabbed
		2	DeviceFrozen
	23			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GrabDevice
	1	CARD8		input extension opcode
	1	13		GrabDevice opcode
	2	5+n			request length
	4	WINDOW		grab-window
	4	TIMESTAMP		time
		0	CurrentTime
	2	n		count of events
	1			this-device-mode
		0	Synchronous
		1	Asynchronous
	1			other-devices-mode
		0	Synchronous
		1	Asynchronous
	1	BOOL		owner-events
	1	CARD8		device id
	2			unused
	4n	LISTofEVENTCLASS	event list
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1		Reply
	1	13		GrabDevice opcode
	2	CARD16		sequence number
	4	0		reply length
	1			status
		0	Success
		1	AlreadyGrabbed
		2	InvalidTime
		3	NotViewable
		4	Frozen
	23			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
UngrabDevice
	1	CARD8		input extension opcode
	1	14		UngrabDevice opcode
	2	3		request length
	4	TIMESTAMP		time
		0	CurrentTime
	1	CARD8		device id
	3			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GrabDeviceKey
	1	CARD8		input extension opcode
	1	15		GrabDeviceKey opcode
	2	5+n		request length
	4	WINDOW		grab-window
	2	n		count of events
	2	SETofKEYMASK		modifiers
		#x8000	AnyModifier
	1	CARD8		modifier device
		#x0FF	UseXKeyboard
	1	CARD8		grabbed device
	1	KEYCODE			key
		0	AnyKey
	1			this-device-mode
		0	Synchronous
		1	Asynchronous
	1			other-devices-mode
		0	Synchronous
		1	Asynchronous
	1	BOOL		owner-events
	2			unused
	4n	LISTofEVENTCLASS	event list
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
UngrabDeviceKey
	1	CARD8		input extension opcode
	1	16		UngrabDeviceKey opcode
	2	4		request length
	4	WINDOW		grab-window
	2	SETofKEYMASK		modifiers
		#x8000	AnyModifier
	1	CARD8		modifier device
		#x0FF	UseXKeyboard
	1	KEYCODE			key
		0	AnyKey
	1	CARD8		grabbed device
	3			unused

.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GrabDeviceButton
	1	CARD8		input extension opcode
	1	17		GrabDeviceButton opcode
	2	5+n		request length
	4	WINDOW		grab-window
	1	CARD8		grabbed device
	1	CARD8		modifier device
		#x0FF	UseXKeyboard
	2	n		count of desired events
	2	SETofKEYMASK		modifiers
	1			this-device-mode
		0	Synchronous
		1	Asynchronous
	1			other-device-mode
		0	Synchronous
		1	Asynchronous
	1	BUTTON		button
		0	AnyButton
	1	BOOL		owner-events
		#x8000	AnyModifier
	2			unused
	4n	LISTofEVENTCLASS	event list
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
UngrabDeviceButton
	1	CARD8		input extension opcode
	1	18		UngrabDeviceButton opcode
	2	4		request length
	4	WINDOW		grab-window
	2	SETofKEYMASK		modifiers
		#x8000	AnyModifier
	1	CARD8		modifier device
		#x0FF	UseXKeyboard
	1	BUTTON		button
		0	AnyButton
	1	CARD8		grabbed device
	3			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
AllowDeviceEvents
	1	CARD8		input extension opcode
	1	19		AllowDeviceEvents opcode
	2	3		request length
	4	TIMESTAMP		time
		0	CurrentTime
	1			mode
		0	AsyncThisDevice
		1	SyncThisDevice
		2	ReplayThisDevice
		3	AsyncOtherDevices
		4	AsyncAll
		5	SyncAll
	1	CARD8		device id
	2			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GetDeviceFocus
	1	CARD8		input extension opcode
	1	20		GetDeviceFocus opcode
	2	2		request length
	1	CARD8		device
	3			unused
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1		Reply
	1	20		GetDeviceFocus opcode
	2	CARD16		sequence number
	4	0		reply length
	4	WINDOW		focus
		0	None
		1	PointerRoot
		3	FollowKeyboard
	4	TIMESTAMP	focus time
	1			revert-to
		0	None
		1	PointerRoot
		2	Parent
		3	FollowKeyboard
	15			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
SetDeviceFocus
	1	CARD8		input extension opcode
	1	21		SetDeviceFocus opcode
	2	4		request length
	4	WINDOW		focus
		0	None
		1	PointerRoot
		3	FollowKeyboard
	4	TIMESTAMP		time
		0	CurrentTime
	1			revert-to
		0	None
		1	PointerRoot
		2	Parent
		3	FollowKeyboard
	1	CARD8		device
	2			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GetFeedbackControl
	1	CARD8		input extension opcode
	1	22		GetFeedbackControl opcode
	2	2		request length
	1	CARD8		device id
	3			unused
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1		Reply
	1	22		GetFeedbackControl opcode
	2	CARD16		sequence number
	4	m/4			reply length
	2	n		number of feedbacks supported
	22			unused
	m	LISTofFEEDBACKSTATE	feedbacks
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  FEEDBACKSTATE	KBDFEEDBACKSTATE, PTRFEEDBACKSTATE, INTEGERFEEDBACKSTATE, 
  STRINGFEEDBACKSTATE, BELLFEEDBACKSTATE, or LEDFEEDBACKSTATE
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  KBDFEEDBACKSTATE
	1	0		feedback class id
	1	CARD8		id of this feedback
	2	20		length
	2	CARD16		pitch
	2	CARD16		duration
	4	CARD32		led_mask
	4	CARD32		led_values
	1	    		global_auto_repeat
		0	Off
		1	On
	1	CARD8		click
	1	CARD8		percent
	1	    		unused
	32	LISTofCARD8	auto_repeats
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  PTRFEEDBACKSTATE
	1	0		feedback class id
	1	CARD8		id of this feedback
	2	12		length
	2			unused
	2	CARD16		acceleration-numerator
	2	CARD16		acceleration-denominator
	2	CARD16		threshold
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  INTEGERFEEDBACKSTATE
	1	0		feedback class id
	1	CARD8		id of this feedback
	2	16		length
	4	CARD32		resolution
	4	INT32		minimum value
	4	INT32		maximum value
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  STRINGFEEDBACKSTATE
	1	1		feedback class id
	1	CARD8		id of this feedback
	2	4n+8		length
	2	CARD16		max_symbols
	2	n      		number of keysyms supported
	4n	LISTofKEYSYM		key symbols supported
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  BELLFEEDBACKSTATE
	1	1		feedback class id
	1	CARD8		id of this feedback
	2	12		length
	1	CARD8		percent
	3			unused
	2	CARD16		pitch
	2	CARD16		duration
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  LEDFEEDBACKSTATE
	1	1		feedback class id
	1	CARD8		id of this feedback
	2	12		length
	4	CARD32		led_mask
	4	BITMASK		led_values
		#x0001	On
		#x0002	Off
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
ChangeFeedbackControl
	1	CARD8		input extension opcode
	1	23		ChangeFeedbackControl opcode
	2	3+n/4		request length
	4	BITMASK		value-mask (has n bits set to 1)
		#x0001	keyclick-percent
		#x0002	bell-percent
		#x0004	bell-pitch
		#x0008	bell-duration
		#x0010	led
		#x0020	led-mode
		#x0040	key
		#x0080	auto-repeat-mode
		#x0001	string
		#x0001	integer
		#x0001	acceleration-numerator
		#x0002	acceleration-denominator
		#x0004	acceleration-threshold
	1	CARD8		device id
	1	CARD8		feedback class id
	2			unused
	n	FEEDBACKCLASS
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  FEEDBACKCLASS	KBDFEEDBACKCTL, PTRFEEDBACKCTL, INTEGERFEEDBACKCTL, 
  STRINGFEEDBACKCTL, BELLFEEDBACKCTL, or LEDFEEDBACKCTL 
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  KBDFEEDBACKCTL
	1	0		feedback class id
	1	CARD8		id of this feedback
	2	20		length
	1	KEYCODE		key
	1			auto-repeat-mode
		0	Off
		1	On
		2	Default
	1	INT8		key-click-percent
	1	INT8		bell-percent
	2	INT16		bell-pitch
	2	INT16		bell-duration
	4	CARD32		led_mask
	4	CARD32		led_values
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  PTRFEEDBACKCTL
	1	1		feedback class id
	1	CARD8		id of this feedback
	2	12		length
	2			unused
	2	INT16		numerator
	2	INT16		denominator
	2	INT16		threshold
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  STRINGCTL
	1	2		feedback class id
	1	CARD8		id of this feedback
	2	4n+8		length
	2			unused
	2	n		number of keysyms to display
	4n	LISTofKEYSYM		list of key symbols to display
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  INTEGERCTL
	1	3		feedback class id
	1	CARD8		id of this feedback
	2	8		length
	4	INT32		integer to display
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  LEDCTL
	1	4		feedback class id
	1	CARD8		id of this feedback
	2	12		length
	4	CARD32		led_mask
	4	BITMASK		led_values
		#x0001	On
		#x0002	Off
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  BELLCTL
	1	5		feedback class id
	1	CARD8		id of this feedback
	2	8		length
	1	INT8		percent
	3			unused
	2	INT16		pitch
	2	INT16		duration
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GetDeviceKeyMapping
	1	CARD8		input extension opcode
	1	24		GetDeviceKeyMapping opcode
	2	2		request length
	1	CARD8		device
	1	KEYCODE		first-keycode
	1	CARD8		count
	1			unused
.DE
.DS 0
.TA .2i .75i 2.0i 3.0i
.ta .2i .75i 2.0i 3.0i
 =>
	1	1		Reply
	1	24		GetDeviceKeyMapping opcode
	2	CARD16		sequence number
	4	nm		reply length (m = count field from the request)
	1	n		keysyms-per-keycode
	23			unused
	4nm	LISTofKEYSYM		keysyms
.DE
.LP
.DS 0
.TA .2i .75i 2.0i 3.0i
.ta .2i .75i 2.0i 3.0i
ChangeDeviceKeyMapping
	1	CARD8		input extension opcode
	1	25		ChangeDeviceKeyMapping opcode
	2	2+nm		request length
	1	CARD8		device
	1	KEYCODE		first-keycode
	1	m		keysyms-per-keycode
	1	n		keycode-count
	4nm	LISTofKEYSYM		keysyms
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GetDeviceModifierMapping
	1	CARD8		input extension opcode
	1	26		GetDeviceModifierMapping opcode
	2	2		request length
	1	CARD8		device
	3			unused
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1		Reply
	1	26		GetDeviceModifierMapping opcode
	2	CARD16		sequence number
	4	2n		reply length
	1	n		keycodes-per-modifier
	23			unused
	8n	LISTofKEYCODE		keycodes
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
SetDeviceModifierMapping
	1	CARD8		input extension opcode
	1	27		SetDeviceModifier opcode
	2	2+2n		request length
	1	CARD8		device
	1	n		keycodes-per-modifier
	2			unused
	8n	LISTofKEYCODE		keycodes
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1		Reply
	1	27		SetDeviceModifierMapping opcode
	2	CARD16		sequence number
	4	0		reply length
	1			status
		0	Success
		1	Busy
		2	Failed
	23			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GetDeviceButtonMapping
	1	CARD8		input extension opcode
	1	28		GetDeviceButtonMapping opcode
	2	2		request length
	1	CARD8		device
	3			unused
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1		Reply
	1	28		GetDeviceButtonMapping opcode
	2	CARD16		sequence number
	4	(n+p)/4		reply length
	1	n		number of elements in map list
	23			unused
	n	LISTofCARD8			map
	p			unused, p=pad(n)
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
SetDeviceButtonMapping
	1	CARD8		input extension opcode
	1	29		SetDeviceButtonMapping opcode
	2	2+(n+p)/4		request length
	1	CARD8		device
	1	n		length of map
	2			unused
	n	LISTofCARD8			map
	p			unused, p=pad(n)
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1		Reply
	1	29		SetDeviceButtonMapping opcode
	2	CARD16		sequence number
	4	0		reply length
	1			status
		0	Success
		1	Busy
	23			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
QueryDeviceState
	1	CARD8		input extension opcode
	1	30		QueryDeviceState opcode
	2	2		request length
	1	CARD8		device
	3			unused
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
 =>
	1	1			Reply
	1	30			QueryDeviceState opcode
	2	CARD16			sequence number
	4	m/4			reply length
	1	n     	 		number of input classes
	23				unused
	m	LISTofINPUTSTATE
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  INPUTSTATE	KEYSTATE or BUTTONSTATE or VALUATORSTATE
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  KEYSTATE
	1	CARD8		key input class id
	1	36		length
	1	CARD8		num_keys
	1			unused
	32	LISTofCARD8		status of keys
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  BUTTONSTATE
	1	CARD8		button input class id
	1	36		length
	1	CARD8		num_buttons
	1			unused
	32	LISTofCARD8			status of buttons
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
  VALUATORSTATE
	1	CARD8		valuator input class id
	1	4n + 4		length
	1	n		number of valuators
	1			mode
		#x01	DeviceMode (0 = Relative, 1 = Absolute)
		#x02	ProximityState (0 = InProximity, 1 = OutOfProximity)
	4n	LISTofCARD32		status of valuators
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
SendExtensionEvent
	1	CARD8			input extension opcode
	1	31			SendExtensionEvent opcode
	2	4 + 8n + m		request length
	4	WINDOW			destination
	1	CARD8			device
	1	BOOL			propagate
	2	CARD16			eventclass count
	1	CARD8			num_events
	3				unused
	32n	LISTofEVENTS		events to send
	4m	LISTofEVENTCLASS	desired events
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceBell
	1	CARD8		input extension opcode
	1	32		DeviceBell opcode
	2	2		request length
	1	CARD8		device id
	1	CARD8		feedback id
	1	CARD8		feedback class
	1	INT8		percent
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
SetDeviceValuators
	1	CARD8		input extension opcode
	1	33		SetDeviceValuators opcode
	2	2 + n		request length
	1	CARD8		device id
	1	CARD8		first valuator
	1	n    		number of valuators
	1			unused
	4n	LISTofINT32	valuator values to set
.DE
.DS 0
.TA .2i .5i 1.5i 3.5i
.ta .2i .5i 1.5i 3.5i
 =>
	1	1			Reply
	1	33			SetDeviceValuators opcode
	2	CARD16			sequence number
	4	0			reply length
	1	CARD8			status
		0	Success
		1	AlreadyGrabbed
	23				unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
GetDeviceControl
	1	CARD8		input extension opcode
	1	34		GetDeviceControl opcode
	2	2		request length
	2	CARD16		device control type
	1	CARD8		device id
	1			unused
.DE
.DS 0
.TA .2i .5i 1.5i 3.5i
.ta .2i .5i 1.5i 3.5i
 =>
	1	1			Reply
	1	34			GetDeviceControl opcode
	2	CARD16			sequence number
	4	n/4			reply length
	1	CARD8			status
		0			Success
		1			AlreadyGrabbed
		3 + first_error		DeviceBusy
	23				unused
	n				DEVICESTATE
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DEVICESTATE			DEVICERESOLUTIONSTATE
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DEVICERESOLUTIONSTATE
 	2	0		control type
 	2	8 + 12n		length
 	4	n    		num_valuators
 	4n	LISTOfCARD32	resolution values
 	4n	LISTOfCARD32	resolution min_values
 	4n	LISTOfCARD32	resolution max_values
.DE
.LP
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
ChangeDeviceControl
 	1	CARD8		input extension opcode
 	1	35		ChangeDeviceControl opcode
 	2	2+n/4		request length
 	2	CARD16		control type
 	1	CARD8		device id
 	1			unused
 	n			DEVICECONTROL
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DEVICECONTROL		DEVICERESOLUTIONCTL
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DEVICERESOLUTIONCTL
 	2	1		control type
 	2	8 + 4n		length
 	1	CARD8		first_valuator
 	1	n    		num_valuators
 	2			unused
 	4n	LISTOfCARD32	resolution values
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 3.5i
.ta .2i .5i 1.5i 3.5i
 =>
 	1	1			Reply
 	1	35			ChangeDeviceControl opcode
 	2	CARD16			sequence number
 	4	0			reply length
 	1	CARD8			status
 		0			Success
 		1			AlreadyGrabbed
 		3 + first_error		DeviceBusy
 	23				unused
.DE
.ps +2
Events
.ps
.LP
DeviceKeyPress, DeviceKeyRelease, DeviceButtonPress, DeviceButtonRelease,
ProximityIn, ProximityOut, and DeviceStateNotify  events may be followed by 
zero or more DeviceValuator events.  DeviceMotionNotify events will be 
followed by one or more DeviceValuator events.
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceValuator
	1	CARD8		code
	1	CARD8		device id
	2	CARD16		sequence number
	2	SETofKEYBUTMASK		state
	1	n		number of valuators this device reports
	1	m		number of first valuator in this event
	24	LISTofINT32	valuators
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceKeyPress
	1	CARD8		code
	1	KEYCODE		detail
	2	CARD16		sequence number
	4	TIMESTAMP		time
	4	WINDOW		root
	4	WINDOW		event
	4	WINDOW		child
		0	None
	2	INT16		root-x
	2	INT16		root-y
	2	INT16		event-x
	2	INT16		event-y
	2	SETofKEYBUTMASK		state
	1	BOOL		same-screen
	1	CARD8		device id
		#x80		MORE_EVENTS follow
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceKeyRelease
	1	CARD8		code
	1	KEYCODE		detail
	2	CARD16		sequence number
	4	TIMESTAMP		time
	4	WINDOW		root
	4	WINDOW		event
	4	WINDOW		child
		0	None
	2	INT16		root-x
	2	INT16		root-y
	2	INT16		event-x
	2	INT16		event-y
	2	SETofKEYBUTMASK			state
	1	BOOL		same-screen
	1	CARD8		device id
		#x80		MORE_EVENTS follow
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceButtonPress
	1	CARD8		code
	1	BUTTON		detail
	2	CARD16		sequence number
	4	TIMESTAMP		time
	4	WINDOW		root
	4	WINDOW		event
	4	WINDOW		child
		0	None
	2	INT16		root-x
	2	INT16		root-y
	2	INT16		event-x
	2	INT16		event-y
	2	SETofKEYBUTMASK		state
	1	BOOL		same-screen
	1	CARD8		device id
		#x80		MORE_EVENTS follow
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceButtonRelease
	1	CARD8		code
	1	BUTTON		detail
	2	CARD16		sequence number
	4	TIMESTAMP		time
	4	WINDOW		root
	4	WINDOW		event
	4	WINDOW		child
		0	None
	2	INT16		root-x
	2	INT16		root-y
	2	INT16		event-x
	2	INT16		event-y
	2	SETofKEYBUTMASK		state
	1	BOOL		same-screen
	1	CARD8		device id
		#x80		MORE_EVENTS follow
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceMotionNotify
	1	CARD8		code
	1			detail
		0	Normal
		1	Hint
	2	CARD16		sequence number
	4	TIMESTAMP		time
	4	WINDOW		root
	4	WINDOW		event
	4	WINDOW		child
	 	0	None
	2	INT16		root-x
	2	INT16		root-y
	2	INT16		event-x
	2	INT16		event-y
	2	SETofKEYBUTMASK		state
	1	BOOL		same-screen
	1	CARD8		device id
		#x80		MORE_EVENTS follow
.DE
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceFocusIn
	1	CARD8		code
	1			detail
		0	Ancestor
		1	Virtual
		2	Inferior
		3	Nonlinear
		4	NonlinearVirtual
		5	Pointer
		6	PointerRoot
		7	None
	2	CARD16		sequence number
	4	TIMESTAMP		time
	4	WINDOW		event
	1			mode
		0	Normal
		1	Grab
		2	Ungrab
		3	WhileGrabbed
	1	CARD8		device id
	18			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceFocusOut
	1	CARD8		code
	1			detail
		0	Ancestor
		1	Virtual
		2	Inferior
		3	Nonlinear
		4	NonlinearVirtual
		5	Pointer
		6	PointerRoot
		7	None
	2	CARD16		sequence number
	4	TIMESTAMP		time
	4	WINDOW		event
	1			mode
		0	Normal
		1	Grab
		2	Ungrab
		3	WhileGrabbed
	1	CARD8		device id
	18			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
ProximityIn
	1	CARD8		code
	1			unused
	2	CARD16		sequence number
	4	TIMESTAMP		time
	4	WINDOW		root
	4	WINDOW		event
	4	WINDOW		child
		0	None
	2	INT16		root-x
	2	INT16		root-y
	2	INT16		event-x
	2	INT16		event-y
	2	SETofKEYBUTMASK			state
	1	BOOL		same-screen
	1	CARD8		device id
		#x80		MORE_EVENTS follow
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
ProximityOut
	1	CARD8		code
	1			unused
	2	CARD16		sequence number
	4	TIMESTAMP		time
	4	WINDOW		root
	4	WINDOW		event
	4	WINDOW		child
		0	None
	2	INT16		root-x
	2	INT16		root-y
	2	INT16		event-x
	2	INT16		event-y
	2	SETofKEYBUTMASK			state
	1	BOOL		same-screen
	1	CARD8		device id
		#x80		MORE_EVENTS follow
.DE
.LP
DeviceStateNotify events may be immediately followed by zero or one
DeviceKeyStateNotify and/ or zero or more DeviceValuator events.
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceStateNotify
	1	CARD8		code
	1	CARD8		device id
		#x80		MORE_EVENTS follow
	2	CARD16		sequence number
	4	TIMESTAMP		time
	1	CARD8		num_keys
	1	CARD8		num_buttons
	1	CARD8		num_valuators
 	1	CARD8		valuator mode and input classes reported
 		#x01	reporting keys
 		#x02	reporting buttons
 		#x04	reporting valuators
 		#x40	device mode (0 = Relative, 1 = Absolute)
 		#x80	proximity state (0 = InProximity, 1 = OutOfProximity)
	4	LISTofCARD8		first 32 keys (if reported)
	4	LISTofCARD8		first 32 buttons (if reported)
	12	LISTofCARD32		first 3 valuators (if reported)
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceKeyStateNotify
	1	CARD8		code
	1	CARD8		device id
		#x80		MORE_EVENTS follow
	2	CARD16		sequence number
	28	LISTofCARD8		state of keys 33-255
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceButtonStateNotify
	1	CARD8		code
	1	CARD8		device id
		#x80		MORE_EVENTS follow
	2	CARD16		sequence number
	28	LISTofCARD8		state of buttons 33-255
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceValuator
	1	CARD8		code
	1	CARD8		device id
	2	CARD16		sequence number
	2	SETofKEYBUTMASK		state
	1	n		number of valuators this device reports
	1	n		number of first valuator in this event
	24	LISTofINT32	valuators
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
DeviceMappingNotify
	1	CARD8		code
	1	CARD8		device id
	2	CARD16		sequence number
	1			request
		0	MappingModifier
		1	MappingKeyboard
		2	MappingPointer
	1	KEYCODE		first-keycode
	1	CARD8		count
	1			unused
	4	TIMESTAMP		time
	20			unused
.DE
.LP
.DS 0
.TA .2i .5i 1.5i 2.5i
.ta .2i .5i 1.5i 2.5i
ChangeDeviceNotify
	1	CARD8		code
	1	CARD8		id of device specified on change request
	2	CARD16		sequence number
	4	TIMESTAMP		time
	1			request
		0	NewPointer
		1	NewKeyboard
	23			unused
.DE
.bp
.PX
