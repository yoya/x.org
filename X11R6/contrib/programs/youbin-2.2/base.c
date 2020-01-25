/*
 * Program:     $RCSfile: base.c,v $  $Revision: 4.1 $
 *
 * Purpose:     Basic routines of internet "youbin" service.
 *
 * Author:      K.Agusa     agusa@nuie.nagoya-u.ac.jp
 *              S.Yamamoto  yamamoto@nuie.nagoya-u.ac.jp
 *
 * Date:        1993/07/27
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

#ifndef lint
static char rcsid[] =
    "$Id: base.c,v 4.1 1994/05/31 08:44:02 yamamoto Exp $";
#endif /* not lint */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <varargs.h>

#include "youbin.h"

void
print_usage(usage)
char    **usage;
{
    while (*usage != NULL) {
        fprintf(stderr, "%s\n", *usage++);
    }
    fprintf(stderr, "VERSION\n");
    fprintf(stderr, "    Version           %s\n", VERSION);
    fprintf(stderr, "    Patch level       %s\n", PATCH_LEVEL);
    fprintf(stderr, "    Protocol version  %s\n", PROTOCOL_VERSION);
}

void
debug(va_alist)
va_dcl
{
    char        *format;
    va_list     args;
    
    if (debug_mode) {
        va_start(args);
        format = va_arg(args, char *);
        vfprintf(stderr, format, args);
        va_end(args);
    }
}

/*
 * Convert IP address to string.
 */

void
addr_to_str(buff, addr)
char        *buff;
SockAddr    addr;
{
    union {
        unsigned long   inet;
        unsigned char   c[4];
    } union_addr;
    
    union_addr.inet = addr.sin_addr.s_addr;
    sprintf(buff, "%d.%d.%d.%d",
            union_addr.c[0], union_addr.c[1], union_addr.c[2], union_addr.c[3]);
}
