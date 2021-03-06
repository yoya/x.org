/*
 * Copyright (c) 1994 Paul Vojta.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * NOTES:
 *	This code was originally written by Ricardo Telichevesky
 *	(ricardo@rle-vlsi-mit.edu) and Luis Miguel Silveira
 *	(lms@rle-vlsi-mit.edu).
 *	It was largely influenced by similar code in the SeeTeX/XTeX
 *	package by Dirk Grunwald (grunwald@colorado.edu).
 */

/* ||| To do:
 *	ALWAYS_CLOSE_SERVER_CONNECTION?
 *	Is there some way of interrupting a process?
 *	fork
 *	extra bytes on input
 */

#include "xdvi.h"
#include <memory.h>
#include <signal.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <NeWS/psio.h>
#include <xvps/pscanvas.h>

/* if POSIX O_NONBLOCK is not available, use O_NDELAY */
#if !defined(O_NONBLOCK) && defined(O_NDELAY)
#define	O_NONBLOCK O_NDELAY
#endif

/* Condition for retrying a write */
#include <errno.h>
#ifdef	EWOULDBLOCK
#ifdef	EAGAIN
#define	AGAIN_CONDITION	(errno == EWOULDBLOCK || errno == EAGAIN)
#else	/* EAGAIN */
#define	AGAIN_CONDITION	(errno == EWOULDBLOCK)
#endif	/* EAGAIN */
#else	/* EWOULDBLOCK */
#ifdef	EAGAIN
#define	AGAIN_CONDITION	(errno == EAGAIN)
#endif	/* EAGAIN */
#endif	/* EWOULDBLOCK */

#ifdef	STREAMSCONN
#include <poll.h>
#endif

#if	HAS_SIGIO
#include <fcntl.h>
#include <signal.h>
#ifndef	FASYNC
#undef	HAS_SIGIO
#define	HAS_SIGIO 0
#endif
#endif

#if	NeedFunctionPrototypes
char	*strtok(char *, char *);
#else
char	*strtok();
#endif

#define	Fprintf	(void) fprintf


/* define ALWAYS_CLOSE_SERVER_CONNECTION if you want to close the server
   connection all the time */
#undef	ALWAYS_CLOSE_SERVER_CONNECTION


		/*
		 * Some setup code.
		 */
static	_Xconst	char	str0[]	= "\
/OW2? version cvi 2 eq def \
OW2? \
{ /setlinewidth { pop } def} \
{ /NeWS 3 0 findpackage beginpackage \
  /X11 3 0 findpackage beginpackage} \
ifelse \
currentcanvas /Color get \
currentcanvas /Colormap get getcubedescription null eq and \
   {8 {{currentcanvas /Colormap get 1 index dup dup dup newcube} stopped \
    {pop pop pop pop pop} {exit} ifelse \
    2 div cvi dup 1 eq {exit} if} loop pop} \
if\n";
		/*
		 * This string reads chunks (delimited by %%xdvimark).
		 * The first character of a chunk tells whether a given chunk
		 * is to be done within save/restore or not.
		 * The `H' at the end tells it that the first group is a
		 * header; i.e., no save/restore.
		 */
static	_Xconst	char	preamble[]	= "\
/xdvi$line 81 string def \
/xdvi$run {$error null ne {$error /newerror false put} if \
 {currentfile cvx stopped \
 $error null eq {false} {$error /newerror get} ifelse and \
 {handleerror} if} stopped pop} def \
/xdvi$dslen countdictstack def \
{currentfile read not {exit} if 72 eq \
    {xdvi$run} \
    {/xdvi$sav save def xdvi$run \
      clear countdictstack xdvi$dslen sub {end} repeat xdvi$sav restore} \
  ifelse \
  {(%%xdvimark) currentfile xdvi$line {readline} stopped \
    {clear} {{eq {false exit} if} {true exit} ifelse} ifelse }loop {exit} if \
  58 tagprint flush \
}loop\nH";

extern	_Xconst	char	psheader[];
extern	int	psheaderlen;

static	_Xconst	char	preamble2[]	= " stop\n%%xdvimark\n";
#define	stopstring	preamble2

#define	postscript	resource._postscript


/* global procedures (besides initNeWS) */

