/* @(#)KeyPress.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
/*

Copyright (c) 1991, 1992, 1993, 1994  FUJITSU LIMITED

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE FUJITSU LIMITED BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the FUJITSU LIMITED shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the FUJITSU LIMITED.

  Author: Takashi Fujiwara     FUJITSU LIMITED 
                               fujiwara@a80.tech.yk.fujitsu.co.jp
          Kiyoaki oya          FUJITSU LIMITED
	  Riki Kawaguchi       FUJITSU LIMITED
	  Toshimitu Terazono   FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include <ctype.h>
#ifndef _toupper
#define _toupper(c)    ((c)-'a'+'A')
#endif /* _toupper */
#ifndef _tolower
#define _tolower(c)    ((c)-'A'+'a')
#endif /* _tolower */

#define XK_KATAKANA
#include "Xfeoak.h"
#include <X11/keysym.h>
#include <X11/Xutil.h>
#include "KanjiTbl.h"
#include "TransTbl.h"

int	 C_key_count;

Static void
setup_conversion_sw()
{
    int			 i;
    unsigned int	 modifier;

    for (i = 0; conversionSW[i].modifier_name; i++) {
	C_key[i*3] = ParseModifierList(keybord_type,
				conversionSW[i].modifier_name);
	C_key[(i*3)+1] = ParseModifierList(keybord_type,
				conversionSW[i].modifierMask_name);
	C_key[(i*3)+2] = ParseKeystring(keybord_type,
				conversionSW[i].keysym_name, &modifier);
	C_key[i*3] |= modifier;
    }
    C_key_count = i;
    if (keybord_type != FKEY_NONE)
	C_key_count--;
    return;
}

int
InitialTranslationTable()
{
    if( keybord_type == FKEY_G_JIS || keybord_type == FKEY_DS_JIS )
	transmode = &default_jis_mode;
    else 
    if( keybord_type == FKEY_G_OYA || keybord_type == FKEY_DS_OYA )
	transmode = &default_oak_mode;
    else 
	transmode = &default_non_mode;
    KeystringToKeysym(keybord_type, transmode);
    setup_conversion_sw();
    return( 0);
}

int
IsMode(keysym, mod)
    KeySym		 keysym;
    unsigned int	 mod;
{
    int		 i;

    for (i = 0; i < C_key_count; i++)
	if ((mod & C_key[i*3+1]) == C_key[i*3] && keysym == C_key[i*3+2])
	    return (1);
    return (0);
}

#ifdef SUPPORT_HOTKEY
Static Bool
CheckHotKey(state, keysym, hotkey)
    unsigned int	 state;
    KeySym		 keysym;
    XIMHotKeytriggers	*hotkey;
{
    int			 num = hot_key->num_hot_key;
    XIMHotKeyTrigger	*key = hot_key->key;
    register int	 i;

    for (i = 0; i < num; i++) {
	if ((key[i].keysym == keysym)
	 && ((state & key[i].modifier_mask) == key[i].modifier))
	    return Trur;
    }
    return False;
}
#endif

int CurrentKeyPress( event)
    XEvent         *event;
    {
    char           string[128];
    KeySym         keysym;
    int            i, count;
    static XComposeStatus statusmode = {0, 0};
    int		event_type;
    int		f_mach;
    FtTransTable	*current_trans_table;

    bzero( string, 128);
    count = XmuLookupKana( event, string, 128, &keysym, &statusmode );
    event_type  = event->type;

#ifdef SUPPORT_HOTKEY
    if (current_client->hotkey_state == XIMHotKeyStateON) {
	if (CheckHotKey(event->state, keysym, current_client->hotkey))
	    return;
    }
#endif

    if( current_client->henkan_mode == 1) {
	if( focus_in_client != current_client ) {
	    SetFocusClient();
	    }
	switch( current_client->siftmode  ) {	
	case FS_HIRAGANA:
		current_trans_table = transmode->hiragana;
		break;
	case FS_KATAKANA:
		current_trans_table = transmode->katakana;
		break;
	case FS_UPPER_ASCII:
		current_trans_table = transmode->bigascii;
		break;
	case FS_LOWER_ASCII:
		current_trans_table = transmode->smallascii;
		break;
	    }

	f_mach = TranslationProc( current_trans_table, event,keysym,string,count);
	if( f_mach == 0  
	 && event_type == KeyPress ) {
    	    if( current_client->select_kanji_mod ) 
		Beep();
	    else 
	        SendKeyEvent( event);
	    }
	}
    else {
	if( transmode->henkanoff ) {
	    current_trans_table = transmode->henkanoff;
	    f_mach = TranslationProc( current_trans_table, event,keysym,string,count );
	    }
	if(event_type == KeyPress 
	 && !IsMode( keysym, event->xkey.state))
	        SendKeyEvent( event);
	}

    if( event_type == KeyPress
    && IsMode( keysym, event->xkey.state) ) {
	if( current_client->prot_type == FPRT_XIM ) {
	    Fj_ChangeMode( event,keysym,string,count,FK_NIHONGO);
	    if( current_client->henkan_mode == 0 )
		XimCnversionEnd();
	    }
        }
    }

