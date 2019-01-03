/* automatically generated by pepy 5.0 #1 (mss.andrew.cmu.edu), do not edit! */

#include <psap.h>

static char *pepyid = "pepy 5.0 #1 (mss.andrew.cmu.edu) of Mon May 22 20:44:32 EDT 1989";

#define	advise	PY_advise

void	advise ();

/* Generated from module ISWR5 */
# line 5 "iswr5.py"


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

#define TK_TKI_TKERR
#define TK_TKI_APIRT

#include <odatk.h>

#undef TK_TKI_TKERR
#undef TK_TKI_APIRT

#define TK_IMPL_ISWR

#include <iswr.h>

#undef TK_IMPL_ISWR

#define PEPYPARM struct isode2toolkit_t *

INT_type dummy;


#ifndef PEPYPARM
#define PEPYPARM char *
#endif /* PEPYPARM */
extern PEPYPARM NullParm;

/* ARGSUSED */

int	build_ISWR5_Logical__Object__Descriptor (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
struct isode2toolkit_t *  parm;
{
# line 75 "iswr5.py"
    

    VALUE_type ObjectType;
    INT_type BigObjectType;


    PE	p0_z = NULLPE;
    register PE *p0 = &p0_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SEQ)) == NULLPE) {
        advise (NULLCP, "Logical-Object-Descriptor: out of memory");
        return NOTOK;
    }
    (*p0) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_OBJECT_TYPE,
			(POINTER_type) &ObjectType,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {	/* object-type */
        {
# line 86 "iswr5.py"

	    switch( ObjectType ){
		case at_OBJECT_TYPE_doc_logical_root:
		    BigObjectType = (INT_type) 0;
		    break;
		case at_OBJECT_TYPE_comp_logical_obj:
		    BigObjectType = (INT_type) 1;
		    break;
		case at_OBJECT_TYPE_bas_logical_obj:
		    BigObjectType = (INT_type) 2;
		    break;
		default:
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5__Logical__Object__Descriptor]: bad object type" );
		    dummy = TKERR_EndErrMsg();
		    return( NOTOK );
	    }
	
        }
        if (build_ISWR5_Logical__Object__Type (p0, 0, BigObjectType , NULLCP, NullParm) == NOTOK)
            return NOTOK;

#ifdef DEBUG
        (void) testdebug ((*p0), "object-type");