static	void	toggleNeWS ARGS((void));
static	void	destroyNeWS ARGS((void));
static	void	interruptNeWS ARGS((void));
static	void	endpageNeWS ARGS((void));
static	void	drawbeginNeWS ARGS((Drawable, int, int, int, int, double,
				char *));
static	void	drawrawNeWS ARGS((char *));
static	void	drawfileNeWS ARGS((char *));
static	void	drawendNeWS ARGS((char *));

static	struct psprocs	news_procs = {
	/* toggle */		toggleNeWS,
	/* destroy */		destroyNeWS,
	/* interrupt */		interruptNeWS,
	/* endpage */		endpageNeWS,
	/* drawbegin */		drawbeginNeWS,
	/* drawraw */		drawrawNeWS,
	/* drawfile */		drawfileNeWS,
	/* drawend */		drawendNeWS};

/* signal handler to hairy PostScript code */
static void psio_sigpipe_handler();

/* define local static variables */
static	int	NeWS_mag;		/* magnification currently in use */
static	int	NeWS_shrink;		/* shrink factor currently in use */
static	Boolean	NeWS_active;		/* if we've started a page yet */
static	int	NeWS_pending;		/* number of ack's we're expecting */
static	int	NeWS_sending;		/* level of nesting in send() */
static	Boolean	NeWS_pending_int;	/* if interrupt rec'd while in send() */
static	Boolean	NeWS_destroyed	= False;


/*
 *	NeWS I/O code.  This should send PS code to NeWS,
 *	receive acknowledgements, and receive X events in the meantime.
 *	It also checks for SIGPIPE errors.
 */

#ifndef	STREAMSCONN
static	int		numfds;
static	fd_set		readfds;
static	fd_set		writefds;
#define	XDVI_ISSET(a, b, c)	FD_ISSET(a, b)
#else	/* STREAMSCONN */
struct pollfd		fds[3] = {{0, POLLOUT, 0},
				  {0, POLLIN, 0},
				  {0, POLLIN, 0}};
#define	XDVI_ISSET(a, b, c)	(fds[c].revents)
#endif	/* STREAMSCONN */


/*---------------------------------------------------------------------------*
  psio_sigpipe_handler  ()

  Arguments: sig, code, scp, addr (see man page for signal)
  Returns: (void)
  Side-Effects: SIGPIPE signal is flagged as sigpipe_error variable is set.

  Description:
  Handler for SIGPIPE error generated by a broken pipe in the connection
  to the NeWS server; this may be duer to some abnormal condition, or some
  hairy PostScript code containing commands not implemented by the server.

+----------------------------------------------------------------------------*/

static	Boolean	sigpipe_error = False;

static	struct sigaction psio_sigpipe_handler_struct;
	/* initialized to {psio_sigpipe_handler, (sigset_t) 0, 0} in initNeWS */

/* ARGSUSED */
static	void
psio_sigpipe_handler(sig, code, scp, addr)
	int		sig;
	int		code;
	struct sigcontext *scp;
	char		*addr;
{
	sigpipe_error = True;
}


/*
 *	read_from_NeWS - This does the actual retrieving of acknowledgements.
 *	If other bytes appear on the file - tough.
 */

static	void
read_from_NeWS()
{
	for (;;) {
	    int retval;

	    retval = ps_checkfor(PostScriptInput, PSIO_FIND_TAG, 58);
	    if (retval == 0) break;
	    if (retval < 0) {
		Fprintf(stderr, "ps_checkfor: %d\n", retval);
		return;
	    }
	    (void) ps_checkfor(PostScriptInput, PSIO_WAIT_TAG, 58);
	    --NeWS_pending;
	    if (debug & DBG_PS)
		Printf("Got NeWS ack; %d pending.\n", NeWS_pending);
	}
}


/*
 *	This actually sends the bytes to NeWS.
 */

