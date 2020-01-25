/* $SonyId: SjConvert.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
/******************************************************************

Copyright (c) 1992, 1993, 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Sony Corporation shall
not be used in advertising or otherwise to promote the sale, use or ot
her dealings in this Software without prior written authorization from
Sony Corporation.

******************************************************************/
#include	<ctype.h>
#include	<X11/keysymdef.h>
#include	<Xmw/Xmw.h>
#include	"sjxa.h"
#include	"Ximp.h"


static Boolean	StringsAreEqual( s1, s2 )
register char    *s1, *s2;
{
    register char       c;

    while( (c = *s1) != '\0' ) {
	if( isupper(c) )
	    c = tolower( c );
	if( c != *s2 )
	    return( False );
	s1++;
	s2++;
    }
    return( True );
}


static void CvtStringToCharacterCode( args, num_args, from_val, to_val )
XrmValue	*args;
Cardinal	*num_args;
XrmValue	*from_val;
XrmValue	*to_val;
{
    char			*in_str = (char *)(from_val->addr);
    static CharacterCode	ii;

    to_val->size = sizeof(CharacterCode);
    to_val->addr = (caddr_t)&ii;

    if( StringsAreEqual( in_str, "sjis" ) )
	ii = _SJIS;
    else if( StringsAreEqual( in_str, "euc" ) )
	ii = _EUC;
    else if( StringsAreEqual( in_str, "jis" ) )
	ii = _JIS;
    else if( StringsAreEqual( in_str, "kuten" ) )
	ii = _KUTEN;
    else {
	ii = _SJIS;
	XtStringConversionWarning( (char *)from_val->addr, XmwRCharacterCode );
    }
}


static void CvtStringToConversionMode( args, num_args, from_val, to_val )
XrmValue	*args;
Cardinal	*num_args;
XrmValue	*from_val;
XrmValue	*to_val;
{
    char			*in_str = (char *)(from_val->addr);
    static ConversionMode	ii;

    to_val->size = sizeof(ConversionMode);
    to_val->addr = (caddr_t)&ii;

    if( StringsAreEqual( in_str, "zenkakuhiragana" ) )
	ii = ZENKAKU_HIRAGANA;
    else if( StringsAreEqual( in_str, "zenkakukatakana" ) )
	ii = ZENKAKU_KATAKANA;
    else if( StringsAreEqual( in_str, "hankakukatakana" ) )
	ii = HANKAKU_KATAKANA;
    else if( StringsAreEqual( in_str, "zenkakualpha" ) )
	ii = ZENKAKU_ALPHA;
    else if( StringsAreEqual( in_str, "hankakualpha" ) )
	ii = HANKAKU_ALPHA;
    else if( StringsAreEqual( in_str, "sjis" ) )
	ii = SJIS;
    else if( StringsAreEqual( in_str, "euc" ) )
	ii = EUC;
    else if( StringsAreEqual( in_str, "jis" ) )
	ii = JIS;
    else if( StringsAreEqual( in_str, "kuten" ) )
	ii = KUTEN;
    else {
	ii = ZENKAKU_HIRAGANA;
	XtStringConversionWarning( (char *)from_val->addr, XmwRConversionMode );
    }
}


