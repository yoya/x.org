
/* 
     Copyright (c) 1991 xtexcad  V1.3 by K. Zitzmann

     The X Consortium, and any party obtaining a copy of these files from
     the X Consortium, directly or indirectly, is granted, free of charge, a
     full and unrestricted irrevocable, world-wide, paid up, royalty-free,
     nonexclusive right and license to deal in this software and
     documentation files (the "Software"), including without limitation the
     rights to use, copy, modify, merge, publish, distribute, sublicense,
     and/or sell copies of the Software, and to permit persons who receive
     copies from any such party to do so.  This license includes without
     limitation a license to do the foregoing actions under any patents of
     the party supplying this software to the X Consortium.
*/


/* ereignis.c */

#include "x_stuff.h"
#include "ereignis.h"
#include "file_sel.h"
#include "oberfl.h"
#include "graphics.h"
#include "io_trans.h"
#include <math.h>
#include "pics.h"
#include "pickedit.h"

/* used by the cross-hair-pointer */
int             h_cross = -999;
int             v_cross = -999;




void
about_handler(Widget widget, XtPointer closure, XtPointer call_data)
{
	/* displays additional information about this program, like */
	/* the copyright,author etc. */

	Display        *disp;
	Window          win;
	Widget          child, child2, box;
	XtTranslations  trans_table;
	char            destination[80] = "<LeaveNotify>: XtMenuPopdown()";
	Position        x, y, x_rel, y_rel;
	char            ss[200];


	
	strcpy(ss,titlename);
	strcat(ss," -  FreeWare-Utility For The Development\nOf Portable LaTeX-Graphics.\n\n");
	strcat(ss,"    Release 20.08.1992\n");

	
	about = XtCreatePopupShell("About...", transientShellWidgetClass, canvas, NULL,0);


	n = 0;
	XtSetArg(args[n], XtNborderWidth, 0);
	n++;
	box = XtCreateManagedWidget("abt1", boxWidgetClass, about, args, n);

	n = 0;
	disp = XtDisplay(toplevel);
	win = XtWindow(toplevel);
	about_map = XCreateBitmapFromData(disp, win, texcad_bits, texcad_width, texcad_height);
	XtSetArg(args[n], XtNborderWidth, 0);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNbitmap, about_map);
	n++;
	XtSetArg(args[n], XtNwidth, 2 * texcad_width);
	n++;
	
	child = XtCreateManagedWidget("abt2", labelWidgetClass, box, args, n);

	n = 0;
	XtSetArg(args[n], XtNheight, texcad_height);
	n++;
	XtSetArg(args[n], XtNborderWidth, 0);
	n++;
	XtSetArg(args[n], XtNwidth, 2 * texcad_width);
	n++;
	
	child2 = XtCreateManagedWidget(ss, labelWidgetClass, box, args, n);



	/* Override Translation Manager */

	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(about, trans_table);


	/* setting the widget-position */
	n = 0;
	XtSetArg(args[n], XtNx, &x_rel);
	n++;
	XtSetArg(args[n], XtNy, &y_rel);
	n++;
	XtGetValues(canvas, args, n);	/* get the relative coordinates of
					 * widget: canvas */
	XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);
	n = 0;
	XtSetArg(args[n], XtNx, x + cmd_btn_width + 3 * new_section);
	n++;
	XtSetArg(args[n], XtNy, y + 2 * cmd_btn_height);
	n++;
	XtSetValues(about, args, n);




	XtPopup(about, XtGrabExclusive);



}