#endif

    }

    if ((*p0) != NULLPE)
        if (seq_add ((*pe), (*p0), -1) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor bad sequence: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p0) = NULLPE;

    {	/* descriptor-body */
        if (build_ISWR5_Logical__Object__Descriptor__Body (p0, 0, 0, NULLCP, parm ) == NOTOK)
            return NOTOK;

#ifdef DEBUG
        (void) testdebug ((*p0), "descriptor-body");
#endif

    }

    if ((*p0) != NULLPE)
        if (seq_add ((*pe), (*p0), -1) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor bad sequence: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR5.Logical-Object-Descriptor");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR5_Logical__Object__Type (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    register int p1 = len;

    if (((*pe) = num2prim (p1, PE_CLASS_UNIV, PE_PRIM_INT)) == NULLPE) {
        advise (NULLCP, "Logical-Object-Type: out of memory");
        return NOTOK;
    }
    switch (p1) {
        case 0:	/* document-logical-root */
            break;
        case 1:	/* composite-logical-object */
            break;
        case 2:	/* basic-logical-object */
            break;
        default:
            advise (NULLCP, "Logical-Object-Type has unknown component: %d", p1);
            return NOTOK;
    }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR5.Logical-Object-Type");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR5_Logical__Object__Descriptor__Body (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
struct isode2toolkit_t *  parm;
{
# line 145 "iswr5.py"
    

    STRING_type ObjectIDString;
    CHAR_type *ObjectIDChars;

    STRING_type CommentString;
    CHAR_type *CommentChars;
    INT_type CommentLength;

    STRING_type NameString;
    CHAR_type *NameChars;
    INT_type NameLength;

    CONSTITUENT_type ObjectClass;
    STRING_type ObjectClassIDString;
    CHAR_type *ObjectClassIDChars;

    CONSTITUENT_type PresentStyle;
    STRING_type PresentStyleIDString;
    CHAR_type *PresentStyleIDChars;

    CONSTITUENT_type LayoutStyle;
    STRING_type LayoutStyleIDString;
    CHAR_type *LayoutStyleIDChars;

    VALUE_type ProtectionValue;
    INT_type Protection;

    INT_type count;

    SEQUENCE_CONSTITUENT_type Subordinates;
    STRING_type SubordinatesIDString;
    STRING_type LastSubordinatesIDDigits;
    CHAR_type *SubordinatesIDChars;

    SEQUENCE_CONSTITUENT_type ContentPortions;
    STRING_type ContentIDString;
    STRING_type LastContentIDDigits;
    CHAR_type *ContentIDChars;

    SEQUENCE_BYTE_type ApplCommentBytes;
    CHAR_type *ApplCommentChars;
    INT_type ApplCommentLength;

    STRING_type ResourceNameString;
    CHAR_type *ResourceNameChars;

    PE	p2_z = NULLPE;
    register PE *p2 = &p2_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Logical-Object-Descriptor-Body: out of memory");
        return NOTOK;
    }
    (*p2) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_OBJECT_IDENTIFIER,
			(POINTER_type) &ObjectIDString,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 197 "iswr5.py"

	    ObjectIDChars = String2Chars(ObjectIDString);
	    if ( ObjectIDChars == ERROR_CHARS ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not convert string to object-identifier chars" );
		dummy = TKERR_EndErrMsg();
		return( NOTOK );
	    }
	
        }
        if (build_ISWR3_Object__or__Class__Identifier (p2, 0, 0, ObjectIDChars , NullParm) == NOTOK)
            return NOTOK;
        (*p2) -> pe_class = PE_CLASS_APPL;
        (*p2) -> pe_id = 1;

#ifdef DEBUG
        (void) testdebug ((*p2), "object-identifier");
#endif

    }
    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_SUBORDINATES,
			(POINTER_type) &Subordinates,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        PE	p3 = NULLPE;
        PE	p4_z = NULLPE;
        register PE *p4 = &p4_z;

        if (((*p2) = pe_alloc (PE_CLASS_CONT, PE_FORM_CONS, 0)) == NULLPE) {
            advise (NULLCP, "subordinates: out of memory");
            return NOTOK;
        }
        for ( count=0; count<Subordinates->length; count++ ) {
            {
# line 221 "iswr5.py"

		if ( GetAttr( Subordinates->sequence_value.constituents[count],
			      at_OBJECT_IDENTIFIER,
			      (POINTER_type) &SubordinatesIDString,
			      /* default */ BOOL_false,
			      /* mask */ (PARM_MASK_type *) 0
			    ) == ERROR_INT ) {
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not get attribute at_OBJECT_IDENTIFIER" );
		    dummy = TKERR_EndErrMsg();
		    return ( NOTOK );
		}
		LastSubordinatesIDDigits =
		    GetLastIDDigits(SubordinatesIDString);
		if ( LastSubordinatesIDDigits == ERROR_STRING ) {
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not get last ID digits");
		    dummy = TKERR_EndErrMsg();
		    return( NOTOK );
		}
		SubordinatesIDChars = String2Chars(LastSubordinatesIDDigits);
		if ( SubordinatesIDChars == ERROR_CHARS ) {
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not convert string to subordinates id chars" );
		    dummy = TKERR_EndErrMsg();
		    return( NOTOK );
		}
	    
            }
            {
                if (build_UNIV_NumericString (p4, 0, 0, SubordinatesIDChars , NullParm) == NOTOK)
                    return NOTOK;

#ifdef DEBUG
                (void) testdebug ((*p4), "element");
#endif

            }
            seq_addon ((*p2), p3, (*p4));
            p3 = (*p4);
        }

#ifdef DEBUG
        (void) testdebug ((*p2), "subordinates");
