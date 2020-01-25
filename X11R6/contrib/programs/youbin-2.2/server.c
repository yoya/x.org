/*
 * Program:     $RCSfile: server.c,v $  $Revision: 4.1 $
 *
 * Purpose:     UNIX application program that becomes a server for
 *              the Internet "youbin" service.
 *
 * Usage:       See global variable usage.
 *
 * Author:      K.Agusa     agusa@nuie.nagoya-u.ac.jp
 *              S.Yamamoto  yamamoto@nuie.nagoya-u.ac.jp
 *
 * Date:        1993/07/24
 * Modified:    $Date: 1994/05/31 08:44:02 $
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

#if defined(__STD__)
#include <unistd.h>             /* ANSI only. */
#endif /* defined(__STD__) */

#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>           /* For struct timeval. */ 
#include <netinet/in.h>
#include <errno.h>              /* For EINTR. */
#include <fcntl.h>              /* For O_RDONLY. */
#include <netdb.h>              /* For gethostbyname() and gethostbyaddr(). */
#include <pwd.h>                /* For getpwuid(). */
#include <signal.h>
#include <stdio.h>

#include "youbin.h"
#include "server.h"

#ifndef lint
static char rcsid[] =
    "$Id: server.c,v 4.1 1994/05/31 08:44:02 yamamoto Exp $";
#endif /* not lint */

static char *usage[] = {
    "NAME",
    "    youbind - server for the Internet \"youbin\" service",
    "SYNOPSIS",
    "    youbind [-d][-h][-t][-A port]",
    "OPTIONS",
    "    -d       Debug mode.",
    "    -h       Help: print out the usage.",
    "    -t       Trace: print out beginning and end of service.",
    "    -A port  Auxiliary Service port name.",
    "AUTHORS",
    "    K.Agusa     agusa@nuie.nagoya-u.ac.jp",
    "    S.Yamamoto  yamamoto@nuie.nagoya-u.ac.jp",
    NULL
};

/*
 * Global variables.
 */

SockAddr    sa;                             /* Server address. */
SockAddr    ca;                             /* Client address. */

int         sockid;                         /* For sockets. */

char        *prog_name;                     /* This program name. */
char        *service_name = SERVICE_NAME;   /* Service name. */

/* Port for non standard /bin/mail. For example, MIPS's /bin/mail uses
   mbiff (6512/udp) port instead of biff (512/udp). */
char        *aux_port_name = '\0';

char        spool_path[MAXPATHLEN + 1];     /* Spool path. */
char        *spool_file;                    /* File name part of spool path. */

char        NAK_buff[MESS_LEN + 1];

/* Note, NAK_reason is modified with side effect of make_user(). */
char        *NAK_reason;

int         flag_timeout = OFF;             /* Be set in sig_alarm(). */
int         flag_hup = OFF;                 /* Be set in sig_hup(). */
int         flag_quit = OFF;                /* Be set in sig_quit(). */

/* Mode flags. */
int         debug_mode = OFF;
int         trace_mode = OFF;

int         exitcode = EXIT_SUCCESS;

/* The followings are only for debug. */
static char *state_name[] = {
    "ACKED", "IDLE", "WAIT", "TO1", "TO2", "NOT_USED"
};

/*
 * External variables.
 */

extern int          errno;

/* For getopt(). */
extern char         *optarg;
extern int          optind, opterr;

