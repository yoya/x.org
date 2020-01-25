/*
 * Program:     $RCSfile: youbin_sub.c,v $  $Revision: 4.2 $
 *
 * Purpose:     UNIX library program for the Internet "youbin" service.
 *
 * Usage:       int
 *              youbin_sub(server, options, func, file_id)
 *              char    *server;    Server host name, NULL specifis this host.
 *              char    *options;   "D" for Debug.
 *                                  "B" for Biff compatibilty.
 *              void    (*func)();  Function called with Status packet.
 *                                  func(size, date, contents)
 *              int     file_id;    File id to notify update timing.
 *
 * Author:      K.Agusa     agusa@nuie.nagoya-u.ac.jp
 *              S.Yamamoto  yamamoto@nuie.nagoya-u.ac.jp
 *
 * Date:        1993/09/08
 * Modified:    $Date: 1994/06/12 08:35:27 $
 *
 * Copyright:   K.Agusa and S.Yamamoto  1993 - 94
 *
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge,
 * a full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and documentation
 * files (the "Software"), including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons who receive copies from any such
 * party to do so. This license includes without limitation a license to do
 * the foregoing actions under any patents of the party supplying this
 * software to the X Consortium.
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/param.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <ctype.h>
#include <netdb.h>
#include <pwd.h>                /* For getpwuid(). */
#include <setjmp.h>
#include <signal.h>
#include <stdio.h>

#include "youbin.h"

#ifndef lint
static char rcsid[] =
    "$Id: youbin_sub.c,v 4.2 1994/06/12 08:35:27 yamamoto Exp $";
#endif /* not lint */

static jmp_buf          env;
static int              sockid;             /* Socket ID. */
static char             my_id[32];          /* Youbin registration ID. */
static SockAddr         sa;                 /* Server's socket address. */
static int              saved_ppid;         /* Parent process ID. */
static int              biff_flag = OFF;    /* Mail header request. */

/*
 * Function declarations.
 */

int                 youbin_sub();

/*
 * Local function declarations.
 */

static void         open_udp();
static int          do_Wakeup();
static void         ST_loop();
static int          is_Update_packet();
static int          Status_packet();
static void         send_packet();
static void         send_Quit();
static int          receive_packet();
static void         sig_quit();

int
youbin_sub(server, options, func, file_id)
char    *server, *options;
void    (*func)();
int     file_id;
{
    char   user_name[USER_NAME_LEN + 1];        /* User name. */
    char   server_name[MAXHOSTNAMELEN + 1];
    
    int                 retval;                 /* Exit value. */
    int                 timer;                  /* Timer value from server. */
    SockAddr            ca;                     /* Socket address. */
    struct passwd       *pwent;                 /* For get user name. */
    char                *cp;
    
    saved_ppid = getppid();                     /* Save parent process ID. */
    if ((pwent = getpwuid(getuid())) != NULL) { /* Set user name. */
        strcpy(user_name, pwent->pw_name);
    } else {
        fprintf(stderr, "%s: Invalid user\n", LIB_NAME);
        return (ERR_USER);
    }
    if (server == NULL) {                       /* Get server host name. */
        gethostname(server_name, sizeof(server_name));
        server = server_name;
    }

    if (options != NULL) {                      /* Parse options. */
        for (cp = options; *cp != '\0'; cp++) {
            switch (*cp) {
              case 'D':                         /* Debug. */
                debug_mode = ON;
                break;
              case 'B':                         /* Biff. */ 
                biff_flag = ON;
                break;
            }
        }
    }

    signal(SIGHUP, sig_quit);                   /* Set signal handlers. */
    signal(SIGINT, sig_quit);
    signal(SIGQUIT, sig_quit);
    signal(SIGTERM, sig_quit);

    if ((retval = setjmp(env)) != 0) {
        switch (retval) {
          case SERV_HUP:
            sleep(atoi(my_id) % CLIENT_WAIT_TIME);
            break;
          case SERV_QUIT:
            return (EXIT_SUCCESS);
            break;
          default:
            return (retval);
            break;
        }
    } else {
        open_udp(server, &ca);                  /* Open UDP port. */
    }
    while (1) {
        strcpy(my_id, "0");                     /* For debug. */
        timer = do_Wakeup(user_name);           /* Establish connection. */
        ST_loop(timer, func, file_id);          /* Status-Thanks loop. */
    }
}