#endif

    }
    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_CONTENT_PORTIONS,
			(POINTER_type) &ContentPortions,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        PE	p5 = NULLPE;
        PE	p6_z = NULLPE;
        register PE *p6 = &p6_z;

        if (((*p2) = pe_alloc (PE_CLASS_CONT, PE_FORM_CONS, 1)) == NULLPE) {
            advise (NULLCP, "content-portions: out of memory");
            return NOTOK;
        }
        for ( count=0; count<ContentPortions->length; count++ ) {
            {
# line 266 "iswr5.py"

		if ( GetAttr( ContentPortions ->
				  sequence_value.constituents[count],
			      at_CONTENT_ID_LOGICAL,
			      (POINTER_type) &ContentIDString,
			      /* default */ BOOL_false,
			      /* mask */ (PARM_MASK_type *) 0
			    ) == ERROR_INT ) {
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not get attribute CONTENT_ID_LOGICAL" );
		    dummy = TKERR_EndErrMsg();
		    return ( NOTOK );
		}
		LastContentIDDigits =
		    GetLastIDDigits(ContentIDString);
		if ( LastContentIDDigits == ERROR_STRING ) {
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not get last ID digits");
		    dummy = TKERR_EndErrMsg();
		    return( NOTOK );
		}
		ContentIDChars = String2Chars(LastContentIDDigits);
		if ( ContentIDChars == ERROR_CHARS ) {
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not convert string to content id chars" );
		    dummy = TKERR_EndErrMsg();
		    return( NOTOK );
		}
	    
            }
            {
                if (build_UNIV_NumericString (p6, 0, 0, ContentIDChars , NullParm) == NOTOK)
                    return NOTOK;

#ifdef DEBUG
                (void) testdebug ((*p6), "element");
#endif

            }
            seq_addon ((*p2), p5, (*p6));
            p5 = (*p6);
        }

#ifdef DEBUG
        (void) testdebug ((*p2), "content-portions");
#endif

    }
    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_OBJECT_CLASS,
			(POINTER_type) &ObjectClass,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 310 "iswr5.py"

	    if ( GetAttr( ObjectClass,
			  at_OBJECT_CLASS_IDENTIFIER,
			  (POINTER_type) &ObjectClassIDString,
			  /* default */ BOOL_false,
			  /* mask */ (PARM_MASK_type *) 0
			) == ERROR_INT ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not get attribute at_OBJECT_CLASS_IDENTIFIER" );
		dummy = TKERR_EndErrMsg();
		return ( NOTOK );
	    }
	    ObjectClassIDChars = String2Chars(ObjectClassIDString);
	    if ( ObjectClassIDChars == ERROR_CHARS ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not convert string to object-class-identifier chars" );
		dummy = TKERR_EndErrMsg();
		return( NOTOK );
	    }
	
        }
        if (build_ISWR3_Object__or__Class__Identifier (p2, 0, 0, ObjectClassIDChars , NullParm) == NOTOK)
            return NOTOK;
        (*p2) -> pe_class = PE_CLASS_CONT;
        (*p2) -> pe_id = 2;

#ifdef DEBUG
        (void) testdebug ((*p2), "object-class");
#endif

    }
    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_USER_READABLE_COMMENTS,
			(POINTER_type) &CommentString,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 363 "iswr5.py"

	    CommentLength = CommentString->length;
	    CommentChars = CommentString->sequence_value.chars;
	
        }
        if (build_ISWR4_Comment__String (p2, 0,  CommentLength , CommentChars , NullParm) == NOTOK)
            return NOTOK;
        (*p2) -> pe_class = PE_CLASS_CONT;
        (*p2) -> pe_id = 8;

#ifdef DEBUG
        (void) testdebug ((*p2), "user-readable-comments");
#endif

    }
    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_USER_VISIBLE_NAME,
			(POINTER_type) &NameString,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 400 "iswr5.py"

	    NameLength = NameString->length;
	    NameChars = NameString->sequence_value.chars;
	
        }
        if (build_ISWR4_Comment__String (p2, 0,  NameLength , NameChars , NullParm) == NOTOK)
            return NOTOK;
        (*p2) -> pe_class = PE_CLASS_CONT;
        (*p2) -> pe_id = 14;

