/*
 * Program:     $RCSfile: list.c,v $  $Revision: 4.3 $
 *
 * Author:      K.Agusa     agusa@nuie.nagoya-u.ac.jp
 *              S.Yamamoto  yamamoto@nuie.nagoya-u.ac.jp
 *
 * Date:        1993/07/24
 * Modified:    $Date: 1994/06/12 09:36:43 $
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

#include <sys/param.h>
#include <netinet/in.h>
#include <netdb.h>              /* For gethostbyname() and gethostbyaddr(). */
#include <pwd.h>                /* For getpwuid(). */
#include <stdio.h>

#include "youbin.h"
#include "server.h"

#ifndef lint
static char rcsid[] =
    "$Id: list.c,v 4.3 1994/06/12 09:36:43 yamamoto Exp $";
#endif /* not lint */

extern SockAddr     ca;

User        users[MAX_USER];
State       states[MAX_STATE]; 

UserP       UserAlist;                      /* User availabe list. */
User        UserList;                       /* User list. Note not pointer. */
StateP      StateAlist;                     /* State available list. */

/*
 * User sturucture.
 */

void
init_users()
{
    int     i;
    
    /* Initialize users. */
    for (i = 0; i < MAX_USER - 1; i++) {
        users[i].next = &users[i + 1];
    }
    users[MAX_USER - 1].next = NULL;
    
    /* Initialize states. */
    for (i = 0; i < MAX_STATE - 1; i++) {
        states[i].next = &states[i + 1];
        states[i].state = NOT_USED;
    }
    states[MAX_STATE - 1].next = NULL;
    states[MAX_STATE - 1].state = NOT_USED;
    
    UserAlist = users;
    StateAlist = states;
    UserList.next = NULL;
}

StateP
make_user(uname, options, reason)
char    *uname, *options, *reason;
{
    /* 
     * If this function return NULL then reason shows why it fails.
     */
    int             biff_flag = OFF;                /* Mail header request. */
    State           dummy;
    StateP          sp, spp;
    UserP           up;
    
    while (*options != '\0') {
        switch (*options++) {
          case 'B':                                 /* Biff. */ 
            if (is_trusted_user(uname)) {
                biff_flag = ON;
            } else {
                strcpy(reason, "Not trusted host or trusted user");
                return (NULL);
            }
            break;
        }
    }

    /* Check user name with reading passwd file. */
    if (getpwnam(uname) == NULL) {                  /* Invalid user. */  
        warn_log("Invalid user name: %s\n", uname);
        strcpy(reason, "Invalid user name");
        return (NULL);
    }
    /* Check user list. */
    if ((up = find_user(uname)) != NULL) {          /* Already registered. */
        for (dummy.next = up->stat, spp = &dummy;
             (sp = spp->next) != NULL; spp = sp) {
            if (check_ca(&(sp->ca)) != FALSE) {     /* Existing state. */
                return (sp);
            }
        }
        /* Already registered user, but new request. */
        if ((sp = new_state()) != NULL) {
            spp->next = sp;
            sp->ca = ca;
            sp->parent = spp->parent;
            sp->mode.biff = biff_flag;
            return (sp);
        }
        strcpy(reason, "No space for State");
        return (NULL);
    }
    /* New user. Make user entry. */
    if ((up = new_user(uname)) == NULL) {           /* Fail to create user. */
        strcpy(reason, "No space for User");
        return (NULL);
    }
    /* Make state stucture. */
    if ((sp = new_state()) == NULL) {
        dispose_user(up);                           /* Can not use the area. */
        strcpy(reason, "No space for State");
        return (NULL);
    }
    up->stat = sp;
    sp->ca = ca;                                    /* Save port address. */
    sp->parent = up;
    sp->mode.biff = biff_flag;
    check_spool(up);                                /* Set size and date. */
    return (sp);
}

UserP
find_user(name)
char    *name;
{
    UserP   up;
    
    for (up = UserList.next; up != NULL; up = up->next) {
        if (strcmp(up->name, name) != 0) {      /* Not matched, check next. */
            continue;
        }
        return (up);                                /* Find. */
    }
    return (NULL);
}

UserP
new_user(name)
char    *name;    
{
    UserP   new;
    
    if ((new = UserAlist) != NULL) {
        UserAlist = new->next;
        strcpy(new->name, name);
        new->next = UserList.next;
        UserList.next = new;
    } else {
        warn_log("No more space for User\n");
    }
    return (new);
}

void
dispose_user(up)
UserP   up;
{
    up->next = UserAlist;
    UserAlist = up;
}

/*
 * State sturucture.
 */

StateP
new_state()
{
    StateP  new;
    
    if ((new = StateAlist) != NULL) {
        StateAlist = new->next;
        new->next = NULL;
        new->state = WAIT;
    } else {
        warn_log("No more space for State\n");
    }
    return (new);
}

void
dispose_state(sp)
StateP  sp;
{
    sp->state = NOT_USED;
    sp->next = StateAlist;
    StateAlist = sp;
}

void
del_state(id)
StateP  id;
{
    State   dummy;
    StateP  s, sp;
    UserP   u, up, user;
    
    user = id->parent;
    dummy.next = user->stat;
    for (sp = &dummy; (s = sp->next) != NULL; sp = s) {
        if (s == id) {
            sp->next = s->next;
            dispose_state(s);
            if ((user->stat = dummy.next) == NULL) {
                for (up = &UserList; (u = up->next) != NULL; up = u) {
                    if (u == user) {
                        up->next = u->next;
                        dispose_user(u);
                        return;
                    }
                }
                warn_log("Can not find user: [%ld]\n", (long)id);
            }
            return;
        }
    }
    warn_log("Can not find user: [%ld]\n", (long)id);
}

StateP
get_id(buff)
char    *buff;
{
    StateP  sp;
    
    /* Note, should check ca. */
    sp = (StateP)atol(buff);
    if (sp < states || &states[MAX_STATE - 1] < sp ||
        (((long)sp - (long)states) % sizeof(State)) != 0 ||
        sp->state == NOT_USED) {
        return (NULL);
    }
    return (sp);
}

/*
 * Authentification.
 */

int
is_trusted_user(user_name)
char    *user_name;
{
    struct hostent  *hp;
    
    if ((hp = gethostbyaddr((char *)&ca.sin_addr,
                            sizeof(ca.sin_addr), ca.sin_family)) == NULL) {
        sys_error_log("gethostbyaddr");
        return (FAIL);
    }
    return ((IPPORT_RESERVED / 2) <= ca.sin_port &&
            ca.sin_port < IPPORT_RESERVED &&
            ruserok(hp->h_name, 0, user_name, user_name) == 0);
}
