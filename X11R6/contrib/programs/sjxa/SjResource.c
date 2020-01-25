/* $SonyId: SjResource.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
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
#include	<stdlib.h>
#include	<sys/param.h>
#include	<netdb.h>
#include	<X11/keysym.h>
#include	<X11/Xlib.h>
#include	<X11/StringDefs.h>
#include	<Xmw/XmwStrDefs.h>
#include	"sjxa.h"
#include	"resource.h"
#include	"SjString.h"

Boolean		server_name_flag = False;
char		server_name[MAXHOSTNAMELEN];


static void
#if NeedFunctionPrototypes
get_sj3serv( Widget widget, int offset, XrmValue *value )
#else /* NeedFunctionPrototypes */
get_sj3serv( widget, offset, value )
    Widget	 widget;
    int		 offset;
    XrmValue	*value;
#endif /* NeedFunctionPrototypes */
{
    char	*name;

    value->size = sizeof(char *);
    value->addr = server_name;

    name = getenv( "SJ3SERV" );
    if( name != NULL ) {
	strcpy( server_name, name );
	name = server_name;
	while( *name != '\0'  &&  *name != ' '  &&  *name != '\t' )
	    name++;
	*name = '\0';
    }
    else
	strcpy( server_name, "localhost" );
    server_name_flag = True;
}


