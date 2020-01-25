/*
 * Copyright 1987 by Tektronix Inc, Beaverton, Oregon and
 * Massachusetts Institute of Technology
 * 
 *
 *                         All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of Tektronix and 
 * Massachusetts Institute of Technology not be used in advertising
 * or publicity pertaining to distribution of the software without
 * specific, written prior permission.
 *
 *
 * TEKTRONIX DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * TEKTRONIX BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */
/* 
 * $Header: resource.c,v 4.2 88/02/15 10:04:43 jim Exp $ 
 * $Locker:  $ 
 */
#include <copyright.h>

/*	Routines to manage various kinds of resources:
 *
 *	Add_resource, Free_resource, Free_client_resources,
 *	Bit_size, Pix_size,
 *	Get_font, Store_cursor,
 *	Define_self, Reset_hosts, Add_host, Remove_host, Invalid_host,
 *	Store_cut, Fetch_cut, Rotate_cuts, Reset_cuts,
 *	Init_colormap,
 *	Get_color, Get_cells, Free_colors, Store_colors, Query_color,
 *	Alloc_rectangle, Free_rectangles, Free_rectangle_storage,
 *	Xalloc, Xrealloc
 */

#include "Xint.h"
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#ifndef SYSV
#include <net/if.h>
#endif /* SYSV */
#include <netinet/in.h>
#include <netdb.h>
#include <ctype.h>
#ifdef SYSV
#include <sys/param.h>
#endif
#ifdef DNETCONN
#include <netdnet/dn.h>
#include <netdnet/dnetdb.h>
#endif

#include <X11/Xos.h>

typedef struct _host {
	short family;
	short len;
	char addr[4];		/* will need to be bigger eventually */
	struct _host *next;
} HOST;

static HOST *selfhosts = NULL;
static HOST *validhosts = NULL;

char *Xalloc(), *Xrealloc(), *malloc(), *realloc();

extern int errno;
extern u_char Xstatus;

/* Define this host for access control */

Define_self (fd)
	int fd;
{
	char buf[2048];
	register int n;
	int len;
	caddr_t addr;
	short family;
	register HOST *host;
#ifdef SYSV
	struct hostent *myhostent;
	struct hostent *gethostbyname();

	char	hostname[120];
	register struct hostent  *hp;

	union {
		struct  sockaddr   sa;
		struct  sockaddr_in  in;
	} saddr;
	
	struct	sockaddr_in	*inetaddr;

	gethostname (hostname, sizeof(hostname));
	hp = gethostbyname (hostname);
	if (hp == NULL)		;   /* error	*/
	else
	{ saddr.sa.sa_family = hp->h_addrtype;
	  inetaddr = (struct sockaddr_in *) (&(saddr.sa));
	  bcopy ( hp->h_addr, &(inetaddr->sin_addr), hp->h_length);
	  family = Convert_addr ( &(saddr.sa), &len, &addr);
	  if ( family <= 0)  ;      /* error	must not hapen */
	  else
	    { for (host = selfhosts;
		   host && ( family != host->family ||
			     bcmp ( addr, host->addr, len));
		   host = host->next) ;
	      if ( host) ;  /* this host already exists, do nothing. */
	       else
		{ /* add this host to the host list.	*/
		  host = (HOST *) Xalloc (sizeof (HOST));
		  host->family = family;
		  host->len = len;
		  bcopy ( addr, host->addr, len);
		  host->next = selfhosts;
		  selfhosts = host;
		}
	    }
	}
#else /* end SYSV */
	struct ifconf ifc;
	register struct ifreq *ifr;

	ifc.ifc_len = sizeof (buf);
	ifc.ifc_buf = buf;

	if (ioctl (fd, (int) SIOCGIFCONF, (caddr_t) &ifc) < 0)
	    Error ("Getting interface configuration");
	for (ifr = ifc.ifc_req, n = ifc.ifc_len / sizeof (struct ifreq);
	     --n >= 0;
	     ifr++) {
	    if ((family = Convert_addr (&ifr->ifr_addr, &len, &addr)) <= 0)
		continue;
	    for (host = selfhosts;
		 host && (family != host->family ||
			  bcmp (addr, host->addr, len));
		 host = host->next) ;
	    if (host) continue;
	    host = (HOST *) Xalloc (sizeof (HOST));
	    host->family = family;
	    host->len = len;
	    bcopy(addr, host->addr, len);
	    host->next = selfhosts;
	    selfhosts = host;
	}
#endif /* SYSV */

}

