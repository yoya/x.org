/*
Copyright 1988, 1989 Marshall T. Rose

The ISODE is openly available but is NOT in the public domain. You are
allowed and encouraged to take this software and build commercial
products. However, as a condition of use, you are required to "hold
harmless" all contributors.
Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted,
provided that this notice and the reference to this notice appearing
in each software module be retained unaltered, and that the name of
any contributors shall not be used in advertising or publicity pertaining
to distribution of the software without specific written prior permission.
No contributor makes any representations about the suitability of this
software for any purpose. It is provided "as is" without express or
implied warranty.

ALL CONTRIBUTORS DISCLAIM ALL WARRANTIES WITH
REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED
WARRANTIES OR MERCHANTIBILITY AND FITNESS. IN
NO EVENT SHALL ANY CONTRIBUTOR BE LIABLE FOR
ANY SPECIAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
OF USE, DATA OR PROFITS, WHETHER IN ACTION OF
CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFWTARE.

As used above, "contributor" includes, but is not limited to:

	The MITRE Corporation
	The Northrop Corporation
	University College London
	The University of Nottingham
	The Wollongong Group, Inc.
	Marshall T. Rose

In particular, the Northrop Coporation provided the initial
sponsorship for the ISODE and teh Wollongong Group, Inc.,
continued supporting this effort.
*/
/* automatically generated by pepy 5.0 #2 (mss.andrew.cmu.edu), do not edit! */

#include "psap.h"

#define	advise	PY_advise

void	advise ();

/* Generated from module UNIV */

#ifndef	lint
static char *rcsid = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/oda/isode/RCS/UPEPY-03.c,v 1.1 89/09/21 05:17:56 mss Exp $";
#endif

#ifndef PEPYPARM
#define PEPYPARM char *
#endif /* PEPYPARM */
extern PEPYPARM NullParm;

/* ARGSUSED */

int	print_UNIV_IA5String (pe, explicit, len, buffer, parm)
register PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    register char *p2;
    int p2_len;

    if (explicit
            && PE_ID (pe -> pe_class, pe -> pe_id)
                    != PE_ID (PE_CLASS_UNIV, 22)) {
        advise (NULLCP, "IA5String bad class/id: %s/0x%x", 
                pe_classlist[pe -> pe_class], pe -> pe_id);
        return NOTOK;
    }
    if ((p2 = prim2str (pe, &p2_len)) == NULLCP) {
        advise (NULLCP, "IA5String bad octetstring: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    vstring (pe);
    if (len)
        *len = p2_len;
    if (buffer)
        *buffer = p2;
    else
        if (p2)
            free (p2);

    return OK;
}
