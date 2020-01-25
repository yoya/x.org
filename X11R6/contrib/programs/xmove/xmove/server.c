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
 * File: server.c
 *
 * Description: Code to decode and print X11 protocol

 * The possible values for ByteProceessing are prototyped below.
 * These are the common values for ByteProcessing:
 *   For clients:
 *	    StartSetUpMessage
 *	    FinishSetUpMessage
 *	    ClientRequest
 *   For servers:
 *	    StartSetUpReply
 *	    FinishSetUpReply
 *	    ServerPacket
 *
 */

#define NEED_REPLIES

#include <sys/time.h>          /* for struct timeval * */
#include <errno.h>             /* for EINTR, EADDRINUSE, ... */
#include <X11/Xproto.h>
#include <malloc.h>

#include "xmove.h"
#include "select_args.h"

/* function prototypes: */
/* server.c: */

/* possible values for ByteProcesssing: */

static int StartSetUpMessage P((Pointer private_data , unsigned char *buf , long n ));
static int FinishSetUpMessage P((Pointer private_data , unsigned char *buf , long n ));
static int ClientRequest P((Pointer private_data , unsigned char *buf, long n ));
static int StartSetUpReply P((Pointer private_data , unsigned char *buf , long n ));
static int FinishSetUpReply P((Pointer private_data , unsigned char *buf , long n ));
static int ServerPacket P((Pointer private_data , unsigned char *buf, long n ));
static int XmoveCtrlMoveAll P((Pointer private_data, unsigned char *buf, long n));
static int XmoveCtrlList P((Pointer private_data, unsigned char *buf, long n));
static int XmoveCtrlQuit P((Pointer private_data, unsigned char *buf, long n));
static int XmoveCtrlMove P((Pointer private_data, unsigned char *buf, long n));
static int XmoveCtrlRequest P((Pointer private_data, unsigned char *buf, long n));
static int XmoveCtrlStartSetUp P((Pointer private_data, unsigned char *buf, long n));
static int XmoveCtrlFinishSetUp P((Pointer private_data, unsigned char *buf, long n));
static int XmoveMovedClientSetUp P((Pointer private_data, unsigned char *buf, long n));
static int XmoveMovedClientSetUp2 P((Pointer private_data, unsigned char *buf, long n));
static int XmoveMovedClientGetName P((Pointer private_data, unsigned char *buf, long n));
static int MoveWindowRequest1 P((Server *server, unsigned char *buf, long n));
static int MoveWindowRequest2 P((Pointer private_data, unsigned char *buf, long n));
static int IgnoreServerBytes P((Pointer private_data, unsigned char *buf, long n));
static int IgnoreClientBytes P((Pointer private_data, unsigned char *buf, long n));

/* other prototypes: */

static int verify_conn P((int fd, unsigned long ip_addr, int typelen, unsigned char *type, int keylen, unsigned char *key));

/* end function prototypes */

extern Bool					ignore_bytes;
extern LinkList                                 meta_client_list;
extern LinkList	                                client_list;
extern int					CurrentVerbose;
extern int					ErrorVerbose;
extern Bool					littleEndian;
extern long                                     ReadDescriptors[mskcnt];

static long ZeroTime1 = -1;
static long ZeroTime2 = -1;
static struct timeval   tp;
static xConnSetupPrefix NoAuthReply;
static char *NoAuthError = "Client is not authorized to connect to Server  ";

/*
 * print the time since we started in hundredths (1/100) of seconds
 */

