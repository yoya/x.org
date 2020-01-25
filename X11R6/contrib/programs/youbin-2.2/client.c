/*
 * Program:     $RCSfile: client.c,v $  $Revision: 4.1 $
 *
 * Purpose:     UNIX application program that becomes a client for
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

#include <sys/param.h>  
#include <netinet/in.h>
#include <errno.h>
#include <pwd.h>                /* For getpwuid(). */
#include <signal.h>             /* For kill(). */
#include <sgtty.h>              /* For isatty(). */
#include <stdio.h>
#include <sys/file.h>           /* For open(). */

#include <sys/types.h>          /* For open() on Solaris. */
#include <sys/stat.h>           /* For open() on Solaris. */
#include <fcntl.h>              /* For open() on Solaris. */

#include "youbin.h"

/* 
 * Compatibility of getting current working directory pathname.
 *
 * SYSV:        getcwd().  
 * Otherwise:   BSD's getwd().
 */

#ifdef SYSV
#define getwd(x)    getcwd(x, MAXPATHLEN)
#endif /* SYSV */

#ifndef lint
static char rcsid[] =
    "$Id: client.c,v 4.1 1994/05/31 08:44:02 yamamoto Exp $";
#endif /* not lint */

static char *usage[] = {
    "NAME",
    "    youbin - client for the Internet \"youbin\" service",
    "SYNOPSIS",
    "    youbin [-b][-d][-h][-m file][-p prog][-s host]",
    "OPTIONS",
    "    -b       Biff mode: print some header fields and top of body.",
    "    -d       Debug mode.",
    "    -h       Help: print out the usage.",
    "    -m file  Mail check: file checked by sh or csh as mail spool.",
    "    -p prog  Program to be executed and connected via pipe.",
    "    -s host  Host name of mail server.",
    "AUTHORS",
    "    K.Agusa     agusa@nuie.nagoya-u.ac.jp",
    "    S.Yamamoto  yamamoto@nuie.nagoya-u.ac.jp",
    NULL
};

/*
 * Option string.
 * "B": Biff comptible (require some header fields and top of body).
 * "D": debug mode (not used yet).
 */
#define OPTION_STRING   ""

/*
 * Global variables.
 */

char    user_name[USER_NAME_LEN + 1];       /* User name. */
char    *prog_name;                         /* This program name. */
char    child_prog_name[MAXPATHLEN + 1];    /* Child program name. */
char    mail_check_name[MAXPATHLEN + 1];    /* For -m flag. */
char    *service_name = SERVICE_NAME;       /* Service name. */
char    server_name[MAXHOSTNAMELEN + 1];    /* Server name. */

char    *cr;
FILE    *input, *output;                    /* Pipe, stdin or stdout. */

/*
 * NONE:        Nothing is printed.
 * NOTIFY:      "New mail has arrived" is printed.
 * MESS_REQ:    Print some header fields and top of body are printed.
 */
enum    {NONE, NOTIFY, MESS_REQ} biff_mode = NONE;

/* Mode flag. */
int     debug_mode = OFF;

int     ppid;                               /* Parent process id */
int     exitcode = EXIT_SUCCESS;

/*
 * External variables.
 */

extern int      errno;

/* For getopt(). */
extern char     *optarg;
extern int      optind, opterr;

/*
 * Local functions.
 */

char            *ctime();

int             youbin_sub();
void            init();
void            biff();
void            mail_check();
void            pass_through();

int             make_writable_file();
void            send_signal();

int
main(argc, argv)
int     argc;
char    **argv;
{
    int     result;
    char    options[128];
    void    (*user_func)();
    
    init(argc, argv);

    if (child_prog_name[0] != '\0') {           /* -p option specified. */
        user_func = pass_through;
    } else if (mail_check_name[0] != '\0') {    /* -m option specified. */
        user_func = mail_check;
    } else if (isatty(fileno(output))) {        /* Invoked on a terminal. */
        user_func = biff;
    } else {        /* Called from other program or beging redirectted. */
        user_func = pass_through;
    }

    strcpy(options, OPTION_STRING);
    if (biff_mode == MESS_REQ) {
        strcat(options, "Biff");
    }
    
    result =  youbin_sub(server_name, options, user_func, -1);
    if (mail_check_name[0] != '\0') {
        unlink(mail_check_name);
    }
    debug("%s: Exit code: %d\n", prog_name, result);
    exit(result);
}

