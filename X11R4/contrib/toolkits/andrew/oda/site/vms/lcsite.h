
/*
$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/oda/site/vms/RCS/lcsite.h,v 1.1 89/09/21 06:11:23 mss Exp $
$Source: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/oda/site/vms/RCS/lcsite.h,v $

(c) Copyright 1988 Carnegie Mellon University

See "The CMU ODA Tool Kit" (available as CMU/ITC tech report 
CMU-ITC-071) for full text & permitted uses.

$Log:	lcsite.h,v $
 * Revision 1.1  89/09/21  06:11:23  mss
 * Initial revision
 * 
 * Revision 1.4  89/09/11  13:22:17  mss
 * changed error_pointer
 * 
 * Revision 1.3  88/11/03  11:42:10  jr
 * Remove defn of ONES_COMPLEMENT
 * 
 * Revision 1.2  88/09/30  14:46:10  mss
 * Changes BITS_PER_INT to not be casted so that VMS doesn't complain
 * 
 * Revision 1.7  88/08/29  16:26:33  jr
 * Add ERROR_MESG_SIZE
 * 
 * Revision 1.6  88/08/15  16:07:31  mss
 * Casted (clothed?) all naked constants so they have the
 * appropriate type
 * 
 * Revision 1.5  88/07/28  10:00:41  mz0k
 * Typo error corrected.
 * 
 * Revision 1.4  88/07/27  12:46:17  mz0k
 * Added BITS_PER_INT macro
 * 
 * Revision 1.3  88/06/08  17:26:43  annm
 * no change
 * 
 * Revision 1.2  88/06/08  15:54:51  annm
 * added copyright notice
 * 
*/

/*

	Site-dependent include file for Andrew

*/

#ifndef TK_INCLUDED_SITE
#define TK_INCLUDED_SITE

    /* Integer type */
typedef int		    INT_type;

    /* We use 2's complement */
    /* So, don't define the macro ONES_COMPLEMENT */

    /* Maximum & minimum integers */
#define MAXIMUM_INT	    (0x7FFFFFFF)
#define MINIMUM_INT	    (0x80000001)

    /* Bits in an integer -- note that VMS doesn't like casting in array bounds */
#define	BITS_PER_INT	    32

    /* Character type */
typedef char		    CHAR_type;
#define MakeChar(c)	    ( (CHAR_type) c)

    /* Byte type */
typedef unsigned char	    BYTE_type;
#define MakeByte(b)	    ( (BYTE_type) b)

    /* Boolean type */
typedef BYTE_type	    BOOL_type;

    /* TRUE & FALSE */
#define BOOL_false	    ( (BOOL_type) 0)
#define BOOL_true	    ( (BOOL_type) 1)

    /* Values */
typedef short int	    VALUE_type;


    /* Tags */
typedef short int	    TAG_type;

    /* Parameter mask */
typedef int		    PARM_MASK_type;

    /* Arbitrary pointers */
typedef char		    *POINTER_type;
#define LCL_ERROR_POINTER	    ((POINTER_type) 0)

    /* We use register's */
#define REGISTER	    register

    /* Size for error message buffer */
#define ERROR_MESG_SIZE	    (200)

#endif
