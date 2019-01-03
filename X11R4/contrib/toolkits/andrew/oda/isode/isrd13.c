/* automatically generated by pepy 5.0 #1 (mss.andrew.cmu.edu), do not edit! */

#include <psap.h>

static char *pepyid = "pepy 5.0 #1 (mss.andrew.cmu.edu) of Mon May 22 20:44:32 EDT 1989";

#define	advise	PY_advise

void	advise ();

/* Generated from module ISRD13 */
# line 5 "isrd13.py"


/*
 *
 * (C) Copyright 1989 by Carnegie Mellon University
 *
 * Permission to use, copy, modify, and distribute these programs
 * and their documentation for any purpose and without fee is
 * hereby granted, provided that this copyright and permission
 * notice appear on all copies and supporting documentation, and
 * that the name of Carnegie Mellon University not be used in
 * advertising or publicity pertaining to distribution of the
 * programs without specific prior permission and notice be given
 * in supporting documentation that copying and distribution is
 * by permission of Carnegie Mellon University.
 *
 * Carnegie Mellon University makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * as is, without express or implied warranty.
 *
 * Software by Ann Marks and James T. Lui,
 * Information Technology Center, Carnegie Mellon University,
 * except where noted.
 *
 */

#include <odatk.h>

#define TK_IMPL_ISRD

#include <isrd.h>

#undef TK_IMPL_ISRD

#define PEPYPARM struct isode2toolkit_t *


#ifndef PEPYPARM
#define PEPYPARM char *
#endif /* PEPYPARM */
extern PEPYPARM NullParm;

/* ARGSUSED */

int	parse_ISRD13_Raster__Gr__Coding__Attributes (pe, explicit, len, buffer, parm)
register PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    int p0_count = 0;
    register PE p0;

#ifdef DEBUG
    (void) testdebug (pe, "ISRD13.Raster-Gr-Coding-Attributes");
#endif

    if (explicit) {
        if (pe -> pe_class != PE_CLASS_UNIV
                || pe -> pe_form != PE_FORM_CONS
                || pe -> pe_id != PE_CONS_SET) {
            advise (NULLCP, "Raster-Gr-Coding-Attributes bad class/form/id: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
        }
    }
    else
        if (pe -> pe_form != PE_FORM_CONS) {
            advise (NULLCP, "Raster-Gr-Coding-Attributes bad form: %d", pe -> pe_form);
            return NOTOK;
        }

    if ((p0 = prim2set (pe)) == NULLPE) {
        advise (NULLCP, "Raster-Gr-Coding-Attributes bad set: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    pe = p0;

    if (p0 = set_find (pe, PE_CLASS_CONT, 0)) {
#ifdef DEBUG
        (void) testdebug (p0, "number-of-pels-per-line");
#endif

        if (prim2num (p0) == NOTOK
                && p0 -> pe_errno != PE_ERR_NONE) {
            advise (NULLCP, "number-of-pels-per-line bad integer: %s",
                    pe_error (p0 -> pe_errno));
            return NOTOK;
        }
        p0_count ++;
    }
    else {
        /* set default here using yp -> yp_default */
    }

    if (p0 = set_find (pe, PE_CLASS_CONT, 1)) {
#ifdef DEBUG
        (void) testdebug (p0, "number-of-lines");
#endif

        if (prim2num (p0) == NOTOK
                && p0 -> pe_errno != PE_ERR_NONE) {
            advise (NULLCP, "number-of-lines bad integer: %s",
                    pe_error (p0 -> pe_errno));
            return NOTOK;
        }
        p0_count ++;
    }
    else {
        /* set default here using yp -> yp_default */
    }

    if (p0 = set_find (pe, PE_CLASS_CONT, 2)) {
#ifdef DEBUG
        (void) testdebug (p0, "compression");
#endif

        if (parse_ISRD13_Compression (p0, 0, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p0_count ++;
    }
    if (p0 = set_find (pe, PE_CLASS_CONT, 3)) {
#ifdef DEBUG
        (void) testdebug (p0, "number-of-discarded-pels");
#endif

        if (prim2num (p0) == NOTOK
                && p0 -> pe_errno != PE_ERR_NONE) {
            advise (NULLCP, "number-of-discarded-pels bad integer: %s",
                    pe_error (p0 -> pe_errno));
            return NOTOK;
        }
        p0_count ++;
    }
    else {
        /* set default here using yp -> yp_default */
    }

    if (p0_count != pe -> pe_cardinal)
        advise (NULLCP, "warning: extra or duplicate members present in SET");

    return OK;
}

/* ARGSUSED */

int	parse_ISRD13_Compression (pe, explicit, len, buffer, parm)
register PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    register int p1;

#ifdef DEBUG
    (void) testdebug (pe, "ISRD13.Compression");
#endif

    if (explicit) {
        if (pe -> pe_class != PE_CLASS_UNIV
                || pe -> pe_form != PE_FORM_PRIM
                || pe -> pe_id != PE_PRIM_INT) {
            advise (NULLCP, "Compression bad class/form/id: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
        }
    }
    else
        if (pe -> pe_form != PE_FORM_PRIM) {
            advise (NULLCP, "Compression bad form: %d", pe -> pe_form);
            return NOTOK;
        }

    if ((p1 = prim2num (pe)) == NOTOK
            && pe -> pe_errno != PE_ERR_NONE) {
        advise (NULLCP, "Compression bad integer: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    if (len)
        *len = p1;
    switch (p1) {
        case 0:	/* uncompressed */
            break;
        case 1:	/* compressed */
            break;
        default:
            advise (NULLCP, "Compression has unknown component: %d", p1);
            return NOTOK;
    }

    return OK;
}