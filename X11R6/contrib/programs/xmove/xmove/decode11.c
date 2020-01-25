/*                            xmove
 *                            -----
 *             A Pseudoserver For Client Mobility
 *
 *   Copyright (c) 1994         Ethan Solomita
 *
 *   The X Consortium, and any party obtaining a copy of these files from
 *   the X Consortium, directly or indirectly, is granted, free of charge, a
 *   full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *   nonexclusive right and license to deal in this software and
 *   documentation files (the "Software"), including without limitation the
 *   rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons who receive
 *   copies from any such party to do so.  This license includes without
 *   limitation a license to do the foregoing actions under any patents of
 *   the party supplying this software to the X Consortium.
 */
/*
 * Project: XMON - An X protocol monitor
 * 
 * File: decode11.c
 * 
 * Description: Decoding and switching routines for the X11 protocol
 * 
 * There are 4 types of things in X11: requests, replies, errors, and
 * events.
 * 
 * Each of them has a format defined by a small integer that defines the
 * type of the thing.
 * 
 * Requests have an opcode in the first byte.
 *
 * Events have a code in the first byte.
 * 
 * Errors have a code in the second byte (the first byte is 0)
 * 
 * Replies have a sequence number in bytes 2 and 3.  The sequence number
 * should be used to identify the request that was sent, and from that
 * request we can determine the type of the reply.
 * 
 */

#include "common.h"

#include "x11.h"
#include "xmond.h"
#include "linkl.h"

#ifdef XMOVE
#include "xmove.h"
#endif
#include "select_args.h"

#define	MAX_REQUEST			127
#define	MAX_EXT_REQUEST		255

#ifdef XMOVE
  #define	MAX_EVENT			35
#else
  #define	MAX_EVENT			34
#endif

#define	MAX_ERROR			17

/*
 * We need to keep the sequence number for a request to match it with an
 * expected reply.  The sequence number is associated only with the
 * particular connection that we have. We would expect these replies to be
 * handled as a FIFO queue.
*/

typedef struct
{
	unsigned short    SequenceNumber;
	short   Request;
#ifdef XMOVE
	/* these fields are necessary to deal with the */
	/* GetKeyboardMapping request.  See appropriate comments there */

	Bool modified;
	unsigned char client_first_keycode, client_count;
	unsigned char server_first_keycode, server_count;
#endif
}
	QueueEntry;

/* function prototypes: */
/* decode11.c: */
static void SequencedReplyExpected P((short RequestType ));
static short CheckReplyTable P((Server *server,
				unsigned short SequenceNumber,
				Bool checkZero,
				QueueEntry *modified));
/* end function prototypes */

extern Bool					ignore_bytes;

extern int					RequestVerbose;
extern int					EventVerbose;
extern int					ReplyVerbose;
extern int					ErrorVerbose;
extern int					CurrentVerbose;

extern long WriteDescriptors[mskcnt];

static int					Lastfd;
static unsigned short				LastSequenceNumber;
static short				LastReplyType;


Global void
    DecodeRequest(buf, n)