int
TranslationProc( current_trans_table, event,keysym,string,count )
    FtTransTable   *current_trans_table;
    XEvent         *event;
    char           *string;
    KeySym         keysym;
    int            count;
    {
    unsigned int   event_state;
    int		   event_type;
    unsigned int   f_state;
    KeySym	   f_keysym;
    int		jj, f_mach = 0;

    event_state = event->xkey.state & (ControlMask|ShiftMask);
    event_type  = event->type;

	for(; current_trans_table!=0; current_trans_table=current_trans_table->next_item) {
	    f_state = current_trans_table->state;
	    if( (f_state & FtKeyUp) != 0) {
		if( event_type == KeyRelease ) {
		    f_state &= ~FtKeyUp;
		    }
		else
		    continue;
		}
	    else if( event_type == KeyPress ) {
		f_state &= ~FtKeyUpDown;
		}
	    else {
		    continue;
		}

	    if( f_state == AnyModifier
	    || event_state  == f_state ) {
		f_keysym = current_trans_table->keysym;
		if( f_keysym == FT_KANA ) {
                    if((keysym < XK_kana_fullstop)
		    || (keysym > XK_semivoicedsound)) 
		        continue;
		    }
	        else
		if( f_keysym == FT_ASCII ) {
                    if((keysym <  XK_space)
		    || (keysym >  XK_asciitilde)) 
		        continue;
		    }
	        else
		if( keysym != f_keysym ) {
		    continue;
		    }
		(*current_trans_table->proc)( event,keysym,string,count,current_trans_table->arg);
		f_mach = 1;
		}
	    }
	return( f_mach );
	}

SendKeyEvent( event)
    XEvent      *event;
    {
    if( current_client != root_client) {
	if( current_client->prot_type == FPRT_XIM )
	    (void)XimSendKeyEvent( event );
	}
    }

int
SendBuffByLength( window, input_data, input_length)
    Window	window;
    ushort	*input_data;
    int		input_length;
    {
    if( current_client->prot_type == FPRT_XIM )
	return XimSendBuffByLength( current_client->focus_window,
					input_data, input_length );
    }

int
Fj_PutAscii( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    int		num;
    if( current_client->select_kanji_mod ) {
	num = (int)*string - (int)'0';
	fj_ExecuteSelectKanji( num);
	}
    else
    if(( !current_client->romaji_mode ) 
    && ( current_client->logical_kanashift ) ) {
	AsciiToKanaInJISkeybord( string, length );
	if( event->xkey.state == 0 && keysym == XK_space )
	    *string = ' ';
	fj_PutKana( string,length);
	}
    else {
	if(( event->xkey.state == (LockMask|ShiftMask) ) 
	|| ( current_client->logical_capslock != 0 )) {
	    int ii;
	    char *cc;
	    for( ii=length,cc=string; ii>0; ii--, cc++) {
		if( isupper(*cc) )
		  *cc = _tolower(*cc);
		else
		if( islower(*cc) )
		  *cc = _toupper(*cc);
	        }
	    }
	fj_PutAscii( string,length);
	}
    }

