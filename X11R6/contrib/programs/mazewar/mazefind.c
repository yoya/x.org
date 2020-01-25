/* $Header: /tilde/cak/src/X/mw/RCS/mazefind.c,v 1.8 1994/05/31 20:30:47 cak Exp $ */

/* 
 * mazefind.c - find ongoing mazewar games
 * 
 * Author:	Christopher A. Kent
 * 		Western Research Laboratory
 * 		Digital Equipment Corporation
 * Date:	Thu Dec  4 1986
 */

/***********************************************************
Copyright 1986 by Digital Equipment Corporation, Maynard, Massachusetts,

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital not be
used in advertising or publicity pertaining to disstribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/
/****************************************************************
Modified by Xerox Corporation.
Copyright (c) 1992, 1994 Xerox Corporation.
All rights reserved.
 
Redistribution and use of this modified software in source and binary
forms are permitted provided that the above copyright notice and this
paragraph appear in all copies, and that any documentation related to
such distribution and use acknowledge that the software was developed
by the Digital Equipment Corporation, and modified by Xerox Corporation.
Neither the name of Digital Equipment Corporation nor the Xerox tradename
may be used to endorse or promote products derived from this software
without specific prior written permission.
THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
******************************************************************/
/****************************************************************
The X Consortium, and any party obtaining a copy of these files from
the X Consortium, directly or indirectly, is granted, free of charge, a
full and unrestricted irrevocable, world-wide, paid up, royalty-free,
nonexclusive right and license to deal in this software and
documentation files (the "Software"), including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons who receive
copies from any such party to do so.  This license includes without
limitation a license to do the foregoing actions under any patents of
the party supplying this software to the X Consortium.
******************************************************************/

/*
 * $Log: mazefind.c,v $
 * Revision 1.8  1994/05/31  20:30:47  cak
 * copyright hacking for the X consortium
 *
 * Revision 1.7  1994/05/31  03:03:00  cak
 * Paw through interfaces to find proper broadcast address
 * in the face of subnetting.
 *
 * Revision 1.6  1994/05/30  23:00:33  cak
 * Integrated Ron Frederick's multicast changes.
 *
 * Revision 1.5  1988/09/07  11:23:42  kent
 *
 * Revision 1.4  88/08/25  09:57:53  kent
 * 
 * 
 * Revision 1.3  88/06/15  16:36:20  kent
 * 
 * 
 * Revision 1.2  87/03/31  14:42:26  kent
 * Portability considerations, especially byteswapping to/from the net.
 * 
 * Revision 1.1  86/12/04  17:19:31  kent
 * Initial revision
 * 
 */

static char rcs_ident[] = "$Header: /tilde/cak/src/X/mw/RCS/mazefind.c,v 1.8 1994/05/31 20:30:47 cak Exp $";

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include <netinet/in.h>
#include <net/if.h>
#include <netinet/if_ether.h>

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>

#include "mazewar.h"

struct answer {
	Sockaddr	source;
	int		srclen;
	RatPacket	packet;
};

Boolean	FoundOne = FALSE;

#ifdef	MULTICAST
u_char	game_ttl = 255;
#endif	MULTICAST

/* 
 * Broadcast to check out all the games on the net. This is basically
 * findDuke() from init.c.
 */

main(argc, argv)
    	char		**argv;
{
	register int	i;
	int		cnt = 0;
	Sockaddr	rmtAddr;
	struct	timeval	timeout;
	RatPacket	*b = (RatPacket *) malloc(sizeof(RatPacket));
	struct answer	answer;
	int		fds, fd;
	int		ret, cc;
	int		mazePort;
	struct servent	*mazeService;
	char		buf[128];
	Boolean		duke(), freeSlot();
	struct in_addr	inet_makeaddr();
	unsigned long	broadAddr();
	Sockaddr	*resolveHost();

#ifdef	MULTICAST
	if (argc > 1) {
		if ((argc == 3) && !strcmp(argv[1], "-t")) {
			game_ttl = atoi(argv[2]);
		} else {
			fprintf(stderr, "Usage: %s [-t ttl]\n", argv[0]);
			exit(-1);
		}
	}
#endif	MULTICAST

	fd = socket(AF_INET, SOCK_DGRAM, 0);
	if (fd < 0)
		MFError("can't get socket");
#ifndef	MULTICAST
#ifdef	SO_BROADCAST
#ifdef	BSD_43
	{
	int	one = 1;

	if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, &one, sizeof one) < 0)
		MFError("can't get broadcast permission");
	}