void
init(argc, argv)
int     argc;
char    **argv;
{
    int             c;
    struct passwd   *pwent;
#ifndef SYSV
    struct sgttyb   gtty;
#endif /* not SYSV */
    
    if ((prog_name = strrchr(argv[0], '/')) != NULL) {
        prog_name++;
    } else {
        prog_name = argv[0];
    }
    
    opterr = 0;
    while ((c = getopt(argc, argv, "bdhm:p:s:")) != EOF) {
        switch (c) {
          case 'b':                         /* Biff compatible. */
            biff_mode = MESS_REQ;
            if (mail_check_name[0] != '\0') {
                exitcode++;                 /* -b and -m are exclusive. */
            }
            break;
          case 'd':                         /* Debug. */
            debug_mode = ON;
            break;
          case 'h':                         /* Help. */
            print_usage(usage);
            exit(EXIT_SUCCESS);
            break;
          case 'm':                         /* Mail check. */
            strcpy(mail_check_name, optarg);
            if (biff_mode == MESS_REQ || child_prog_name[0] != '\0') {
                /* -b and -m are exclusive. -m and -p are also exclusive. */
                exitcode++;
            }
            break;
          case 'p':                         /* Child program. */
            strcpy(child_prog_name, optarg);
            if (mail_check_name[0] != '\0') {
                exitcode++;                 /* -m and -p are exclusive. */
            }
            break;
          case 's':                         /* Server host name. */
            strcpy(server_name, optarg);
            break;
          default:
            exitcode++;
            break;
        }
    }
    if (optind != argc || exitcode != EXIT_SUCCESS) {
        fprintf(stderr, "%s: Unknown options\n", prog_name);
        print_usage(usage);
        exit(EXIT_FAILURE);
    }
    
    ppid = getppid();                       /* Save parent PID for exiting */

    if (mail_check_name[0] != '\0') {       /* Mail check file for shells. */
        if (make_writable_file(mail_check_name) == FAIL) {
            fprintf(stderr, "%s: No access right to mail check file: %s\n",
                    prog_name, mail_check_name);
            exit(EXIT_FAILURE);
        }
    } else {
        if (biff_mode == NONE) {
            biff_mode = NOTIFY;
        }
    }

    /* Get local host name. */
    if (server_name[0] == '\0') {
        gethostname(server_name, sizeof(server_name));
    }
    
    /* Get user name. */
    if ((pwent = getpwuid(getuid())) != NULL) {
        strcpy(user_name, pwent->pw_name);
    } else {
        fprintf(stderr, "%s: Invalid user\n", prog_name);
        exit(EXIT_FAILURE);
    }
    
    /* Open pipe and set streams. */
    if (child_prog_name[0] != '\0') {       /* Child program specified. */
        char    command_buff[MAXPATHLEN + 1];
        
        sprintf(command_buff, "%s -pipe", child_prog_name);
        if ((output = popen(command_buff, "w")) == NULL) {
            perror("popen");
            exit(EXIT_FAILURE);
        }
        input = stdin;
    } else {
        input = stdin;
        output = stdout;
    }

#ifdef SYSV
    cr = "\r";
#else /* not SYSV */
    /* This code is available only on BSD. */
    (void)ioctl(fileno(stdout), TIOCGETP, &gtty);
    cr = ((gtty.sg_flags & CRMOD) && !(gtty.sg_flags & RAW)) ? "" : "\r";
#endif /* not SYSV */
}

/*
 * Called in biff mode.
 */

void
biff(size, date, mess)
int         size;
/* time_t      date; */
int         date;
char        *mess;
{
    char            *cp;
    static int      saved_size = 0;
    static time_t   saved_date = 0;
    
    /* 
     * debug("    biff(): size = %d, date = %d, mess = \"%s\"\n",
     *       size, date, mess);
     */
    
    if (getppid() != ppid) {                    /* Parent process died. */
        send_signal(getpid(), SIGQUIT);
        return;
    }
    
    if (size == saved_size && date == saved_date) {
        return;                                 /* Spool has not changed. */
    }
    saved_size = size;                          /* Remember the values. */
    saved_date = date;
    
    if (size == 0) {                            /* Do nothing. */
        return;
    }
    
    if (biff_mode != NONE) {
        /* Do not need to distinguish NOTIFY and MESS_REQ. */
        fprintf(stdout,
                "%s\n\007New mail for %s@%s\007 has arrived:%s\n----%s\n",
                cr, user_name, server_name, cr, cr);
        for (/* Empty. */; *mess != '\0'; mess = cp + 1) {
            if ((cp = strchr(mess, '\n')) == NULL) {
                break;
            }
            *cp = '\0';
            fprintf(stdout,"%s%s\n", mess, cr);
        }
        fflush(stdout);
    }
}

/*
 * Called in mail check mode.
 */