static void	CvtStringToProcessStartKeys( args, num_args, from_val, to_val )
XrmValue	*args;
Cardinal	*num_args;
XrmValue	*from_val;
XrmValue	*to_val;
{
    char		*in_str = (char *)(from_val->addr);
    static XimpKey	*keys = NULL, key;
    char		 event[256], tmp[256];
    char		*modifier, *detail;
    char		 c, *ss;
    int			 ii, count;
    Boolean		 exclamation, tilde;
    static struct {
	char	*name;
	int	len;
	long	mask;
    } mod[] = {
	{ "Ctrl",	4,	ControlMask	},
	{ "Lock",	4,	LockMask	},
	{ "Caps",	4,	LockMask	},
	{ "Shift",	5,	ShiftMask	},
	{ "Alt",	3,	Mod1Mask	},
	{ "Meta",	4,	Mod1Mask	},
	{ NULL,		0,	0		}};
#define	AllMask	(ControlMask | LockMask | ShiftMask | Mod1Mask)

    to_val->size = sizeof(XimpKey *);
    count = 0;

    keys = (XimpKey *)calloc( 1, sizeof(XimpKey) );
    do {
	ss = event;
	while( (c = *in_str) != '\0' ) {
	    in_str++;
	    if( c == '\n' )
		break;
	    *ss++ = c;
	}
	*ss = '\0';
	strcpy( tmp, event );

	if( (ss = strstr( event, "<Key>" )) != NULL )
	    detail = ss + 5;
	else if( (ss = strstr( event, "<KeyDown>" )) != NULL )
	    detail = ss + 9;
	else {
	    XtStringConversionWarning( tmp, "beginConversionKey" );
	    continue;
	}
	*ss = '\0';

	modifier = event;
	key.modifier = 0;
	key.modifier_mask = 0;
	key.keysym = XK_VoidSymbol;
	exclamation = False;
	do {
	    while( *modifier == ' '  ||  *modifier == '\t' )
		modifier++;
	    if( *modifier == '\0' )
		break;
	    if( strncmp( modifier, "None", 4 ) == 0 ) {
		if( key.modifier_mask != 0  ||  exclamation ) {
		    XtStringConversionWarning( tmp, "beginConversionKey" );
		    goto error;
		}
		key.modifier_mask = AllMask;
		modifier += 4;
	    }
	    else {
		if( *modifier == '!' ) {
		    if( key.modifier_mask != 0  ||  exclamation ) {
			XtStringConversionWarning( tmp, "beginConversionKey" );
			goto error;
		    }
		    key.modifier_mask = AllMask;
		    exclamation = True;
		    modifier++;
		    continue;
		}
		if( (tilde = (*modifier == '~')) ) {
		    modifier++;
		    while( *modifier == ' '  ||  *modifier == '\t' )
			modifier++;
		}
		for( ii = 0; mod[ii].name != NULL; ii++ ) {
		    if( strncmp( modifier, mod[ii].name, mod[ii].len ) == 0 ) {
			key.modifier_mask |= mod[ii].mask;
			if( !tilde )
			    key.modifier |= mod[ii].mask;
			modifier += mod[ii].len;
			break;
		    }
		}
		if( mod[ii].name == NULL ) {
		    XtStringConversionWarning( tmp, "beginConversionKey" );
		    goto error;
		}
	    }
	} while( *modifier != '\0' );

	while( *detail == ' '  ||  *detail == '\t' )
	    detail++;
	for( ss = detail; isalnum(*ss)  ||  *ss == '_'; ss++ );
	*ss = '\0';
	if( (key.keysym = XStringToKeysym( detail )) == NoSymbol ) {
	    key.keysym = XK_VoidSymbol;
	    XtStringConversionWarning( tmp, "beginConversionKey" );
	}
	else {
	    keys = (XimpKey *)realloc( (char *)keys,
				       sizeof(XimpKey) * (count+2) );
	    keys[count] = key;
	    count++;
	    keys[count].modifier = 0;
	    keys[count].modifier_mask = 0;
	    keys[count].keysym = XK_VoidSymbol;
	}
      error:;
    } while( *in_str != '\0' );
    to_val->addr = (caddr_t)&keys;
}


void	AddConverter( app )
XtAppContext	app;
{
    XtAppAddConverter( app, XtRString, XmwRCharacterCode,
		       CvtStringToCharacterCode, NULL, 0 );
    XtAppAddConverter( app, XtRString, XmwRConversionMode,
		       CvtStringToConversionMode, NULL, 0 );
    XtAppAddConverter( app, XtRString, XmwRProcessStartKeys,
		       CvtStringToProcessStartKeys, NULL, 0 );
}
