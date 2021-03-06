static char *hwview_rcsid = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/atk/examples/atkbook/hwinset/RCS/hwview.c,v 1.3 1991/09/12 16:15:57 bobg Exp $";

/* **************************************************** *\
Copyright 1989 Nathaniel S. Borenstein
Permission to use, copy, modify, and distribute this software and its
documentation for any purpose and without fee is hereby granted,
provided that the above copyright notice appear in all copies and
that both that copyright notice and this permission notice appear in
supporting documentation, and that the name of 
Nathaniel S. Borenstein not be used in
advertising or publicity pertaining to distribution of the software
without specific, written prior permission. 

Nathaniel S. Borenstein DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
Nathaniel S. Borenstein BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY
DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
\* ***************************************************** */
#include <hwview.eh>
#include <graphic.ih>

void hwview__FullUpdate(self, type, left, top, width, height)
    struct hwview *self;
    enum view_UpdateType type;
    long left, top, width, height;
{
    struct rectangle Rect;

    hwview_GetVisualBounds(self, &Rect);
    hwview_MoveTo(self, Rect.left + Rect.width/2,
		   Rect.top + Rect.height/2);
    hwview_DrawString(self, "Hello, world!",
	graphic_BETWEENTOPANDBASELINE
	| graphic_BETWEENLEFTANDRIGHT);
}
