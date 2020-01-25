/*    Copyright 1989-94 Daniel Dardailler.
Permission to use, copy, modify, distribute, and sell this software
and its documentation for any purpose is hereby granted without fee,
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in
supporting documentation, and that the name of Daniel Dardailler not
be used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  Daniel
Dardailler makes no representations about the suitability of this
software for any purpose.  It is provided "as is" without express or
implied warranty.  
*/
/* ed_write.c */
#include <stdio.h>
#include "couche.h"		/* myEvent */
#include "clientimage.h"		/* ClientImage pour fedor.h */
#include "fedor.h"

extern fedchar  chartab[MAXFONTCARD];
extern int editresol ;
extern BackColor, CurColor ;

Autom_write(pev)
	myEvent        *pev;
{
	static int      x, y;
	int             hcar, code;
	register int    i, j;
	register fedchar *pcar;
	int oldresol ;
	int  color ;

	switch (pev->type) {
	case EnterZone:
		Afficher_boutons("ORIGIN", "ORIGIN");
		Invpoint(x, y);
		break;
	case ButtonPressed:
		Invpoint(x, y);
		x = convert(pev->x);
		y = convert(pev->y);
		Invpoint(x, y);
		break;
	case KeyPressed:
		code = pev->key;
		if ((code >= 0) && (code < MAXFONTCARD)) {
		        Invpoint(x, y);
			Dodo();
			pcar = &chartab[code];
			x += pcar->hadj;
			hcar = pcar->down - pcar->up;
			for (i = 0; i < pcar->hsize; i++)
			    for (j = 0; j < hcar; j++)
				if (Rast_Inq(pcar->image, i, j) != 0 ) 
				  /* pcar monochrome */
					Adjpoint(x + i, y + pcar->up + j);
				      
			x += pcar->hincr;
			oldresol = editresol ;
			Changer_editresol() ;
			if (oldresol != editresol) MontrerCarTrav() ; 
			else {
				CarFen();
				Aff_all();
			}
			Invpoint(x, y);
		}
		break;
	case LeaveZone:
		Afficher_numview(-1, -1);	/* restaure le gris */
		Afficher_boutons("", "");
		Invpoint(x, y);
		break;
	}
}