#define Offset(x)	XtOffset(SjxaP,x)
static XtResource local_resources[] = {
    {
	"baseSjxaAccelerators", "BaseSjxaAccelerators", XtRAcceleratorTable,
	sizeof(XtTranslations), Offset( base_accelerators ),
	XtRTranslationTable, (XtPointer)NULL },
    {
	"sjxaAccelerators", "SjxaAccelerators", XtRAcceleratorTable,
	sizeof(XtTranslations), Offset( accelerators ),
	XtRTranslationTable, (XtPointer)NULL },
    {
	"sjxa", "Sjxa", XtRString,
	sizeof(String), Offset( sjxa ),
	XtRString, "" },
    {
	"f1Label", "F1Label", XtRString,
	sizeof(String), Offset( function_label[0] ),
	XtRString, "" },
    {
	"f2Labea", "F2Label", XtRString,
	sizeof(String), Offset( function_label[1] ),
	XtRString, "" },
    {
	"f3Label", "F3Label", XtRString,
	sizeof(String), Offset( function_label[2] ),
	XtRString, "" },
    {
	"f4Label", "F4Label", XtRString,
	sizeof(String), Offset( function_label[3] ),
	XtRString, "" },
    {
	"f5Label", "F5Label", XtRString,
	sizeof(String), Offset( function_label[4] ),
	XtRString, "" },
    {
	"f6Label", "F6Label", XtRString,
	sizeof(String), Offset( function_label[5] ),
	XtRString, "" },
    {
	"f7Label", "F7Label", XtRString,
	sizeof(String), Offset( function_label[6] ),
	XtRString, "" },
    {
	"f8Label", "F8Label", XtRString,
	sizeof(String), Offset( function_label[7] ),
	XtRString, "" },
    {
	"f9Label", "F9Label", XtRString,
	sizeof(String), Offset( function_label[8] ),
	XtRString, "" },
    {
	"f10Label", "F10Label", XtRString,
	sizeof(String), Offset( function_label[9] ),
	XtRString, "" },
    {
	"hankakuAlpha", "HankakuAlpha", XtRString,
	sizeof(String), Offset( hankaku_alpha ),
	XtRString, "" },
    {
	"zenkakuAlpha", "ZenkakuAlpha", XtRString,
	sizeof(String), Offset( zenkaku_alpha ),
	XtRString, "" },
    {
	"hankakuKatakana", "HankakuKatakana", XtRString,
	sizeof(String), Offset( hankaku_katakana ),
	XtRString, "" },
    {
 	"zenkakuKatakana", "ZenkakuKatakana", XtRString,
	sizeof(String), Offset( zenkaku_katakana ),
	XtRString, "" },
    {
	"zenkakuHiragana", "ZenkakuHiragana", XtRString,
	sizeof(String), Offset( zenkaku_hiragana ),
	XtRString, "" },
    {
	"lockHankakuAlpha", "LockHankakuAlpha", XtRString,
	sizeof(String), Offset( lock_hankaku_alpha ),
	XtRString, "" },
    {
	"lockZenkakuAlpha", "LockZenkakuAlpha", XtRString,
	sizeof(String), Offset( lock_zenkaku_alpha ),
	XtRString, "" },
    {
	"lockHankakuKatakana", "LockHankakuKatakana", XtRString,
	sizeof(String), Offset( lock_hankaku_katakana ),
	XtRString, "" },
    {
 	"lockZenkakuKatakana", "LockZenkakuKatakana", XtRString,
	sizeof(String), Offset( lock_zenkaku_katakana ),
	XtRString, "" },
    {
	"lockZenkakuHiragana", "LockZenkakuHiragana", XtRString,
	sizeof(String), Offset( lock_zenkaku_hiragana ),
	XtRString, "" },
    {
	"shiftJis", "ShiftJis", XtRString,
	sizeof(String), Offset( sjis ),
	XtRString, "" },
    {
	"euc", "Euc", XtRString,
	sizeof(String), Offset( euc ),
	XtRString, "" },
    {
	"jis", "Jis", XtRString,
	sizeof(String), Offset( jis ),
	XtRString, "" },
    {
	"kuten", "Kuten", XtRString,
	sizeof(String), Offset( kuten ),
	XtRString, "" },
    {
	"supplementaryEuc", "SupplementaryEuc", XtRString,
	sizeof(String), Offset( supplementary_euc ),
	XtRString, "" },
    {
	"supplementaryJis", "SupplementaryJis", XtRString,
	sizeof(String), Offset( supplementary_jis ),
	XtRString, "" },
    {
	"supplementaryKuten", "SupplementaryKuten", XtRString,
	sizeof(String), Offset( supplementary_kuten ),
	XtRString, "" },
    {
	"server", "Server", XtRString,
	sizeof(String), Offset( server ),
	XtRCallProc, (caddr_t)get_sj3serv },
    {
	"wrapToSelect", "WrapToSelect", XtRInt,
	sizeof(int), Offset( wrap_to_select ),
	XtRImmediate, (caddr_t)5 },
    {
	"initialMode", "InitialMode", XmwRConversionMode,
	sizeof(ConversionMode), Offset( initial_mode ),
	XtRString, "ZenkakuHiragana" },
    {
	"defaultCode", "DefaultCode", XmwRCharacterCode,
	sizeof(CharacterCode), Offset( code ),
	XtRString, "sjis" },
    {
	"mallocError", "MallocError", XtRString,
	sizeof(String), Offset( malloc_error ),
	XtRString, "Not enough memory" },
    {
	"supplementaryKanjiError", "SupplementaryKanjiError", XtRString,
	sizeof(String), Offset( supplementary_kanji_error ),
	XtRString, "" },
    {
	"diedError", "DiedError", XtRString,
	sizeof(String), Offset( died_error ),
	XtRString, "" },
    {
	"openError1", "OpenError1", XtRString,
	sizeof(String), Offset( open_error1 ),
	XtRString, "" },
    {
	"openError2", "OpenError2", XtRString,
	sizeof(String), Offset( open_error2 ),
	XtRString, "" },
    {
	"openError3", "OpenError3", XtRString,
	sizeof(String), Offset( open_error3 ),
	XtRString, "" },
    {
	"openError4", "OpenError4", XtRString,
	sizeof(String), Offset( open_error4 ),
	XtRString, "" },
    {
	"openError5", "OpenError5", XtRString,
	sizeof(String), Offset( open_error5 ),
	XtRString, "" },
    {
	"openError6", "OpenError6", XtRString,
	sizeof(String), Offset( open_error6 ),
	XtRString, "" },
    {
	"openError7", "OpenError7", XtRString,
	sizeof(String), Offset( open_error7 ),
	XtRString, "" },
    {
	"closeError1", "CloseError1", XtRString,
	sizeof(String), Offset( close_error1 ),
	XtRString, "" },
    {
	"closeError2", "CloseError2", XtRString,
	sizeof(String), Offset( close_error2 ),
	XtRString, "" },
    {
	"closeError3", "CloseError3", XtRString,
	sizeof(String), Offset( close_error3 ),
	XtRString, "" },
    {
	"closeError4", "CloseError4", XtRString,
	sizeof(String), Offset( close_error4 ),
	XtRString, "" },
    {
	"closeError5", "CloseError5", XtRString,
	sizeof(String), Offset( close_error5 ),
	XtRString, "" },
    {
	"closeError6", "CloseError6", XtRString,
	sizeof(String), Offset( close_error6 ),
	XtRString, "" },
    {
	"closeError7", "CloseError7", XtRString,
	sizeof(String), Offset( close_error7 ),
	XtRString, "" },
    {
	"closeError8", "CloseError8", XtRString,
	sizeof(String), Offset( close_error8 ),
	XtRString, "" },
    {
	"addError1", "AddError1", XtRString,
	sizeof(String), Offset( add_error1 ),
	XtRString, "" },
    {
	"addError2", "AddError2", XtRString,
	sizeof(String), Offset( add_error2 ),
	XtRString, "" },
    {
	"addError3", "AddError3", XtRString,
	sizeof(String), Offset( add_error3 ),
	XtRString, "" },
    {
	"addError4", "AddError4", XtRString,
	sizeof(String), Offset( add_error4 ),
	XtRString, "" },
    {
	"addError5", "AddError5", XtRString,
	sizeof(String), Offset( add_error5 ),
	XtRString, "" },
    {
	"addError6", "AddError6", XtRString,
	sizeof(String), Offset( add_error6 ),
	XtRString, "" },
    {
	"addError7", "AddError7", XtRString,
	sizeof(String), Offset( add_error7 ),
	XtRString, "" },
    {
	"deleteError1", "DeleteError1", XtRString,
	sizeof(String), Offset( delete_error1 ),
	XtRString, "" },
    {
	"deleteError2", "DeleteError2", XtRString,
	sizeof(String), Offset( delete_error2 ),
	XtRString, "" },
    {
	"deleteError3", "DeleteError3", XtRString,
	sizeof(String), Offset( delete_error3 ),
	XtRString, "" },
    {
	"deleteError4", "DeleteError4", XtRString,
	sizeof(String), Offset( delete_error4 ),
	XtRString, "" },
    {
	"deleteError5", "DeleteError5", XtRString,
	sizeof(String), Offset( delete_error5 ),
	XtRString, "" },
    {
	"deleteError6", "DeleteError6", XtRString,
	sizeof(String), Offset( delete_error6 ),
	XtRString, "" },
    {
	"learningError1", "LearningError1", XtRString,
	sizeof(String), Offset( learning_error1 ),
	XtRString, "" },
    {
	"learningError2", "LearningError2", XtRString,
	sizeof(String), Offset( learning_error2 ),
	XtRString, "" },
    {
	"conversionError1", "ConversionError1", XtRString,
	sizeof(String), Offset( conversion_error1 ),
	XtRString, "" },
    {
	"hinshi1", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[0] ),
	XtRString, "" },
    {
	"hinshi2", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[1] ),
	XtRString, "" },
    {
	"hinshi3", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[2] ),
	XtRString, "" },
    {
	"hinshi4", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[3] ),
	XtRString, "" },
    {
	"hinshi5", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[4] ),
	XtRString, "" },
    {
	"hinshi6", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[5] ),
	XtRString, "" },
    {
	"hinshi7", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[6] ),
	XtRString, "" },
    {
	"hinshi8", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[7] ),
	XtRString, "" },
    {
	"hinshi9", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[8] ),
	XtRString, "" },
    {
	"hinshi10", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[9] ),
	XtRString, "" },
    {
	"hinshi11", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[10] ),
	XtRString, "" },
    {
	"hinshi12", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[11] ),
	XtRString, "" },
    {
	"hinshi13", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[12] ),
	XtRString, "" },
    {
	"hinshi14", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[13] ),
	XtRString, "" },
    {
	"hinshi15", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[14] ),
	XtRString, "" },
    {
	"hinshi16", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[15] ),
	XtRString, "" },
    {
	"hinshi17", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[16] ),
	XtRString, "" },
    {
	"hinshi18", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[17] ),
	XtRString, "" },
    {
	"hinshi19", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[18] ),
	XtRString, "" },
    {
	"hinshi20", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[19] ),
	XtRString, "" },
    {
	"hinshi21", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[20] ),
	XtRString, "" },
    {
	"hinshi22", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[21] ),
	XtRString, "" },
    {
	"hinshi23", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[22] ),
	XtRString, "" },
    {
	"hinshi24", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[23] ),
	XtRString, "" },
    {
	"hinshi25", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[24] ),
	XtRString, "" },
    {
	"hinshi26", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[25] ),
	XtRString, "" },
    {
	"hinshi27", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[26] ),
	XtRString, "" },
    {
	"hinshi28", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[27] ),
	XtRString, "" },
    {
	"hinshi29", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[28] ),
	XtRString, "" },
    {
	"hinshi30", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[29] ),
	XtRString, "" },
    {
	"hinshi31", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[30] ),
	XtRString, "" },
    {
	"hinshi32", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[31] ),
	XtRString, "" },
    {
	"hinshi33", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[32] ),
	XtRString, "" },
    {
	"hinshi34", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[33] ),
	XtRString, "" },
    {
	"hinshi35", "Hinshi", XtRString,
	sizeof(String), Offset( hinshi[34] ),
	XtRString, "" },
    {
	"processStartKeys", "ProcessStartKeys", XmwRProcessStartKeys,
	sizeof(XimpKey *), Offset( process_start_keys ),
	XtRString, "<Key>Kanji" },
    {
	"autoReplace", "AutoReplace", XtRBoolean,
	sizeof(Boolean), Offset( auto_replace ),
	XtRString, "True" },
    {
	"ignorePreeditColor", "IgnorePreeditColor", XtRBoolean,
	sizeof(Boolean), Offset( ignore_preedit_color ),
	XtRString, "False" },
    {
	"ignoreStatusColor", "IgnoreStatusColor", XtRBoolean,
	sizeof(Boolean), Offset( ignore_status_color ),
	XtRString, "False" },
    {
	"autoRaise", "AutoRaise", XtRBoolean,
	sizeof(Boolean), Offset( auto_raise ),
	XtRString, "True" },
    {
	"conversionMessage", "MultiClientMessage", XtRBoolean,
	sizeof(Boolean), Offset( conversion_message ),
	XtRString, "False" },
    {
	"resetMessage", "MultiClientMessage", XtRBoolean,
	sizeof(Boolean), Offset( reset_message ),
	XtRString, "False" },
    {
	"kanaMode", "KanaMode", XtRBoolean,
	sizeof(Boolean), Offset( kana_mode ),
	XtRString, "False" },
    {
	"debug", "Debug", XtRInt,
	sizeof(int), Offset( debug ),
	XtRInt, 0 },