int
Fj_PutKana( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	if(( keybord_type != FKEY_G_OYA )
	&& ((current_client->siftmode & FS_KANA) != 0 )) {
	    int num = -1;
	    switch( (int)*string ) {
	    case 0xc7: num = 1; break;
	    case 0xcc: num = 2; break;
	    case 0xb1: num = 3; break;
	    case 0xb3: num = 4; break;
	    case 0xb4: num = 5; break;
	    case 0xb5: num = 6; break;
	    case 0xd4: num = 7; break;
	    case 0xd5: num = 8; break;
	    case 0xd6: num = 9; break;
	    case 0xdc: num = 0; break;
		}
	    fj_ExecuteSelectKanji( num);
	    }
	else
	    Beep();
	}
    else {
	fj_PutKana( string,length);
	}
    }

int
Fj_Henkan( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	fj_MoveCursorSelectKanji( KS_NEXT);
	}
    else {
	fj_Henkan();
	}
    }

int
Fj_Muhenkan( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	fj_MoveCursorSelectKanji( KS_PREV);
	}
    else {
	fj_Muhenkan();
	}
    }

int
Fj_Maekouho( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	fj_MoveCursorSelectKanji( KS_PREV);
	}
    else {
	fj_Maekouho();
	}
    }

int
Fj_HenkanCancel( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	fj_CancelSelectKanji();
	}
    else {
	fj_HenkanCancel();
	}
    }

int
Fj_HenkanExecute( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	fj_ExecuteSelectKanji( current_client->select_kanji_no);
	}
    else {
	fj_exec();
	}
    }

int
Fj_HenkanExecuteLinefeed( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	fj_MoveCursorSelectKanji( KS_NEXT);
	}
    else {
	if( current_client->convert_count != 0) {
	    fj_exec();
	    SendKeyEvent( event );
	    }
	else
	if( current_client->last_colums == 0 ) {
	    SendKeyEvent( event );
	    }
	else
	    Beep();
	}
    }

int
Fj_CursorMoveTop( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	fj_MoveCursorSelectKanji( KS_PREV);
	}
    else {
	if( current_client->convert_count != 0
	&&  current_client->current_colums == current_client->last_colums ) {
	    fj_exec();
	    SendKeyEvent( event );
	    }
	else
	if( current_client->last_colums == 0 ) {
	    SendKeyEvent( event );
	    }
	else
	    fj_CursorMove( KS_TOP);
	}
    }

int
Fj_CursorMoveBottom( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	fj_MoveCursorSelectKanji( KS_NEXT);
	}
    else {
	if( current_client->convert_count != 0
	&&  current_client->current_colums == current_client->last_colums ) {
	    fj_exec();
	    SendKeyEvent( event );
	    }
	else
	if( current_client->last_colums == 0 ) {
	    SendKeyEvent( event );
	    }
	else
	    fj_CursorMove( KS_BOTTOM);
	}
    }

int
Fj_CursorMoveFoward( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	fj_MoveCursorSelectKanji( KS_FOWARD);
	}
    else {
	if( current_client->convert_count != 0
	&&  current_client->current_colums == current_client->last_colums ) {
	    fj_exec();
	    SendKeyEvent( event );
	    }
	else
	if( current_client->last_colums == 0 ) {
	    SendKeyEvent( event );
	    }
	else
	    fj_CursorMove( KS_FOWARD);
	}
    }

int
Fj_CursorMoveBackward( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	fj_MoveCursorSelectKanji( KS_BACKWARD);
	}
    else {
	if( current_client->convert_count != 0
	&&  current_client->current_colums == current_client->last_colums ) {
	    fj_exec();
	    SendKeyEvent( event );
	    }
	else
	if( current_client->last_colums == 0 ) {
	    SendKeyEvent( event );
	    }
	else
	    fj_CursorMove( KS_BACKWARD);
	}
    }

int
Fj_DeleteChar( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	Beep();
	}
    else {
	fj_DeleteChar();
	}
    }

int
Fj_InsertChar( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	Beep();
	}
    else {
	fj_InsertChar();
	}
    }

int
Fj_SelectKanji( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    if( current_client->select_kanji_mod ) {
	Beep();
	}
    else {
	fj_SelectKanji( arg);
	}
    }

int
Fj_SendEvent( event,keysym,string,length,arg)
    XEvent      *event;
    KeySym      keysym;
    char	*string;
    int		length;
    FtArg	arg;
    {
    SendKeyEvent( event);
    }