static	void
send(cp, len)
	_Xconst	char	*cp;
	int		len;
{
	struct sigaction orig;
#ifdef	STREAMSCONN
	int	retval;
#endif

	if (PostScript == (PSFILE *) NULL) return;

	if (!NeWS_sending) {
	    (void) sigaction(SIGPIPE, &psio_sigpipe_handler_struct, &orig);
	    sigpipe_error = False;
	}
	++NeWS_sending;

#if	HAS_SIGIO
	(void) fcntl(ConnectionNumber(DISP), F_SETFL,
	    fcntl(ConnectionNumber(DISP), F_GETFL, 0) & ~FASYNC);
#endif

#ifndef	STREAMSCONN
	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
#endif

	for (;;) {

#ifndef	STREAMSCONN
	    FD_SET(ConnectionNumber(DISP), &readfds);
	    FD_SET(PostScript->file, &writefds);
	    FD_SET(PostScriptInput->file, &readfds);

	    if (select(numfds, &readfds, &writefds, (fd_set *) NULL,
		    (struct timeval *) NULL) < 0 && errno != EINTR) {
		perror("select (NeWS_send)");
		break;
	    }
#else	/* STREAMSCONN */
	    for (;;) {
		retval = poll(fds, XtNumber(fds), -1);
		if (retval >= 0 || errno != EAGAIN) break;
	    }
	    if (retval < 0) {
		perror("poll (NeWS_send)");
		break;
	    }
#endif	/* STREAMSCONN */

	    if (XDVI_ISSET(PostScriptInput->file, &readfds, 1))
		read_from_NeWS();
	    if (XDVI_ISSET(PostScript->file, &writefds, 0)) {
		int	old_flags;
		int	bytes;

		old_flags = fcntl(PostScript->file, F_GETFL, 0);
		if (old_flags < 0) break;
		/* set to be non-blocking */
		if (fcntl(PostScript->file, F_SETFL, old_flags | O_NONBLOCK)
			< 0)
		    break;
		bytes = write(PostScript->file, cp, len);
		if (bytes == -1) {
		    if (!AGAIN_CONDITION) perror("gs_send");
		}
		else {
		    cp += bytes;
		    len -= bytes;
		}
		if (fcntl(PostScript->file, F_SETFL, old_flags) < 0) break;
		if (len == 0 || sigpipe_error) break;
	    }
	    if (XDVI_ISSET(ConnectionNumber(DISP), &readfds, 2)) {
		allow_can = False;
		read_events(False);
		allow_can = True;
		if (PostScript == (PSFILE *) NULL) break;	/* if timeout occurred */
	    }
	}

#if	HAS_SIGIO
	(void) fcntl(ConnectionNumber(DISP), F_SETFL,
	    fcntl(ConnectionNumber(DISP), F_GETFL, 0) | FASYNC);
#endif

	if (--NeWS_sending == 0) {
	    /* put back generic handler for SIGPIPE */
	    (void) sigaction(SIGPIPE, &orig, (struct sigaction *) NULL);

	    if (sigpipe_error) {
		Fputs("NeWS died unexpectedly.\n", stderr);
		destroyNeWS();
		draw_bbox();
	    }

	    if (NeWS_pending_int) {
		NeWS_pending_int = False;
		interruptNeWS();
	    }
	}
}

/*
 *	Wait for acknowledgement from NeWS.  With NeWS we have no choice but
 *	to wait (||| I think).
 */

static	void
waitack()
{
#ifdef	STREAMSCONN
	int	retval;
#endif
#if	HAS_SIGIO
	int	oldflags;
#endif

	if (PostScript == (PSFILE *) NULL) return;

#if	HAS_SIGIO
	oldflags = fcntl(ConnectionNumber(DISP), F_GETFL, 0);
	(void) fcntl(ConnectionNumber(DISP), F_SETFL, oldflags & ~FASYNC);
#endif
#ifndef	STREAMSCONN
	FD_ZERO(&readfds);
#endif
	while (NeWS_pending > 0) {
#ifndef	STREAMSCONN
	    FD_SET(ConnectionNumber(DISP), &readfds);
	    FD_SET(PostScriptInput->file, &readfds);
	    if (select(numfds, &readfds, (fd_set *) NULL, (fd_set *) NULL,
		    (struct timeval *) NULL) < 0 && errno != EINTR) {
		perror("select (gs_waitack)");
		break;
	    }
#else	/* STREAMSCONN */
	    for (;;) {
		retval = poll(fds + 1, XtNumber(fds) - 1, -1);
		if (retval >= 0 || errno != EAGAIN) break;
	    }
	    if (retval < 0) {
		perror("poll (gs_waitack)");
		break;
	    }
#endif	/* STREAMSCONN */


	    if (XDVI_ISSET(PostScriptInput->file, &readfds, 1))
		read_from_NeWS();
	    if (XDVI_ISSET(ConnectionNumber(DISP), &readfds, 2)) {
		allow_can = False;
		read_events(False);
		allow_can = True;
		if (PostScript == (PSFILE *) NULL) break;	/* if timeout occurred */
	    }
	}
#if	HAS_SIGIO
	(void) fcntl(ConnectionNumber(DISP), F_SETFL, oldflags);
#endif
}