#ifdef USE_SJX
    {
	"sjxProtocol", "SjxProtocol", XtRBoolean,
	sizeof(Boolean), Offset( sjx_protocol ),
	XtRString, "True" },
#endif
#ifdef USE_XLC
    {
	"xlcProtocol", "XlcProtocol", XtRBoolean,
	sizeof(Boolean), Offset( xlc_protocol ),
	XtRString, "True" },
#endif
#ifdef USE_XIMP
    {
	"ximProtocol", "XimProtocol", XtRInt,
	sizeof(int), Offset( xim_protocol ),
	XtRImmediate, (caddr_t)4 },
#endif
#if defined(USE_XIMCP)
    {
	"ximcProtocol", "XimcProtocol", XtRBoolean,
	sizeof(Boolean), Offset( ximc_protocol ),
	XtRString, "True" },
#endif
};

static XtResource code_resources[] = {
    {
	"shiftJis", "ShiftJis", XtRString,
	sizeof(String), Offset( code_sjis ),
	XtRString, "" },
    {
	"euc", "Euc", XtRString,
	sizeof(String), Offset( code_euc ),
	XtRString, "" },
    {
	"jis", "Jis", XtRString,
	sizeof(String), Offset( code_jis ),
	XtRString, "" },
    {
	"kuten", "Kuten", XtRString,
	sizeof(String), Offset( code_kuten ),
	XtRString, "" }
};

