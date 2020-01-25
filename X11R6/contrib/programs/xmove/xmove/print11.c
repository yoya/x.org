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
 * File: print11.c
 * 
 * Description: Request, Reply, Event, Error Printing
 * 
 * In printing the contents of the fields of the X11 packets, some fields
 * are of obvious value, and others are of lesser value.  To control the
 * amount of output, we generate our output according to the level of
 * verbose-ness that was selected by the user.
 * 
 * CurrentVerbose = 0 ==  Nothing
 * 
 * CurrentVerbose = 1 ==  Headers only (request/reply/... names)
 * 
 * CurrentVerbose = 2 ==  Most interesting fields
 * 
 * CurrentVerbose = 3 ==  All fields
 * 
 * CurrentVerbose = 4 ==  All fields, and hex dump
 * 
 */

/*
 *
 * $Author: ethan $
 * $Date: 1993/02/11 03:14:34 $
 * $Locker: ethan $
 * $Revision: 1.15 $
 * $Log: print11.c,v $
 * Revision 1.15  1993/02/11  03:14:34  ethan
 * *** empty log message ***
 *
 * Revision 1.14  1992/12/21  19:25:23  pds
 * *** empty log message ***
 *
 * Revision 1.13  1992/12/15  18:31:41  ethan
 * stolen back by pds
 *
 * Revision 1.12  1992/12/14  05:05:48  pds
 * *** empty log message ***
 *
 * Revision 1.11  1992/11/21  00:23:30  pds
 * some small bug hunts
 *
 * Revision 1.10  1992/11/11  04:30:57  ethan
 * prints fd source of errors
 *
 * Revision 1.9  1992/11/09  00:13:05  ethan
 * removed various attributes routines
 *
 * Revision 1.7  1992/10/21  05:02:29  ethan
 * *** empty log message ***
 *
 * Revision 1.6  1992/10/20  22:13:38  pds
 * *** empty log message ***
 *
 * Revision 1.5  1992/10/16  16:46:45  pds
 * *** empty log message ***
 *
 * Revision 1.4  1992/10/15  02:22:39  pds
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/13  22:38:38  ethan
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/13  22:36:16  pds
 * *** empty log message ***
 *
 * Revision 1.1  1992/10/13  22:18:18  pds
 * Initial revision
 *
 * Revision 1.1  1992/10/13  22:08:21  pds
 * Initial revision
 *
 * Revision 2.9  1992/04/11  01:05:37  shamash
 * multiple clients work, all events properly managed
 *
 * Revision 2.8  1992/02/22  07:01:07  shamash
 * little/big endian bug fixed
 *
 * Revision 2.7  1992/02/22  05:52:04  shamash
 * keyboard mapping properly done
 *
 * Revision 2.6  1992/02/11  23:10:43  shamash
 * verbose 0 works!
 *
 * Revision 2.5  1992/01/31  06:50:03  shamash
 * emacs is mobile!!
 *
 *
 */

#include <malloc.h>
#include "xmove.h"
#include "x11.h"
#include <X11/Xatom.h>
#include "select_args.h"

extern Bool ignore_bytes;
extern long WriteDescriptors[mskcnt];

void ProcessPutImage(unsigned char *buf);
void ProcessGetImage(unsigned char *buf);

#if 0
ALLOCATION OF RESOURCE IDS
--------------------------

This applies to the X11R4 Xsun server that came from MIT.  OpenWindows
is definitely different. 

XID = 32 bits = resource ID for any resource

0000 0000 0000 0000   0000 0000 0000 0000
cccc cccc ccxx xxxx   xxxx xxxx xxxx xxxx

22 bits for resource XIDS
10 bits for client IDs

#endif

/*
 * To aid in making the choice between level 1 and level 2, we define the
 * following, which does not print relatively unimportant fields.
*/
#define printfield(a,b,c,d,e) if (CurrentVerbose > 2) PrintField(a,b,c,d,e)


/* function prototypes: */
/* print11.c: */
static void PrintFailedSetUpReply P((unsigned char *buf ));
static void PrintSuccessfulSetUpReply P((unsigned char *buf ));

/* end function prototypes */

Global int					CurrentVerbose;

static char *REQUESTHEADER = "............REQUEST";
static char *EVENTHEADER = "..............EVENT";
static char *SYNTHETIC_EVENTHEADER = "....SYNTHETIC EVENT";
static char *ERRORHEADER = "..............ERROR";
static char *REPLYHEADER = "..............REPLY";
/*static GrabPtr GlobGrab;*/
typedef struct {
     Atom    server_atom;
     Bool    delete;
     Window  window;
} GetPropertyTemps;
typedef struct {
     ColormapPtr  cmap;
     card16       red, green, blue;
} AllocCmapTemps;

Global void
PrintSetUpMessage(buf)
		unsigned char *buf;
{
	short   n;
	short   d;

	enterprocedure("PrintSetUpMessage");

	if (CurrentVerbose < 2)
		return;

	SetIndentLevel(PRINTCLIENT);
	PrintField(buf, 0, 1, BYTEMODE, "byte-order");
	PrintField(buf, 2, 2, CARD16, "major-version");
	PrintField(buf, 4, 2, CARD16, "minor-version");
	printfield(buf, 6, 2, DVALUE2(n), "length of name");
	n = IShort(&buf[6]);
	printfield(buf, 8, 2, DVALUE2(d), "length of data");
	d = IShort(&buf[8]);
	PrintString8(&buf[12], n, "authorization-protocol-name");
	PrintString8(&buf[ROUNDUP4((long)(12 + n))], d, "authorization-protocol-data");
}

Global void
PrintSetUpReply(buf)
		unsigned char *buf;
{
    enterprocedure("PrintSetUpReply");

#ifndef XMOVE
    if (CurrentVerbose < 2)
	return;
#endif

    SetIndentLevel(PRINTSERVER);
    if (IByte(&buf[0]))
	PrintSuccessfulSetUpReply(buf);
    else
	PrintFailedSetUpReply(buf);
}


/* what follows are functions to be called when the client sends a
   SendEvent request. Although the mappings are very similar to the
   mappings for events, they are in the opposite direction, they
   should not print the detail information, and should not require
   any special handling unless it is sending ClientMessage. */

Global void
SendKeyPressEvent(unsigned char *buf)
{
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));

    /* child window */
    if (ILong(&buf[16]))
	 ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Request));
}

Global void
SendKeyReleaseEvent(unsigned char *buf)
{
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));

    /* child window */
    if (ILong(&buf[16]))
	 ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Request));
}

Global void
SendButtonPressEvent(unsigned char *buf)
{
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));

    /* child window */
    if (ILong(&buf[16]))
      ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Request));
}

Global void
SendButtonReleaseEvent(unsigned char *buf)
{
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));

    /* child window */
    if (ILong(&buf[16]))
      ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Request));
}

Global void
SendMotionNotifyEvent(unsigned char *buf)
{
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));

    /* child window */
    if (ILong(&buf[16]))
	 ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Request));
}

Global void
SendEnterNotifyEvent(unsigned char *buf)
{
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));

    /* child window */
    if (ILong(&buf[16]))
	 ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Request));
}

Global void
SendLeaveNotifyEvent(unsigned char *buf)
{
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));

    /* child window */
    if (ILong(&buf[16]))
	 ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Request));
}

Global void
SendFocusInEvent(unsigned char *buf)
{
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
}

Global void
SendFocusOutEvent(unsigned char *buf)
{
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
}

Global void
SendKeymapNotifyEvent(unsigned char *buf)
{
     return;
}

Global void
SendExposeEvent(unsigned char *buf)
{
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
}

Global void
SendGraphicsExposureEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapDrawable(ILong(&buf[4]), Request));
}

Global void
SendNoExposureEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapDrawable(ILong(&buf[4]), Request));
}

Global void
SendVisibilityNotifyEvent(unsigned char *buf)
{
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
}

Global void
SendCreateNotifyEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
}

Global void
SendDestroyNotifyEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
}

Global void
SendUnmapNotifyEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
}

Global void
SendMapNotifyEvent(unsigned char *buf)
{
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
}

Global void
SendMapRequestEvent(unsigned char *buf)
{
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
}

Global void
SendReparentNotifyEvent(unsigned char *buf)
{
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));
}

Global void
SendConfigureNotifyEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
    if (ILong(&buf[12]))
	 ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));
}

Global void
SendConfigureRequestEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
    if (ILong(&buf[12]))
	 ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));
}

Global void
SendGravityNotifyEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
}

Global void
SendResizeRequestEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
}

Global void
SendCirculateNotifyEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));
}

Global void
SendCirculateRequestEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
}

Global void
SendPropertyNotifyEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapAtom(ILong(&buf[8]), Request));
}

Global void
SendSelectionClearEvent(unsigned char *buf)
{
    if (ILong(&buf[8]))
	 ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
    ISetLong(&buf[12], MapAtom(ILong(&buf[12]), Request));
}

Global void
SendSelectionRequestEvent(unsigned char *buf)
{
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));
    ISetLong(&buf[16], MapAtom(ILong(&buf[16]), Request));
    ISetLong(&buf[20], MapAtom(ILong(&buf[20]), Request));
    if (ILong(&buf[24]))
	 ISetLong(&buf[24], MapAtom(ILong(&buf[24]), Request));
}

Global void
SendSelectionNotifyEvent(unsigned char *buf)
{
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
    ISetLong(&buf[12], MapAtom(ILong(&buf[12]), Request));
    ISetLong(&buf[16], MapAtom(ILong(&buf[16]), Request));
    if (ILong(&buf[20]))
	 ISetLong(&buf[20], MapAtom(ILong(&buf[20]), Request));
}

Global void
SendColormapNotifyEvent(unsigned char *buf)
{
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    if (ILong(&buf[8]))
	 ISetLong(&buf[8], MapColormapID(ILong(&buf[8]), Request));
}

Global void
SendClientMessageEvent(unsigned char *buf)
{
     Window window, new_window;
     Atom type, new_type;
     AtomPtr atom;
     
     window = new_window = ILong(&buf[4]);
     type = new_type = ILong(&buf[8]);
     
     ISetLong(&buf[4], MapWindowID(window, Request));
     ISetLong(&buf[8], MapAtom(type, Request));
     atom = FindAtom(type);
     
     if (atom && atom->type_map_func) {
	  (atom->type_map_func->MapProperty)(MapSendClientMessage, NULL,
	    &new_type, NULL, atom, False, &new_window, buf+12, 20);
	  
	  if (new_type != type)
	       ISetLong(&buf[8], new_type);
	  
	  if (new_window != window)
	       ISetLong(&buf[4], new_window);

	  if (ignore_bytes) {	/* set by mapping function, it used sync_server() */
	       SendBuffer(server->alt_fd, &buf[0], 44);
	       ++server->alt_seqno;
	  }
     }
}

Global void
SendMappingNotifyEvent(unsigned char *buf)
{
     return;
}


/*
 * Error Printing procedures
 * 
#ifdef XMOVE
 *
 * These routines define all the types of errors that can come back
 * from the server to the client.  We don't bother modifying these,
 * because hopefully a real client won't get any of these 
 *
#endif
 */

Global void
RequestError(unsigned char *buf)
{
    Dprintf(("XMOVE error: Request Error\n"))

    PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Request */ ;

    if (CurrentVerbose < 2)
	return;

    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 8, 2, CARD16, "minor opcode");
    PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
ValueError(buf)
		unsigned char *buf;
{
     Dprintf(("XMOVE error: Value Error\n"))
     
     PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Value */ ;
     if (CurrentVerbose < 2)
	  return;
     printfield(buf, 2, 2, CARD16, "sequence number");
     PrintField(buf, 4, 4, INT32, "bad value");
     PrintField(buf, 8, 2, CARD16, "minor opcode");
     PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
WindowError(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE error: Window Error\n"))
  if (ILong(&buf[4]))
       ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Error));
#endif

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Window */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, CARD32, "bad resource id");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
PixmapError(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE error: Pixmap Error\n"))
  if (ILong(&buf[4]))
       ISetLong(&buf[4], MapPixmapID(ILong(&buf[4]), Error));
#endif

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Pixmap */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, CARD32, "bad resource id");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
AtomError(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE error: Atom Error\n"))
  if (ILong(&buf[4]))
       ISetLong(&buf[4], MapAtom(ILong(&buf[4]), Error));
#endif

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Atom */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, CARD32, "bad atom id");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
CursorError(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE error: Cursor Error\n"))
  if (ILong(&buf[4]))
       ISetLong(&buf[4], MapCursorID(ILong(&buf[4]), Error));
#endif

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Cursor */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, CARD32, "bad resource id");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
FontError(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE error: Font Error\n"))
  if (ILong(&buf[4]))
       ISetLong(&buf[4], MapFontID(ILong(&buf[4]), Error));
#endif

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Font */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, CARD32, "bad resource id");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
MatchError(buf)
		unsigned char *buf;
{
     Dprintf(("XMOVE error: Match Error\n"))

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Match */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
DrawableError(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE error: Drawable Error\n"))
  if (ILong(&buf[4]))
       ISetLong(&buf[4], MapDrawable(ILong(&buf[4]), Error));
#endif

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Drawable */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, CARD32, "bad resource id");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
AccessError(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE error: Access Error\n"))
#endif

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Access */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
AllocError(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE error: Alloc Error\n"))
#endif

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Alloc */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
ColormapError(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE error: Colormap Error\n"))
  if (ILong(&buf[4]))
       ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Error));
#endif

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Colormap */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, CARD32, "bad resource id");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
GContextError(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE error: GContext Error\n"))
  if (ILong(&buf[4]))
       ISetLong(&buf[4], MapGCID(ILong(&buf[4]), Error));
#endif

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* GContext */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, CARD32, "bad resource id");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
IDChoiceError(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     unsigned long id = ILong(&buf[4]);
     Dprintf(("XMOVE error: IDChoice Error\n"));
     ISetLong(&buf[4], MapResource(id, Reply, id));
#endif

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* IDChoice */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, CARD32, "bad resource id");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
NameError(buf)
		unsigned char *buf;
{
     Dprintf(("XMOVE error: Name Error\n"))

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Name */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
LengthError(buf)
		unsigned char *buf;
{
     Dprintf(("XMOVE error: Length Error\n"))


	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Length */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

Global void
ImplementationError(buf)
		unsigned char *buf;
{
     Dprintf(("XMOVE error: Implementation Error\n"))

	PrintField(buf, 1, 1, ERROR, ERRORHEADER) /* Implementation */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 8, 2, CARD16, "minor opcode");
	PrintField(buf, 10, 1, CARD8, "major opcode");
}

/*
 *
 * Event Printing procedures
 *
#ifdef XMOVE
 *
 * These are all the events that the server can send the client.  The
 * changes that are made are relatively minor, usually just involving
 * mapping a window id, or something along those lines.
 *
 * Nothing is remembered from these events.  The appropriate fields
 * are mapped according to their type, and passed along.
 *
#endif
 *
 */

static void
PrintEventName(buf)
		unsigned char *buf;
{
	if (IByte(&buf[0]) & 0x80)
		PrintField(buf, 0, 1, EVENT, SYNTHETIC_EVENTHEADER);
	else
		PrintField(buf, 0, 1, EVENT, EVENTHEADER);
}

Global void
KeyPressEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Event));

    /* child window */
    if (ILong(&buf[16]))
	 ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Event));
#endif

    PrintEventName(buf) /* KeyPress */ ;
    if (CurrentVerbose < 2)
	return;
    PrintField(buf, 1, 1, KEYCODE, "detail");
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, TIMESTAMP, "time");
    PrintField(buf, 8, 4, WINDOW, "root");
    PrintField(buf, 12, 4, WINDOW, "event");
    PrintField(buf, 16, 4, WINDOW, "child");
    PrintField(buf, 20, 2, INT16, "root-x");
    PrintField(buf, 22, 2, INT16, "root-y");
    PrintField(buf, 24, 2, INT16, "event-x");
    PrintField(buf, 26, 2, INT16, "event-y");
    PrintField(buf, 28, 2, SETofKEYBUTMASK, "state");
    PrintField(buf, 30, 1, BOOL, "same-screen");
}

Global void
KeyReleaseEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Event));

    /* child window */
    if (ILong(&buf[16]))
	 ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Event));
#endif
    PrintEventName(buf) /* KeyRelease */ ;
    if (CurrentVerbose < 2)
	return;
    PrintField(buf, 1, 1, KEYCODE, "detail");
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, TIMESTAMP, "time");
    PrintField(buf, 8, 4, WINDOW, "root");
    PrintField(buf, 12, 4, WINDOW, "event");
    PrintField(buf, 16, 4, WINDOW, "child");
    PrintField(buf, 20, 2, INT16, "root-x");
    PrintField(buf, 22, 2, INT16, "root-y");
    PrintField(buf, 24, 2, INT16, "event-x");
    PrintField(buf, 26, 2, INT16, "event-y");
    PrintField(buf, 28, 2, SETofKEYBUTMASK, "state");
    PrintField(buf, 30, 1, BOOL, "same-screen");
}

Global void
ButtonPressEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Event));

    /* child window */
    if (ILong(&buf[16]))
      ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Event));
#endif

    PrintEventName(buf) /* ButtonPress */ ;
    
    if (CurrentVerbose < 2)
	return;

    PrintField(buf, 1, 1, BUTTON, "detail");
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, TIMESTAMP, "time");
    PrintField(buf, 8, 4, WINDOW, "root");
    PrintField(buf, 12, 4, WINDOW, "event");
    PrintField(buf, 16, 4, WINDOW, "child");
    PrintField(buf, 20, 2, INT16, "root-x");
    PrintField(buf, 22, 2, INT16, "root-y");
    PrintField(buf, 24, 2, INT16, "event-x");
    PrintField(buf, 26, 2, INT16, "event-y");
    PrintField(buf, 28, 2, SETofKEYBUTMASK, "state");
    PrintField(buf, 30, 1, BOOL, "same-screen");
}

Global void
ButtonReleaseEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Event));

    /* child window */
    if (ILong(&buf[16]))
      ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Event));
#endif

    PrintEventName(buf) /* ButtonRelease */ ;
    if (CurrentVerbose < 2)
	return;
    PrintField(buf, 1, 1, BUTTON, "detail");
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, TIMESTAMP, "time");
    PrintField(buf, 8, 4, WINDOW, "root");
    PrintField(buf, 12, 4, WINDOW, "event");
    PrintField(buf, 16, 4, WINDOW, "child");
    PrintField(buf, 20, 2, INT16, "root-x");
    PrintField(buf, 22, 2, INT16, "root-y");
    PrintField(buf, 24, 2, INT16, "event-x");
    PrintField(buf, 26, 2, INT16, "event-y");
    PrintField(buf, 28, 2, SETofKEYBUTMASK, "state");
    PrintField(buf, 30, 1, BOOL, "same-screen");
}

Global void
MotionNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Event));

    /* child window */
    if (ILong(&buf[16]))
	 ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Event));
#endif

	PrintEventName(buf) /* MotionNotify */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, MOTIONDETAIL, "detail");
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, TIMESTAMP, "time");
	PrintField(buf, 8, 4, WINDOW, "root");
	PrintField(buf, 12, 4, WINDOW, "event");
	PrintField(buf, 16, 4, WINDOW, "child");
	PrintField(buf, 20, 2, INT16, "root-x");
	PrintField(buf, 22, 2, INT16, "root-y");
	PrintField(buf, 24, 2, INT16, "event-x");
	PrintField(buf, 26, 2, INT16, "event-y");
	PrintField(buf, 28, 2, SETofKEYBUTMASK, "state");
	PrintField(buf, 30, 1, BOOL, "same-screen");
}

Global void
EnterNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Event));

    /* child window */
    if (ILong(&buf[16]))
	 ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Event));
#endif

	PrintEventName(buf) /* EnterNotify */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, ENTERDETAIL, "detail");
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, TIMESTAMP, "time");
	PrintField(buf, 8, 4, WINDOW, "root");
	PrintField(buf, 12, 4, WINDOW, "event");
	PrintField(buf, 16, 4, WINDOW, "child");
	PrintField(buf, 20, 2, INT16, "root-x");
	PrintField(buf, 22, 2, INT16, "root-y");
	PrintField(buf, 24, 2, INT16, "event-x");
	PrintField(buf, 26, 2, INT16, "event-y");
	PrintField(buf, 28, 2, SETofKEYBUTMASK, "state");
	PrintField(buf, 30, 1, BUTTONMODE, "mode");
	PrintField(buf, 31, 1, SCREENFOCUS, "same-screen, focus");
}

Global void
LeaveNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* root window */
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));

    /* event window */
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Event));

    /* child window */
    if (ILong(&buf[16]))
	 ISetLong(&buf[16], MapWindowID(ILong(&buf[16]), Event));
#endif
	PrintEventName(buf) /* LeaveNotify */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, ENTERDETAIL, "detail");
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, TIMESTAMP, "time");
	PrintField(buf, 8, 4, WINDOW, "root");
	PrintField(buf, 12, 4, WINDOW, "event");
	PrintField(buf, 16, 4, WINDOW, "child");
	PrintField(buf, 20, 2, INT16, "root-x");
	PrintField(buf, 22, 2, INT16, "root-y");
	PrintField(buf, 24, 2, INT16, "event-x");
	PrintField(buf, 26, 2, INT16, "event-y");
	PrintField(buf, 28, 2, SETofKEYBUTMASK, "state");
	PrintField(buf, 30, 1, BUTTONMODE, "mode");
	PrintField(buf, 31, 1, SCREENFOCUS, "same-screen, focus");
}

Global void
FocusInEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
#endif

    PrintEventName(buf) /* FocusIn */ ;
    if (CurrentVerbose < 2)
	return;
    PrintField(buf, 1, 1, ENTERDETAIL, "detail");
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "event");
    PrintField(buf, 8, 1, BUTTONMODE, "mode");
}

Global void
FocusOutEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
#endif

    PrintEventName(buf) /* FocusOut */ ;
    if (CurrentVerbose < 2)
	return;
    PrintField(buf, 1, 1, ENTERDETAIL, "detail");
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "event");
    PrintField(buf, 8, 1, BUTTONMODE, "mode");
}


#ifdef XMOVE
/*
 * this event is the equivalent of EnterNotify and FocusIn, but for
 * the keyboard focus.
 *
 * Since no resources are passed in this event, nothing is done
 */
#endif

Global void
KeymapNotifyEvent(unsigned char *buf)
{
	PrintEventName(buf) /* KeymapNotify */ ;
	if (CurrentVerbose < 2)
		return;
	PrintBytes(&buf[1], (long)31,"keys");
}

Global void
ExposeEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
#endif

    PrintEventName(buf) /* Expose */ ;

    if (CurrentVerbose < 2)
	return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 2, CARD16, "x");
    PrintField(buf, 10, 2, CARD16, "y");
    PrintField(buf, 12, 2, CARD16, "width");
    PrintField(buf, 14, 2, CARD16, "height");
    PrintField(buf, 16, 2, CARD16, "count");
}