/*---------------------------------------------------------------------------*
  initNeWS()

  Arguments: None.
  Returns: True if and only if initialization succeeded
  Side-Effects: Static variables may be set.

  Description:
  Initializes variables for the application main loop.

+----------------------------------------------------------------------------*/

Boolean
initNeWS()
{
	static	NeWStoken newstoken;

	/* now try to open the connection to the NeWS server */
	if (ps_open_PostScript() == (PSFILE *) NULL)
	    return False;

#ifndef	STREAMSCONN
	numfds = ConnectionNumber(DISP);
	if (numfds < PostScript->file) numfds = PostScript->file;
	if (numfds < PostScriptInput->file) numfds = PostScriptInput->file;
	++numfds;
#else	/* STREAMSCONN */
	fds[0].fd = PostScript->file;
	fds[1].fd = PostScriptInput->file;
	fds[2].fd = ConnectionNumber(DISP);
#endif	/* STREAMSCONN */

	psio_sigpipe_handler_struct.sa_handler = psio_sigpipe_handler;
	sigemptyset(&psio_sigpipe_handler_struct.sa_mask);

	NeWS_active = NeWS_pending_int = False;
	NeWS_sending = 0;
	NeWS_pending = 1;

	ps_flush_PostScript();
	send(str0, sizeof(str0) - 1);
	/* get xid of window, then make this window the NeWS canvas */
	(void) ps_token_from_xid(mane.win, &newstoken);
	if (newstoken != -1) {
	    ps_setcanvas(newstoken);
	    ps_flush_PostScript();
	    send(preamble, sizeof(preamble) - 1);
	    send(psheader, psheaderlen);
	    send(preamble2, sizeof(preamble2) - 1);
	}

	if (NeWS_destroyed) return False;

	/* success */

	NeWS_mag = NeWS_shrink = -1;

	psp = news_procs;
	if (!postscript) toggleNeWS();	/* if we got a 'v' already */

	return True;
}


/*---------------------------------------------------------------------------*
  toggleNeWS()

  Arguments: none
  Returns: (void)
  Side-Effects: psp.drawbegin is changed

  Description:
  Used to toggle the rendering of PostScript by the NeWS server

+----------------------------------------------------------------------------*/

static	void
toggleNeWS()
{
	if (postscript) psp.drawbegin = drawbeginNeWS;
	else {
	    interruptNeWS();
	    psp.drawbegin = drawbegin_none;
	}
}


/*---------------------------------------------------------------------------*
  destroyNeWS()

  Arguments: none
  Returns: (void)
  Side-Effects: the pointer to the NeWS file is nulled

  Description:
  Close the connection to the NeWS server; used when rendering is terminated
  in any way.

+----------------------------------------------------------------------------*/

static	void
destroyNeWS()
{
	psp = no_ps_procs;
	NeWS_destroyed = True;
}


/*---------------------------------------------------------------------------*
  interruptNeWS()

  Arguments: none
  Returns: void

  Description:
  Close the connection to the NeWS server; used when rendering is terminated
  because of an interruption in the viewing of the current page.
  ||| It would be nice if we could asynchronously ``wake up'' a NeWS process
  (preferably by sending something along the X socket); then we could do
  better than just to wait.

+----------------------------------------------------------------------------*/

static	void
interruptNeWS()
{
	if (debug & DBG_PS) Puts("Running interruptNeWS()");
	if (NeWS_sending) NeWS_pending_int = True;
	else {
	    if (NeWS_active) {
		send(stopstring, sizeof(stopstring) - 1);
		NeWS_active = False;
	    }
	    psp.interrupt = NullProc;	/* prevent deep recursion in waitack */
	    waitack();
	    psp.interrupt = interruptNeWS;
	}
}