void
mail_check(size, date, mess)
int         size;
/* time_t      date; */
int         date;
char        *mess;
{
    static int      saved_size = 0;
    static time_t   saved_date = 0;
    char            buff[MESS_LEN + 1];
    int             fid;
    
    /* 
     * debug("    mail_check(): size = %d, date = %d, mess = \"%s\"\n",
     *       size, date, mess);
     */
    
    if (getppid() != ppid) {                    /* Parent process died. */
        send_signal(getpid(), SIGQUIT);
        return;
    }
    
    if (size == saved_size && date == saved_date) {
        return;                                 /* Spool has not changed. */
    }
    saved_size = size;                          /* Remember the values. */
    saved_date = date;
    
    if (size == 0) {                            /* Truncate mail check file. */
        if (mail_check_name[0] != '\0') {
            if (truncate( mail_check_name, 0L) == -1) {
                fprintf(stderr, "%s: Can not truncate file: %s\n",
                        prog_name, mail_check_name);
                perror("truncate:");
            }
        }
        return;
    }
    
    /* Size is not zero. New mail arrived. */
    if (mail_check_name[0] != '\0') {
        if ((fid = open(mail_check_name, (O_WRONLY | O_APPEND))) == -1) {
            fprintf(stderr,"%s: Can not re-open mail check file: %s\n",
                    prog_name, mail_check_name);
            perror("open");
            exit(EXIT_FAILURE);
        }
        sprintf(buff, "New mail for %s at %s", user_name, ctime(&date));
        /* Note, ctime() adds new line. */
        if (write(fid, buff, strlen(buff)) == -1) {
            fprintf(stderr,"%s: Can not write to mail check file: %s\n",
                    prog_name, mail_check_name);
            perror("write");
            exit(EXIT_FAILURE);
        }
        close(fid);
    }
}

/*
 * Called for passing through message.
 * For xyoubin.
 */

void
pass_through(size, date, mess)
int         size;
int         date;
char        *mess;
{
    /* 
     * debug("    pass_through(): size = %d, date = %d, mess = \"%s\"\n",
     *       size, date, mess);
     */
    
    if (getppid() != ppid) {                    /* Parent process died. */
        send_signal(getpid(), SIGQUIT);
        return;
    }
    
    /* Pipe requests a data in one packet. */
    if (mess[0] != '\0') {
        fprintf(output, "%d %d %s", size, date, mess);
    } else {
        fprintf(output, "%d %d", size, date);
    }
    fflush(output);
}

/*
 * Check file.
 * (1) If it exists, it should be writable by the user.
 * (2) If not exists, the directory should be writable by the user.
 *     Then make a file with mode 0600.
 */

int
make_writable_file(name_arg)
char    *name_arg;
{
    char    *cp, name[MAXPATHLEN + 1], pathname[MAXPATHLEN + 1];
    int     fid;
    
    strcpy(name, name_arg);             /* Preserve argument. */

    if (access(name, (F_OK | W_OK)) == 0){
        if (truncate(name, 0L)){
            fprintf(stderr, "%s: Can not truncate file: %s\n", prog_name, name);
            perror("truncate");
            return (FAIL);
        }
        return (OK);                    /* File exists and writable */
    }
    if (errno != ENOENT){
        perror("access");
        return (FAIL);
    }
    /* File does not exist. */
    if ((cp = strrchr(name, '/')) == NULL){
        /* File on currenct working directory. */
        if (getwd(pathname) == NULL){
            fprintf(stderr, "%s: Can not get working directry: %s\n",
                    prog_name, pathname);
            return (FAIL);
        }
        cp = name;
    } else {                            /* File name with prefix. */
        *cp = '\0';
        strcpy(pathname, ((cp == name) ? "/" : name));
        cp++; 
    }
    if (access(pathname, W_OK)){
        perror("access");
        return (FAIL);
    }
    strcat(pathname, "/");
    strcat(pathname, cp);
    if ((fid = open(pathname, (O_WRONLY | O_CREAT), 0600)) == -1){
        fprintf(stderr, "%s: Can not open file: %s\n", prog_name, pathname);
        perror("open");
        return (FAIL);
    }
    close(fid);
    if (chown(pathname, getuid(), getgid()) != 0) {
        fprintf(stderr, "%s: Can not change owner: %s\n", prog_name, pathname);
        return (FAIL);
    }
    return (OK);
}

void
send_signal(pid, sig)
int     sig;
{
    if (kill(pid, sig) == -1) {
        fprintf(stderr, "%s: Can not send signal %d to process %d\n",
                prog_name, sig, pid);
        perror("kill");
    }
}