#ifdef DEBUG
        (void) testdebug ((*p2), "user-visible-name");
#endif

    }
    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_PRESENTATION_STYLE,
			(POINTER_type) &PresentStyle,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 417 "iswr5.py"

	    if ( GetAttr( PresentStyle,
			  at_PRESENTATION_STYLE_ID,
			  (POINTER_type) &PresentStyleIDString,
			  /* default */ BOOL_false,
			  /* mask */ (PARM_MASK_type *) 0
			) == ERROR_INT ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not get attribute at_PRESEMTATOPN_STYLE_ID" );
		dummy = TKERR_EndErrMsg();
		return ( NOTOK );
	    }
	    PresentStyleIDChars = String2Chars(PresentStyleIDString);
	    if ( PresentStyleIDChars == ERROR_CHARS ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not convert string to presentation-style-identifier chars" );
		dummy = TKERR_EndErrMsg();
		return( NOTOK );
	    }
	
        }
        if (build_ISWR3_Style__Identifier (p2, 0, 0, PresentStyleIDChars , NullParm) == NOTOK)
            return NOTOK;
        (*p2) -> pe_class = PE_CLASS_CONT;
        (*p2) -> pe_id = 17;

#ifdef DEBUG
        (void) testdebug ((*p2), "presentation-style");
#endif

    }
    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_LAYOUT_STYLE,
			(POINTER_type) &LayoutStyle,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 450 "iswr5.py"

	    if ( GetAttr( LayoutStyle,
			  at_LAYOUT_STYLE_IDENTIFIER,
			  (POINTER_type) &LayoutStyleIDString,
			  /* default */ BOOL_false,
			  /* mask */ (PARM_MASK_type *) 0
			) == ERROR_INT ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not get attribute at_LAYOUT_STYLE_ID" );
		dummy = TKERR_EndErrMsg();
		return ( NOTOK );
	    }
	    LayoutStyleIDChars = String2Chars(LayoutStyleIDString);
	    if ( LayoutStyleIDChars == ERROR_CHARS ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not convert string to layout-style-identifier chars" );
		dummy = TKERR_EndErrMsg();
		return( NOTOK );
	    }
	
        }
        if (build_ISWR3_Style__Identifier (p2, 0, 0, LayoutStyleIDChars , NullParm) == NOTOK)
            return NOTOK;
        (*p2) -> pe_class = PE_CLASS_CONT;
        (*p2) -> pe_id = 19;

#ifdef DEBUG
        (void) testdebug ((*p2), "layout-style");
#endif

    }
    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_PROTECTION,
			(POINTER_type) &ProtectionValue,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 483 "iswr5.py"

	    switch( ProtectionValue ) {
		case at_PROTECTION_unprotected:
		    Protection = (INT_type) 0;
		    break;
		case at_PROTECTION_protected:
		    Protection = (INT_type) 1;
		    break;
		default:
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5__Logical__Object__Descriptor]: bad protection value" );
		    dummy = TKERR_EndErrMsg();
		    return( NOTOK );
	    }
	
        }
        if (build_ISWR5_Protection (p2, 0, Protection , NULLCP, NullParm) == NOTOK)
            return NOTOK;
        (*p2) -> pe_class = PE_CLASS_CONT;
        (*p2) -> pe_id = 20;

#ifdef DEBUG
        (void) testdebug ((*p2), "protection");
#endif

    }
    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p2) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_APPLICATION_COMMENTS,
			(POINTER_type) &ApplCommentBytes,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 511 "iswr5.py"

	    if ( Bytes2Chars( ApplCommentBytes,
			      &ApplCommentChars,
			      &ApplCommentLength
			    ) == ERROR_INT ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not convert bytes to chars" );
		dummy = TKERR_EndErrMsg();
		return ( NOTOK );
	    }
	
        }
        if (build_ISWR4_Comment__String (p2, 0,  ApplCommentLength , ApplCommentChars , NullParm) == NOTOK)
            return NOTOK;
        (*p2) -> pe_class = PE_CLASS_CONT;
        (*p2) -> pe_id = 25;

