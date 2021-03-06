/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1991 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
static char *chompv_rcsid = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/atk/champ/RCS/chompv.c,v 2.8 1991/09/12 16:03:29 bobg Exp $";

#include <andrewos.h>	/* time.h */
#include <class.h>
#include <chompv.eh>
#include <month.ih>
#include <monthv.ih>
#include <lpair.ih>
#include <textv.ih>
#include <im.ih>
#include <event.ih>
#include <scroll.ih>


boolean chompview__InitializeObject(c, self)
struct classheader *c;
struct chompview *self;
{
    int mon, yr;

    self->lp1 = lpair_New();
    self->toplpair = lpair_New();
    self->tv = textview_New();
    self->m1 = month_New();
    self->m2 = month_New();
    self->mv1 = monthview_New();
    self->mv2 = monthview_New();
    self->s = scroll_New();
    if (!self->lp1 || !self->lp1 || !self->tv || !self->m1 || !self->mv1 || !self->m2 || !self->mv2 || !self->s) return(FALSE);
    mon = month_GetMonth(self->m2);
    yr = month_GetYear(self->m2);
    if (++mon >= 12) {
	mon = 0;
	++yr;
    }
    month_SetMonthAndYear(self->m2, mon, yr);
    monthview_SetDataObject(self->mv1,self->m1);
    monthview_SetDataObject(self->mv2,self->m2);
    monthview_SetTextview(self->mv1, self->tv);
    monthview_SetTextview(self->mv2, self->tv);
    monthview_ResetMonth(self->mv2, FALSE);
    monthview_ResetMonth(self->mv1, FALSE); /* This goes second to "win" the textview */
    scroll_SetView(self->s, self->tv);
    lpair_VTFixed(self->lp1, self->mv1, self->mv2, 125, 1);
    lpair_HTFixed(self->toplpair, self->lp1, self->s, 175, 1);
    return(TRUE);
}

void chompview__FinalizeObject(c, self)
struct classheader *c;
struct chompview *self;
{
    lpair_Destroy(self->toplpair);
    lpair_Destroy(self->lp1);
    scroll_Destroy(self->s);
    textview_Destroy(self->tv);
    month_Destroy(self->m1);
    month_Destroy(self->m2);
    monthview_Destroy(self->mv1);
    monthview_Destroy(self->mv2);
}

enum view_DSattributes chompview__DesiredSize(self, width, height, pass, dWidth, dheight)
struct chompview *self;
long width;
long height;
enum view_DSpass pass;
long *dWidth;
long *dheight;
{
    *dWidth = 1000;
    *dheight = 250;
    return(view_WidthFlexible | view_HeightFlexible);
}

void chompview__FullUpdate(self, type, left, top, width, height)
struct chompview *self;
enum view_UpdateType type;
long left;
long top;
long width;
long height;
{
    struct rectangle Rect;

    if((type == view_LastPartialRedraw) || (type == view_FullRedraw)) {
	chompview_GetLogicalBounds(self, &Rect);
	chompview_DrawRectSize(self, Rect.left, Rect.top, Rect.width-1, Rect.height-1);
	Rect.left += 2;
	Rect.top += 2;
	Rect.width -= 4;
	Rect.height -= 4;
	lpair_InsertView(self->toplpair, self, &Rect);
	lpair_FullUpdate(self->toplpair, type, left, top, width, height);
    }
}

struct view *chompview__Hit(self, action, x, y, numberOfClicks)
struct chompview *self;
enum view_MouseAction action;
long x, y, numberOfClicks;
{
    return(lpair_Hit(self->toplpair, action, x, y, numberOfClicks));
}

void chompview__Print(self, f, process, final, toplevel)
struct chompview *self;
FILE *f;
char *process;
char *final;
int toplevel;
{
    textview_Print(self->tv, f, process, final, toplevel);
}