Global void
GraphicsExposureEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapDrawable(ILong(&buf[4]), Event));
#endif

	PrintEventName(buf) /* GraphicsExposure */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, DRAWABLE, "drawable");
	PrintField(buf, 8, 2, CARD16, "x");
	PrintField(buf, 10, 2, CARD16, "y");
	PrintField(buf, 12, 2, CARD16, "width");
	PrintField(buf, 14, 2, CARD16, "height");
	PrintField(buf, 16, 2, CARD16, "minor-opcode");
	PrintField(buf, 18, 2, CARD16, "count");
	PrintField(buf, 20, 1, CARD8, "major-opcode");
}

#ifdef XMOVE
/*
 * This event is the opposite of GraphicsExpose, so that a client that
 * requests these events always gets one.  This one means "A Graphics
 * Expose is not going to happen, so don't worry about it."  This way,
 * a client can block waiting for an event after a XCopyRegion() or
 * some other request that can generate one, and not worry about not
 * getting one
 */
#endif

Global void
NoExposureEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapDrawable(ILong(&buf[4]), Event));
#endif
    
    PrintEventName(buf) /* NoExposure */ ;

    if (CurrentVerbose < 2)
	return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 2, CARD16, "minor-opcode");
    PrintField(buf, 10, 1, CARD8, "major-opcode");
}

#ifdef XMOVE
/*
 * All VisibilityNotify events caused by a hierarchy change are
 * generated after any hierarchy event caused by that change 
 */
#endif

Global void
VisibilityNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
#endif

    PrintEventName(buf) /* VisibilityNotify */ ;
    if (CurrentVerbose < 2)
	return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 1, VISIBLE, "state");
}

/*
 * This event is reported to clients selecting SubstructureNotify on
 * the parent and is generated when the window is created.  The
 * arguments are as in the CreateWindow request
 */

Global void
CreateNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
#endif

	PrintEventName(buf) /* CreateNotify */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, WINDOW, "parent");
	PrintField(buf, 8, 4, WINDOW, "window");
	PrintField(buf, 12, 2, INT16, "x");
	PrintField(buf, 14, 2, INT16, "y");
	PrintField(buf, 16, 2, CARD16, "width");
	PrintField(buf, 18, 2, CARD16, "height");
	PrintField(buf, 20, 2, CARD16, "border-width");
	PrintField(buf, 22, 1, BOOL, "override-redirect");
}

/*
 * This event is reported to clients selecting StructureNotify on the
 * window and to clients selecting SubstructureNotify on the parent.  It
 * is generated when the window is destroyed.
 */

Global void
DestroyNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
#endif
	PrintEventName(buf) /* DestroyNotify */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, WINDOW, "event");
	PrintField(buf, 8, 4, WINDOW, "window");
}

/*
 * This event is reported to clients selecting StructureNotify on the
 * window and to clients selecting SubstructureNotify on the parent.  It
 * is generated when the window changes state from mapped to unmapped.
 */

Global void
UnmapNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
#endif
    PrintEventName(buf) /* UnmapNotify */ ;
    if (CurrentVerbose < 2)
	return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "event");
    PrintField(buf, 8, 4, WINDOW, "window");
    PrintField(buf, 12, 1, BOOL, "from-configure");
}


/*
 * This event is reported to clients selecting StructureNotify on the
 * window and to clients selecting SubstructureNotify on the parent.  It
 * is generated when the window changes state from unmapped to mapped.
 */

Global void
MapNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
#endif
    
    PrintEventName(buf) /* MapNotify */ ;
    if (CurrentVerbose < 2)
	return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "event");
    PrintField(buf, 8, 4, WINDOW, "window");
    PrintField(buf, 12, 1, BOOL, "override-redirect");
}

/*
 * This event is reported to the client selecting SubstructureRedirect
 * on the parent and is generated when a MapWindow request is issued on
 * an unmapped window with an override-redirect attribute of False
 */

Global void
MapRequestEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
#endif

    PrintEventName(buf) /* MapRequest */ ;
    if (CurrentVerbose < 2)
	return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "parent");
    PrintField(buf, 8, 4, WINDOW, "window");
}

/*
 * This event is generated when the window is reparented 
 */

Global void
ReparentNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* window */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Event));
#endif

	PrintEventName(buf) /* ReparentNotify */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, WINDOW, "event");
	PrintField(buf, 8, 4, WINDOW, "window");
	PrintField(buf, 12, 4, WINDOW, "parent");
	PrintField(buf, 16, 2, INT16, "x");
	PrintField(buf, 18, 2, INT16, "y");
	PrintField(buf, 20, 1, BOOL, "override-redirect");
}

/*
 * This event is generated when a ConfigureWindow request actually
 * changes the state of the window
 */

Global void
ConfigureNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
    if (ILong(&buf[12]))
	 ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Event));
#endif

    PrintEventName(buf) /* ConfigureNotify */ ;
    if (CurrentVerbose < 2)
	return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "event");
    PrintField(buf, 8, 4, WINDOW, "window");
    PrintField(buf, 12, 4, WINDOW, "above-sibling");
    PrintField(buf, 16, 2, INT16, "x");
    PrintField(buf, 18, 2, INT16, "y");
    PrintField(buf, 20, 2, CARD16, "width");
    PrintField(buf, 22, 2, CARD16, "height");
    PrintField(buf, 24, 2, CARD16, "border-width");
    PrintField(buf, 26, 1, BOOL, "override-redirect");
}

/*
 * This event is generated when a ConfigureWindow request is issued on
 * the window by some other client
 */

Global void
ConfigureRequestEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
    if (ILong(&buf[12]))
	 ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Event));
#endif

	PrintEventName(buf) /* ConfigureRequest */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, STACKMODE, "stack-mode");
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, WINDOW, "parent");
	PrintField(buf, 8, 4, WINDOW, "window");
	PrintField(buf, 12, 4, WINDOW, "sibling");
	PrintField(buf, 16, 2, INT16, "x");
	PrintField(buf, 18, 2, INT16, "y");
	PrintField(buf, 20, 2, CARD16, "width");
	PrintField(buf, 22, 2, CARD16, "height");
	PrintField(buf, 24, 2, CARD16, "border-width");
	PrintField(buf, 26, 2, CONFIGURE_BITMASK, "value-mask");
}

/*
 * This event is generated when a window is moved because of a change
 * in size of the parent.
 */

Global void
GravityNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
#endif

	PrintEventName(buf) /* GravityNotify */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, WINDOW, "event");
	PrintField(buf, 8, 4, WINDOW, "window");
	PrintField(buf, 12, 2, INT16, "x");
	PrintField(buf, 14, 2, INT16, "y");
}

/*
 * This event is reported to the client selecting ResizeRedirect on
 * the window and is generated when a ConfigureWIndow request by some
 * other client on the window attempts to change the size of the window. 
 */

Global void
ResizeRequestEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
#endif

    PrintEventName(buf) /* ResizeRequest */ ;
    if (CurrentVerbose < 2)
	return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 2, CARD16, "width");
    PrintField(buf, 10, 2, CARD16, "height");
}


/*
 * This event is generated when the window is actually restacked from
 * a CirculateWindow request.
 */

Global void
CirculateNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
    ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Event));
#endif

	PrintEventName(buf) /* CirculateNotify */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, WINDOW, "event");
	PrintField(buf, 8, 4, WINDOW, "window");
	PrintField(buf, 12, 4, WINDOW, "parent");
	PrintField(buf, 16, 1, CIRSTAT, "place");
}

/* 
 * This event is generated when a CirculateWindow request is issued on
 * the parent and a window actually needs to be restacked.
 */

Global void
CirculateRequestEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
#endif

    PrintEventName(buf) /* CirculateRequest */ ;
    if (CurrentVerbose < 2)
	return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "parent");
    PrintField(buf, 8, 4, WINDOW, "window");

    /* bytes 12..15 are not used, for some strange reason */

    PrintField(buf, 16, 1, CIRSTAT, "place");
}

/*
 * This event is generated with state NewValue when a property of the
 * window is changed using ChangeProperty or RotateProperties
 */

Global void
PropertyNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    ISetLong(&buf[8], MapAtom(ILong(&buf[8]), Event));
#endif

    PrintEventName(buf) /* PropertyNotify */ ;
    if (CurrentVerbose < 2)
	return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 4, ATOM, "atom");
    PrintField(buf, 12, 4, TIMESTAMP, "time");
    PrintField(buf, 16, 1, PROPCHANGE, "state");
}

/*
 * This event is reported to the current owner of a selection and is
 * generated when a new owner is being defined by means of
 * SetSelectionOwner.  
 */

Global void
SelectionClearEvent(unsigned char *buf)
{
#ifdef XMOVE
    if (ILong(&buf[8]))
	 ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
    ISetLong(&buf[12], MapAtom(ILong(&buf[12]), Event));
#endif

    PrintEventName(buf) /* SelectionClear */ ;
    if (CurrentVerbose < 2)
	return;
    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 4, TIMESTAMP, "time");
    PrintField(buf, 8, 4, WINDOW, "owner");
    PrintField(buf, 12, 4, ATOM, "selection");
}

/*
 * This event is generated when a client issues a ConvertSelection
 * request.
 */ 

Global void
SelectionRequestEvent(unsigned char *buf)
{
#ifdef XMOVE
     Window requestor = ILong(&buf[12]);
     Atom   target    = ILong(&buf[20]);
     Atom   property  = ILong(&buf[24]);
     
     Window new_requestor = requestor;
     Atom   new_target    = target;
     Atom   new_property  = property;
     
     AtomPtr atom;
     
     ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
     ISetLong(&buf[12], MapWindowID(requestor, Event));
     ISetLong(&buf[16], MapAtom(ILong(&buf[16]), Event));
     ISetLong(&buf[20], MapAtom(target, Event));
     if (property)
	  ISetLong(&buf[24], MapAtom(property, Event));

     atom = FindServerAtom(target);

     /* NOTE: we don't pass the selection because MapSelectionRequest must be
	fakeable by MULTIPLE_map and MULTIPLE_map won't know the selection. */
     
     if (atom->type_map_func) {
	  (atom->type_map_func->MapProperty)(MapSelectionRequest,
            &new_property, &new_target, 0, atom,
            0, &requestor, NULL, 0);

	  if (new_requestor != requestor)
	       ISetLong(&buf[12], new_requestor);

	  if (new_target != target)
	       ISetLong(&buf[20], new_target);

	  if (new_property != property)
	       ISetLong(&buf[24], new_property);
     }
     
#endif

	PrintEventName(buf) /* SelectionRequest */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, TIMESTAMP, "time");
	PrintField(buf, 8, 4, WINDOW, "owner");
	PrintField(buf, 12, 4, WINDOW, "requestor");
	PrintField(buf, 16, 4, ATOM, "selection");
	PrintField(buf, 20, 4, ATOM, "target");
	PrintField(buf, 24, 4, ATOM, "property");
}

/* 
 * This event is generated by the server in reponse to a
 * ConvertSelection request when there is no owner for the selection
 */

Global void
SelectionNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Event));
    ISetLong(&buf[12], MapAtom(ILong(&buf[12]), Event));
    ISetLong(&buf[16], MapAtom(ILong(&buf[16]), Event));
    if (ILong(&buf[20]))
	 ISetLong(&buf[20], MapAtom(ILong(&buf[20]), Event));
#endif

	PrintEventName(buf) /* SelectionNotify */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, TIMESTAMP, "time");
	PrintField(buf, 8, 4, WINDOW, "requestor");
	PrintField(buf, 12, 4, ATOM, "selection");
	PrintField(buf, 16, 4, ATOM, "target");
	PrintField(buf, 20, 4, ATOM, "property");
}

/*
 * This event is generated with value True for new when the colormap
 * attribute of the window is changed and is generated with value False
 * for new when the colormap of a window is installed or uninstalled. 
 */

Global void
ColormapNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Event));
    if (ILong(&buf[8]))
	 ISetLong(&buf[8], MapColormapID(ILong(&buf[8]), Event));
#endif

	PrintEventName(buf) /* ColormapNotify */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, WINDOW, "window");
	PrintField(buf, 8, 4, COLORMAP, "colormap");
	PrintField(buf, 12, 1, BOOL, "new");
	PrintField(buf, 13, 1, CMAPCHANGE, "state");
}

/* 
 * This event is only generated by clients using SendEvent.  The type
 * specifies how the data is to be interpreted by the receiving client;
 * the server places no interpretation on the type or the data.
 */

Global void
ClientMessageEvent(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     Atom server_type = ILong(&buf[8]);
     Atom new_type = server_type;
     Window server_window = ILong(&buf[4]);
     Window new_window = server_window;
     AtomPtr atom;
     
     ISetLong(&buf[4], MapWindowID(server_window, Event));
     ISetLong(&buf[8], MapAtom(server_type, Event));
     atom = FindServerAtom(server_type);
     
     if (atom && atom->type_map_func) {
	  (atom->type_map_func->MapProperty)(MapClientMessage, NULL,
	     &new_type, NULL, atom, False, &new_window, buf+12, 20);

	  if (new_window != server_window)
	       ISetLong(&buf[4], new_window);
	  
	  if (new_type != server_type)
	       ISetLong(&buf[8], new_type);
     }
#endif

	PrintEventName(buf) /* ClientMessage */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, CARD8, "format");
	printfield(buf, 2, 2, CARD16, "sequence number");
	PrintField(buf, 4, 4, WINDOW, "window");
	PrintField(buf, 8, 4, ATOM, "type");
	PrintBytes(&buf[12], (long)20,"data");
}

/*
 * This event is sent to all clients.  There is no mechanism to
 * express disinterest in this event.  The detail indicates the kind of
 * change that occurred.
 */

Global void
MappingNotifyEvent(unsigned char *buf)
{
#ifdef XMOVE
    /* XXX - this is a gross hack, but it has to be done.  Set the */
    /* first-keycode to 0x0, and the count to 0xff, so that the entire */
    /* range of keys is invalidated.  This is used when the client */
    /* switches to a new server that has a different keyboard mapping */

    ISetByte(&buf[5], GetCurrentClientMinKeycode());
    ISetByte(&buf[6], GetCurrentClientMaxKeycode());
#endif
    PrintEventName(buf) /* MappingNotify */ ;

    if (CurrentVerbose < 2)
	return;

    printfield(buf, 2, 2, CARD16, "sequence number");
    PrintField(buf, 4, 1, MAPOBJECT, "request");
    PrintField(buf, 5, 1, KEYCODE, "first-keycode");
    PrintField(buf, 6, 1, CARD8, "count");
}

#ifdef XMOVE
/*
 * Here begins the Request/Reply section of this protocol analyzer.
 * This is probably the most complex section of this code, and is
 * certainly the heart of all the automagic that happens.  Unlike most
 * of the events, which (mostly) simply required mapping of XID
 * resource ID's, most of the requests demand an action, such as
 * adding the resource being created to a list, or something like that
 */
#endif

/*
 * Request and Reply Printing procedures
 */

/* ES 1/4/93 17:30  window_id now stores the entire resource ID, client
   side and all. Before that was being removed, but this would be disastrous
   for conflicts between IDs: what if a window is the child of the root
   window (usually 0x24), and at the same time we have a window that has
   id 24? In particular, if we tag descendants of 0x24 we will enter
   recursion hell.
*/

Global void
CreateWindow(unsigned char *buf)
{
#ifdef XMOVE

    WindowPtr temp_window;

    temp_window = Tcalloc(1, WindowRec);

    temp_window -> window_id = ILong(&buf[4]);
    temp_window -> parent_id = ILong(&buf[8]);
    temp_window -> x = IShort(&buf[12]);
    temp_window -> y = IShort(&buf[14]);
    temp_window -> width = IShort(&buf[16]);
    temp_window -> height = IShort(&buf[18]);
    temp_window -> border_width = IShort(&buf[20]);
    temp_window -> class = IShort(&buf[22]);
    temp_window -> visual.visualid = ILong(&buf[24]);
    temp_window -> depth = IByte(&buf[1]);
    temp_window -> attributes_mask = 0;
    temp_window -> cmap = (ColormapPtr)client->colormap_list.top->contents;
    temp_window -> mapped = 0;
    temp_window -> mapped_order = 0;
    temp_window -> submapped_order = 0;
    temp_window -> tag = 0;
    temp_window -> Raise_Lowest = 0;
    temp_window -> Lower_Highest = 0;
    AddWindowToCurrentClient(temp_window);

    ParseWindowBitmaskValueList(temp_window,
				(unsigned long) ILong(&buf[28]),/* bitmask */
				(unsigned char *) &buf[32]);   /* value list */

    /* map the window ID's */

    if (temp_window->depth > 1)
	 ISetByte(&buf[1], server->default_depth);
    
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));

    if (ILong(&buf[24]))
      ISetLong(&buf[24], MapVisualID(ILong(&buf[24]), Request));

    MapWindowBitmaskValueList(temp_window,
				(unsigned long) ILong(&buf[28]),/* bitmask */
				(unsigned char *) &buf[32]);   /* value list */

    /* if client isn't using screen 0, it'll parent its top-level windows
       to a different root window than is stored in client->root_window.
       Change client->root_window and server->root_window to reflect the
       changes. */

    if (((temp_window->parent_id & ~client->resource_mask) == 0) &&
	(temp_window->parent_id != client->root_window)) {
	 client->root_window = temp_window->parent_id;
	 if (!client->xmoved_to_elsewhere)
	      server->root_window = temp_window->parent_id;
    }
#endif

    /* Request CreateWindow is opcode 1 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CreateWindow */ ;

    if (CurrentVerbose < 2)
	return;

    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    PrintField(buf, 1, 1, CARD8, "depth");
    printfield(buf, 2, 2, DVALUE2(8 + n), "request length");
    PrintField(buf, 4, 4, WINDOW, "wid");
    PrintField(buf, 8, 4, WINDOW, "parent");
    PrintField(buf, 12, 2, INT16, "x");
    PrintField(buf, 14, 2, INT16, "y");
    PrintField(buf, 16, 2, CARD16, "width");
    PrintField(buf, 18, 2, CARD16, "height");
    PrintField(buf, 20, 2, CARD16, "border-width");
    PrintField(buf, 22, 2, WINDOWCLASS, "class");
    PrintField(buf, 24, 4, VISUALIDC, "visual");
    PrintField(buf, 28, 4, WINDOW_BITMASK, "value-mask");
    PrintValues(&buf[28], 4, WINDOW_BITMASK, &buf[32], "value-list");

}

Global void
ChangeWindowAttributes(buf)
unsigned char *buf;
{
#ifdef XMOVE
    WindowPtr temp_window;

    Dprintf(("XMOVE ChangeWindowAttribute on window 0x%x\n",ILong(&buf[4])))

    temp_window = FindWindowFromCurrentClient((Window) ILong(&buf[4]));

    if(temp_window == (WindowPtr) NULL)
      Dprintf(("ChangeWindowAttributes:  invalid window id.\n"))

    else
      ParseWindowBitmaskValueList((WindowPtr) temp_window,
				  (unsigned long) ILong(&buf[8]),
				  (unsigned char *) &buf[12]);

    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));

    if (temp_window)
	 MapWindowBitmaskValueList(temp_window,
				   (unsigned long) ILong(&buf[8]),/* bitmask */
				   (unsigned char *) &buf[12]);/* value list */


#endif

    /* Request ChangeWindowAttributes is opcode 2 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ChangeWindowAttributes */ ;

    if (CurrentVerbose < 2)
	return;

    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");
    
    printfield(buf, 2, 2, DVALUE2(3 + n), "request length");
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 4, WINDOW_BITMASK, "value-mask");
    PrintValues(&buf[8], 4, WINDOW_BITMASK, &buf[12], "value-list");
}

Global void
GetWindowAttributes(unsigned char * buf)
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

    /* Request GetWindowAttributes is opcode 3 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetWindowAttributes */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 2, 2, CONST2(2), "request length");
    PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
GetWindowAttributesReply(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     ColormapPtr cmap;
     Colormap cmap_id = ILong(&buf[28]);
     
     ISetLong(&buf[8], MapVisualID(ILong(&buf[8]), Reply));
     
     if (cmap_id) {
	  cmap_id = MapColormapID(cmap_id, Reply);
	  cmap = FindColormapFromCurrentClient(cmap_id);
	  if (cmap == NULL)
	       cmap = (ColormapPtr)client->colormap_list.top->contents;
	  ISetLong(&buf[28], cmap_id);
     } else {
	  cmap = (ColormapPtr)client->colormap_list.top->contents;
     }
     
     ISetLong(&buf[20], MapColorCell(ILong(&buf[20]), cmap, Reply));

#endif  
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetWindowAttributes */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, BACKSTORE, "backing-store");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(3), "reply length");
	PrintField(buf, 8, 4, VISUALID, "visual");
	PrintField(buf, 12, 2, WINDOWCLASS, "class");
	PrintField(buf, 14, 1, BITGRAVITY, "bit-gravity");
	PrintField(buf, 15, 1, WINGRAVITY, "win-gravity");
	PrintField(buf, 16, 4, CARD32, "backing-planes");
	PrintField(buf, 20, 4, CARD32, "backing-pixel");
	PrintField(buf, 24, 1, BOOL, "save-under");
	PrintField(buf, 25, 1, BOOL, "map-is-installed");
	PrintField(buf, 26, 1, MAPSTATE, "map-state");
	PrintField(buf, 27, 1, BOOL, "override-redirect");
	PrintField(buf, 28, 4, COLORMAP, "colormap");
	PrintField(buf, 32, 4, SETofEVENT, "all-event-masks");
	PrintField(buf, 36, 4, SETofEVENT, "your-event-mask");
	PrintField(buf, 40, 2, SETofDEVICEEVENT, "do-not-propagate-mask");
}