#ifdef DEBUG
        (void) testdebug ((*p2), "application-comments");
#endif

    }
    if ((*p2) != NULLPE)
        if (set_add ((*pe), (*p2)) == NOTOK) {
            advise (NULLCP, "Logical-Object-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR5.Logical-Object-Descriptor-Body");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR5_Logical__Class__Descriptor (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
struct isode2toolkit_t *  parm;
{
# line 539 "iswr5.py"
    

    VALUE_type ObjectType;
    INT_type BigObjectType;


    PE	p7_z = NULLPE;
    register PE *p7 = &p7_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SEQ)) == NULLPE) {
        advise (NULLCP, "Logical-Class-Descriptor: out of memory");
        return NOTOK;
    }
    (*p7) = NULLPE;

    {	/* object-type */
        {
# line 550 "iswr5.py"

	    if ( GetAttr( parm->constituent,
			  at_OBJECT_TYPE,
			  (POINTER_type) &ObjectType,
			  /* default */ BOOL_false,
			  /* mask */ (PARM_MASK_type *) 0
			) == ERROR_INT ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Class__Descriptor__Body]: can not get attribute at_OBJECT_TYPE" );
		dummy = TKERR_EndErrMsg();
		return ( NOTOK );
	    }
	    switch( ObjectType ){
		case at_OBJECT_TYPE_doc_logical_root:
		    BigObjectType = (INT_type) 0;
		    break;
		case at_OBJECT_TYPE_comp_logical_obj:
		    BigObjectType = (INT_type) 1;
		    break;
		case at_OBJECT_TYPE_bas_logical_obj:
		    BigObjectType = (INT_type) 2;
		    break;
		default:
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5__Logical__Class__Descriptor]: bad object type" );
		    dummy = TKERR_EndErrMsg();
		    return( NOTOK );
	    }
	
        }
        if (build_ISWR5_Logical__Object__Type (p7, 0, BigObjectType , NULLCP, NullParm) == NOTOK)
            return NOTOK;

#ifdef DEBUG
        (void) testdebug ((*p7), "object-type");
#endif

    }

    if ((*p7) != NULLPE)
        if (seq_add ((*pe), (*p7), -1) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor bad sequence: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p7) = NULLPE;

    {	/* descriptor-body */
        if (build_ISWR5_Logical__Class__Descriptor__Body (p7, 0, 0, NULLCP, parm ) == NOTOK)
            return NOTOK;

#ifdef DEBUG
        (void) testdebug ((*p7), "descriptor-body");
#endif

    }

    if ((*p7) != NULLPE)
        if (seq_add ((*pe), (*p7), -1) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor bad sequence: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR5.Logical-Class-Descriptor");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR5_Logical__Class__Descriptor__Body (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
# line 591 "iswr5.py"
    

    STRING_type CommentString;
    CHAR_type *CommentChars;
    INT_type CommentLength;

    STRING_type NameString;
    CHAR_type *NameChars;
    INT_type NameLength;

    STRING_type ObjectClassIDString;
    CHAR_type *ObjectClassIDChars;

    CONSTITUENT_type PresentStyle;
    STRING_type PresentStyleIDString;
    CHAR_type *PresentStyleIDChars;

    CONSTITUENT_type LayoutStyle;
    STRING_type LayoutStyleIDString;
    CHAR_type *LayoutStyleIDChars;

    VALUE_type ProtectionValue;
    INT_type Protection;

    INT_type count;

    SEQUENCE_CONSTITUENT_type ContentPortions;
    STRING_type ContentIDString;
    STRING_type LastContentIDDigits;
    CHAR_type *ContentIDChars;

    CONSTRUCTION_EXPR_type *GenForSubordinates;

    SEQUENCE_BYTE_type ApplCommentBytes;
    CHAR_type *ApplCommentChars;
    INT_type ApplCommentLength;

    STRING_type ResourceNameString;
    CHAR_type *ResourceNameChars;

    PE	p8_z = NULLPE;
    register PE *p8 = &p8_z;

    if (((*pe) = pe_alloc (PE_CLASS_UNIV, PE_FORM_CONS, PE_CONS_SET)) == NULLPE) {
        advise (NULLCP, "Logical-Class-Descriptor-Body: out of memory");
        return NOTOK;
    }
    (*p8) = NULLPE;

    {
        {
# line 636 "iswr5.py"

	    if ( GetAttr( parm->constituent,
			  at_OBJECT_CLASS_IDENTIFIER,
			  (POINTER_type) &ObjectClassIDString,
			  /* default */ BOOL_false,
			  /* mask */ (PARM_MASK_type *) 0
			) == ERROR_INT ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Class__Descriptor__Body]: can not get attribute at_OBJECT_CLASS_IDENTIFIER" );
		dummy = TKERR_EndErrMsg();
		return ( NOTOK );
	    }
	    ObjectClassIDChars = String2Chars(ObjectClassIDString);
	    if ( ObjectClassIDChars == ERROR_CHARS ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Class__Descriptor__Body]: can not convert string to object-class-identifier chars" );
		dummy = TKERR_EndErrMsg();
		return( NOTOK );
	    }
	
        }
        if (build_ISWR3_Object__or__Class__Identifier (p8, 0, 0, ObjectClassIDChars , NullParm) == NOTOK)
            return NOTOK;
        (*p8) -> pe_class = PE_CLASS_APPL;
        (*p8) -> pe_id = 1;