void
draw_coords(Widget cmd, XtPointer hi, XEvent *event,
	    Boolean *continue_to_dispatch)
{
	/* displays the absolute and relative coordinates of the pointer. */
	/* Furthermore it handles the cross-wire */

	Display        *disp = XtDisplay(coordinates);
	Window          win = XtWindow(coordinates);
	Display        *disp2 = XtDisplay(pboard);
	Window          win2 = XtWindow(pboard);
	int             x, y, h, v;
	float           a, b;
	unsigned int    mask;
	char            s1[13];
	char            s2[13];





	snapPointerPosition(&x, &y, &mask);

	if ((h_cross != x) || (v_cross != y))
	{	/* redraw */
		if (zoomed == True)
		{
			a = x;
			b = y;
			zoomed2real(&a, &b);
			h = (int) a;
			v = (int) b;
			if (h < 0)
				h = 0;
			if (v < 0)
				v = 0;
			if (x < 1)
				sprintf(s1, "X=000,%3.3i", h);
			else
				sprintf(s1, "X=%3.3i,%3.3i", x, h);
			if (y < 1)
				sprintf(s2, "Y=000,%3.3i", v);
			else
				sprintf(s2, "Y=%3.3i,%3.3i", y, v);
		} else
		{
			if (x < 1)
				strcpy(s1, "X=---,000");
			else
				sprintf(s1, "X=---,%3.3i", x);
			if (y < 1)
				strcpy(s2, "Y=---,000");
			else
				sprintf(s2, "Y=---,%3.3i", y);
		}

		XClearArea(disp, win, 15, 11, 70, 30, False);
		
/* JM */
		
		if (black_white_patch)
			XSetFunction(disp, gc, GXand);
		else
			XSetFunction(disp, gc, GXor);

		XDrawString(disp, win, gc, 3, 12, "  rel abs", 9);
		XDrawString(disp, win, gc, 3, 24, s1, 9);
		XDrawString(disp, win, gc, 3, 36, s2, 9);

/* JM */	if (black_white_patch)
			XSetFunction(disp, gc, GXequiv);
		else
			XSetFunction(disp, gc, GXxor);

		/* cross */

		if (cross == True)
		{
			XDrawLine(disp2, win2, gc, x, 0, x, y_A4_max);
			XDrawLine(disp2, win2, gc, 0, y, x_A4_max, y);


			/* delete old */
			if (h_cross != -999)
			{
				XDrawLine(disp2, win2, gc, h_cross, 0, h_cross, y_A4_max);
				XDrawLine(disp2, win2, gc, 0, v_cross, x_A4_max, v_cross);
			}
		}	/* cross==True */
	}	/* redraw */
	h_cross = x;
	v_cross = y;


}


void
ruler_on()
{
	/* enable ruler */

	char            s[3];
	int             x, y, step = 4;
	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	Boolean         first = True;
	int             adjust;
	float           hor = 1, ver = 1;
	int             zahl;


	for (x = step; x < x_A4_max; x += step)
	{
		if ((x % 16) == 0)
			XDrawLine(disp, win, gc, x, y_A4_max, x, y_A4_max - 4);
		else if ((x % 40) == 0)
		{
			XDrawLine(disp, win, gc, x, y_A4_max, x, y_A4_max - 7);
			if (first == True)
				first = False;	/* don't draw the first one */
			else
			{
				if (zoomed == True)
				{
					hor = (float) x;
					zoomed2real(&hor, &ver);
					zahl = (int) (hor + 0.5);
				} else
					zahl = x;
				sprintf(s, "%i", (int) (zahl));
				if (strlen(s) == 2)
					adjust = 10;
				else
					adjust = 13;
				XDrawString(disp, win, gc, x - adjust, y_A4_max - 10, s, strlen(s));
			}
		} else
			XDrawLine(disp, win, gc, x, y_A4_max, x, y_A4_max - 2);
	}

	for (y = y_A4_max - step; y > 0; y -= step)
	{
		if ((y % 16) == 0)
			XDrawLine(disp, win, gc, 0, y, 4, y);
		else if ((y % 40) == 0)
		{
			XDrawLine(disp, win, gc, 0, y, 7, y);
			if (zoomed == True)
			{
				ver = (float) y;
				zoomed2real(&hor, &ver);
				zahl = (int) (ver + 0.5);
			} else
				zahl = y;
			sprintf(s, "%i", (int) (zahl));
			XDrawString(disp, win, gc, 10, y + 4, s, strlen(s));
		} else
			XDrawLine(disp, win, gc, 0, y, 2, y);
	}
}



void
ruler_switch(Widget w, XtPointer closure, XtPointer call_data)
{
	/* called by the settings menu (callback) */
	/* toggles the use of the ruler */
	
	if (ruler == True)
		ruler = False;
	else
		ruler = True;

	ruler_on();
}


void
cross_switch(Widget w, XtPointer closure, XtPointer call_data)
{
	/* toggles the use of the cross-hair */

	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	unsigned int    mask;
	
	if (cross == True)
		cross = False;	/* redraw cross-cursor */


	else
	{
		cross = True;
		/* draw it */
		snapPointerPosition(&h_cross, &v_cross, &mask);
	}

	XDrawLine(disp, win, gc, h_cross, 0, h_cross, y_A4_max);
	XDrawLine(disp, win, gc, 0, v_cross, x_A4_max, v_cross);


}