static int
do_Wakeup(name)
char    *name;
{
    char            Wakeup_buff[MESS_LEN + 1];  /* Make Wakeup packet. */
    char            buff[MESS_LEN + 1];         /* Communication buffer. */
    fd_set          fdset, fdset_tmp;
    struct timeval  timeout;
    int             retry;                      /* Retry counter for Wakeup. */
    int             timer;
    int             nfound;
    
    sprintf(Wakeup_buff, "W %s %s", name, PROTOCOL_VERSION);
    if (biff_flag == ON) {
        strcat(Wakeup_buff, " B");
    }
    /* Prepare Wakeup packet. */
    FD_ZERO(&fdset);                            /* Clear descriptor sets. */
    FD_SET(sockid, &fdset);                     /* Check socket only. */
    timeout.tv_sec = (long)(UNIT_TIME * 2);     /* Seconds. */
    timeout.tv_usec = 0L;                       /* And microseconds. */
    
    for (retry = 5; 0 < retry; retry--) {       /* Max 5 times trial. */
        send_packet(Wakeup_buff);               /* Send Wakeup packet. */
        fdset_tmp = fdset;
        if ((nfound = select(sockid + 1, &fdset_tmp, 0, 0, &timeout)) < 0) {
            perror("select");                   /* Something wrong. */
            continue;
        }
        if (nfound == 0 || !FD_ISSET(sockid, &fdset_tmp)) {
            continue;                           /* Timeout or invalid input. */
        }
        if (receive_packet(buff, MESS_LEN) == FAIL) {
            continue;                           /* Can not get packet. */
        }
        if (*buff == 'R') {
            if (sscanf(buff + PREFIX_LEN, "%s %d", my_id, &timer) == 2) {
                /* Get correct Registered packet. */
                return (timer);
            }
            /* Registerd packet with illegal parameter. */
            fprintf(stderr, "%s: Illegal register packet: %s\n", LIB_NAME, buff);
            continue;
        } else if (strncmp(buff, "NAK", strlen("NAK")) == 0) {
            /* NAK packet. */
            fprintf(stderr, "%s: Not accepted: %s\n", LIB_NAME, buff);
            longjmp(env, ERR_NAK);
        } else {                                /* Illegal response. */
            fprintf(stderr, "%s: Unexpected response: %s\n", LIB_NAME, buff);
        }
    }
    fprintf(stderr, "%s: Retry times exceeded\n", LIB_NAME);
    longjmp(env, ERR_RETRY);                    /* Too many retry. */
}

static void
ST_loop(timer, func, fid)
int         timer, fid;
int         (*func)();
{
    char            Thanks_buff[MESS_LEN + 1];
    char            Update_buff[MESS_LEN + 1];
    fd_set          fdset, fdset_tmp;
    struct timeval  timeout;
    int             nfound;
    int             nread;
    int             waitval = 0;            /* Eliminate Thanks rush. */
    int             TP_wait = OFF;          /* Thanks packet send request. */
    
    FD_ZERO(&fdset);
    FD_SET(sockid, &fdset);
    if (0 <= fid) {
        FD_SET(fid, &fdset);
        nread = MAX(sockid, fid) + 1;
    } else {
        nread = sockid + 1;
    }
    sprintf(Thanks_buff, "T %s", my_id);        /* Make Thanks packet. */
    sprintf(Update_buff, "U /%s", my_id);       /* Make Update packet. */
    timeout.tv_usec = 0L;
    
    while (1) {
        if (getppid() != saved_ppid) {
            send_Quit();
            longjmp(env, ERR_PPID);
        }
        waitval = (waitval + 1) % CLIENT_WAIT_TIME;
        /* Wait time varies dynamically. */
        timeout.tv_sec = (long)((TP_wait == OFF) ? timer : waitval);
        fdset_tmp = fdset;
        if ((nfound = select(nread, &fdset_tmp, 0, 0, &timeout)) < 0) {
            perror("select");                   /* Select error. */
            continue;
        }
        if (nfound == 0) {                      /* Timeout happens. */
            if (TP_wait == ON) {                /* Need to send T packet? */
                send_packet(Thanks_buff);       /* Send it. */
                TP_wait = OFF;                  /* Clear flag. */
                continue;
            }
            return;                             /* No signal from server. */
        }
        if (FD_ISSET(sockid, &fdset_tmp)) {
            if (Status_packet(func) == OK) {    /* Check packet validity. */
                if (TP_wait == ON) {            /* Get before send T packet? */
                    send_packet(Thanks_buff);   /* Send T packet immediately. */
                    TP_wait = OFF;
                } else {                        /* Otherwise set flag. */
                    TP_wait = ON;
                }
            }
        }
        if (0 <= fid && FD_ISSET(fid, &fdset_tmp)) {
            if (is_Update_packet(fid) == OK) {
                send_packet(Update_buff);
            }
        }
    }
}

/*
 * (1) If received packet is Status packet, return OK.
 * (2) Else return FAIL.
 */

static int
Status_packet(func)
int     (*func)();
{
    char    buff[HEADER_LEN + 1], *cp1, *cp2;
    int     size, date;
    
    if (receive_packet(buff, HEADER_LEN) == FAIL) {
        return (FAIL);
    }
    if (*buff != 'S') {                     /* Check format. */
        fprintf(stderr, "%s: Status packet is expected: %s\n", LIB_NAME, buff);
        return (FAIL);
    }
    size = strtol(buff + PREFIX_LEN, &cp1, 10);
    if (*cp1 == ' ') {
        date = strtol(cp1, &cp2, 10);
        if (*cp2 == '\0' || isspace(*cp2)) {
            func(size, date, ((*cp2 == '\0') ? cp2 : (cp2 + 1)));
            return (OK);
        }
    }
    fprintf(stderr, "%s: Illegal Status packet: %s\n", LIB_NAME, buff);
    return (FAIL);
}

