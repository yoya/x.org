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
#include <stdio.h>
#include <sys/types.h>         /* needed by sys/socket.h and netinet/in.h */
#include <sys/uio.h>           /* for struct iovec, used by socket.h */
#include <sys/socket.h>        /* for AF_INET, SOCK_STREAM, ... */
#include <sys/ioctl.h>         /* for FIONCLEX, FIONBIO, ... */
#ifdef SYSV
#include <sys/fcntl.h>
#ifndef FD_CLOEXEC
#define FD_CLOEXEC 1
#endif
#endif
#include <netinet/in.h>        /* struct sockaddr_in */
#include <netdb.h>             /* struct servent * and struct hostent * */
#include <errno.h>             /* for EINTR, EADDRINUSE, ... */
#include <stdlib.h>
#include <X11/X.h>
#include "xmovectrl.h"

char *XmoveDisplay;
char *XmoveFullDisplay;
int XmovePort;
int XmoveFd;
Bool littleEndian;
int auth_namelen, auth_datalen;
char *auth_name, *auth_data;

main(int argc, char **argv, char **envp)
{
     char **cur_argv = argv + 1;
     int num_args = argc - 1;
     
     if (argc == 1) {
	  usage(*argv);
	  return -1;
     }

     SetlittleEndian();
     
     if (**(cur_argv) == '-')       /* get environment variable DISPLAY */
	  XmoveDisplay = getenv("DISPLAY");
     else {                           /* get it from first argument */
	  XmoveDisplay = *(cur_argv++);
	  num_args--;
     }

     if (XmoveDisplay == NULL) {
	  printf("%s: xmove display must be specified\n", *argv);
	  usage(*argv);
	  return -1;
     }
     
     XmoveFullDisplay = strdup(XmoveDisplay);
     XmovePort = ParseDisplay(XmoveDisplay); /* replaces ':' with '\0' */
     
     if (XmovePort == -1) {
	  printf("%s: Unable to parse xmove address: %s\n",*argv,XmoveDisplay);
	  return -1;
     }

     XmoveFd = ConnectToServer();
     DoServerSetup(XmoveFd);
     
     if (strcmp(*cur_argv, "-moveall") == 0)
	  return (Ctrl_moveall(--num_args, ++cur_argv));
	       
     else if (strcmp(*cur_argv, "-list") == 0)
	  return (Ctrl_list(--num_args, ++cur_argv));

     else if (strcmp(*cur_argv, "-move") == 0)
	  return (Ctrl_move(--num_args, ++cur_argv));
     
     else if (strcmp(*cur_argv, "-quit") == 0)
	  return (Ctrl_quit(--num_args, ++cur_argv));
     
     else {
	  printf("%s: Unknown command: %s\n", *argv, *cur_argv);
	  return -1;
     }
}


ConnectToServer()
{
    int ServerFD;
    struct sockaddr_in  sin;
    struct hostent *hp;
    
    /* establish a socket to the name server for this host */
    bzero((char *)&sin, sizeof(sin));
    ServerFD = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerFD < 0)
    {
	perror("socket() to Server failed");
	exit(-1);
    }

    (void) setsockopt(ServerFD, SOL_SOCKET, SO_REUSEADDR,  (char *) NULL, 0);
    (void) setsockopt(ServerFD, SOL_SOCKET, SO_USELOOPBACK,(char *) NULL, 0);

    hp = gethostbyname(XmoveDisplay);
    if (hp == 0)
    {
	perror("gethostbyname failed");
	exit(-1);
    }

    sin.sin_family = AF_INET;
    bcopy((char *)hp->h_addr, (char *)&sin.sin_addr, hp->h_length);
    sin.sin_port = htons (6000 + XmovePort);

    /* ******************************************************** */
    /* try to connect to Server */
    
    if (connect(ServerFD, (struct sockaddr *)&sin, sizeof(sin)) < 0)
    {
	if (errno != 0) {
	    perror("connect");
	    exit(-1);
       }
    }

    return(ServerFD);
}     


