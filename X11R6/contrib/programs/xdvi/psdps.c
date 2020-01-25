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

#include "xdvi.h"
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/time.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <DPS/XDPSlib.h>
#include <DPS/dpsXclient.h>
#include <DPS/dpsexcept.h>
#include <DPS/dpsclient.h>

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


		/*
		 * This string reads chunks (delimited by %%xdvimark).
		 * The first character of a chunk tells whether a given chunk
		 * is to be done within save/restore or not.
		 * The `H' at the end tells it that the first group is a
		 * header; i.e., no save/restore.
		 */
static	char	preamble[]	= "\
/xdvi$line 81 string def \
/xdvi$run {{$error null ne {$error /newerror false put}if \
 currentfile cvx stopped \
 $error null eq {false} {$error /newerror get} ifelse and \
 {handleerror} if} stopped pop} def \
/xdvi$dslen countdictstack def \
{currentfile read not {exit} if 72 eq \
  {xdvi$run} \
  {/xdvi$sav save def xdvi$run \
   clear countdictstack xdvi$dslen sub {end} repeat xdvi$sav restore} \
 ifelse \
 {(%%xdvimark) currentfile xdvi$line {readline} stopped \
  {clear} {pop eq {exit} if} ifelse }loop \
 (xdvi$Ack\n) print flush \
}loop\nH";

extern	char	psheader[];
extern	int	psheaderlen;

#define	postscript	resource._postscript


/* global procedures (besides initDPS) */

static	void	toggleDPS ARGS((void));
static	void	destroyDPS ARGS((void));
static	void	interruptDPS ARGS((void));
static	void	endpageDPS ARGS((void));
static	void	drawbeginDPS ARGS((int, int, char *));
static	void	drawrawDPS ARGS((char *));
static	void	drawfileDPS ARGS((char *));
static	void	drawendDPS ARGS((char *));

static	struct psprocs	dps_procs = {
	/* toggle */		toggleDPS,
	/* destroy */		destroyDPS,
	/* interrupt */		interruptDPS,
	/* endpage */		endpageDPS,
	/* drawbegin */		drawbeginDPS,
	/* drawraw */		drawrawDPS,
	/* drawfile */		drawfileDPS,
	/* drawend */		drawendDPS};

static	DPSContext DPS_ctx = NULL;
static	DPSSpace DPS_space = NULL;
static	int	DPS_mag;		/* magnification currently in use */
static	int	DPS_shrink;		/* shrink factor currently in use */
static	Boolean	DPS_active;		/* if we've started a page */
static	int	DPS_pending;		/* number of ack's we're expecting */


#if	0
static	void	DPSErrorProcHandler();
#else
#define	DPSErrorProcHandler	DPSDefaultErrorProc
#endif


static	char	ackstr[]	= "xdvi$Ack\n";

#define	LINELEN	21
#define	BUFLEN	(LINELEN + sizeof(ackstr))
static	char	line[BUFLEN + 1];
static	char	*linepos	= line;

static	void
TextProc(ctxt, buf, count)
	DPSContext	ctxt;
	char		*buf;
	unsigned long	count;
{
	int	i;
	char	*p;
	char	*p0;

	while (count > 0) {
	    i = line + BUFLEN - linepos;
	    if (i > count) i = count;
	    (void) bcopy(buf, linepos, i);
	    linepos += i;
	    buf += i;
	    count -= i;
	    p0 = line;
	    for (;;) {
		if (p0 >= linepos) {
		    linepos = line;
		    break;
		}
		p = memchr(p0, '\n', linepos - p0);
		if (p == NULL) {
		    if (p0 != line) {
			(void) bcopy(p0, line, linepos - p0);
			linepos -= p0 - line;
		    }
		    else if (linepos == line + BUFLEN) {
			char	c;
			
			c = line[LINELEN];
			line[LINELEN] = '\0';
			Printf("DPS: %s\n", line);
			line[LINELEN] = c;
			linepos -= LINELEN;
			(void) bcopy(line + LINELEN, line, linepos - line);
		    }
		    break;
		}
		if (p >= p0 + 8 && memcmp(p - 8, ackstr, 9) == 0) {
		    --DPS_pending;
		    if (debug & DBG_PS)
			Printf("Got DPS ack; %d pending.\n", DPS_pending);
		    ++p;
		    (void) bcopy(p, p - 9, linepos - p);
		    linepos -= 9;
		    continue;
		}
		*p = '\0';
		Printf("DPS: %s\n", p0);
		p0 = p + 1;
	    }
	}
}


