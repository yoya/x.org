/*
 *			  COPYRIGHT 1986
 *		   DIGITAL EQUIPMENT CORPORATION
 *		       MAYNARD, MASSACHUSETTS
 *			ALL RIGHTS RESERVED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
 * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
 * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE FOR
 * ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
 *
 * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT RIGHTS,
 * APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN ADDITION TO THAT
 * SET FORTH ABOVE.
 *
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting documentation,
 * and that the name of Digital Equipment Corporation not be used in advertising
 * or publicity pertaining to distribution of the software without specific, 
 * written prior permission.
 */

/* Defines for Command Button Input Parsing */
#define T_COMMAND_TYPE 			1
#define T_COMMAND_TEXT 			2
#define T_COMMAND_PIXM 			3
#define T_COMMAND_FONTINFO 		4
#define T_COMMAND_TXCOLOR 		5
#define T_COMMAND_PIXMW 		6
#define T_COMMAND_PIXMH 		7
#define T_COMMAND_HITYPE 		8
#define T_COMMAND_BACKGRCOLOR 		9
#define T_COMMAND_BRCOLOR 		10
#define T_COMMAND_BRWIDTH 		11
#define T_COMMAND_SELSTATE 		12
#define T_COMMAND_PROC 			13
#define T_COMMAND_TAG 			14
#define T_COMMAND_ONHI 			15
#define T_COMMAND_OFFHI 		16
#define T_COMMAND_NOTIFY 		17
#define T_COMMAND_REFRESH 		18

/* Defines for Boolean Button Input Parsing */
#define T_BOOLEAN_TYPE 			1
#define T_BOOLEAN_TEXT0			2
#define T_BOOLEAN_TEXT1 		3
#define T_BOOLEAN_PIXM0 	        4
#define T_BOOLEAN_PIXM1 		5
#define T_BOOLEAN_FONTINFO 		6
#define T_BOOLEAN_TXCOLOR 		7
#define T_BOOLEAN_PIXMW 		8
#define T_BOOLEAN_PIXMH 		9
#define T_BOOLEAN_HITYPE 		10
#define T_BOOLEAN_BACKGRCOLOR 		11
#define T_BOOLEAN_BRCOLOR 		12
#define T_BOOLEAN_BRWIDTH 		13
#define T_BOOLEAN_SELSTATE 		14
#define T_BOOLEAN_PROC 			15
#define T_BOOLEAN_TAG 			16
#define T_BOOLEAN_ONHI 			17
#define T_BOOLEAN_OFFHI 		18
#define T_BOOLEAN_NOTIFY 		19
#define T_BOOLEAN_REFRESH 		20


/* Defines for Scroll Bar Input Parsing */
#define T_SCROLLBAR_ORIENT 		1
#define T_SCROLLBAR_STHICK 		2
#define T_SCROLLBAR_THUMBPOS 		3
#define T_SCROLLBAR_THUMBSIZE 		4
#define T_SCROLLBAR_BRCOLOR 		5
#define T_SCROLLBAR_BACKGROUND 		6
#define T_SCROLLBAR_BRWIDTH 		7
#define T_SCROLLBAR_THUMBCOLOR 		8
#define T_SCROLLBAR_TPROC		9
#define T_SCROLLBAR_PPROC 		10
#define T_SCROLLBAR_TAG 		11
#define T_SCROLLBAR_NOTIFY 		12
#define T_SCROLLBAR_REFRESH 		13

/* Defines for Scroll Bar INput Parsing */
#define T_VALUATOR_TYPE			1
#define T_VALUATOR_ORIENT		2
#define T_VALUATOR_MAXDIM		3
#define T_VALUATOR_MINDIM		4
#define T_VALUATOR_INDPOS		5
#define T_VALUATOR_INDSIZE		6
#define T_VALUATOR_BRCOLOR		7
#define T_VALUATOR_BACKGROUND		8
#define T_VALUATOR_BRWIDTH		9
#define T_VALUATOR_INDCOLOR		10
#define T_VALUATOR_PROC			11
#define T_VALUATOR_TAG			12
#define T_VALUATOR_NOTIFY		13
#define T_VALUATOR_REFRESH		14

