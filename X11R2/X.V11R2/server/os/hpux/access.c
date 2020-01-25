/***********************************************************
Copyright 1987 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

/* $Header: access.c,v 1.6 88/02/19 11:59:00 rws Exp $ */

#include "X.h"
#include "Xproto.h"
#include "misc.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#ifndef hpux
#include <net/if.h>
#endif

#ifdef hpux
#include <sys/utsname.h>
#endif

#include <netdb.h>
#ifdef TCPCONN
#include <netinet/in.h>
#endif /* TCPCONN */
#ifdef DNETCONN
#include <netdnet/dn.h>
#include <netdnet/dnetdb.h>
#endif
#undef NULL
#include <stdio.h>
#include "dixstruct.h"
#include "osdep.h"


#define DONT_CHECK -1
extern char	*index();

typedef struct _host {
	short		family;
	short		len;
	unsigned char	addr[4];	/* will need to be bigger eventually */
	struct _host *next;
} HOST;

static HOST *selfhosts = NULL;
static HOST *validhosts = NULL;
static int AccessEnabled = TRUE;

typedef struct {
    int af, xf;
} FamilyMap;

static FamilyMap familyMap[] = {
#ifdef     AF_DECnet
    {AF_DECnet, FamilyDECnet},
#endif /* AF_DECnet */
#ifdef     AF_CHAOS
    {AF_CHAOS, FamilyChaos},
#endif /* AF_CHAOS */
#ifdef    AF_INET
    {AF_INET, FamilyInternet}
#endif
};

#define FAMILIES ((sizeof familyMap)/(sizeof familyMap[0]))

#ifdef hpux
/* Define this host for access control.  Find all the hosts the OS knows about 
 * for this fd and add them to the selfhosts list.
 * HPUX version - hpux does not have SIOCGIFCONF ioctl;
 */
DefineSelf (fd)
    int fd;
{
    register int n;
    int	len;
    caddr_t	addr;
    int		family;
    register HOST	*host;

    struct utsname name;
    register struct hostent  *hp;

    union {
	struct  sockaddr   sa;
	struct  sockaddr_in  in;
    } saddr;
	
    struct	sockaddr_in	*inetaddr;

    /* Why not use gethostname()?  Well, at least on my system, I've had to
     * make an ugly kernel patch to get a name longer than 8 characters, and
     * uname() lets me access to the whole string (it smashes release, you
     * see), whereas gethostname() kindly truncates it for me.
     */
    uname(&name);
    hp = gethostbyname (name.nodename);
    if (hp != NULL) {
	saddr.sa.sa_family = hp->h_addrtype;
	inetaddr = (struct sockaddr_in *) (&(saddr.sa));
	bcopy ( hp->h_addr, &(inetaddr->sin_addr), hp->h_length);
	family = ConvertAddr ( &(saddr.sa), &len, &addr);
	if ( family > 0) {
	    for (host = selfhosts;
		 host && ( family != host->family ||
			  bcmp ( addr, host->addr, len));
		 host = host->next) ;
	    if (!host) {
		/* add this host to the host list.	*/
		host = (HOST *) xalloc (sizeof (HOST));
		host->family = family;
		host->len = len;
		bcopy ( addr, host->addr, len);
		host->next = selfhosts;
		selfhosts = host;
	    }
	}
    }
}

#else
/* Define this host for access control.  Find all the hosts the OS knows about 
 * for this fd and add them to the selfhosts list.
 */
DefineSelf (fd)
    int fd;
{
    char		buf[2048];
    struct ifconf	ifc;
    register int	n;
    int 		len;
    pointer 		addr;
    int 		family;
    register HOST 	*host;
    register struct ifreq *ifr;
    
    ifc.ifc_len = sizeof (buf);
    ifc.ifc_buf = buf;
    if (ioctl (fd, (int) SIOCGIFCONF, (pointer) &ifc) < 0)
        Error ("Getting interface configuration");
    for (ifr = ifc.ifc_req, n = ifc.ifc_len / sizeof (struct ifreq); --n >= 0;
     ifr++)
    {
        if ((family = ConvertAddr (&ifr->ifr_addr, &len, &addr)) <= 0)
	    continue;
        for (host = selfhosts; host && (family != host->family ||
	  bcmp (addr, host->addr, len)); host = host->next)
	    ;
        if (host)
	    continue;
        host = (HOST *) xalloc (sizeof (HOST));
        host->family = family;
        host->len = len;
        bcopy(addr, host->addr, len);
        host->next = selfhosts;
        selfhosts = host;
    }
}
#endif hpux

