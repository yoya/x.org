/* 
 *		Copyright IBM Corporation 1989
 *
 *                      All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation, and that the name of IBM not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 *
 * IBM DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
 * IBM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
 * ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
 * ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *
 * $Header: functbl.h,v 4.1 89/09/06 16:02:57 amy Exp $ 
 */

#include "screen_items.h"

/*
 *  tbls of functions asscociated with screen_items
 */

struct popup_tbl_entry
{
	KEY key;
	int (*init)();
	int (*exec)();
	int (*cleanup)();
};

extern struct popup_tbl_entry popup_func_tbl[];
extern int popup_func_tbl_sz;
extern IDX find_popup_func_tbl_idx();

struct menu_item_tbl_entry
{
	KEY key;
	int (*init)();
	int (*restart)();
	int (*exec)();
	int (*cleanup)();
};

extern struct menu_item_tbl_entry menu_item_func_tbl[];
extern int menu_item_func_tbl_sz;
extern IDX find_menu_item_func_tbl_idx();

struct menu_tbl_entry
{
	KEY key;
	int (*init)();
	int (*cleanup)();
};

extern struct menu_tbl_entry menu_func_tbl[];
extern int menu_func_tbl_sz;
extern IDX find_menu_func_tbl_idx();


/*
 *  screen areas
 *
 *  there will be a table of screen area types and the routines to
 *  handle them
 */

typedef struct screen_area_tbl_entry SCREEN_AREA_TBL_ENTRY;
struct screen_area_tbl_entry
{
	AREA area;			/* screen area key */
	int (*draw)();			/* func to draw area */
	int (*exec)();			/* func to execute */
	BOOLEAN is_active;		/* true if screen area is
					   displayed on screen */
	Gpoint window[2];			/* window[0] = (xmin,ymin)
					   window[1] = (xmax,ymax) */
	Gpoint viewport[2];		/* viewport[0] = (xmin,ymin)
					   viewport[1] = (xmax,ymax) */
};

extern int screen_tbl_sz;		/* screen tbl size */
extern SCREEN_AREA_TBL_ENTRY screen_tbl[];	/* screen tbl */
extern IDX find_screen_tbl_idx();