DoServerSetup(int server_fd)
{
     unsigned char iobuf[512];
     int send_len, typelen, keylen;

     if (littleEndian)
	  ISetByte(&iobuf[0], 0x6C);
     else
	  ISetByte(&iobuf[0], 0x42);
     
     ISetShort(&iobuf[2], 0xFE);
     ISetShort(&iobuf[4], 0);
     ISetShort(&iobuf[6], 0);
     ISetShort(&iobuf[8], 0);
     ISetShort(&iobuf[10], 11);

     /* we assume that only MIT-MAGIC-COOKIE-1 will be used, which is 18
	characters long and pads to 20. Just laziness. If we add new support
	we'll change the assumptions made below. */

     send_len = 12;
     if (getauthkey(XmoveFullDisplay, (char *)iobuf+12, &typelen, (char *)iobuf+32, &keylen) != -1) {
	  ISetShort(&iobuf[6], 18);
	  send_len += 20;
	  ISetShort(&iobuf[8], keylen);
	  send_len += pad(keylen);
     }

     SendBuffer(server_fd, iobuf, send_len);
     ReceiveBuffer(server_fd, iobuf, 1); /* 1==success, 0==not-authorized */
     if (iobuf[0] != 1) {
	  fprintf(stderr, "Not authorized to connect with xmove\n");
	  exit(-1);
     }
     return;
}

Ctrl_moveall(num_args, my_argv)
int num_args;
char **my_argv;
{
     unsigned char buf[256];
     char *screen_str = my_argv[2], screen;
     char authtype[128], authkey[128];
     int authtypelen, authkeylen;
     int newdisp_len;
     int newdisp_pad;
     int replylen;
     
     if (num_args != 1 && (num_args != 3 || strcmp(my_argv[1], "-screen") != 0)) {
	  printf("moveall option expects one argument, plus optional screen\n");
	  return -1;
     }
	 
     if (num_args == 3) {
	  screen = strtol(screen_str, &screen_str, 10);
	  if (screen_str == my_argv[2]) { /* ie. argument wasn't a number */
	       printf("screen must be a number.\n");
	       return -1;
	  }
     } else {
	  screen = -1;
     }
     
     newdisp_len = strlen(*my_argv) + 1;
     
     newdisp_pad = pad(newdisp_len);
     
     ISetShort(&buf[0], 0); /* command: MoveAll */
     ISetShort(&buf[2], newdisp_len);
     if (screen == -1)
	  ISetShort(&buf[4], 0);
     else
	  ISetShort(&buf[4], 1);
     ISetShort(&buf[6], 0);
     ISetShort(&buf[8], 0);
     
     if (move_getauthkey(*my_argv, authtype, &authtypelen, authkey, &authkeylen) != -1)
	  ISetShort(&buf[6], authkeylen); /* note we don't transmit authtype */
     
     SendBuffer(XmoveFd, (char *)buf, 10);
     SendBuffer(XmoveFd, *my_argv, newdisp_len);
     SendBuffer(XmoveFd, buf, newdisp_pad - newdisp_len);/* send pad garbage */
     if (screen != -1) {
	  SendBuffer(XmoveFd, &screen, 1);
	  SendBuffer(XmoveFd, buf, 3);
     }

     if (IShort(&buf[6]))
	  SendBuffer(XmoveFd, (unsigned char *)authkey, authkeylen);
     
     ReceiveBuffer(XmoveFd, &buf[0], 4);
     replylen = ILong(&buf[0]);
     if (replylen && replylen < 256) {
	  ReceiveBuffer(XmoveFd, &buf[0], replylen);
	  buf[replylen] = '\0';
	  printf("%s", buf);
     }
     
     return 0;
}
     