Global void
DestroyWindow(unsigned char *buf)
{
#ifdef XMOVE
     WindowPtr window;
     
     /* for this request, we have to remove the window structure from
	this particular client, if it exists. As well, DestroyWindow
	automatically destroys all subwindows, so remove them too. */

     if (window = FindWindowFromCurrentClient(ILong(&buf[4]))) {
	  TagAllDescendants(ILong(&buf[4]));
	  window->tag = 1;
	  DestroyTaggedWindows();
     }
     
     ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

	/* Request DestroyWindow is opcode 4 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* DestroyWindow */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
DestroySubwindows(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  TagAllDescendants(ILong(&buf[4]));
  DestroyTaggedWindows();

  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif
  
	/* Request DestroySubwindows is opcode 5 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* DestroySubwindows */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
ChangeSaveSet(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif  
  
	/* Request ChangeSaveSet is opcode 6 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ChangeSaveSet */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, INS_DEL, "mode");
	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
ReparentWindow(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  WindowPtr temp_window;
  Window window     = ILong(&buf[4]);
  Window new_parent = ILong(&buf[8]);
  
  ISetLong(&buf[4], MapWindowID(window,     Request));
  ISetLong(&buf[8], MapWindowID(new_parent, Request));

  Dprintf(("XMOVE ReparentWindow Request on window 0x%x\n",ILong(&buf[4])))

  temp_window = FindWindowFromCurrentClient(window);
    
  if(temp_window == NULL)
       Dprintf(("ReparentNotifyEvent:  invalid window id.\n"))
  else
       temp_window->parent_id = new_parent;
  
#endif

	/* Request ReparentWindow is opcode 7 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ReparentWindow */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(4), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
	PrintField(buf, 8, 4, WINDOW, "parent");
	PrintField(buf, 12, 2, INT16, "x");
	PrintField(buf, 14, 2, INT16, "y");
}

Global void
MapWindow(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  int order;
  WindowPtr temp_window;

  order = GetCurrentClientMaxMappedOrder();
  order++;
  SetCurrentClientMaxMappedOrder(order);
  
  Dprintf(("XMOVE set window mapping on window %x\n",ILong(&buf[4])))
  Dprintf(("XMOVE setting mapped_order to %d\n", order))

  temp_window = FindWindowFromCurrentClient(ILong(&buf[4]));
  if (temp_window)
       temp_window->mapped_order = order;
  
  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

	/* Request MapWindow is opcode 8 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* MapWindow */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
MapSubwindows(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  int order;
  WindowPtr temp_window;

  order = GetCurrentClientMaxMappedOrder();
  order++;
  SetCurrentClientMaxMappedOrder(order);
  
  Dprintf(("XMOVE set subwindow mapping on window %x\n",ILong(&buf[4])))
  Dprintf(("XMOVE setting submapped_order to %d\n", order))
  temp_window = FindWindowFromCurrentClient(ILong(&buf[4]));
  if (temp_window)
       temp_window->submapped_order = order;

  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

	/* Request MapSubwindows is opcode 9 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* MapSubwindows */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
UnmapWindow(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  WindowPtr temp_window;

  temp_window = FindWindowFromCurrentClient(ILong(&buf[4]));
  if (temp_window)
       temp_window->mapped_order = 0;


  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

	/* Request UnmapWindow is opcode 10 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* UnmapWindow */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
UnmapSubwindows(buf)
		unsigned char *buf;
{
#ifdef XMOVE

/* this requuires more work...   The unmap need to be done ala mapsubwindows
   using the stacking order */

  WindowPtr temp_window;

  temp_window = FindWindowFromCurrentClient(ILong(&buf[4]));
  if (temp_window)
       temp_window->submapped_order = 0;

  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));

#endif

	/* Request UnmapSubwindows is opcode 11 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* UnmapSubwindows */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
ConfigureWindow(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     int sibling_index = 12, mask = IShort(&buf[8]);

     if (mask & CWSibling) {
	  mask &= (CWSibling-1);
	  while (mask) {
	       if (mask & 1)
		    sibling_index += 4;
	       mask >>= 1;
	  }

	  ISetLong(&buf[sibling_index], MapWindowID(ILong(&buf[sibling_index]), Request));
     }

     ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
  
#endif

	/* Request ConfigureWindow is opcode 12 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ConfigureWindow */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(3 + n), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
	PrintField(buf, 8, 2, CONFIGURE_BITMASK, "value-mask");
	PrintValues(&buf[8], 2, CONFIGURE_BITMASK, &buf[12], "value-list");
}

Global void
CirculateWindow(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  WindowPtr temp_window;
  
  temp_window = FindWindowFromCurrentClient(ILong(&buf[4]));
  if (temp_window)
       if (IByte(&buf[1]))
	    temp_window->Lower_Highest++;

       else
	    temp_window->Raise_Lowest++;

  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

	/* Request CirculateWindow is opcode 13 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CirculateWindow */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, CIRMODE, "direction");
	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
GetGeometry(buf)
		unsigned char *buf;
{
#ifdef XMOVE
    ISetLong(&buf[4], MapDrawable(ILong(&buf[4]), Request));
#endif

	/* Request GetGeometry is opcode 14 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetGeometry */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, DRAWABLE, "drawable");
}

Global void
GetGeometryReply(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Reply));
#endif

	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetGeometry */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, CARD8, "depth");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 4, WINDOW, "root");
	PrintField(buf, 12, 2, INT16, "x");
	PrintField(buf, 14, 2, INT16, "y");
	PrintField(buf, 16, 2, CARD16, "width");
	PrintField(buf, 18, 2, CARD16, "height");
	PrintField(buf, 20, 2, CARD16, "border-width");
}

Global void
QueryTree(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

	/* Request QueryTree is opcode 15 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* QueryTree */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
QueryTreeReply(buf)
		unsigned char *buf;
{
     int n = IShort(&buf[16]);
     int count = n;
	
#ifdef XMOVE
     ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Reply));
     if (ILong(&buf[12]))
	  ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Reply));
     /* Now remap n children windows */
     
     while (count--)
	  ISetLong(&buf[32+(4*count)], MapWindowID(ILong(&buf[32+(4*count)]), Reply));
#endif

     PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* QueryTree */ ;
     if (CurrentVerbose < 2)
	  return;
     printfield(buf, 2, 2, CARD16, "sequence number");
     printfield(buf, 4, 4, DVALUE4(n), "reply length");
     PrintField(buf, 8, 4, WINDOW, "root");
     PrintField(buf, 12, 4, WINDOW, "parent");
     printfield(buf, 16, 2, DVALUE2(n), "number of children");
     PrintList(&buf[32], (long)n, WINDOW, "children");
}

Global void
InternAtom(buf)
		unsigned char *buf;
{
     short n = IShort(&buf[4]);
     
#ifdef XMOVE
     char *name = malloc(n+1);

     (void)strncpy(name, (char *)&buf[8], n);
     name[n] = '\0';

     if (client->misc)
	  free(client->misc);

     client->misc = name;
#endif

	/* Request InternAtom is opcode 16 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* InternAtom */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, BOOL, "only-if-exists");
	printfield(buf, 2, 2, DVALUE2(2 + (n + p) / 4), "request length");
	printfield(buf, 4, 2, DVALUE2(n), "length of name");
	PrintString8(&buf[8], n, "name");
}

Global void
InternAtomReply(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     Atom atom = ILong(&buf[8]);
     AtomPtr temp_atom;
     char *name = client->misc;

     if (atom) {
	  temp_atom = CreateNewAtom(atom, name, Reply);
	  if (temp_atom)
	       ISetLong(&buf[8], temp_atom->client_atom);
     }

     free(name);
     client->misc = NULL;
#endif

	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* InternAtom */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 4, ATOM, "atom");
}

Global void
GetAtomName(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  ISetLong(&buf[4], MapAtom(ILong(&buf[4]), Request));
#endif
  
	/* Request GetAtomName is opcode 17 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetAtomName */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, ATOM, "atom");
}

Global void
GetAtomNameReply(buf)
		unsigned char *buf;
{
	short   n;
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetAtomName */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4((n + p) / 4), "reply length");
	printfield(buf, 8, 2, DVALUE2(n), "length of name");
	n = IShort(&buf[8]);
	PrintString8(&buf[32], n, "name");
}


Global void
ChangeProperty(buf)
		unsigned char *buf;
{
    long numBytes = IByte(&buf[16]) / 8 * ILong(&buf[20]); /* unitsize*len */
    int curbuf = 24;

#ifdef XMOVE
    Atom client_atom = ILong(&buf[8]);
    Atom new_atom    = client_atom;
    Atom client_type = ILong(&buf[12]);
    Atom new_type    = client_type;
    Window client_window = ILong(&buf[4]);
    Window new_window    = client_window;
    AtomPtr atom;
    void (*map_func)() = NULL;

    ISetLong(&buf[4],  MapWindowID(client_window, Request));
    ISetLong(&buf[8],  MapAtom(client_atom, Request));
    ISetLong(&buf[12], MapAtom(client_type, Request));
    
    atom = FindAtom(client_atom);
    if (atom->transient_map_func) {
	 map_func = atom->transient_map_func->MapProperty;
	 atom->transient_map_func = NULL;
    } else if (atom->property_map_func)
	 map_func = atom->property_map_func->MapProperty;
    else {
	 atom = FindAtom(client_type);
	 if (atom->type_map_func)
	      map_func = atom->type_map_func->MapProperty;
    }
    
    if (map_func) {
	 (map_func)(MapChangeProp, &new_atom, &new_type, NULL,
		    atom, False, &new_window, buf+24, numBytes);

	 if (new_window != client_window)
	      ISetLong(&buf[4], new_window);

	 if (new_atom != client_atom)
	      ISetLong(&buf[8], new_atom);

	 if (new_type != client_type)
	      ISetLong(&buf[12], new_type);

	  if (ignore_bytes) {	/* set by mapping function, it used sync_server() */
	       SendBuffer(server->alt_fd, &buf[0], 24+ROUNDUP4(numBytes));
	       ++server->alt_seqno;
	  }
    }
    
#endif

    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ChangeProperty */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    PrintField(buf, 1, 1, CHANGEMODE, "mode");
    printfield(buf, 2, 2, DVALUE2(6 + (n + p) / 4), "request length");
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 4, ATOM, "property");
    PrintField(buf, 12, 4, ATOM, "type");
    PrintField(buf, 16, 1, CARD8, "format");
    printfield(buf, 20, 4, CARD32, "length of data");

    if (ILong(&buf[12]) == 31 /* string */)
	PrintString8(&buf[24], (short)numBytes, "data");
    else
	PrintBytes(&buf[24], numBytes, "data");
}

Global void
DeleteProperty(buf)
unsigned char *buf;
{
#ifdef XMOVE
     Atom client_atom = ILong(&buf[8]);
     Atom new_atom    = client_atom;
     Window client_window = ILong(&buf[4]);
     Window new_window    = client_window;
     AtomPtr atom;

     ISetLong(&buf[4], MapWindowID(client_window, Request));
     ISetLong(&buf[8], MapAtom(client_atom, Request));
     
     atom = FindAtom(client_atom);
     if (atom->transient_map_func)
	  atom->transient_map_func = NULL;
     
     if (atom && atom->property_map_func) {
	  (atom->property_map_func->MapProperty)(MapDeleteProp, &new_atom,
	    NULL, NULL, atom, True, &new_window, NULL, 0);
	  
	  if (new_window != client_window)
	       ISetLong(&buf[4], new_window);

	  if (new_atom != client_atom)
	       ISetLong(&buf[8], new_atom);

	  if (ignore_bytes) {	/* set by mapping function, it used sync_server() */
	       SendBuffer(server->alt_fd, &buf[0], 12);
	       ++server->alt_seqno;
	  }
     }
#endif

	/* Request DeleteProperty is opcode 19 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* DeleteProperty */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(3), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
	PrintField(buf, 8, 4, ATOM, "property");
}

Global void
GetProperty(buf)
unsigned char *buf;
{
#ifdef XMOVE
     Atom client_atom = ILong(&buf[8]);
     Atom new_atom    = client_atom;
     Atom client_type = ILong(&buf[12]);
     Atom new_type    = client_type;
     AtomPtr atom;
     
     Window client_window = ILong(&buf[4]);
     Window new_window = client_window;
     
     Bool delete = IByte(&buf[1]);
     GetPropertyTemps *temps = malloc(sizeof(GetPropertyTemps));
     void (*map_func)() = NULL;
     
     Dprintf(("XMOVE GetProperty on window 0x%x\n",ILong(&buf[4])));
     
     ISetLong(&buf[4], MapWindowID(client_window, Request));
     ISetLong(&buf[8], MapAtom(client_atom, Request));
     if (ILong(&buf[12]))
	  ISetLong(&buf[12], MapAtom(client_type, Request));
     
     atom = FindAtom(client_atom);
     if (atom->transient_map_func)
	  map_func = atom->transient_map_func->MapProperty;
     else if (atom->property_map_func)
	  map_func = atom->property_map_func->MapProperty;
     else if (client_type != 0) {
	  atom = FindAtom(client_type);
	  if (atom->type_map_func)
	       map_func = atom->type_map_func->MapProperty;
     }
     
     if (map_func) {
	  (map_func)(MapGetProp, &new_atom, &new_type, NULL,
		     atom, delete, &new_window, NULL, 0);
	  
	  if (new_window != client_window)
	       ISetLong(&buf[4], new_window);
	  
	  if (new_atom != client_atom)
	       ISetLong(&buf[8], new_atom);
	  
	  if (new_type != client_type)
	       ISetLong(&buf[12], new_type);

	  if (ignore_bytes) {	/* set by mapping function, it used sync_server() */
	       SendBuffer(server->alt_fd, &buf[0], 24);
	       ++server->alt_seqno;
	  }
     }
     
     temps->server_atom = ILong(&buf[8]);
     temps->delete = delete;
     temps->window = ILong(&buf[4]);
     
     if (client->misc)
	  free(client->misc);
     
     client->misc = temps;
#endif

	/* Request GetProperty is opcode 20 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetProperty */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, BOOL, "delete");
	printfield(buf, 2, 2, CONST2(6), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
	PrintField(buf, 8, 4, ATOM, "property");
	PrintField(buf, 12, 4, ATOM, "type");
	PrintField(buf, 16, 4, CARD32, "long-offset");
	printfield(buf, 20, 4, CARD32, "long-length");
}

Global void
GetPropertyReply(buf)
unsigned char *buf;
{
     int   length = IByte(&buf[1]) / 8 * ILong(&buf[16]);
     int   curbuf = 32;
     Atom server_type = ILong(&buf[8]);
     Atom new_type    = server_type;
     AtomPtr atom;
     GetPropertyTemps *temps = client->misc;
     void (*map_func)() = NULL;

#ifdef XMOVE
     ISetLong(&buf[8], MapAtom(server_type, Reply));
     
     atom = FindServerAtom(temps->server_atom);
     if (atom->transient_map_func)
	  map_func = atom->transient_map_func->MapProperty;
     else if (atom->property_map_func)
	  map_func = atom->property_map_func->MapProperty;
     else if (server_type) {
	  atom = FindServerAtom(server_type);
	  if (atom->type_map_func)
	       map_func = atom->type_map_func->MapProperty;
     }
    
     if (map_func) {
	  (map_func)(MapGetPropReply, &temps->server_atom, &new_type,
		     NULL, atom, temps->delete, &temps->window, buf+32, length);

	  if (new_type != server_type)
	       ISetLong(&buf[8], new_type);
     }

     if (temps->delete && atom->transient_map_func)
	  atom->transient_map_func = NULL;
     
     free(client->misc);
     client->misc = NULL;
#endif	 

     PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetProperty */ ;
     if (CurrentVerbose < 2)
	  return;
     PrintField(buf, 1, 1, CARD8, "format");
     printfield(buf, 2, 2, CARD16, "sequence number");
     printfield(buf, 4, 4, DVALUE4((n + p) / 4), "reply length");
     PrintField(buf, 8, 4, ATOM, "type");
     PrintField(buf, 12, 4, CARD32, "bytes-after");
     printfield(buf, 16, 4, CARD32, "length of value");
     
     if (server_type == 31 /* string */)
	  PrintString8(&buf[32], (short)(length), "value");
     else
	  PrintBytes(&buf[32], length, "value");
}

Global void
ListProperties(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

	/* Request ListProperties is opcode 21 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ListProperties */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
ListPropertiesReply(buf)
		unsigned char *buf;
{
	short n = IShort(&buf[8]);
#ifdef XMOVE
	while (n--)
	  ISetLong(&buf[32 + n*4],
		   MapAtom(ILong(&buf[32 + n*4]), Reply));
#endif
	n = IShort(&buf[8]);
	
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* ListProperties */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4(n), "reply length");
	printfield(buf, 8, 2, DVALUE2(n), "number of atoms");
	PrintList(&buf[32], (long)n, ATOM, "atoms");
}

Global void
SetSelectionOwner(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  SelectionPtr temp_selection = (SelectionPtr)1; /* non-Null value */

  ForAllInList(&client->selection_list) {
       temp_selection = (SelectionPtr)
	    CurrentContentsOfList(&client->selection_list);

       if (temp_selection->selection == ILong(&buf[8])) {
	    temp_selection = NULL;
	    break;
       }
  }
  
  if (temp_selection) {      /* ie. selection not found in client list */
       Dprintf(("Adding new selection %d\n",ILong(&buf[8])))
       temp_selection   = (SelectionPtr)Tcalloc(1, SelectionRec);
       temp_selection->selection = ILong(&buf[8]);
       
       AddSelectionToCurrentClient(temp_selection);
  }
  
  if (ILong(&buf[4]))
       ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
  ISetLong(&buf[8], MapAtom(ILong(&buf[8]), Request));
#endif

	/* Request SetSelectionOwner is opcode 22 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* SetSelectionOwner */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(4), "request length");
	PrintField(buf, 4, 4, WINDOW, "owner");
	PrintField(buf, 8, 4, ATOM, "selection");
	PrintField(buf, 12, 4, TIMESTAMP, "time");
}

Global void
GetSelectionOwner(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  ISetLong(&buf[4], MapAtom(ILong(&buf[4]), Request));
#endif
	/* Request GetSelectionOwner is opcode 23 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetSelectionOwner */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, ATOM, "selection");
}

Global void
GetSelectionOwnerReply(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     if (ILong(&buf[8]))
	  ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Reply));
#endif

	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetSelectionOwner */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 4, WINDOW, "owner");
}

Global void
ConvertSelection(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     Window requestor = ILong(&buf[4]);
     Atom   selection = ILong(&buf[8]);
     Atom   target    = ILong(&buf[12]);
     Atom   property  = ILong(&buf[16]);
     
     Window new_requestor = requestor;
     Atom   new_selection = selection;
     Atom   new_target    = target;
     Atom   new_property  = property;
     
     AtomPtr atom;
     
     ISetLong(&buf[4], MapWindowID(requestor, Request));
     ISetLong(&buf[8], MapAtom(selection, Request));
     ISetLong(&buf[12], MapAtom(target, Request));
     if (property)
	  ISetLong(&buf[16], MapAtom(property, Request));

     atom = FindAtom(target);
     if (atom && atom->type_map_func) {
	  (atom->type_map_func->MapProperty)(MapConvertSelection,
	    &new_property, &new_target, &new_selection, atom,
	    0, &requestor, NULL, 0);

	  if (new_requestor != requestor)
	       ISetLong(&buf[4], new_requestor);

	  if (new_selection != selection)
	       ISetLong(&buf[8], new_selection);

	  if (new_target != target)
	       ISetLong(&buf[12], new_target);

	  if (new_property != property)
	       ISetLong(&buf[16], new_property);

	  if (ignore_bytes) {	/* set by mapping function, it used sync_server() */
	       SendBuffer(server->alt_fd, &buf[0], 24);
	       ++server->alt_seqno;
	  }
     }
#endif

	/* Request ConvertSelection is opcode 24 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ConvertSelection */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(6), "request length");
	PrintField(buf, 4, 4, WINDOW, "requestor");
	PrintField(buf, 8, 4, ATOM, "selection");
	PrintField(buf, 12, 4, ATOM, "target");
	PrintField(buf, 16, 4, ATOM, "property");
	PrintField(buf, 20, 4, TIMESTAMP, "time");
}

Global void
SendEvent(buf)
unsigned char *buf;
{
#ifdef XMOVE
     Window base_window = ILong(&buf[4]);
     Window window, new_window;
	  
     if (base_window > 1)
	  ISetLong(&buf[4], MapWindowID(base_window, Request));

     buf += 12;			/* skip to beginning of event data */

     switch (IByte(&buf[0]))
     {
     case 2:
	  SendKeyPressEvent(buf);
	  break;
     case 3:
	  SendKeyReleaseEvent(buf);
	  break;
     case 4:
	  SendButtonPressEvent(buf);
	  break;
     case 5:
	  SendButtonReleaseEvent(buf);
	  break;
     case 6:
	  SendMotionNotifyEvent(buf);
	  break;
     case 7:
	  SendEnterNotifyEvent(buf);
	  break;
     case 8:
	  SendLeaveNotifyEvent(buf);
	  break;
     case 9:
	  SendFocusInEvent(buf);
	  break;
     case 10:
	  SendFocusOutEvent(buf);
	  break;
     case 11:
	  SendKeymapNotifyEvent(buf);
	  break;
     case 12:
	  SendExposeEvent(buf);
	  break;
     case 13:
	  SendGraphicsExposureEvent(buf);
	  break;
     case 14:
	  SendNoExposureEvent(buf);
	  break;
     case 15:
	  SendVisibilityNotifyEvent(buf);
	  break;
     case 16:
	  SendCreateNotifyEvent(buf);
	  break;
     case 17:
	  SendDestroyNotifyEvent(buf);
	  break;
     case 18:
	  SendUnmapNotifyEvent(buf);
	  break;
     case 19:
	  SendMapNotifyEvent(buf);
	  break;
     case 20:
	  SendMapRequestEvent(buf);
	  break;
     case 21:
	  SendReparentNotifyEvent(buf);
	  break;
     case 22:
	  SendConfigureNotifyEvent(buf);
	  break;
     case 23:
	  SendConfigureRequestEvent(buf);
	  break;
     case 24:
	  SendGravityNotifyEvent(buf);
	  break;
     case 25:
	  SendResizeRequestEvent(buf);
	  break;
     case 26:
	  SendCirculateNotifyEvent(buf);
	  break;
     case 27:
	  SendCirculateRequestEvent(buf);
	  break;
     case 28:
	  SendPropertyNotifyEvent(buf);
	  break;
     case 29:
	  SendSelectionClearEvent(buf);
	  break;
     case 30:
	  SendSelectionRequestEvent(buf);
	  break;
     case 31:
	  SendSelectionNotifyEvent(buf);
	  break;
     case 32:
	  SendColormapNotifyEvent(buf);
	  break;
     case 33:
	  /* when SendEventing a ClientMessage there are two
	     windows specified: the recipient for the event and
	     the window specified in the event. If they are the
	     same (which usually they will be) and if a map_func
	     changes the window specified in the event, change
	     it in the recipient window as well. */
	     
	  window = ILong(&buf[4]);
	  SendClientMessageEvent(buf);
	  new_window = ILong(&buf[4]);
	  if (window == base_window && window != new_window)
	       ISetLong(&buf[-8], new_window);
	  break;
     case 34:
	  SendMappingNotifyEvent(buf);
	  break;
     default:
	  fprintf(stdout, "####### Unimplemented Event opcode %d\n", Event);
	  break;
     }

     buf -= 12;
#endif
     
     /* Request SendEvent is opcode 25 */
     PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* SendEvent */ ;
     if (CurrentVerbose < 2)
	  return;
     if (CurrentVerbose > 2)
	  PrintField(SBf, 0, 4, INT32, "sequence number");
     
     PrintField(buf, 1, 1, BOOL, "propagate");
     printfield(buf, 2, 2, CONST2(11), "request length");
     PrintField(buf, 4, 4, WINDOWD, "destination");
     PrintField(buf, 8, 4, SETofEVENT, "event-mask");

     /* this code musn't be un-commented until we have a way of printing
	the event that doesn't call DecodeEvent */
     
/*	PrintField(buf, 12, 32, EVENTFORM, "event");*/
}

