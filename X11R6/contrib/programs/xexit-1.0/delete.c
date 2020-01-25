/*
 * $Id: delete.c,v 1.2 1994/06/06 14:29:17 stumpf Exp $
 *
 * Copyright (c) 1992-1994   Markus Stumpf
 * 
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE X CONSORTIUM OR THE AUTHOR BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of the X Consortium shall
 * not be used in advertising or otherwise to promote the sale, use or
 * other dealings in this Software without prior written authorization
 * from the X Consortium.
 *
 * $Source: /usr/wiss/stumpf/src/CVS/xexit/delete.c,v $
 */

#ifndef lint
static char *RCSId ="$Id: delete.c,v 1.2 1994/06/06 14:29:17 stumpf Exp $";
#endif /* lint */

#include "xexit.h"

void DeleteAllWindows _P((Display * dpy));
static void DeleteWindow _P((Display *, Window));
static void ParseDeleteTree _P((Display *, Window));
static void ParseKillTree _P((Display *, Window));
static void SaveYourself _P((Display *, Window));
int MyErrorHandler _P((Display *, XErrorEvent *));

Boolean		restart = False;
Atom            _XA_WM_SAVE_YOURSELF;
Atom            _XA_WM_DELETE_WINDOW;
Atom            _XA_WM_PROTOCOLS;

#define ICCCM_WmSaveYourself    (1L<<1)
#define ICCCM_WmDeleteWindow    (1L<<2)

/* ------------------------------------------------------------------------
 */
#if NeedFunctionPrototypes
int
MyErrorHandler(Display     *dpy,
	       XErrorEvent *event)
#else
int
MyErrorHandler(dpy, event)
    Display        *dpy;
    XErrorEvent    *event;
#endif				/* NeedFunctionPrototypes */
{
#ifdef TESTIT
    char	buffer[BUFSIZ];
#endif /* TESTIT */

    restart = True;

#ifdef TESTIT
    fprintf(stderr, "errorHandler called()\n");
    fprintf(stderr, "serial=0x%x, code=0x%x - 0x%x, resource=0x%x\n",
	   event->serial, event->request_code, event->minor_code,
	   event->resourceid);

    XGetErrorText(dpy, event->error_code, buffer, BUFSIZ);
    fprintf(stderr, "  %s\n", buffer);
    fflush(stderr);
#endif /* TESTIT */

    return(0);
}



/* ------------------------------------------------------------------------
 */
#if NeedFunctionPrototypes
static void
SaveYourself(Display * dpy,
	     Window window)
#else
static void
SaveYourself(dpy, window)
    Display        *dpy;
    Window          window;
#endif				/* NeedFunctionPrototypes */
{
    XEvent          event;
    int             status;

    event.type = ClientMessage;
    event.xclient.display = dpy;
    event.xclient.window = window;

    event.xclient.message_type = _XA_WM_PROTOCOLS;
    event.xclient.format = 32;
    event.xclient.data.l[0] = _XA_WM_SAVE_YOURSELF;
    event.xclient.data.l[1] = CurrentTime;

    status = XSendEvent(dpy, window, False, 0L, &event);
    XFlush(dpy);
}


/* ------------------------------------------------------------------------
 */
#if NeedFunctionPrototypes
static void
DeleteWindow(Display * dpy,
	     Window window)
#else
static void
DeleteWindow(dpy, window)
    Display        *dpy;
    Window          window;
#endif				/* NeedFunctionPrototypes */
{
    XEvent          event;
    int             status;

    event.type = ClientMessage;
    event.xclient.display = dpy;
    event.xclient.window = window;

    event.xclient.message_type = _XA_WM_PROTOCOLS;
    event.xclient.format = 32;
    event.xclient.data.l[0] = _XA_WM_DELETE_WINDOW;
    event.xclient.data.l[1] = CurrentTime;

    status = XSendEvent(dpy, window, False, 0L, &event);
    XFlush(dpy);
}


/* ------------------------------------------------------------------------
 */
#if NeedFunctionPrototypes
static void
ParseDeleteTree(Display * dpy,
	  Window parent)
#else
static void
ParseDeleteTree(dpy, parent)
    Display        *dpy;
    Window          parent;