/* Defines for Label Input Parsing */
#define T_LABEL_STRING			1
#define T_LABEL_FONTINFO		2
#define T_LABEL_WIDTH			3
#define T_LABEL_HEIGHT			4
#define T_LABEL_JUSTIFY			5
#define T_LABEL_BORDERWIDTH		6
#define T_LABEL_FOREGROUND_PIXEL	7

/* Defines for Text Input Parsing */
#define T_TEXT_TOP			1
#define T_TEXT_SEL_TYPE			2
#define T_TEXT_SEL_MUTUAL_EXCLUDE	3
#define T_TEXT_OPTIONS			4
#define T_TEXT_LEFT_MARGIN		5
#define T_TEXT_INSERT_POS		6
#define T_TEXT_WORD_DELIM		7
#define T_TEXT_NOTIFY_SYMBOLS		8
#define T_TEXT_NOTIFY_ROUTINE		9
#define T_TEXT_NOTIFY_TAG		10
#define T_TEXT_SOURCE			11
#define T_TEXT_WIDTH			12
#define T_TEXT_HEIGHT			13
#define T_TEXT_BRWIDTH			14
#define T_TEXT_HIGHLIGHT		15
#define T_TEXT_UNHIGHLIGHT		16
#define T_TEXT_REPLACE			17
#define T_TEXT_FONT_INFO		18

/* Defines for Menu Input Parsing */
#define T_MENU_X			1
#define T_MENU_Y			2
#define T_MENU_WIDTH			3
#define T_MENU_HEIGHT			4
#define T_MENU_ROWS			5
#define T_MENU_COLS			6
#define T_MENU_BORDER			7
#define T_MENU_BDRWIDTH			8
#define T_MENU_BACKGROUND		9
#define T_MENU_FOREGROUND		10
#define T_MENU_CURSOR			11
#define T_MENU_PROC			12
#define T_MENU_TAG			13
#define T_MENU_NOTIFY			14

/* Defines for Menu Entry Input Parsing */
#define T_MENUENTRY_ROW			1
#define T_MENUENTRY_COL			2
#define T_MENUENTRY_WINDOW		3
#define T_MENUENTRY_FLAGS		4

/* Defines for Menu Entry Flags */
#define IsTitle				1

/* Defines for General Form Input Parsing */
#define T_FORM_BACKGROUND               1
#define T_FORM_BORDER                   2
#define T_FORM_BDRWIDTH                 3
#define T_FORM_WIDTH                    4
#define T_FORM_HEIGHT                   5
#define T_FORM_X                        6
#define T_FORM_Y                        7
#define T_FORM_TOOLS                    8
#define T_FORM_HINTS                    9
#define T_FORM_FORMATTER                10

/* Defines for Form Item Input Parsing */
#define T_FORMITEM_WIDTH                1
#define T_FORMITEM_HEIGHT               2
#define T_FORMITEM_X                    3
#define T_FORMITEM_Y                    4
#define T_FORMITEM_W_HINT               5
#define T_FORMITEM_H_HINT               6
#define T_FORMITEM_X_HINT               7
#define T_FORMITEM_Y_HINT               8

/* Defines for Paned Window Input Parsing (internal panes) */
#define T_PANED_ORIENT                  1
#define T_PANED_POSITION                2
#define T_PANED_HEIGHT                  3
#define T_PANED_WIDTH                   4
#define T_PANED_MIN                     5
#define T_PANED_MAX                     6
#define T_PANED_AUTOCHANGE              7
#define T_PANED_BRCOLOR                 8
#define T_PANED_BACKGROUND              9
#define T_PANED_WINDOW		       10

/* TArg typedef and macro */
typedef struct _Targ {
	int name;
	caddr_t data;
} Targ;

#define TSetArg(arg, n, d) \
	( (arg).name = (n), \
	(arg).data = (caddr_t)(d) )