#ifdef DEBUG
        (void) testdebug ((*p8), "object-class-identifier");
#endif

    }
    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_GEN_FOR_SUBORDINATES,
			(POINTER_type) &GenForSubordinates,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        if (build_ISWR3_Construction__Expression (p8, 0, 0, NULLCP, GenForSubordinates ) == NOTOK)
            return NOTOK;

#ifdef DEBUG
        (void) testdebug ((*p8), "generator-for-subordinates");
#endif

        {	/* generator-for-subordinates TAG PUSHDOWN */
            PE p9_z;
            register PE *p9 = &p9_z;

            if ((*p9 = pe_alloc (PE_CLASS_CONT, PE_FORM_CONS, 0)) == NULLPE) {
                advise (NULLCP, "generator-for-subordinates: out of memory");
                return NOTOK;
            }
            (*p9) -> pe_cons = (*p8);
            (*p8) = *p9;
        }
    }
    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_CONTENT_PORTIONS,
			(POINTER_type) &ContentPortions,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        PE	p10 = NULLPE;
        PE	p11_z = NULLPE;
        register PE *p11 = &p11_z;

        if (((*p8) = pe_alloc (PE_CLASS_CONT, PE_FORM_CONS, 1)) == NULLPE) {
            advise (NULLCP, "content-portions: out of memory");
            return NOTOK;
        }
        for ( count=0; count<ContentPortions->length; count++ ) {
            {
# line 675 "iswr5.py"

		if ( GetAttr( ContentPortions ->
				  sequence_value.constituents[count],
			      at_CONTENT_ID_LOGICAL,
			      (POINTER_type) &ContentIDString,
			      /* default */ BOOL_false,
			      /* mask */ (PARM_MASK_type *) 0
			    ) == ERROR_INT ) {
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5_Logical__Class__Descriptor__Body]: can not get attribute CONTENT_ID_LOGICAL" );
		    dummy = TKERR_EndErrMsg();
		    return ( NOTOK );
		}
		LastContentIDDigits =
		    GetLastIDDigits(ContentIDString);
		if ( LastContentIDDigits == ERROR_STRING ) {
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5_Logical__Class__Descriptor__Body]: can not get last ID digits");
		    dummy = TKERR_EndErrMsg();
		    return( NOTOK );
		}
		ContentIDChars = String2Chars(LastContentIDDigits);
		if ( ContentIDChars == ERROR_CHARS ) {
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5_Logical__Class__Descriptor__Body]: can not convert string to content id chars" );
		    dummy = TKERR_EndErrMsg();
		    return( NOTOK );
		}
	    
            }
            {
                if (build_UNIV_NumericString (p11, 0, 0, ContentIDChars , NullParm) == NOTOK)
                    return NOTOK;

#ifdef DEBUG
                (void) testdebug ((*p11), "element");
#endif

            }
            seq_addon ((*p8), p10, (*p11));
            p10 = (*p11);
        }

