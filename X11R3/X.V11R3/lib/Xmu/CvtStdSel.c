/* $XConsortium: CvtStdSel.c,v 1.7 88/11/01 17:00:44 jim Exp $
 *
 * Copyright 1988 by the Massachusetts Institute of Technology
 *
 * This file contains routines to handle common selection targets.
 *
 * Public entry points:
 *
 *	XmuConvertStandardSelection()	return a known selection
 */

#include <X11/copyright.h>

#include <X11/IntrinsicP.h>
#include <X11/Xatom.h>
#include <X11/Shell.h>
#include <X11/ShellP.h>
#include <stdio.h>
#include <netdb.h>
#include <sys/socket.h>
#include "Xmu.h"


static char *get_os_name ()
{
	char *os_name = NULL;
	FILE *f = NULL;

#ifdef OS_NAME
	return XtNewString(OS_NAME);
#else

#if !defined(X_OS_FILE)
#ifdef SYSV			/* keep separate until makedepend fixed */
#define USE_UNAME
#endif
#ifdef ultrix
#define USE_UNAME
#endif
#endif /*X_OS_FILE*/


#ifdef USE_UNAME
#ifdef ultrix
#include <sys/limits.h>		/* fixed in Ultrix 3.0 */
#endif
#include <sys/utsname.h>
	struct utsname uts;

	if (uname (&uts) == 0) {
	    int len = strlen(uts.sysname);
#ifndef hpux				/* because of hostname length crock */
	    len += 1 + strlen(uts.release);
#endif
	    os_name = XtMalloc (len);
	    strcpy (os_name, uts.sysname);
#ifndef hpux
	    strcat (os_name, " ");
	    strcat (os_name, uts.release);
#endif
	    return os_name;
	}
#endif

#ifdef X_OS_FILE
	f = fopen(X_OS_FILE, "r");
	if (!f)
#endif
#ifdef MOTD_FILE
	       f = fopen(MOTD_FILE, "r");
#endif
	if (f) {
	    char motd[512];
	    motd[0] = '\0';
	    (void) fgets(motd, 511, f);
	    fclose(f);
	    motd[511] = '\0';
	    if (motd[0] != '\0') {
		int len = strlen(motd);
		if (motd[len - 1] == '\n')
		    motd[len - 1] = '\0';
		return XtNewString(motd);
	    }
	}

#ifdef sun
	return XtNewString("SunOS");
#else
# if !defined(SYSV) && defined(unix)
	return XtNewString("BSD");
# else
	return NULL;
# endif
#endif

#endif /*OS_NAME*/
}


Boolean XmuConvertStandardSelection(w, time, selection, target,
				    type, value, length, format)
    Widget w;
    Time time;
    Atom *selection, *target, *type;
    caddr_t *value;
    unsigned long *length;
    int *format;
{
    Display *d = XtDisplay(w);
    if (*target == XA_TIMESTAMP(d)) {
	*value = XtMalloc(4);
	if (sizeof(long) == 4)
	    *(long*)*value = time;
	else {
	    long temp = time;
	    bcopy( ((char*)&temp)+sizeof(long)-4, (char*)*value, 4);
	}
	*type = XA_INTEGER;
	*length = 1;
	*format = 32;
	return True;
    }
    if (*target == XA_HOSTNAME(d) || *target == XA_IP_ADDRESS(d)) {
	char hostname[1024];
	gethostname(hostname, 1024);
	if (*target == XA_HOSTNAME(d)) {
	    *value = XtNewString(hostname);
	    *type = XA_STRING;
	    *length = strlen(hostname);
	    *format = 8;
	    return True;
	}
	else { /* *target == XA_IP_ADDRESS(d) */
	    struct hostent *hostent = gethostbyname(hostname);
	    if (hostent->h_addrtype != AF_INET) return False;
	    *length = hostent->h_length;
	    *value = XtMalloc(*length);
	    bcopy(hostent->h_addr, *value, *length);
	    *type = XA_NET_ADDRESS(d);
	    *format = 8;
	    return True;
	}
    }
#ifdef DNETCONN
    if (*target == XA_DECNET_ADDRESS(d)) {
	return False;		/* %%% NYI */
    }
#endif
    if (*target == XA_USER(d)) {
	char *name = (char*)getenv("USER");
	if (name == NULL) return False;
	*value = XtNewString(name);
	*type = XA_STRING;
	*length = strlen(name);
	*format = 8;
	return True;
    }
    if (*target == XA_CLASS(d)) {
	Widget parent = XtParent(w);
	char *class;
	int len;
	while (parent != NULL &&
	       !XtIsSubclass(w, applicationShellWidgetClass)) {
	    w = parent;
	    parent = XtParent(w);
	}
	if (XtIsSubclass(w, applicationShellWidgetClass))
	    class = ((ApplicationShellWidget) w)->application.class;
	else {
	    class = XrmQuarkToString( XtClass(w)->core_class.xrm_class );
	}
	*length = (len=strlen(w->core.name)) + strlen(class) + 2;
	*value = XtMalloc(*length);
	strcpy( (char*)*value, w->core.name );
	strcpy( (char*)*value+len+1, class );
	*type = XA_STRING;
	*format = 8;
	return True;
    }
    if (*target == XA_NAME(d)) {
	Widget parent = XtParent(w);
	while (parent != NULL &&
	       !XtIsSubclass(w, wmShellWidgetClass)) {
	    w = parent;
	    parent = XtParent(w);
	}
	if (!XtIsSubclass(w, wmShellWidgetClass)) return False;
	*value = XtNewString( ((WMShellWidget) w)->wm.title );
	*length = strlen(*value);
	*type = XA_STRING;
	*format = 8;
	return True;
    }
    if (*target == XA_CLIENT_WINDOW(d)) {
	Widget parent = XtParent(w);
	while (parent != NULL) {
	    w = parent;
	    parent = XtParent(w);
	}
	*value = XtMalloc(sizeof(Window));
	*(Window*)*value = w->core.window;
	*type = XA_WINDOW;
	*length = 1;
	*format = 32;
	return True;
    }
    if (*target == XA_OWNER_OS(d)) {
	*value = get_os_name();
	if (*value == NULL) return False;
	*type = XA_STRING;
	*length = strlen(*value);
	*format = 8;
	return True;
    }
    if (*target == XA_TARGETS(d)) {
#if defined(unix) && defined(DNETCONN)
#  define NUM_TARGETS 9
#else
#  if defined(unix) || defined(DNETCONN)
#    define NUM_TARGETS 8
#  else
#    define NUM_TARGETS 7
#  endif
#endif
	Atom* std_targets = (Atom*)XtMalloc(NUM_TARGETS*sizeof(Atom));
	int i = 0;
	std_targets[i++] = XA_TIMESTAMP(d);
	std_targets[i++] = XA_HOSTNAME(d);
	std_targets[i++] = XA_IP_ADDRESS(d);
	std_targets[i++] = XA_USER(d);
	std_targets[i++] = XA_CLASS(d);
	std_targets[i++] = XA_NAME(d);
	std_targets[i++] = XA_CLIENT_WINDOW(d);
#ifdef unix
	std_targets[i++] = XA_OWNER_OS(d);
#endif
#ifdef DNETCONN
	std_targets[i++] = XA_DECNET_ADDRESS(d);
#endif
	*value = (caddr_t)std_targets;
	*type = XA_ATOM;
	*length = NUM_TARGETS;
	*format = 32;
	return True;
    }
    /* else */
    return False;
}
