/*
 * Program:     $RCSfile: youbin.h,v $  $Revision: 4.4 $
 *
 * Purpose:     Header file of the Internet "youbin" service.
 *
 * Author:      K.Agusa     agusa@nuie.nagoya-u.ac.jp
 *              S.Yamamoto  yamamoto@nuie.nagoya-u.ac.jp
 *
 * Date:        1993/07/24
 * Modified:    $Date: 1994/06/14 05:46:27 $
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
                            
#define VERSION             "2.2"           /* Version. */
#define PATCH_LEVEL         "0"             /* Patch level. */
#define PROTOCOL_VERSION    "2"             /* Protocol version. */

/* 
 *                  Platform definitions.
 */

#if defined(SVR4) && !defined(SYSV)
#define SYSV
#endif /* defined(SVR4) && !defined(SYSV) */

/*
 *                  For compatibility.
 */

#ifdef SYSV
#include <string.h>                         /* For strchr(). */
#else /* not SYSV */
#include <strings.h>
#if !defined(strchr) && !defined(strrchr)
#define strchr(str, c)          index((str), (c))
#define strrchr(str, c)         rindex((str), (c))
#endif /* !defined(strchr) && !defined(strrchr) */
#endif /* not SYSV */

#ifdef SYSV
#define bzero(dst, len)         memset((dst), '\0', (len))
#define bcopy(src, dst, len)    memcpy((dst), (src), (len))
#endif

/*
 *                  Basic values.
 */

#ifndef TRUE
#define TRUE                1
#define FALSE               0
#endif /* not TRUE */ 

#define ON                  1
#define OFF                 0

#define OK                  1
#define FAIL                0

#if !defined(MAXHOSTNAMELEN)
#define MAXHOSTNAMELEN      256
#endif /* !defined(MAXHOSTNAMELEN) */

#if !defined(R_OK)
#define R_OK                4
#define W_OK                2
#define F_OK                0
#endif /* !defined(R_OK) */

/*
 * Exit code.
 */ 
#ifndef EXIT_SUCCESS    
#define EXIT_SUCCESS        0
#endif /* not EXIT_SUCCESS */

#ifndef EXIT_FAILURE
#define EXIT_FAILURE        1
#endif /* not EXIT_FAILURE */

/*
 *                  Constant definitions.
 */

#ifndef LIB_NAME
#define LIB_NAME            "youbin"        /* Library name. */
#endif /* not LIB_NAME */

#ifndef MAIL_SPOOL
#define MAIL_SPOOL          "/usr/spool/mail/"
#endif /* not MAIL_SPOOL */

#ifndef SERVICE_NAME
#define SERVICE_NAME        "biff"
#endif /* not SERVICE_NAME */

#define MAX_USER            256             /* Max users. */ 
#define MAX_STATE           (4 * MAX_USER)  /* 4 requests per user. */

#define USER_NAME_LEN       32              /* Max user name length. */

#define CA_ADDR             ((StateP)NULL)

#define LOG_LEN             1024            /* Max log message length. */

#define MAX_DEBUG_COLUMN    40              /* Max debug write length. */

/*
 *                  Protocol definitions.
 */

#ifndef UNIT_TIME
#define UNIT_TIME           180             /* Clock tic in sec. */
#endif /* not UNIT_TIME */

#define CLIENT_TIME_OUT     (6 * UNIT_TIME) /* Timer for client abort. */

/*
 * Client waits for that time before sending Thanks packet.
 * And client waits for the next Wakeup packet.
 */
#define CLIENT_WAIT_TIME    ((int)((2 * UNIT_TIME) / 3))

#define MESS_LEN            256             /* Max message length. */

#define PREFIX_LEN          2               /* Length of "W " and so on. */

/*
 * For youbin_sub().
 * Ordinary message and 3 header lines and 7 body lines.
 * Note, Should check RFC822.
 */
#define HEADER_LEN      (MESS_LEN + (80 * (3 + 7)))

/*
 *                  Macros.
 */

#ifndef MIN
#define MIN(x, y)           (((x) < (y)) ? (x) : (y))
#endif
    
#ifndef MAX
#define MAX(x, y)           (((x) < (y)) ? (y) : (x))
#endif

/*
 *                  Type definitions.
 */

typedef struct sockaddr_in  SockAddr;

/*
 *                  Global variables.
 */

extern char     *prog_name;

extern int      debug_mode;
extern int      trace_mode;

/*
 *                  Function declarations.
 */

long        atol();
    
void        proc_mail();
void        proc_client();

void        do_Update();

void        send_header();
void        send_Registerd();
void        send_Status();

void        recv_packet();

void        debug();

void        print_usage();

/*
 *                  Exit code of youbin_sub().
 */

#define ERR_USER        1
#define ERR_HOST        2
#define ERR_SERVICE     3
#define ERR_SOCKET      4
#define ERR_BIND        5
#define ERR_NAK         6
#define ERR_RETRY       7
#define ERR_PPID        8

#define RET_QUIT        80                  /* Received "Q" from stdin. */
#define RET_SIG         81                  /* Received signals. */

#define SERV_HUP        90                  /* Received "Q hup" from server. */
#define SERV_QUIT       91                  /* Received "Q quit" from server. */