unsigned char			*buf;
long					n;
{
    int						fd = client->fdd->fd;
    short					Request = IByte (&buf[0]);
    
    client->SequenceNumber += 1;
    if (client->SequenceNumber == 0) {
	 client->LastMoveSeqNo = -1; /* the seqno just reset to 0. reset lastmoveseqno. */
/*	 kill(getpid(), 18);*/
    }
    
    ISetLong((unsigned char *)SBf, (unsigned long)client->SequenceNumber);
    
    if (CurrentVerbose = RequestVerbose)
	 SetIndentLevel(PRINTCLIENT);
    
    if (CurrentVerbose > 3)
	DumpItem("Request", fd, buf, IShort(&buf[2]));

    if (Request <= 0 || 127 < Request)
	fprintf
	    (
	     stdout, "Extended request opcode: %d, SequenceNumber: %d\n",
	     Request, client->SequenceNumber
	     );
    else
	switch (Request)
	{
	  case 1:
	    CreateWindow(buf);
	    break;
	  case 2:
	    ChangeWindowAttributes(buf);
	    break;
	  case 3:
	    GetWindowAttributes(buf);
	    ReplyExpected(Request);
	    break;
	  case 4:
	    DestroyWindow(buf);
	    break;
	  case 5:
	    DestroySubwindows(buf);
	    break;
	  case 6:
	    ChangeSaveSet(buf);
	    break;
	  case 7:
	    ReparentWindow(buf);
	    break;
	  case 8:
	    MapWindow(buf);
	    break;
	  case 9:
	    MapSubwindows(buf);
	    break;
	  case 10:
	    UnmapWindow(buf);
	    break;
	  case 11:
	    UnmapSubwindows(buf);
	    break;
	  case 12:
	    ConfigureWindow(buf);
	    break;
	  case 13:
	    CirculateWindow(buf);
	    break;
	  case 14:
	    GetGeometry(buf);
	    ReplyExpected(Request);
	    break;
	  case 15:
	    QueryTree(buf);
	    ReplyExpected(Request);
	    break;
	  case 16:
	    InternAtom(buf);
	    ReplyExpected(Request);
	    break;
	  case 17:
	    GetAtomName(buf);
	    ReplyExpected(Request);
	    break;
	  case 18:
	    ChangeProperty(buf);
	    break;
	  case 19:
	    DeleteProperty(buf);
	    break;
	  case 20:
	    GetProperty(buf);
	    ReplyExpected(Request);
	    break;
	  case 21:
	    ListProperties(buf);
	    ReplyExpected(Request);
	    break;
	  case 22:
	    SetSelectionOwner(buf);
	    break;
	  case 23:
	    GetSelectionOwner(buf);
	    ReplyExpected(Request);
	    break;
	  case 24:
	    ConvertSelection(buf);
	    break;
	  case 25:
	    SendEvent(buf);
	    break;
	  case 26:
	    GrabPointer(buf);
	    ReplyExpected(Request);
	    break;
	  case 27:
	    UngrabPointer(buf);
	    break;
	  case 28:
	    GrabButton(buf);
	    break;
	  case 29:
	    UngrabButton(buf);
	    break;
	  case 30:
	    ChangeActivePointerGrab(buf);
	    break;
	  case 31:
	    GrabKeyboard(buf);
	    ReplyExpected(Request);
	    break;
	  case 32:
	    UngrabKeyboard(buf);
	    break;
	  case 33:
	    GrabKey(buf);
	    break;
	  case 34:
	    UngrabKey(buf);
	    break;
	  case 35:
	    AllowEvents(buf);
	    break;
	  case 36:
	    GrabServer(buf);
	    break;
	  case 37:
	    UngrabServer(buf);
	    break;
	  case 38:
	    QueryPointer(buf);
	    ReplyExpected(Request);
	    break;
	  case 39:
	    GetMotionEvents(buf);
	    ReplyExpected(Request);
	    break;
	  case 40:
	    TranslateCoordinates(buf);
	    ReplyExpected(Request);
	    break;
	  case 41:
	    WarpPointer(buf);
	    break;
	  case 42:
	    SetInputFocus(buf);
	    break;
	  case 43:
	    GetInputFocus(buf);
	    ReplyExpected(Request);
	    break;
	  case 44:
	    QueryKeymap(buf);
	    ReplyExpected(Request);
	    break;
	  case 45:
	    OpenFont(buf);
	    break;
	  case 46:
	    CloseFont(buf);
	    break;
	  case 47:
	    QueryFont(buf);
	    ReplyExpected(Request);
	    break;
	  case 48:
	    QueryTextExtents(buf);
	    ReplyExpected(Request);
	    break;
	  case 49:
	    ListFonts(buf);
	    ReplyExpected(Request);
	    break;
	  case 50:
	    ListFontsWithInfo(buf);
	    ReplyExpected(Request);
	    break;
	  case 51:
	    SetFontPath(buf);
	    break;
	  case 52:
	    GetFontPath(buf);
	    ReplyExpected(Request);
	    break;
	  case 53:
	    CreatePixmap(buf);
	    break;
	  case 54:
	    FreePixmap(buf);
	    break;
	  case 55:
	    CreateGC(buf);
	    break;
	  case 56:
	    ChangeGC(buf);
	    break;
	  case 57:
	    CopyGC(buf);
	    break;
	  case 58:
	    SetDashes(buf);
	    break;
	  case 59:
	    SetClipRectangles(buf);
	    break;
	  case 60:
	    FreeGC(buf);
	    break;
	  case 61:
	    ClearArea(buf);
	    break;
	  case 62:
	    CopyArea(buf);
	    break;
	  case 63:
	    CopyPlane(buf);
	    break;
	  case 64:
	    PolyPoint(buf);
	    break;
	  case 65:
	    PolyLine(buf);
	    break;
	  case 66:
	    PolySegment(buf);
	    break;
	  case 67:
	    PolyRectangle(buf);
	    break;
	  case 68:
	    PolyArc(buf);
	    break;
	  case 69:
	    FillPoly(buf);
	    break;
	  case 70:
	    PolyFillRectangle(buf);
	    break;
	  case 71:
	    PolyFillArc(buf);
	    break;
	  case 72:
	    PutImage(buf);
	    break;
	  case 73:
	    GetImage(buf);
	    ReplyExpected(Request);
	    break;
	  case 74:
	    PolyText8(buf);
	    break;
	  case 75:
	    PolyText16(buf);
	    break;
	  case 76:
	    ImageText8(buf);
	    break;
	  case 77:
	    ImageText16(buf);
	    break;
	  case 78:
	    CreateColormap(buf);
	    break;
	  case 79:
	    FreeColormap(buf);
	    break;
	  case 80:
	    CopyColormapAndFree(buf);
	    break;
	  case 81:
	    InstallColormap(buf);
	    break;
	  case 82:
	    UninstallColormap(buf);
	    break;
	  case 83:
	    ListInstalledColormaps(buf);
	    ReplyExpected(Request);
	    break;
	  case 84:
	    AllocColor(buf);
	    ReplyExpected(Request);
	    break;
	  case 85:
	    AllocNamedColor(buf);
	    ReplyExpected(Request);
	    break;
	  case 86:
	    AllocColorCells(buf);
	    ReplyExpected(Request);
	    break;
	  case 87:
	    AllocColorPlanes(buf);
	    ReplyExpected(Request);
	    break;
	  case 88:
	    FreeColors(buf);
	    break;
	  case 89:
	    StoreColors(buf);
	    break;
	  case 90:
	    StoreNamedColor(buf);
	    break;
	  case 91:
	    QueryColors(buf);
	    ReplyExpected(Request);
	    break;
	  case 92:
	    LookupColor(buf);
	    ReplyExpected(Request);
	    break;
	  case 93:
	    CreateCursor(buf);
	    break;
	  case 94:
	    CreateGlyphCursor(buf);
	    break;
	  case 95:
	    FreeCursor(buf);
	    break;
	  case 96:
	    RecolorCursor(buf);
	    break;
	  case 97:
	    QueryBestSize(buf);
	    ReplyExpected(Request);
	    break;
	  case 98:
	    QueryExtension(buf);
	    ReplyExpected(Request);
	    break;
	  case 99:
	    ListExtensions(buf);
	    ReplyExpected(Request);
	    break;
	  case 100:
	    ChangeKeyboardMapping(buf);
	    break;
	    
	  case 101:
	    /* GetKeyboardMapping() will call ReplyExpected() or */
	    /* ModifiedKeyMappingReplyExpected() by itself, since it */
	    /* needs to determine which to call */  

	    GetKeyboardMapping(buf);
#ifndef XMOVE
	    ReplyExpected(Request);
#endif		
	    break;
	  case 102:
	    ChangeKeyboardControl(buf);
	    break;
	  case 103:
	    GetKeyboardControl(buf);
	    ReplyExpected(Request);
	    break;
	  case 104:
	    Bell(buf);
	    break;
	  case 105:
	    ChangePointerControl(buf);
	    break;
	  case 106:
	    GetPointerControl(buf);
	    ReplyExpected(Request);
	    break;
	  case 107:
	    SetScreenSaver(buf);
	    break;
	  case 108:
	    GetScreenSaver(buf);
	    ReplyExpected(Request);
	    break;
	  case 109:
	    ChangeHosts(buf);
	    break;
	  case 110:
	    ListHosts(buf);
	    ReplyExpected(Request);
	    break;
	  case 111:
	    SetAccessControl(buf);
	    break;
	  case 112:
	    SetCloseDownMode(buf);
	    break;
	  case 113:
	    KillClient(buf);
	    break;
	  case 114:
	    RotateProperties(buf);
	    break;
	  case 115:
	    ForceScreenSaver(buf);
	    break;
	  case 116:
	    SetPointerMapping(buf);
	    ReplyExpected(Request);
	    break;
	  case 117:
	    GetPointerMapping(buf);
	    ReplyExpected(Request);
	    break;
	  case 118:
	    SetModifierMapping(buf);
	    ReplyExpected(Request);
	    break;
	  case 119:
	    GetModifierMapping(buf);
	    ReplyExpected(Request);
	    break;
	  case 127:
	    NoOperation(buf);
	    break;
	  default:
	    fprintf(stdout, "####### Illegal request opcode (%d)\n", Request);
	    break;
	}
}