/*---------------------------------------------------------------------------*
  waitack()

  Arguments: none.

  Returns: (void)

  Description:
  Waits until the requisite number of acknowledgements has been received from
  the context.

+----------------------------------------------------------------------------*/

#ifndef	STREAMSCONN
static	fd_set		readfds;
#define	XDVI_ISSET(a, b, c)	FD_ISSET(a, b)
#else	/* STREAMSCONN */
struct pollfd		fds[1] = {{0, POLLIN, 0}};
#define	XDVI_ISSET(a, b, c)	(fds[c].revents)
#endif	/* STREAMSCONN */

static	void
waitack()
{
#ifdef	STREAMSCONN
	int		retval;
#endif
#if	HAS_SIGIO
	int		oldflags;
#endif

#if	HAS_SIGIO
	oldflags = fcntl(ConnectionNumber(DISP), F_GETFL, 0);
	(void) fcntl(ConnectionNumber(DISP), F_SETFL, oldflags & ~FASYNC);
#endif
#ifndef	STREAMSCONN
	FD_ZERO(&readfds);
#endif
	while (DPS_pending > 0) {
#ifndef	STREAMSCONN
	    FD_SET(ConnectionNumber(DISP), &readfds);
	    if (select(ConnectionNumber(DISP) + 1, &readfds, (fd_set *) NULL,
		    (fd_set *) NULL, (struct timeval *) NULL) < 0
		    && errno != EINTR) {
		perror("select (DPS_waitack)");
		break;
	    }
#else	/* STREAMSCONN */
	    for (;;) {
		retval = poll(fds, XtNumber(fds), -1);
		if (retval >= 0 || errno != EAGAIN) break;
	    }
	    if (retval < 0) {
		perror("poll (DPS_waitack)");
		break;
	    }
#endif	/* STREAMSCONN */

	    if (XDVI_ISSET(ConnectionNumber(DISP), &readfds, 0)) {
		allow_can = False;
		read_events(False);
		allow_can = True;
		if (DPS_ctx == NULL) break;	/* if interrupt occurred */
	    }
	}
#if	HAS_SIGIO
	(void) fcntl(ConnectionNumber(DISP), F_SETFL, oldflags);
#endif
}


/*---------------------------------------------------------------------------*
  initDPS()

  Arguments: (none)
  Returns: (void)
  Side-Effects: DPS_ctx may be set as well as other static variables.

  Description:
  Initializes variables from the application main loop.  Checks to see if
  a connection to the DPS server can be opened.

+----------------------------------------------------------------------------*/

Boolean
initDPS()
{
  /* now try to create a context */
  DPS_ctx = XDPSCreateSimpleContext(DISP, mane.win, ruleGC, 0, 0,
				    TextProc, DPSDefaultErrorProc, NULL);
  if (DPS_ctx == NULL)
    return False;

  DPS_space = DPSSpaceFromContext(DPS_ctx);
  DPSWritePostScript(DPS_ctx, preamble, sizeof(preamble) - 1);
  DPSWritePostScript(DPS_ctx, psheader, psheaderlen);
  DPSPrintf(DPS_ctx, "matrix setmatrix stop\n%%%%xdvimark\n");

#ifdef	STREAMSCONN
	fds[0].fd = ConnectionNumber(DISP);
#endif	/* STREAMSCONN */

  DPS_mag = DPS_shrink = -1;
  DPS_active = False;
  DPS_pending = 1;
  DPSFlushContext(DPS_ctx);
  psp = dps_procs;

  return True;
}


