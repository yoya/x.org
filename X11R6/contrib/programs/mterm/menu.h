/*
 *	$Header: /var/home/sm/ws/michael/work/xterm/RCS/menu.h,v 1.2 1994/06/07 00:05:30 michael Exp $ Sony Corporation
 */
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
/* $XConsortium: menu.h,v 1.25 94/04/17 20:23:31 gildea Exp $ */
/*

Copyright (c) 1989  X Consortium

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
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/

typedef struct _MenuEntry {
    char *name;
    void (*function)();
    Widget widget;
} MenuEntry;

extern MenuEntry mainMenuEntries[], vtMenuEntries[], tekMenuEntries[];
extern MenuEntry fontMenuEntries[];
extern Arg menuArgs[];

extern void HandleAllowSends();
extern void HandleSetVisualBell();
#ifdef ALLOWLOGGING
extern void HandleLogging();
#endif
extern void HandleRedraw();
extern void HandleSendSignal();
extern void HandleQuit();
extern void HandleScrollbar();
extern void HandleJumpscroll();
extern void HandleReverseVideo();
extern void HandleAutoWrap();
extern void HandleReverseWrap();
extern void HandleAutoLineFeed();
extern void HandleAppCursor();
extern void HandleAppKeypad();
extern void HandleScrollKey();
extern void HandleScrollTtyOutput();
extern void HandleAllow132();
extern void HandleCursesEmul();
extern void HandleMarginBell();
extern void HandleAltScreen();
extern void HandleSoftReset();
extern void HandleHardReset();
extern void HandleClearSavedLines();
extern void HandleSetTerminalType();
extern void HandleVisibility();
extern void HandleSetTekText();
extern void HandleTekPage();
extern void HandleTekReset();
extern void HandleTekCopy();
extern void DoSecureKeyboard();
#ifdef STATUSLINE
extern void HandleStatusLine();
extern void HandleReverseStatus();
#endif /* STATUSLINE */

/*
 * The following definitions MUST match the order of entries given in 
 * the mainMenuEntries, vtMenuEntries, and tekMenuEntries arrays in menu.c.
 */

/*
 * items in primary menu
 */
#define mainMenu_securekbd 0
#define mainMenu_allowsends 1
#ifdef ALLOWLOGGING
#define mainMenu_logging 2
#endif
#define mainMenu_redraw 3
#define mainMenu_line1 4
#define mainMenu_suspend 5
#define mainMenu_continue 6
#define mainMenu_interrupt 7
#define mainMenu_hangup 8
#define mainMenu_terminate 9
#define mainMenu_kill 10
#define mainMenu_line2 11
#ifdef I18N
#ifdef ALLLOWLOGGING
#define mainMenu_latin1 12
#define mainMenu_latin2 13
#define mainMenu_latin3 14
#define mainMenu_latin4 15
#define mainMenu_cyrillic 16
#define mainMenu_arabic 17
#define mainMenu_greek 18
#define mainMenu_hebrew 19
#define mainMenu_latin5 20
#define mainMenu_jis 21
#define mainMenu_sjis 22
#define mainMenu_jeuc 23
#define mainMenu_keuc 24
#define mainMenu_ceuc 25
#define mainMenu_line3 26
#define mainMenu_quit 27
#else /* ALLOWLOGGING */
#define mainMenu_latin1 11
#define mainMenu_latin2 12
#define mainMenu_latin3 13
#define mainMenu_latin4 14
#define mainMenu_cyrillic 15
#define mainMenu_arabic 16
#define mainMenu_greek 17
#define mainMenu_hebrew 18
#define mainMenu_latin5 19
#define mainMenu_jis 20
#define mainMenu_sjis 21
#define mainMenu_jeuc 22
#define mainMenu_keuc 23
#define mainMenu_ceuc 24
#define mainMenu_line3 25
#define mainMenu_quit 26
#endif /* ALLOWLOGGING */
#else /* I18N */
#define mainMenu_quit 12
#endif /* I18N */


/*
 * items in vt100 mode menu
 */