Global void
PrintTime()
{
	static long lastsec = 0;
	long    sec /* seconds */ ;
	long    hsec /* hundredths of a second */ ;

	gettimeofday(&tp, (struct timezone *)NULL);
	if (ZeroTime1 == -1 || (tp.tv_sec - lastsec) >= 1000)
		{
			ZeroTime1 = tp.tv_sec;
			ZeroTime2 = tp.tv_usec / 10000;
		}

	lastsec = tp.tv_sec;
	sec = tp.tv_sec - ZeroTime1;
	hsec = tp.tv_usec / 10000 - ZeroTime2;
	if (hsec < 0)
		{
			hsec += 100;
			sec -= 1;
		}
	fprintf(stdout, "%2d.%02ld: ", sec, hsec);
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
     if (littleEndian == XmoveLittleEndian && (((int)buf)&3) == 0) {
	  *(unsigned long *)buf = value;
	  return;
     }
     
    if(littleEndian)
    {
	buf[0] = value & 0xff;
	buf[1] = (value >> 8) & 0xff;
	buf[2] = (value >> 16) & 0xff;
	buf[3] = (value >> 24) & 0xff;
    }
    else
    {
	buf[0] = (value >> 24) & 0xff;
	buf[1] = (value >> 16) & 0xff;
	buf[2] = (value >> 8) & 0xff;
	buf[3] = value & 0xff;
    }
}

Global void 
ISetShort(unsigned char buf[], unsigned short value)
{
     if (littleEndian == XmoveLittleEndian && (((int)buf)&1) == 0) {
	  *(unsigned short *)buf = value;
	  return;
     }
     
    if(littleEndian)
    {
	buf[1] = (value >> 8) & 0xff;
	buf[0] = value & 0xff;
    }
    else
    {
	buf[0] = (value >> 8) & 0xff;
	buf[1] = value & 0xff;
    }
}

/*Global void
ISetByte(unsigned char buf[], unsigned char value)
{
    buf[0] = value;
}

Global void
ISetBool(unsigned char buf[], Bool value)
{
    buf[0] = value;
}*/

Global unsigned long
ILong (buf)
unsigned char *buf;
{
     if (littleEndian == XmoveLittleEndian && (((int)buf)&3) == 0)
	  return (*((unsigned long *)buf));
     
    if (littleEndian)
	return((((((buf[3] << 8) | buf[2]) << 8) | buf[1]) << 8) | buf[0]);
    return((((((buf[0] << 8) | buf[1]) << 8) | buf[2]) << 8) | buf[3]);
}

Global unsigned short
IShort (buf)
unsigned char   buf[];
{
     if (littleEndian == XmoveLittleEndian && (((int)buf)&1) == 0)
	  return *(unsigned short *)buf;
     
	if (littleEndian)
		return (buf[1] << 8) | buf[0];
	return((buf[0] << 8) | buf[1]);
}

/*Global unsigned short
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
*/

/*
 * Byte Processing Routines.  Each routine MUST set num_Needed
 * and ByteProcessing.  It probably needs to do some computation first.
 */

Global void
StartClientConnection(client)
	Client *client;
{
	/* when a new connection is started, we have no saved bytes */
	client->fdd->inBuffer.data = NULL;
	client->fdd->inBuffer.BlockSize = 0;
	client->fdd->inBuffer.num_Saved = 0;
	client->fdd->inBuffer.alt_data = NULL;
	client->fdd->inBuffer.alt_Saved = 0;
	client->fdd->inBuffer.data_offset = 0;
	client->fdd->outBuffer.data = NULL;
	client->fdd->outBuffer.BlockSize = 0;
	client->fdd->outBuffer.num_Saved = 0;
	client->fdd->outBuffer.alt_data = NULL;
	client->fdd->outBuffer.alt_Saved = 0;
	client->fdd->outBuffer.data_offset = 0;
	client->fdd->outBuffer.src_fd = client->server->fdd->fd;

	/* each new connection gets a request sequence number */
	client->SequenceNumber = 0;

	/* we need 12 bytes to start a SetUp message */
	client->fdd->ByteProcessing = StartSetUpMessage;
	client->fdd->inBuffer.num_Needed = 12;
}

#if 0

