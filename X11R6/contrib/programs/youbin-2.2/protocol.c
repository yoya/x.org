/*
 * Program:     $RCSfile: protocol.c,v $  $Revision: 4.1 $
 *
 * Purpose:     Protocol routines of internet "youbin" service.
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

#include <sys/types.h>          /* Must be included before "sys/socket.h". */
#include <sys/socket.h>         /* For struct sockaddr. */ 
#include <netinet/in.h>         /* For struct sockaddr_in. */
#include <netdb.h>              /* For gethostbyname() and gethostbyaddr(). */
#include <stdio.h>

#include "youbin.h"
#include "server.h"

#ifndef lint
static char rcsid[] =
    "$Id: protocol.c,v 4.1 1994/05/31 08:44:02 yamamoto Exp $";
#endif /* not lint */

void
do_Wakeup(mess, endp)
char    *mess, *endp;
{
    /* 
     * mess := "<user name> <protocol version>"
     *       | "<user name> <protocol version> <options>"
     */
    char        *cp;                        /* Pointer to <protocol version>. */
    char        *cp1;                       /* Pointer to <option>. */
    StateP      id;                         /* User ID. */

    if ((cp = strchr(mess, ' ')) == NULL) {
        strcpy(NAK_reason, "Invalid version");
        send_packet(NAK_buff, CA_ADDR);
        return;
    }
    *cp++ = '\0';                           /* Trim protocol version. */
    if ((cp1 = strchr(cp, ' ')) == NULL) {
        cp1 = endp;                         /* No options. */
    } else {
        *cp1++ = '\0';                      /* Trim options. */
    }
    
    if (strcmp(cp, PROTOCOL_VERSION) != 0) {    /* Check protocol version. */
        strcpy(NAK_reason, "Invalid version");
        send_packet(NAK_buff, CA_ADDR);
        return;
    }

    if ((id = make_user(mess, cp1, NAK_reason)) != NULL) {
        if (debug_mode || trace_mode) {
            char            buff_log[LOG_LEN];  /* Only for log. */
            struct hostent  *hp;

            if ((hp = gethostbyaddr((char *)&ca.sin_addr,
                        sizeof(ca.sin_addr), ca.sin_family)) == NULL) {
                sys_error_log("gethostbyaddr");
            } 
            sprintf(buff_log, "Wakeup packet: %s [%d]: host = %s, port = %d\n",
                    mess, id, (hp->h_name ? hp->h_name : ""), ca.sin_port);
            debug_log("    %s", buff_log);
            trace(buff_log);
        }
        send_Registerd(id);
        send_Status(id);
    } else {                            /* Invalid user. */
        send_packet(NAK_buff, CA_ADDR); /* Send to global ca. */
    }
}

void
do_Update(mess)
char    *mess;
{
    /*
     * Value of mess is one of below:
     * (1) packet = "U <user name>", mess = "<user name>"
     * (2) packet = "U /<user id>", mess = "/<user id>"
     * (3) packet = "<user name>@<mailbox offset>", mess = "<user name>".
     */
    UserP   up;
    StateP  sp;
    
    if (*mess == '/') {         /* Case (2). */
        if ((sp = get_id(mess + 1)) != NULL) {
            up = sp->parent;
        } else {
            up = NULL;
        }
    } else {                    /* Case (1) or (3). */
        up = find_user(mess);
    }
    if (up != NULL) {
        check_spool(up);
        for (sp = up->stat; sp != NULL; sp = sp->next) {
            send_Status(sp);
        }
    }
}

void
send_Registerd(sp)
StateP  sp;
{
    char    buff[MESS_LEN + 1];

    sprintf(buff, "R %ld %d", (long)sp, CLIENT_TIME_OUT); 
    send_packet(buff, sp);
}

void
send_Status(sp)
StateP  sp;
{
    UserP   up;
    char    buff[MESS_LEN + 1];
    
    up = sp->parent;
    sprintf(buff, "S %d %d", up->size, up->time);
    send_packet(buff, sp);
}

void
send_Quit(reason)
char    *reason;
{
    StateP  sp;
    UserP   up;
    char    buff[MESS_LEN + 1];
    
    sprintf(buff, "Q %s", reason);
    for (up = UserList.next; up != NULL; up = up->next) {
        for (sp = up->stat; sp != NULL; sp = sp->next) {
            send_packet(buff, sp);
        }
    }
}