Ctrl_move(num_args, my_argv)
int num_args;
char **my_argv;
{
     unsigned char buf[256];
     char *screen_str = my_argv[2], screen, **cur_argv = my_argv;
     char authtype[128], authkey[128];
     int authtypelen, authkeylen;
     int newdisp_len;
     int newdisp_pad;
     int replylen;

     if (num_args < 2) {
	  printf("move option expects at least two arguments\n");
	  return -1;
     }

     if (strcmp(my_argv[1], "-screen") == 0) {
	  if (num_args < 4) {
	       printf("insufficient arguments. must specify new display, new screen and at least one client\n");
	       return -1;
	  }

	  screen = strtol(screen_str, &screen_str, 10);
	  if (screen_str == my_argv[2]) { /* ie. argument wasn't a number */
	       printf("screen must be a number.\n");
	       return -1;
	  }
	  
	  num_args -= 2;
	  cur_argv += 2;
     } else
	  screen = -1;

     newdisp_len = strlen(*my_argv) + 1;
     newdisp_pad = pad(newdisp_len);

     ISetShort(&buf[0], 2); /* command: Move */
     ISetShort(&buf[2], newdisp_len);
     if (screen == -1)
	  ISetShort(&buf[4], 0);
     else
	  ISetShort(&buf[4], 1);
     ISetShort(&buf[6], --num_args * 4);
     ISetShort(&buf[8], 0);

     if (move_getauthkey(*my_argv, authtype, &authtypelen, authkey, &authkeylen) != -1)
	  ISetShort(&buf[8], authkeylen); /* note we don't transmit authtype */
     
     SendBuffer(XmoveFd, (char *)buf, 10);
     SendBuffer(XmoveFd, *my_argv, newdisp_len);
     SendBuffer(XmoveFd, buf, newdisp_pad - newdisp_len);/* send pad garbage */
     if (screen != -1) {
	  SendBuffer(XmoveFd, &screen, 1);
	  SendBuffer(XmoveFd, buf, 3);
     }

     while (num_args--) {
	  ISetLong(buf, atoi(*++cur_argv));
	  SendBuffer(XmoveFd, (char *)buf, 4);
     }

     if (IShort(&buf[8]))
	  SendBuffer(XmoveFd, (unsigned char *)authkey, authkeylen);
     
     ReceiveBuffer(XmoveFd, &buf[0], 4);
     replylen = ILong(&buf[0]);
     if (replylen && replylen < 256) {
	  ReceiveBuffer(XmoveFd, &buf[0], replylen);
	  buf[replylen] = '\0';
	  printf("%s\n", buf);
     }
     
     return 0;
}
     
Ctrl_list(num_args, my_argv)
int num_args;
char **my_argv;
{
     unsigned long num_clients;
     unsigned char buf[80];
     
     if (num_args != 0) {
	  printf("list option expects no arguments\n");
	  return -1;
     }

     ISetShort(&buf[0], 1); /* command: List */
     ISetShort(&buf[2], 0);
     ISetShort(&buf[4], 0);
     ISetShort(&buf[6], 0);
     ISetShort(&buf[8], 0);

     SendBuffer(XmoveFd, (char *)buf, 10);
     ReceiveBuffer(XmoveFd, (char *)&num_clients, sizeof(unsigned long));
     num_clients = ILong((unsigned char *)&num_clients); /* convert endianisms */
     
     while (num_clients--) {
	  ReceiveBuffer(XmoveFd, (char *)buf, 80);
	  printf("%.80s", buf);
     }

     return 0;
}
     
Ctrl_quit(num_args, my_argv)
int num_args;
char **my_argv;
{
     unsigned char buf[10];
     
     if (num_args != 0) {
	  printf("quit expects no arguments\n");
	  return -1;
     }

     while (1) {
	  printf("Confirm: Tell xmove at %s:%d to quit? ", XmoveDisplay, XmovePort);
	  fflush(stdout);
	  if (scanf("%10s", &buf[0]) == EOF)
	       return -1;
	  if (buf[0] == 'n' || buf[0] == 'N')
	       return 10;
	  if (buf[0] == 'y' || buf[0] == 'Y')
	       break;
     }

     ISetShort(&buf[0], 3); /* command: Quit */
     ISetShort(&buf[2], 0);
     ISetShort(&buf[4], 0);
     ISetShort(&buf[6], 0);
     ISetShort(&buf[8], 0);

     SendBuffer(XmoveFd, (char *)buf, 10);
     ReceiveBuffer(XmoveFd, &buf[0], 1);
     return 0;
}
     
ParseDisplay(char *display_name)
{
     char *colon = strchr(display_name, ':');

     if (!colon)
	  return -1;

     *(colon++) = '\0';

     return atoi(colon);
}

usage(char *appname)
{
     printf("%s: usage: xmovectrl [xmove_display_name] -command parameter...\n",
	    appname);
}