#define vtMenu_scrollbar 0
#define vtMenu_jumpscroll 1
#define vtMenu_reversevideo 2
#define vtMenu_autowrap 3
#define vtMenu_reversewrap 4
#define vtMenu_autolinefeed 5
#define vtMenu_appcursor 6
#define vtMenu_appkeypad 7
#define vtMenu_scrollkey 8
#define vtMenu_scrollttyoutput 9
#define vtMenu_allow132 10
#define vtMenu_cursesemul 11
#define vtMenu_visualbell 12
#define vtMenu_marginbell 13
#define vtMenu_altscreen 14
#ifdef STATUSLINE
#define vtMenu_statusline 15
#define vtMenu_reversestatus 16
#define vtMenu_line1 17
#define vtMenu_softreset 18
#define vtMenu_hardreset 19
#define vtMenu_clearsavedlines 20
#define vtMenu_line2 21
#define vtMenu_tekshow 22
#define vtMenu_tekmode 23
#define vtMenu_vthide 24
#else /* STATUSLINE */
#define vtMenu_line1 15
#define vtMenu_softreset 16
#define vtMenu_hardreset 17
#define vtMenu_clearsavedlines 18
#define vtMenu_line2 19
#define vtMenu_tekshow 20
#define vtMenu_tekmode 21
#define vtMenu_vthide 22
#endif /* STATUSLINE */

/*
 * items in vt100 font menu
 */
#define fontMenu_fontdefault 0
#define fontMenu_font1 1
#define fontMenu_font2 2
#define fontMenu_font3 3
#define fontMenu_font4 4
#define fontMenu_font5 5
#define fontMenu_font6 6
#define fontMenu_lastBuiltin fontMenu_font6
#define fontMenu_fontescape 7
#define fontMenu_fontsel 8
/* number of non-line items should match NMENUFONTS in ptyx.h */


/*
 * items in tek4014 mode menu
 */
#define tekMenu_tektextlarge 0
#define tekMenu_tektext2 1
#define tekMenu_tektext3 2
#define tekMenu_tektextsmall 3
#define tekMenu_line1 4
#define tekMenu_tekpage 5
#define tekMenu_tekreset 6
#define tekMenu_tekcopy 7
#define tekMenu_line2 8
#define tekMenu_vtshow 9
#define tekMenu_vtmode 10
#define tekMenu_tekhide 11


/*
 * macros for updating menus
 */

#define update_menu_item(w,mi,val) { if (mi) { \
    menuArgs[0].value = (XtArgVal) ((val) ? term->screen.menu_item_bitmap \
				          : None); \
    XtSetValues (mi, menuArgs, (Cardinal) 1); }}


#define set_sensitivity(w,mi,val) { if (mi) { \
    menuArgs[1].value = (XtArgVal) (val); \
    XtSetValues (mi, menuArgs+1, (Cardinal) 1);  }}



/*
 * there should be one of each of the following for each checkable item
 */


#define update_securekbd() \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_securekbd].widget, \
		    term->screen.grabbedKbd)

#define update_allowsends() \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_allowsends].widget, \
		    term->screen.allowSendEvents)

#ifdef I18N
#define update_inputcode() \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_latin1].widget, \
		    term->screen.icode == T_LATIN1); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_latin2].widget, \
		    term->screen.icode == T_LATIN2); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_latin3].widget, \
		    term->screen.icode == T_LATIN3); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_latin4].widget, \
		    term->screen.icode == T_LATIN4); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_cyrillic].widget, \
		    term->screen.icode == T_CYRILLIC); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_arabic].widget, \
		    term->screen.icode == T_ARABIC); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_greek].widget, \
		    term->screen.icode == T_GREEK); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_hebrew].widget, \
		    term->screen.icode == T_HEBREW); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_latin5].widget, \
		    term->screen.icode == T_LATIN5); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_jis].widget, \
		    term->screen.icode == T_JIS); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_sjis].widget, \
		    term->screen.icode == T_SJIS); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_jeuc].widget, \
		    term->screen.icode == T_JEUC); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_keuc].widget, \
		    term->screen.icode == T_KEUC); \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_ceuc].widget, \
		    term->screen.icode == T_CEUC);