Global void
DecodeReply(unsigned char *buf, long n)
{
    int				fd = server->fdd->fd;
    unsigned short		SequenceNumber = IShort (&buf[2]);
    short			Request;
#ifdef XMOVE
    QueueEntry			modified;
#endif

    /* LastMoveSeqNo contains the seqno of the last request
       sent during the move process. Ignore any replies that
       were originated during the move process. */
    
    if ((long)SequenceNumber <= client->LastMoveSeqNo) {
	 ignore_bytes = True;
	 return;
    }

    modified.modified = False;
    Request = CheckReplyTable(server, 
			      SequenceNumber, 
			      True,
			      &modified);

    ISetShort(&buf[2], (unsigned short)(SequenceNumber + client->SequenceMapping));
    
#ifdef XMOVE
    if(modified.modified == True)
    {
	Dprintf(("reply for modified request received.\n"))
	Dprintf(("client requested first=%d, count = %d\n", 
	       modified.client_first_keycode, modified.client_count))
	Dprintf(("server reply contains first=%d, count=%d\n", 
	       modified.server_first_keycode, modified.server_count))
    }
#endif

    if (CurrentVerbose = ReplyVerbose)
	 SetIndentLevel(PRINTSERVER);

/*    if (Request == 0)
	return;*/ 

#ifndef XMOVE
    if (CurrentVerbose <= 0)
	return;
#endif

    RBf[0] = Request;		/* for the PrintField in the Reply procedure */


    if (CurrentVerbose > 3)
	DumpItem("Reply", fd, buf, n);

    if (Request <= 0 || 127 < Request)
	fprintf(stdout, "####### Extended reply opcode %d\n", Request);
    else
	switch (Request)
	{
	  case 3:
	    GetWindowAttributesReply(buf);
	    break;
	  case 14:
	    GetGeometryReply(buf);
	    break;
	  case 15:
	    QueryTreeReply(buf);
	    break;
	  case 16:
	    InternAtomReply(buf);
	    break;
	  case 17:
	    GetAtomNameReply(buf);
	    break;
	  case 20:
	    GetPropertyReply(buf);
	    break;
	  case 21:
	    ListPropertiesReply(buf);
	    break;
	  case 23:
	    GetSelectionOwnerReply(buf);
	    break;
	  case 26:
	    GrabPointerReply(buf);
	    break;
	  case 31:
	    GrabKeyboardReply(buf);
	    break;
	  case 38:
	    QueryPointerReply(buf);
	    break;
	  case 39:
	    GetMotionEventsReply(buf);
	    break;
	  case 40:
	    TranslateCoordinatesReply(buf);
	    break;
	  case 43:
	    GetInputFocusReply(buf);
	    break;
	  case 44:
	    QueryKeymapReply(buf);
	    break;
	  case 47:
	    QueryFontReply(buf);
	    break;
	  case 48:
	    QueryTextExtentsReply(buf);
	    break;
	  case 49:
	    ListFontsReply(buf);
	    break;
	  case 50:
	    ListFontsWithInfoReply(buf);
	    break;
	  case 52:
	    GetFontPathReply(buf);
	    break;
	  case 73:
	    GetImageReply(buf);
	    break;
	  case 83:
	    ListInstalledColormapsReply(buf);
	    break;
	  case 84:
	    AllocColorReply(buf);
	    break;
	  case 85:
	    AllocNamedColorReply(buf);
	    break;
	  case 86:
	    AllocColorCellsReply(buf);
	    break;
	  case 87:
	    AllocColorPlanesReply(buf);
	    break;
	  case 91:
	    QueryColorsReply(buf);
	    break;
	  case 92:
	    LookupColorReply(buf);
	    break;
	  case 97:
	    QueryBestSizeReply(buf);
	    break;
	  case 98:
	    QueryExtensionReply(buf);
	    break;
	  case 99:
	    ListExtensionsReply(buf);
	    break;
	  case 101:
#ifdef XMOVE
	    /* this may be a reply for a request that was modified. */
	    /* If so, we need to modify this request */

	    if (modified.modified == True)
	    {
		unsigned char *new_reply;
		long new_reply_size;
		unsigned char n = IByte(&buf[1]); /* keysyms/keycode */
		unsigned long index = modified.server_first_keycode -
		    modified.client_first_keycode;

		/* here, reconstruct the new buffer to send out to the */
		/* client, send it to the client, and set ignore_bytes */
		/* so it is not duplicated */
		

		/* compute the in memory size */
		new_reply_size = 32 + 4 * modified.client_count * n;

		new_reply = Tcalloc(new_reply_size, unsigned char);

		/* copy the header */
		bcopy((char *)buf, (char *)new_reply, 32);

		/* copy the actual data, offset appropriately into the */
		/* destination buffer */

		bcopy((char *) &buf[32],
		      (char *)&new_reply[32+(4*n*index)], 
		      (modified.server_count * n * 4));

		/* poke in the right size information into the header */
		ISetLong(&new_reply[4], (n * modified.client_count));
		   
		/* send it off to this client */
		SaveBytes(&(client->fdd->outBuffer), 
			  new_reply,
			  new_reply_size);

		/* set ignore_bytes so that this packet is not */
		/* transmitted by the main loop as well */
		ignore_bytes = True;
		free(new_reply);
	    }
	    else
#endif
		GetKeyboardMappingReply(buf);
	    break;
	  case 103:
	    GetKeyboardControlReply(buf);
	    break;
	  case 106:
	    GetPointerControlReply(buf);
	    break;
	  case 108:
	    GetScreenSaverReply(buf);
	    break;
	  case 110:
	    ListHostsReply(buf);
	    break;
	  case 116:
	    SetPointerMappingReply(buf);
	    break;
	  case 117:
	    GetPointerMappingReply(buf);
	    break;
	  case 118:
	    SetModifierMappingReply(buf);
	    break;
	  case 119:
	    GetModifierMappingReply(buf);
	    break;
	  default:
	    fprintf(stdout, "####### Unimplemented reply opcode %d\n", Request);
	    break;
	}
}