Global void
GrabPointer(buf)
		unsigned char *buf;
{
#ifdef XMOVE
/*  GrabPtr temp_grab;

  temp_grab = Tcalloc(1, GrabRec);
  temp_grab->type = 26;
  temp_grab->owner_events = IByte(&buf[1]);
  temp_grab->grab_window = ILong(&buf[4]);
  temp_grab->event_mask = IShort(&buf[8]);
  temp_grab->pointer_mode = IByte(&buf[10]);
  temp_grab->keyboard_mode = IByte(&buf[11]);
  temp_grab->confine_to = ILong(&buf[12]);
  temp_grab->cursor = ILong(&buf[16]);
  temp_grab->time = ILong(&buf[20]);
  GlobGrab = temp_grab;
*/

  Dprintf(("XMOVE Pointer is being grabbed\n"))

  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
  if (ILong(&buf[12]))
       ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));
  if (ILong(&buf[16]))
       ISetLong(&buf[16], MapCursorID(ILong(&buf[16]), Request));
#endif

	/* Request GrabPointer is opcode 26 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GrabPointer */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, BOOL, "owner-events");
	printfield(buf, 2, 2, CONST2(6), "request length");
	PrintField(buf, 4, 4, WINDOW, "grab-window");
	PrintField(buf, 8, 2, SETofPOINTEREVENT, "event-mask");
	PrintField(buf, 10, 1, PK_MODE, "pointer-mode");
	PrintField(buf, 11, 1, PK_MODE, "keyboard-mode");
	PrintField(buf, 12, 4, WINDOW, "confine-to");
	PrintField(buf, 16, 4, CURSOR, "cursor");
	PrintField(buf, 20, 4, TIMESTAMP, "time");
}

Global void
GrabPointerReply(buf)
		unsigned char *buf;
{
/*#ifdef XMOVE
  if (IShort(&buf[1]) == 0) {
    RemoveGrabPointerFromCurrentClient(); 
    AddGrabToCurrentClient(GlobGrab);
    Dprintf(("adding grab (pointer) to client with grab_window %x\n",GlobGrab->grab_window))
  } else {
       printf(("grab pointer failed: status == %d\n",IByte(&buf[1])));
       free((char *)GlobGrab);
  }
#endif
*/
        PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GrabPointer */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, GRABSTAT, "status");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
}

Global void
UngrabPointer(buf)
		unsigned char *buf;
{
/*#ifdef XMOVE
     RemoveGrabPointerFromCurrentClient();
     Dprintf(("Removing grab (pointer)\n"))
#endif*/
	/* Request UngrabPointer is opcode 27 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* UngrabPointer */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, TIMESTAMP, "time");
}

Global void
GrabButton(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  GrabPtr temp_grab;

  temp_grab = Tcalloc(1, GrabRec);
  temp_grab->type = 28;
  temp_grab->owner_events = IByte(&buf[1]);
  temp_grab->grab_window = ILong(&buf[4]);
  temp_grab->event_mask = IShort(&buf[8]);
  temp_grab->pointer_mode = IByte(&buf[10]);
  temp_grab->keyboard_mode = IByte(&buf[11]);
  temp_grab->confine_to = ILong(&buf[12]);
  temp_grab->cursor = ILong(&buf[16]);
  temp_grab->button = IByte(&buf[20]);
  temp_grab->modifiers = IShort(&buf[22]);
  Dprintf(("XMOVE GrabButton\n"))
  AddGrabToCurrentClient(temp_grab);
  Dprintf(("adding grab (button) to client with grab_window %x\n",temp_grab->grab_window))
  
  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
  if (ILong(&buf[12]))
       ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Request));
  if (ILong(&buf[16]))
       ISetLong(&buf[16], MapCursorID(ILong(&buf[16]), Request));
#endif 

	/* Request GrabButton is opcode 28 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GrabButton */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, BOOL, "owner-events");
	printfield(buf, 2, 2, CONST2(6), "request length");
	PrintField(buf, 4, 4, WINDOW, "grab-window");
	PrintField(buf, 8, 2, SETofPOINTEREVENT, "event-mask");
	PrintField(buf, 10, 1, PK_MODE, "pointer-mode");
	PrintField(buf, 11, 1, PK_MODE, "keyboard-mode");
	PrintField(buf, 12, 4, WINDOW, "confine-to");
	PrintField(buf, 16, 4, CURSOR, "cursor");
	PrintField(buf, 20, 1, BUTTONA, "button");
	PrintField(buf, 22, 2, SETofKEYMASK, "modifiers");
}

Global void
UngrabButton(buf)
		unsigned char *buf;
{
#ifdef XMOVE

  RemoveGrabButtonFromCurrentClient(IByte(&buf[1]), ILong(&buf[4]),
				    IShort(&buf[8]));
  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif
	/* Request UngrabButton is opcode 29 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* UngrabButton */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, BUTTONA, "button");
	printfield(buf, 2, 2, CONST2(3), "request length");
	PrintField(buf, 4, 4, WINDOW, "grab-window");
	PrintField(buf, 8, 2, SETofKEYMASK, "modifiers");
}

Global void
ChangeActivePointerGrab(buf)
		unsigned char *buf;
{
#ifdef XMOVE
/*
  ChangeGrabPointerFromCurrentClient(ILong(&buf[4]), ILong(&buf[8]),
				     IShort(&buf[10]));
*/
  Dprintf(("XMOVE ChangeActivePointerGrab\n"))

  if (ILong(&buf[4]))
    ISetLong(&buf[4], MapCursorID(ILong(&buf[4]), Request));
#endif

	/* Request ChangeActivePointerGrab is opcode 30 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ChangeActivePointerGrab */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");
	printfield(buf, 2, 2, CONST2(4), "request length");
	PrintField(buf, 4, 4, CURSOR, "cursor");
	PrintField(buf, 8, 4, TIMESTAMP, "time");
	PrintField(buf, 12, 2, SETofPOINTEREVENT, "event-mask");
}

Global void
GrabKeyboard(buf)
		unsigned char *buf;
{
#ifdef XMOVE
/*  GrabPtr temp_grab;

  temp_grab = Tcalloc(1, GrabRec);
  temp_grab->type = 31;
  temp_grab->owner_events = IByte(&buf[1]);
  temp_grab->grab_window = ILong(&buf[4]);
  temp_grab->time = ILong(&buf[8]);
  temp_grab->pointer_mode = IByte(&buf[12]);
  temp_grab->keyboard_mode = IByte(&buf[13]);
  GlobGrab = temp_grab;
  
  Dprintf(("XMOVE Keyboard is being grabbed\n"))

*/
  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

	/* Request GrabKeyboard is opcode 31 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GrabKeyboard */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, BOOL, "owner-events");
	printfield(buf, 2, 2, CONST2(4), "request length");
	PrintField(buf, 4, 4, WINDOW, "grab-window");
	PrintField(buf, 8, 4, TIMESTAMP, "time");
	PrintField(buf, 12, 1, PK_MODE, "pointer-mode");
	PrintField(buf, 13, 1, PK_MODE, "keyboard-mode");
}

Global void
GrabKeyboardReply(buf)
		unsigned char *buf;
{
#ifdef XMOVE
/*  if (IShort(&buf[1]) == 0) {
       RemoveGrabKeyboardFromCurrentClient();
       AddGrabToCurrentClient(GlobGrab);
       Dprintf(("adding grab (keyboard) to client with grab_window %x\n",GlobGrab->grab_window))
  } else {
       Dprintf(("grab keyboard failed: status == %d\n",IByte(&buf[1])))
       free((char *)GlobGrab);
  }
*/
#endif
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GrabKeyboard */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, GRABSTAT, "status");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
}

Global void
UngrabKeyboard(buf)
		unsigned char *buf;
{
#ifdef XMOVE
/*     RemoveGrabKeyboardFromCurrentClient();
     Dprintf(("Removing grab (keyboard)\n"))*/
#endif
	/* Request UngrabKeyboard is opcode 32 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* UngrabKeyboard */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, TIMESTAMP, "time");
}

Global void
GrabKey(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  GrabPtr temp_grab;

  temp_grab = Tcalloc(1, GrabRec);
  temp_grab->type = 33;
  temp_grab->owner_events = IByte(&buf[1]);
  temp_grab->grab_window = ILong(&buf[4]);
  temp_grab->modifiers = IShort(&buf[8]);
  temp_grab->key = IByte(&buf[10]);
  temp_grab->pointer_mode = IByte(&buf[11]);
  temp_grab->keyboard_mode = IByte(&buf[12]);
  Dprintf(("XMOVE GrabKey\n"))
  AddGrabToCurrentClient(temp_grab);
  Dprintf(("adding grab (key) to client with grab_window %x\n",temp_grab->grab_window))

  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

	/* Request GrabKey is opcode 33 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GrabKey */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, BOOL, "owner-events");
	printfield(buf, 2, 2, CONST2(4), "request length");
	PrintField(buf, 4, 4, WINDOW, "grab-window");
	PrintField(buf, 8, 2, SETofKEYMASK, "modifiers");
	PrintField(buf, 10, 1, KEYCODEA, "key");
	PrintField(buf, 11, 1, PK_MODE, "pointer-mode");
	PrintField(buf, 12, 1, PK_MODE, "keyboard-mode");
}

Global void
UngrabKey(buf)
		unsigned char *buf;
{
#ifdef XMOVE

  /* check this.  must keep track of key grabs */

  RemoveGrabKeyFromCurrentClient(IByte(&buf[1]), ILong(&buf[4]),
				 IShort(&buf[2]));
  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif


	/* Request UngrabKey is opcode 34 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* UngrabKey */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, KEYCODEA, "key");
	printfield(buf, 2, 2, CONST2(3), "request length");
	PrintField(buf, 4, 4, WINDOW, "grab-window");
	PrintField(buf, 8, 2, SETofKEYMASK, "modifiers");
}

Global void
AllowEvents(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     /* NOTE: AllowEvents is ignored if time is greater than the server's
        internal time, but regardless not reply is given. Hence this command
	may have failed and we will never know. Nothing, I believe, can be
	done */

/*   GrabPtr temp_grab;
   card8 mode = IByte(&buf[1]);
   Time time = ILong(&buf[4]);

   ForAllInList(&client->grab_list)
   {
	temp_grab = (GrabPtr)CurrentContentsOfList(&client->grab_list);
	if (temp_grab->type == 26 || temp_grab->type == 31) {
	     if (time == 0 || time > temp_grab->time) {
		  if (mode == 0 || mode == 6)
		       temp_grab->pointer_mode = 1;
		  else if (mode == 1  
	
		  */
  Dprintf(("XMOVE AllowEvents\n"))
#endif
	/* Request AllowEvents is opcode 35 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* AllowEvents */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, EVENTMODE, "mode");
	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, TIMESTAMP, "time");
}

Global void
GrabServer(buf)
		unsigned char *buf;
{
     client->grabbed_server = True;
     Dprintf(("XMOVE GrabServer\n"))

	/* Request GrabServer is opcode 36 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GrabServer */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
UngrabServer(buf)
		unsigned char *buf;
{
     client->grabbed_server = False;
     Dprintf(("XMOVE UngrabServer\n"))

	/* Request UngrabServer is opcode 37 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* UngrabServer */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
QueryPointer(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE QueryPointer\n"))
  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

	/* Request QueryPointer is opcode 38 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* QueryPointer */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
QueryPointerReply(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Reply));
  if (ILong(&buf[12]))
       ISetLong(&buf[12], MapWindowID(ILong(&buf[12]), Reply));
#endif
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* QueryPointer */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, BOOL, "same-screen");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 4, WINDOW, "root");
	PrintField(buf, 12, 4, WINDOW, "child");
	PrintField(buf, 16, 2, INT16, "root-x");
	PrintField(buf, 18, 2, INT16, "root-y");
	PrintField(buf, 20, 2, INT16, "win-x");
	PrintField(buf, 22, 2, INT16, "win-y");
	PrintField(buf, 24, 2, SETofKEYBUTMASK, "mask");
}

Global void
GetMotionEvents(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE GetMotionEvents\n"))
  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif
	/* Request GetMotionEvents is opcode 39 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetMotionEvents */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(4), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
	PrintField(buf, 8, 4, TIMESTAMP, "start");
	PrintField(buf, 12, 4, TIMESTAMP, "stop");
}

Global void
GetMotionEventsReply(buf)
		unsigned char *buf;
{
	long   n;
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetMotionEvents */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4(2*n), "reply length");
	printfield(buf, 8, 4, DVALUE4(n), "number of events");
	n = ILong(&buf[8]);
	PrintList(&buf[32], n, TIMECOORD, "events");
}

Global void
TranslateCoordinates(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
  ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
#endif

  /* Request TranslateCoordinates is opcode 40 */
  PrintField(buf, 0, 1, REQUEST, REQUESTHEADER);
  /* TranslateCoordinates */ 
  if (CurrentVerbose < 2)
    return;
  if (CurrentVerbose > 2)
    PrintField(SBf, 0, 4, INT32, "sequence number");
  
  printfield(buf, 2, 2, CONST2(4), "request length");
  PrintField(buf, 4, 4, WINDOW, "src-window");
  PrintField(buf, 8, 4, WINDOW, "dst-window");
  PrintField(buf, 12, 2, INT16, "src-x");
  PrintField(buf, 14, 2, INT16, "src-y");
}

Global void
TranslateCoordinatesReply(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     if (ILong(&buf[8]))
	  ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Reply));
#endif

	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* TranslateCoordinates */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, BOOL, "same-screen");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 4, WINDOW, "child");
	PrintField(buf, 12, 2, INT16, "dst-x");
	PrintField(buf, 14, 2, INT16, "dst-y");
}

Global void
WarpPointer(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  Dprintf(("XMOVE WarpPointer\n"))
  if (ILong(&buf[4]))
       ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
  if (ILong(&buf[8]))
       ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
#endif

	/* Request WarpPointer is opcode 41 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* WarpPointer */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(6), "request length");
	PrintField(buf, 4, 4, WINDOW, "src-window");
	PrintField(buf, 8, 4, WINDOW, "dst-window");
	PrintField(buf, 12, 2, INT16, "src-x");
	PrintField(buf, 14, 2, INT16, "src-y");
	PrintField(buf, 16, 2, CARD16, "src-width");
	PrintField(buf, 18, 2, CARD16, "src-height");
	PrintField(buf, 20, 2, INT16, "dst-x");
	PrintField(buf, 22, 2, INT16, "dst-y");
}

Global void
SetInputFocus(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  /* check this.  Keep track of input focus */
  Dprintf(("XMOVE SetInputFocus\n"))
  if (ILong(&buf[4]) > 1)
       ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

	/* Request SetInputFocus is opcode 42 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* SetInputFocus */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, FOCUSAGENT, "revert-to");
	printfield(buf, 2, 2, CONST2(3), "request length");
	PrintField(buf, 4, 4, WINDOWNR, "focus");
	PrintField(buf, 8, 4, TIMESTAMP, "time");
}

Global void
GetInputFocus(buf)
		unsigned char *buf;
{


	/* Request GetInputFocus is opcode 43 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetInputFocus */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
GetInputFocusReply(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     if (ILong(&buf[8]) > 1)
	  ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Reply));
#endif

	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetInputFocus */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, FOCUSAGENT, "revert-to");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 4, WINDOWNR, "focus");
}

Global void
QueryKeymap(buf)
		unsigned char *buf;
{
	/* Request QueryKeymap is opcode 44 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* QueryKeymap */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
QueryKeymapReply(buf)
		unsigned char *buf;
{
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* QueryKeymap */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(2), "reply length");
	PrintBytes(&buf[8], 32L, "keys");
}

Global void
OpenFont(unsigned char *buf)
{
    short n = IShort(&buf[8]);	/* length of the font name */

#ifdef XMOVE
    FontPtr temp_font;

    /* create a new font structure, add it to the current client, and */
    /* fill in the appropriate fields */
    
    temp_font = Tmalloc(FontRec);
    temp_font->font_id = ILong(&buf[4]);
    temp_font->name = Tcalloc(n+1, char);
    strncpy(temp_font->name, (char *)&buf[12], n);

    AddFontToCurrentClient(temp_font);

    ISetLong(&buf[4], MapFontID(ILong(&buf[4]), Request));
    
#endif

    /* Request OpenFont is opcode 45 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* OpenFont */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 2, 2, DVALUE2(3 + (n + p) / 4), "request length");
    PrintField(buf, 4, 4, FONT, "font-id");
    printfield(buf, 8, 2, DVALUE2(n), "length of name");
    PrintString8(&buf[12], n, "name");
}

Global void
CloseFont(unsigned char *buf)

{
#ifdef XMOVE
    RemoveFontFromCurrentClient(ILong(&buf[4]));
    ISetLong(&buf[4], MapFontID(ILong(&buf[4]), Request));
#endif

	/* Request CloseFont is opcode 46 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CloseFont */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, FONT, "font");
}

Global void
QueryFont(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  ISetLong(&buf[4], MapFontID(ILong(&buf[4]), Request));
#endif
  
	/* Request QueryFont is opcode 47 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* QueryFont */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, FONTABLE, "font");
}

Global void
QueryFontReply(buf)
		unsigned char *buf;
{
	short   n;
	long    m;
	long   k;

	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* QueryFont */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4(7 + 2*n + 3*m), "reply length");
	PrintField(buf, 8, 12, CHARINFO, "min-bounds");
	PrintField(buf, 24, 12, CHARINFO, "max-bounds");
	PrintField(buf, 40, 2, CARD16, "min-char-or-byte2");
	PrintField(buf, 42, 2, CARD16, "max-char-or-byte2");
	PrintField(buf, 44, 2, CARD16, "default-char");
	printfield(buf, 46, 2, DVALUE2(n), "number of FONTPROPs");
	n = IShort(&buf[46]);
	PrintField(buf, 48, 1, DIRECT, "draw-direction");
	PrintField(buf, 49, 1, CARD8, "min-byte1");
	PrintField(buf, 50, 1, CARD8, "max-byte1");
	PrintField(buf, 51, 1, BOOL, "all-chars-exist");
	PrintField(buf, 52, 2, INT16, "font-ascent");
	PrintField(buf, 54, 2, INT16, "font-descent");
	printfield(buf, 56, 4, DVALUE4(m), "number of CHARINFOs");
	if (CurrentVerbose < 4)
	     return;
	m = ILong(&buf[56]);
	k = PrintList(&buf[60], (long)n, FONTPROP, "properties");
	PrintList(&buf[(int)(60 + k)], (long)m, CHARINFO, "char-infos");
}

Global void
QueryTextExtents(buf)
		unsigned char *buf;
{
        unsigned int n;

#ifdef XMOVE
  ISetLong(&buf[4], MapFontID(ILong(&buf[4]), Request));
#endif

	/* Request QueryTextExtents is opcode 48 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* QueryTextExtents */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 1, 1, BOOL, "odd length?");
	printfield(buf, 2, 2, DVALUE2(2 + (2*n + p) / 4), "request length");
	n = (IShort(&buf[2]) - 2) * 2;
	if (IBool(&buf[1]))
		n -= 1;
	PrintField(buf, 4, 4, FONTABLE, "font");
	PrintString16(&buf[8], n, "string");
}

Global void
QueryTextExtentsReply(buf)
		unsigned char *buf;
{
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* QueryTextExtents */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, DIRECT, "draw-direction");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 2, INT16, "font-ascent");
	PrintField(buf, 10, 2, INT16, "font-descent");
	PrintField(buf, 12, 2, INT16, "overall-ascent");
	PrintField(buf, 14, 2, INT16, "overall-descent");
	PrintField(buf, 16, 4, INT32, "overall-width");
	PrintField(buf, 20, 4, INT32, "overall-left");
	PrintField(buf, 24, 4, INT32, "overall-right");
}

Global void
ListFonts(buf)
		unsigned char *buf;
{
	short   n;
	/* Request ListFonts is opcode 49 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ListFonts */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(2 + (n + p) / 4), "request length");
	PrintField(buf, 4, 2, CARD16, "max-names");
	printfield(buf, 6, 2, DVALUE2(n), "length of pattern");
	n = IShort(&buf[6]);
	PrintString8(&buf[8], n, "pattern");
}

Global void
ListFontsReply(buf)
		unsigned char *buf;
{
	short   n;

	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* ListFonts */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4((n + p) / 4), "reply length");
	printfield(buf, 8, 2, CARD16, "number of names");
	n = IShort(&buf[8]);
	PrintListSTR(&buf[32], (long)n, "names");
}

Global void
ListFontsWithInfo(buf)
		unsigned char *buf;
{
	short   n;
	/* Request ListFontsWithInfo is opcode 50 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ListFontsWithInfo */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(2 + (n + p) / 4), "request length");
	PrintField(buf, 4, 2, CARD16, "max-names");
	printfield(buf, 6, 2, DVALUE2(n), "length of pattern");
	n = IShort(&buf[6]);
	PrintString8(&buf[8], n, "pattern");
}

Global void
ListFontsWithInfoReply(buf)
		unsigned char *buf;
{
#ifdef NOT_YET
	short which;

	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* ListFontsWithInfo */ ;
	if (CurrentVerbose < 2) return;
	which = IByte(&buf[1]);
	if (which != 0)
		{
			ListFontsWithInfoReply1(buf);
			KeepLastReplyExpected();
		}
	else
		ListFontsWithInfoReply2(buf);
#endif
	Dprintf(("ERROR: ListFontsWithInfo not supported -- client may be unstable"))
}

Global void
ListFontsWithInfoReply1(buf)
		unsigned char *buf;
{
	short   n;
	short   m;
	printfield(buf, 1, 1, DVALUE1(n), "length of name in bytes");
	n = IByte(&buf[1]);
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4(7 + 2*m + (n + p) / 4), "reply length");
	PrintField(buf, 8, 12, CHARINFO, "min-bounds");
	PrintField(buf, 24, 12, CHARINFO, "max-bounds");
	PrintField(buf, 40, 2, CARD16, "min-char-or-byte2");
	PrintField(buf, 42, 2, CARD16, "max-char-or-byte2");
	PrintField(buf, 44, 2, CARD16, "default-char");
	printfield(buf, 46, 2, DVALUE2(m), "number of FONTPROPs");
	m = IShort(&buf[46]);
	PrintField(buf, 48, 1, DIRECT, "draw-direction");
	PrintField(buf, 49, 1, CARD8, "min-byte1");
	PrintField(buf, 50, 1, CARD8, "max-byte1");
	PrintField(buf, 51, 1, BOOL, "all-chars-exist");
	PrintField(buf, 52, 2, INT16, "font-ascent");
	PrintField(buf, 54, 2, INT16, "font-descent");
	PrintField(buf, 56, 4, CARD32, "replies-hint");
	PrintList(&buf[60], (long)m, FONTPROP, "properties");
	PrintString8(&buf[60 + 8 * m], n, "name");
}