/************************************************************************************
  the server connection goes through a cycle of states:

                                                 +---<----+
                                                 |        |
                                                 |        +
                                                 V        |
  StartSetUpReply --> FinishSetUpReply --> ServerPacket --+


  The ServerPacket routine determines whether the data is a Reply, Event or Error.
  It then calls the appropriate processing routines, or just returns if we need to
  read in more data first.

  The client connection goes through the following similar cycle:

                                                    +----<-----+
                                                    |          |
                                                    V          |
  StartSetUpMessage --> FinishSetUpMessage --> ClientRequest --+

*************************************************************************************/

#endif

Global void
StartServerConnection(server)
Server *server;
{
    /* when a new connection is started, we have no saved bytes */
    server->fdd->inBuffer.data = NULL;
    server->fdd->inBuffer.BlockSize = 0;
    server->fdd->inBuffer.num_Saved = 0;
    server->fdd->inBuffer.alt_data = NULL;
    server->fdd->inBuffer.alt_Saved = 0;
    server->fdd->inBuffer.data_offset = 0;
    server->fdd->outBuffer.data = NULL;
    server->fdd->outBuffer.BlockSize = 0;
    server->fdd->outBuffer.num_Saved = 0;
    server->fdd->outBuffer.alt_data = NULL;
    server->fdd->outBuffer.alt_Saved = 0;
    server->fdd->outBuffer.data_offset = 0;
    server->fdd->outBuffer.src_fd = server->client->fdd->fd;

    /* when a new connection is started, we have no reply Queue */
    initList(&server->reply_list);
    server->formats = malloc(sizeof(ImageFormat));
    initList(&server->formats->format_list);
    initList(&server->formats->depth_list);
    
    /* we need 8 bytes to start a SetUp reply */
    server->fdd->ByteProcessing = StartSetUpReply;
    server->fdd->inBuffer.num_Needed = 8;
}

#ifdef XMOVE

/*
 * This sets up the buffer information in the server structure for
 * a new server, called from move.c.
 */

Global void
RestartServerConnection(server)
Server *server;
{
    /* when a new connection is started, we have no saved bytes */
    server->fdd->inBuffer.data = NULL;
    server->fdd->inBuffer.BlockSize = 0;
    server->fdd->inBuffer.num_Saved = 0;
    server->fdd->inBuffer.alt_data = NULL;
    server->fdd->inBuffer.alt_Saved = 0;
    server->fdd->inBuffer.data_offset = 0;
    server->fdd->outBuffer.data = NULL;
    server->fdd->outBuffer.BlockSize = 0;
    server->fdd->outBuffer.num_Saved = 0;
    server->fdd->outBuffer.alt_data = NULL;
    server->fdd->outBuffer.alt_Saved = 0;
    server->fdd->outBuffer.data_offset = 0;
    server->fdd->outBuffer.src_fd = server->client->fdd->fd;

    /* when a new connection is started, we have no reply Queue */
    initList(&server->reply_list);
    server->formats = malloc(sizeof(ImageFormat));
    initList(&server->formats->format_list);
    initList(&server->formats->depth_list);
    
    /* Since we are continuing a previously started session, we need */
    /* 32 bytes to continue */

    server->fdd->ByteProcessing = ServerPacket;
    server->fdd->inBuffer.num_Needed = 32;
}
#endif

/*
 * StartSetUpMessage:
 *
 * we need the first 12 bytes to be able to determine if, and how many,
 * additional bytes we need for name and data authorization.  However, we
 * can't process the first 12 bytes until we get all of them, so
 * return zero bytes used, and increase the number of bytes needed
 */