/* the following set of routines change values within a buffer.  They */
/* are used when we need to change the value of something within an */
/* X11 request.  A routine is available for each of the major types */
/* (long, short, byte, and Bool). */

Global void
ISetLong(buf, value)
unsigned char buf[];
unsigned long value;
{
     *((unsigned long *)&buf[0]) = value;
}

Global void 
ISetShort(unsigned char buf[], unsigned short value)
{
     *((unsigned short *)&buf[0]) = value;
}

Global void
ISetByte(unsigned char buf[], unsigned char value)
{
    buf[0] = value;
}

Global void
ISetBool(unsigned char buf[], Bool value)
{
    buf[0] = value;
}

Global unsigned long
ILong (buf)
unsigned char   buf[];
{
    return(*(unsigned long *)&buf[0]);
}

Global unsigned short
IShort (buf)
unsigned char   buf[];
{
        return(*(unsigned short *)&buf[0]);
}

Global unsigned short
IByte (buf)
unsigned char   buf[];
{
	return(buf[0]);
}

Global Bool
IBool(buf)
		unsigned char   buf[];
{
	if (buf[0] != 0)
		return(True);
	else
		return(False);
}

SetlittleEndian()
{
     int foo = 1;

     if (((char *)&foo)[0] == 1)
	  littleEndian = True;
     else
	  littleEndian = False;
}


Bool
SendBuffer(int fd, char *buf, int BytesToWrite)
{
    int			num_written;

    if (BytesToWrite > 0)
    {
	do
	{
	    num_written = write (fd, buf, BytesToWrite);
	    if (num_written > 0)
	    {
		BytesToWrite -= num_written;
		buf += num_written;
	    }
	}
	while (BytesToWrite > 0 && (num_written > 0 || errno == EWOULDBLOCK ||
				    errno == EINTR || errno == EAGAIN));
	
	if (BytesToWrite > 0)
	    /* could not send request to server.  What should we do? */
	  return False;
    }
    return True;
}

Bool
ReceiveBuffer(int fd, char *buf, int BytesToRead)
{
    int			num_read;

    if (BytesToRead > 0)
    {
	/* loop until we get all the bytes promised us */
	do
	{
	    num_read = read (fd, buf, BytesToRead);
	    if (num_read > 0)
	    {
		BytesToRead -= num_read;
		buf += num_read;
	    }
	}
	while (BytesToRead > 0);
    }
    return True;
}


/*
 * pad:
 * 
 * we will need to be able to interpret the values stored in the requests
 * as various built-in types.  The following routines support the types
 * built into X11
 */
Global int
pad (n)
		long    n;
{
	/* round up to next multiple of 4 */
	return((n + 3) & ~0x3);
}


int getauthkey(char *server, char *authtype, int *authtypelen, char *authkey, int *authkeylen)
{
     char shellcmd[128];
     char authhex[128];
     char *curauthhex = authhex;
     FILE *xauth;

     sprintf(shellcmd, "xauth list %s", server);
     if (!(xauth = popen(shellcmd, "r")))
	  return -1;

     if (fscanf(xauth, "%*s %127s %127s", authtype, authhex) != 2 ||
	 strcmp(authtype, "MIT-MAGIC-COOKIE-1") != 0)
     {
	  (void)pclose(xauth);
	  return -1;
     }

     *authtypelen = 18;
     
     while (*curauthhex != '\0') {
	  int newval;

	  sscanf(curauthhex, "%2x", &newval);
	  curauthhex += 2;
	  *(unsigned char *)authkey = (unsigned char)newval;
	  authkey++;
     }
     *authkeylen = (curauthhex - authhex) >> 1;

     pclose(xauth);
     return 0;
}

     
int move_getauthkey(char *server, char *authtype, int *authtypelen, char *authkey, int *authkeylen)
{
     char *colon = strchr(server, ':');
     char newserver[256];

     if (colon)
	  return getauthkey(server, authtype, authtypelen, authkey, authkeylen);

     sprintf(newserver, "%s:1", server);
     if (getauthkey(newserver, authtype, authtypelen, authkey, authkeylen) == 0)
	  return 0;

     sprintf(newserver, "%s:0", server);
     return getauthkey(newserver, authtype, authtypelen, authkey, authkeylen);
}
