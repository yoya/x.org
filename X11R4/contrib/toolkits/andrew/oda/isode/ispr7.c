/* automatically generated by pepy 5.0 #1 (mss.andrew.cmu.edu), do not edit! */

#include <psap.h>

static char *pepyid = "pepy 5.0 #1 (mss.andrew.cmu.edu) of Mon May 22 20:44:32 EDT 1989";

#define	advise	PY_advise

void	advise ();

/* Generated from module ISPR7 */
# line 5 "ispr7.py"


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

int	print_ISPR7_Default__Value__Lists__Layout (pe, explicit, len, buffer, parm)
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
            advise (NULLCP, "Default-Value-Lists-Layout bad class/form/id: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
        }
    }
    else
        if (pe -> pe_form != PE_FORM_CONS) {
            advise (NULLCP, "Default-Value-Lists-Layout bad form: %d", pe -> pe_form);
            return NOTOK;
        }

    if ((p0 = prim2set (pe)) == NULLPE) {
        advise (NULLCP, "Default-Value-Lists-Layout bad set: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    pe = p0;

    vpush ();
    if (p0 = set_find (pe, PE_CLASS_CONT, 2)) {
        vname ("page-attributes");
        if (print_ISPR7_Page__Attributes (p0, 0, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p0_count ++;
    }
    if (p0 = set_find (pe, PE_CLASS_CONT, 3)) {
        vname ("frame-attributes");
        if (print_ISPR7_Frame__Attributes (p0, 0, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p0_count ++;
    }
    if (p0 = set_find (pe, PE_CLASS_CONT, 4)) {
        vname ("block-attributes");
        if (print_ISPR7_Block__Attributes (p0, 0, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p0_count ++;
    }
    if (p0_count != pe -> pe_cardinal)
        advise (NULLCP, "warning: extra or duplicate members present in SET");
    vpop ();

    return OK;
}

/* ARGSUSED */

int	print_ISPR7_Default__Value__Lists__Logical (pe, explicit, len, buffer, parm)
register PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    int p1_count = 0;
    register PE p1;

    if (explicit) {
        if (pe -> pe_class != PE_CLASS_UNIV
                || pe -> pe_form != PE_FORM_CONS
                || pe -> pe_id != PE_CONS_SET) {
            advise (NULLCP, "Default-Value-Lists-Logical bad class/form/id: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
        }
    }
    else
        if (pe -> pe_form != PE_FORM_CONS) {
            advise (NULLCP, "Default-Value-Lists-Logical bad form: %d", pe -> pe_form);
            return NOTOK;
        }

    if ((p1 = prim2set (pe)) == NULLPE) {
        advise (NULLCP, "Default-Value-Lists-Logical bad set: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    pe = p1;

    vpush ();
    if (p1 = set_find (pe, PE_CLASS_CONT, 5)) {
        vname ("composite-logical-attributes");
        if (print_ISPR7_Composite__Logical__Attributes (p1, 0, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p1_count ++;
    }
    if (p1 = set_find (pe, PE_CLASS_CONT, 6)) {
        vname ("basic-logical-attributes");
        if (print_ISPR7_Basic__Logical__Attributes (p1, 0, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p1_count ++;
    }
    if (p1_count != pe -> pe_cardinal)
        advise (NULLCP, "warning: extra or duplicate members present in SET");
    vpop ();

    return OK;
}

/* ARGSUSED */

int	print_ISPR7_Page__Attributes (pe, explicit, len, buffer, parm)
register PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    int p2_count = 0;
    register PE p2;

    if (explicit) {
        if (pe -> pe_class != PE_CLASS_UNIV
                || pe -> pe_form != PE_FORM_CONS
                || pe -> pe_id != PE_CONS_SET) {
            advise (NULLCP, "Page-Attributes bad class/form/id: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
        }
    }
    else
        if (pe -> pe_form != PE_FORM_CONS) {
            advise (NULLCP, "Page-Attributes bad form: %d", pe -> pe_form);
            return NOTOK;
        }

    if ((p2 = prim2set (pe)) == NULLPE) {
        advise (NULLCP, "Page-Attributes bad set: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    pe = p2;

    vpush ();
    if ( (p2 = set_find (pe, PE_CLASS_CONT, 1)) ) {
        if (print_ISPR7_Attribute (p2, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p2_count ++;
    }
    if ( (p2 = set_find (pe, PE_CLASS_CONT, 2)) ) {
        if (print_ISPR7_Attribute (p2, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p2_count ++;
    }
    if ( (p2 = set_find (pe, PE_CLASS_CONT, 3)) ) {
        if (print_ISPR7_Attribute (p2, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p2_count ++;
    }
    if ( (p2 = set_find (pe, PE_CLASS_CONT, 4)) ) {
        if (print_ISPR7_Attribute (p2, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p2_count ++;
    }
    if ( (p2 = set_find (pe, PE_CLASS_CONT, 5)) ) {
        if (print_ISPR7_Attribute (p2, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p2_count ++;
    }
    if ( (p2 = set_find (pe, PE_CLASS_CONT, 6)) ) {
        if (print_ISPR7_Attribute (p2, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p2_count ++;
    }
    if ( (p2 = set_find (pe, PE_CLASS_CONT, 9)) ) {
        if (print_ISPR7_Attribute (p2, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p2_count ++;
    }
    if ( (p2 = set_find (pe, PE_CLASS_CONT, 11)) ) {
        if (print_ISPR7_Attribute (p2, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p2_count ++;
    }
    if (p2_count != pe -> pe_cardinal)
        advise (NULLCP, "warning: extra or duplicate members present in SET");
    vpop ();

    return OK;
}

/* ARGSUSED */

int	print_ISPR7_Frame__Attributes (pe, explicit, len, buffer, parm)
register PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    int p3_count = 0;
    register PE p3;

    if (explicit) {
        if (pe -> pe_class != PE_CLASS_UNIV
                || pe -> pe_form != PE_FORM_CONS
                || pe -> pe_id != PE_CONS_SET) {
            advise (NULLCP, "Frame-Attributes bad class/form/id: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
        }
    }
    else
        if (pe -> pe_form != PE_FORM_CONS) {
            advise (NULLCP, "Frame-Attributes bad form: %d", pe -> pe_form);
            return NOTOK;
        }

    if ((p3 = prim2set (pe)) == NULLPE) {
        advise (NULLCP, "Frame-Attributes bad set: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    pe = p3;

    vpush ();
    if ( (p3 = set_find (pe, PE_CLASS_CONT, 0)) ) {
        if (print_ISPR7_Attribute (p3, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p3_count ++;
    }
    if ( (p3 = set_find (pe, PE_CLASS_CONT, 1)) ) {
        if (print_ISPR7_Attribute (p3, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p3_count ++;
    }
    if ( (p3 = set_find (pe, PE_CLASS_CONT, 2)) ) {
        if (print_ISPR7_Attribute (p3, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p3_count ++;
    }
    if ( (p3 = set_find (pe, PE_CLASS_CONT, 4)) ) {
        if (print_ISPR7_Attribute (p3, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p3_count ++;
    }
    if ( (p3 = set_find (pe, PE_CLASS_CONT, 7)) ) {
        if (print_ISPR7_Attribute (p3, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p3_count ++;
    }
    if ( (p3 = set_find (pe, PE_CLASS_CONT, 11)) ) {
        if (print_ISPR7_Attribute (p3, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p3_count ++;
    }
    if ( (p3 = set_find (pe, PE_CLASS_CONT, 12)) ) {
        if (print_ISPR7_Attribute (p3, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p3_count ++;
    }
    if (p3_count != pe -> pe_cardinal)
        advise (NULLCP, "warning: extra or duplicate members present in SET");
    vpop ();

    return OK;
}

/* ARGSUSED */

int	print_ISPR7_Block__Attributes (pe, explicit, len, buffer, parm)
register PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    int p4_count = 0;
    register PE p4;

    if (explicit) {
        if (pe -> pe_class != PE_CLASS_UNIV
                || pe -> pe_form != PE_FORM_CONS
                || pe -> pe_id != PE_CONS_SET) {
            advise (NULLCP, "Block-Attributes bad class/form/id: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
        }
    }
    else
        if (pe -> pe_form != PE_FORM_CONS) {
            advise (NULLCP, "Block-Attributes bad form: %d", pe -> pe_form);
            return NOTOK;
        }

    if ((p4 = prim2set (pe)) == NULLPE) {
        advise (NULLCP, "Block-Attributes bad set: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    pe = p4;

    vpush ();
    if ( (p4 = set_find (pe, PE_CLASS_CONT, 0)) ) {
        if (print_ISPR7_Attribute (p4, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p4_count ++;
    }
    if ( (p4 = set_find (pe, PE_CLASS_CONT, 1)) ) {
        if (print_ISPR7_Attribute (p4, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p4_count ++;
    }
    if ( (p4 = set_find (pe, PE_CLASS_CONT, 2)) ) {
        if (print_ISPR7_Attribute (p4, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p4_count ++;
    }
    if ( (p4 = set_find (pe, PE_CLASS_CONT, 3)) ) {
        if (print_ISPR7_Attribute (p4, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p4_count ++;
    }
    if ( (p4 = set_find (pe, PE_CLASS_CONT, 9)) ) {
        if (print_ISPR7_Attribute (p4, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p4_count ++;
    }
    if ( (p4 = set_find (pe, PE_CLASS_CONT, 11)) ) {
        if (print_ISPR7_Attribute (p4, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p4_count ++;
    }
    if ( (p4 = set_find (pe, PE_CLASS_CONT, 12)) ) {
        if (print_ISPR7_Attribute (p4, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p4_count ++;
    }
    if (p4_count != pe -> pe_cardinal)
        advise (NULLCP, "warning: extra or duplicate members present in SET");
    vpop ();

    return OK;
}

/* ARGSUSED */

int	print_ISPR7_Composite__Logical__Attributes (pe, explicit, len, buffer, parm)
register PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    int p5_count = 0;
    register PE p5;

    if (explicit) {
        if (pe -> pe_class != PE_CLASS_UNIV
                || pe -> pe_form != PE_FORM_CONS
                || pe -> pe_id != PE_CONS_SET) {
            advise (NULLCP, "Composite-Logical-Attributes bad class/form/id: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
        }
    }
    else
        if (pe -> pe_form != PE_FORM_CONS) {
            advise (NULLCP, "Composite-Logical-Attributes bad form: %d", pe -> pe_form);
            return NOTOK;
        }

    if ((p5 = prim2set (pe)) == NULLPE) {
        advise (NULLCP, "Composite-Logical-Attributes bad set: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    pe = p5;

    vpush ();
    if ( (p5 = set_find (pe, PE_CLASS_CONT, 8)) ) {
        if (print_ISPR7_Attribute (p5, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p5_count ++;
    }
    if ( (p5 = set_find (pe, PE_CLASS_CONT, 10)) ) {
        if (print_ISPR7_Attribute (p5, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p5_count ++;
    }
    if (p5_count != pe -> pe_cardinal)
        advise (NULLCP, "warning: extra or duplicate members present in SET");
    vpop ();

    return OK;
}

/* ARGSUSED */

int	print_ISPR7_Basic__Logical__Attributes (pe, explicit, len, buffer, parm)
register PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    int p6_count = 0;
    register PE p6;

    if (explicit) {
        if (pe -> pe_class != PE_CLASS_UNIV
                || pe -> pe_form != PE_FORM_CONS
                || pe -> pe_id != PE_CONS_SET) {
            advise (NULLCP, "Basic-Logical-Attributes bad class/form/id: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
        }
    }
    else
        if (pe -> pe_form != PE_FORM_CONS) {
            advise (NULLCP, "Basic-Logical-Attributes bad form: %d", pe -> pe_form);
            return NOTOK;
        }

    if ((p6 = prim2set (pe)) == NULLPE) {
        advise (NULLCP, "Basic-Logical-Attributes bad set: %s",
                pe_error (pe -> pe_errno));
        return NOTOK;
    }
    pe = p6;

    vpush ();
    if ( (p6 = set_find (pe, PE_CLASS_CONT, 3)) ) {
        if (print_ISPR7_Attribute (p6, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p6_count ++;
    }
    if ( (p6 = set_find (pe, PE_CLASS_CONT, 8)) ) {
        if (print_ISPR7_Attribute (p6, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p6_count ++;
    }
    if ( (p6 = set_find (pe, PE_CLASS_CONT, 9)) ) {
        if (print_ISPR7_Attribute (p6, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p6_count ++;
    }
    if ( (p6 = set_find (pe, PE_CLASS_CONT, 10)) ) {
        if (print_ISPR7_Attribute (p6, 1, NULLIP, NULLVP, NullParm) == NOTOK)
            return NOTOK;
        p6_count ++;
    }
    if (p6_count != pe -> pe_cardinal)
        advise (NULLCP, "warning: extra or duplicate members present in SET");
    vpop ();

    return OK;
}

/* ARGSUSED */

int	print_ISPR7_Attribute (pe, explicit, len, buffer, parm)
PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    vpush ();
    switch (PE_ID (pe -> pe_class, pe -> pe_id)) {
        case PE_ID (PE_CLASS_CONT, 0):	/* position */
            {
                vname ("position");
                if (print_ISPR4_Measure__Pair (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 1):	/* dimensions */
            {
                vname ("dimensions");
                if (print_ISPR4_Measure__Pair (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 2):	/* transparency */
            {
                vname ("transparency");
                if (print_ISPR4_Transparency (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 3):	/* presentation-attributes */
            {
                vname ("presentation-attributes");
                if (print_ISPR6_Presentation__Attributes (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 4):	/* layout-path */
            {
                vname ("layout-path");
                if (print_ISPR4_One__Of__Four__Angles (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 5):	/* page-position */
            {
                vname ("page-position");
                if (print_ISPR4_Measure__Pair (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 6):	/* medium-type */
            {
                vname ("medium-type");
                if (print_ISPR4_Medium__Type (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 7):	/* permitted-categories */
            {
                register PE p7;

                vname ("permitted-categories");
                if ((p7 = prim2set (pe)) == NULLPE) {
                    advise (NULLCP, "permitted-categories bad set: %s",
                            pe_error (pe -> pe_errno));
                    return NOTOK;
                }
                pe = p7;

                vpush ();
                for (p7 = first_member (pe); p7; p7 = next_member (pe, p7)) {
                    if (print_ISPR3_Layout__Category__Name (p7, 1, NULLIP, NULLVP, NullParm) == NOTOK)
                        return NOTOK;
                }
                vpop ();
            }
            break;
        case PE_ID (PE_CLASS_CONT, 8):	/* protection */
            {
                vname ("protection");
                if (print_ISPR5_Protection (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 9):	/* presentation-style */
            {
                vname ("presentation-style");
                if (print_ISPR3_Style__Identifier (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 10):	/* layout-style */
            {
                vname ("layout-style");
                if (print_ISPR3_Style__Identifier (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 11):	/* colour */
            {
                vname ("colour");
                if (print_ISPR4_Colour (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 12):	/* border */
            {
                vname ("border");
                if (print_ISPR4_Border (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;

        default:
            advise (NULLCP, "Attribute has unknown choice: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
    }
    vpop ();

    return OK;
}
