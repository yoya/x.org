/* automatically generated by pepy 5.0 #1 (mss.andrew.cmu.edu), do not edit! */

#include <psap.h>

static char *pepyid = "pepy 5.0 #1 (mss.andrew.cmu.edu) of Mon May 22 20:44:32 EDT 1989";

#define	advise	PY_advise

void	advise ();

/* Generated from module ISPR1 */
# line 5 "ispr1.py"


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

int	print_ISPR1_Interchange__Data__Element (pe, explicit, len, buffer, parm)
PE	pe;
int	explicit;
int    *len;
char  **buffer;
PEPYPARM parm;
{
    vpush ();
    switch (PE_ID (pe -> pe_class, pe -> pe_id)) {
        case PE_ID (PE_CLASS_CONT, 0):	/* document-profile */
            {
                vname ("document-profile");
                if (print_ISPR2_Document__Profile__Descriptor (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 1):	/* layout-object-class */
            {
                vname ("layout-object-class");
                if (print_ISPR4_Layout__Class__Descriptor (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 2):	/* layout-object */
            {
                vname ("layout-object");
                if (print_ISPR4_Layout__Object__Descriptor (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 3):	/* content-portion */
            {
                vname ("content-portion");
                if (print_ISPR8_Text__Unit (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 5):	/* logical-object-class */
            {
                vname ("logical-object-class");
                if (print_ISPR5_Logical__Class__Descriptor (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 6):	/* logical-object */
            {
                vname ("logical-object");
                if (print_ISPR5_Logical__Object__Descriptor (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 7):	/* presentation-style */
            {
                vname ("presentation-style");
                if (print_ISPR6_Presentation__Style__Descriptor (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;
        case PE_ID (PE_CLASS_CONT, 8):	/* layout-style */
            {
                vname ("layout-style");
                if (print_ISPR6_Layout__Style__Descriptor (pe, 0, NULLIP, NULLVP, NullParm) == NOTOK)
                    return NOTOK;
            }
            break;

        default:
            advise (NULLCP, "Interchange-Data-Element has unknown choice: %s/%d/0x%x",
                    pe_classlist[pe -> pe_class], pe -> pe_form, pe -> pe_id);
            return NOTOK;
    }
    vpop ();

    return OK;
}