static int
StartSetUpMessage (private_data, buf, n)
	Pointer					private_data;
	unsigned char			*buf;
	long					n;
{
	Client					*client = (Client *) private_data;
	unsigned short				namelength;
	unsigned short				datalength;

	/* According to the X11 protocol, the client determines the */
	/* byte ordering of all values passed between the server and */
	/* the client.  The client identifies itself either is */
	/* LittleEndian (LSBFirst) or BigEndian (MSBFirst), and the */
	/* server is then responsible for talking back the same way */

	littleEndian = client->fdd->littleEndian = (buf[0] == 'l');
	client->server->fdd->littleEndian = littleEndian;

	if (IShort(&buf[2]) == 0xFE) /* signal -- xmove control message */
	     return (XmoveCtrlStartSetUp (private_data, buf, n));
	
	else if (IShort(&buf[2]) == 0xFD) /* signal -- xmove'd client */
	     return (XmoveMovedClientSetUp (private_data, buf, n));

	else {
	     namelength = IShort(&buf[6]);
	     datalength = IShort(&buf[8]);
	     client->fdd->ByteProcessing = FinishSetUpMessage;
	     client->fdd->inBuffer.num_Needed += ROUNDUP4((long)namelength) + ROUNDUP4((long)datalength);
	}
	
	return(0);
}

static int
FinishSetUpMessage (private_data, buf, n)
	Pointer					private_data;
	unsigned char				*buf;
	long					n;
{
	Client					*client = (Client *) private_data;
	int msglen = client->fdd->inBuffer.num_Needed;

	CurrentVerbose = ErrorVerbose;
	PrintSetUpMessage(buf);

	/* This is a *hack* made necessary by an evil bug in OpenWindows 3.2 and
	 * earlier which causes it to lock up once it receives a new connection
	 * until it receives the setup message, thus causing XListHosts in CheckAuth()
	 * to block. To fix this, we send the message to the server now and set
	 * ignore_bytes to True
	 */

	*ValidSetUpMessage = *buf; /* set the byte ordering info */
	SendBuffer(client->server->fdd->fd, ValidSetUpMessage, ValidSetUpMessageLen);
	ignore_bytes = True;

	/* Send client's startup message to the alternate server connection
	   as well. */
	
	SendBuffer(client->server->alt_fd, ValidSetUpMessage, ValidSetUpMessageLen);
	
	/* Verify authorization for this client to connect to default server */

	if (verify_conn(client->fdd->fd, client->ip_addr, IShort(&buf[6]), buf+12,
			IShort(&buf[8]), buf+12+ROUNDUP4(IShort(&buf[6]))) == -1)
	     return -1;

	client->startup_message = malloc(msglen);
	bcopy(buf, client->startup_message, msglen);
	client->startup_message_len = msglen;

	/* after a set-up message, we expect a string of requests */
	client->fdd->ByteProcessing = ClientRequest;
	client->fdd->inBuffer.num_Needed = 4;
	return(msglen);
}

static int
XmoveMovedClientSetUp (Pointer private_data, unsigned char *buf, long n)
{
     Client *client = (Client *) private_data;

     /* since protocol-major-version buf[2] holds the signal that
	tells xmove that this is an xmoved client, we put the version
	info in unused buf[10] and here we put it back where it belongs */
     
     ISetShort(&buf[2], IShort(&buf[10]));

     client->xmoved_from_elsewhere = True;
     client->server->fdd->pass_through = True;

     
     client->fdd->ByteProcessing = XmoveMovedClientSetUp2;
     client->fdd->inBuffer.num_Needed += ROUNDUP4((long)IShort(&buf[6])) + ROUNDUP4((long)IShort(&buf[8]));
     return 0;
}

static int
XmoveMovedClientSetUp2 (Pointer private_data, unsigned char *buf, long n)
{
     Client *client = (Client *) private_data;
     int msglen = client->fdd->inBuffer.num_Needed;

     close(server->alt_fd);
     server->alt_fd = 0;
     SendBuffer(server->fdd->fd, buf, msglen);
     
     ignore_bytes = True;
     
     /* Verify authorization for this client to connect to default server */
     
     if (verify_conn(client->fdd->fd, client->ip_addr, IShort(&buf[6]), buf+12,
		     IShort(&buf[8]), buf+12+ROUNDUP4(IShort(&buf[6]))) == -1)
	  return -1;

     /* there is one unused byte in the setup encoding at location 1
	which we are using to hold the length of the name of the
	incoming client */

     if (IByte(&buf[1])) {
	  client->fdd->ByteProcessing = XmoveMovedClientGetName;
	  client->fdd->inBuffer.num_Needed = IByte(&buf[1]);
     } else {
	  client->fdd->pass_through = True;
     }
     
     return msglen;
}