/*---------------------------------------------------------------------------*
  endpageNeWS()

  Arguments: none
  Returns: (void)
  Side-Effects: the NeWS_active variable is cleared.

  Description:
  Should be called at the end of a page to end this chunk for the NeWS server.

+----------------------------------------------------------------------------*/

static	void
endpageNeWS()
{
	if (debug & DBG_PS)
	    Puts("endpage sent to NeWS Server");
	if (NeWS_active) {
	    send(stopstring, sizeof(stopstring) - 1);
	    NeWS_active = False;
	    waitack();
	}
}


/*---------------------------------------------------------------------------*
  drawbeginNeWS  ()

  Arguments: xul, yul - coordinates of the upper left corner of the figure
	     cp - string with the bounding box line data
  Returns: (void)

  Description:
  Opens a connection to the NeWS server and send in the preamble and the
  bounding box information after correctly computing resolution factors.
  In case no rendering is to be done, outlines the figure.  An outline is
  also generated whenever the PostScript code is too hairy and generates
  a SIGPIPE signal.

+----------------------------------------------------------------------------*/

static	void
drawbeginNeWS(xul, yul, cp)
	int		xul, yul;
	char		*cp;
{
	char	buf[100];
	static	_Xconst	char	str[]	= " TeXDict begin\n";

	if (debug & DBG_PS) {
	    Printf("xul= %d yul= %d\n", xul, yul);
	    Printf("String = < %s >\n", cp);
	}

	/* catch up on the X side */
	XSync(DISP, 0);

	if (!NeWS_active) {
	    /* send initialization to NeWS server */
	    if (magnification != NeWS_mag) {
		Sprintf(buf, "H TeXDict begin /DVImag %d 1000 div def \
end stop\n%%%%xdvimark\n",
		    NeWS_mag = magnification);
		send(buf, strlen(buf));
		++NeWS_pending;
	    }
	    if (mane.shrinkfactor != NeWS_shrink) {
		Sprintf(buf, "H TeXDict begin %d %d div dup \
/Resolution X /VResolution X \
end stop\n%%%%xdvimark\n",
		    pixels_per_inch, NeWS_shrink = mane.shrinkfactor);
		send(buf, strlen(buf));
		++NeWS_pending;
	    }
	    send(str, sizeof(str) - 1);
	    NeWS_active = True;
	    ++NeWS_pending;
	}

	Sprintf(buf, "%d %d moveto\n", xul, yul);
	send(buf, strlen(buf));
	send(cp, strlen(cp));
}


/*---------------------------------------------------------------------------*
  drawrawNeWS()

  Arguments: origcp - the raw string to be sent to the postscript interpreter
  Returns: (void)
  Side-Effects: (none)

  Description:
  If there is a valid connection to the NeWS server, just send the string to
  the interpreter, else leave.

+----------------------------------------------------------------------------*/