void
automatic_refresh(Widget w, caddr_t client_data, caddr_t call_data)
{
	if (refr_auto == True)
		refr_auto = False;
	else
		refr_auto = True;
}


void
full_dina4(Widget w, caddr_t client_data, caddr_t call_data)
{
	if (dina4 == True)
		dina4 = False;
	else
		dina4 = True;
}


void
l_length_switch(Widget w, caddr_t client_data, caddr_t call_data)
{
	if (line_length == 0)
		line_length = 10;
	else
		line_length = 0;
}

void
aus_raster(Widget w, XtPointer closure, XtPointer call_data)
{
	/* toggles the use of a raster */
	/* realizes raster height/width input */

	XtAppContext    app_context;	/* Applikationcontext */
	XEvent          event;
	Position        x, y, x_rel, y_rel;
	int             h, v;
	unsigned int    mask;
	


	if (raster == False)
	{
		XtPopdown(radio_up);
		PointerPosition(&h, &v, &mask);
		/* setting position of widget */
		n = 0;
		XtSetArg(args[n], XtNx, &x_rel);
		n++;
		XtSetArg(args[n], XtNy, &y_rel);
		n++;
		XtGetValues(canvas, args, n);
		/* get the relative coordinates of widget: canvas */
		XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);
		n = 0;
		XtSetArg(args[n], XtNx, x + cmd_btn_width + 2 * new_section);
		n++;
		XtSetArg(args[n], XtNy, y + 4 * cmd_btn_height);
		n++;
		XtSetValues(raster_up, args, n);
		/* position set ! */

		/* query raster-height */
		XtPopup(raster_up, XtGrabExclusive);

		app_context = XtWidgetToApplicationContext(raster_up);

		visible = True;

		while (visible)
		{
			XtAppNextEvent(app_context, &event);
			XtDispatchEvent(&event);

		}

	} else
		raster = False;

}

void
raster_on()
{
	/* display raster */

	Display        *disp = XtDisplay(pboard);
	Window          win = XtWindow(pboard);
	int             x, y, x_disp, y_disp, zoom=(zoomed==True) ? 10 : 1;
	float           h = 1, v = 1;	/* dummy init */
	int 		cross_len=3;
	int		cross_space;
	char		d_list[2];


	/* enable raster display on pboard */

	y = rast_height; 
	x = rast_height;

	if (zoomed==True)
	{
		d_list[0] = (char)cross_len;
		d_list[1] = (char)10;
		XSetLineAttributes(disp, copy_gc, 0, LineOnOffDash, CapButt, JoinMiter);
	}
	else
	{
		cross_space=rast_height-cross_len; 
	
		d_list[0]=(char)cross_len;
		d_list[1]=(char)cross_space;
	}


		XSetDashes(disp,copy_gc,0,d_list,2);



	for (; (x <= 600 - rast_height); x += rast_height)
	{
		if (zoomed == True)
		{
			h = (float) x;
			real2zoomed(&h, &v);
			x_disp = (int) h;
		} else
			x_disp = x;

		XDrawLine(disp, win, copy_gc, x_disp, rast_height-(int)(cross_len/2), x_disp, 840 - (int)(cross_len/2));
	}



	for (; (y <= 840 - rast_height); y += rast_height)
	{
		if (zoomed == True)
		{
			v = (float) y;
			real2zoomed(&h, &v);
			y_disp = (int) v;
		} else
			y_disp = y;

		XDrawLine(disp, win, copy_gc, rast_height-(int)(cross_len/2), y_disp, 600 - (int)(cross_len/2), y_disp);

	}


}


void
raster_off()
{

	/* disable raster display on pboard */

	raster_on();



}


