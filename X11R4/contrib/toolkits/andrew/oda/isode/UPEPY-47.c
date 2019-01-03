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
static char *rcsid = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R4/andrew/oda/isode/RCS/UPEPY-47.c,v 1.1 89/09/21 05:19:50 mss Exp $";
#endif

#ifndef PEPYPARM
#define PEPYPARM char *
#endif /* PEPYPARM */
extern PEPYPARM NullParm;

/* ARGSUSED */

int	parse_UNIV_EXTERNAL (pe, explicit, len, buffer, parm)
register PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    register PE p22;

#ifdef DEBUG
    (void) testdebug (pe, "UNIV.EXTERNAL");
#endif

    if (explicit
            && PE_ID (pe -> pe_class, pe -> pe_id)
                    != PE_ID (PE_CLASS_UNIV, 8)) {
        advise (NULLCP, "EXTERNAL bad class/id: %s/0x%x", 
                pe_classlist[pe -> pe_class], pe -> pe_id);
        return NOTOK;
    }
    if ((p22 = prim2seq (pe)) == NULLPE) {
        advise (NULLCP, "EXTERNAL bad sequence: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    pe = p22;

    {
        register PE p23;

        if ((p23 = first_member (pe)) != NULLPE
                && PE_ID (p23 -> pe_class, p23 -> pe_id)
                        != PE_ID (PE_CLASS_UNIV, 6))
            p23 = NULLPE;
        if (p23 != NULLPE) {
            p22 = p23;

            {	/* direct-reference */
#ifdef DEBUG
                (void) testdebug (p23, "direct-reference");
#endif

                if (p23 -> pe_class != PE_CLASS_UNIV
                        || p23 -> pe_form != PE_FORM_PRIM
                        || p23 -> pe_id != PE_PRIM_OID) {
                    advise (NULLCP, "direct-reference bad class/form/id: %s/%d/0x%x",
                            pe_classlist[p23 -> pe_class], p23 -> pe_form, p23 -> pe_id);
                    return NOTOK;
                }

                if (prim2oid (p23) == NULLOID) {
                    advise (NULLCP, "direct-reference bad object identifier: %s",
                            pe_error (p23 -> pe_errno));
                    return NOTOK;
                }
            }
        }
    }

    {
        register PE p24;

        if ((p24 = (pe != p22 ? next_member (pe, p22) : first_member (pe))) 
                    != NULLPE
                && PE_ID (p24 -> pe_class, p24 -> pe_id)
                        != PE_ID (PE_CLASS_UNIV, 2))
            p24 = NULLPE;
        if (p24 != NULLPE) {
            p22 = p24;

            {	/* indirect-reference */
#ifdef DEBUG
                (void) testdebug (p24, "indirect-reference");
#endif

                if (p24 -> pe_class != PE_CLASS_UNIV
                        || p24 -> pe_form != PE_FORM_PRIM
                        || p24 -> pe_id != PE_PRIM_INT) {
                    advise (NULLCP, "indirect-reference bad class/form/id: %s/%d/0x%x",
                            pe_classlist[p24 -> pe_class], p24 -> pe_form, p24 -> pe_id);
                    return NOTOK;
                }

                if (prim2num (p24) == NOTOK
                        && p24 -> pe_errno != PE_ERR_NONE) {
                    advise (NULLCP, "indirect-reference bad integer: %s",
                            pe_error (p24 -> pe_errno));
                    return NOTOK;
                }
            }
        }
        else {
            /* set default here using yp -> yp_default */
        }

    }

    {
        register PE p25;

        if ((p25 = (pe != p22 ? next_member (pe, p22) : first_member (pe))) 
                    != NULLPE
                && PE_ID (p25 -> pe_class, p25 -> pe_id)
                        != PE_ID (PE_CLASS_UNIV, 7))
            p25 = NULLPE;
        if (p25 != NULLPE) {
            p22 = p25;

            {	/* data-value-descriptor */
#ifdef DEBUG
                (void) testdebug (p25, "data-value-descriptor");
#endif

                if (parse_UNIV_ObjectDescriptor (p25, 1, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
        }
    }

    {
        register PE p26;

        if ((p26 = (pe != p22 ? next_member (pe, p22) : first_member (pe))) != NULLPE) {
            p22 = p26;

            {	/* encoding */
#ifdef DEBUG
                (void) testdebug (p26, "encoding");
#endif

                switch (PE_ID (p26 -> pe_class, p26 -> pe_id)) {
                    case PE_ID (PE_CLASS_CONT, 0):	/* single-ASN1-type */
                        {
                            register PE p27 = p26;

                            {	/* single-ASN1-type TAG PULLUP */
                                register PE p28;

                                if ((p28 = prim2set (p27)) == NULLPE) {
                                    advise (NULLCP, "single-ASN1-type bad single-ASN1-type: %s",
                                            pe_error (p27 -> pe_errno));
                                    return NOTOK;
                                }
                                if (p28 -> pe_cardinal != 1) {
                                    advise (NULLCP, "single-ASN1-type too many elements for tagged single-ASN1-type: %d",
                                            p28 -> pe_cardinal);
                                    return NOTOK;
                                }
                                p27 = first_member (p28);
                            }
                            {
#ifdef DEBUG
                                (void) testdebug (p27, "single-ASN1-type");
#endif

                            }
                        }
                        break;
                    case PE_ID (PE_CLASS_CONT, 1):	/* octet-aligned */
                        {
#ifdef DEBUG
                            (void) testdebug (p26, "octet-aligned");
#endif

                        }
                        break;
                    case PE_ID (PE_CLASS_CONT, 2):	/* arbitrary */
                        {
#ifdef DEBUG
                            (void) testdebug (p26, "arbitrary");
#endif

                            if (prim2bit (p26) == NULLPE) {
                                advise (NULLCP, "arbitrary bad bitstring: %s",
                                        pe_error (p26 -> pe_errno));
                                return NOTOK;
                            }
                        }
                        break;

                    default:
                        advise (NULLCP, "encoding has unknown choice: %s/%d/0x%x",
                                pe_classlist[p26 -> pe_class], p26 -> pe_form, p26 -> pe_id);
                        return NOTOK;
                }
            }
        }
        else {
            advise (NULLCP, "EXTERNAL missing encoding element");
            return NOTOK;
        }

    }


    if (pe -> pe_cardinal > 4) {
        advise (NULLCP, "EXTERNAL has too many elements(4): %d",
                pe -> pe_cardinal);
        return NOTOK;
    }

    return OK;
}