#else	BSD_43
	if (setsockopt(fd, SOL_SOCKET, SO_BROADCAST, NULL, 0) < 0)
		MFError("can't get broadcast permission");
#endif	BSD_43
#endif	SO_BROADCAST
#endif	MULTICAST

	mazeService = getservbyname(MAZESERVICE, "udp");
	if (mazeService != (struct servent *) NULL)
		mazePort = mazeService->s_port;
	else
		mazePort = htons(MAZEPORT);

#ifdef	MULTICAST
	rmtAddr.sin_family = AF_INET;
	rmtAddr.sin_addr.s_addr = inet_addr(GAME_ADDR);

	if (setsockopt(fd, IPPROTO_IP, IP_MULTICAST_TTL, &game_ttl,
		sizeof(game_ttl)) < 0) MFError("setsockopt IP_MULTICAST_TTL");
#else	MULTICAST
	gethostname(buf, sizeof(buf));
	bcopy((char *)resolveHost(buf), (char *) &rmtAddr, sizeof(rmtAddr));
	rmtAddr.sin_addr = inet_makeaddr(inet_netof(rmtAddr.sin_addr),
					broadAddr());
#endif	MULTICAST
	rmtAddr.sin_port = mazePort;

	setupSurvey(b);
	ConvertOutgoing(b);
	if (sendto(fd, (char *) b, sizeof(*b), 0, &rmtAddr, sizeof(rmtAddr)) < 0)
		MFError("survey");

	while (1) {
		fds = (1<<fd);
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;
		ret = select(32, &fds, NULL, NULL, &timeout);
		if (ret < 0)
			MFError("select");
		if (ret == 0)
			break;
		answer.srclen = sizeof(Sockaddr);
		cc = recvfrom(fd, &answer.packet, sizeof(RatPacket),
			0, &answer.source, &answer.srclen);
		if (cc < 0)
			MFError("recv");
		ConvertIncoming(&answer.packet);
		if (answer.packet.type != RAT_STATUS)
			continue;
		if (!duke(&answer)) {
#ifndef	MULTICAST
			RatStatus	rs;

			rs = (RatStatus) &answer.packet.body;
			setupSurvey(b);
			ConvertOutgoing(b);
			if (rs->rats[rs->dukeRat].addr.sin_port == mazePort)
				continue;	/* will answer himself */
				
			if (sendto(fd, b, sizeof(*b), 0,
				   &rs->rats[rs->dukeRat].addr,
				   sizeof(rs->rats[rs->dukeRat].addr)) < 0)
				MFError("resend");
			continue;
#endif	MULTICAST
		} else
			printAnswer(&answer);
	}
	if (FoundOne == FALSE)
		printf("No one's playing now\n");
	exit(0);
}

/*
 * figure out the right broadcast address to use
 */