Global void
DecodeError(buf, n)
	unsigned char			*buf;
	long					n;
{
	int						fd = server->fdd->fd;
	short					Error = IByte (&buf[1]);

	/* LastMoveSeqNo contains the seqno of the last request
	   sent during the most recent move process. Ignore any errors that
	   were originated during the move process. */
    
	if ((long)IShort(&buf[2]) <= client->LastMoveSeqNo) {
	     ignore_bytes = True;
	     return;
	}
	
	Dprintf(("XMOVE Error from fd %d\n",fd))

	(void)CheckReplyTable (server, 
			       (short)IShort(&buf[2]), 
			       False, 
			       (QueueEntry *) NULL);
	
	ISetShort(&buf[2], (unsigned short)(IShort(&buf[2]) + client->SequenceMapping));
	
	CurrentVerbose = ErrorVerbose;
	if (CurrentVerbose <= 0)
		return;
	SetIndentLevel(PRINTSERVER);
	if (CurrentVerbose > 3)
		DumpItem("Error", fd, buf, n);
	if (Error < 1 || Error > 17)
		fprintf(stdout, "####### Extended Error opcode %d\n", Error);
	else
		switch (Error)
		{
		case 1:
			RequestError(buf);
			break;
		case 2:
			ValueError(buf);
			break;
		case 3:
			WindowError(buf);
			break;
		case 4:
			PixmapError(buf);
			break;
		case 5:
			AtomError(buf);
			break;
		case 6:
			CursorError(buf);
			break;
		case 7:
			FontError(buf);
			break;
		case 8:
			MatchError(buf);
			break;
		case 9:
			DrawableError(buf);
			break;
		case 10:
			AccessError(buf);
			break;
		case 11:
			AllocError(buf);
			break;
		case 12:
			ColormapError(buf);
			break;
		case 13:
			GContextError(buf);
			break;
		case 14:
			IDChoiceError(buf);
			break;
		case 15:
			NameError(buf);
			break;
		case 16:
			LengthError(buf);
			break;
		case 17:
			ImplementationError(buf);
			break;
		default:
			fprintf(stdout, "####### Unimplemented Error opcode %d\n", Error);
			break;
		}
}