static	void
drawrawNeWS(origcp)
	char	*origcp;
{
	char		*pt, *ptm1, *cp1, *ocp1;
	static	char	*cp;
	static	unsigned int cplen = 0;
	unsigned int	len;
	double		angle;
	Boolean		found	= False;

	if (!NeWS_active)
	    return;

	if (debug & DBG_PS)
	    Printf("Raw PS sent to context: <%s>\n", origcp);

	/* take a look at the string:  NeWS bums on certain rotations */
	len = strlen(origcp) + 4;
	if (cplen < len) {
	    if (cplen != 0) free(cp);
	    cplen = len;
	    cp = xmalloc(cplen, "string in drawrawNeWS");
	}
	ocp1 = origcp;
	pt = origcp;
	while (*pt == ' ' || *pt == '\t') ++pt;
	cp1 = cp;
	for (;;) {
	    ptm1 = pt;
	    while (*pt != '\0' && *pt != ' ' && *pt != '\t') ++pt;
	    if (*pt == '\0') break;
	    while (*pt == ' ' || *pt == '\t') ++pt;
	    if (strncmp(pt, "rotate", 6) == 0
		    && (pt[6] == '\0' || pt[6] == ' ' || pt[6] == '\t')) {
		/* found rotate; check angle */
		if (sscanf(ptm1, "%lf", &angle) >= 1) {
		    found = True;
		    while (angle > 360.0)
			angle -= 360;
		    while (angle < -360.0)
			angle += 360;
		    if (angle == 90.0) {
			angle = 89.999;
			(void) memcpy(cp1, ocp1, ptm1 - ocp1);
			cp1 += ptm1 - ocp1;
			Strcpy(cp1, "89.999 rotate ");
			cp1 += strlen(cp1);
			while (*pt != '\0' && *pt != ' ' && *pt != '\t') ++pt;
			while (*pt == ' ' || *pt == '\t') ++pt;
			ocp1 = pt;
		    } else if (angle == -90.0) {
			angle = -89.999;
			(void) memcpy(cp1, ocp1, ptm1 - ocp1);
			cp1 += ptm1 - ocp1;
			Strcpy(cp1, "-89.999 rotate ");
			cp1 += strlen(cp1);
			while (*pt != '\0' && *pt != ' ' && *pt != '\t') ++pt;
			while (*pt == ' ' || *pt == '\t') ++pt;
			ocp1 = pt;
		    } else if (angle == 0.0) {
			(void) memcpy(cp1, ocp1, ptm1 - ocp1);
			cp1 += ptm1 - ocp1;
			while (*pt != '\0' && *pt != ' ' && *pt != '\t') ++pt;
			while (*pt == ' ' || *pt == '\t') ++pt;
			ocp1 = pt;
		    }
		}
	    }
	}
	Strcpy(cp1, ocp1);
	if ((debug & DBG_PS) && found) {
	    Printf("String is now <%s>\n", cp);
	    Printf("Found rotate string.  Angle is %g degrees.\n", angle);
	}

	len = strlen(cp);
	cp[len] = '\n';
	send(cp, len + 1);
}


/*---------------------------------------------------------------------------*
  drawfileNeWS()

  Arguments: cp - string with the postscript file pathname
  Returns: (void)
  Side-Effects: none

  Description:
  Postscript file containing the figure is opened and sent to the NeWS server.
  Figure is outlined in case hairy code produces a SIGPIPE signal.

+----------------------------------------------------------------------------*/

static	void
drawfileNeWS(cp)
	char	*cp;
{
	char		buffer[1025];
	int		blen;
	int		bytes;
	FILE		*psfile;
	struct sigaction orig;

	if (!NeWS_active)
	    return;

#ifndef	VMS
	if ((psfile = xfopen(cp, "r")) == NULL)
#else
	if ((psfile = xfopen(cp, "r", "?")) == NULL)
#endif
	{
	    Fprintf(stderr,"[%%NeWS Server: cannot access file %s%%]\n", cp);
	    draw_bbox();
	} else {
	    if (debug & DBG_PS)
		Printf("printing file %s\n", cp);
	    /* some hairy PS code generates SIGPIPE signals; handle them */
	    (void) sigaction(SIGPIPE, &psio_sigpipe_handler_struct, &orig);
	    sigpipe_error = False;
	    NeWS_sending = 1;
	    if (!sigpipe_error)
		for (;;) {
		    blen = fread(buffer, sizeof(char), 1024, psfile);
		    if (blen == 0) break;
		    send(buffer, blen);
		    if (sigpipe_error) break;
		}
	    (void) fclose(psfile);

	    --NeWS_sending;
	    /* put back generic handler for SIGPIPE */
	    (void) sigaction(SIGPIPE, &orig, (struct sigaction *) NULL);

	    if (sigpipe_error) {
		Fputs("NeWS died unexpectedly.\n", stderr);
		destroyNeWS();
		draw_bbox();
	    }

	    if (NeWS_pending_int) {
		NeWS_pending_int = False;
		interruptNeWS();
	    }
	}
}


/*---------------------------------------------------------------------------*
  drawendNeWS()

  Arguments: cp - string with indication of the end of the special
  Returns: (void)

  Description:
  Sends the indication of end of the figure PostScript code.

+----------------------------------------------------------------------------*/

static	void
drawendNeWS(cp)
	char	*cp;
{
	if (!NeWS_active)
	    return;

	if (debug & DBG_PS)
	    Puts("drawend sent to NeWS Server");
	send(cp, strlen(cp));
	send("\n", 1);
}