static int
XmoveMovedClientGetName (Pointer private_data, unsigned char *buf, long n)
{
     Client *client = (Client *) private_data;
     int msglen = client->fdd->inBuffer.num_Needed;

     client->window_name = malloc(msglen);
     bcopy(buf, client->window_name, msglen);

     client->fdd->pass_through = True;
     ignore_bytes = True;
}

static int
XmoveCtrlMoveAll (Pointer private_data, unsigned char *buf, long n)
{
     Client *client = (Client *) private_data;
     char *new_display = (char *)buf+10, new_screen;
     char *key, *retval = NULL;
     int msglen = client->fdd->inBuffer.num_Needed;

     /* buf[2]:		display name length
	buf[4]:		== 4 if screen is specified
	buf[6]:		length of MIT-MAGIC-COOKIE-1 key
	*/

     ignore_bytes = True;

     if (IShort(&buf[4])) {
	  new_screen = *(new_display + ROUNDUP4(IShort(&buf[2])));
	  new_display += 4;
     } else {
	  new_screen = -1;
     }
     
     key = new_display + ROUNDUP4(IShort(&buf[2]));
     MoveAll((char *)buf+10, new_screen, client->meta_client, IShort(&buf[6]), key, &retval);

     /* send something back to xmovectrl to signal completion */
     
     if (retval) {
	  int retvallen = strlen(retval);
	  u_char tmpbuf[4];
     
	  ISetLong(&tmpbuf[0], retvallen);
	  SendBuffer(client->fdd->fd, tmpbuf, 4);
	  SendBuffer(client->fdd->fd, (u_char *)retval, retvallen);
	  free(retval);
     } else {
	  int retvallen = 0;
	  SendBuffer(client->fdd->fd, (unsigned char *)&retvallen, 4);
     }
     
     client->fdd->ByteProcessing = XmoveCtrlRequest;
     client->fdd->inBuffer.num_Needed = 10;
     return(msglen);
}

static int
XmoveCtrlMove (Pointer private_data, unsigned char *buf, long n)
{
     Client *client = (Client *) private_data;
     unsigned char *cur_client_id;
     int client_count = IShort(buf+6) / 4;
     int client_id;
     char new_screen, *new_display = (char *)buf+10;
     char *key, *retval = NULL;
     int msglen = client->fdd->inBuffer.num_Needed;
     
     /* buf[2]:		display name length
	buf[4]:		== 4 if screen is specified
	buf[6]:		4 * number of clients specified
	buf[8]:		length of MIT-MAGIC-COOKIE-1 key
	*/
     
     ignore_bytes = True;

     cur_client_id = buf + 10 + ROUNDUP4(IShort(&buf[2]));
     
     if (IShort(&buf[4])) {
	  new_screen = *cur_client_id;
	  cur_client_id += 4;
     } else {
	  new_screen = -1;
     }

     key = (char *)cur_client_id + client_count*4;
     
     while (client_count--) {
	  client_id = ILong(cur_client_id);
	  cur_client_id += 4;

	  ForAllInList(&meta_client_list)
	  {
	       meta_client = (MetaClient *)CurrentContentsOfList(&meta_client_list);
	       if (meta_client->meta_id == client_id) {
		    MoveClient(new_display, new_screen, IShort(&buf[8]), key, &retval);
		    break;
	       }
	  }

	  if (retval)
	       break;
     }
	  
     /* send something back to xmovectrl to signal completion */

     if (retval) {
	  int retvallen = strlen(retval);
	  u_char tmpbuf[4];
     
	  ISetLong(&tmpbuf[0], retvallen);
	  SendBuffer(client->fdd->fd, tmpbuf, 4);
	  SendBuffer(client->fdd->fd, (u_char *)retval, retvallen);
	  free(retval);
     } else {
	  int retvallen = 0;
	  SendBuffer(client->fdd->fd, (unsigned char *)&retvallen, 4);
     }

     client->fdd->ByteProcessing = XmoveCtrlRequest;
     client->fdd->inBuffer.num_Needed = 10;
     return(msglen);
}