Global void
ListFontsWithInfoReply2(buf)
		unsigned char *buf;
{
	PrintField(buf, 1, 1, CONST1(0), "last-reply indicator");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(7), "reply length");
}

Global void
SetFontPath(buf)
		unsigned char *buf;
{
	short   n;
	/* Request SetFontPath is opcode 51 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* SetFontPath */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(2 + (n + p) / 4), "request length");
	printfield(buf, 4, 2, CARD16, "number of paths");
	n = IShort(&buf[4]);
	PrintListSTR(&buf[8], (long)n, "paths");
}

Global void
GetFontPath(buf)
		unsigned char *buf;
{
	/* Request GetFontPath is opcode 52 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetFontPath */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 2, 2, CONST2(1), "request list");
}

Global void
GetFontPathReply(buf)
		unsigned char *buf;
{
	short   n;
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetFontPath */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4((n + p) / 4), "reply length");
	printfield(buf, 8, 2, CARD16, "number of paths");
	n = IShort(&buf[8]);
	PrintListSTR(&buf[32], (long)n, "paths");
}

Global void
CreatePixmap(unsigned char *buf)
{
#ifdef XMOVE
    PixmapPtr temp_pixmap;
    Drawable drawable_id = ILong(&buf[8]);
    union {WindowPtr win; PixmapPtr pix;} drawable;

    temp_pixmap = Tcalloc(1, PixmapRec);
    temp_pixmap -> pixmap_id = ILong(&buf[4]);
    temp_pixmap -> drawable = drawable_id;
    temp_pixmap -> width = IShort(&buf[12]); 
    temp_pixmap -> height = IShort(&buf[14]);
    temp_pixmap -> depth = IByte(&buf[1]);

    /* if the client has created custom colormaps, then locate the drawable
       and set this pixmap's colormap to it's colormap. */

    if (client->allocated_colormaps == 0)
	 temp_pixmap->cmap = (ColormapPtr)client->colormap_list.top->contents;
    else {
	 if (drawable.win = FindWindowFromCurrentClient(drawable_id))
	      temp_pixmap->cmap = drawable.win->cmap;
	 else if (drawable.pix = FindPixmapFromCurrentClient(drawable_id))
	      temp_pixmap->cmap = drawable.pix->cmap;
	 else
	      temp_pixmap->cmap = (ColormapPtr)client->colormap_list.top->contents;
    }
    
    AddPixmapToCurrentClient(temp_pixmap);
    Dprintf(("adding pixmap to client with drawable %x\n",ILong(&buf[8])));
    if (temp_pixmap->depth > 1)
	 ISetByte(&buf[1], server->default_depth);
    ISetLong(&buf[4], MapPixmapID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapDrawable(ILong(&buf[8]), Request));
#endif

    /* Request CreatePixmap is opcode 53 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CreatePixmap */ ;
    if (CurrentVerbose < 2)
	return;

    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    PrintField(buf, 1, 1, CARD8, "depth");
    printfield(buf, 2, 2, CONST2(4), "request length");
    PrintField(buf, 4, 4, PIXMAP, "pixmap-id");
    PrintField(buf, 8, 4, DRAWABLE, "drawable");
    PrintField(buf, 12, 2, CARD16, "width");
    PrintField(buf, 14, 2, CARD16, "height");
}

Global void
FreePixmap(unsigned char *buf)
{
#ifdef XMOVE

  PixmapPtr temp_pixmap = FindPixmapFromCurrentClient(ILong(&buf[4]));

  /* Mark this pixmap for later removal from the new server after a switch
     is complete.  To understand this, see the comment on FreeGC() */

  Dprintf(("XMOVE got a free pixmap request on pixmap 0x%x\n",ILong(&buf[4])))

  ISetLong(&buf[4], MapPixmapID(ILong(&buf[4]), Request));
  
  if (temp_pixmap) {
       if (temp_pixmap->used_by_cursor) {
	    if (client->grabbed_server) {
		 /* clear server's buffer before sending getimage request. */
		 /* for all we know the CreatePixmap call for this pixmap */
		 /* is still waiting in the outBuffer for the server. */
		 
		 while (server->fdd->outBuffer.num_Saved)
		      SendXmoveBuffer(server->fdd->fd, &server->fdd->outBuffer);
		 
		 /* since the GetImage request will go out before the
		    FreePixmap request we need to decrement the SeqNum
		    for XMOVEGetImage() */
		 
		 while (temp_pixmap->pixmap_image == NULL) {
		      --client->SequenceNumber;
		      temp_pixmap->pixmap_image =
			   XMOVEGetImage(server->fdd->fd, &client->SequenceNumber,
					 server, ILong(&buf[4]), 0, 0,
					 temp_pixmap->width, temp_pixmap->height,
					 0xFFFFFFFF, XYPixmap);
		      ++client->SequenceNumber;
		      --client->SequenceMapping;
		 }
	    } else {
		 synchronize_server_1();
		 synchronize_server_2();
		 
		 temp_pixmap->pixmap_image =
		      XMOVEGetImage(server->alt_fd, &server->alt_seqno,
				    server, ILong(&buf[4]), 0, 0,
				    temp_pixmap->width, temp_pixmap->height,
				    0xFFFFFFFF, XYPixmap);

		 ignore_bytes = True;
		 SendBuffer(server->alt_fd, buf, 8);
		 ++server->alt_seqno;
	    }
       } else {
	    RemovePixmapFromCurrentClient(temp_pixmap->pixmap_id);
       }
  } else
       printf ("Free pixmap called on unknown pixmap\n");


#endif

	/* Request FreePixmap is opcode 54 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* FreePixmap */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, PIXMAP, "pixmap");
}

Global void
CreateGC(unsigned char *buf)
{
#ifdef XMOVE
     GContext gc = ILong(&buf[4]);
     Drawable drawable = ILong(&buf[8]);
     GCPtr temp_gc;
     PixmapPtr temp_pixmap;
     
     
     temp_gc = FindGCFromCurrentClient(gc);
     
     if (temp_gc == (GCPtr) NULL) {
	  PixmapPtr pixmap = FindPixmapFromCurrentClient(drawable);
	  
	  temp_gc = Tmalloc(GCRec);
	  temp_gc -> gc_id = gc;
	  temp_gc -> drawable_id = drawable;
	  temp_gc -> values_mask = 0;
	  if (pixmap && pixmap->depth == 1)
	       temp_gc->depth = 1;
	  else
	       temp_gc->depth = client->default_depth;
	  temp_gc -> cmap = (ColormapPtr)TopOfList(&client->colormap_list);
	  AddGCToCurrentClient(temp_gc);
     }
     
     ParseGCBitmaskValueList(temp_gc,
			     (unsigned long) ILong(&buf[12]),
			     (unsigned char *) &buf[16]);
     
     ISetLong(&buf[4], MapGCID(gc, Request));
     ISetLong(&buf[8], MapDrawable(drawable, Request));
     
     
     MapGCBitmaskValueList(temp_gc,
			   (unsigned long) ILong(&buf[12]),
			   (unsigned char *) &buf[16]);

#endif

    /* Request CreateGC is opcode 55 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CreateGC */ ;

    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 2, 2, DVALUE2(4 + n), "request length");
    PrintField(buf, 4, 4, GCONTEXT, "graphic-context-id");
    PrintField(buf, 8, 4, DRAWABLE, "drawable");
    PrintField(buf, 12, 4, GC_BITMASK, "value-mask");
    PrintValues(&buf[12], 4, GC_BITMASK, &buf[16], "value-list");
}

Global void
ChangeGC(unsigned char *buf)
{
#ifdef XMOVE
    GCPtr temp_gc;

    temp_gc = FindGCFromCurrentClient((GContext) ILong(&buf[4]));
    
    if(temp_gc == (GCPtr) NULL)
    {
	 Dprintf(("XMOVE ChangeGC:  invalid gc id.\n"))
    }
    else
    {
	ParseGCBitmaskValueList(temp_gc,
				(unsigned long) ILong(&buf[8]),
				(unsigned char *) &buf[12]);


	MapGCBitmaskValueList(temp_gc,
			      (unsigned long) ILong(&buf[8]),
			      (unsigned char *) &buf[12]);

    }


    ISetLong(&buf[4], MapGCID(ILong(&buf[4]), Request));

#endif
	
    /* Request ChangeGC is opcode 56 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ChangeGC */ ;

    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 2, 2, DVALUE2(3 + n), "request length");
    PrintField(buf, 4, 4, GCONTEXT, "gc");
    PrintField(buf, 8, 4, GC_BITMASK, "value-mask");
    PrintValues(&buf[8], 4, GC_BITMASK, &buf[12], "value-list");
}

Global void
CopyGC(buf)
		unsigned char *buf;
{
#ifdef XMOVE
    GCPtr temp_gc_src;
    GCPtr temp_gc_dst;

    temp_gc_src = FindGCFromCurrentClient((GContext) ILong(&buf[4]));
    temp_gc_dst = FindGCFromCurrentClient((GContext) ILong(&buf[8]));
    
    if(temp_gc_src == (GCPtr) NULL)
      Dprintf(("CopyGC:  invalid gc id for source.\n"))

    else if(temp_gc_dst == (GCPtr) NULL)
      Dprintf(("CopyGC:  invalid gc id for destination.\n"))
    
    else
      ParseGCBitmaskcopy(temp_gc_src, temp_gc_dst,
			 (unsigned long) (ILong(&buf[12])
					  & temp_gc_src->values_mask));

    ISetLong(&buf[4], MapGCID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapGCID(ILong(&buf[8]), Request));
#endif

	/* Request CopyGC is opcode 57 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CopyGC */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(4), "request length");
	PrintField(buf, 4, 4, GCONTEXT, "src-gc");
	PrintField(buf, 8, 4, GCONTEXT, "dst-gc");
	PrintField(buf, 12, 4, GC_BITMASK, "value-mask");
}

Global void
SetDashes(buf)
		unsigned char *buf;
{
	short   n;

#ifdef XMOVE
    ISetLong(&buf[4], MapGCID(ILong(&buf[4]), Request));
#endif

	/* Request SetDashes is opcode 58 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* SetDashes */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(3 + (n + p) / 4), "request length");
	PrintField(buf, 4, 4, GCONTEXT, "gc");
	PrintField(buf, 8, 2, CARD16, "dash-offset");
	printfield(buf, 10, 2, DVALUE2(n), "length of dashes");
	n = IShort(&buf[10]);
	PrintBytes(&buf[12], (long)n, "dashes");
}

Global void
SetClipRectangles(buf)
		unsigned char *buf;
{
	unsigned int n;

#ifdef XMOVE
    ISetLong(&buf[4], MapGCID(ILong(&buf[4]), Request));
#endif

	/* Request SetClipRectangles is opcode 59 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* SetClipRectangles */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, RECTORDER, "ordering");
	printfield(buf, 2, 2, DVALUE2(3 + 2*n), "request length");
	n = (IShort(&buf[2]) - 3) / (unsigned)2;
	PrintField(buf, 4, 4, GCONTEXT, "gc");
	PrintField(buf, 8, 2, INT16, "clip-x-origin");
	PrintField(buf, 10, 2, INT16, "clip-y-origin");
	PrintList(&buf[12], (long)n, RECTANGLE, "rectangles");
}

Global void
FreeGC(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  
  GContext gc = ILong(&buf[4]);

  Dprintf(("XMOVE got a free gc request \n"));
  
  ISetLong(&buf[4], MapGCID(gc, Request));
  RemoveGCFromCurrentClient(gc);

#endif

	/* Request FreeGC is opcode 60 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* FreeGC */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, GCONTEXT, "gc");
}

Global void
ClearArea(unsigned char *buf)
{
#ifdef XMOVE
    /* map the window ID */
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif

    /* Request ClearArea is opcode 61 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ClearArea */ ;
    
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    PrintField(buf, 1, 1, BOOL, "exposures");
    printfield(buf, 2, 2, CONST2(4), "request length");
    PrintField(buf, 4, 4, WINDOW, "window");
    PrintField(buf, 8, 2, INT16, "x");
    PrintField(buf, 10, 2, INT16, "y");
    PrintField(buf, 12, 2, CARD16, "width");
    PrintField(buf, 14, 2, CARD16, "height");
}

Global void
CopyArea(unsigned char *buf)
{

#ifdef XMOVE

  Drawable srcID = ILong(&buf[4]), dstID = ILong(&buf[8]);
  GContext GCID = ILong(&buf[12]);
  union { WindowPtr win; PixmapPtr pix; } src, dst;
  GCPtr gc = FindGCFromCurrentClient(GCID);
  Bool do_sync = True;

  /* There are four conditions: the source and the destination can
     both be either windows or pixmaps. From the perspective of
     colormaps, if they are both windows we don't need to do anything,
     as windows have specific colormaps. If one is pixmap and one is
     window, we convert the pixmap's contents to the colormap of the
     window. If both pixmaps, we convert the destination pixmap to that
     of the source. In all cases we convert the GC if necessary. */

     if ((src.win = FindWindowFromCurrentClient(srcID)) != NULL) {
	  if (((dst.pix = FindPixmapFromCurrentClient(dstID)) != NULL) &&
	      (dst.pix->cmap != src.win->cmap) && (dst.pix->depth > 1))
	  {
	       ConvertPixmapToColormap(dst.pix, src.win->cmap, &do_sync);
	  }
	  if ((gc != NULL) && (gc->cmap != src.win->cmap))
	       ConvertGCToColormap(gc, src.win->cmap, &do_sync);
	  
     } else if ((src.pix = FindPixmapFromCurrentClient(srcID)) != NULL) {
	  if ((dst.win = FindWindowFromCurrentClient(dstID)) != NULL) {
	       if ((src.pix->cmap != dst.win->cmap) && (src.pix->depth > 1)) {
		    ConvertPixmapToColormap(src.pix, dst.win->cmap, &do_sync);
	       }
	       if ((gc != NULL) && (gc->cmap != dst.win->cmap))
		    ConvertGCToColormap(gc, dst.win->cmap, &do_sync);
	       
	  } else if ((dst.pix = FindPixmapFromCurrentClient(dstID)) != NULL) {
	       if ((src.pix->cmap != dst.pix->cmap) && (dst.pix->depth > 1)) {
		    ConvertPixmapToColormap(src.pix, dst.pix->cmap, &do_sync);
	       }
	       if (dst.pix->cmap != gc->cmap)
		    ConvertGCToColormap(gc, dst.pix->cmap, &do_sync);
	  }
     }

  /* source */
  ISetLong(&buf[4], MapDrawable(srcID, Request));
  /* destination */
  ISetLong(&buf[8], MapDrawable(dstID, Request));
  ISetLong(&buf[12], MapGCID(GCID, Request));

  if (do_sync == False) {
       ignore_bytes = True;
       server->alt_seqno++;
       SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
       XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
  }
  
#endif

    /* Request CopyArea is opcode 62 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CopyArea */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 2, 2, CONST2(7), "request length");
    PrintField(buf, 4, 4, DRAWABLE, "src-drawable");
    PrintField(buf, 8, 4, DRAWABLE, "dst-drawable");
    PrintField(buf, 12, 4, GCONTEXT, "gc");
    PrintField(buf, 16, 2, INT16, "src-x");
    PrintField(buf, 18, 2, INT16, "src-y");
    PrintField(buf, 20, 2, INT16, "dst-x");
    PrintField(buf, 22, 2, INT16, "dst-y");
    PrintField(buf, 24, 2, CARD16, "width");
    PrintField(buf, 26, 2, CARD16, "height");
}

Global void
CopyPlane(unsigned char *buf)
{
#ifdef XMOVE
     
     Drawable srcID = ILong(&buf[4]), dstID = ILong(&buf[8]);
     GContext GCID = ILong(&buf[12]);
     union { WindowPtr win; PixmapPtr pix; } src, dst;
     GCPtr gc = FindGCFromCurrentClient(GCID);
     Bool do_sync = True;
     
     /* See CopyArea above for comments */

     if ((src.win = FindWindowFromCurrentClient(srcID)) != NULL) {
	  if (((dst.pix = FindPixmapFromCurrentClient(dstID)) != NULL) &&
	      (dst.pix->cmap != src.win->cmap) && (dst.pix->depth > 1))
	  {
	       ConvertPixmapToColormap(dst.pix, src.win->cmap, &do_sync);
	  }
	  if ((gc != NULL) && (gc->cmap != src.win->cmap))
	       ConvertGCToColormap(gc, src.win->cmap, &do_sync);
	  
     } else if ((src.pix = FindPixmapFromCurrentClient(srcID)) != NULL) {
	  if ((dst.win = FindWindowFromCurrentClient(dstID)) != NULL) {
	       if ((src.pix->cmap != dst.win->cmap) && (src.pix->depth > 1)) {
		    ConvertPixmapToColormap(src.pix, dst.win->cmap, &do_sync);
	       }
	       if ((gc != NULL) && (gc->cmap != dst.win->cmap))
		    ConvertGCToColormap(gc, dst.win->cmap, &do_sync);
	       
	  } else if ((dst.pix = FindPixmapFromCurrentClient(dstID)) != NULL) {
	       if ((src.pix->cmap != dst.pix->cmap) && (dst.pix->depth > 1)) {
		    ConvertPixmapToColormap(src.pix, dst.pix->cmap, &do_sync);
	       }
	       if (dst.pix->cmap != gc->cmap)
		    ConvertGCToColormap(gc, dst.pix->cmap, &do_sync);
	  }
     }
     
     /* source */
     ISetLong(&buf[4], MapDrawable(srcID, Request));
     /* destination */
     ISetLong(&buf[8], MapDrawable(dstID, Request));
     ISetLong(&buf[12], MapGCID(GCID, Request));

     if (do_sync == False) {
	  ignore_bytes = True;
	  server->alt_seqno++;
	  SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	  XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
     }
     
#endif
     
     /* Request CopyPlane is opcode 63 */
     PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CopyPlane */ ;
     if (CurrentVerbose < 2)
	  return;
     if (CurrentVerbose > 2)
	  PrintField(SBf, 0, 4, INT32, "sequence number");
     
     printfield(buf, 2, 2, CONST2(8), "request length");
     PrintField(buf, 4, 4, DRAWABLE, "src-drawable");
     PrintField(buf, 8, 4, DRAWABLE, "dst-drawable");
     PrintField(buf, 12, 4, GCONTEXT, "gc");
     PrintField(buf, 16, 2, INT16, "src-x");
     PrintField(buf, 18, 2, INT16, "src-y");
     PrintField(buf, 20, 2, INT16, "dst-x");
     PrintField(buf, 22, 2, INT16, "dst-y");
     PrintField(buf, 24, 2, CARD16, "width");
     
     PrintField(buf, 28, 4, CARD32, "bit-plane");
}

Global void
PolyPoint(unsigned char *buf)
{
    unsigned int n;
#ifdef XMOVE
    GContext gcid = ILong(&buf[8]);
    Drawable did = ILong(&buf[4]);

    ISetLong(&buf[4], MapDrawable(did, Request));
    ISetLong(&buf[8], MapGCID(gcid, Request));

    FixGCnPixColormaps(gcid, did);
	 
    if (ignore_bytes) {
	 server->alt_seqno++;
	 SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	 XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
    }
	 
#endif

    /* Request PolyPoint is opcode 64 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* PolyPoint */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    PrintField(buf, 1, 1, COORMODE, "coordinate-mode");
    printfield(buf, 2, 2, DVALUE2(3 + n), "request length");
    n = (IShort(&buf[2]) - 3);
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 4, GCONTEXT, "gc");
    PrintList(&buf[12], (long)n, POINT, "points");
}

Global void
PolyLine(unsigned char *buf)
{
    unsigned int n;

#ifdef XMOVE
    GContext gcid = ILong(&buf[8]);
    Drawable did = ILong(&buf[4]);

    ISetLong(&buf[4], MapDrawable(did, Request));
    ISetLong(&buf[8], MapGCID(gcid, Request));

    FixGCnPixColormaps(gcid, did);
	 
    if (ignore_bytes) {
	 server->alt_seqno++;
	 SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	 XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
    }
	 
#endif

    /* Request PolyLine is opcode 65 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* PolyLine */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    PrintField(buf, 1, 1, COORMODE, "coordinate-mode");
    printfield(buf, 2, 2, DVALUE2(3 + n), "request length");
    n = (IShort(&buf[2]) - 3);
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 4, GCONTEXT, "gc");
    PrintList(&buf[12], (long)n, POINT, "points");
}

Global void
PolySegment(unsigned char *buf)
{
    unsigned int n;

#ifdef XMOVE
    GContext gcid = ILong(&buf[8]);
    Drawable did = ILong(&buf[4]);

    ISetLong(&buf[4], MapDrawable(did, Request));
    ISetLong(&buf[8], MapGCID(gcid, Request));

    FixGCnPixColormaps(gcid, did);
	 
    if (ignore_bytes) {
	 server->alt_seqno++;
	 SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	 XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
    }
	 
#endif

    /* Request PolySegment is opcode 66 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* PolySegment */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 2, 2, DVALUE2(3 + 2*n), "request length");
    n = (IShort(&buf[2]) - 3) / (unsigned)2;
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 4, GCONTEXT, "gc");
    PrintList(&buf[12], (long)n, SEGMENT, "segments");
}

Global void
PolyRectangle(unsigned char *buf)
{
    unsigned int n;

#ifdef XMOVE
    GContext gcid = ILong(&buf[8]);
    Drawable did = ILong(&buf[4]);

    ISetLong(&buf[4], MapDrawable(did, Request));
    ISetLong(&buf[8], MapGCID(gcid, Request));

    FixGCnPixColormaps(gcid, did);
	 
    if (ignore_bytes) {
	 server->alt_seqno++;
	 SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	 XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
    }
	 
#endif

    /* Request PolyRectangle is opcode 67 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* PolyRectangle */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 2, 2, DVALUE2(3 + 2*n), "request length");
    n = (IShort(&buf[2]) - 3) / (unsigned)2;
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 4, GCONTEXT, "gc");
    PrintList(&buf[12], (long)n, RECTANGLE, "rectangles");
}

Global void
PolyArc(unsigned char *buf)
{
    unsigned int n;

#ifdef XMOVE
    GContext gcid = ILong(&buf[8]);
    Drawable did = ILong(&buf[4]);

    ISetLong(&buf[4], MapDrawable(did, Request));
    ISetLong(&buf[8], MapGCID(gcid, Request));

    FixGCnPixColormaps(gcid, did);
	 
    if (ignore_bytes) {
	 server->alt_seqno++;
	 SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	 XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
    }
	 
#endif

	/* Request PolyArc is opcode 68 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* PolyArc */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(3 + 3*n), "request length");
	n = (IShort(&buf[2]) - 3) / (unsigned)3;
	PrintField(buf, 4, 4, DRAWABLE, "drawable");
	PrintField(buf, 8, 4, GCONTEXT, "gc");
	PrintList(&buf[12], (long)n, ARC, "arcs");
}