Global void
DecodeEvent(unsigned char *buf, long n, Bool real_event)
{
    short Event = IByte (&buf[0]) & 0x7f;

    /* LastMoveSeqNo contains the seqno of the last request
       sent during the move process. Ignore any events that
       were originated during the move process. */
    
    if ((long)IShort(&buf[2]) < client->LastMoveSeqNo) {
	 ignore_bytes = True;
	 return;
    }

    ISetShort(&buf[2], (unsigned short)(IShort(&buf[2]) + client->SequenceMapping));
	
    if (real_event)
    {
	if (CurrentVerbose = EventVerbose)
	     SetIndentLevel(PRINTSERVER);

#ifndef XMOVE
	/* I want to be able to run XMove with verbose 0, so it */
	/* doesn't print out any of the junk that is in print11.c */

	if (CurrentVerbose <= 0)
	    return;
#endif

	if (CurrentVerbose > 3)
	    DumpItem("Event", server->fdd->fd, buf, n);
    }

    if (Event < 2 || Event >= MAX_EVENT)
	fprintf(stdout, "####### Extended Event opcode %d\n", Event);
    else
	switch (Event)
	{
	  case 2:
	    KeyPressEvent(buf);
	    break;
	  case 3:
	    KeyReleaseEvent(buf);
	    break;
	  case 4:
	    ButtonPressEvent(buf);
	    break;
	  case 5:
	    ButtonReleaseEvent(buf);
	    break;
	  case 6:
	    MotionNotifyEvent(buf);
	    break;
	  case 7:
	    EnterNotifyEvent(buf);
	    break;
	  case 8:
	    LeaveNotifyEvent(buf);
	    break;
	  case 9:
	    FocusInEvent(buf);
	    break;
	  case 10:
	    FocusOutEvent(buf);
	    break;
	  case 11:
	    KeymapNotifyEvent(buf);
	    break;
	  case 12:
	    ExposeEvent(buf);
	    break;
	  case 13:
	    GraphicsExposureEvent(buf);
	    break;
	  case 14:
	    NoExposureEvent(buf);
	    break;
	  case 15:
	    VisibilityNotifyEvent(buf);
	    break;
	  case 16:
	    CreateNotifyEvent(buf);
	    break;
	  case 17:
	    DestroyNotifyEvent(buf);
	    break;
	  case 18:
	    UnmapNotifyEvent(buf);
	    break;
	  case 19:
	    MapNotifyEvent(buf);
	    break;
	  case 20:
	    MapRequestEvent(buf);
	    break;
	  case 21:
	    ReparentNotifyEvent(buf);
	    break;
	  case 22:
	    ConfigureNotifyEvent(buf);
	    break;
	  case 23:
	    ConfigureRequestEvent(buf);
	    break;
	  case 24:
	    GravityNotifyEvent(buf);
	    break;
	  case 25:
	    ResizeRequestEvent(buf);
	    break;
	  case 26:
	    CirculateNotifyEvent(buf);
	    break;
	  case 27:
	    CirculateRequestEvent(buf);
	    break;
	  case 28:
	    PropertyNotifyEvent(buf);
	    break;
	  case 29:
	    SelectionClearEvent(buf);
	    break;
	  case 30:
	    SelectionRequestEvent(buf);
	    break;
	  case 31:
	    SelectionNotifyEvent(buf);
	    break;
	  case 32:
	    ColormapNotifyEvent(buf);
	    break;
	  case 33:
	    ClientMessageEvent(buf);
	    break;
	  case 34:
	    MappingNotifyEvent(buf);
	    break;
	  default:
	    fprintf(stdout, "####### Unimplemented Event opcode %d\n", Event);
	    break;
	}
}