int
main(argc, argv)
int     argc;
char    **argv;
{
    int             auxid = 0;              /* Auxiliary fid. */
    int             max_fd;                 /* For select. */
    fd_set          fdset, fdset_tmp;
    SockAddr        aux_sa;                 /* Auxiliary server address. */
    struct timeval  dummy;
    
    init(argc, argv);                       /* Check arguments. */
    init_log();                             /* Initialize syslog. */
    init_users();                           /* Initialize users and states. */

    FD_ZERO(&fdset);                        /* Clear descriptor sets. */
    sockid = open_udp(service_name, &sa);   /* Port for clients. */
    FD_SET(sockid, &fdset);
    if (aux_port_name != NULL) {
        auxid = open_udp(aux_port_name, &aux_sa);
        FD_SET(auxid, &fdset);
    }
    max_fd = MAX(sockid, auxid) + 1;
    
    signal(SIGINT, sig_quit);               /* Set signal handlers. */ 
    signal(SIGQUIT, sig_quit);
    signal(SIGTERM, sig_quit);
    signal(SIGHUP, sig_hup);
    signal(SIGALRM, sig_alarm);

    /*
     * Set dummy time out interval. The time out never happen,
     * because periodic() is called in every UNIT_TIME.
     */
    dummy.tv_sec = (long)(UNIT_TIME * 10);
    dummy.tv_usec = 0L;

    /* Dive into main loop. Don't use setjmp() and longjmp(),
       because list maintenance routines are in critical section. */
    alarm(UNIT_TIME);
    while (1) {
        if (flag_hup == ON) {       /* First time call or catching SIGHUP. */ 
            send_Quit("hup");       /* Kill all clients. */
            init_users();           /* Re-initialize users and states. */
            flag_hup = OFF;
        }
        if (flag_quit == ON) {      /* Catch SIGINT, SIGTERM or SIGQUIT. */ 
            send_Quit("quit");
            exit(EXIT_SUCCESS);
        }
        if (flag_timeout == ON) {   /* Timeout. */
            periodic();
            flag_timeout = OFF;
        }
        fdset_tmp = fdset;
        if (select(max_fd, &fdset_tmp, 0, 0, &dummy) < 0) {
            if (errno != EINTR) {
                sys_error_log("select");
            }
            continue;
        }
        if (FD_ISSET(sockid, &fdset_tmp)) {
            get_mess(sockid);
        }
        if (auxid != 0 && FD_ISSET(auxid, &fdset_tmp)) {
            get_mess(auxid);
        }
    }
}

void
init(argc, argv)
int     argc;
char    **argv;
{
    int   c;
    
    if ((prog_name = strrchr(argv[0], '/')) != NULL) {
        prog_name++;
    } else {
        prog_name = argv[0];
    }

    strcpy(spool_path, MAIL_SPOOL);
    spool_file = spool_path + strlen(spool_path);

    strcpy(NAK_buff, "NAK ");
    NAK_reason = NAK_buff + strlen(NAK_buff);

    opterr = 0;
    while ((c = getopt(argc, argv, "dhtA:S:")) != EOF) {
        switch (c) {
          case 'd':                         /* Debug mode. */
            debug_mode = ON;
            break;
          case 'h':                         /* Help. */
            print_usage(usage);
            exit(EXIT_SUCCESS);
            break;
          case 't':                         /* Trace mode. */
            trace_mode = ON;
            break;
          case 'A':                         /* Auxiliary port name. */
            aux_port_name = optarg;
            break;
          case 'S':                         /* Service port name. For debug. */
            service_name = optarg;
            break;
          default:
            exitcode++;
            break;
        }
    }
    if (optind != argc || exitcode != EXIT_SUCCESS) {
        warn_log("Unknown options\n"); 
        print_usage(usage);
        exit(EXIT_FAILURE);
    }
}

void
get_mess(fid)
int     fid;
{
    char            buff[MESS_LEN + 1];
    int             len, flen = sizeof(ca);
    
    if ((len = recvfrom(fid, buff, MESS_LEN, 0, &ca, &flen)) < 0) {
        if (errno != EINTR) {
            warn_log("Error in receiving packet\n");
        }
        return;
    }
    buff[len] = '\0';                       /* Zero termination. */
    if (len < 2) {
        debug_log("Too short length packet: %s\n", buff);
        return;
    }
    if (flen != sizeof(ca) || ca.sin_family != AF_INET) {
        char    buff_log[LOG_LEN];

        addr_to_str(buff_log, sa);
        warn_log("Invalid source address: %s\n", buff);
        warn_log("    IP = %s, port = %d\n", buff_log, sa.sin_port);
        return;
    }
    if (buff[1] != ' ') {
        /* Trim the last newline.
           Packet from /bin/mail has a ending newline. */
        if (buff[len - 2] == '\n') {
            buff[len - 2] = '\0';
        } else if (buff[len - 1] == '\n') {
            buff[len - 1] = '\0';
        }
        debug_log("Recv: from /bin/mail on port %s: \"%s\"\n",
                  ((fid == sockid) ? service_name : aux_port_name), buff);
        proc_mail(buff);
    } else {
        debug_log("Recv: from client: \"%s\"\n", buff);
        proc_client(buff, len);
    }
}