Global void
FillPoly(unsigned char *buf)
{
	unsigned int n;

#ifdef XMOVE
    GContext gcid = ILong(&buf[8]);
    Drawable did = ILong(&buf[4]);

    ISetLong(&buf[4], MapDrawable(did, Request));
    ISetLong(&buf[8], MapGCID(gcid, Request));

    FixGCnPixColormaps(gcid, did);
	 
    if (ignore_bytes) {
	 server->alt_seqno++;
	 SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	 XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
    }
	 
#endif

	/* Request FillPoly is opcode 69 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* FillPoly */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(4 + n), "request length");
	n = (IShort(&buf[2]) - 4);
	PrintField(buf, 4, 4, DRAWABLE, "drawable");
	PrintField(buf, 8, 4, GCONTEXT, "gc");
	PrintField(buf, 12, 1, POLYSHAPE, "shape");
	PrintField(buf, 13, 1, COORMODE, "coordinate-mode");
	PrintList(&buf[16], (long)n, POINT, "points");
}

Global void
PolyFillRectangle(unsigned char *buf)
{
    unsigned int n;

#ifdef XMOVE
    GContext gcid = ILong(&buf[8]);
    Drawable did = ILong(&buf[4]);

    ISetLong(&buf[4], MapDrawable(did, Request));
    ISetLong(&buf[8], MapGCID(gcid, Request));

    FixGCnPixColormaps(gcid, did);
	 
    if (ignore_bytes) {
	 server->alt_seqno++;
	 SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	 XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
    }
	 
#endif

    /* Request PolyFillRectangle is opcode 70 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* PolyFillRectangle */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 2, 2, DVALUE2(3 + 2*n), "request length");
    n = (IShort(&buf[2]) - 3) / (unsigned)2;
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 4, GCONTEXT, "gc");
    PrintList(&buf[12], (long)n, RECTANGLE, "rectangles");
}

Global void
PolyFillArc(unsigned char *buf)
{
    unsigned int n = (IShort(&buf[2]) - 3) / (unsigned)3;

#ifdef XMOVE
    GContext gcid = ILong(&buf[8]);
    Drawable did = ILong(&buf[4]);

    ISetLong(&buf[4], MapDrawable(did, Request));
    ISetLong(&buf[8], MapGCID(gcid, Request));

    FixGCnPixColormaps(gcid, did);
	 
    if (ignore_bytes) {
	 server->alt_seqno++;
	 SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	 XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
    }
	 
#endif

    /* Request PolyFillArc is opcode 71 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* PolyFillArc */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 2, 2, DVALUE2(3 + 3*n), "request length");
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 4, GCONTEXT, "gc");
    PrintList(&buf[12], (long)n, ARC, "arcs");
}

Global void
PutImage(unsigned char *buf)
{
    unsigned int n;
    PixmapPtr temp_pixmap;

#ifdef XMOVE
    ProcessPutImage(buf);
#endif

    /* Request PutImage is opcode 72 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* PutImage */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    PrintField(buf, 1, 1, IMAGEMODE, "format");
    printfield(buf, 2, 2, DVALUE2(6 + (n + p) / 4), "request length");

    /* the size of the Image is overestimated by the following */
    /* computation of n, because we ignore that padding of the */
    /* request to a multiple of 4 bytes.  The image may not be a */
    /* multiple of 4 bytes.  The actual size of the image is */
    /* determined as follows: for format = Bitmap or format = */
    /* XYPixmap, the size is (left-pad + width) [ pad to multiple */
    /* of bitmap-scanline-pad from SetUpReply ] divide by 8 to get */
    /* bytes times height times depth for format = ZPixmap, take */
    /* the depth and use it to find the bits-per-pixel and */
    /* scanline-pad given in one of the SetUpReply DEPTH records. */
    /* width * bits-per-pixel pad to multiple of scanline-pad */
    /* divide by 8 to get bytes times height times depth For */
    /* simplicity, we ignore all this and just use the request */
    /* length to (over)estimate the size of the image */ 

    n = (IShort(&buf[2]) - 6) * 4;
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 4, GCONTEXT, "gc");
    PrintField(buf, 12, 2, CARD16, "width");
    PrintField(buf, 14, 2, CARD16, "height");
    PrintField(buf, 16, 2, INT16, "dst-x");
    PrintField(buf, 18, 2, INT16, "dst-y");
    PrintField(buf, 20, 1, CARD8, "left-pad");
    PrintField(buf, 21, 1, CARD8, "depth");
    if (CurrentVerbose > 3)
	 PrintBytes(&buf[24], (long)n, "data");
}

Global void
GetImage(unsigned char *buf)
{
#ifdef XMOVE
    char *temps = malloc(20);

    bcopy(buf, temps, 20);
    
    if (client->misc)
	free(client->misc);

    client->misc = temps;

    ISetLong(&buf[4], MapDrawable(ILong(&buf[4]), Request));
#endif

    /* Request GetImage is opcode 73 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetImage */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    PrintField(buf, 1, 1, IMAGEMODE, "format");
    printfield(buf, 2, 2, CONST2(5), "request length");
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 2, INT16, "x");
    PrintField(buf, 10, 2, INT16, "y");
    PrintField(buf, 12, 2, CARD16, "width");
    PrintField(buf, 14, 2, CARD16, "height");
    PrintField(buf, 16, 4, CARD32, "plane-mask");
}

Global void
GetImageReply(unsigned char *buf)
{
     long    n;

#ifdef XMOVE
     ProcessGetImage(buf);
#endif

    PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetImage */ ;
    if (CurrentVerbose < 2)
	return;
    PrintField(buf, 1, 1, CARD8, "depth");
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4((n + p) / 4), "reply length");

    /* to properly compute the actual size of the image, we have to */
    /* remember the width and height values from the request.  Again, */
    /* we (over)estimate its length from the length of the reply */

    n = ILong(&buf[4]) * 4;
    PrintField(buf, 8, 4, VISUALID, "visual");
    PrintBytes(&buf[32], n, "data");
}

Global void
PolyText8(unsigned char *buf)
{
	unsigned int n = (IShort(&buf[2]) - 4) * 4;

#ifdef XMOVE
    GContext gcid = ILong(&buf[8]);
    Drawable did = ILong(&buf[4]);

    ISetLong(&buf[4], MapDrawable(did, Request));
    ISetLong(&buf[8], MapGCID(gcid, Request));

    FixGCnPixColormaps(gcid, did);
	 
    if (ignore_bytes) {
	 server->alt_seqno++;
	 SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	 XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
    }
	 
#endif

	/* Request PolyText8 is opcode 74 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* PolyText8 */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(4 + (n + p) / 4), "request length");
	PrintField(buf, 4, 4, DRAWABLE, "drawable");
	PrintField(buf, 8, 4, GCONTEXT, "gc");
	PrintField(buf, 12, 2, INT16, "x");
	PrintField(buf, 14, 2, INT16, "y");
	PrintTextList8(&buf[16], n, "items");

	Dprintf(("In PolyText8.  n = %d\n",n))

}

Global void
PolyText16(unsigned char *buf)
{
	unsigned int n = (IShort(&buf[2]) - 4) * 4;

#ifdef XMOVE
    GContext gcid = ILong(&buf[8]);
    Drawable did = ILong(&buf[4]);

    ISetLong(&buf[4], MapDrawable(did, Request));
    ISetLong(&buf[8], MapGCID(gcid, Request));

    FixGCnPixColormaps(gcid, did);
	 
    if (ignore_bytes) {
	 server->alt_seqno++;
	 SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	 XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
    }
	 
#endif

	/* Request PolyText16 is opcode 75 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* PolyText16 */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(4 + (n + p) / 4), "request length");
	PrintField(buf, 4, 4, DRAWABLE, "drawable");
	PrintField(buf, 8, 4, GCONTEXT, "gc");
	PrintField(buf, 12, 2, INT16, "x");
	PrintField(buf, 14, 2, INT16, "y");
	PrintTextList16(&buf[16], n, "items");
}

Global void
ImageText8(unsigned char *buf)
{
    short   n = IByte(&buf[1]);

#ifdef XMOVE
    GContext gcid = ILong(&buf[8]);
    Drawable did = ILong(&buf[4]);

    ISetLong(&buf[4], MapDrawable(did, Request));
    ISetLong(&buf[8], MapGCID(gcid, Request));

    FixGCnPixColormaps(gcid, did);
	 
    if (ignore_bytes) {
	 server->alt_seqno++;
	 SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	 XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
    }
	 
#endif

    /* Request ImageText8 is opcode 76 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ImageText8 */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 1, 1, DVALUE1(n), "length of string");
    printfield(buf, 2, 2, DVALUE2(4 + (n + p) / 4), "request length");
    PrintField(buf, 4, 4, DRAWABLE, "drawable");
    PrintField(buf, 8, 4, GCONTEXT, "gc");
    PrintField(buf, 12, 2, INT16, "x");
    PrintField(buf, 14, 2, INT16, "y");
    PrintString8(&buf[16], n, "string");
}

Global void
ImageText16(unsigned char *buf)
{
     short   n = IByte(&buf[1]);

#ifdef XMOVE
     GContext gcid = ILong(&buf[8]);
     Drawable did = ILong(&buf[4]);
     
     ISetLong(&buf[4], MapDrawable(did, Request));
     ISetLong(&buf[8], MapGCID(gcid, Request));
     
     FixGCnPixColormaps(gcid, did);
     
     if (ignore_bytes) {
	  server->alt_seqno++;
	  SendBuffer(server->alt_fd, &buf[0], IShort(&buf[2]) * 4);
	  XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
     }
	 
#endif

	/* Request ImageText16 is opcode 77 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ImageText16 */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 1, 1, DVALUE1(n), "length of string");
	printfield(buf, 2, 2, DVALUE2(4 + (2*n + p) / 4), "request length");
	PrintField(buf, 4, 4, DRAWABLE, "drawable");
	PrintField(buf, 8, 4, GCONTEXT, "gc");
	PrintField(buf, 12, 2, INT16, "x");
	PrintField(buf, 14, 2, INT16, "y");
	PrintString16(&buf[16], n, "string");
}

Global void
CreateColormap(buf)
		unsigned char *buf;
{
#ifdef XMOVE
    ColormapPtr temp_colormap;

    temp_colormap = Tcalloc(1, ColormapRec);

    temp_colormap -> colormap_id = ILong(&buf[4]);
    temp_colormap -> visual_id = ILong(&buf[12]);
    temp_colormap -> cell_count = 1 << GetCurrentServerDefaultDepth();
    temp_colormap -> cell_array =
	 Tcalloc(temp_colormap->cell_count, ColorCellRec);

    AddColormapToCurrentClient(temp_colormap);

    /* Not sure what to do with visualid. */
    
    ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapWindowID(ILong(&buf[8]), Request));
    ISetLong(&buf[12], MapVisualID(ILong(&buf[12]), Request));
#endif

	/* Request CreateColormap is opcode 78 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CreateColormap */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, ALLORNONE, "alloc");
	printfield(buf, 2, 2, CONST2(4), "request length");
	PrintField(buf, 4, 4, COLORMAP, "color-map-id");
	PrintField(buf, 8, 4, WINDOW, "window");
	PrintField(buf, 12, 4, VISUALID, "visual");
}

Global void
FreeColormap(buf)
		unsigned char *buf;
{

#ifdef XMOVE
    ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Request));
    RemoveColormapFromCurrentClient(ILong(&buf[4]));
#endif


	/* Request FreeColormap is opcode 79 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* FreeColormap */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, COLORMAP, "cmap");
}

Global void
CopyColormapAndFree(buf)
		unsigned char *buf;
{
#ifdef XMOVE
    ColormapPtr temp_colormap = FindColormapFromCurrentClient(ILong(&buf[4]));
    ColormapPtr source_colormap = FindColormapFromCurrentClient(ILong(&buf[8]));

    if (temp_colormap)		/* should be NULL -- duplicate colormap */
	 RemoveColormapFromCurrentClient(temp_colormap->colormap_id);
    
    temp_colormap = Tcalloc(1, ColormapRec);

    temp_colormap -> colormap_id = ILong(&buf[4]);
    temp_colormap -> visual_id = ILong(&buf[12]);
    
    if (source_colormap) {
	 temp_colormap->cell_count = source_colormap->cell_count;
	 temp_colormap->cell_array =
	      Tcalloc(temp_colormap->cell_count, ColorCellRec);

	 memcpy(temp_colormap->cell_array, source_colormap->cell_array,
		temp_colormap->cell_count * sizeof(ColorCellRec));
	 memset(source_colormap->cell_array, 0, sizeof(ColorCellRec));
    } else {			/* we have to guess how many colors to alloc */
	 temp_colormap->cell_count = 1 << GetCurrentServerDefaultDepth();
	 temp_colormap->cell_array =
	      Tcalloc(temp_colormap->cell_count, ColorCellRec);
    }
    
    AddColormapToCurrentClient(temp_colormap);
#endif

	/* Request CopyColormapAndFree is opcode 80 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CopyColormapAndFree */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(3), "request length");
	PrintField(buf, 4, 4, COLORMAP, "color-map-id");
	PrintField(buf, 8, 4, COLORMAP, "src-cmap");
}

Global void
InstallColormap(buf)
		unsigned char *buf;
{
#ifdef XMOVE
    ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Request));
#endif  

	/* Request InstallColormap is opcode 81 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* InstallColormap */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, COLORMAP, "cmap");
}

Global void
UninstallColormap(buf)
		unsigned char *buf;
{
#ifdef XMOVE
    ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Request));
#endif  

	/* Request UninstallColormap is opcode 82 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* UninstallColormap */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, COLORMAP, "cmap");
}

Global void
ListInstalledColormaps(buf)
		unsigned char *buf;
{
#ifdef XMOVE
    ISetLong(&buf[4], MapWindowID(ILong(&buf[4]), Request));
#endif  

	/* Request ListInstalledColormaps is opcode 83 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ListInstalledColormaps */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
}

Global void
ListInstalledColormapsReply(buf)
		unsigned char *buf;
{
	short   n;
	int count;
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* ListInstalledColormaps */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4(n), "reply length");
	printfield(buf, 8, 2, DVALUE2(n), "number of cmaps");
	n = IShort(&buf[8]);

#ifdef XMOVE
  /* Now remap n colormaps */
  for (count=0; count < n; count++)
      ISetLong(&buf[32+(4*count)], MapColormapID(ILong(&buf[32+(4*count)]), Reply));
#endif


	PrintList(&buf[32], (long)n, COLORMAP, "cmaps");
}

Global void
AllocColor(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     AllocCmapTemps *temps = malloc(sizeof(AllocCmapTemps));

     temps->cmap  = FindColormapFromCurrentClient(ILong(&buf[4]));
     temps->red   = IShort(&buf[8]);
     temps->green = IShort(&buf[10]);
     temps->blue  = IShort(&buf[12]);

     /* if the client thinks it is on a color machine, but is actually
	on a B&W machine, we must convert its red, green and blue
	values into an average value and stuff that average back into the
	red, green and blue. */
     
     if (client->color == True && server->color == False) {
	  unsigned short bw_value = RGB2BW(temps->red, temps->green, temps->blue);

	  ISetShort(&buf[8], bw_value);
	  ISetShort(&buf[10], bw_value);
	  ISetShort(&buf[12], bw_value);
     }
	  
     if (client->misc)
	  free(client->misc);
     
     client->misc = temps;
     
     ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Request));
#endif

	/* Request AllocColor is opcode 84 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* AllocColor */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(4), "request length");
	PrintField(buf, 4, 4, COLORMAP, "cmap");
	PrintField(buf, 8, 2, CARD16, "red");
	PrintField(buf, 10, 2, CARD16, "green");
	PrintField(buf, 12, 2, CARD16, "blue");
}

Global void
AllocColorReply(buf)
unsigned char *buf;
{
#ifdef XMOVE
     AllocCmapTemps *temps = client->misc;
     
     ColorCellPtr cell;
     
     cell = AllocNewColorCell(ILong(&buf[16]), temps->cmap, False,
			      temps->red, temps->green, temps->blue, NULL);
     cell->usage_count++;
     
     ISetLong(&buf[16], cell->client_pixel);
     
     free(temps);
     client->misc = NULL;
#endif

	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* AllocColor */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 2, CARD16, "red");
	PrintField(buf, 10, 2, CARD16, "green");
	PrintField(buf, 12, 2, CARD16, "blue");
	PrintField(buf, 16, 4, CARD32, "pixel");
}

Global void
AllocNamedColor(buf)
		unsigned char *buf;
{
     short   n = IShort(&buf[8]);

#ifdef XMOVE  
     ColormapPtr *temps = malloc(sizeof(ColormapPtr));
     char *name = malloc(n+1);

     *temps = FindColormapFromCurrentClient(ILong(&buf[4]));
     
     strncpy(name, (char *)buf+12, n);
     name[n] = '\0';

     if (client->misc)
	  free(client->misc);

     client->misc = temps;

     if (client->misc2)
	  free(client->misc2);

     client->misc2 = name;

     ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Request));
#endif

	/* Request AllocNamedColor is opcode 85 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* AllocNamedColor */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(3 + (n + p) / 4), "request length");
	PrintField(buf, 4, 4, COLORMAP, "cmap");
	printfield(buf, 8, 2, DVALUE2(n), "length of name");
	PrintString8(&buf[12], n, "name");
}

Global void
AllocNamedColorReply(buf)
		unsigned char *buf;
{
#ifdef XMOVE
     ColormapPtr cmap = *(ColormapPtr *)client->misc;
     char *name = client->misc2;
     
     ColorCellPtr cell;
     
     cell = AllocNewColorCell(ILong(&buf[8]), cmap, False, 0, 0, 0, name);
     cell->usage_count++;

     free(client->misc);	/* free ColormapPtr *, but not char *name */
     client->misc  = NULL;
     client->misc2 = NULL;
     
     ISetLong(&buf[8], cell->client_pixel);
#endif

	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* AllocNamedColor */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 4, CARD32, "pixel");
	PrintField(buf, 12, 2, CARD16, "exact-red");
	PrintField(buf, 14, 2, CARD16, "exact-green");
	PrintField(buf, 16, 2, CARD16, "exact-blue");
	PrintField(buf, 18, 2, CARD16, "visual-red");
	PrintField(buf, 20, 2, CARD16, "visual-green");
	PrintField(buf, 22, 2, CARD16, "visual-blue");
}

Global void
AllocColorCells(buf)
unsigned char *buf;
{
#ifdef XMOVE
     ColormapPtr *temps = malloc(sizeof(ColormapPtr));

     *temps = FindColormapFromCurrentClient(ILong(&buf[4]));
     
     if (client->misc)
	  free(client->misc);
     
     client->misc = temps;
     
     if (IShort(&buf[10]))
	  printf("client '%s' has allocated color planes and is not mobile\n",
		 GetCurrentClientName());

     ISetLong(&buf[4], MapColormapID((*temps)->colormap_id, Request));
#endif

	/* Request AllocColorCells is opcode 86 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* AllocColorCells */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, BOOL, "contiguous");
	printfield(buf, 2, 2, CONST2(3), "request length");
	PrintField(buf, 4, 4, COLORMAP, "cmap");
	PrintField(buf, 8, 2, CARD16, "colors");
	PrintField(buf, 10, 2, CARD16, "planes");
}

Global void
AllocColorCellsReply(buf)
unsigned char *buf;
{
     short   n = IShort(&buf[8]);
     short   m = IShort(&buf[10]);
     short   k = 32 + 4*n;
     short counter = m;

#ifdef XMOVE
     ColormapPtr cmap = *(ColormapPtr *)client->misc;
     unsigned char *curbuf = buf + 32;

     while (curbuf < buf + 32 + n*4) {
	  ColorCellPtr cell = AllocNewColorCell(ILong(curbuf), cmap, True,
						0, 0, 0, NULL);
	  cell->usage_count++;

	  ISetLong(curbuf, cell->client_pixel);

	  curbuf += 4;
     }

     free(client->misc);
     client->misc = NULL;
#endif     
     
     PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* AllocColorCells */ ;
     if (CurrentVerbose < 2)
	  return;
     printfield(buf, 2, 2, CARD16, "sequence number");
     printfield(buf, 4, 4, DVALUE4(n + m), "reply length");
     printfield(buf, 8, 2, DVALUE2(n), "number of pixels");
     printfield(buf, 10, 2, DVALUE2(m), "number of masks");
     PrintList(&buf[32], (long)n, CARD32, "pixels");
     PrintList(&buf[k], (long)m, CARD32, "masks");
}

Global void
AllocColorPlanes(buf)
unsigned char *buf;
{
#ifdef XMOVE
     ColormapPtr *temps = malloc(sizeof(ColormapPtr));

     *temps = FindColormapFromCurrentClient(ILong(&buf[4]));

     if (client->misc)
	  free(client->misc);
     
     client->misc = temps;
     
     if (IShort(&buf[10]) || IShort(&buf[12]) || IShort(&buf[14]))
	  printf("client '%s' has allocated color planes and is not mobile\n",
		 GetCurrentClientName());
     
     ISetLong(&buf[4], MapColormapID((*temps)->colormap_id, Request));
#endif

	/* Request AllocColorPlanes is opcode 87 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* AllocColorPlanes */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, BOOL, "contiguous");
	printfield(buf, 2, 2, CONST2(4), "request length");
	PrintField(buf, 4, 4, COLORMAP, "cmap");
	PrintField(buf, 8, 2, CARD16, "colors");
	PrintField(buf, 10, 2, CARD16, "reds");
	PrintField(buf, 12, 2, CARD16, "greens");
	PrintField(buf, 14, 2, CARD16, "blues");
}

Global void
AllocColorPlanesReply(buf)
unsigned char *buf;
{
     short   n = IShort(&buf[8]);
#ifdef XMOVE
     ColormapPtr cmap = *(ColormapPtr *)client->misc;
     unsigned char *curbuf = buf + 32;
     
     while (curbuf < buf + 32 + n*4) {
	  ColorCellPtr cell = AllocNewColorCell(ILong(curbuf), cmap, True,
						0, 0, 0, NULL);
	  cell->usage_count++;

	  ISetLong(curbuf, cell->client_pixel);

	  curbuf += 4;
     }

     free(client->misc);
     client->misc = NULL;
#endif
     
     PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* AllocColorPlanes */ ;
     if (CurrentVerbose < 2)
	  return;
     printfield(buf, 2, 2, CARD16, "sequence number");
     printfield(buf, 4, 4, DVALUE4(n), "reply length");
     printfield(buf, 8, 2, DVALUE2(n), "number of pixels");
     
     PrintField(buf, 12, 4, CARD32, "red-mask");
     PrintField(buf, 16, 4, CARD32, "green-mask");
     PrintField(buf, 20, 4, CARD32, "blue-mask");
     PrintList(&buf[32], (long)n, CARD32, "pixels");
}