/* Reset access control list to initial hosts */
ResetHosts (display)
    char *display;
{
    register HOST	*host, *self;
    char 		hostname[120];
    char		fname[32];
    FILE		*fd;
    char		*ptr;
    union {
        struct sockaddr	sa;
#ifdef TCPCONN
        struct sockaddr_in in;
#endif /* TCPCONN */
#ifdef DNETCONN
        struct sockaddr_dn dn;
#endif
    } 			saddr;
#ifdef DNETCONN
    struct nodeent 	*np;
    struct dn_naddr 	dnaddr, *dnaddrp, *dnet_addr();
#endif
    int			family;
    int			len;
    pointer		addr;
    register struct hostent *hp;

    while (host = validhosts)
    {
        validhosts = host->next;
        xfree (host);
    }
    for (self = selfhosts; self; self = self->next)
    {
        host = (HOST *) xalloc (sizeof (HOST));
        *host = *self;
        host->next = validhosts;
        validhosts = host;
    }
    strcpy (fname, "/etc/X");
    strcat (fname, display);
    strcat (fname, ".hosts");
    if (fd = fopen (fname, "r")) 
    {
        while (fgets (hostname, sizeof (hostname), fd))
	{
    	if (ptr = index (hostname, '\n'))
    	    *ptr = 0;
#ifdef DNETCONN
    	if ((ptr = index (hostname, ':')) && (*(ptr + 1) == ':'))
	{
    	    /* node name (DECnet names end in "::") */
    	    *ptr = 0;
    	    if (dnaddrp = dnet_addr(hostname))
	    {
    		    /* allow nodes to be specified by address */
    		    NewHost (AF_DECnet, (pointer) dnaddrp);
    	    }
	    else
	    {
    		if (np = getnodebyname (hostname))
		{
    		    /* node was specified by name */
    		    saddr.sa.sa_family = np->n_addrtype;
    		    if ((family = ConvertAddr (&saddr.sa, &len, &addr)) ==
		      AF_DECnet)
		    {
    			bzero ((pointer) &dnaddr, sizeof (dnaddr));
    			dnaddr.a_len = np->n_length;
    			bcopy (np->n_addr, (pointer) dnaddr.a_addr,
			  np->n_length);
    			NewHost (family, (pointer) &dnaddr);
    		    }
    		}
    	    }
    	}
	else
	{
#endif /* DNETCONN */
#ifdef TCPCONN
    	    /* host name */
    	    if (hp = gethostbyname (hostname))
	    {
    		saddr.sa.sa_family = hp->h_addrtype;
    		if ((family = ConvertAddr (&saddr.sa, &len, &addr)) > 0)
#ifdef NEW_HEADER_WITH_OLD_LIBRARY
    		    NewHost (family, hp->h_addr_list);
#else
    		    NewHost (family, hp->h_addr);
#endif

    	    }
#endif /* TCPCONN */
#ifdef DNETCONN
    	}	
#endif /* DNETCONN */
        }
        fclose (fd);
    }
}

static Bool
AuthorizedClient(client)
    ClientPtr client;
{
    int    		alen, family;
    struct sockaddr	from;
    pointer		addr;
    register HOST	*host;

    alen = sizeof (from);
    if (!getpeername (((osPrivPtr)client->osPrivate)->fd, &from, &alen))
    {
        if ((family = ConvertAddr (&from, &alen, &addr)) >= 0)
	{
	    if (family == 0)
		return TRUE;
	    for (host = selfhosts; host; host = host->next)
	    {
		if (family == host->family &&
		    !bcmp (addr, host->addr, alen))
		    return TRUE;
	    }
	}
    }
    return FALSE;
}

/* Add a host to the access control list.  This is the external interface
 * called from the dispatcher */

int
AddHost (client, family, length, pAddr)
    int			client;
    int                 family;
    unsigned            length;        /* of bytes in pAddr */
    pointer             pAddr;
{
    int			len;
    register HOST	*host;
    int                 unixFamily;

    if (!AuthorizedClient(client))
	return(BadAccess);
    unixFamily = UnixFamily(family);
    if ((len = CheckFamily (DONT_CHECK, unixFamily)) < 0)
        return BadMatch;

    if (len != length)
        return BadMatch;
    for (host = validhosts; host; host = host->next)
    {
        if (unixFamily == host->family && !bcmp (pAddr, host->addr, len))
    	    return Success;
    }
    host = (HOST *) xalloc (sizeof (HOST));
    host->family = unixFamily;
    host->len = len;
    bcopy(pAddr, host->addr, len);
    host->next = validhosts;
    validhosts = host;
    return Success;
}

/* Add a host to the access control list. This is the internal interface 
 * called when starting or resetting the server */
NewHost (family, addr)
    int		family;
    pointer	addr;
{
    int		len;
    register HOST *host;

    if ((len = CheckFamily (DONT_CHECK, family)) < 0)
        return;
    for (host = validhosts; host; host = host->next)
    {
        if (family == host->family && !bcmp (addr, host->addr, len))
    	return;
    }
    host = (HOST *) xalloc (sizeof (HOST));
    host->family = family;
    host->len = len;
    bcopy(addr, host->addr, len);
    host->next = validhosts;
    validhosts = host;
}

/* Remove a host from the access control list */

