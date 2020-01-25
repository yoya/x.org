/*
 * Program:     $RCSfile: server.h,v $  $Revision: 4.1 $
 *
 * Purpose:     Header file of the Internet "youbin" service.
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

/*
 *                  Type definitions.
 */

enum vecs       {ACKED, IDLE, WAIT, TO1, TO2, NOT_USED};

typedef struct _State       *StateP;
typedef struct _User        *UserP;

typedef struct _State {
    enum vecs           state;
    SockAddr            ca;
    struct {
        unsigned int    biff : 1;   /* Biff compatible mode. */
    } mode;
    UserP               parent;     /* To name structure. */
    StateP              next;       /* If someone requests twice. */
} State;

typedef struct _User {
    char            name[USER_NAME_LEN];
    long            size;           /* Size of user mail spool file. */
    long            time;           /* Date of user mail spool file. */
    StateP          stat;           /* Pointer to State. */
    UserP           next;           /* Link to next User. */
} User;

/*
 *                  External variables.
 */

extern char         *NAK_reason;
extern char         NAK_buff[MESS_LEN + 1];
extern SockAddr     ca;

/* List structures. */
extern User         users[MAX_USER];
extern State        states[MAX_STATE]; 

extern UserP        UserAlist;
extern User         UserList;
extern StateP       StateAlist;

/*
 *                  Function declarations.
 */

void        check_spool();
void        get_mess();
void        init();
int         open_udp();
void        print_addr();

void        do_Wakeup();
void        send_packet();
void        send_Quit();

void        periodic();

void        sig_quit();
void        sig_hup();
void        sig_alarm();

/*
 *                  Function declarations: "list.c".
 */

int         is_trusted_user();
int         check_ca();

void        init_users();
StateP      make_user();
UserP       find_user();
UserP       new_user();
void        dispose_user();
void        del_state();

StateP      new_state();
void        dispose_state();
StateP      get_id();

/*
 *                  Function declarations: "log.c".
 */

void        debug_log();
void        warn_log();
void        info_log();
void        error_log();
void        sys_error_log();
void        output_log();

void        trace();
