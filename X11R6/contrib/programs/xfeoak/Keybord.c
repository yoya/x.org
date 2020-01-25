/* @(#)Keybord.c      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
/*

Copyright (c) 1992, 1993, 1994  FUJITSU LIMITED

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
          Riki Kawaguchi       FUJITSU LIMITED
	  Toshimitu Terazono   FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

#include <ctype.h>
#ifndef _toupper
#define _toupper(c)    ((c)-'a'+'A')
#endif
#ifndef _tolower
#define _tolower(c)    ((c)-'A'+'a')
#endif
#include <X11/Xlib.h>
#define XK_KATAKANA
#include <X11/keysym.h>
#include <X11/Xutil.h>

#include "Xfeoak.h"
#include "KanjiTbl.h"
#include "TransTbl.h"

#ifndef NULL
#define	NULL	0
#endif

typedef struct _StrToModifierRec {
    char		*modname;
    unsigned int	 modifier;
} StrToModifierRec;

typedef struct	_StrToKeySymRec {
    char		*keyname;
    unsigned int	 add_modifier;
    KeySym		 keysym;
} StrToKeySymRec;

static StrToModifierRec		G_modifierRec[] = {
    /* Modifier Name */		/* Modifier */
    {"Ctrl",			ControlMask},
    {"Shift",			ShiftMask},
    {"ControlMask",		ControlMask},
    {"ShiftMask",		ShiftMask},
    {"FtKeyUpDown",		FtKeyUpDown},
    {"RomajiMask",		ShiftMask},
    {"FtKeyUp",			FtKeyUp},
    {"",			0},
};

static StrToModifierRec		D_modifierRec[] = {
    /* Modifier Name */		/* Modifier */
    {"Ctrl",			ControlMask},
    {"Shift",			ShiftMask},
    {"ControlMask",		ControlMask},
    {"ShiftMask",		ShiftMask},
    {"FtKeyUpDown",		FtKeyUpDown},
    {"RomajiMask",		ControlMask},
    {"FtKeyUp",			FtKeyUp},
    {(char *)0,			0},
};

static unsigned int
_ParseModifier(modname, modifierRec)
    char		*modname;
    StrToModifierRec	*modifierRec;
{
    int			 i;
    unsigned int	 modifier = 0;

    for (i = 0; modifierRec[i].modname; i++) {
	if (!strcmp(modifierRec[i].modname, modname))
	    break;
    }
    modifier = modifierRec[i].modifier;
    return modifier;
}

static char *
_ParseModifierList(str, modifierRec, modifier)
    char		*str;
    StrToModifierRec	*modifierRec;
    unsigned int	*modifier;
{
    int			 length;
    char		*ptr;
    char		 mod[32];

    *modifier = 0;
    for (; str && *str && *str != '@' && *str != '#';) {
	ptr = str;
	for (; *ptr && *ptr != ':' && *ptr != '@' && *str != '#'; ptr++);
	length = (int )(ptr - str);
	if (length != 0) {
	    strncpy(mod, str, length);
	    mod[length] = '\0';
	    *modifier |= _ParseModifier(mod, modifierRec);
	}
	str = ptr;
	if (*str == ':')
	    str++;
    }
    if (str && *str)
	str++;
    return str;
}

static StrToKeySymRec		G_keysymRec[] = {
    /* keysym name */	    /* Additional Modifier */	/* KeySym */
    {"Kanji",		    0,				XK_Kanji},
    {"ConversionSW",	    0,				XK_Henkan_Mode},
    {"Henkan_Mode",	    0,				XK_Henkan_Mode},
    {"Eisu_toggle",	    0,				XK_Eisu_toggle},
    {"Hiragana_Katakana",   0,				XK_Hiragana_Katakana},
    {"Romaji",		    0,				XK_Romaji},
    {"FT_ASCII",	    0,				FT_ASCII},
    {"FT_KANA",		    0,				FT_KANA},
    {(char *)0,		    0,				NoSymbol},
};