#ifdef DEBUG
        (void) testdebug ((*p8), "content-portions");
#endif

    }
    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_USER_READABLE_COMMENTS,
			(POINTER_type) &CommentString,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 739 "iswr5.py"

	    CommentLength = CommentString->length;
	    CommentChars = CommentString->sequence_value.chars;
	
        }
        if (build_ISWR4_Comment__String (p8, 0,  CommentLength , CommentChars , NullParm) == NOTOK)
            return NOTOK;
        (*p8) -> pe_class = PE_CLASS_CONT;
        (*p8) -> pe_id = 8;

#ifdef DEBUG
        (void) testdebug ((*p8), "user-readable-comments");
#endif

    }
    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_USER_VISIBLE_NAME,
			(POINTER_type) &NameString,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 776 "iswr5.py"

	    NameLength = NameString->length;
	    NameChars = NameString->sequence_value.chars;
	
        }
        if (build_ISWR4_Comment__String (p8, 0,  NameLength , NameChars , NullParm) == NOTOK)
            return NOTOK;
        (*p8) -> pe_class = PE_CLASS_CONT;
        (*p8) -> pe_id = 14;

#ifdef DEBUG
        (void) testdebug ((*p8), "user-visible-name");
#endif

    }
    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_PRESENTATION_STYLE,
			(POINTER_type) &PresentStyle,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 793 "iswr5.py"

	    if ( GetAttr( PresentStyle,
			  at_PRESENTATION_STYLE_ID,
			  (POINTER_type) &PresentStyleIDString,
			  /* default */ BOOL_false,
			  /* mask */ (PARM_MASK_type *) 0
			) == ERROR_INT ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Class__Descriptor__Body]: can not get attribute at_PRESENTATION_STYLE_ID" );
		dummy = TKERR_EndErrMsg();
		return ( NOTOK );
	    }
	    PresentStyleIDChars = String2Chars(PresentStyleIDString);
	    if ( PresentStyleIDChars == ERROR_CHARS ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Class__Descriptor__Body]: can not convert string to presentation-style-identifier chars" );
		dummy = TKERR_EndErrMsg();
		return( NOTOK );
	    }
	
        }
        if (build_ISWR3_Style__Identifier (p8, 0, 0, PresentStyleIDChars , NullParm) == NOTOK)
            return NOTOK;
        (*p8) -> pe_class = PE_CLASS_CONT;
        (*p8) -> pe_id = 17;

#ifdef DEBUG
        (void) testdebug ((*p8), "presentation-style");
#endif

    }
    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_LAYOUT_STYLE,
			(POINTER_type) &LayoutStyle,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 826 "iswr5.py"

	    if ( GetAttr( LayoutStyle,
			  at_LAYOUT_STYLE_IDENTIFIER,
			  (POINTER_type) &LayoutStyleIDString,
			  /* default */ BOOL_false,
			  /* mask */ (PARM_MASK_type *) 0
			) == ERROR_INT ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Class__Descriptor__Body]: can not get attribute at_LAYOUT_STYLE_IDENTIFIER" );
		dummy = TKERR_EndErrMsg();
		return ( NOTOK );
	    }
	    LayoutStyleIDChars = String2Chars(LayoutStyleIDString);
	    if ( LayoutStyleIDChars == ERROR_CHARS ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Class__Descriptor__Body]: can not convert string to layout-style-identifier chars" );
		dummy = TKERR_EndErrMsg();
		return( NOTOK );
	    }
	
        }
        if (build_ISWR3_Style__Identifier (p8, 0, 0, LayoutStyleIDChars , NullParm) == NOTOK)
            return NOTOK;
        (*p8) -> pe_class = PE_CLASS_CONT;
        (*p8) -> pe_id = 19;