void
proc_mail(mess)
char    *mess;
{
    /* 
     * This routine sends a part of mail like biff.
     * Add a flag "B" to Wakeup packet if you want.
     */
    char    *cp;
    off_t   offset;
    UserP   up;
    StateP  sp;
    
    if ((cp =  strchr(mess, '@')) == NULL) {
        warn_log("Invalid packet from /bin/mail: %s\n", mess);
        return;
    }
    
    *cp = '\0';
    if((up = find_user(mess)) == NULL) {    /* Check if user is registered. */
        return;
    }
    offset = atol(cp + 1);                  /* Get offset address. */
    check_spool(up);                        /* Update size and time. */
    for (sp = up->stat; sp != NULL; sp = sp->next) {
        if (sp->mode.biff) {
            send_header(sp, offset);        /* Send header and so on. */
        } else {
            send_Status(sp);                /* Notify of modification. */
        }
    }
}

void
send_header(sp, offset)
StateP  sp;
off_t   offset;
{
    /*
     * Some part of this function are copied from BSD source.
     */
    FILE    *fi;
    int     len, line_cnt, char_cnt;
    int     flag_in_header;
    char    *line;
    char    buff[HEADER_LEN];

    strcpy(spool_file, sp->parent->name);
    if ((fi = fopen(spool_path, "r")) == NULL) {    /* Check mail spool. */
        sys_error_log("open");              /* Not created yet. */
        return;
    }
    sprintf(buff, "S %d %d ", sp->parent->size, sp->parent->time);
    line = buff + strlen(buff);
    (void)fseek(fi, offset, 0);             /* Locate to proper position. */

    /*
     * Print the first 7 lines or 560 characters of the new mail
     * (whichever comes first).  Skip header crap other than
     * From, Subject, To, and Date.
     */
    line_cnt = 7;
    char_cnt = 560;
    flag_in_header = ON;
    while (fgets(line, sizeof(buff) - (line - buff), fi) != NULL) {
        if (flag_in_header == ON) {
            if (line[0] == '\n') {
                flag_in_header = OFF;
                continue;
            }
            if (line[0] == ' ' || line[0] == '\t' ||
                (strncmp(line, "From:", 5) && strncmp(line, "Subject:", 8))) {
                continue;
            }
        }
        if (line_cnt <= 0 || char_cnt <= 0) {
            strcpy(line, "...more...\n");
            break;
        }
        len = strlen(line);
        line += len;
        char_cnt -= len;
        line_cnt--;
    }
    fclose(fi);
    /* 
     * strcpy(line, "----\n");
     */
    send_packet(buff, sp);              /* I do not know if too large packet. */
}

void
proc_client(buff, len)
char  *buff;
int   len;
{
    StateP      id;                         /* User ID. */
    
    switch (*buff) {
      case 'W':                             /* Wakeup packet. */
        do_Wakeup(buff + PREFIX_LEN, buff + len);
        break;
      case 'T':                             /* Thanks packet. */
        if ((id = get_id(buff + PREFIX_LEN)) == NULL) { /* Invalid user. */
            warn_log("Invalid Thanks packet: %s\n", buff);
        } else {
            debug_log("    Thanks packet: %s [%ld]: %s\n",
                      id->parent->name, id, state_name[id->state]);
            id->state = ACKED;
        }
        break;
      case 'U':                             /* Update packet. */
        do_Update(buff + PREFIX_LEN);
        break;
      case 'Q':                             /* Quit packet. */
        if ((id = get_id(buff + PREFIX_LEN)) == NULL) { /* Invalid user. */
            warn_log("Invalid Quit packet: %s\n", buff);
        } else {
            char            buff_log[LOG_LEN];  /* Only for log. */

            sprintf(buff_log, "Quit packet: %s [%ld] will be deleted\n",
                    id->parent->name, id);
            debug_log("    %s", buff_log);
            trace(buff_log);
            del_state(id);
        }
        break;
      default:
        warn_log("Unknown packet: %s\n", buff);
        break;
    }
}

void
sig_alarm()
{
    alarm(UNIT_TIME);
    flag_timeout = ON;
    signal(SIGALRM, sig_alarm);
}

void
sig_hup()
{
    warn_log("Catch signal HUP\n");
    flag_hup = ON;
    signal(SIGHUP, sig_hup);
}

void
sig_quit()
{
    warn_log("Catch signal INT, TERM or QUIT\n");
    flag_quit = ON;
}