static int
XmoveCtrlQuit (Pointer private_data, unsigned char *buf, long n)
{
     Client *client = (Client *) private_data;
     int rd_array = 0;
     int curFD = 0;
     char retval = 0;

     ignore_bytes = True;
     while (rd_array < mskcnt) {
	  while (ReadDescriptors[rd_array]) {
	       if ((ReadDescriptors[rd_array] & 1) && (curFD != client->fdd->fd)) {
		    printf("closing %d\n", curFD);
		    close(curFD);
	       }
	       curFD++;
	       ReadDescriptors[rd_array] /= 2;
	  }

	  rd_array++;
     }

     while (write(client->fdd->fd, &retval, 1) == 0);
     exit(0);
}
     

static int
XmoveCtrlList (Pointer private_data, unsigned char *buf, long n)
{
     Client *client = (Client *) private_data;
     int num_clients = -1;	/* don't include ourselves -- start at -1 */
     unsigned char num_clients_buf[4], out_buf[80];
     int msglen = client->fdd->inBuffer.num_Needed;

     ignore_bytes = True;

     ForAllInList(&client_list)
     {
	  num_clients++;
     }

     ISetLong(num_clients_buf, num_clients);
     SendBuffer(client->fdd->fd, num_clients_buf, 4);
     
     ForAllInList(&meta_client_list) {
	  MetaClient *tmp_meta;

	  tmp_meta = (MetaClient *)CurrentContentsOfList(&meta_client_list);
	  
	  ForAllInList(&tmp_meta->client_list)
	  {
	       Client *tmp_client;
	       
	       tmp_client = (Client *)CurrentContentsOfList(&tmp_meta->client_list);
	       if (tmp_client != client) {
		    sprintf((char *)out_buf, "%-5d %-20.20s %-52.52s\n",
			    tmp_meta->meta_id,
			    (tmp_client->window_name ?
			     tmp_client->window_name : "(no name)"),
			    (tmp_client->xmoved_to_elsewhere ?
			     tmp_client->server->server_name : "local"));
		    SendBuffer(client->fdd->fd, out_buf, 80);
	       }
	       
	  }
     }

     client->fdd->ByteProcessing = XmoveCtrlRequest;
     client->fdd->inBuffer.num_Needed = 10;
     return(msglen);
}

static int
XmoveCtrlStartSetUp (Pointer private_data, unsigned char *buf, long n)
{
     Client *client = (Client *) private_data;

     /* since protocol-major-version buf[2] holds the signal that
	tells xmove that this is a control client, we put the version
	info in unused buf[10] and here we put it back where it belongs */
     
     ISetShort(&buf[2], IShort(&buf[10]));

     client->fdd->ByteProcessing = XmoveCtrlFinishSetUp;
     client->fdd->inBuffer.num_Needed += ROUNDUP4((long)IShort(&buf[6])) + ROUNDUP4((long)IShort(&buf[8]));

     return 0;
}

static int
XmoveCtrlFinishSetUp (Pointer private_data, unsigned char *buf, long n)
{
     Client *client = (Client *) private_data;
     Server *server = client->server;
     int msglen = client->fdd->inBuffer.num_Needed;

     client->fdd->ByteProcessing = XmoveCtrlRequest;
     client->fdd->inBuffer.num_Needed = 10;
     server->fdd->ByteProcessing = IgnoreServerBytes;
     server->fdd->inBuffer.num_Needed = 10000;

     close(server->alt_fd);
     server->alt_fd = 0;
     SendBuffer(server->fdd->fd, buf, msglen);
     
     ignore_bytes = True;
     
     /* Verify authorization for this client to connect to default server */
     
     if (verify_conn(client->fdd->fd, client->ip_addr, IShort(&buf[6]), buf+12,
		     IShort(&buf[8]), buf+12+ROUNDUP4(IShort(&buf[6]))) == -1) {
	  while (write(client->fdd->fd, "\000", 1) == 0);
	  return -1;
     }

     while (write(client->fdd->fd, "\001", 1) == 0);
     return msglen;
}