#ifdef DEBUG
        (void) testdebug ((*p8), "layout-style");
#endif

    }
    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_PROTECTION,
			(POINTER_type) &ProtectionValue,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 859 "iswr5.py"

	    switch( ProtectionValue ) {
		case at_PROTECTION_unprotected:
		    Protection = (INT_type) 0;
		    break;
		case at_PROTECTION_protected:
		    Protection = (INT_type) 1;
		    break;
		default:
		    dummy = TKERR_StartErrMsg();
		    dummy = TKERR_Format("[build_ISWR5__Logical_Class__Descriptor]: bad protection value" );
		    dummy = TKERR_EndErrMsg();
		    return( NOTOK );
	    }
	
        }
        if (build_ISWR5_Protection (p8, 0, Protection , NULLCP, NullParm) == NOTOK)
            return NOTOK;
        (*p8) -> pe_class = PE_CLASS_CONT;
        (*p8) -> pe_id = 20;

#ifdef DEBUG
        (void) testdebug ((*p8), "protection");
#endif

    }
    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_RESOURCE,
			(POINTER_type) &ResourceNameString,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 887 "iswr5.py"

	    ResourceNameChars = String2Chars(ResourceNameString);
	    if ( ResourceNameChars == ERROR_CHARS ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Class__Descriptor__Body]: can not convert string to resource-name chars" );
		dummy = TKERR_EndErrMsg();
		return( NOTOK );
	    }
	
        }
        if (build_ISWR3_Resource__Name (p8, 0, 0, ResourceNameChars , NullParm) == NOTOK)
            return NOTOK;
        (*p8) -> pe_class = PE_CLASS_CONT;
        (*p8) -> pe_id = 24;

#ifdef DEBUG
        (void) testdebug ((*p8), "resource");
#endif

    }
    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }
    (*p8) = NULLPE;

    if ( GetAttr( parm->constituent,
			at_APPLICATION_COMMENTS,
			(POINTER_type) &ApplCommentBytes,
			/* default */ BOOL_false,
			/* mask */ (PARM_MASK_type *) 0
		      ) != ERROR_INT ) {
        {
# line 909 "iswr5.py"

	    if ( Bytes2Chars( ApplCommentBytes,
			      &ApplCommentChars,
			      &ApplCommentLength
			    ) == ERROR_INT ) {
		dummy = TKERR_StartErrMsg();
		dummy = TKERR_Format("[build_ISWR5_Logical__Object__Descriptor__Body]: can not convert bytes to chars" );
		dummy = TKERR_EndErrMsg();
		return ( NOTOK );
	    }
	
        }
        if (build_ISWR4_Comment__String (p8, 0,  ApplCommentLength , ApplCommentChars , NullParm) == NOTOK)
            return NOTOK;
        (*p8) -> pe_class = PE_CLASS_CONT;
        (*p8) -> pe_id = 25;

#ifdef DEBUG
        (void) testdebug ((*p8), "application-comments");
#endif

    }
    if ((*p8) != NULLPE)
        if (set_add ((*pe), (*p8)) == NOTOK) {
            advise (NULLCP, "Logical-Class-Descriptor-Body bad set: %s",
                    pe_error ((*pe) -> pe_errno));
            return NOTOK;
        }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR5.Logical-Class-Descriptor-Body");
#endif


    return OK;
}

/* ARGSUSED */

int	build_ISWR5_Protection (pe, explicit, len, buffer, parm)
register PE     *pe;
int	explicit;
int	len;
char   *buffer;
PEPYPARM parm;
{
    register int p12 = len;

    if (((*pe) = num2prim (p12, PE_CLASS_UNIV, PE_PRIM_INT)) == NULLPE) {
        advise (NULLCP, "Protection: out of memory");
        return NOTOK;
    }
    switch (p12) {
        case 0:	/* unprotected */
            break;
        case 1:	/* protected */
            break;
        default:
            advise (NULLCP, "Protection has unknown component: %d", p12);
            return NOTOK;
    }

#ifdef DEBUG
    (void) testdebug ((*pe), "ISWR5.Protection");
#endif


    return OK;
}