static int
is_Update_packet(fid)
int     fid;
{
    int   len;
    char  buff[MESS_LEN + 1];
    
    if ((len = read(fid, buff, MESS_LEN)) < 0) {
        perror("read");
        return (FAIL);
    }
    buff[len] = '\0';                       /* Needed for debug only. */
    debug("%s: User command: %s\n", LIB_NAME, buff);

    switch (*buff) {
      case 'U':
        return (OK);
        break;
      case 'Q':
        send_Quit();
        longjmp(env, RET_QUIT);
        break;
      default:
        fprintf(stderr, "%s: Unkown command: %s\n", LIB_NAME, buff);
        return (FAIL);
        break;
    }
}

static int
receive_packet(mess, buflen)
char    *mess;
int     buflen;
{
    SockAddr    fa;
    int         len, flen = sizeof(fa);
    
    if ((len = recvfrom(sockid, mess, buflen, 0, &fa, &flen)) < 0) {
        perror("recvfrom");
        return (FALSE);
    }
    mess[len] = '\0' ;                      /* Zero termination. */

    /* Print first MAX_DEBUG_COLUMN characters of message. */
    debug("%s: [%s]: Recv: \"%.*s\"\n", LIB_NAME, my_id, MAX_DEBUG_COLUMN, mess);
    if (*mess == 'Q') {
        longjmp(env, (strcmp(mess + PREFIX_LEN, "hup") ? SERV_QUIT : SERV_HUP));
    }
    return (TRUE);
}

static void
send_Quit()
{
    char    buff[MESS_LEN + 1];

    sprintf(buff, "Q %s", my_id);
    send_packet(buff);
}

static void
send_packet(mess)
char    *mess;
{
    int     len;
    
    len = strlen(mess);
    debug("%s: [%s]: Send: \"%s\"\n", LIB_NAME, my_id, mess);
    if (sendto(sockid, mess, len, 0, &sa, sizeof(sa)) != len) {
        fprintf(stderr, "%s: Error in sending packet: %s\n", LIB_NAME, mess);
    }
}

static void
open_udp(host, ca)
char        *host;
SockAddr    *ca;
{
    int             i;
    struct hostent  *hp;            /* Result of host name lookup. */
    struct servent  *sp;            /* Result of service lookup. */
    
    /*
     * Look up the specified host name.
     */
    if ((hp = gethostbyname(host)) == NULL) {
        unsigned long   addr;

        if ((addr = inet_addr(host)) == -1 ||       /* Try as dotted-decimal. */
            (hp = gethostbyaddr((char *)&addr, sizeof(addr),
                                AF_INET)) == NULL) {
            fprintf(stderr, "%s: No such host: %s\n", LIB_NAME, host);
            longjmp(env, ERR_HOST);
        }
    }
    /*
     * Look up the service.
     */
    if ((sp = getservbyname(SERVICE_NAME, "udp")) == NULL) {
        fprintf(stderr, "%s: No such service: %s\n", LIB_NAME, SERVICE_NAME);
        longjmp(env, ERR_SERVICE);
    }
    /*
     * Allocate an open socket.
     */
    if ((sockid = socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        longjmp(env, ERR_SOCKET);
    }
    /* 
     * Make a server address.
     */
    bzero((char *)&sa, sizeof(sa));
    sa.sin_family = hp->h_addrtype;
    bcopy((char *)hp->h_addr, (char *)&sa.sin_addr, hp->h_length);
    sa.sin_port = sp->s_port;
    /*
     * Bind to localhost.
     */
    bzero((char *)ca, sizeof(*ca));
    ca->sin_family = hp->h_addrtype;
    ca->sin_addr.s_addr = htonl(INADDR_ANY);

    if (biff_flag == ON) {
        for (i = (IPPORT_RESERVED - 1); (IPPORT_RESERVED / 2) <= i; i--) {
            /* Try to find reserved port. */
            ca->sin_port = htons(i);
            if (bind(sockid, ca, sizeof(*ca)) == 0) {
                return;
            }
        }
    } else {                                    /* Try to find ordinary port. */
        ca->sin_port = htons(0);
        if (bind(sockid, ca, sizeof(*ca)) == 0) {
            return;
        }
    }
    perror("bind");                             /* Can not find. */
    longjmp(env, ERR_BIND);
}

static void
sig_quit()
{
    debug("%s: Catch signal INT, TERM or QUIT\n", LIB_NAME);
    send_Quit();
    longjmp(env, RET_SIG);
}