/* Reset access control list to initial hosts */

Reset_hosts (display)
	char *display;
{
	register HOST *host, *self;
	char hostname[120];
	char fname[32];
	FILE *fd;
	char *ptr;
	register struct hostent *hp;
	union {
	    struct sockaddr sa;
	    struct sockaddr_in in;
#ifdef DNETCONN
	    struct sockaddr_dn dn;
#endif
	} saddr;
#ifdef DNETCONN
	struct nodeent *np;
	struct dn_naddr dnaddr, *dnaddrp, *dnet_addr();
#endif
	short family;
	int len;
	caddr_t addr;

	while (host = validhosts) {
	    validhosts = host->next;
	    free ((caddr_t) host);
	}
	for (self = selfhosts; self; self = self->next) {
	    host = (HOST *) Xalloc (sizeof (HOST));
	    *host = *self;
	    host->next = validhosts;
	    validhosts = host;
	}
	strcpy (fname, "/etc/X");
	strcat (fname, display);
	strcat (fname, ".hosts");
	if (fd = fopen (fname, "r")) {
	    while (fgets (hostname, sizeof (hostname), fd)) {
		if (isspace(hostname[0])) continue; /* make sure host there */
		if (hostname[0] == '#') continue;
		if (ptr = index (hostname, '\n'))
		    *ptr = 0;
#ifdef DNETCONN
		if ((ptr = index (hostname, ':')) && (*(ptr + 1) == ':')) {
		    /* node name (DECnet names end in "::") */
		    *ptr = 0;
		    if (dnaddrp = dnet_addr(hostname)) {
			    /* allow nodes to be specified by address */
			    Add_host (-1, XAF_DECnet, (caddr_t) dnaddrp);
		    } else {
			if (np = getnodebyname (hostname)) {
			    /* node was specified by name */
			    saddr.sa.sa_family = np->n_addrtype;
			    if ((family = Convert_addr (&saddr.sa, &len, &addr)) == XAF_DECnet) {
				bzero ((caddr_t) &dnaddr, sizeof (dnaddr));
				dnaddr.a_len = np->n_length;
				bcopy (np->n_addr, (caddr_t) dnaddr.a_addr, np->n_length);
				Add_host (-1, family, (caddr_t) &dnaddr);
			    }
			}
		    }
		} else {
#endif
		    /* host name */
		    if (hp = gethostbyname (hostname)) {
			saddr.sa.sa_family = hp->h_addrtype;
			if ((family = Convert_addr (&saddr.sa, &len, &addr)) > 0)
			    Add_host (-1, family, hp->h_addr);
		    }
#ifdef DNETCONN
		}	
#endif
	    }
	    fclose (fd);
	}
}

/* Add a host to the access control list */

Add_host (client, family, addr)
	int client;
	short family;
	caddr_t addr;
{
	int len;
	register HOST *host;

	if ((len = Check_family (client, family)) < 0)
	    return;
	for (host = validhosts; host; host = host->next) {
	    if (family == host->family && !bcmp (addr, host->addr, len))
		return;
	}
	host = (HOST *) Xalloc (sizeof (HOST));
	host->family = family;
	host->len = len;
	bcopy(addr, host->addr, len);
	host->next = validhosts;
	validhosts = host;
}

/* Remove a host from the access control list */

Remove_host (client, family, addr)
	int client;
	short family;
	caddr_t addr;
{
	int len;
	register HOST *host, **prev;

	if ((len = Check_family (client, family)) < 0)
	    return;
	for (prev = &validhosts;
	     (host = *prev) &&
	      (family != host->family || bcmp (addr, host->addr, len));
	     prev = &host->next) ;
	if (host) {
	    *prev = host->next;
	    free ((caddr_t) host);
	}
}

/* Get all hosts in the access control list */

Get_hosts (family, data)
	short family;
	char **data;
{
	int len;
	register int n;
	register caddr_t ptr;
	register HOST *host;

	if ((len = Check_family (-1, family)) < 0)
	    return (-1);
	n = 0;
	for (host = validhosts; host; host = host->next) {
	    if (host->family == family)
		n += len;
	}
	if (n) {
	    *data = ptr = Xalloc (n);
	    for (host = validhosts; host; host = host->next) {
		if (host->family == family) {
		    bcopy (host->addr, ptr, len);
		    ptr += len;
		}
	    }
	}
	return (n);
}