static int
XmoveCtrlRequest (Pointer private_data, unsigned char *buf, long n)
{
     Client *client = (Client *) private_data;
     Server *server = client->server;
     int msglen = client->fdd->inBuffer.num_Needed;

     client->fdd->inBuffer.num_Needed += ROUNDUP4((long)IShort(&buf[2])) +
	  ROUNDUP4((long)IShort(&buf[4])) + ROUNDUP4((long)IShort(&buf[6])) +
          ROUNDUP4((long)IShort(&buf[8]));

     switch (IShort(&buf[0])) {
     case 0:                    /* 0 == MoveAll */
	  client->fdd->ByteProcessing = XmoveCtrlMoveAll;
	  return 0;

     case 1:			/* 1 == List */
	  client->fdd->ByteProcessing = XmoveCtrlList;
	  return 0;

     case 2:			/* 2 == Move */
	  client->fdd->ByteProcessing = XmoveCtrlMove;
	  return 0;

     case 3:
	  client->fdd->ByteProcessing = XmoveCtrlQuit;
	  return 0;
	  
     otherwise:
	  ignore_bytes = True;
	  client->fdd->inBuffer.num_Needed = 10;
	  return msglen;
     }

     return 0;
}
     
static int
ClientRequest (private_data, buf, n)
Pointer			private_data;
unsigned char		*buf;
long			n;
{
    long requestlength = IShort(&buf[2]) << 2;
    Client         *client = (Client *) private_data;

    if ((u_long)n < requestlength) {
	 client->fdd->ByteProcessing = ClientRequest;
	 client->fdd->inBuffer.num_Needed = requestlength;
	 return(0);
    }

    DecodeRequest(buf, requestlength);

    client->fdd->inBuffer.num_Needed = 4;
    return(requestlength);
}

static int
StartSetUpReply (private_data, buf, n)
Pointer		private_data;
unsigned char	*buf;
long		n;
{
    short	replylength = IShort(&buf[6]);
    Server	*server = (Server *) private_data;

    server->fdd->ByteProcessing = FinishSetUpReply;
    server->fdd->inBuffer.num_Needed += 4 * replylength;
    return(0);
}

static int
FinishSetUpReply (private_data, buf, n)
Pointer		private_data;
unsigned char	*buf;
long		n;
{
    Server	*server = (Server *) private_data;
    unsigned char foo[256];
    int num_to_read;
    int msglen = server->fdd->inBuffer.num_Needed;

    CurrentVerbose = ErrorVerbose;
    PrintSetUpReply(buf);
    
    if (IByte(&buf[0]) == 0)	/* setup failed */
	 return -1;

    server->fdd->ByteProcessing = ServerPacket;
    server->fdd->inBuffer.num_Needed = 32;

    if (server->alt_fd == 0)
	 return(msglen);
    
    ReceiveBuffer(server->alt_fd, foo, 8);

    if (IByte(&foo[0]) == 0) {	/* setup failed on alt_fd */
	 printf("xmove: could not open second connection to server for new client\n");
	 return(-1);		/* tell caller to close connection */
    }

    /* read in and ignore the setup reply from the alternate connection
       to the server */
    
    num_to_read = IShort(&foo[6])*4;
    while (num_to_read > 256) {
	 ReceiveBuffer(server->alt_fd, foo, 256);
	 if (server->alt_resource_base == 0)
	      client->alt_resource_base = server->alt_resource_base = ILong(&foo[4]);
	 num_to_read -= 256;
    }
    ReceiveBuffer(server->alt_fd, foo, num_to_read);
    if (server->alt_resource_base == 0)
	 client->alt_resource_base = server->alt_resource_base = ILong(&foo[4]);

    return(msglen);
}

