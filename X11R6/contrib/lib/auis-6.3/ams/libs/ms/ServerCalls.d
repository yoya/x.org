\begindata{text,537620304}
\textdsversion{12}
\template{default}
\define{italic
menu:[Font,Italic]
attr:[FontFace Italic Int Set]}
\define{bold
menu:[Font,Bold]
attr:[FontFace Bold Int Set]}
\define{chapter
menu:[Title,Chapter]
attr:[Justification LeftJustified Point 0]
attr:[FontFace Bold Int Set]
attr:[FontSize PreviousFontSize Point 4]}
\define{section
menu:[Title,Section]
attr:[Justification LeftJustified Point 0]
attr:[FontFace Bold Int Set]
attr:[FontSize PreviousFontSize Point 2]}
\define{subsection
menu:[Title,Subsection]
attr:[Justification LeftJustified Point 0]
attr:[FontFace Bold Int Set]}
\define{paragraph
menu:[Title,Paragraph]
attr:[Justification LeftJustified Point 0]
attr:[FontFace Italic Int Set]}
\define{bigger
menu:[Font,Bigger]
attr:[FontSize PreviousFontSize Point 2]}
\define{indent
menu:[Region,Indent]
attr:[LeftMargin LeftMargin Inch 32768]
attr:[RightMargin RightMargin Inch 32768]}
\define{typewriter
menu:[Font,Typewriter]
attr:[FontFace FixedFace Int Set]
attr:[FontFamily AndyType Int 0]}
\define{display
menu:[Region,Display]
attr:[LeftMargin LeftMargin Inch 32768]
attr:[RightMargin RightMargin Inch 32768]
attr:[Justification LeftJustified Point 0]}
\define{example
menu:[Region,Example]
attr:[LeftMargin LeftMargin Inch 32768]
attr:[Justification LeftJustified Point 0]
attr:[FontFace FixedFace Int Set]
attr:[FontFamily AndyType Int 0]}
\define{description
menu:[Region,Description]
attr:[LeftMargin LeftMargin Inch 32768]
attr:[Indent LeftEdge Inch -32768]}
\define{quotation
menu:[Region,Quotation]
attr:[LeftMargin LeftMargin Inch 32768]
attr:[RightMargin RightMargin Inch 32768]
attr:[FontFace Italic Int Set]}
\define{subscript
menu:[Font,Subscript]
attr:[Script PreviousScriptMovement Point 2]
attr:[FontSize PreviousFontSize Point -2]}
\define{superscript
menu:[Font,Superscript]
attr:[Script PreviousScriptMovement Point -6]
attr:[FontSize PreviousFontSize Point -2]}
\define{smaller
menu:[Font,Smaller]
attr:[FontSize PreviousFontSize Point -2]}
\define{heading
menu:[Title,Heading]
attr:[LeftMargin LeftMargin Inch -13107]
attr:[Justification LeftJustified Point 0]
attr:[FontFace Bold Int Set]}
\define{majorheading
menu:[Title,MajorHeading]
attr:[Justification Centered Point 0]
attr:[FontSize PreviousFontSize Point 4]}
\define{formatnote
menu:[Region,FormatNote]
attr:[Flags PassThru Int Set]}
\define{subheading
menu:[Title,Subheading]
attr:[Justification LeftJustified Point 0]
attr:[FontFace Bold Int Set]}
\define{center
menu:[Justify,Center]
attr:[Justification Centered Point 0]}
\define{flushleft
menu:[Justify,FlushLeft]
attr:[Justification LeftJustified Point 0]}
\define{flushright
menu:[Justify,FlushRight]
attr:[Justification RightJustified Point 0]}
\define{leftindent
menu:[Region,LeftIndent]
attr:[LeftMargin LeftMargin Inch 32768]}
\center{\majorheading{The Andrew Message System Server-Client Interface

}
Nathaniel S. Borenstein

CMU-ITC

}
\heading{Introduction}


This paper describes the server-client interface of the Andrew Message System. 
 It presumes a basic familiarity with the normal use and function of the 
system.  For such information, please consult the introductory paper [1] or 
the user documentation [2].


\heading{Basic Concepts}