static StrToKeySymRec		D_keysymRec[] = {
    /* keysym name */	    /* Additional Modifier */	/* KeySym */
    {"Kanji",		    0,				XK_Henkan_Mode},
    {"ConversionSW",	    0,				XK_Kanji},
    {"Henkan_Mode",	    0,				XK_Kanji},
    {"Eisu_toggle",	    0,				XK_Eisu_Shift},
    {"Hiragana_Katakana",   0,				XK_Mode_switch},
    {"Romaji",		    0,				XK_Kanji},
    {"FT_ASCII",	    0,				FT_ASCII},
    {"FT_KANA",		    0,				FT_KANA},
    {(char *)0,		    0,				NoSymbol},
};

static void
_ParseKeystring(str, keysymRec, modifier, keysym)
    char		*str;
    StrToKeySymRec	*keysymRec;
    unsigned int	*modifier;
    KeySym		*keysym;
{
    char		*ptr;
    int			 i;
    int			 length;
    char		 mod[32];

    *modifier = 0;
    *keysym   = NoSymbol;
    ptr = str;
    for (; ptr && *ptr; ptr++);
    length = (int )(ptr - str);
    if (length != 0) {
	for (i = 0; keysymRec[i].keyname; i++) {
	    if (!strcmp(keysymRec[i].keyname, str))
		break;
	}
	*modifier = keysymRec[i].add_modifier;
	if ((*keysym = keysymRec[i].keysym) == NoSymbol) {
	    *keysym = XStringToKeysym(str);
	}
    }
    return;
}

static void
_GetModifierRec(keybord_type, modifierRec)
    int			  keybord_type;
    StrToModifierRec	**modifierRec;
{
    if (keybord_type == FKEY_G_OYA || keybord_type == FKEY_G_JIS) {
	*modifierRec = G_modifierRec;
    } else if (keybord_type == FKEY_DS_OYA || keybord_type == FKEY_DS_JIS) {
	*modifierRec = D_modifierRec;
    } else {
	*modifierRec = G_modifierRec;
    }
    return;
}

static void
_GetKeySymRec(keybord_type, keysymRec)
    int			  keybord_type;
    StrToKeySymRec	**keysymRec;
{
    if (keybord_type == FKEY_G_OYA || keybord_type == FKEY_G_JIS) {
	*keysymRec   = G_keysymRec;
    } else if (keybord_type == FKEY_DS_OYA || keybord_type == FKEY_DS_JIS) {
	*keysymRec   = D_keysymRec;
    } else {
	*keysymRec   = G_keysymRec;
    }
    return;
}

int
KeystringToKeysym(keybord_type, mode)
    int			  keybord_type;
    FtTransTable	**mode;
{
    int			 i;
    unsigned int	 modifier;
    KeySym		 keysym;
    char		*str;
    FtTransTable	*tbl;
    StrToModifierRec	*modifierRec;
    StrToKeySymRec	*keysymRec;

    _GetModifierRec(keybord_type, &modifierRec);
    _GetKeySymRec(keybord_type, &keysymRec);

    for (i = 0; i < sizeof(FtTransMode) / sizeof(FtTransTable *); i++) {
	if ((tbl = mode[i]) == NULL) continue;
	for (; tbl; tbl = tbl->next_item) {
	    str = _ParseModifierList(tbl->key, modifierRec, &modifier);
	    tbl->state = modifier;
	    _ParseKeystring(str, keysymRec, &modifier, &keysym);
	    tbl->state |= modifier;
	    tbl->keysym = keysym;
	}
    }
    return 0;
}

unsigned long
ParseModifierList(keybord_type, str)
    int		 keybord_type;
    char	*str;
{
    unsigned int	 modifier;
    StrToModifierRec	*modifierRec;

    _GetModifierRec(keybord_type, &modifierRec);
    _ParseModifierList(str, modifierRec, &modifier);
    return modifier;
}

KeySym
ParseKeystring(keybord_type, str, modifier)
    int			 keybord_type;
    char		*str;
    unsigned int	*modifier;
{
    KeySym		 keysym;
    StrToKeySymRec	*keysymRec;

    _GetKeySymRec(keybord_type, &keysymRec);
    _ParseKeystring(str, keysymRec, modifier, &keysym);
    return keysym;
}