void
get_number(Widget widget, XEvent *event, String *params, Cardinal *num_params)
{
	/* raster-stuff */
	int             j=0,i=0;
	char		zahl[10],un[2];
	Boolean		unknown=False;
	float 		num=0.0;
	char		out[300];

	raster = True;
	ulen=1.0; /* standard unitlength */
	unit=1; /* standard: points */

	/* set new raster-height/width or default value */

	/* scan the input ! */
	zahl[0]='\0';

	while( ( (isdigit(chain[i])) || (chain[i]=='.') ) && (i<=strlen(chain)) )
	{
		zahl[i]=chain[i];
		i++;
		zahl[i]='\0';
	}

	/* the number is in zahl */

	num=(zahl[0]!='\0') ? atof(zahl) : 0.0;

	/* get the unit - take 'pt' if none is specified */

	while(i<=strlen(chain) && j<=2)
	{
		if (isalpha(chain[i])) un[j++]=chain[i];
		i++;
	}
	
	if ((toupper(un[0]))=='I') unit=2; /* Inch */
	else
	if ((toupper(un[0]))=='C') unit=3; /* Cm */
	else
	if ((toupper(un[0]))=='M') unit=4; /* Mm */
	else
	if ((toupper(un[0]))=='P') /* Pica or Point */
	{
		if ((toupper(un[1]))=='T') unit=1; /* point */
		else 
		if ((toupper(un[1]))=='C') unit=5; /* pica */
		else unknown=True;
	} else unknown=True;
	
	if (unknown==True) /* unit not recognized */
		unit=1; /* default-unit = points */
	
	if (num!=0.0) rast_height=(int)compute_length(num);
		else rast_height=40; /* standard raster */

	if (rast_height<4)
	{
		num=-1.0;
		rast_height=40;
	}
	
	visible = False;

	XtPopdown(raster_up);

	out[0]='\0';

	if ( (num==0.0) && (unknown==True) )
		strcat(out,"ERROR !!!\n - Unit not recognized\n - No value specified\n\nInvoking standard raster...\n");
	else if (unknown==True) strcat(out,"ERROR !!!\n - Unit not recognized\n\nTaking POINTS now !\n");
	else if (num==0.0) strcat(out,"ERROR !!!\n - No valid value !\nInvoking standard raster...\n");
	


	if (num==-1.0) strcat(out,"Sorry - raster too small !\nInvoking standard raster...\n");


	if (out[0]!='\0') leave_pick(out);


	

}



void
snap_on(Widget w, caddr_t client_data, caddr_t call_data)
{
	/* enable snap function for LaTeX-Graph-File */

	if (snap == False)
		snap = True;
	else
		snap = False;

}



void
slopes(Widget w, caddr_t client_data, caddr_t call_data)
{
	/* toggles the use of finit/infinite slopes */

	if (steigung == False)
	{
		steigung = True;
		/* all slopes allowed */
		line_slope = 0;
		arrow_slope = 0;
	} else
	{
		steigung = False;
		line_slope = slope_line;
		arrow_slope = slope_arrow;
	}

}

void
diams(Widget w, caddr_t client_data, caddr_t call_data)
{
	/* toggles the use of unlimited diameters */
	if (durchmesser == False)
	{
		durchmesser = True;
		circle_diameter[0] = -1;
		disc_diameter[0] = -1;
	} else
	{
		durchmesser = False;
		circle_diameter[0] = 1;
		disc_diameter[0] = 1;
	}
}


void
load_handler(Widget widget, XtPointer closure, XtPointer call_data)
{
	/* read a LaTeX-file from the medium */

	char            title[] = "Load object";
	char            pattern[] = "*.tex";
	char           *name;
	Position        x, y, x_rel, y_rel;


	n = 0;
	XtSetArg(args[n], XtNx, &x_rel);
	n++;
	XtSetArg(args[n], XtNy, &y_rel);
	n++;
	XtGetValues(canvas, args, n);	/* get the relative coordinates of
					 * widget: canvas */

	XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);
        
	centerpopup(canvas,fileselector);
	name =
		file_select(pattern, title);

	if (name != NULL)
	{
		perform_erase();	/* clear data-base */
		load_it(name);
	}
}




void
trans_handler(Widget widget, XtPointer closure, XtPointer call_data)
{
	/* translate the objects to pure LaTeX-code */

	char            title[] = "Save object";
	char            pattern[] = "*.tex";
	char           *name;
	Position        x, y, x_rel, y_rel;
	if (
	    disc_start == NULL &&
	    kreis_start == NULL &&
	    filledBox_start == NULL &&
	    oval_start == NULL &&
	    strich_start == NULL &&
	    pfeil_start == NULL &&
	    framedBox_start == NULL &&
	    dashedBox_start == NULL &&
	    bezier_start == NULL &&
	    message_start == NULL)
	{
		leave_pick("SAVE:\n\nThere are no objects !");
		return;
	}
	n = 0;
	XtSetArg(args[n], XtNx, &x_rel);
	n++;
	XtSetArg(args[n], XtNy, &y_rel);
	n++;
	XtGetValues(canvas, args, n);	/* get the relative coordinates of
					 * widget: canvas */

	XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);

	centerpopup(canvas,fileselector);
	name =
		file_select(pattern, title);

	if (name != NULL)
		trans_it(name);

}

