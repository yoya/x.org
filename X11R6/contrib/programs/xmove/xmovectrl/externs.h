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
#ifndef EXTERNS_H
#define EXTERNS_H

#include <sys/time.h>
#include <sys/socket.h>

/* stupid BS compiler doesn't know these */

/*Global void perror P((char *));

Global int printf P((char *, ...));
Global int fprintf P((FILE *, char *, ...));
Global int sscanf P((char *, char *, ...)); */

/*Global void exit P((int));*/
Global int close P((int));
/*Global void fflush P((FILE *));*/
Global int atoi P((const char *str));
Global int gethostname P((char *name, int namelen));
int socket P((int domain, int type, int protocol));
/*Global int setsockopt P((int s, int level, int optname, char *optval, int optlen));*/
Global int  bind P((int s, struct sockaddr *name, int namelen));
Global int listen P((int s, int backlog));
Global int ioctl P((int fd, int request, caddr_t arg));
Global int select P((int width,
		     fd_set *readfds,
		     fd_set *writefds, 
		     fd_set *exceptfds, 
		     struct timeval *timeout));
Global int read P((int fd, char *buf, int nbyte));
Global int write P((int fd, char *buf, int nbyte));
Global int accept P((int s, struct sockaddr *addr, int *addrlen));
Global int connect P((int s, struct sockaddr *name, int namelen));

#include <malloc.h>

/*Global void *malloc P((int size));
Global void *realloc P((char *ptr, int size));
Global void *calloc P((int nelem, int elsize));
Global void free P((char *ptr));*/
Global int gettimeofday P((struct timeval *tp, struct timezone *tzp));
Global int getdtablesize P((void));

#endif /* EXTERNS_H */