void
periodic()
{
    UserP                   up, upp;
    State                   dummy;
    StateP                  sp, spp;
    static unsigned int     times;
    
    debug_log("periodic: %d\n", times);
    if (times++ & 1) {                      /* Check spool every two tic. */
        for (up = UserList.next; up != NULL; up = up->next) {
            check_spool(up);
        }
    }
    for (upp = &UserList; (up = upp->next) != NULL; upp = up) {
        for (dummy.next = up->stat, spp = &dummy; (sp = spp->next) != NULL;
             spp = sp) {
            debug_log("    %s [%ld]: %s -> %s\n", up->name, sp,
                      state_name[sp->state], state_name[sp->state + 1]);
            switch (sp->state) {
              case ACKED:
                sp->state = IDLE;
                break;
              case IDLE:
                sp->state = WAIT;
                send_Status(sp);
                break;
              case WAIT:
                sp->state = TO1;
                send_Status(sp);
                break;
              case TO1:
                sp->state = TO2;
                send_Status(sp);
                break;
              default:                      /* Something wrong. */
                warn_log("%s [%ld]: Unknown state: %d\n",
                         up->name, (long)sp, sp->state);
                /* Don't break. */
              case TO2:                     /* TO2 and fatal error. */
                spp->next = sp->next;
                dispose_state(sp);
                sp = spp;
                break;
            }
        }
        if ((up->stat = dummy.next) == NULL) {
            upp->next = up->next;
            dispose_user(up);
            up = upp;
        }
    }
}

/* 
 * Check given sockaddr_in with lastly received sockaddr_in which is in
 * global variable ca. Boolean function. TRUE means equal.
 */

int
check_ca(saddr)
SockAddr    *saddr;
{
    return (ca.sin_family == saddr->sin_family &&
            ca.sin_port == saddr->sin_port &&
            ca.sin_addr.s_addr == saddr->sin_addr.s_addr);
}

int
open_udp(service, saddr)
char        *service;
SockAddr    *saddr;
{
    struct hostent  *hp;        /* Result of host name lookup. */
    struct servent  *sp;        /* Result of service lookup. */
    char            localhost[MAXHOSTNAMELEN + 1];  /* Local host name. */
    int             id;         /* socket id to be returned    */
    
    /*
     * Look up the specified host name.
     */
    gethostname(localhost, MAXHOSTNAMELEN);
    if ((hp = gethostbyname(localhost)) == NULL) {
        warn_log("No such host: %s\n", prog_name, localhost);
        exit(1);
    }
    /*
     * Look up the service.
     */
    if ((sp = getservbyname(service, "udp")) == NULL) {
        warn_log("No such service: %s\n", service);
        exit(1);
    }
    /*
     * Allocate an open socket.
     */
    if ((id = socket(hp->h_addrtype, SOCK_DGRAM, 0)) < 0) {
        sys_error_log("socket");
        exit(EXIT_FAILURE);
    }
    /*
     * Connect to the remote server.
     */
    bzero((char *)saddr, sizeof(*saddr));
    saddr->sin_family = hp->h_addrtype;
    saddr->sin_addr.s_addr = htonl(INADDR_ANY);
    saddr->sin_port = sp->s_port;
    if (bind(id, saddr, sizeof(*saddr)) < 0) {
        sys_error_log("bind");
        exit(EXIT_FAILURE);
    }
    return (id);
}

/* 
 * Set user spool's size and modified time.
 */

void
check_spool(up)
UserP     up;
{
    struct stat     status;                     /* A structure for fstat(). */
    int             i;
    
    strcpy(spool_file, up->name);
    if ((i = open(spool_path, O_RDONLY)) == -1) {   /* Check mail spool. */
        if (errno != ENOENT) {
            sys_error_log("open");
        }
        up->size = up->time = 0;
    } else {
        if (fstat(i, &status) < 0) {            /* Get spool attributes. */
            sys_error_log("fstat");
            up->size = up->time = 0;
        } else {
            up->size = status.st_size;
            up->time = status.st_mtime;
        }
        close(i);
    }
}

void
send_packet(mess, sp)
char        *mess;
StateP      sp;
{
    SockAddr    *saddr;
    
    saddr = (sp == CA_ADDR) ? &ca : &(sp->ca);
    /* Print first MAX_DEBUG_COLUMN characters of message. */
    debug_log("Send: %s [%ld]: \"%.*s\"\n",
              ((sp == CA_ADDR) ? "" : sp->parent->name), sp,
              MAX_DEBUG_COLUMN, mess); 
    if (sendto(sockid, mess, strlen(mess), 0, saddr, sizeof(*saddr)) < 0) {
        warn_log("Error in sending packet: %s\n", mess);
    }
}