Global void
FreeColors(buf)
unsigned char *buf;
{
     unsigned int n;
     
#ifdef XMOVE
     unsigned char *curbuf = buf + 12;
     ColormapPtr cmap;
     card32 pixel;
     
     if (cmap = FindColormapFromCurrentClient(ILong(&buf[4])))
	  while (curbuf < buf + IShort(&buf[2])*4) {
	       pixel = ILong(curbuf);
	       ISetLong(curbuf, MapColorCell(pixel, cmap, Request));
	       Dprintf(("Freeing color %d which maps to color %d\n",pixel,ILong(curbuf)));
	       RemoveColorCellFromColormap(pixel, cmap);
	       curbuf += 4;
	  }
     
     ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Request));
#endif
     
     /* Request FreeColors is opcode 88 */
     PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* FreeColors */ ;
     if (CurrentVerbose < 2)
	  return;
     if (CurrentVerbose > 2)
	  PrintField(SBf, 0, 4, INT32, "sequence number");
     
     printfield(buf, 2, 2, DVALUE2(3 + n), "request length");
     n = IShort(&buf[2]) - 3;
     PrintField(buf, 4, 4, COLORMAP, "cmap");
     PrintField(buf, 8, 4, CARD32, "plane-mask");
     PrintList(&buf[12], (long)n, CARD32, "pixels");
}

Global void
StoreColors(buf)
unsigned char *buf;
{
     unsigned int n;

#ifdef XMOVE
     ColormapPtr cmap = FindColormapFromCurrentClient(ILong(buf+4));
     unsigned char *curbuf = buf + 8;
     
     if (cmap) {
	  ColorCellPtr cell;

	  while (curbuf < buf + IShort(buf+2)*4) {
	       unsigned char color_mask;
	       
	       if(cell = FindColorCellFromColormap(ILong(curbuf), cmap)) {
		    if (cell->color_name) {
			 free(cell->color_name);
			 cell->color_name = NULL;
		    }
	       } else {
		    cell = AllocNewColorCell(ILong(curbuf), cmap, True,
					     0, 0, 0, NULL);
		    cell->usage_count++;
	       }
	       
	       cell->read_write = True;
	       ISetLong(curbuf, cell->server_pixel);
	       
	       color_mask = IByte(&curbuf[10]);
	       if (color_mask & DoRed)
		    cell->red   = IShort(&curbuf[4]);
	       if (color_mask & DoGreen)
		    cell->green = IShort(&curbuf[6]);
	       if (color_mask & DoBlue)
		    cell->blue  = IShort(&curbuf[8]);
	       
	       /* if the client thinks it is on a color machine, but is actually
		  on a B&W machine, we must convert its red, green and blue
		  values into an average value and stuff that average back into the
		  red, green and blue. */
	       
	       if (client->color == True && server->color == False) {
		    unsigned short bw_value =
			 RGB2BW(cell->red, cell->green, cell->blue);
		    
		    ISetShort(&curbuf[4], bw_value);
		    ISetShort(&curbuf[6], bw_value);
		    ISetShort(&curbuf[8], bw_value);
	       }
	       
	       curbuf += 12;
	  }
     }

     ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Request));
#endif

     /* Request StoreColors is opcode 89 */
     PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* StoreColors */ ;
     if (CurrentVerbose < 2)
	  return;
     if (CurrentVerbose > 2)
	  PrintField(SBf, 0, 4, INT32, "sequence number");
     
     printfield(buf, 2, 2, DVALUE2(2 + 3*n), "request length");
     n = (IShort(&buf[2]) - 2) / (unsigned)3;
     PrintField(buf, 4, 4, COLORMAP, "cmap");
     PrintList(&buf[8], (long)n, COLORITEM, "items");
}

Global void
StoreNamedColor(buf)
		unsigned char *buf;
{
     short   n;

#ifdef XMOVE
     ColormapPtr cmap = FindColormapFromCurrentClient(ILong(&buf[4]));
     
     if (cmap) {
	  unsigned short n = IShort(buf+12);
	  ColorCellPtr cell;
	  
	  if (cell = FindColorCellFromColormap(ILong(&buf[8]), cmap)) {
	       if (cell->color_name)
		    free(cell->color_name);
	  } else {
	       cell = AllocNewColorCell(ILong(&buf[8]), cmap, True, 0, 0, 0, NULL);
	       cell->usage_count++;
	  }

	  cell->read_write = True;
	  cell->color_name = malloc(n+1);
	  strncpy(cell->color_name, (char *)buf+16, n);
	  cell->color_name[n] = '\0';

	  ISetLong(&buf[8], cell->client_pixel);
     }
     
     ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Request));
#endif

	/* Request StoreNamedColor is opcode 90 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* StoreNamedColor */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, COLORMASK, "which colors?");
	printfield(buf, 2, 2, DVALUE2(4 + (n + p) / 4), "request length");
	PrintField(buf, 4, 4, COLORMAP, "cmap");
	PrintField(buf, 8, 4, CARD32, "pixel");
	printfield(buf, 12, 2, DVALUE2(n), "length of name");
	n = IShort(&buf[12]);
	PrintString8(&buf[16], n, "name");
}

Global void
QueryColors(buf)
unsigned char *buf;
{
     unsigned int n = IShort(&buf[2]) - 2;

#ifdef XMOVE
     unsigned char *curbuf = buf + 8;
     ColormapPtr cmap = FindColormapFromCurrentClient(ILong(&buf[4]));

     if (cmap)
	  while (curbuf < buf + 8 + n*4) {
	       ISetLong(curbuf, MapColorCell(ILong(curbuf), cmap, Request));
	       curbuf += 4;
	  }
					
     ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Request));
#endif

	/* Request QueryColors is opcode 91 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* QueryColors */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(2 + n), "request length");
	PrintField(buf, 4, 4, COLORMAP, "cmap");
	PrintList(&buf[8], (long)n, CARD32, "pixels");
}

Global void
QueryColorsReply(buf)
		unsigned char *buf;
{
	short   n;
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* QueryColors */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4(2*n), "reply length");
	printfield(buf, 8, 2, DVALUE2(n), "number of colors");
	n = IShort(&buf[8]);
	PrintList(&buf[32], (long)n, RGB, "colors");
}

Global void
LookupColor(buf)
		unsigned char *buf;
{
	short   n;

	/* Request LookupColor is opcode 92 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* LookupColor */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(3 + (n + p) / 4), "request length");
	PrintField(buf, 4, 4, COLORMAP, "cmap");
	printfield(buf, 8, 2, DVALUE2(n), "length of name");
	n = IShort(&buf[8]);
	PrintString8(&buf[12], n, "name");

#ifdef XMOVE  
  ISetLong(&buf[4], MapColormapID(ILong(&buf[4]), Request));
#endif
}

Global void
LookupColorReply(buf)
		unsigned char *buf;
{
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* LookupColor */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 2, CARD16, "exact-red");
	PrintField(buf, 10, 2, CARD16, "exact-green");
	PrintField(buf, 12, 2, CARD16, "exact-blue");
	PrintField(buf, 14, 2, CARD16, "visual-red");
	PrintField(buf, 16, 2, CARD16, "visual-green");
	PrintField(buf, 18, 2, CARD16, "visual-blue");
}

Global void
CreateCursor(buf)
		unsigned char *buf;
{

#ifdef XMOVE
    CursorPtr temp_cursor;
    PixmapPtr temp_pixmap;

    temp_cursor = Tmalloc(CursorRec);

    temp_cursor -> cursor_id = ILong(&buf[4]);
    temp_cursor -> source_pixmap = ILong(&buf[8]);
    temp_cursor -> mask_pixmap = ILong(&buf[12]);
    temp_cursor -> foreground_color.red = IShort(&buf[16]);
    temp_cursor -> foreground_color.green = IShort(&buf[18]);
    temp_cursor -> foreground_color.blue = IShort(&buf[20]);
    temp_cursor -> background_color.red = IShort(&buf[22]);
    temp_cursor -> background_color.green = IShort(&buf[24]);
    temp_cursor -> background_color.blue = IShort(&buf[26]);
    temp_cursor -> x = IShort(&buf[28]);
    temp_cursor -> y = IShort(&buf[30]);

    AddCursorToCurrentClient(temp_cursor);

    temp_pixmap = FindPixmapFromCurrentClient((Pixmap) ILong(&buf[8]));
    if (temp_pixmap)
	 temp_pixmap->used_by_cursor = True;
    else
	 printf ("Cursor created with unknown source pixmap\n");

    ISetLong(&buf[4], MapCursorID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapPixmapID(ILong(&buf[8]), Request));
    if (ILong(&buf[12])) {
	 temp_pixmap = FindPixmapFromCurrentClient((Pixmap) ILong(&buf[12]));
	 if (temp_pixmap)
	      temp_pixmap->used_by_cursor = True;
	 else
	      printf ("Cursor created with unknown mask pixmap\n");
	 
	 ISetLong(&buf[12], MapPixmapID(ILong(&buf[12]), Request));
    }

#endif

	/* Request CreateCursor is opcode 93 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CreateCursor */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(8), "request length");
	PrintField(buf, 4, 4, CURSOR, "cursor-id");
	PrintField(buf, 8, 4, PIXMAP, "source");
	PrintField(buf, 12, 4, PIXMAP, "mask");
	PrintField(buf, 16, 2, CARD16, "fore-red");
	PrintField(buf, 18, 2, CARD16, "fore-green");
	PrintField(buf, 20, 2, CARD16, "fore-blue");
	PrintField(buf, 22, 2, CARD16, "back-red");
	PrintField(buf, 24, 2, CARD16, "back-green");
	PrintField(buf, 26, 2, CARD16, "back-blue");
	PrintField(buf, 28, 2, CARD16, "x");
	PrintField(buf, 30, 2, CARD16, "y");
}

Global void
CreateGlyphCursor(unsigned char *buf)
{
#ifdef XMOVE
    GlyphCursorPtr temp_cursor;

    temp_cursor = Tmalloc(GlyphCursorRec);

    temp_cursor -> cursor_id = ILong(&buf[4]); 
    temp_cursor -> source_font = ILong(&buf[8]);
    temp_cursor -> mask_font = ILong(&buf[12]);
    temp_cursor -> source_char = IShort(&buf[16]);
    temp_cursor -> mask_char = IShort(&buf[18]);

    temp_cursor -> foreground_color.red = IShort(&buf[20]);
    temp_cursor -> foreground_color.green = IShort(&buf[22]);
    temp_cursor -> foreground_color.blue = IShort(&buf[24]);
    temp_cursor -> foreground_color.flags = DoRed | DoGreen | DoBlue;

    temp_cursor -> background_color.red = IShort(&buf[26]);
    temp_cursor -> background_color.green = IShort(&buf[28]);
    temp_cursor -> background_color.blue = IShort(&buf[30]);
    temp_cursor -> background_color.flags = DoRed | DoGreen | DoBlue;

    AddGlyphCursorToCurrentClient(temp_cursor);

    ISetLong(&buf[4], MapCursorID(ILong(&buf[4]), Request));
    ISetLong(&buf[8], MapFontID(ILong(&buf[8]), Request));
    
    if (ILong(&buf[12]))
      ISetLong(&buf[12], MapFontID(ILong(&buf[12]), Request));
    
#endif

    /* Request CreateGlyphCursor is opcode 94 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* CreateGlyphCursor */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 2, 2, CONST2(8), "request length");
    PrintField(buf, 4, 4, CURSOR, "cursor-id");
    PrintField(buf, 8, 4, FONT, "source-font");
    PrintField(buf, 12, 4, FONT, "mask-font");
    PrintField(buf, 16, 2, CARD16, "source-char");
    PrintField(buf, 18, 2, CARD16, "mask-char");
    PrintField(buf, 20, 2, CARD16, "fore-red");
    PrintField(buf, 22, 2, CARD16, "fore-green");
    PrintField(buf, 24, 2, CARD16, "fore-blue");
    PrintField(buf, 26, 2, CARD16, "back-red");
    PrintField(buf, 28, 2, CARD16, "back-green");
    PrintField(buf, 30, 2, CARD16, "back-blue");
}

Global void
FreeCursor(unsigned char *buf)
{
#ifdef XMOVE
    /* remove this cursor from the current client, and map the ID */

    RemoveCursorFromCurrentClient(ILong(&buf[4]));
    ISetLong(&buf[4], MapCursorID(ILong(&buf[4]), Request));
#endif
	/* Request FreeCursor is opcode 95 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* FreeCursor */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, CURSOR, "cursor");
}

Global void
RecolorCursor(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  /* first, make the changes to the cursor data structure that xmove is 
     storing so that the change is seen when a client is moved */

  Cursor xcursor;
  short fg_r, fg_g, fg_b, bg_r, bg_g, bg_b;

  xcursor = ILong(&buf[4]);
  fg_r = IShort(&buf[8]);
  fg_g = IShort(&buf[10]);
  fg_b = IShort(&buf[12]);
  bg_r = IShort(&buf[14]);
  bg_g = IShort(&buf[16]);
  bg_b = IShort(&buf[18]);
  RecolorCursorFromCurrentClient(xcursor,fg_r,fg_g,fg_b,bg_r,bg_g,bg_b);
  
  ISetLong(&buf[4], MapCursorID(ILong(&buf[4]), Request));  
#endif
	/* Request RecolorCursor is opcode 96 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* RecolorCursor */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(5), "request length");
	PrintField(buf, 4, 4, CURSOR, "cursor");
	PrintField(buf, 8, 2, CARD16, "fore-red");
	PrintField(buf, 10, 2, CARD16, "fore-green");
	PrintField(buf, 12, 2, CARD16, "fore-blue");
	PrintField(buf, 14, 2, CARD16, "back-red");
	PrintField(buf, 16, 2, CARD16, "back-green");
	PrintField(buf, 18, 2, CARD16, "back-blue");
}

Global void
QueryBestSize(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  ISetLong(&buf[4], MapDrawable(ILong(&buf[4]), Request));
#endif

	/* Request QueryBestSize is opcode 97 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* QueryBestSize */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, OBJECTCLASS, "class");
	printfield(buf, 2, 2, CONST2(3), "request length");
	PrintField(buf, 4, 4, DRAWABLE, "drawable");
	PrintField(buf, 8, 2, CARD16, "width");
	PrintField(buf, 10, 2, CARD16, "height");
}

Global void
QueryBestSizeReply(buf)
		unsigned char *buf;
{
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* QueryBestSize */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 2, CARD16, "width");
	PrintField(buf, 10, 2, CARD16, "height");
}

Global void
QueryExtension(buf)
		unsigned char *buf;
{
	short   n;
	/* Request QueryExtension is opcode 98 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* QueryExtension */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(2 + (n + p) / 4), "request length");
	printfield(buf, 4, 2, DVALUE2(n), "length of name");
	n = IShort(&buf[4]);
	PrintString8(&buf[8], n, "name");
}

Global void
QueryExtensionReply(buf)
		unsigned char *buf;
{
#ifdef XMOVE
  ISetByte(&buf[8],0);
  ISetByte(&buf[9],0);
  ISetByte(&buf[10],0);
  ISetByte(&buf[11],0);
#endif
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* QueryExtension */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 1, BOOL, "present");
	PrintField(buf, 9, 1, CARD8, "major-opcode");
	PrintField(buf, 10, 1, CARD8, "first-event");
	PrintField(buf, 11, 1, CARD8, "first-error");
}

Global void
ListExtensions(buf)
		unsigned char *buf;
{
	/* Request ListExtensions is opcode 99 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ListExtensions */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
ListExtensionsReply(buf)
		unsigned char *buf;
{
	short   n;

	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* ListExtensions */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 1, 1, CARD8, "number names");
	n = IByte(&buf[1]);
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4((n + p) / 4), "reply length");
	PrintListSTR(&buf[32], (long)n, "names");
}

Global void
ChangeKeyboardMapping(buf)
		unsigned char *buf;
{
	short   n;
	short   m;

	/*check this.  should we change the keyboard mapping stuff here */

	/* Request ChangeKeyboardMapping is opcode 100 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ChangeKeyboardMapping */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, DVALUE1(n), "keycode-count");
	n = IByte(&buf[1]);
	printfield(buf, 2, 2, DVALUE2(2 + nm), "request length");
	PrintField(buf, 4, 1, KEYCODE, "first-keycode");
	PrintField(buf, 5, 1, DVALUE1(m), "keysyms-per-keycode");
	m = IByte(&buf[5]);
	PrintList(&buf[8], (long)(n * m), KEYSYM, "keysyms");
}

Global void
GetKeyboardMapping(unsigned char *buf)
{
#ifdef XMOVE
    unsigned char client_first, client_count, client_max;
    unsigned char server_min, server_count, server_max;
    Bool modified = False;
    
    /* the client believes that the server has keys ranging from 0x0 */
    /* to 0xff, but the server may not have that many.  If this */
    /* request is within the legal server limits, then we ignore it, */
    /* otherwise we need to make this request fit the server, and pad */
    /* the reply to make the client happy */

    client_first = IByte(&buf[4]);
    client_count = IByte(&buf[5]);
    client_max = client_first + client_count - 1;

    server_min = GetCurrentServerMinKeycode();
    server_max = GetCurrentServerMaxKeycode();
    server_count = server_max - server_min + 1;

    if (client_first < server_min)
    {
	client_first = server_min;
	modified = True; 
    }

    if (client_max > server_max)
    {
	client_max = server_max;
	modified = True;
    }

    client_count = client_max - client_first + 1;

    /* what should go out at this point are (min, count), because that */
    /* is all the current server can handle.  But, we need to remember */
    /* what the client requested, so that we can pad the response when */
    /* it comes from the server, so we can suite the client */

    if(modified == True)
    {
	ModifiedKeyMappingReplyExpected(101, 		    /* reply type */
				IByte(&buf[4]),     /* client first_keycode */
				IByte(&buf[5]),	    /* client count */
				client_first, 	    /* server first_keycode */
				client_count);	    /* server count */
    }
    else
    {
	ReplyExpected(101);
    }

    /* update the request */

    ISetByte(&buf[4], client_first);
    ISetByte(&buf[5], client_count);
#endif

    /* Request GetKeyboardMapping is opcode 101 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetKeyboardMapping */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");

    printfield(buf, 2, 2, CONST2(2), "request length");
    PrintField(buf, 4, 1, KEYCODE, "first-keycode");
    PrintField(buf, 5, 1, CARD8, "count");
}

Global void
GetKeyboardMappingReply(buf)
unsigned char *buf;
{
    long    n;
    PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetKeyboardMapping */ ;
    if (CurrentVerbose < 2)
	return;

    PrintField(buf, 1, 1, DVALUE1(n), "keysyms-per-keycode");
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(n*m), "reply length");
    n = ILong(&buf[4]);
    PrintList(&buf[32], n, KEYSYM, "keysyms");
}

Global void
ChangeKeyboardControl(buf)
		unsigned char *buf;
{
	/* Request ChangeKeyboardControl is opcode 102 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ChangeKeyboardControl */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(2 + n), "request length");
	PrintField(buf, 4, 4, KEYBOARD_BITMASK, "value-mask");
	PrintValues(&buf[4], 4, KEYBOARD_BITMASK, &buf[8], "value-list");
}

Global void
GetKeyboardControl(buf)
		unsigned char *buf;
{
	/* Request GetKeyboardControl is opcode 103 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetKeyboardControl */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
GetKeyboardControlReply(buf)
		unsigned char *buf;
{
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetKeyboardControl */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, OFF_ON, "global-auto-repeat");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(5), "reply length");
	PrintField(buf, 8, 4, CARD32, "led-mask");
	PrintField(buf, 12, 1, CARD8, "key-click-percent");
	PrintField(buf, 13, 1, CARD8, "bell-percent");
	PrintField(buf, 14, 2, CARD16, "bell-pitch");
	PrintField(buf, 16, 2, CARD16, "bell-duration");
	PrintBytes(&buf[20], 32L, "auto-repeats");
}

Global void
Bell(buf)
		unsigned char *buf;
{
	/* Request Bell is opcode 104 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* Bell */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, INT8, "percent");
	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
ChangePointerControl(buf)
		unsigned char *buf;
{
	/* Request ChangePointerControl is opcode 105 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ChangePointerControl */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(3), "request length");
	PrintField(buf, 4, 2, INT16, "acceleration-numerator");
	PrintField(buf, 6, 2, INT16, "acceleration-denominator");
	PrintField(buf, 8, 2, INT16, "threshold");
	PrintField(buf, 10, 1, BOOL, "do-acceleration");
	PrintField(buf, 11, 1, BOOL, "do-threshold");
}

Global void
GetPointerControl(buf)
		unsigned char *buf;
{
	/* Request GetPointerControl is opcode 106 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetPointerControl */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
GetPointerControlReply(buf)
		unsigned char *buf;
{
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetPointerControl */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 2, CARD16, "acceleration-numerator");
	PrintField(buf, 10, 2, CARD16, "acceleration-denominator");
	PrintField(buf, 12, 2, CARD16, "threshold");
}

Global void
SetScreenSaver(buf)
		unsigned char *buf;
{
	/* Request SetScreenSaver is opcode 107 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* SetScreenSaver */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(3), "request length");
	PrintField(buf, 4, 2, INT16, "timeout");
	PrintField(buf, 6, 2, INT16, "interval");
	PrintField(buf, 8, 1, NO_YES, "prefer-blanking");
	PrintField(buf, 9, 1, NO_YES, "allow-exposures");
}

Global void
GetScreenSaver(buf)
		unsigned char *buf;
{
	/* Request GetScreenSaver is opcode 108 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetScreenSaver */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
GetScreenSaverReply(buf)
		unsigned char *buf;
{
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetScreenSaver */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
	PrintField(buf, 8, 2, CARD16, "timeout");
	PrintField(buf, 10, 2, CARD16, "interval");
	PrintField(buf, 12, 1, NO_YES, "prefer-blanking");
	PrintField(buf, 13, 1, NO_YES, "allow-exposures");
}

Global void
ChangeHosts(buf)
		unsigned char *buf;
{
	short   n;
	/* Request ChangeHosts is opcode 109 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ChangeHosts */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, INS_DEL, "mode");
	printfield(buf, 2, 2, DVALUE2(2 + (n + p) / 4), "request length");
	PrintField(buf, 4, 1, HOSTFAMILY, "family");
	printfield(buf, 6, 2, CARD16, "length of address");
	n = IShort(&buf[6]);
	PrintBytes(&buf[8], (long)n, "address");
}

Global void
ListHosts(buf)
		unsigned char *buf;
{
	/* Request ListHosts is opcode 110 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ListHosts */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
ListHostsReply(buf)
		unsigned char *buf;
{
	short   n;
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* ListHosts */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, DIS_EN, "mode");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4(n / 4), "reply length");
	printfield(buf, 8, 2, CARD16, "number of hosts");
	n = IShort(&buf[8]);
	PrintList(&buf[32], (long)n, HOST, "hosts");
}

Global void
SetAccessControl(buf)
		unsigned char *buf;
{
	/* Request SetAccessControl is opcode 111 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* SetAccessControl */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, DIS_EN, "mode");
	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