int
RemoveHost (client, family, length, pAddr)
    int			client;
    int                 family;
    unsigned            length;        /* of bytes in pAddr */
    pointer             pAddr;
{
    int			len,
                        unixFamily;
    register HOST	*host, **prev;

    if (!AuthorizedClient(client))
	return(BadAccess);
    unixFamily = UnixFamily(family);
    if ((len = CheckFamily (DONT_CHECK, unixFamily)) < 0)
        return BadMatch;
    if (len != length)
        return BadMatch;
    for (prev = &validhosts;
         (host = *prev) && (unixFamily != host->family ||
		            bcmp (pAddr, host->addr, len));
         prev = &host->next)
        ;
    if (host)
    {
        *prev = host->next;
        xfree (host);
    }
    return Success;
}

/* Get all hosts in the access control list */
int
GetHosts (data, pnHosts, pEnabled)
    pointer		*data;
    int			*pnHosts, *pEnabled;
{
    int			len;
    register int 	n = 0;
    register pointer	ptr;
    register HOST	*host;
    int			nHosts = 0;
    int			*lengths = (int *) NULL;

    *pEnabled = AccessEnabled ? EnableAccess : DisableAccess;
    for (host = validhosts; host; host = host->next)
    {
        if ((len = CheckFamily (DONT_CHECK, host->family)) < 0)
            return (-1);
	lengths = (int *) xrealloc(lengths, (nHosts + 1) * sizeof(int));
	lengths[nHosts++] = len;
	n += (((len + 3) >> 2) << 2) + sizeof(xHostEntry);
    }
    if (n)
    {
        *data = ptr = (pointer) xalloc (n);
	nHosts = 0;
        for (host = validhosts; host; host = host->next)
	{

	    len = lengths[nHosts++];
	    ((xHostEntry *)ptr)->family = XFamily(host->family);
	    ((xHostEntry *)ptr)->length = len;
	    ptr += sizeof(xHostEntry);
	    bcopy (host->addr, ptr, len);
	    ptr += ((len + 3) >> 2) << 2;
        }
    }
    *pnHosts = nHosts;
    xfree(lengths);
    return (n);
}

/* Check for valid address family, and for local host if client modification.
 * Return address length.
 */

CheckFamily (connection, family)
    int			connection;
    int			family;
{
    struct sockaddr	from;
    int	 		alen;
    pointer		addr;
    register HOST	*host;
    int 		len;

    switch (family)
    {
#ifdef TCPCONN
      case AF_INET:
        len = sizeof (struct in_addr);
        break;
#endif 
#ifdef DNETCONN
      case AF_DECnet:
        len = sizeof (struct dn_naddr);
        break;
#endif
      default:
        /* BadValue */
        return (-1);
    }
    if (connection == DONT_CHECK)
        return (len);
    alen = sizeof (from);
    if (!getpeername (connection, &from, &alen))
    {
        if ((family = ConvertAddr (&from, &alen, &addr)) >= 0)
	{
	    if (family == 0)
		return (len);
	    for (host = selfhosts; host; host = host->next)
	    {
		if (family == host->family &&
		    !bcmp (addr, host->addr, alen))
		    return (len);
	    }
	}
    }
    /* Bad Access */
    return (-1);
}

/* Check if a host is not in the access control list. 
 * Returns 1 if host is invalid, 0 if we've found it. */

InvalidHost (saddr, len)
    register struct sockaddr	*saddr;
    int				len;
{
    int 			family;
    pointer			addr;
    register HOST 		*host;
    if ((family = ConvertAddr (saddr, len ? &len : 0, &addr)) < 0)
        return (1);
    if (family == 0)
        return (0);
    if (!AccessEnabled)   /* just let them in */
        return(0);    
    for (host = validhosts; host; host = host->next)
    {
        if (family == host->family && !bcmp (addr, host->addr, len))
    	    return (0);
    }
    return (1);
}

ConvertAddr (saddr, len, addr)
    register struct sockaddr	*saddr;
    int				*len;
    pointer			*addr;
{
    if (len == 0)
        return (0);
    switch (saddr->sa_family)
    {
      case AF_UNSPEC:
#ifndef hpux
      case AF_UNIX:
#endif hpux
        return (0);
      case AF_INET:
#ifdef TCPCONN
        *len = sizeof (struct in_addr);
        *addr = (pointer) &(((struct sockaddr_in *) saddr)->sin_addr);
        return (AF_INET);
#endif TCPCONN
        break;
#ifdef DNETCONN
      case AF_DECnet:
        *len = sizeof (struct dn_naddr);
        *addr = (pointer) &(((struct sockaddr_dn *) saddr)->sdn_add);
        return (AF_DECnet);
#endif
      default:
        break;
    }
    return (-1);
}

ChangeAccessControl(client, fEnabled)
    ClientPtr client;
    int fEnabled;
{
    if (!AuthorizedClient(client))
	return BadAccess;
    AccessEnabled = fEnabled;
    return Success;
}

static int XFamily(af)
    int af;
{
    int i;
    for (i = 0; i < FAMILIES; i++)
        if (familyMap[i].af == af)
            return familyMap[i].xf;
    return -1;
}

static int UnixFamily(xf)
    int xf;
{
    int i;
    for (i = 0; i < FAMILIES; i++)
        if (familyMap[i].xf == xf)
            return familyMap[i].af;
    return -1;
}