#endif				/* NeedFunctionPrototypes */
{
    Window         *children, root_return, parent_return;
    unsigned int    num_children, i;
    XClassHint     *class_hint;
    Atom           *protocols;
    int             num_prots;
    unsigned long   flags;

    class_hint = XAllocClassHint();
    if (XGetClassHint(dpy, parent, class_hint))
    {
	if (XGetWMProtocols(dpy, parent, &protocols, &num_prots))
	{
	    register int    j;
	    register Atom  *atom;
	    flags = 0L;

	    for (j = 0, atom = protocols; j < num_prots; j++, atom++)
	    {
		if (*atom == _XA_WM_DELETE_WINDOW)
		    flags |= ICCCM_WmDeleteWindow;
		if (*atom == _XA_WM_SAVE_YOURSELF)
		    flags |= ICCCM_WmSaveYourself;
            }
	    if (flags & ICCCM_WmSaveYourself)
	    {
#ifdef TESTIT
		    fprintf(stderr, "SAVE_YOURSELF 0x%x - %s (%s)\n",
			    parent,
			    class_hint->res_name,
			    class_hint->res_class);
		    fflush(stderr);
#endif /* TESTIT */
		    SaveYourself(dpy, parent);
	    }

	    if (flags & ICCCM_WmDeleteWindow)
	    {
#ifdef TESTIT
		    fprintf(stderr, "DELETE 0x%x - %s (%s)\n",
			    parent,
			    class_hint->res_name,
			    class_hint->res_class);
		    fflush(stderr);
#else
		    DeleteWindow(dpy, parent);
#endif /* TESTIT */
	    }

	    if (protocols)
		XFree(protocols);
	}
	if (class_hint->res_class)
	    XFree(class_hint->res_class);
	if (class_hint->res_name)
	    XFree(class_hint->res_name);
#ifdef TESTIT
	fprintf(stderr, "processed 0x%x - %s (%s)\n",
		parent,
		class_hint->res_name,
		class_hint->res_class);
	fflush(stderr);
#endif /* TESTIT */
    }
    else
    {
	XQueryTree(dpy, parent, &root_return, &parent_return, &children,
		   &num_children);
	for (i = 0; i < num_children; i++)
	{
	    if (restart)
	    {
#ifdef TESTIT
		fprintf(stderr, "RETURN TO LOOP\n");
		fflush(stderr);
#endif /* TESTIT */
		return;
	    }
	    ParseDeleteTree(dpy, children[i]);
	}
    }

}


/* ------------------------------------------------------------------------
 */
#if NeedFunctionPrototypes
static void
ParseKillTree(Display * dpy,
	  Window parent)
#else
static void
ParseKillTree(dpy, parent)
    Display        *dpy;
    Window          parent;
#endif				/* NeedFunctionPrototypes */
{
    Window         *children, root_return, parent_return;
    unsigned int    num_children, i;

    XQueryTree(dpy, parent, &root_return, &parent_return, &children,
	       &num_children);
#ifdef TESTIT
    fprintf(stderr, "    found %d\n", num_children);
    fflush(stderr);
#endif /* TESTIT */

    for (i = 0; i < num_children; i++)
    {
#ifdef TESTIT
	fprintf(stderr, "would KILL   0x%x\n", children[i]);
	fflush(stderr);
#else
	XKillClient(dpy, children[i]);
#endif /* TESTIT */
    }
    XFlush(dpy);
}


/* ------------------------------------------------------------------------
 */
#if NeedFunctionPrototypes
void
DeleteAllWindows(Display * dpy)
#else
void
DeleteAllWindows(dpy)
    Display        *dpy;
#endif				/* NeedFunctionPrototypes */
{
    _XA_WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    _XA_WM_SAVE_YOURSELF = XInternAtom(dpy, "WM_SAVE_YOURSELF", False);
    _XA_WM_PROTOCOLS = XInternAtom(dpy, "WM_PROTOCOLS", False);

    XSetErrorHandler(MyErrorHandler);

    do
    {
	restart = False;
        ParseDeleteTree(dpy, DefaultRootWindow(dpy));
    } while (restart == True);

    sleep(1);
    ParseKillTree(dpy, DefaultRootWindow(dpy));
    ParseKillTree(dpy, DefaultRootWindow(dpy));
    XBell(dpy, 100);
    XFlush(dpy);
    XBell(dpy, 100);
    XFlush(dpy);
}