#ifdef NOT_YET
/*
 * KeepLastReplyExpected:
 * 
 * another reply is expected for the same reply as we just had.  This is
 * only used with ListFontsWithInfo
 */
Global void
KeepLastReplyExpected()
{
	SequencedReplyExpected(Lastfd, LastSequenceNumber, LastReplyType);
}
#endif

/*
 * SequencedReplyExpected:
 * 
 * A reply is expected to the type of request given for the fd associated
 * with this one
 */
static void
SequencedReplyExpected(short RequestType)
{
    unsigned short		SequenceNumber = client->SequenceNumber;
    QueueEntry			*p;

    /* create a new queue entry */
    p = Tmalloc(QueueEntry);
    p->SequenceNumber = SequenceNumber;
    p->Request = RequestType;
#ifdef XMOVE
    p->modified = False;
#endif

    appendToList(&server->reply_list, (Pointer)p);
}

/*
 * CheckReplyTable:
 * 
 * search for the type of request that is associated with a reply to the
 * given sequence number for this fd
 */

static short
CheckReplyTable (Server *server, 
		 unsigned short SequenceNumber, 
		 Bool checkZero,
		 QueueEntry *modified)
{
    int			fd = server->fdd->fd;
    QueueEntry		*p;

    ForAllInList(&server->reply_list)
    {
	p = (QueueEntry *)CurrentContentsOfList(&server->reply_list);
	if (SequenceNumber == p->SequenceNumber)
	{
	    /* save the Request type */
	    Lastfd = fd;
	    LastSequenceNumber = p->SequenceNumber;
	    LastReplyType = p->Request;
#ifdef XMOVE
	    /* if modified is true, then fill in the structure passed */
	    /* to us with the appropriate values */
	    if ((p->modified == True) && (modified != (QueueEntry *) NULL))
	    {
		modified -> modified = True;
		modified -> client_first_keycode = p->client_first_keycode;
		modified -> client_count = p->client_count;
		modified -> server_first_keycode = p->server_first_keycode;
		modified -> server_count = p->server_count;
	    }
#endif
	    /* pull the queue entry out of the queue for this fd */
	    freeCurrent(&server->reply_list);
	    return(LastReplyType);
	}
    }

    /* not expecting a reply for that sequence number */
    if (checkZero)
    {
	printf("Unexpected reply, SequenceNumber: %d.\n", SequenceNumber);

	if (ListIsEmpty(&server->reply_list))
	    printf("No expected replies\n");
	else
	{
	    printf("Expected replies are:\n");
	    ForAllInList(&server->reply_list)
	    {
		p = (QueueEntry *)CurrentContentsOfList(&server->reply_list);
		printf("Reply on fd %d for sequence %d is type %d\n",
		     fd, p->SequenceNumber, p->Request);
	    }
	    printf("End of expected replies\n");
	}
    }
    return(0);
}


/*
 * ReplyExpected:
 * 
 * A reply is expected to the type of request given for the sequence
 * number associated with this fd
 */
Global void
ReplyExpected(short RequestType)
{
    SequencedReplyExpected(RequestType);
}

#ifdef XMOVE
Global void
ModifiedKeyMappingReplyExpected(short RequestType,
				unsigned char client_first_keycode,
				unsigned char client_count,
				unsigned char server_first_keycode,
				unsigned char server_count)
{
    unsigned short		SequenceNumber = client->SequenceNumber;
    QueueEntry			*p;

    /* create a new queue entry */
    p = Tmalloc(QueueEntry);
    p->SequenceNumber = SequenceNumber;
    p->Request = RequestType;
    p->modified = True;
    p->client_first_keycode = client_first_keycode;
    p->client_count = client_count;
    p->server_first_keycode = server_first_keycode;
    p->server_count = server_count;

    appendToList(&server->reply_list, (Pointer)p);
}
#endif
