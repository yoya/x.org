/*
 * Program:     $RCSfile: log.c,v $  $Revision: 4.1 $
 *
 * Purpose:     Log facility of internet "youbin" service.
 *
 * Author:      K.Agusa     agusa@nuie.nagoya-u.ac.jp
 *              S.Yamamoto  yamamoto@nuie.nagoya-u.ac.jp
 *
 * Date:        1993/12/27
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

#include <sys/types.h>
/* #include <sys/socket.h> */
#include <netinet/in.h>
#include <stdio.h>
#include <varargs.h>

#include "youbin.h"
#include "server.h"

#ifndef lint
static char rcsid[] =
    "$Id: log.c,v 4.1 1994/05/31 08:44:02 yamamoto Exp $";
#endif /* not lint */

#ifdef SYSLOG
#include <syslog.h>
#endif /* SYSLOG */

#ifndef SYSLOG_FACILITY
#define SYSLOG_FACILITY     LOG_DAEMON
#endif /* not SYSLOG_FACILITY */

#ifndef LOG_WARNING
#define LOG_WARNING         (-1)    /* Dummy. */
#endif /* not LOG_WARNING */

#ifndef LOG_INFO
#define LOG_INFO            (-1)    /* Dummy. */
#endif /* not LOG_INFO */

#ifndef LOG_ERR
#define LOG_ERR             (-1)    /* Dummy. */
#endif /* not LOG_ERR */

static int  flag_syslog = ON;       /* Always true. */
static int  flag_console = ON;      /* Always true. */

extern int          errno;
extern char         *sys_errlist[];

/*
 * Initialize.
 */ 

void
init_log()
{
#ifdef SYSLOG
    openlog(prog_name, (LOG_CONS | LOG_PID), SYSLOG_FACILITY);
#endif
}

/*
 * Top level functions.
 */ 

void
debug_log(va_alist)
va_dcl
{
    char        buff_log[LOG_LEN + 1], *format;
    va_list     args;
    
    if (debug_mode) {
        va_start(args);
        format = va_arg(args, char *);
        vsprintf(buff_log, format, args);
        va_end(args);
        output_log(LOG_DEBUG, buff_log);
    }
}

void
warn_log(va_alist)
va_dcl
{
    char        buff_log[LOG_LEN + 1], *format;
    va_list     args;
    
    va_start(args);
    format = va_arg(args, char *);
    vsprintf(buff_log, format, args);
    va_end(args);
    output_log(LOG_WARNING, buff_log);
}

void
info_log(va_alist)
va_dcl
{
    char        buff_log[LOG_LEN + 1], *format;
    va_list     args;
    
    va_start(args);
    format = va_arg(args, char *);
    vsprintf(buff_log, format, args);
    va_end(args);
    output_log(LOG_INFO, buff_log);
}

void
error_log(va_alist)
va_dcl
{
    char        buff_log[LOG_LEN + 1], *format;
    va_list     args;
    
    va_start(args);
    format = va_arg(args, char *);
    vsprintf(buff_log, format, args);
    va_end(args);
    output_log(LOG_ERR, buff_log);
}

void
trace(va_alist)
va_dcl
{
    /* Output trace message to syslog with LOG_INFO level. */
    char        buff_log[LOG_LEN + 1], *format;
    va_list     args;
    
    if (trace_mode) {
        va_start(args);
        format = va_arg(args, char *);
        vsprintf(buff_log, format, args);
        va_end(args);
        output_log(LOG_INFO, buff_log);
    }
}

void
sys_error_log(mess)
char    *mess;
{
    /* Output system error messages to syslog. */
    char        buff_log[LOG_LEN + 1];

    sprintf(buff_log, "%s: %s\n", mess, sys_errlist[errno]);
    output_log(LOG_ERR, buff_log);
}

/*
 * Subroutines.
 */

void
output_log(syslog_level, mess) 
int         syslog_level; 
char        *mess;
{
#ifdef SYSLOG
    if (flag_syslog) {
        syslog(syslog_level, mess);
    }
#endif /* SYSLOG */

    if (flag_console) {
        fprintf(stderr, "%s: %s", prog_name, mess); 
    }
}