void
general_purpose_handler(Widget cmd, caddr_t id, caddr_t call_data)
{
	/* distributes all other incoming events */

	int             i;
	char            txt[100];
	XtTranslations  trans_table;
	char            destination[80] = "<EnterWindow> : set_TM_btn1_first() \n\
		       <LeaveWindow> : left()";
	static XtActionsRec actions[80] = {{"set_TM_btn1_first", set_TM_btn1_first},
	{"left", left}};


	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(destination);
	XtOverrideTranslations(pboard, trans_table);

	/* set the new current action code */
	graph_action = *(char *)id;

	strcpy(txt, titlename);
	strcat(txt, "  :  ");

	switch (graph_action)
	{
	case 'L':
		strcat(txt, "#### Line ####");
		headline(toplevel, txt);
		break;

	case 'V':
		strcat(txt, "#### Vector ####");
		headline(toplevel, txt);
		break;

	case 'Z':
		strcat(txt, "#### Area Zoom ####");
		headline(toplevel, txt);
		break;

	case 'T':
		strcat(txt, "#### Text ####");
		headline(toplevel, txt);
		break;

	case 'N':
		strcat(txt, "#### Box --> framed ####");
		headline(toplevel, txt);
		break;

	case 'D':
		strcat(txt, "#### Box --> dashed ####");
		headline(toplevel, txt);
		break;

	case 'F':
		strcat(txt, "#### Box --> filled ####");
		headline(toplevel, txt);
		break;

	case 'C':
		strcat(txt, "#### Circle --> normal ####");
		headline(toplevel, txt);
		break;

	case 'O':
		strcat(txt, "#### Circle --> oval ####");
		headline(toplevel, txt);
		break;

	case 'B':
		strcat(txt, "#### Circle --> filled ####");
		headline(toplevel, txt);
		break;

	case 'A':
		strcat(txt, "#### Erase --> erasing the entire database ####");
		headline(toplevel, txt);
		query_ask_erase();
		if (txtpos[0] == 'j')
			perform_erase();
		XtUninstallTranslations(pboard);
		left(0, 0, 0, 0);
		break;

	case 'G':
		if ( (i=sorry_msg())==-1 ) break;
		strcat(txt, "#### Erase --> pick the desired object ####");
		headline(toplevel, txt);
		break;

	case 'P':
		if ( (i=sorry_msg())==-1 ) break;
		strcat(txt, "#### Pick object ####");
		headline(toplevel, txt);
		break;

	case 'Y':
		if ( (i=sorry_msg())==-1 ) break;
		strcat(txt, "#### Pick object to copy ####");
		headline(toplevel, txt);
		break;

	case 'I':
		strcat(txt, "#### Bezier ####");
		headline(toplevel, txt);
		break;

	default:
		break;
	}	/* switch */
}





void
quit_handler(Widget cmd, caddr_t client_data, caddr_t call_data)
{
	XFreePixmap(XtDisplay(pboard), about_map);
	perform_erase();	/* clear data-base */
	exit(0);
}


void
options(Widget w, caddr_t client_data, caddr_t call_data)
{
	/* puts the settings-menu to the right position */

	Position        x, y, x_rel, y_rel;
	n = 0;
	XtSetArg(args[n], XtNx, &x_rel);
	n++;
	XtSetArg(args[n], XtNy, &y_rel);
	n++;

	/* get the relative coordinates of widget: canvas */
	XtGetValues(canvas, args, n);

	XtTranslateCoords(canvas, x_rel, y_rel, &x, &y);

	n = 0;
	XtSetArg(args[n], XtNx, x + cmd_btn_width + 2 * new_section);
	n++;
	XtSetArg(args[n], XtNy, y + 9 * cmd_btn_height);
	n++;
	XtSetValues(radio_up, args, n);

	XtPopupSpringLoaded(radio_up);
}


void
answer(Widget w, XtPointer closure, XtPointer call_data)
{
	char c = *(char *)closure;
	txtpos[0] = c;
	XtPopdown(ask_up);
	visible = False;

}