SetCloseDownMode(buf)
		unsigned char *buf;
{
	/* Request SetCloseDownMode is opcode 112 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* SetCloseDownMode */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, CLOSEMODE, "mode");
	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
KillClient(buf)
		unsigned char *buf;
{
	/* Request KillClient is opcode 113 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* KillClient */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(2), "request length");
	PrintField(buf, 4, 4, RESOURCEID, "resource");
}

Global void
RotateProperties(buf)
		unsigned char *buf;
{
	short   n;
#ifdef XMOVE
	n = IShort(&buf[8]);
	while (n--)
	  ISetLong(&buf[12 + n*4],
		   MapAtom(ILong(&buf[12 + n*4]), Request));
#endif
	/* Request RotateProperties is opcode 114 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* RotateProperties */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, DVALUE2(3 + n), "request length");
	PrintField(buf, 4, 4, WINDOW, "window");
	printfield(buf, 8, 2, DVALUE2(n), "number of properties");
	n = IShort(&buf[8]);
	PrintField(buf, 10, 2, INT16, "delta");
	PrintList(&buf[12], (long)n, ATOM, "properties");
}

Global void
ForceScreenSaver(buf)
		unsigned char *buf;
{
	/* Request ForceScreenSaver is opcode 115 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* ForceScreenSaver */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, SAVEMODE, "mode");
	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
SetPointerMapping(buf)
		unsigned char *buf;
{
	short   n;

	Dprintf(("XMOVE SetPointerMapping\n"))

	/* Request SetPointerMapping is opcode 116 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* SetPointerMapping */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 1, 1, DVALUE1(n), "length of map");
	n = IByte(&buf[1]);
	printfield(buf, 2, 2, DVALUE2(1 + (n + p) / 4), "request length");
	PrintBytes(&buf[4], (long)n,"map");
}

Global void
SetPointerMappingReply(buf)
		unsigned char *buf;
{
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* SetPointerMapping */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, RSTATUS, "status");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
}

Global void
GetPointerMapping(buf)
		unsigned char *buf;
{
	/* Request GetPointerMapping is opcode 117 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetPointerMapping */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
GetPointerMappingReply(buf)
		unsigned char *buf;
{
	short   n;
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetPointerMapping */ ;
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 1, 1, DVALUE1(n), "length of map");
	n = IByte(&buf[1]);
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, DVALUE4((n + p) / 4), "reply length");
	PrintBytes(&buf[32], (long)n,"map");
}

Global void
SetModifierMapping(buf)
		unsigned char *buf;
{
	short   n;
	/* Request SetModifierMapping is opcode 118 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* SetModifierMapping */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	PrintField(buf, 1, 1, DVALUE1(n), "keycodes-per-modifier");
	n = IByte(&buf[1]);
	printfield(buf, 2, 2, DVALUE2(1 + 2*n), "request length");
	PrintBytes(&buf[4 + 0 * n], (long)n,"Shift keycodes");
	PrintBytes(&buf[4 + 1 * n], (long)n,"Lock keycodes");
	PrintBytes(&buf[4 + 2 * n], (long)n,"Control keycodes");
	PrintBytes(&buf[4 + 3 * n], (long)n,"Mod1 keycodes");
	PrintBytes(&buf[4 + 4 * n], (long)n,"Mod2 keycodes");
	PrintBytes(&buf[4 + 5 * n], (long)n,"Mod3 keycodes");
	PrintBytes(&buf[4 + 6 * n], (long)n,"Mod4 keycodes");
	PrintBytes(&buf[4 + 7 * n], (long)n,"Mod5 keycodes");
}

Global void
SetModifierMappingReply(buf)
		unsigned char *buf;
{
	PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* SetModifierMapping */ ;
	if (CurrentVerbose < 2)
		return;
	PrintField(buf, 1, 1, RSTATUS, "status");
	printfield(buf, 2, 2, CARD16, "sequence number");
	printfield(buf, 4, 4, CONST4(0), "reply length");
}

Global void
GetModifierMapping(unsigned char *buf)
{
    /* Request GetModifierMapping is opcode 119 */
    PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* GetModifierMapping */ ;
    if (CurrentVerbose < 2)
	return;
    if (CurrentVerbose > 2)
	PrintField(SBf, 0, 4, INT32, "sequence number");
    
    printfield(buf, 2, 2, CONST2(1), "request length");
}

Global void
GetModifierMappingReply(unsigned char *buf)
{
    short   n;

    PrintField(RBf, 0, 1, REPLY, REPLYHEADER) /* GetModifierMapping */ ;
    if (CurrentVerbose < 2)
	return;

    PrintField(buf, 1, 1, DVALUE1(n), "keycodes-per-modifier");
    n = IByte(&buf[1]);
    printfield(buf, 2, 2, CARD16, "sequence number");
    printfield(buf, 4, 4, DVALUE4(2*n), "reply length");
    PrintList(&buf[32], (long)n, KEYCODE, "keycodes");
}

Global void
NoOperation(buf)
		unsigned char *buf;
{
	/* Request NoOperation is opcode 127 */
	PrintField(buf, 0, 1, REQUEST, REQUESTHEADER) /* NoOperation */ ;
	if (CurrentVerbose < 2)
		return;
	if (CurrentVerbose > 2)
		PrintField(SBf, 0, 4, INT32, "sequence number");

	printfield(buf, 2, 2, CONST2(1), "request length");
}


/* display setup reply information */



static void
PrintFailedSetUpReply(buf)
		unsigned char *buf;
{
	short   n;

	PrintField(buf, 0, 1, 0, "SetUp Failed");
	if (CurrentVerbose < 2)
		return;
	printfield(buf, 1, 1, DVALUE1(n), "length of reason in bytes");
	n = IByte(&buf[1]);
	PrintField(buf, 2, 2, CARD16, "major-version");
	PrintField(buf, 4, 2, CARD16, "minor-version");
	printfield(buf, 6, 2, DVALUE2((n + p) / 4), "length of data");
	PrintString8(&buf[8], n, "reason");
}

static void
PrintSuccessfulSetUpReply(unsigned char *buf)
{
    short   v = IShort(&buf[24]);
    short   n = IByte(&buf[29]);
    short   m = IByte(&buf[28]);

    Colormap default_cmap;
    ColormapPtr colormap;
    ColorCellPtr colorcell;
    
#ifdef XMOVE
    unsigned char *screen = buf + ROUNDUP4((long)v) + 40 + 8*n;
    
    /* we can only be in this section of the code when we are parsing */
    /* the reply from the *initial* server that we connected to, and */
    /* not any that we switched to, so all values taken here can be */
    /* assumed to be the first received by either side */

    Dprintf(("client base = 0x%x, mask = 0x%x\n",
	   ILong(&buf[12]),
	   ILong(&buf[16])));
    
    /* store the resource ID base and mask into the client and server */

    client->resource_base = server->resource_base = ILong(&buf[12]);
    client->resource_mask = server->resource_mask = ILong(&buf[16]);

    SetCurrentServerImageByteOrder(IByte(&buf[30]));
    SetCurrentServerImageBitmapBitOrder(IByte(&buf[31]));
    SetCurrentServerImageBitmapUnit(IByte(&buf[32]));
    SetCurrentServerImageBitmapPad(IByte(&buf[33]));

    SetCurrentClientRootWindow(ILong(screen));
    SetCurrentServerRootWindow(ILong(screen));

    default_cmap = ILong(screen + 4);
    SetCurrentServerDefaultColormap(default_cmap);
    SetCurrentClientDefaultColormap(default_cmap);
    SetCurrentServerDefaultVisualID(ILong(screen + 32));
    SetCurrentClientDefaultVisualID(ILong(screen + 32));

    /* add the default colormap to the client's list of colormaps, and allocate
       color cell entries for the while and black defaults. We assume that
       white is pure white and black is pure black, as will the client. If the
       client finds out that this isn't true, so will we */
    
    colormap = (ColormapPtr)Tcalloc(1, ColormapRec);
    colormap->colormap_id = default_cmap;
    colormap->visual_id = ILong(screen + 32);
    client->default_depth = server->default_depth = IByte(screen + 38);
    colormap->cell_count = (1 << IByte(screen + 38));
    colormap->cell_array = Tcalloc(colormap->cell_count, ColorCellRec);

    colorcell = AllocNewColorCell(ILong(screen + 8), colormap, False, 0xFFFF,
				  0xFFFF, 0xFFFF, NULL);
    colorcell->usage_count++;

    colorcell = AllocNewColorCell(ILong(screen + 12), colormap, False,
				   0, 0, 0, NULL);
    colorcell->usage_count++;

    AddColormapToCurrentClient(colormap);
    
    SaveDepths(server, screen);
    SaveFormats(server, buf + 40 + ROUNDUP4((long)v), IByte(buf+29));
    server->formats->flags = IMAGE_1BIT_SAFE | IMAGE_NBIT_SAFE;
    client->formats = server->formats;

    client->color = server->color;
    SetCurrentClientMinKeycode(IByte(&buf[34]));
    SetCurrentClientMaxKeycode(IByte(&buf[35]));

    SetCurrentServerMinKeycode(IByte(&buf[34]));
    SetCurrentServerMaxKeycode(IByte(&buf[35]));

    SetCurrentServerMaxReqLength(IShort(&buf[26]));
    
    /* in case this client is ever switched to a server with more lax */
    /* restrictions on the min/max keycode numbers, we have to set */
    /* them to the maximum values allowed */

    ISetByte(&buf[34], 0x0);
    ISetByte(&buf[35], 0xF0);

    /* we must change the server's response to deny having backing store
       and save-behinds. Pass pointer to first screen and #-of-screens. */
    
    AdjustSCREEN(&buf[ROUNDUP4((long)(40 + v) + 8 * n)], (long)m);

#endif

    if (CurrentVerbose < 2)
	return;
    PrintField(buf, 2, 2, CARD16, "protocol-major-version");
    PrintField(buf, 4, 2, CARD16, "protocol-minor-version");
    printfield(buf, 6, 2, DVALUE2(8 + 2*n + (v+p+m) / 4), "length of data");
    PrintField(buf, 8, 4, CARD32, "release-number");
    PrintField(buf, 12, 4, CARD32, "resource-id-base");
    PrintField(buf, 16, 4, CARD32, "resource-id-mask");
    PrintField(buf, 20, 4, CARD32, "motion-buffer-size");
    printfield(buf, 24, 2, DVALUE2(v), "length of vendor");
    printfield(buf, 26, 2, CARD16, "maximum-request-length");
    printfield(buf, 28, 1, CARD8, "number of roots");
    printfield(buf, 29, 1, DVALUE1(n), "number of pixmap-formats");
    PrintField(buf, 30, 1, BYTEORDER, "image-byte-order");
    PrintField(buf, 31, 1, BYTEORDER, "bitmap-format-bit-order");
    PrintField(buf, 32, 1, CARD8, "bitmap-format-scanline-unit");
    PrintField(buf, 33, 1, CARD8, "bitmap-format-scanline-pad");
    PrintField(buf, 34, 1, KEYCODE, "min-keycode");
    PrintField(buf, 35, 1, KEYCODE, "max-keycode");
    PrintString8(&buf[40], v, "vendor");
    PrintList(&buf[ROUNDUP4((long)(40 + v))], (long)n, FORMAT, "pixmap-formats");
    PrintList(&buf[ROUNDUP4((long)(40 + v) + 8 * n)], (long)m, SCREEN, "roots");
}

/* ProcessPutImage is in the unfortunate position of having to read in
   the XPutImage request from a client, remake the request in the form
   expected by the server, and then put that newly converted image to
   the server instead of the image sent by the client. */

void
ProcessPutImage(unsigned char *buf)
{
     char *data;
     ColormapPtr cmap;
     GCPtr gc;
     WindowPtr window;
     PixmapPtr pixmap;
     XImage *image, *new_image;
     unsigned int plane_mask;
     struct _XGC xgc;
     Bool converted_gc = False;
     
     int          format;
     unsigned int dst_x;
     unsigned int dst_y;
     unsigned int width;
     unsigned int height;
     unsigned int depth        = IByte(&buf[21]);
     
     Drawable     drawable     = ILong(&buf[4]);
     Drawable     map_drawable;
     
     GContext     gid          = ILong(&buf[8]);       
     GContext     map_gid      = MapGCID(gid, Request);
     
     Dprintf(("PutImage of format %d\n", format));

     /* with 1 bitplane image we have the problem that they can
	be used as stipples, for example, where 1 indicates the
	foreground color and 0 indicates the background color,
	rather than indicating pixel values in a colormap. To
	map these would be wrong. So we leave all 1 bitplane
	images' pixels unmapped. It is unlikely that anything
	bad will come of not mapping 1 bitplane images. */
     
     /* if it is a 1-bit image and the flag IMAGE_1BIT_SAFE is set
	just map the GC and Drawable and send the request through untouched. */

     if (depth == 1 && (client->formats->flags & IMAGE_1BIT_SAFE)) {
	  ISetLong(&buf[4], MapDrawable(drawable, Request));
	  ISetLong(&buf[8], map_gid);
	  return;
     }

     format = IByte(&buf[1]);
     width  = IShort(&buf[12]);
     height = IShort(&buf[14]);
     
     if ((pixmap = FindPixmapFromCurrentClient(drawable)) != NULL) {
	  cmap = pixmap->cmap;
	  map_drawable = MapPixmapID(drawable, Request);
     } else if ((window = FindWindowFromCurrentClient(drawable)) != NULL) {
	  cmap = window->cmap;
	  map_drawable = MapWindowID(drawable, Request);
     } else {
	  cmap = (ColormapPtr)client->colormap_list.top->contents;
	  map_drawable = MapDrawable(drawable, Request);
     }

     xgc.gid = map_gid;
     gc = FindGCFromCurrentClient(gid);

     if ((gc->cmap != cmap) && (format == XYBitmap)) {
	  Bool do_sync = True;
	  
	  /* flush the outgoing buffer. eg: client sends CreatePixmap followed by
	     PutImage. The CreatePixmap request could still be buffered and not yet
	     sent to the server. */

	  while (server->fdd->outBuffer.num_Saved)
	       SendXmoveBuffer(server->fdd->fd, &server->fdd->outBuffer);
	  
	  ConvertGCToColormap(gc, cmap, &do_sync);
	  if (do_sync == False)
	       converted_gc = True;
     }
     
     if ((depth != 1) && (depth == client->default_depth) &&
	 (client->formats->flags & IMAGE_NBIT_SAFE) && (format == ZPixmap))
     {
	  int bits_per_pixel = XMOVEGetBitsPerPixel(server->formats, depth);
	  int bpl = ROUNDUP(width * bits_per_pixel, XMOVEGetScanlinePad(server->formats, depth)) >> 3;
	  
	  ISetLong(&buf[4], map_drawable);
	  ISetLong(&buf[8], map_gid);
	  if (bits_per_pixel == 8)
	       MapImage8(buf+24, width, height, depth, bpl, cmap, Request);
	  else if (bits_per_pixel == 16)
	       MapImage16(buf+24, width, height, depth, bpl, cmap, Request,
			  (client->formats->image_byte_order != XmoveLittleEndian),
			  (server->formats->image_byte_order != XmoveLittleEndian));
	  else if (bits_per_pixel == 32)
	       MapImage32(buf+24, width, height, depth, bpl, cmap, Request,
			  (client->formats->image_byte_order != XmoveLittleEndian),
			  (server->formats->image_byte_order != XmoveLittleEndian));

	  if (!converted_gc)
	       return;

	  ignore_bytes = True;
	  SendBuffer(server->alt_fd, buf, IShort(&buf[2]) << 2);
	  server->alt_seqno++;
	  XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
	  return;
     }

     if (gc && (gc->values_mask & GCPlaneMask))
	  plane_mask = gc->values.plane_mask;
     else
	  plane_mask = 0xFFFFFFFF;
     
     image = XMOVEPreCreateImage(client->formats, NULL, format, NULL, plane_mask,
				 depth, (char *)buf+24, width, height);
     
     /* Let CreateImage figure out how many bytes per line. Don't
	allocate space for new_image's data till after it CreateImage
	does our work for us. */

     new_image = XMOVEPreCreateImage(server->formats, NULL, format, NULL, plane_mask,
				     (depth == 1 ? 1 : server->default_depth),
				     NULL, width, height);

     /* XYPixmap is stored in bitplanes, rather than chunky like ZPixmap.
	bytes_per_line is the number of bytes per line per plane, so multiply
	by the number of planes. */

     if (format == XYPixmap)
	  new_image->data = malloc(new_image->bytes_per_line * new_image->depth * height);
     else
	  new_image->data = malloc(new_image->bytes_per_line * height);

     if (depth == 1)
	  CopyImage(image, new_image, width, height);
     else
	  MoveImage(image, new_image, width, height, cmap, Request);

     dst_x = IShort(&buf[16]);
     dst_y = IShort(&buf[18]);

     /* Since XMOVEPutImage will increment SequenceNumber itself, undo the
	incrementing performed by DecodeRequest. */
     
     if (converted_gc) {
	  /* we don't need to call SendXmoveBuffer - already done when GC
	     was converted above. */
	  
	  XMOVEPutImage(server->alt_fd, &server->alt_seqno, server->formats,
			map_drawable, &xgc, new_image, 0, 0,
			dst_x, dst_y, width, height);
	  XMOVEGetInputFocus(server->alt_fd, &server->alt_seqno);
     } else {
	  while (server->fdd->outBuffer.num_Saved)
	       SendXmoveBuffer(server->fdd->fd, &server->fdd->outBuffer);
	  
	  --client->SequenceNumber;
	  
	  XMOVEPutImage(server->fdd->fd, &client->SequenceNumber, server->formats,
			map_drawable, &xgc, new_image, 0, 0,
			dst_x, dst_y, width, height);
     }
     
     free((char *)image);
     free(new_image->data);
     free((char *)new_image);
     
     ignore_bytes = True;
}

/* ProcessGetImage is in the unfortunate position of having to read in
   the XGetImageReply from the server, remake the request in the form
   expected by the client, and then put that newly converted image to
   the client instead of the image sent by the server. */

void
ProcessGetImage(unsigned char *buf)
{
     unsigned char *req = client->misc;
     char *data;
     ColormapPtr cmap;
     XImage *image, *new_image;
     unsigned int new_image_size;
     PixmapPtr pixmap;
     WindowPtr window;
     
     int          format       = IByte(&req[1]);
     Drawable     drawable     = ILong(&req[4]);
     unsigned int width        = IShort(&req[12]);
     unsigned int height       = IShort(&req[14]);
     unsigned int plane_mask   = ILong(&req[16]);
     
     unsigned int depth        = IByte(&buf[1]);
     
     Dprintf(("GetImage of format %d\n", format));
     
     free(client->misc);
     client->misc = NULL;
     
     ISetLong(&buf[8], MapVisualID(ILong(&buf[8]), Reply));

     /* with 1 bitplane image we have the problem that they can
	be used as stipples, for example, where 1 indicates the
	foreground color and 0 indicates the background color,
	rather than indicating pixel values in a colormap. To
	map these would be wrong. So we leave all 1 bitplane
	images' pixels unmapped. It is unlikely that anything
	bad will come of not mapping 1 bitplane images. */
     
     /* if it is a 1-bit image and the flag IMAGE_1BIT_SAFE is set
	just map the Visual and send the request through untouched. */

     if ((depth == 1) && (client->formats->flags & IMAGE_1BIT_SAFE))
	  return;

     if ((pixmap = FindPixmapFromCurrentClient(drawable)) != NULL)
	  cmap = pixmap->cmap;
     else if ((window = FindWindowFromCurrentClient(drawable)) != NULL)
	  cmap = window->cmap;
     else
	  cmap = (ColormapPtr)client->colormap_list.top->contents;

     if ((depth != 1) && (depth == client->default_depth) &&
	 (client->formats->flags & IMAGE_NBIT_SAFE) && (format == ZPixmap))
     {
	  int bits_per_pixel = XMOVEGetBitsPerPixel(client->formats, depth);
	  int bpl = ROUNDUP(width * bits_per_pixel, XMOVEGetScanlinePad(client->formats, depth)) >> 3;
	  
	  if (bits_per_pixel == 8)
	       MapImage8(buf+32, width, height, depth, bpl, cmap, Reply);
	  else if (bits_per_pixel == 16)
	       MapImage16(buf+32, width, height, depth, bpl, cmap, Reply,
			  (server->formats->image_byte_order != XmoveLittleEndian),
			  (client->formats->image_byte_order != XmoveLittleEndian));
	  else if (bits_per_pixel == 32)
	       MapImage32(buf+32, width, height, depth, bpl, cmap, Reply,
			  (server->formats->image_byte_order != XmoveLittleEndian),
			  (client->formats->image_byte_order != XmoveLittleEndian));

	  return;
     }

     image = XMOVEPreCreateImage(server->formats, NULL, format, NULL, plane_mask,
				 depth, (char *)buf+32, width, height);
     
     /* Let CreateImage figure out how many bytes per line. Don't
	allocate space for new_image's data till after it CreateImage
	does our work for us. */

     new_image = XMOVEPreCreateImage(client->formats, NULL, format, NULL, plane_mask,
				     (depth == 1 ? 1 : client->default_depth),
				     NULL, width, height);

     /* XYPixmap is stored in bitplanes, rather than chunky like ZPixmap.
	bytes_per_line is the number of bytes per line per plane, so multiply
	by the number of planes. */

     if (format == XYPixmap)
	  new_image_size = new_image->bytes_per_line * new_image->depth * height;
     else
	  new_image_size = new_image->bytes_per_line * height;
     
     new_image->data = malloc(new_image_size);

     if (depth == 1)
	  CopyImage(image, new_image, width, height);
     else
	  MoveImage(image, new_image, width, height, cmap, Reply);

     ISetByte(&buf[1], image->depth);
     ISetLong(&buf[4], new_image_size/4);

     SaveBytes(&client->fdd->outBuffer, buf, 32);
     SaveBytes(&client->fdd->outBuffer, (unsigned char *)new_image->data,
	       new_image_size);

     free(new_image->data);
     free((char *)image);
     free((char *)new_image);

     ignore_bytes = True;
}

FixGCnPixColormaps(GContext gcid, Drawable did)
{
     GCPtr gc;
     union {WindowPtr win; PixmapPtr pix;} dst;
     Bool do_sync;
     
     if (client->allocated_colormaps)
	  if (gc = FindGCFromCurrentClient(gcid)) {
	       do_sync = True;
	       
	       if (dst.win = FindWindowFromCurrentClient(did)) {
		    if (gc->cmap != dst.win->cmap) {
			 ConvertGCToColormap(gc, dst.win->cmap, &do_sync);
		    }
	       } else if (dst.pix = FindPixmapFromCurrentClient(did)) {
		    if (gc->cmap != dst.pix->cmap) {
			 ConvertGCToColormap(gc, dst.pix->cmap, &do_sync);
		    }
	       }
	       
	       if (!do_sync)
		    ignore_bytes = True;
	  }
}