/* Check for valid address family, and for local host if client modification.
 * Return address length.
 */

Check_family (client, family)
	int client;
	short family;
{
	struct sockaddr from;
	int alen;
	caddr_t addr;
	register HOST *host;
	int len;

	switch (family) {
	case XAF_INET:
	    len = sizeof (struct in_addr);
	    break;
#ifdef DNETCONN
	case XAF_DECnet:
	    len = sizeof (struct dn_naddr);
	    break;
#endif
	default:
	    Xstatus = BadValue;
	    return (-1);
	}
	if (client < 0) return (len);
	alen = sizeof (from);
	if (!getpeername (client, &from, &alen)) {
	    if ((family = Convert_addr (&from, &alen, &addr)) >= 0) {
		if (family == 0) return (len);
		for (host = selfhosts; host; host = host->next) {
		    if (family == host->family &&
			!bcmp (addr, host->addr, alen))
			return (len);
		}
	    }
	}
	Xstatus = BadAccess;
	return (-1);
}

/* Check if a host is not in the access control list */

Invalid_host (saddr, len)
	register struct sockaddr *saddr;
	int len;
{
	short family;
	caddr_t addr;
	HOST *host;

	if ((family = Convert_addr (saddr, &len, &addr)) < 0)
	    return (1);
	if (family == 0) return (0);
	for (host = validhosts; host; host = host->next) {
	    if (family == host->family && !bcmp (addr, host->addr, len))
		return (0);
	}
	return (1);
}

Convert_addr (saddr, len, addr)
	register struct sockaddr *saddr;
	int *len;
	caddr_t *addr;
{
	if (len == 0) return (0);
	switch (saddr->sa_family) {
	case AF_UNSPEC:
	case AF_UNIX:
	    return (0);
	case AF_INET:
	    *len = sizeof (struct in_addr);
	    *addr = (caddr_t) &(((struct sockaddr_in *) saddr)->sin_addr);
	    return (XAF_INET);
#ifdef DNETCONN
	case AF_DECnet:
	    *len = sizeof (struct dn_naddr);
	    *addr = (caddr_t) &(((struct sockaddr_dn *) saddr)->sdn_add);
	    return (XAF_DECnet);
#endif
	default:
	    break;
	}
	return (-1);
}

/* malloc wrap-around, to take care of the "no memory" case, since
 * it would be difficult in many places to "back out" on failure.
 */

char *Xalloc (amount)
	int amount;
{
	register char *ptr;

	if (ptr = malloc ((unsigned) amount))
	    return (ptr);
	errno = ENOMEM;
	Error ("Allocating");
	/*NOTREACHED*/
}

/* realloc wrap-around, to take care of the "no memory" case, since
 * it would be difficult in many places to "back out" on failure.
 */

char *Xrealloc (ptr, amount)
	register char *ptr;
	int amount;
{
	if (ptr)
	    ptr = realloc (ptr, (unsigned) amount);
	else
	    ptr = malloc ((unsigned) amount);
	if (ptr)
	    return (ptr);
	errno = ENOMEM;
	Error ("Allocating");
	/*NOTREACHED*/
}

/* Determine the number of bytes in a bitmap (including padding) */

Bit_size (height, width)
	register int height, width;
{
	if (height > 0 && width > 0)
	    return (WordPad(BitmapSize(width, height)));
	Xstatus = BadValue;
	return (0);
}

/* Determine the number of bytes in a pixmap (including padding) */

Pix_size (client, format, height, width)
        register int format, height, width;
{
	int	devicePlanes = DevicePlanes(client);

	if (devicePlanes == 1)
		return( Bit_size (height, width) );
	if (height > 0 && width > 0) {
	    if (format == XYFormat)
		return (WordPad(XYPixmapSize(width, height, devicePlanes)));
	    else if (format == ZFormat) {
		if (devicePlanes > 8)
		    return (WordPad(WZPixmapSize(width, height)));
		else if (devicePlanes > 1)
		    return (BytePad(BZPixmapSize(width, height)));
	    }
	}
	Xstatus = BadValue;
	return (0);
}

