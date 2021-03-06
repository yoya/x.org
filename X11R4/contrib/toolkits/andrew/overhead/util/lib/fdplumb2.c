/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1989 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* $Header: /afs/.andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/overhead/util/lib/RCS/fdplumb2.c,v 2.4 89/02/10 23:25:13 ghoti Exp $ */
/* $Source: /afs/.andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/overhead/util/lib/RCS/fdplumb2.c,v $ */

#ifndef lint
static char *rcsid = "$Header: ";
#endif /* lint */

#include <stdio.h>
#include <fdplumbi.h>

dbg_vclose(fd)
int fd;
{
    RegisterCloseFile(fd);
    return(vclose(fd));
}

dbg_vfclose(fp)
FILE *fp;
{
    RegisterCloseFile(fileno(fp));
    return(vfclose(fp));
}