unsigned long
broadAddr()
{
#ifdef 	SIOCGIFBRDADDR
    	int s, count;
	unsigned long myaddr;
	char buf[BUFSIZ];
	struct hostent *hp;
	struct ifreq *ifrp;
	struct ifconf ifc;
	struct sockaddr_in *sin;
	
	s = socket(AF_INET, SOCK_DGRAM, 0);
	
	gethostname(buf, sizeof(buf)-1);
	if ((hp = gethostbyname(buf)) != NULL) {
	    bcopy(hp->h_addr, (char *)&myaddr, sizeof(myaddr));
	} else {
	    fprintf(stderr, "Can't get my address\n");
	    return (BROAD_ADDR);
	}

	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if (ioctl(s, SIOCGIFCONF, (char *)&ifc) < 0) {
	    perror("ioctl (get interface configuration)");
	    return (BROAD_ADDR);
	}

	ifrp = ifc.ifc_req;
	count = ifc.ifc_len/sizeof(struct ifreq);
	while (--count >= 0) {
	    sin = (struct sockaddr_in *)&ifrp->ifr_addr;
	    if (sin->sin_addr.s_addr == myaddr) break;
	    ifrp++;
	}
	
	if (count < 0) {
	    fprintf(stderr, "Primary interface not found.\n");
	    return (BROAD_ADDR);
	}

	if (ioctl(s, SIOCGIFFLAGS, (caddr_t)ifrp) == -1) {
	    perror("ioctl (get interface flags)");
	    return (BROAD_ADDR);
	}

	if ((ifrp->ifr_flags & IFF_BROADCAST) == 0) {
	    fprintf(stderr, "Primary interface not broadcast capable.\n");
	    return (BROAD_ADDR);
	}

	if (ioctl(s, SIOCGIFBRDADDR, (caddr_t)ifrp) == -1) {
	    perror("ioctl (get interface broadcast addr)");
	    return (BROAD_ADDR);
	}

	return (sin->sin_addr.s_addr);
#else
    	return (BROAD_ADDR);
#endif	/* SIOCGIFBRDADDR */
}

MFError(s)
char *s;
{
	fprintf(stderr, "mazefind: %s\n", s);
	perror("mazefind");
	exit(-1);
}

/* 
 * Fill in the RAT_SURVEY packet.
 */

setupSurvey(b)
RatPacket *b;
{
	RatNew	ratSurvey;

	b->type = RAT_SURVEY;
	ratSurvey = (RatNew) &b->body;
	ratSurvey->pass = RAT_PASSWORD;
}

/* 
 * Check if this guy's the duke of his game.
 */

Boolean
duke(a)
struct answer	*a;
{
	RatStatus	test;

	test = (RatStatus) &a->packet.body;
	return !bcmp((char *) &test->rats[test->dukeRat].addr.sin_addr,
			(char *) &a->source.sin_addr, sizeof(struct in_addr));
}

/* 
 * Decode an answer packet.
 */

printAnswer(a)
struct answer	*a;
{
	RatStatus	status;
	RatId		ratId;
	RatInfo		rat;
	struct hostent	*host;

	status = (RatStatus) &a->packet.body;
	
	if (FoundOne == FALSE) {
		FoundOne = TRUE;
		printf("Current game(s):\n");
		printf("\t%-20s\t%s\t\thost\n", "Name", "score");
	} else
		printf("\n");
	for(ratId = 0; ratId < MAXRATS; ratId++) {
		rat = (RatInfo) &status->rats[ratId];
		if (rat->playing == FALSE)
			continue;

		if (ratId == status->dukeRat)
			printf("duke");
		printf("\t");

		printf("%-20s\t%d\t\t", rat->name, rat->score);

		host = gethostbyaddr(&rat->addr.sin_addr,
				sizeof(struct in_addr), AF_INET);
		if (host != NULL)
			printf("%s\n", host->h_name);
		else
			printf("%s\n", inet_ntoa(rat->addr.sin_addr));
	}
}

/*
 * Resolve the specified host name into an internet address.  The "name" may
 * be either a character string name, or an address in the form a.b.c.d where
 * the pieces are octal, decimal, or hex numbers.  Returns a pointer to a
 * sockaddr_in struct (note this structure is statically allocated and must
 * be copied), or NULL if the name is unknown.
 */

Sockaddr *
resolveHost(name)
register char *name;
{
	register struct hostent *fhost;
	struct in_addr fadd;
	static Sockaddr sa;

	if ((fhost = gethostbyname(name)) != NULL) {
		sa.sin_family = fhost->h_addrtype;
		sa.sin_port = 0;
		bcopy(fhost->h_addr, &sa.sin_addr, fhost->h_length);
	} else {
		fadd.s_addr = inet_addr(name);
		if (fadd.s_addr != -1) {
			sa.sin_family = AF_INET;	/* grot */
			sa.sin_port = 0;
			sa.sin_addr.s_addr = fadd.s_addr;
		} else
			return(NULL);
	}
	return(&sa);
}

