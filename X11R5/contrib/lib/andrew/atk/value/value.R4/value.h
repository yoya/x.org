/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1991 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* $Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/atk/value/value.R4/RCS/value.h,v 1.1 1991/09/30 18:16:13 susan Exp $ */
/* $ACIS: $ */
/* $Source: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/atk/value/value.R4/RCS/value.h,v $ */

#if !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS)
static char *rcsid_h = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/atk/value/value.R4/RCS/value.h,v 1.1 1991/09/30 18:16:13 susan Exp $ ";
#endif /* !defined(lint) && !defined(LOCORE) && defined(RCS_HDRS) */

/* Values for Notify Observers */

#define value_NOCHANGE		0
#define value_BADVALUE		1
#define value_NEWVALUE		2
#define	value_NOTACTIVE		3
#define value_ACTIVE		4