static int
MoveWindowRequest1 (Server *server, unsigned char *buf, long n)
{
     server->fdd->ByteProcessing = MoveWindowRequest2;
     server->fdd->inBuffer.num_Needed = 32 + (ILong(&buf[4]) * 4);
     ignore_bytes = True;
     return(0);
}

static int
MoveWindowRequest2 (Pointer private_data, unsigned char *buf, long n)
{
     Server *server = (Server *)private_data;
     char *retval;
     int msglen = server->fdd->inBuffer.num_Needed;

     /* after being moved, there are no further communications between
	us and the old xmove server, so we don't bother updating
	ByteProcessing and inBuffer.num_Needed. */

     meta_client = server->client->meta_client;

     MoveClient((char *)buf+36, buf[32], IShort(&buf[10]), (char *)buf+36+ROUNDUP4(IShort(&buf[8])), &retval);

     if (retval) {
	  int retvallen = strlen(retval);
	  u_char tmpbuf[4];
     
	  ISetLong(&tmpbuf[0], retvallen);
	  SendBuffer(client->fdd->fd, tmpbuf, 4);
	  SendBuffer(client->fdd->fd, (u_char *)retval, retvallen);
	  free(retval);
     } else {
	  int retvallen = 0;
	  SendBuffer(client->fdd->fd, (unsigned char *)&retvallen, 4);
     }

     ignore_bytes = True;
     server->fdd->ByteProcessing = ServerPacket;
     server->fdd->inBuffer.num_Needed = 32;
     return(msglen);
}

static int
ServerPacket (private_data, buf, n)
	Pointer					private_data;
	unsigned char			*buf;
	long					n;
{
	short					PacketType = IByte(&buf[0]);
	Server					*server = (Server *) private_data;

	if (PacketType == 0) {
	     DecodeError(buf, 32);
	     return 32;
	} else if (PacketType == 1) {
	     long replylength = 32 + (ILong(&buf[4]) << 2);

	     if (n < replylength) {
		  server->fdd->inBuffer.num_Needed = replylength;
		  return 0;
	     }

	     DecodeReply(buf, replylength);
	     server->fdd->inBuffer.num_Needed = 32;
	     return replylength;
	} else {
	     if (buf[0] == 253)
		  return(MoveWindowRequest1(server, buf, n));

	     DecodeEvent(buf, 32, True);
	     return 32;
	}
}

static int
IgnoreServerBytes (private_data, buf, n)
Pointer		private_data;
unsigned char	*buf;
long		n;
{
     return(n);
}

static int
IgnoreClientBytes (private_data, buf, n)
Pointer		private_data;
unsigned char	*buf;
long		n;
{
     return(n);
}

ParseDisplay(char *display_name)
{
     char *colon = strchr(display_name, ':');

     if (!colon)
	  return -1;

     *(colon++) = '\0';

     return atoi(colon);
}

static int verify_conn(int fd, unsigned long ip_addr,
		       int typelen, unsigned char *type, int keylen, unsigned char *key)
{
     if (!CheckAuth(ip_addr, typelen, type, keylen, key)) {
	  NoAuthReply.success = False;
	  NoAuthReply.lengthReason = 45;
	  ISetShort((unsigned char *)&NoAuthReply.majorVersion, 11);
	  ISetShort((unsigned char *)&NoAuthReply.minorVersion, 0);
	  ISetShort((unsigned char *)&NoAuthReply.length, 12);
	  
	  SendBuffer(fd, (unsigned char *)&NoAuthReply, sizeof(xConnSetupPrefix));
	  SendBuffer(fd, (unsigned char *)NoAuthError, NoAuthReply.length*4);
	  
	  return -1;
     } else {
	  return 0;
     }
}
	