/* 
 * Convert the contents of a packet to network order before sending.
 */

ConvertOutgoing(p)
RatPacket *p;
{
	char		buf[64];
	RatId		ratId;
	RatLocation	ratLoc;
	RatKill		ratKill;
	RatDead		ratDead;
	RatStatus	ratStatus;
	RatNew		ratNew;
	RatGone		ratGone;
	RatQuery	ratQuery;
	RatAlive	ratAlive;
	RatMove		ratMove;

	switch(p->type) {
	case RAT_LOCATION:
		ratLoc = (RatLocation) &p->body;
		ratLoc->ratId = htonl(ratLoc->ratId);
		ratLoc->xLoc =  htons(ratLoc->xLoc);
		ratLoc->yLoc =  htons(ratLoc->yLoc);
		ratLoc->dir =   htons(ratLoc->dir);
		ratLoc->score = htonl(ratLoc->score);
		break;

	case RAT_KILL:
		ratKill = (RatKill) &p->body;
		ratKill->ratId = htonl(ratKill->ratId);
		ratKill->xLoc  = htons(ratKill->xLoc);
		ratKill->yLoc  = htons(ratKill->yLoc);
		ratKill->dir   = htons(ratKill->dir);
		break;

	case RAT_DEAD:
		ratDead = (RatDead) &p->body;
		ratDead->ratId = htonl(ratDead->ratId);
		ratDead->killedBy = htonl(ratDead->killedBy);
		break;

	case RAT_STATUS:
		ratStatus = (RatStatus) &p->body;
		ratStatus->dukeRat = htonl(ratStatus->dukeRat);
		for (ratId = 0; ratId < MAXRATS; ratId++) {
			RatInfo	ratInfo;

			ratInfo = &ratStatus->rats[ratId];
			ratInfo->playing = htons(ratInfo->playing);
			ratInfo->xLoc = htons(ratInfo->xLoc);
			ratInfo->yLoc = htons(ratInfo->yLoc);
			ratInfo->dir = htons(ratInfo->dir);
			ratInfo->score = htonl(ratInfo->score);
			ratInfo->addr.sin_family =
				ntohs(ratInfo->addr.sin_family);
			/* don't touch address or name */
		}
		break;

	case RAT_NEW:
		ratNew = (RatNew) &p->body;
		ratNew->pass = htons(ratNew->pass);
		ratNew->xLoc = htons(ratNew->xLoc);
		ratNew->yLoc = htons(ratNew->yLoc);
		ratNew->dir  = htons(ratNew->dir);
		ratNew->addr.sin_family =
			htons(ratNew->addr.sin_family);
		/* don't touch address or name */
		break;

	case RAT_GOING:
		ratGone = (RatGone) &p->body;
		ratGone->ratId = htonl(ratGone->ratId);
		break;

	case RAT_QUERY:
		ratQuery = (RatQuery) &p->body;
		ratQuery->ratId = htonl(ratQuery->ratId);
		break;

	case RAT_ALIVE:
		ratAlive = (RatAlive) &p->body;
		ratAlive->ratId = htonl(ratAlive->ratId);
		break;

	case RAT_SURVEY:
		ratNew = (RatNew) &p->body;
		ratNew->pass = htons(ratNew->pass);
		ratNew->xLoc = htons(ratNew->xLoc);
		ratNew->yLoc = htons(ratNew->yLoc);
		ratNew->dir  = htons(ratNew->dir);
		/* don't touch address or name */
		break;

	case RAT_MOVE:
		ratMove = (RatMove) &p->body;
		ratMove->ratId = htonl(ratMove->ratId);
		break;

	default:
		sprintf(buf, "ConvertOutgoing bad type %d (%d)",
			p->type, htons(p->type));
		MFError(buf);
	}
	p->type = htonl(p->type);
}