/*---------------------------------------------------------------------------*
  toggleDPS()

  Arguments: none
  Returns: (void)
  Side-Effects: psp.drawbegin is changed.

  Description:
  Used to toggle the rendering of PostScript by the DPS server

+----------------------------------------------------------------------------*/

static	void
toggleDPS()
{
  if (debug & DBG_PS) Puts("Toggling DPS on or off");
  if (postscript) psp.drawbegin = drawbeginDPS;
  else {
    interruptDPS();
    psp.drawbegin = drawbegin_none;
  }
}


/*---------------------------------------------------------------------------*
  destroyDPS()

  Arguments: none
  Returns: (void)
  Side-Effects: the context is nulled out and destroyed.

  Description:
  Close the connection to the DPS server; used when rendering is terminated
  in any way.

+----------------------------------------------------------------------------*/

static	void
destroyDPS()
{
  if (debug & DBG_PS)
    Puts("Calling destroyDPS()");
  if (linepos > line) {
    *linepos = '\0';
    Printf("DPS: %s\n", line);
  }
  DPSDestroySpace(DPS_space);
  psp = no_ps_procs;
}


/*---------------------------------------------------------------------------*
  interruptDPS()

  Arguments: none
  Returns: (void)
  Side-Effects: the context may be nulled out and destroyed.

  Description:
  Close the connection to the DPS server; used when rendering is terminated
  because of an interruption in the viewing of the current page.

+----------------------------------------------------------------------------*/

static	void
interruptDPS()
{
  if (debug & DBG_PS)
    Puts("Running interruptDPS()");

  if (DPS_pending > 0) {
    if (debug & DBG_PS)
      Printf("interruptDPS: code is now %d\n", XDPSGetContextStatus(DPS_ctx));

    /*
     * I would really like to use DPSInterruptContext() here, but (at least
     * on an RS6000) I can't get it to work.
     */

    DPSDestroyContext(DPS_ctx);
    DPS_ctx = NULL;
    DPS_active = False;
    DPS_pending = 0;
  }
}


/*---------------------------------------------------------------------------*
  endpageDPS()

  Arguments: none
  Returns: (void)
  Side-Effects: the DPS_active variable is cleared.

  Description:
  Should be called at the end of a page to end this chunk for the DPS server.

+----------------------------------------------------------------------------*/

static	void
endpageDPS()
{
  if (DPS_active) {
    if (debug & DBG_PS)
      Puts("Endpage sent to context");
    DPSPrintf(DPS_ctx, "stop\n%%%%xdvimark\n");
    DPSFlushContext(DPS_ctx);
    DPS_active = False;
    waitack();
  }
}


/*---------------------------------------------------------------------------*
  drawbeginDPS  ()

  Arguments: xul, yul - coordinates of the upper left corner of the figure
             cp - string with the bounding box line data
  Returns: (void)
  Side-Effects: DPS_ctx is set is set and connection to DPS server is
                opened.

  Description:
  Opens a connection to the DPS server and send in the preamble and the
  bounding box information after correctly computing resolution factors.
  In case no rendering is to be done, outlines the figure.
  An outline is also generated whenever the a context cannot be allocated

+----------------------------------------------------------------------------*/

