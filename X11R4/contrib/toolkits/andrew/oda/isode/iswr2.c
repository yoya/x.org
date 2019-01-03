/* automatically generated by pepy 5.0 #1 (mss.andrew.cmu.edu), do not edit! */

#include <psap.h>

static char *pepyid = "pepy 5.0 #1 (mss.andrew.cmu.edu) of Mon May 22 20:44:32 EDT 1989";

#define	advise	PY_advise

void	advise ();

/* Generated from module ISWR2 */
# line 4 "iswr2.py"


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

#define TK_IMPL_ISWR

#include <iswr.h>

#undef TK_IMPL_ISWR

#define PEPYPARM struct isode2toolkit_t *


#ifndef PEPYPARM
#define PEPYPARM char *
#endif /* PEPYPARM */
extern PEPYPARM NullParm;

/* ARGSUSED */

int	build_ISWR2_Document__Profile__Descriptor (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p0_z = NULLPE;
    register PE *p0 = &p0_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Document-Profile-Descriptor: out of memory");
        return NOTOK;
    }
    (*p0) = NULLPE;

    if ((*p0) != NULLPE)
        if (set_add ((*pe), (*p0)) == NOTOK) {
            advise (NULLCP, "Document-Profile-Descriptor bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p0) = NULLPE;

    if ((*p0) != NULLPE)
        if (set_add ((*pe), (*p0)) == NOTOK) {
            advise (NULLCP, "Document-Profile-Descriptor bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p0) = NULLPE;

    if ((*p0) != NULLPE)
        if (set_add ((*pe), (*p0)) == NOTOK) {
            advise (NULLCP, "Document-Profile-Descriptor bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p0) = NULLPE;

    if ((*p0) != NULLPE)
        if (set_add ((*pe), (*p0)) == NOTOK) {
            advise (NULLCP, "Document-Profile-Descriptor bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p0) = NULLPE;

    if ((*p0) != NULLPE)
        if (set_add ((*pe), (*p0)) == NOTOK) {
            advise (NULLCP, "Document-Profile-Descriptor bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p0) = NULLPE;

    if ((*p0) != NULLPE)
        if (set_add ((*pe), (*p0)) == NOTOK) {
            advise (NULLCP, "Document-Profile-Descriptor bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p0) = NULLPE;

    if ((*p0) != NULLPE)
        if (set_add ((*pe), (*p0)) == NOTOK) {
            advise (NULLCP, "Document-Profile-Descriptor bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p0) = NULLPE;

    if ((*p0) != NULLPE)
        if (set_add ((*pe), (*p0)) == NOTOK) {
            advise (NULLCP, "Document-Profile-Descriptor bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p0) = NULLPE;

    if ((*p0) != NULLPE)
        if (set_add ((*pe), (*p0)) == NOTOK) {
            advise (NULLCP, "Document-Profile-Descriptor bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p0) = NULLPE;

    if ((*p0) != NULLPE)
        if (set_add ((*pe), (*p0)) == NOTOK) {
            advise (NULLCP, "Document-Profile-Descriptor bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p0) = NULLPE;

    if ((*p0) != NULLPE)
        if (set_add ((*pe), (*p0)) == NOTOK) {
            advise (NULLCP, "Document-Profile-Descriptor bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Document-Profile-Descriptor");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Document__Characteristics (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p1_z = NULLPE;
    register PE *p1 = &p1_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Document-Characteristics: out of memory");
        return NOTOK;
    }
    (*p1) = NULLPE;

    if ((*p1) != NULLPE)
        if (set_add ((*pe), (*p1)) == NOTOK) {
            advise (NULLCP, "Document-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p1) = NULLPE;

    if ((*p1) != NULLPE)
        if (set_add ((*pe), (*p1)) == NOTOK) {
            advise (NULLCP, "Document-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p1) = NULLPE;

    if ((*p1) != NULLPE)
        if (set_add ((*pe), (*p1)) == NOTOK) {
            advise (NULLCP, "Document-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p1) = NULLPE;

    if ((*p1) != NULLPE)
        if (set_add ((*pe), (*p1)) == NOTOK) {
            advise (NULLCP, "Document-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p1) = NULLPE;

    if ((*p1) != NULLPE)
        if (set_add ((*pe), (*p1)) == NOTOK) {
            advise (NULLCP, "Document-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p1) = NULLPE;

    if ((*p1) != NULLPE)
        if (set_add ((*pe), (*p1)) == NOTOK) {
            advise (NULLCP, "Document-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p1) = NULLPE;

    if ((*p1) != NULLPE)
        if (set_add ((*pe), (*p1)) == NOTOK) {
            advise (NULLCP, "Document-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p1) = NULLPE;

    if ((*p1) != NULLPE)
        if (set_add ((*pe), (*p1)) == NOTOK) {
            advise (NULLCP, "Document-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p1) = NULLPE;

    if ((*p1) != NULLPE)
        if (set_add ((*pe), (*p1)) == NOTOK) {
            advise (NULLCP, "Document-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Document-Characteristics");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Doc__Appl__Profile__Defaults (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p2_z = NULLPE;
    register PE *p2 = &p2_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Doc-Appl-Profile-Defaults: out of memory");
        return NOTOK;
    }
    (*p2) = NULLPE;

    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Doc-Appl-Profile-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Doc-Appl-Profile-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Doc-Appl-Profile-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Doc-Appl-Profile-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Doc-Appl-Profile-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Doc-Appl-Profile-Defaults");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Document__Architecture__Defaults (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p3_z = NULLPE;
    register PE *p3 = &p3_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Document-Architecture-Defaults: out of memory");
        return NOTOK;
    }
    (*p3) = NULLPE;

    if ((*p3) != NULLPE)
        if (set_add ((*pe), (*p3)) == NOTOK) {
            advise (NULLCP, "Document-Architecture-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p3) = NULLPE;

    if ((*p3) != NULLPE)
        if (set_add ((*pe), (*p3)) == NOTOK) {
            advise (NULLCP, "Document-Architecture-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p3) = NULLPE;

    if ((*p3) != NULLPE)
        if (set_add ((*pe), (*p3)) == NOTOK) {
            advise (NULLCP, "Document-Architecture-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p3) = NULLPE;

    if ((*p3) != NULLPE)
        if (set_add ((*pe), (*p3)) == NOTOK) {
            advise (NULLCP, "Document-Architecture-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p3) = NULLPE;

    if ((*p3) != NULLPE)
        if (set_add ((*pe), (*p3)) == NOTOK) {
            advise (NULLCP, "Document-Architecture-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p3) = NULLPE;

    if ((*p3) != NULLPE)
        if (set_add ((*pe), (*p3)) == NOTOK) {
            advise (NULLCP, "Document-Architecture-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p3) = NULLPE;

    if ((*p3) != NULLPE)
        if (set_add ((*pe), (*p3)) == NOTOK) {
            advise (NULLCP, "Document-Architecture-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p3) = NULLPE;

    if ((*p3) != NULLPE)
        if (set_add ((*pe), (*p3)) == NOTOK) {
            advise (NULLCP, "Document-Architecture-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p3) = NULLPE;

    if ((*p3) != NULLPE)
        if (set_add ((*pe), (*p3)) == NOTOK) {
            advise (NULLCP, "Document-Architecture-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p3) = NULLPE;

    if ((*p3) != NULLPE)
        if (set_add ((*pe), (*p3)) == NOTOK) {
            advise (NULLCP, "Document-Architecture-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p3) = NULLPE;

    if ((*p3) != NULLPE)
        if (set_add ((*pe), (*p3)) == NOTOK) {
            advise (NULLCP, "Document-Architecture-Defaults bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Document-Architecture-Defaults");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Non__Basic__Doc__Characteristics (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p4_z = NULLPE;
    register PE *p4 = &p4_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Non-Basic-Doc-Characteristics: out of memory");
        return NOTOK;
    }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p4) = NULLPE;

    if ((*p4) != NULLPE)
        if (set_add ((*pe), (*p4)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Non-Basic-Doc-Characteristics");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Non__Basic__Struc__Characteristics (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p5_z = NULLPE;
    register PE *p5 = &p5_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Non-Basic-Struc-Characteristics: out of memory");
        return NOTOK;
    }
    (*p5) = NULLPE;

    if ((*p5) != NULLPE)
        if (set_add ((*pe), (*p5)) == NOTOK) {
            advise (NULLCP, "Non-Basic-Struc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Non-Basic-Struc-Characteristics");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Additional__Doc__Characteristics (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p6_z = NULLPE;
    register PE *p6 = &p6_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Additional-Doc-Characteristics: out of memory");
        return NOTOK;
    }
    (*p6) = NULLPE;

    if ((*p6) != NULLPE)
        if (set_add ((*pe), (*p6)) == NOTOK) {
            advise (NULLCP, "Additional-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p6) = NULLPE;

    if ((*p6) != NULLPE)
        if (set_add ((*pe), (*p6)) == NOTOK) {
            advise (NULLCP, "Additional-Doc-Characteristics bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Additional-Doc-Characteristics");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Fonts__List (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p7 = NULLPE;
    PE	p8_z = NULLPE;
    register PE *p8 = &p8_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Fonts-List: out of memory");
        return NOTOK;
    }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Fonts-List");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Fonts__List__Prime (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p9_z = NULLPE;
    register PE *p9 = &p9_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Fonts-List-Prime: out of memory");
        return NOTOK;
    }
    (*p9) = NULLPE;

    {
        register int p10 = 0;

        if (((*p9) = num2prim (p10, PE_CLASS_UNIV, 2)) == NULLPE) {
            advise (NULLCP, "font-identifier: out of memory");
            return NOTOK;
        }

#ifdef DEBUG
        (void) testdebug ((*p9), "font-identifier");
#endif

    }
    if ((*p9) != NULLPE)
        if (set_add ((*pe), (*p9)) == NOTOK) {
            advise (NULLCP, "Fonts-List-Prime bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p9) = NULLPE;

    {
        if (build_ISWR18_Font__Reference (p9, 0, 0, NULLCP, NullParm) == NOTOK)
            return NOTOK;
        (*p9) -> pe_class = PE_CLASS_UNIV;
        (*p9) -> pe_id = 16;

#ifdef DEBUG
        (void) testdebug ((*p9), "font-reference");
#endif

    }
    if ((*p9) != NULLPE)
        if (set_add ((*pe), (*p9)) == NOTOK) {
            advise (NULLCP, "Fonts-List-Prime bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Fonts-List-Prime");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Document__Management__Attributes (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p11_z = NULLPE;
    register PE *p11 = &p11_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Document-Management-Attributes: out of memory");
        return NOTOK;
    }
    (*p11) = NULLPE;

    if ((*p11) != NULLPE)
        if (set_add ((*pe), (*p11)) == NOTOK) {
            advise (NULLCP, "Document-Management-Attributes bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p11) = NULLPE;

    if ((*p11) != NULLPE)
        if (set_add ((*pe), (*p11)) == NOTOK) {
            advise (NULLCP, "Document-Management-Attributes bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p11) = NULLPE;

    if ((*p11) != NULLPE)
        if (set_add ((*pe), (*p11)) == NOTOK) {
            advise (NULLCP, "Document-Management-Attributes bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p11) = NULLPE;

    if ((*p11) != NULLPE)
        if (set_add ((*pe), (*p11)) == NOTOK) {
            advise (NULLCP, "Document-Management-Attributes bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p11) = NULLPE;

    if ((*p11) != NULLPE)
        if (set_add ((*pe), (*p11)) == NOTOK) {
            advise (NULLCP, "Document-Management-Attributes bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p11) = NULLPE;

    if ((*p11) != NULLPE)
        if (set_add ((*pe), (*p11)) == NOTOK) {
            advise (NULLCP, "Document-Management-Attributes bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p11) = NULLPE;

    if ((*p11) != NULLPE)
        if (set_add ((*pe), (*p11)) == NOTOK) {
            advise (NULLCP, "Document-Management-Attributes bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p11) = NULLPE;

    if ((*p11) != NULLPE)
        if (set_add ((*pe), (*p11)) == NOTOK) {
            advise (NULLCP, "Document-Management-Attributes bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Document-Management-Attributes");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Document__Description (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p12_z = NULLPE;
    register PE *p12 = &p12_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Document-Description: out of memory");
        return NOTOK;
    }
    (*p12) = NULLPE;

    if ((*p12) != NULLPE)
        if (set_add ((*pe), (*p12)) == NOTOK) {
            advise (NULLCP, "Document-Description bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p12) = NULLPE;

    if ((*p12) != NULLPE)
        if (set_add ((*pe), (*p12)) == NOTOK) {
            advise (NULLCP, "Document-Description bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p12) = NULLPE;

    if ((*p12) != NULLPE)
        if (set_add ((*pe), (*p12)) == NOTOK) {
            advise (NULLCP, "Document-Description bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p12) = NULLPE;

    if ((*p12) != NULLPE)
        if (set_add ((*pe), (*p12)) == NOTOK) {
            advise (NULLCP, "Document-Description bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p12) = NULLPE;

    if ((*p12) != NULLPE)
        if (set_add ((*pe), (*p12)) == NOTOK) {
            advise (NULLCP, "Document-Description bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p12) = NULLPE;

    if ((*p12) != NULLPE)
        if (set_add ((*pe), (*p12)) == NOTOK) {
            advise (NULLCP, "Document-Description bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Document-Description");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Character__Data (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    register char *p13;
    int p13_len;

    p13 = buffer;
    if ((p13_len = len) == 0)
        p13_len = strlen (p13);
    if (p13 == NULLCP) {
        advise (NULLCP, "Character-Data initialization fails");
        return NOTOK;
    }
    if (((*pe) = str2prim (p13, p13_len, PE_CLASS_APPL, 3)) == NULLPE) {
        advise (NULLCP, "Character-Data: out of memory");
        return NOTOK;
    }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Character-Data");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Document__Reference (pe, explicit, len, buffer, parm)
PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Document-Reference");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Dates__and__Times (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p14_z = NULLPE;
    register PE *p14 = &p14_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Dates-and-Times: out of memory");
        return NOTOK;
    }
    (*p14) = NULLPE;

    if ((*p14) != NULLPE)
        if (set_add ((*pe), (*p14)) == NOTOK) {
            advise (NULLCP, "Dates-and-Times bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p14) = NULLPE;

    if ((*p14) != NULLPE)
        if (set_add ((*pe), (*p14)) == NOTOK) {
            advise (NULLCP, "Dates-and-Times bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p14) = NULLPE;

    if ((*p14) != NULLPE)
        if (set_add ((*pe), (*p14)) == NOTOK) {
            advise (NULLCP, "Dates-and-Times bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p14) = NULLPE;

    if ((*p14) != NULLPE)
        if (set_add ((*pe), (*p14)) == NOTOK) {
            advise (NULLCP, "Dates-and-Times bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p14) = NULLPE;

    if ((*p14) != NULLPE)
        if (set_add ((*pe), (*p14)) == NOTOK) {
            advise (NULLCP, "Dates-and-Times bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p14) = NULLPE;

    if ((*p14) != NULLPE)
        if (set_add ((*pe), (*p14)) == NOTOK) {
            advise (NULLCP, "Dates-and-Times bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p14) = NULLPE;

    if ((*p14) != NULLPE)
        if (set_add ((*pe), (*p14)) == NOTOK) {
            advise (NULLCP, "Dates-and-Times bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p14) = NULLPE;

    if ((*p14) != NULLPE)
        if (set_add ((*pe), (*p14)) == NOTOK) {
            advise (NULLCP, "Dates-and-Times bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Dates-and-Times");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Date__and__Time (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    if (build_UNIV_PrintableString (pe, 0, len, buffer, NullParm) == NOTOK)
        return NOTOK;
    (*pe) -> pe_class = PE_CLASS_APPL;
    (*pe) -> pe_id = 4;

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Date-and-Time");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Originators (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p15_z = NULLPE;
    register PE *p15 = &p15_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Originators: out of memory");
        return NOTOK;
    }
    (*p15) = NULLPE;

    if ((*p15) != NULLPE)
        if (set_add ((*pe), (*p15)) == NOTOK) {
            advise (NULLCP, "Originators bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p15) = NULLPE;

    if ((*p15) != NULLPE)
        if (set_add ((*pe), (*p15)) == NOTOK) {
            advise (NULLCP, "Originators bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p15) = NULLPE;

    if ((*p15) != NULLPE)
        if (set_add ((*pe), (*p15)) == NOTOK) {
            advise (NULLCP, "Originators bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p15) = NULLPE;

    if ((*p15) != NULLPE)
        if (set_add ((*pe), (*p15)) == NOTOK) {
            advise (NULLCP, "Originators bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Originators");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Personal__Name (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p16_z = NULLPE;
    register PE *p16 = &p16_z;

    if (((*pe) = pe_alloc (PE_CLASS_APPL, PE_FORM_CONS, 6)) == NULLPE) {
        advise (NULLCP, "Personal-Name: out of memory");
        return NOTOK;
    }
    (*p16) = NULLPE;

    {
        if (build_ISWR2_Character__Data (p16, 0, 0, NULLCP, NullParm) == NOTOK)
            return NOTOK;
        (*p16) -> pe_class = PE_CLASS_CONT;
        (*p16) -> pe_id = 0;

#ifdef DEBUG
        (void) testdebug ((*p16), "surname");
#endif

    }
    if ((*p16) != NULLPE)
        if (set_add ((*pe), (*p16)) == NOTOK) {
            advise (NULLCP, "Personal-Name bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p16) = NULLPE;

    if ((*p16) != NULLPE)
        if (set_add ((*pe), (*p16)) == NOTOK) {
            advise (NULLCP, "Personal-Name bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p16) = NULLPE;

    if ((*p16) != NULLPE)
        if (set_add ((*pe), (*p16)) == NOTOK) {
            advise (NULLCP, "Personal-Name bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p16) = NULLPE;

    if ((*p16) != NULLPE)
        if (set_add ((*pe), (*p16)) == NOTOK) {
            advise (NULLCP, "Personal-Name bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Personal-Name");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Other__User__Information (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p17_z = NULLPE;
    register PE *p17 = &p17_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Other-User-Information: out of memory");
        return NOTOK;
    }
    (*p17) = NULLPE;

    if ((*p17) != NULLPE)
        if (set_add ((*pe), (*p17)) == NOTOK) {
            advise (NULLCP, "Other-User-Information bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p17) = NULLPE;

    if ((*p17) != NULLPE)
        if (set_add ((*pe), (*p17)) == NOTOK) {
            advise (NULLCP, "Other-User-Information bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p17) = NULLPE;

    if ((*p17) != NULLPE)
        if (set_add ((*pe), (*p17)) == NOTOK) {
            advise (NULLCP, "Other-User-Information bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p17) = NULLPE;

    if ((*p17) != NULLPE)
        if (set_add ((*pe), (*p17)) == NOTOK) {
            advise (NULLCP, "Other-User-Information bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p17) = NULLPE;

    if ((*p17) != NULLPE)
        if (set_add ((*pe), (*p17)) == NOTOK) {
            advise (NULLCP, "Other-User-Information bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Other-User-Information");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_External__References (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p18_z = NULLPE;
    register PE *p18 = &p18_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "External-References: out of memory");
        return NOTOK;
    }
    (*p18) = NULLPE;

    if ((*p18) != NULLPE)
        if (set_add ((*pe), (*p18)) == NOTOK) {
            advise (NULLCP, "External-References bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p18) = NULLPE;

    if ((*p18) != NULLPE)
        if (set_add ((*pe), (*p18)) == NOTOK) {
            advise (NULLCP, "External-References bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.External-References");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Local__File__References (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p19 = NULLPE;
    PE	p20_z = NULLPE;
    register PE *p20 = &p20_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Local-File-References: out of memory");
        return NOTOK;
    }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Local-File-References");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Content__Attributes (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p21_z = NULLPE;
    register PE *p21 = &p21_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Content-Attributes: out of memory");
        return NOTOK;
    }
    (*p21) = NULLPE;

    if ((*p21) != NULLPE)
        if (set_add ((*pe), (*p21)) == NOTOK) {
            advise (NULLCP, "Content-Attributes bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p21) = NULLPE;

    if ((*p21) != NULLPE)
        if (set_add ((*pe), (*p21)) == NOTOK) {
            advise (NULLCP, "Content-Attributes bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p21) = NULLPE;

    if ((*p21) != NULLPE)
        if (set_add ((*pe), (*p21)) == NOTOK) {
            advise (NULLCP, "Content-Attributes bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Content-Attributes");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR2_Security__Information (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    PE	p22_z = NULLPE;
    register PE *p22 = &p22_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Security-Information: out of memory");
        return NOTOK;
    }
    (*p22) = NULLPE;

    if ((*p22) != NULLPE)
        if (set_add ((*pe), (*p22)) == NOTOK) {
            advise (NULLCP, "Security-Information bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p22) = NULLPE;

    if ((*p22) != NULLPE)
        if (set_add ((*pe), (*p22)) == NOTOK) {
            advise (NULLCP, "Security-Information bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p22) = NULLPE;

    if ((*p22) != NULLPE)
        if (set_add ((*pe), (*p22)) == NOTOK) {
            advise (NULLCP, "Security-Information bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR2.Security-Information");
#endif


    return OK;
}