/* 
 * Convert the contents of a packet to host order after ConvertIncoming.
 */

ConvertIncoming(p)
RatPacket *p;
{
	char		buf[64];
	RatId		ratId;
	RatLocation	ratLoc;
	RatKill		ratKill;
	RatDead		ratDead;
	RatStatus	ratStatus;
	RatNew		ratNew;
	RatGone		ratGone;
	RatQuery	ratQuery;
	RatAlive	ratAlive;
	RatMove		ratMove;

	p->type = ntohl(p->type);
	switch(p->type) {
	case RAT_LOCATION:
		ratLoc = (RatLocation) &p->body;
		ratLoc->ratId = ntohl(ratLoc->ratId);
		ratLoc->xLoc =  ntohs(ratLoc->xLoc);
		ratLoc->yLoc =  ntohs(ratLoc->yLoc);
		ratLoc->dir =   ntohs(ratLoc->dir);
		ratLoc->score = ntohl(ratLoc->score);
		break;

	case RAT_KILL:
		ratKill = (RatKill) &p->body;
		ratKill->ratId = ntohl(ratKill->ratId);
		ratKill->xLoc  = ntohs(ratKill->xLoc);
		ratKill->yLoc  = ntohs(ratKill->yLoc);
		ratKill->dir   = ntohs(ratKill->dir);
		break;

	case RAT_DEAD:
		ratDead = (RatDead) &p->body;
		ratDead->ratId = ntohl(ratDead->ratId);
		ratDead->killedBy = ntohl(ratDead->killedBy);
		break;

	case RAT_STATUS:
		ratStatus = (RatStatus) &p->body;
		ratStatus->dukeRat = ntohl(ratStatus->dukeRat);
		for (ratId = 0; ratId < MAXRATS; ratId++) {
			RatInfo	ratInfo;

			ratInfo = &ratStatus->rats[ratId];
			ratInfo->playing = ntohs(ratInfo->playing);
			ratInfo->xLoc = ntohs(ratInfo->xLoc);
			ratInfo->yLoc = ntohs(ratInfo->yLoc);
			ratInfo->dir = ntohs(ratInfo->dir);
			ratInfo->score = ntohl(ratInfo->score);
			ratInfo->addr.sin_family =
				ntohs(ratInfo->addr.sin_family);
			/* don't touch address or name */
		}
		break;

	case RAT_NEW:
		ratNew = (RatNew) &p->body;
		ratNew->pass = ntohs(ratNew->pass);
		ratNew->xLoc = ntohs(ratNew->xLoc);
		ratNew->yLoc = ntohs(ratNew->yLoc);
		ratNew->dir  = ntohs(ratNew->dir);
		ratNew->addr.sin_family =
			ntohs(ratNew->addr.sin_family);
		/* don't touch address or name */
		break;

	case RAT_GOING:
		ratGone = (RatGone) &p->body;
		ratGone->ratId = ntohl(ratGone->ratId);
		break;

	case RAT_QUERY:
		ratQuery = (RatQuery) &p->body;
		ratQuery->ratId = ntohl(ratQuery->ratId);
		break;

	case RAT_ALIVE:
		ratAlive = (RatAlive) &p->body;
		ratAlive->ratId = ntohl(ratAlive->ratId);
		break;

	case RAT_SURVEY:
		ratNew = (RatNew) &p->body;
		ratNew->pass = ntohs(ratNew->pass);
		ratNew->xLoc = ntohs(ratNew->xLoc);
		ratNew->yLoc = ntohs(ratNew->yLoc);
		ratNew->dir  = ntohs(ratNew->dir);
		/* don't touch address or name */
		break;

	case RAT_MOVE:
		ratMove = (RatMove) &p->body;
		ratMove->ratId = ntohl(ratMove->ratId);
		break;

	default:
		sprintf(buf, "ConvertIncoming bad type %d (%d)",
			p->type, ntohs(p->type));
		MFError(buf);
	}
}