static	void
drawbeginDPS(xul, yul, cp)
  int xul, yul;
  char *cp;
{
  /* static char faulty_display_vs[]
   * ="DECWINDOWS DigitalEquipmentCorporation UWS4.2LA"; */

  if (debug & DBG_PS)
    Printf("Begin drawing at xul= %d, yul= %d.\n", xul, yul);

  /* we assume that I cannot write the file to the postscript context */
  if (DPS_ctx == NULL) {
    DPS_ctx = XDPSCreateSimpleContext(DISP, mane.win, ruleGC, 0, 0,
				     TextProc, DPSErrorProcHandler, DPS_space);
    if (DPS_ctx == NULL) {
      psp = no_ps_procs;
      draw_bbox();
      return;
    }
    /* XDPSSetStatusMask(DPS_ctx, PSNEEDSINPUTMASK, 0, 0); */
    DPSWritePostScript(DPS_ctx, preamble, sizeof(preamble) - 1);
    /* DPSWritePostScript(DPS_ctx, psheader, psheaderlen); */
    DPSPrintf(DPS_ctx, "matrix setmatrix stop\n%%%%xdvimark\n");
    DPS_mag = DPS_shrink = -1;
    DPS_active = False;
    DPS_pending = 1;
  }

  if (!DPS_active) {
    /* send initialization to context */
    if (magnification != DPS_mag) {
	DPSPrintf(DPS_ctx, "H TeXDict begin /DVImag %d 1000 div def \
end stop\n%%%%xdvimark\n",
	    DPS_mag = magnification);
	++DPS_pending;
    }
    if (mane.shrinkfactor != DPS_shrink) {
	DPSPrintf(DPS_ctx, "H TeXDict begin %d %d div dup \
/Resolution X /VResolution X \
end stop\n%%%%xdvimark\n",
	    pixels_per_inch, DPS_shrink = mane.shrinkfactor);
	++DPS_pending;
    }
    DPSPrintf(DPS_ctx, " TeXDict begin\n");
    DPS_active = True;
    ++DPS_pending;
  }

  DPSPrintf(DPS_ctx, "%d %d moveto\n", xul, yul);
  DPSPrintf(DPS_ctx, "%s\n", cp);
}


/*---------------------------------------------------------------------------*

  drawrawDPS()

  Arguments: cp - the raw string to be sent to the postscript interpreter
  Returns: (void)
  Side-Effects: (none)

  Description:
  If there is a valid postscript context, just send the string to the
  interpreter, else leave.

+----------------------------------------------------------------------------*/

static	void
drawrawDPS(cp)
  char *cp;
{
  if (!DPS_active)
    return;

  if (debug & DBG_PS)
    Printf("Sending raw PS to context: %s\n", cp);

  read_events(False);
  DPSPrintf(DPS_ctx,"%s\n", cp);
}


/*---------------------------------------------------------------------------*
  drawfileDPS()

  Arguments: cp - string with the postscript file pathname
  Returns: (void)
  Side-Effects: none

  Description:
  Postscript file containing the figure is opened and sent to the DPS server.

+----------------------------------------------------------------------------*/

static	void
drawfileDPS(cp)
  char *cp;
{
  char buffer[1025];
  int blen;
  FILE *psfile;

  if (!DPS_active)
    return;

#ifndef	VMS
  if ((psfile = xfopen(cp, "r")) == NULL)
#else
  if ((psfile = xfopen(cp, "r", "?")) == NULL)
#endif
  {
    Fprintf(stderr, "[%%Display PostScript: cannot access file %s%%]\n", cp);
  } else {
    if (debug & DBG_PS)
      Printf("sending file %s\n", cp);
    allow_can = False;
    for (;;) {
      read_events(False);
      if (canit || !DPS_active) break;	/* alt_canit is not a factor here */
      blen = fread(buffer, sizeof(char), 1024, psfile);
      if (blen == 0) break;
      DPSWritePostScript(DPS_ctx, buffer, blen);
    }
    fclose(psfile);
    allow_can = True;
    if (canit) {
      interruptDPS();
      longjmp(canit_env, 1);
    }
  }
}


/*---------------------------------------------------------------------------*
  drawendDPS()

  Arguments: cp - string with indication of the end of the special
  Returns: (void)
  Side-Effects: none

  Description:
  Sends the indication of end of the figure PostScript code.

+----------------------------------------------------------------------------*/

static	void
drawendDPS(cp)
  char *cp;
{
  if (!DPS_active)
    return;

  if (debug & DBG_PS)
    Printf("End PS: %s\n", cp);
  read_events(False);
  DPSPrintf(DPS_ctx,"%s\n", cp);
}
