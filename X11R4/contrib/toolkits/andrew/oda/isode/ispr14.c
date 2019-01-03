/* automatically generated by pepy 5.0 #1 (mss.andrew.cmu.edu), do not edit! */

#include <psap.h>

static char *pepyid = "pepy 5.0 #1 (mss.andrew.cmu.edu) of Mon May 22 20:44:32 EDT 1989";

#define	advise	PY_advise

void	advise ();

/* Generated from module ISPR14 */
# line 5 "ispr14.py"


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


#ifndef PEPYPARM
#define PEPYPARM char *
#endif /* PEPYPARM */
extern PEPYPARM NullParm;

/* ARGSUSED */

int	print_ISPR14_Ra__Gr__Presentation__Features (pe, explicit, len, buffer, parm)
PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    vpush ();
    switch (PE_ID (pe -> pe_class, pe -> pe_id)) {
        case PE_ID (PE_CLASS_CONT, 0):	/* pel-path */
            {
                vname ("pel-path");
                if (print_ISPR12_One__Of__Four__Angles (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 1):	/* line-progression */
            {
                vname ("line-progression");
                if (print_ISPR12_One__Of__Two__Angles (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 2):	/* pel-transmission-density */
            {
                vname ("pel-transmission-density");
                if (print_ISPR12_Pel__Transmission__Density (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;

        default:
            advise (NULLCP, "Ra-Gr-Presentation-Features has unknown choice: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
    }
    vpop ();

    return OK;
}

/* ARGSUSED */

int	print_ISPR14_Raster__Gr__Content__Defaults (pe, explicit, len, buffer, parm)
register PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    int p0_count = 0;
    register PE p0;

    if (explicit) {
        if (pe -> pe_class != PE_CLASS_UNIV
                || pe -> pe_form != PE_FORM_CONS
                || pe -> pe_id != PE_CONS_SET) {
            advise (NULLCP, "Raster-Gr-Content-Defaults bad class/form/id: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
        }
    }
    else
        if (pe -> pe_form != PE_FORM_CONS) {
            advise (NULLCP, "Raster-Gr-Content-Defaults bad form: %d", pe -> pe_form);
            return NOTOK;
        }

    if ((p0 = prim2set (pe)) == NULLPE) {
        advise (NULLCP, "Raster-Gr-Content-Defaults bad set: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    pe = p0;

    vpush ();
    if (p0 = set_find (pe, PE_CLASS_CONT, 0)) {
        vname ("pel-path");
        if (print_ISPR12_One__Of__Four__Angles (p0, 0, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p0_count ++;
    }
    if (p0 = set_find (pe, PE_CLASS_CONT, 1)) {
        vname ("line-progression");
        if (print_ISPR12_One__Of__Two__Angles (p0, 0, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p0_count ++;
    }
    if (p0 = set_find (pe, PE_CLASS_CONT, 2)) {
        vname ("pel-transmission-density");
        if (print_ISPR12_Pel__Transmission__Density (p0, 0, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p0_count ++;
    }
    if (p0 = set_find (pe, PE_CLASS_CONT, 5)) {
        register PE p1 = p0;

        {	/* pel-spacing TAG PULLUP */
            register PE p2;

            if ((p2 = prim2set (p1)) == NULLPE) {
                advise (NULLCP, "pel-spacing bad pel-spacing: %s",
                        pe_error (p1 -> pe_errno));
                return NOTOK;
            }
            if (p2 -> pe_cardinal != 1) {
                advise (NULLCP, "pel-spacing too many elements for tagged pel-spacing: %d",
                        p2 -> pe_cardinal);
                return NOTOK;
            }
            p1 = first_member (p2);
        }
        {
            vname ("pel-spacing");
            if (print_ISPR12_Pel__Spacing (p1, 1, NULLIP, NULLVP, NullParm) == NOTOK)
                return NOTOK;
        }
        p0_count ++;
    }
    if (p0 = set_find (pe, PE_CLASS_CONT, 6)) {
        vname ("spacing-ratio");
        if (print_ISPR12_Spacing__Ratio (p0, 0, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p0_count ++;
    }
    if (p0 = set_find (pe, PE_CLASS_CONT, 8)) {
        vname ("compression");
        if (print_ISPR13_Compression (p0, 0, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p0_count ++;
    }
    if (p0_count != pe -> pe_cardinal)
        advise (NULLCP, "warning: extra or duplicate members present in SET");
    vpop ();

    return OK;
}