The central concept in the design of the Andrew Message System is the 
client-server split.  The \italic{message server} is a process which runs on 
an Andrew workstation (attached to the Vice file system, if the machine is at 
a Vice site), and performs various operations on a message database.  It 
performs these actions in response to explicit remote procedure calls from a 
\italic{client} program, such as the cui or messages programs.  Clients never 
access the database directly, even if they are running on an Andrew 
workstation; all database access is channeled through the message server. 
 This allows client programs to run on a wide variety of machines (such as IBM 
PC's) not necessarily connected to the file system.


The communication between the client and server consists of a set of 
approximately forty remote procedure calls, which permit various operations on 
structures defined in a shared header file, "AMS.h".  The structures defined 
in AMS.h are the portable representations of the database; they do not 
necessarily have any relation to the actual structures stored in the file 
system, but are translated by the message server.  It is worth noting that all 
RPC calls in the message system use fixed size buffers; thus certain calls 
need to be repeated in order to get a complete answer.  It is also worth 
noting that all calls are designed to be idempotent, and that the architecture 
is designed so that a server can die at any moment, with the client 
automatically reconnecting and continuing its work without any loss of 
context.  These goals are nearly completely met, with a few exceptions that 
will be noted as necessary.


The most basic entity in the message system is, naturally, a \italic{message}. 
 A message is simply an ASCII string of unbounded length, formatted as a mail 
message according to then Internet standard, RFC 822 [3].  As explained in 
that standard, each message contains a set of \italic{headers} (which may be 
viewed as attribute/value pairs, and can be retrieved as such by the client 
from the message server), and a \italic{body}, which is the text of the 
message.


Messages are grouped together in \italic{message directories}.  A message 
directory is simply a collection of messages and other message directories. 
 The structure of messages and directories corresponds closely to the UNIX 
structure of files and directories [4].


Within a message directory, several different pieces of information are stored 
regarding the individual messages within that directory.  This information is 
collected together in a single fixed-length structure called a 
\italic{snapshot}.  Snapshots are the structure most commonly exchanged 
between servers and clients; they contain all of the "fast" information, that 
is, all of the information which can be obtained more quickly than the message 
itself.  Snapshots consist of seven sub-structures, known as the caption, id, 
date, attributes, chain, midhash, and replyhash.  


Because different machines do not represent structures identically, message 
snapshots are NOT passed as structures.  Rather, a snapshot is passed as a 
byte array of fixed size, and its components are accessed via 
machine-independent macros, as described below.  (The include file AMS.h is 
required here.  It is automatically included by CUI.h, for clients, and by 
MS.h, for the messageserver itself.)


The \italic{caption} is a one line summary of the message, to be shown, for 
example, to a user to help him decide whether or not to view the body of the 
message.  It most typically contains the date, subject, and sender of the 
message, separated by TAB characters, but this formatting is *not* required, 
and different applications may put entirely different information into the 
caption.  However, the snapshot should be an ASCII string.  The caption is 
accessed as AMS_CAPTION(Snapshot).


The \italic{id} is a unique identifier that is associated with the message. 
  No two messages will ever be given the same ID, provided a few assumptions 
about the world are met.  (For example, provided that new two machines ever 
have the same internet address, and that the same machine never runs with its 
clock at the same time twice, and so on.)  If the same message appears in 
multiple directories, should have the same id.  The id is an 18-digit ASCII 
string which is encoded in a base-64 alphabet defined in the message system 
code.  The id is accessed as AMS_ID(Snapshot).


The \italic{date} is an ordering field, by which messages in a directory are 
ordered.  As the name suggests, this is usually a date, but an application 
could conceivably use a different ordering criterion.  The date is a 6 
character ASCII string, conventionally a UNIX date encoded in base 64.  This 
is accessed as AMS_DATE(Snapshot).


The \italic{attributes} of a message is a set of bits encoding certain facts 
about the message.  Specifically, each bit encodes whether a certain 
"attribute" of a message is true or not.  For example, there is a bit that 
encodes whether or not the message is marked as "deleted".  This is accessed 
as AMS_ATTRIBUTES(Snapshot).  However, there are also macros for picking out 
(and setting) individual attributes, e.g. AMS_GET_ATTRIBUTE(Snapshot, 
AMS_ATT_DELETED) (or AMS_SET_ATTRIBUTE(...)).  The list of defined attribute 
values is in AMS.h.  This macro is also used for accessing user-defined 
attributes by number.  For user defined attribute #i, you would use macros 
like AMS_GET_ATTRIBUTE(Snapshot, AMS_ATT_UATTR(i)).  To map the attribute 
numbers to names within a given folder, there are specialized messageserver 
calls.  Other macros defined in AMS.h useful for attributes include 
AMS_NUM_UATTRS, which is the number of user-defined attributes available, and 
AMS_ATTRNAMEMAX, which is the maximum length of the name of a user-defined 
attribute.


The \italic{chain} of a message is a network-byte-order integer that can be 
used to find "related" messages within a folder.  If one message is clearly 
marked (by In-Reply-To or References headers) as being a reply to another, 
then they should have the same chain value.  The chain is accessed as 
AMS_CHAIN(Snapshot).  Although it is in network byte order, this is probably 
irrelevant to the client, who really only needs to check for equality of chain 
values.  Typical code for accessing the chain looks something like this, 
unfortunately:


\example{bcopy(AMS_CHAIN(SBuf), &refint, sizeof(unsigned long));

ThisChain = ntohl(refint);

}
The midhash and replyhash are also network-byte-order integers.  They are hash 
values for the MessageID and In-ReplyTo/References headers, with a special bit 
to flag duplicate references.  In general, they should be ignored by the 
client, as they are used only by the messageserver in constructing and 
maintaining the CHAIN field.  They are included in the visible Snapshot 
structure only as an extreme convenience to the messageserver, not as good 
software engineering practice.   Clients should leave them alone.


\heading{Message Server Routines Available to Clients}


There are approximately forty message server routines that may be called via 
RPC from the client program.  Some of these are intended to be called 
frequently by application programs, while others are intended primarily for 
the use of the CUI library; in general, application programs should call CUI 
library routines instead of MS routines in several cases, as there are extra 
things going on there involving caching and abstractions of context.


In the discussion below, the message server calls have been separated into 
seven categories.  These are for the calls that relate to information about 
messages, calls that relate to information about message directories, calls 
that relate to mail delivery, calls that relate to remote file access, calls 
that relate to personal subscription/profile information, calls primarily for 
debugging purposes, and miscellaneous calls.


\leftindent{\description{\italic{A note about convention}:  For each 
parameter, I have appended a comment to indicate whether the parameter is a 
value passed in to the server, a result passed back, or something that has a 
value passed in which is modified and passed back.  These are flagged by /* IN 
*/, /* OUT */, and /* INOUT */, respectively.}}


\description{\leftindent{\italic{A note about return codes}:  All message 
server routines return zero upon successfull completion.  If an error occurs, 
they will return a non-zero long integer error code, which actually consists 
of four separate error-encoding bytes packed together via macros defined in 
the include file mserrno.h.  These can be pulled apart with other macros to 
obtain four separate error codes:


\indent{\bold{AMS_ERRNO}: The actual error code.  This will be either a value 
from the standard unix errno.h include file, or from the extended set of 
errors with prefix "EMS" in mserrno.h.  These constants may be mapped on to 
short explanatory strings using the standard UNIX libarry's character array 
sys_errlist[], or, if the values are greater than or equal to EMSBASE and less 
than or equal to EMSLASTERROR, in the character array ms_errlist[].


\bold{AMS_ERRCAUSE}:  The local context in which the error occurred, typically 
a UNIX system call.  These codes are defined by constants in mserrno.h 
prefixed by "EIN_", and may be mapped onto explanatory strings in the 
character array ms_errcauselist[] as long as they are less than or equal to 
EIN_LASTERRORLOCATION.


\bold{AMS_ERRVIA}:  The larger context in which the error occurred, typically 
a message server top-level routine.  These codes are defined by constants in 
mserrno.h prefixed by "EVIA_", and may be mapped onto explanatory strings in 
the character array ms_errivalist[] as long as they are less than or equal to 
EVIA_LASTERROR.


\bold{AMS_RPCERRNO}:  The error code for errors in SNAP remote procedure 
calls.  If AMS_RPCERRNO is non-zero, the other three error values are not 
trustworthy and should be ignored.  These codes are defined by constants in 
mserrno.h prefixed by "RPC_", and may be mapped onto explanatory strings in 
the character array rpc_errlist[] as long as they are less than or equal to 
RPC_LASTERROR.

}
Each client program needs to provide a routine called ReportError, which 
decodes a message server error code and presents the error information to the 
user.  For a better understanding of message server error codes, consult the 
ReportError routines for the cui or messages programs.


By convention, all clients set the global variable "mserrcode" to the result 
of each message server call.   That is, each message server call is performed 
by a statement of the form "mserrcode = MS_WHATEVER(...)".  The error 
reporting routines assume that the mserrcode variable is relevant if the 
'Decode' parameter is set -- see the CUI library documentation for details.

}}

\heading{Calls that relate to information about messages}


\typewriter{\smaller{MS_HeadersSince(FullDirName, datefield, ReturnBuf, 
MaxReturn, startbyte, numbytes, bytesleft)

char *FullDirName, /* IN */

     *datefield, /* IN */

     *ReturnBuf; /* OUT */

int   MaxReturn, /* IN */

     startbyte, /* IN */

     *numbytes, /* OUT */

     *bytesleft; /* OUT */}}


This routine, which properly should be called MS_SnapshotsSince, is used to 
retrieve a set of snapshots from the message server.  Specifically, it is used 
to retrieve snapshots from the directory named with a full path name in the 
FullDirName parameter for which the ordering date is greater than the date 
specified in the datefield parameter.   The datefield parameter is a six byte, 
null-terminated string encoding the UNIX date in base 64.  The message server 
will pack snapshots into ReturnBuf, packing at most MaxReturn bytes.  It will 
ignore an initial number of bytes specified in startbyte.  Note that MaxReturn 
and startbyte should be multiples of AMS_SNAPSHOTSIZE, as defined in AMS.h. 
 The message server will place the number of bytes that were actually packed 
in the numbytes parameter, and the number of bytes remaining beyond what was 
packed in the bytesleft parameter.  This routine is intended to be called 
repeatedly, with increasing values for startbyte, until the returned bytesleft 
is zero.


\typewriter{\smaller{MS_GetPartialBody(DirName, id, Buf, BufLim, offset, 
remaining, ct)

char *DirName, /* IN */

     *id, /* IN */

     *Buf; /* OUT */

int   BufLim, /* IN */

      offset, /* IN */

     *remaining, /* OUT */

     *ct; /* OUT */}}


The CUI_GetPartialBody routine is preferred for client interfaces.


This routine is used to get the body of a message from the server.  The 
message is identified by a directory name (DirName) and an id (id).  (The id 
was presumably obtained by the client via an earlier call to MS_HeadersSince, 
but clients need not maintain the entire snapshot in order to keep a "handle" 
on the message; if they keep the directory name and ID, they can retrieve any 
information they need regarding the message.)  Part of the body (at most 
BufLim bytes) will be packed into the return buffer (Buf),  starting at the 
specified byte position (offset).  The server will place a count of the number 
of bytes returned in the ct parameter, and the number of bytes remaining after 
the portion returned in the remaining parameter.


\typewriter{\smaller{MS_GetSnapshot(dirname, id, SnapshotBuf) 

char *dirname, /* IN */

     *id; /* IN */

char *SnapshotBuf; /* OUT */}}


The CUI_GetSnapshotFromCUID call is preferred for client interfaces.


This routine takes a directory name and id and returns the complete snapshot. 
 SnapshotBuf should be big enough to hold a snapshot, as specified in 
AMS_SNAPSHOTSIZE in AMS.h.


\smaller{\typewriter{MS_GetNthSnapshot(DirName, n, SnapshotBuf)

char *DirName, /* IN */

        *SnapshotBuf; /* OUT */

int n; /* IN */}}


This call will get a particular snapshot from the message server; 
specifically, it will get the n\superscript{th} snapshot in the named 
directory.  SnapshotBuf should be at least as big as AMS_SNAPSHOTSIZE. If N is 
-1, the last snapshot in the folder will be returned.



\smaller{\typewriter{MS_GetHeaderContents(dirname, id, HeaderName, 
HeaderTypeNumber, HeaderBuf, lim) 

char *dirname, /* IN */

     *id, /* IN */

     *HeaderName; /* IN */

char *HeaderBuf; /* OUT */

int   HeaderTypeNumber, /* IN */

      lim; /* }}IN */


The CUI_GetHeaderContents  routine is preferred for client interfaces.


This routine allows the client program to take advantage of the message 
parsing routines built into the message server.  It causes the server to read 
in the message specified by dirname and id, parse it, and return the contents 
of a specified header.  The header to be sought should be either specified by 
name in HeaderName or by number in HeaderTypeNumber.  (The legal values for 
HeaderTypeNumber are specified in the include file AMSHEADPARSE.h.)  It is 
somewhat faster to search by header number.  If you are searching by 
HeaderName, then HeaderTypeNumber should be less than zero.  The server will 
pack the contents of the header, up to lim bytes, in Headerbuf.


MS_HeaderContents exists largely for historical reasons; its use is 
discouraged, as clients are perfectly capable of parsing messages themselves. 
 MS_HeaderContents may be phased out in a future version of the system.


\typewriter{\smaller{MS_AlterSnapshot(dirname, id, NewSnapshot, Code) 

char *dirname, *id, *NewSnapshot;

int Code;}}


In general, this routine should not be used by clients; use CUI_AlterSnapshot 
instead.


This routine is the basic engine for changing existing entries in a message 
directory.  It can be used to change any part of the snapshot of a message. 
 The message, as usual, is identified by dirname and id.  A new value for the 
snapshot for that message is given in the NewSnapshot parameter.  The way in 
which that new value is to be used is specified by the Code parameter, which 
should be one of the following constants defined in AMS.h:


\leftindent{\bold{ASS_REPLACE_ALL}:  The snapshot is simply replaced by the 
new snapshot.


\bold{ASS_REPLACE_ATTRIBUTES}:  The attributes are replaced by the attributes 
in the new snapshot, but everything else is left unchanged.


\bold{ASS_REPLACE_ATT_CAPT}:  The attributes AND caption fields are replaced 
by those in the new snapshot, and everything else is left unchanged.


\bold{ASS_OR_ATTRIBUTES}:  The attributes in the two snapshots are OR'ed 
together, with everything else left unchanged.  (This is used, for example, to 
mark a message as deleted.)


\bold{ASS_AND_ATTRIBUTES}:  The attributes in the two snapshots are AND'ed 
together, with everything else left unchanged.  (This is used, for example, to 
mark a message as undeleted.)}


Note that the AMS_DATE field will NEVER be altered by this call.


\typewriter{\smaller{MS_CloneMessage(SourceDirName, id, DestDirName, Code)

char *SourceDirName, /* IN */

     *id, /* IN */

     *DestDirName; /* IN */

int Code; /* IN */}}


CUI_CloneMessage is preferrred over MS_CloneMessage for client interfaces.


This routine makes a copy of a message.  The message that currently exists in 
the directory SourceDirName, with the specified id, is copied into the 
directory DestDirName.  The semantics of the operation depend on the value of 
the Code variable:


\leftindent{\bold{MS_CLONE_COPY}: The message is simply copied to the new 
directory.


\bold{MS_CLONE_COPYDEL}: The message is copied, and marked for deletion in the 
old directory.


\bold{MS_CLONE_APPEND}: The message is copied to the new directory and the 
date field is changed to position the message at the end of that directory 
(the date is set to the current time or greater).


\bold{MS_CLONE_APPENDDEL}: The message is appended as in MS_CLONE_APPEND, and 
marked for deletion in the old directory.


\bold{MS_CLONE_SYMLINK}:  (NOT YET IMPLEMENTED) The message is created in the 
new directory with its body a symbolic link to the body in the old directory.

\bold{
MS_CLONE_MOVE}:  (NOT YET IMPLEMENTED)  The message is moved from the old 
directory to the new one, effectively deleting and purging it from the old 
one. }



\smaller{\typewriter{long    MS_EditMessage (dirname, id, NewBodyFile, 
Reparse) char   *dirname, /* IN */

       *id, /* IN */

       *NewBodyFile; /* IN */

int	Reparse; /* IN */}}


This routine is used to change the contents of a message.  The body of the 
message identified by dirname and id is replaced by the contents of the file 
NewBodyFile.  If the integer Reparse is nonzero, then the body file is 
reparsed and the snapshot is rebuilt accordingly.  However, the AMS_DATE field 
will NEVER be altered by this call.


\typewriter{\smaller{MS_PrintMessage(DirName, id, flags, printer)

char *DirName, /* IN */

     *id, /* IN */

     *printer;

int   flags; /* IN */}}


The CUI_PrintBodyFromCUID routine is preferred for client interfaces.


This routine queues the message identified by DirName and id for printing.  If 
GoFast is non-zero, the printing will not take place immediately, but will 
instead take place the next time the message server updates its state.  (The 
server updates its state after a minute of inactivity; in single-process 
non-SNAP versions, the state updates typically happen at regular intervals, 
regardless of activity.)  The printing may also take place sooner if the 
number of calls to MS_PrintMessage exceeds the size of the message server's 
printing queue.  


The flags value is a bit vector that can be used to specify certain printing 
options, to wit:


\leftindent{\bold{AMS_PRINT_FIXED} -- Print the body in a fixed-width font

\bold{AMS_PRINT_ROT13} -- Print the body using the netnews Rot13 simple 
decryption scheme.}


The printer value is the name of a printer to print on.  If it is null, the 
default printer will be used.   The printer name is validated by the 
messageserver, to prevent confusing and persistent errors later (since the 
message server maintains, in effect, its own print queue).  You can use the 
MS_PrintMessage call to JUST validate a printer name by passing in null values 
for DirName and id; in this case, if the printer is good, you will get EINVAL 
returned as AMS_ERRNO, whereas if it is bad, you will get EMSBADPRINTER. 
 (Sites can use the AndrewSetup file to customize the algorithm by which 
printer names are validated.)


\smaller{\typewriter{MS_PurgeDeletedMessages(dirname)

char *dirname; /* IN */}}


This routine simply purges the named directory of all messages marked with the 
"deleted" attribute.


\typewriter{\smaller{MS_Epoch(dirname, date64)

char *dirname; /* IN */

char *date64; /* IN */}}


This routine deletes all messages from a message directory \italic{and all its 
subdirectories} whose date field is less than the date given in base 64 as 
date64. It is intended primarily for use in bboard purging. 


\heading{Calls that relate to information about message directories}


\smaller{\typewriter{MS_CreateNewMessageDirectory(DirName, Overwrite, 
BodyDirName)

char *DirName; /* IN */

int   Overwrite; /* IN */

char *BodyDirName; /* IN */}}


The CUI_CreateNewMessageDirectory routine is preferred for client interfaces.


This routine creates a new message directory, using the path name given in 
DirName.  If Overwrite is non-zero, the call will NOT return an error if the 
directory already exists, but will zero the directory file.  Typically, 
BodyDirName should be the same as DirName; if it is not, then all message 
bodies for the directory will be assumed to live elsewhere, in the directory 
BodyDirName.  The feature whereby bodies can live elsewhere has not been 
extensively tested.


\typewriter{\smaller{MS_ReconstructDirectory(DirName, NumGood, NumBad, 
TrustTimeStamp)

char *DirName; /* IN */

int  *NumGood, /* OUT */

     *NumBad, /* OUT */

      TrustTimeStamp; /* IN */}}


This routine reconstructs the directory file for the message directory 
DirName, and is typically needed only if that file has become corrupted, an 
extremely rare event.   If TrustTimeStamp is non-zero, then the timestamp on 
the body files will be used for the date field; otherwise, the date header 
from the message itself will be parsed and used.  The routine places the 
number of files that were successfully parsed and entered in the directory 
file in the NumGood variable, and the number of failures in NumBad. 
 Typically, NumBad should be zero, but an extremely mis-formatted file in the 
directory itself, as well as transient errors such as the file system going 
down, can cause NumBad to be non-zero.  Typically the message server will 
print some error messages to the console if some files can not be dealt with 
properly during reconstruction.


\smaller{\typewriter{MS_ScavengeDirectory((DirName, Recurse, numgood, numbad, 
quiet, Purge)

char *DirName; /* IN */

int Recurse, /* IN */

int *numgood, /* OUT */

int *numbad /* OUT */

int quiet, Purge; /* BOTH IN */}}


This procedure will scavenge the named directory, looking for any mismatches 
between the actual message bodies in the directory and the entries in the 
index file .MS_MsgDir.  If Recurse is non-zero, it will do the same thing 
recursively for all subdirectories.  If quiet is zero, it will print out a lot 
of informative junk a long the way (although in the case of a remote 
messageserver, these messages will go to the console of that machine).  If 
Purge is non-zero, each directory will be purged after it is scavenged.  The 
number of directories successfully scavenged will be returned in numgood, 
while the number that could not be scavenged will be returned in numbad.


\smaller{\typewriter{long    MS_MergeDirectories (SourceDirName, DestDirName)

char   *SourceDirName, /* IN */

       *DestDirName; /* IN */}}


This routine will merge the directory SourceDirName into the directory 
DestDirName.  SourceDirName will cease to exist, and DestDirName will contain 
the union of the two old directories.  Clients should use CUI_MergeDirectories 
rather than MS_MergeDirectories.


\typewriter{\smaller{long    MS_GetDirInfo (DirName, ProtCode, MsgCount)

char   *DirName; /* IN */

int    *ProtCode, /* OUT */

       *MsgCount; /* OUT */}}


This routine returns information about the named directory.  The number of 
messages in the directory is returned in MsgCount, and the protection status 
of that directory for the current user is returned in ProtCode as one of the 
following codes, defined in AMS.h:


\leftindent{\bold{AMS_DIRPROT_READ}: The user has read-only access, and the 
directory is neither a local, external, nor official bboard.


\bold{AMS_DIRPROT_LOCALBB}: The user has read-only access, and the directory 
is in the "LOCAL" domain, as referenced by $LOCAL in the mspath and as defined 
in the site configuration source file siteconfig.c.  Note that this value will 
not be returned for elements in this domain if the user has extra (e.g. 
administrative) rights.


\bold{AMS_DIRPROT_OFFBB}: The user has read-only access, and the directory is 
in the $OFFICIAL domain, as defined previously for $LOCAL.


\bold{AMS_DIRPROT_EXTBB}: The user has read-only access, and the directory is 
in the $EXTERNAL domain, as defined previously for $LOCAL.


\bold{AMS_DIRPROT_MODIFY}: The user has read and write access.


\bold{AMS_DIRPROT_MBOX}: The user has read and write access, and may also 
process incoming mail from the associated Mailbox.


\bold{AMS_DIRPROT_FULLMAIL}: The user has read and write access, and may also 
process incoming mail from the associated Mailbox, and it is one of his 
private mail directories.


\bold{AMS_DIRPROT_AWFUL}:  The user has no access, or the combination of 
access rights on the various relevant directories is horribly muddled.}


\smaller{\typewriter{long    MS_RemoveDirectory (DirName, MaxRemovals)

char   *DirName;  /* IN */

int MaxRemovals; /* IN */}}


This routine deletes the message directory DirName and all its contents.  It 
will not remove anything, however, if the directory has sub-directories, or if 
the number of messages in the directory is greater than MaxRemovals.  (This 
latter check helps prevent people from deleting a directory they believe is 
empty moments after someone else has added something to it.)


\typewriter{\smaller{long MS_CheckMissingFolder(oldname, newname)

char *oldname; /* IN */

char *newname; /* out */}}


This routine will check to see if the folder named by oldname, which 
presumably no longer exists (AMS_ERRNO was ENOENT), has been replaced by 
another folder in the system's ChangedSubscriptions file.  If so, the new name 
will be placed in newname; otherwise, newname will be returned as the null 
string.


\typewriter{\smaller{long MS_RenameDir(OldName, NewName, NewFullName)

char 	*OldName, /* IN */

	*NewName, /* IN */

	*NewFullName; /* OUT */}}


This routine renames a directory.  In the current implementation, the 
directory must have no children.  The OldName is the full pathname of the 
existing directory.  NewName is a shorthand for what it is being renamed to. 
 NewFullName is the new full path name of the directory.  Thus for example, if 
you call it with OldName "/foo/bar/baz" and NewName "hello", NewFullName will 
be returned as "/foo/bar/hello".


\typewriter{\smaller{MS_InstallWelcomeMessage(ParentName, InitDir, InitFile, 
ShortName)

char *ParentName, /* IN */

     *InitDir, /* IN */

     *InitFile, /* IN */

     *ShortName; /* IN */}}


This routine is intended to be called immediately after a directory is 
created, and is used to install an initial message on a new directory and its 
parent directory, if it has one.  The message installed in the parent 
directory will have the "new directory" attribute set, and an appropriate 
X-Andrew-DirectoryCreation header in the body, so that subscribers to the 
parent directory will automatically be asked if they want to subscribe to the 
new child.


If InitDir is non-null, then the new message will be created by copying an old 
message.  That old message will be found in the directory InitDir, with id 
InitFile.  If InitDir is null, then InitFIle must be the full path name of an 
RFC822 message which will be used as the text of the welcome message.


The ParentDir is the name of the parent directory, while ShortName is the name 
of the subdirectory within the parent directory (\italic{not} the full path 
name, simply the last element of the name of the new directory).


\typewriter{\smaller{MS_MatchFolderName(pat, fname)

char *pat, /* IN */

     *fname; /* OUT */

}}
This routine is called to get a list of folder names matching a given pattern. 
 Currently, the pattern is simply a prefix, and you will thus get a list of 
all folder names starting with the string given as pat.  The list is written 
out, one folder name to a line, in short (non-canonical) form, in a temporary 
file, the name of which is returned in fname.


\heading{Calls that relate to mail delivery}


\typewriter{\smaller{MS_NameReplyFile(DirName, id, code, FileName)

char *DirName, /* IN */

     *id, /* IN */

     *FileName;  /* OUT */

int code; /* IN */}}


** Clients should not call this, but should call CUI_NameReplyFile instead.**


This routine will cause the message server to build a template for a new piece 
of outgoing mail.  That template will contain entries for standard headers 
such as To: and CC:, some of which may be filled in.  The mail is typically 
generated in response to an existing message, identified by DirName and id. 
 The template is written out to a file by the message server, which returns 
the name of the file to the client in FileName.


The precise contents of the file are determined by the code value, which 
should be one of the following values defined in AMS.h:


\leftindent{\bold{AMS_REPLY_FRESH}: The DirName and id are ignored, and a 
template for a fresh piece of mail is generated.  This is typically not 
necessary, but is included for completeness to simplify some conceivable mail 
generation algorithms.


\bold{AMS_REPLY_SENDER}:  The mail is generated as a reply to the sender of 
the original message.


\bold{AMS_REPLY_WIDE}: The mail is generated as a reply to the recipients of 
the original message.  (This is typically the To, CC, and Resent-To lines, but 
may be overridden by the X-Andrew-WideReply header.)


\bold{AMS_REPLY_WIDER}:  The mail is generated as a reply to both the sender 
and recipients of the original message.


\bold{AMS_REPLY_FORWARD}:  The mail is generated as an attempt to forward the 
original message; all that is filled in initially is the subject header and 
the body.  If the message being forwarded was in Andrew Toolkit (BE2) format, 
that formatting is stripped out.


\bold{AMS_REPLY_FORWARD_FMT}:  Same as AMS_REPLY_FORWARD, except that BE2 
formatting is preserved; this should only be used by interfaces that 
understand the BE2 datastream.


\bold{AMS_REPLY_REDRAFT}:  The mail is considered to be a draft message which 
is now to be considered again for sending.  Several headers and the body are 
filled in.}


\smaller{\typewriter{MS_ValidateAndReplaceChunk(FileName, inaddr, outaddr, 
outaddrsize, which, outcode)}}


This routine is a "worker" routine that, unfortunately, could not be made 
entirely internal to the message server.  It is intended to be used ONLY by 
the CUI_RewriteHeaderLine function.  Rather than document its extremely 
bizarre semantics, I will here only encourage programmers to use the 
CUI_RewriteHeaderLine routine instead.


Note that the ugliness of MS_ValidateAndReplaceChunk is calculated and was 
done knowingly; there were serious tradeoffs between what we wanted to do on 
the client and server ends.  Thus this is probably a permanent piece of the 
architecture, just an ugly one.  Use the CUI library call.


\typewriter{\smaller{MS_WriteAllMatchesToFile(ambigname, filename)

char *ambigname; /* IN */

char *filename; /* OUT */}}


This routine takes a mail address and writes all possible matches to it in a 
file.  This is also intended for use ONLY by the CUI_RewriteHeaderLine call.


\typewriter{\smaller{MS_SubmitMessage(FileName, DeliveryOptions, ErrorMessage, 
ErrMsgLimit, ClientProgram)

char *FileName, /* IN */

    *ErrorMessage,/* OUT */

    *ClientProgram; /* IN */

int  BlindStatus, /* IN */

    ErrMsgLimit; /* IN */}}


(NOTE:  Interfaces should use CUI_SubmitMessage instead of MS_SubmitMessage.)


This routine causes the message server to delivery mail that has been composed 
by the client and stored as a file accessible to the server (presumably using 
MS_StorePartialFile).  The client should have already validated the recipients 
in the To and CC headers using MS_RewriteHeader or CUI_RewriteHeaderLine.  The 
message server will deliver the mail to the addresses listed in the To and CC 
headers.


The mail is read from the file FileName, which will be deleted if delivery is 
successful.  Certain delivery failures will cause an additional error code to 
be written into returncode; this is the code returned by the qclose (like 
pclose) on the delivery process, and may be susceptible to further analysis.


The value DeliveryOptions tells the server several things about the delivery, 
e.g. whether or not to keep a blind copy.  It should be an OR-ing of bits 
defined in AMS.h:


\leftindent{\bold{AMS_SEND_BLINDYES} -- Keep a blind copy.

\bold{AMS_SEND_BLINDNO} -- Do not keep a blind copy.

\bold{AMS_SEND_ISRESEND} -- Write "ReSent-From" instead of "From", etc.

\bold{AMS_SEND_ILLEGAL} -- Don't complain about non-Ascii bytes or long lines

\bold{AMS_SEND_UNFORMATTED} -- Strip out any Andrew Toolkit (BE2) formatting 
before sending

}
The blind copy will be sent to the user through the mail unless the "bccto" 
option is used, in which case it will be appended directly to an appropriate 
message directory.


Messages from the delivery system will be placed in the ErrorMessage buffer 
when one of the EMSDROPOFF codes is returned; some of these indicate errors, 
others exceptional conditions that are worth noting, even though mail delivery 
succeeded anyway.


The ClientProgram variable should be the name and version number of the client 
interface.  Anything other than letters and digits in this string will be 
converted to dots to ensure compliance with RFC822 requirements for Received 
headers.


\heading{Calls that relate to remote file access}


\smaller{\typewriter{MS_DisambiguateFile(source, target, AccessCode)

char   *source, /* IN */

       *target; /* OUT */

short   AccessCode; /* IN */}}


This routine is used to resolve a file name (source) that the user has typed 
and to return a valid full path name (target).  The resolution involves the 
interpretation of tilde-prefixes, and a search path down the user's mspath. 
 This is the routine that can turn "andrew/gripes" into 
"/cmu/itc/bb/.MESSAGES/andrew/gripes".  Note that the conversion of 
"andrew.gripes" to "andrew/gripes" must still be performed by the interface 
program.  


AccessCode should be one of four values, defined in AMS.h.  If it is 
AMS_DISAMB_EXISTS, then any matching file name is satisfactory.  If it is 
AMS_DISAMB_DIREXISTS, it must be the name of a valid message directory.  If it 
is AMS_DISAMB_FILEEXISTS, it must be the name of an existing file.  If it is 
AMS_DISAMB_FILEMAYBECREATED, it must be the name of a file which at least has 
an existing parent.  (It does NOT check to make sure that the file may be 
created by the current user.)


In general, client programs should \italic{not ever} call MS_Disambiguate file 
with AccessCode == AMS_DISAMB_DIREXISTS.  If that is what you want to do, you 
should call the CUI library routine CUI_DisambiguateDir, which caches all 
directory information to avoid repetitive calls to the server.


\typewriter{\smaller{MS_GetPartialFile(FileName, Buf, BufLim, offset, 
remaining, ct)

char *FileName, /* IN */

     *Buf; /* OUT */

int BufLim, /* IN */

     offset, /* IN */

    *remaining, /* OUT */ 

    *ct; /*OUT */}}


This routine allows the client to retrieve a file from the server, even if the 
client is not on Vice or the file is on the server's local disk.  The File is 
named as FileName, and a portion of it is returned in Buf.  The portion begins 
at position offset in the file and continues for up to BufLim bytes.  The 
server returns a count of the bytes returned in the ct variable, and the 
number of bytes remaining until the end of the file in the remaining variable. 
 The intent is for the client to call this routine repeatedly, with increasing 
values for offset, until the return value of remaining is 0.


\smaller{\typewriter{MS_StorePartialFile(FileName, startpos, len, mode, 
Truncate, WhatToStore)

char *FileName, /* IN */

     *WhatToStore; /* IN */

int   startpos, /* IN */

      len, /* IN */

      mode, /* IN */

      Truncate; /* IN */}}


This routine is the inverse of MS_GetPartialFile, and is used to cause the 
server to store a file.  The file will be stored under the name given as 
FileName, and the server will write len bytes from the buffer WhatToStore 
after doing an lseek to position startpos.  The file will be opened with 
protection mode (standard UNIX protection codes), and will be truncated after 
the write if the Truncate parameter is nonzero.


\typewriter{\smaller{long    MS_UnlinkFile (FileName)

char   *FileName; /* IN */}}


This routine will cause the server to unlink (delete) the named file.  


\smaller{\typewriter{MS_WriteUnscribedBodyFile(DirName, id, FileName)

char *DirName, /* IN */

     *id, /* IN */

     *FileName; /* OUT */}}


This routine takes the message identified by DirName and id, runs it through 
the appropriate routines to strip out multi-media formatting, and writes the 
result in a temporary file, the name of which is returned in FileName.  When 
you're done with the file, you should delete it with MS_UnlinkFile.


\heading{Calls that relate to personal subscription/profile information}


\smaller{\typewriter{MS_GetSearchPathEntry(which, buf, lim)

int     which, /* IN */

        lim; /* IN */

char   *buf; /* OUT */}}


This routine is used to retrieve path names from the user's mspath, the search 
path used for finding message directories.  The which parameter is an integer 
that tells which entry is of interest; 0 is the first entry, and so on.  The 
routine will return an error code with AMS_ERRNO set to ERANGE if which is out 
of range.  Otherwise it will copy the path name into buf, up to lim bytes.


The which parameter may also take on a negative value -- these are defined in 
AMS.h as AMS_MAILPATH, AMS_LOCALPATH, AMS_EXTERNALPATH, and AMS_OFFICIALPATH, 
and are used to access portions of the mspath by their meaning rather than by 
their order.


\typewriter{\smaller{MS_FindMailbox(pathelt, Buf)

int pathelt; /* IN */

char *Buf; /* OUT */}}


This routine takes an integer which is an index into  the user's mspath list 
and returns the name of the associated mailbox, if any, from which new 
messages should be read.  It will return an error code if the user does not 
have the right to read files from the mailbox, or if the user's mspath 
preference has not marked this path element as having a mailbox worth 
checking.

\smaller{\typewriter{MS_NameSubscriptionMapFile(Root, MapFile)

char *Root;  /* IN */

char *MapFile; /* }}OUT */


This routine takes a pathname Root, which should be the full path name of the 
root of message directory tree (a .MESSAGES directory), and will build a file 
that encodes the subscription status of all messages within that directory for 
the current user.  The name of that file is then returned as MapFile.  


The format of the subscription map file is simply a series of lines, each of 
which contains a message directory name followed by a space and an integer. 
 The integer is one of the valid subscription statuses defined in AMS.h, such 
as AMS_UNSUBSCRIBED, AMS_ASKSUBSCRIBED, etc.


If the "Root" parameter is NULL, then the map is built for all things the 
current user subscribes to, and nothing else; this is a reasonably fast 
operation, since that information is typically already in the server's memory.


\typewriter{\smaller{MS_NameChangedMapFile(MapFile, MailOnly, ListAll, 
NumChanged, NumUnavailable, NumMissingFolders, NumSlowpokes, NumFastFellas)

char *MapFile; /* out */

int MailOnly, /* in */

    ListAll, /* in */

    NumChanged, /* out */

    NumUnavailable, /* out */

    NumMissingFolders, /* out */

    NumSlowpokes, /* out */

    NumFastFellas; /* out */}}


This is just like the MS_NameSubscriptionMap call, except that the file it 
builds contains only those folders to which you are subscribed that actually 
have new messages.  Also, there is an extra integer at the end of each line, 
to be described in a moment.


The input parameter MailOnly tells this call to only look at mail folders. 
 The ListAll parameter tells it to list even those things you are subscribed 
to that do NOT have new messages.  However, the last thing on each line in the 
file is a Boolean integer which is zero if there is nothing new and the line 
was included only because ListAll was non-zero.


The return values are the MapFile name and a number of counters that detail 
how well the search worked out.  NumChanged is the number of folders which 
need to be looked at.  NumUnavailable is the number that could not be checked 
due to Vice or network errors.  NumMissingFolders is the number of things on 
the subscription list that no longer exist (these are always included in the 
mapfile, so that the interface will itself be able to discover their 
non-existence and take appropriate action, namely calling 
CUI_HandleMissingFolder).  Finally, NumSlowpokes and NumFastFellas tell how 
many of the folders could be checked quickly (via the master update file 
mechanism) and how many required the slower check of actually opening the 
folder.


\typewriter{\smaller{MS_SetSubscriptionEntry(FullName, NickName, status)

char *FullName; /* IN */

char *NickName; /* IN */

int status; /* IN */}}


This routine alters the user's subscription status for the directory named as 
FullName.  It sets the nickname (the name the user will see during 
subscription updating) and the subscription status (as defined in AMS.h).  ** 
CLIENT ROUTINES SHOULD USE CUI_SetSubscriptionEntry instead. **


\smaller{\typewriter{MS_GetSubscriptionEntry(FullName, NickName, status)

char *FullName; /* IN */

char *NickName; /* OUT */

int *status; /* OUT */ }}


This routine retrieves information about the user's subscription status for 
the directory FullName.  It returns the directory's nickname, and the status, 
as defined in AMS.h


\typewriter{\smaller{MS_GetNextSubsEntry(FullName, NickName, status)

char *FullName; /* INOUT */

char *NickName; /* OUT */

int *status; /* OUT */ }}


This routine is just like MS_GetSubscriptionEntry, except that it retrieves 
information about the succeeding subscription entry.  If FullName is the null 
string, it returns information about the first subscription entry, while 
otherwise it returns information about the succeeding subscription entry.  If 
FullName is the last subscription entry, it returns with FullName null.


\smaller{\typewriter{MS_RebuildSubscriptionMaps()}}


This routine rebuilds the subscription map files for every element on the 
user's mspath to which he has write access.  This routine is called by the CUI 
rebuild command, and should never be necessary since the release of version 
3.1 of the messages program (3.11 of CUI).


\typewriter{\smaller{MS_RebuildMasterUpdateFiles(NumFastGood, NumSlowGood, 
NumBad, NumAbsent, NumProbablyGood)

int *NumFastGood, /* out */

    *NumSlowGood, /* out */

    *NumBad, /* out */

    *NumProbablyGood, /* out */

    *NumAbsent; /* out */

}}
This routine rebuilds the master update files for every element of your mspath 
to which you have write access and in which a master update file already 
exists.  The return values tell how many entries in that file were good; the 
"Fast" and "Slow" refer simply to how easily it good be determined that those 
entries were good.  NumBad is a count of bogus entries in the master update 
file; these should be very rare, and correspond to Vice going down at just the 
wrong moment.  NumAbsent is the number of lines that had to be added to the 
master update file because there was no corresponding line there before. 
 NumProbablyGood is a count of the number of entries that looked bad (and used 
to be counted in NumBad) but reflect changes less than 15 minutes old.  Such 
entries probably would have been picked up soon by the normal mechanism, and 
hence were not really "bad", merely symptomatic of a sort of race condition in 
the checking.


\typewriter{\smaller{MS_RebuildOneMasterUpdateFile(PathElt, NumFastGood, 
NumSlowGood, NumBad, NumAbsent, NumProbablyGood) 

char *PathElt; /* IN */

int *NumFastGood, *NumSlowGood, *NumBad, *NumAbsent, *NumProbablyGood; /* ALL 
OUT */

}}
This routine is just like the previous one, except that it only rebuilds the 
master update file for a single element of your mspath, named in the PathElt 
argument.


\typewriter{\smaller{MS_SetAssociatedTime(FullName, newvalue)

char *FullName, /* IN */

     *newvalue; /* IN */}}


This routine sets the user's profiling information to note that the last 
element the user has seen in the directory FullName was time-stamped with the 
base 64-encoded time newvalue.  This is the information that is used in 
checking for new messages; typically, the newvalue is the AMS_DATE of the 
snapshot of the last message seen.


\smaller{\typewriter{MS_GetAssociatedTime(FullName, Answer, lim)

char *FullName, /* IN */

     *Answer; /* OUT */

int lim; /* IN */}}


This routine retrieves the user's profiling information, specifically the date 
field of the last message seen on the directory FullName.  The base 64 date is 
stored in Answer, up to lim bytes.  (The lim parameter is probably silly; the 
size of a date is defined as AMS_DATESIZE anyway.)


\heading{Calls That Relate To User-Defined Attributes

}
As usual, it is likely that the client will want to use the CUI_ library 
calls, which provide more suitable abstractions.


\typewriter{MS_GetDirAttributes(Dirname, AttrCt, Attrs, SepChar, ShowEmpty)

char *Dirname; /* IN */

int *AttrCt; /* OUT */

char *Attrs; /* OUT */

int SepChar, ShowEmpty; /* IN */}


This routine is used by the client to get the names of all the attributes in a 
folder.   The folder is named as Dirname.  The Attrs buffer is used to return 
the names of all the attributes defined in that folder, separated by the 
character passed in as SepChar.  (The Attrs buffer should be big enough to 
hold all possible attribute names, i.e. AMS_NUM_UATTRS * AMS_ATTRNAMEMAX.) 
 The number of attributes defined are returned as AttrCt.  Finally, ShowEmpty 
is a flag passed in that, if nonzero, tells the messageserver to include the 
phrase defined as UNUSEDATTRNAME (again, in AMS.h) everywhere that an unused 
attribute appears as a "place holder".  To make sense of this, imagine that 
you first created an attribute called "foo", and then one called "bar", and 
then deleted the "foo" attribute.   Now, the "bar" attribute happens to remain 
attribute #2, even though the total number of defined attributes is only 1. 
  If ShowEmpty is non zero, then the UNUSEDATTRNAME will preced the "bar" 
name, making it possible to map names to numbers.   If ShowEmpty is zero, such 
unused attributes are skipped, and the list is useful only, for example, for 
telling the user what attribute exists.  Most of this is hidden behind CUI 
library routines, if you want to avoid understanding it.


\typewriter{MS_AddAttribute(Dirname, Newname, AttNum)

char *Dirname, *Newname; /* both IN */

int *AttNum; /* OUT */}


This routine can be used to add a new attribute to a folder.  The folder is 
named as Dirname, and the attribute is named as Newname; the messageserver 
returns the attribute number assigned to the new attribute.  Likely errors 
include ERANGE, if there are too many attributes in the folder to define a new 
one, and EMSATTREXISTS, when an attribute with that name already exists.


\typewriter{MS_DeleteAttr(DirName, AttrName)

char *DirName, *AttrName; /* BOTH IN */

}
This routine can be used to delete an existing attribute in a folder.  The 
folder name and attribute name are the two input parameters.


\heading{Calls primarily for debugging purposes}


\typewriter{\smaller{MS_GetVersion(Buf, lim) 

char *Buf; /* OUT */

int lim; /* IN */}}


This routine packs a string that defines the message server version into the 
buffer Buf, up to lim bytes.


\smaller{\typewriter{MS_DebugMode(level, snap, malloc)

int   level, /* IN */

      snap, /* IN */

      malloc; /* IN */}}


This routine sets the message server debugging values.  level is the debug 
level for the actual ms code; it is a bit map, defined in MS.h.  snap is the 
SNAP debug mask, as defined in the SNAP code.  malloc is the CheckLevel for 
the ITC malloc, as defined in the malloc documentation.


By default, message server debugging will show up in the console.  This is not 
what you want.  You want to use the following routine:


\typewriter{\smaller{MS_OpenDebuggingPipescript(DoIt) 

int DoIt; /* IN */ }}


This routine causes the message server's debugging output to be redirected to 
a new pipescript process if DoIt is nonzero, or reset to where it used to be 
if DoIt is zero.


\smaller{\typewriter{MS_Die()}}


This routine kills the message server.  The CUI library will probably try to 
reconnect you automatically the next time you try to do anything else of 
interest.  Nonetheless, its always fun.


\heading{Miscellaneous calls}


\typewriter{\smaller{MS_GetConfigurationParameters (MailDomain, len, 
UseAmsDelivery, UseNameSep, DeliveryType)

int *UseAmsDelivery, /* OUT */

	*UseNameSep, /* OUT */

	*DeliveryType, /* OUT */

	len; /* IN */

char *MailDomain; /* OUT */


}}This routine allows the client to get various configuration information from 
the messageserver.  In particular, the MailDomain is the host to give in the 
return address for outgoing mail (not generally needed by clients, but 
occasionally relevant).  It will be truncated to the buffer length specified 
as len.  UseAmsDelivery is a ternary value specifying whether MailDomain uses 
the AMS mail delivery system: 0 means the answer is not known, -1 means that 
MailDomain does not use the AMS mail delivery system, and a value greater than 
0 means that MailDomain indeed uses the AMS mail delivery system.


If MailDomain runs the AMS delivery system, the value of UseNameSep describes 
how MailDomain prefers that its return addresses be composed.  A value for 
UseNameSep less than 0 means that return addresses are to be in the 
userid+@MailDomain form; a value of 0 means that it is unknown how MailDomain 
formats its return addresses (but the userid+ form is always safe); and a 
value greater than 0 is an ascii value of a separator character to be used 
instead of the spaces in the user's full name (e.g., a pw_gecos field).


The values for DeliveryType are constants with names beginning with defined in 
the $ANDREWDIR/include/dropoff.h file.  These values describe how the message 
delivery system is going to work, with -1 (DT_UNK) meaning it is unknown, 0 
(DT_AMS) meaning normal AMS dropoff, and values greater than 0 (DT_CANNOT, 
DT_NONAMS, ...) meaning some kind of restricted dropoff due to an unusual 
intercellular configuration.

\typewriter{\smaller{
MS_AndrewDir (AndrewDir, len)

char *AndrewDir; /* OUT */

int len; /* IN */

}}
This routine returns at most len characters indicating the directory where all 
andrew software is rooted, typically but not necessarily "/usr/andrew".


\typewriter{\smaller{MS_GenTempFileName(Buf)

char *Buf; /* OUT */

}}
This routine generates a temporary file name "guaranteed" to be world-unique 
(at least on the server end...)  The guarantee is weaker on System V or other 
sites that configure the file name limit to 14 characters.


\typewriter{\smaller{MS_ProcessNewMessages(SourceDir, NumGood, NumBad, 
NumLocks, ParseSpecFile, resultcode, FirstError, Buf, buflim)

char *SourceDir, /* IN */

     *ParseSpecFile, /* IN */

     *Buf; /* OUT */

int  *NumGood, /* OUT */

     *NumBad, /* OUT */

     *NumLocks, /* OUT */

     *resultcode, /* OUT */

     *FirstError, /* OUT */

     buflim; /* IN */}}


This routine reads new messages from the mailbox directory SourceDir and puts 
them into message directories.  It returns five integer parameters:  NumGood 
is the number of files read successfully from the mailbox, NumBad is the 
number of files that could not be read or posted properly and were hence left 
in the mailbox, and NumLocks is the number of files in the mailbox that were 
locked by another process (presumably a delivery process) and were less than 
20 minutes old.  (After 20 minutes the locks are overridden.)  FirstError is 
the mserrcode from the first error; it contains more specific information 
about why the first of the errors occurred.


In order to decide which directory to add new messages to, the server reads a 
program which is written in a special language and is found in the file 
ParseSpecFile.  If the ParseSpecFile parameter is null, the server looks for a 
file called ".MS.spec" in the parent of the mailbox directory.  If there is no 
such file, it simply adds messages to a directory which is named by doing the 
equivalent of SourceDir/../.MESSAGES/misc.


The programming of Spec Files is a black art, and is documented separately.


NOTE: As of mid-1988, the FLAMES language (for "Filtering Language for the 
Andrew MEssage System") has begun to supersede Spec Files.  The server now 
looks first for a file called ".AMS.flames" before it looks for ".MS.spec". 
 Since FLAMES is a scaled-down Lisp interpreter, it can have some pretty 
complex error messages; hence the buffer Buf which is passed in to this 
routine, along with its length, buflim.  If and only if a Lisp error occurs 
while executing a FLAMES function, Buf will come back as something other than 
the empty string.  Hopefully, this "something other" will be something useful.


I wouldn't say that the programming of FLAMES files is a black art.  It's gray 
at worst.  However, it too is documented separately.


\typewriter{\smaller{MS_AppendFileToFolder(FileName, FolderName)

char *FileName, *FolderName; /* BOTH IN */}}


This routine takes the message that is stored in the named file and appends it 
as a new message onto the end of the named folder.  If it succeeds, it then 
deletes the original file (to assure idempotence of the operation, naturally.) 
 Obviously errors occur and are returned if the file is inaccessible or 
unparseable, or if the folder does not exist can not be opened for writing.


\typewriter{\smaller{MS_CheckAuthentication(Authenticated)

int *Authenticated; /* OUT */}}


This routine will set the integer Authenticated to be 1 if the server is 
properly authenticated (or if it is not on Vice at all), and 0 if their are 
authentication problems.  It may also return errors, which are most likely to 
be RPC transmission errors or Vice/pioctl failures.


\smaller{\typewriter{MS_UpdateState()}}


This routine causes the message server to update all of its state to the file 
system.  This may include storing the .MS.subscriptions file, storing the 
.MS.profile file, storing open message directories (.MS_MsgDir files), 
printing messages that have been queued for printing, reaping "zombie" 
processes, and probably some other stuff besides.  It is generally called as a 
cleanup message by a terminating client.  However, the MS_FastUpdateState 
routine is better for keepalives.


\smaller{\typewriter{MS_FastUpdateState()}}


This routine is just like MS_UpdateState, except it skips certain 
time-consuming and non-critical functions like printing.  It is ideal for 
client "keepalive" or "checkpoint" messages.


\typewriter{\smaller{MS_ReInitialize()}}


This causes the message server to re-read the user's preferences and to 
perform certain associated operations.  It should be called by each client 
when he first connects to a message server, to ensure that any changes that 
the user has made to his preference file will take place, as he expects, when 
he runs a new program.


\smaller{\typewriter{MS_ConvertOldMail(good, bad)

int *good, *bad; /* OUT */}}


This routine will convert an old Maillib directory, used by the old ReadMail 
program, into messages format.  It will NOT preserve ReadMail class 
information.   A script that will aid in the conversion of ReadMail classes is 
available from the author; it should be used\italic{ instead} of the convert 
routine when class preservation is essential.


\typewriter{\smaller{\smaller{\bigger{MS_TakeHints(DoAll, ProtFailures)

int DoAll, /* IN */

    *ProtFailures; /* OUT */}}}}


This routine will update the Master Update file for bboard directories by 
processing the HINT files that have been left.   If DoAll is non-zero, it will 
pick up all hint files, otherwise it will try to ignore the hints left by 
servers on other machines.  It returns a count of the number of hints that 
could not be taken, generally due to protection failures, in ProtFailures.


\typewriter{\smaller{MS_PrefetchMessage(DirName, id, GetNext)

char *DirName, *id; /* IN */

int MailOnly; /* IN */}}


On a Vice system, this call will tell venus to prefetch a message body file. 
 The message is identified by the DirName and id.  If the GetNext field is 
non-zero, then the message fetched will be the one AFTER the one named.  If 
the message is the last one in a folder and GetNext is non-zero, the error 
code returned will be EINVAL for AMS_ERRNO.  On a non-Vice system, this call 
is a no-op.


If the id is null, then the .MS_MsgDir file (the directory file) will be 
pre-fetched.


\smaller{\typewriter{MS_GetNewMessageCount(FullDirName, numnew, numtotal, 
LastOldDate, InsistOnFetch)

char *FullDirName, /* IN */

        *LastOldDate; /* OUT */

int *numnew, /* OUT */

        *numtotal, /* OUT */

        InsistOnFetch; /* IN */}}


This routine will check a directory for new messages.  It will fill in the 
total number of messages and the number that are new to the current user in 
the numtotal and numnew variables.  It will also fill in the date stamp of the 
last old message in the LastOldDate buffer; this buffer must be at least as 
big as AMS_DATESIZE.  


If InistOnFetch is zero, however, the above description is not quite accurate. 
 In this case, the system will sometimes come up with a quick, short answer 
that there are no new messages.  In this case, the LastOldDate and numtotal 
fields will not be filled in properly; numtotal will be -1 to indicate that 
this is the case.


\typewriter{\smaller{MS_DoIHaveMail(count)

int *count; /* OUT */}}


This routine returns in count the number of pieces of new mail in the user's 
mailbox.


\smaller{\typewriter{MS_SetDeathKnell(dk)

int dk; /* IN */}}


This routine sets the amount of time the message server will wait patiently 
with no activity before it quietly and willfully kills itself.


\typewriter{\smaller{MS_ParseDate(indate, year, month, day, hour, min, sec, 
wday, gtm)

char *indate; /* IN */

int *year, *month, *day, *hour, *min, *sec, *wday; /* ALL OUT */

long *gtm;    /* OUT */}}


This routine parses the date in the indate string and returns its 
interpretation of the date, with the appropriate parts in the appropriate 
places.  gtm is the time as expressed in UNIX gtime() or time(0) format.  The 
date parser invoked here is extremely amazing, and you wouldn't want to have 
to port it to your client machine.


\typewriter{\smaller{MS_HandlePreference(prog, pref, InVal, OutVal, OutLim, 
opcode, resulti, defaulti)}}


This routine is a worker routine for the CUI_GetProfileXXX and 
CUI_SetProfileString calls.  You should not use it directly, but should call 
the CUI_ routines instead. 


\typewriter{\smaller{MS_SetCleanupZombies(doit)

int doit;}}


This routine is a FAKE message server call.   If you call it in the SNAPified 
version, an error message will be printed.  However, it is useful in the 
non-SNAPified versions of programs that want to handle their own zombie 
processes.  If doit is non-zero (the default), the messageserver will continue 
to clean up all zombie processes; if it is zero, such behavior will be 
disabled, and you'd better have your own process cleaning up zombies.


If you don't know what Zombies are on UNIX, you should try not to learn unless 
you need to.  However, if you start running out of processes, that might be 
your problem.


\heading{References (Fill in later)}


[1] Overview paper


[2] User documentation


[3] RFC822


[4] UNIX file system


\begindata{bp,537787456}
\enddata{bp,537787456}
\view{bpv,537787456,0,0,0}
Copyright 1988 Carnegie Mellon University and IBM. All rights reserved.

\smaller{\smaller{$Disclaimer: 

Permission to use, copy, modify, and distribute this software and its 

documentation for any purpose is hereby granted without fee, 

provided that the above copyright notice appear in all copies and that 

both that copyright notice, this permission notice, and the following 

disclaimer appear in supporting documentation, and that the names of 

IBM, Carnegie Mellon University, and other copyright holders, not be 

used in advertising or publicity pertaining to distribution of the software 

without specific, written prior permission.



IBM, CARNEGIE MELLON UNIVERSITY, AND THE OTHER COPYRIGHT HOLDERS 

DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 

ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.  IN NO EVENT 

SHALL IBM, CARNEGIE MELLON UNIVERSITY, OR ANY OTHER COPYRIGHT HOLDER 

BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 

DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, 

WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS 

ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 

OF THIS SOFTWARE.

 $}}

\enddata{text,537620304}