#endif /* I18N */

#ifdef ALLOWLOGGING
#define update_logging() \
  update_menu_item (term->screen.mainMenu, \
		    mainMenuEntries[mainMenu_logging].widget, \
		    term->screen.logging)
#endif

#define update_scrollbar() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_scrollbar].widget, \
		    term->screen.scrollbar)

#define update_jumpscroll() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_jumpscroll].widget, \
		    term->screen.jumpscroll)

#define update_reversevideo() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_reversevideo].widget, \
		    (term->flags & REVERSE_VIDEO))

#define update_autowrap() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_autowrap].widget, \
		    (term->flags & WRAPAROUND))

#define update_reversewrap() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_reversewrap].widget, \
		    (term->flags & REVERSEWRAP))

#define update_autolinefeed() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_autolinefeed].widget, \
		    (term->flags & LINEFEED))

#define update_appcursor() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_appcursor].widget, \
		    (term->keyboard.flags & CURSOR_APL))

#define update_appkeypad() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_appkeypad].widget, \
		    (term->keyboard.flags & KYPD_APL))

#define update_scrollkey() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_scrollkey].widget,  \
		    term->screen.scrollkey)

#define update_scrollttyoutput() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_scrollttyoutput].widget, \
		    term->screen.scrollttyoutput)

#define update_allow132() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_allow132].widget, \
		    term->screen.c132)
  
#define update_cursesemul() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_cursesemul].widget, \
		    term->screen.curses)

#define update_visualbell() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_visualbell].widget, \
		    term->screen.visualbell)

#define update_marginbell() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_marginbell].widget, \
		    term->screen.marginbell)

#define update_altscreen() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_altscreen].widget, \
		    term->screen.alternate)

#ifdef STATUSLINE
#define update_statusline() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_statusline].widget, \
		    term->screen.statusheight)

#define update_reversestatus() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_reversestatus].widget, \
		    term->screen.reversestatus)
#endif /* STATUSLINE */

#define update_tekshow() \
  update_menu_item (term->screen.vtMenu, \
		    vtMenuEntries[vtMenu_tekshow].widget, \
		    term->screen.Tshow)

#define update_vttekmode() { \
    update_menu_item (term->screen.vtMenu, \
		      vtMenuEntries[vtMenu_tekmode].widget, \
		      term->screen.TekEmu) \
    update_menu_item (term->screen.tekMenu, \
		      tekMenuEntries[tekMenu_vtmode].widget, \
		      !term->screen.TekEmu) }

#define update_vtshow() \
  update_menu_item (term->screen.tekMenu, \
		    tekMenuEntries[tekMenu_vtshow].widget, \
		    term->screen.Vshow)


#define set_vthide_sensitivity() \
  set_sensitivity (term->screen.vtMenu, \
		   vtMenuEntries[vtMenu_vthide].widget, \
		   term->screen.Tshow)

#define set_tekhide_sensitivity() \
  set_sensitivity (term->screen.tekMenu, \
		   tekMenuEntries[tekMenu_tekhide].widget, \
		   term->screen.Vshow)

#define set_altscreen_sensitivity(val) \
  set_sensitivity (term->screen.vtMenu,\
		   vtMenuEntries[vtMenu_altscreen].widget, (val))


/*
 * macros for mapping font size to tekMenu placement
 */
#define FS2MI(n) (n)			/* font_size_to_menu_item */
#define MI2FS(n) (n)			/* menu_item_to_font_size */

#define set_tekfont_menu_item(n,val) \
  update_menu_item (term->screen.tekMenu, \
		    tekMenuEntries[FS2MI(n)].widget, \
		    (val))

#define set_menu_font(val) \
  update_menu_item (term->screen.fontMenu, \
		    fontMenuEntries[term->screen.menu_font_number].widget, \
		    (val))