Sjxa	resources;


void	GetResources( widget )
    Widget	widget;
{
    int		 ii, length;

    XtGetApplicationResources( widget, &resources, local_resources,
			       XtNumber(local_resources), NULL, 0 );

    for( ii = length = 0; ii < 10; ii++ )
	if( length < strlen(resources.function_label[ii]) )
	    length = strlen(resources.function_label[ii]);
    resources.function_items = calloc( 1, length * 10 + 1 );
    resources.function_par_length = length;
    for( ii = 0; ii < 10; ii++ )
	strcpy( &resources.function_items[ii*length],
		resources.function_label[ii] );

    XtGetSubresources( widget, &resources, "code", "Code", code_resources,
		       XtNumber(code_resources), NULL, 0 );

    if( resources.kana_mode )
	XtGetSubresources( widget, &resources, "kana", "Kana", local_resources,
			   XtNumber(local_resources), NULL, 0 );
}


Boolean
#if NeedFunctionPrototypes
is_started_key( XKeyEvent *event )
#else /* NeedFunctionPrototypes */
is_started_key( event )
    XKeyEvent	*event;
#endif /* NeedFunctionPrototypes */
{
    XimpKey	*list;
    KeySym	 keysym;

    keysym = XLookupKeysym( event, 0 );
    for( list = resources.process_start_keys; list->keysym != XK_VoidSymbol;
      list++ ) {
	if( (keysym == list->keysym)  &&
	    ((event->state & list->modifier_mask) == list->modifier) )
	    return( True );
    }
    return( False );
}
