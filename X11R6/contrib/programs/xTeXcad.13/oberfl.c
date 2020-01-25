
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


/* oberfl.c */


#include <stdio.h>
#include <math.h>
#include "extdef.h"
#include "x_stuff.h"
#include "oberfl.h"
#include "ereignis.h"
#include "file_sel.h"
#include "graphics.h"
#include "pics.h"





/* constant */

const Dimension cmd_btn_width = 60;
const Dimension cmd_btn_height = 60;

const           new_section = 22;

const           vertical_distance = 3;
/* workspace placement with xorigin and yorigin */

const           yorigin = 3;
#define xorigin (cmd_btn_width+2*new_section)

/* DIN A4 Page (in points) */
const           y_A4_max = 840;
const           y_A4_min = 0;
const           x_A4_max = 600;
const           x_A4_min = 0;
/* zoom-window is x div 10 and y div 10 */

/* global variables */

Widget          top, top2, toplevel, canvas, pboard, textup, wtext, input_up, box_me_up;
Widget          line_cmd, box_cmd, circle_cmd, text_cmd, file_cmd;
Widget          vector_cmd, zoom_cmd, refresh_cmd, copy_cmd, erase_cmd, edit_cmd,
                options_cmd;
Widget          snap1, raster1, radio_up, slp, dias;
Widget          txt2, txt3, raster_up, ask_up, sign_up, sign_msg;
Widget          coordinates, about, pop_adjust_up, adj_txt2,fileselector,bezier_cmd;
Widget		lft,rgt,up,dwn,zentr;

Arg             args[10];
int             n, m;
char            filename[100] = "";
char            chain[100] = "";
char            boxinput[256] = "";
char            textinput[256] = "";
char            titlename[200] = "";
char            txtpos[3] = "";
float           xur = 0.0, yur = 0.0;

Boolean         snap, raster, steigung, durchmesser, refr_auto, dina4;	/* radio-button-options */
Boolean         zoomed, ruler, cross;
char            graph_action, shadow = '-';	/* current action is stored
						 * here */

GC              gc, copy_gc;	/* Graphics Context Identifier */



/* user data */
char	aa='A';
char	bb='B';
char	cc='C';
char	dd='D';
char	ee='E';
char	ff='F';
char	gg='G';
char	hh='H';
char	ii='I';
char	jj='J';
char	kk='K';
char	ll='L';
char	mm='M';
char	nn='N';
char	oo='O';
char	pp='P';
char	qq='Q';
char	rr='R';
char	ss='S';
char	tt='T';
char	uu='U';
char	vv='V';
char	ww='W';
char	xx='X';
char	yy='Y';
char	zz='Z';
char point='.';
char DUMMY='|';
char minus='-';
char	ka='a';
char	kb='b';
char	kc='c';
char	kd='d';
char	ke='e';
char	kf='f';
char	kg='g';
char	kh='h';
char	ki='i';
char	kj='j';
char	klein_k='k';
char	kl='l';
char	km='m';
char	kn='n';
char	ko='o';
char	kp='p';
char	kq='q';
char	kr='r';
char	ks='s';
char	kt='t';
char	ku='u';
char	kv='v';
char	kw='w';
char	kx='x';
char	ky='y';
char	kz='z';

/* Figure Data Base */
struct fig1     oval;
struct fig2     filledBox, strich, pfeil, disc, kreis;
struct fig3     framedBox;
struct fig4     dashedBox;
struct fig5     message;
struct fig6	bezier;

struct fig2    *disc_start = NULL, *disc_curr = NULL, *disc_marker = NULL;
struct fig2    *kreis_start = NULL, *kreis_curr = NULL, *kreis_marker = NULL;
struct fig2    *filledBox_start = NULL, *filledBox_curr = NULL, *filledBox_marker = NULL;
struct fig1    *oval_start = NULL, *oval_curr = NULL, *oval_marker = NULL;
struct fig2    *strich_start = NULL, *strich_curr = NULL, *strich_marker = NULL;
struct fig2    *pfeil_start = NULL, *pfeil_curr = NULL, *pfeil_marker = NULL;
struct fig3    *framedBox_start = NULL, *framedBox_curr = NULL, *framedBox_marker = NULL;
struct fig4    *dashedBox_start = NULL, *dashedBox_curr = NULL, *dashedBox_marker = NULL;
struct fig5    *message_start = NULL, *message_curr = NULL, *message_marker = NULL;
struct fig6    *bezier_start = NULL, *bezier_curr = NULL, *bezier_marker = NULL; 		




int             x_zoom_max;
int             x_zoom_min;
int             y_zoom_max;
int             y_zoom_min;
Pixmap          about_map;
int		xmotion,ymotion;











void
main(int argc, char *argv[])
{
    	Display        *disp;
	Window          win;
	
	if (argc > 1)
		if (*argv[1] == '-')  
		if (*++argv[1] == 'h')
		{
			printf("\n");
			printf("This is xTeXcad, FreeWare, Version 1.3  for X11R6\n");
			printf("written by K.Zitzmann, University of Koblenz, FRG\n");
			printf("=================================================\n\n");
			exit(0);
		} else argv[1]--;
		
	strcpy(titlename, "xTeXcad 1.3");

	snap = False;
	raster = False;
	steigung = False;
	durchmesser = False;
	zoomed = False;
	dina4 = False;
	refr_auto = True;
	ruler = True;
	cross = False;

	x_zoom_min = -999;

	
	toplevel = XtInitialize("xtexcad", "Xtexcad", NULL,0, &argc, argv);

	/* connect the TeXcad - icon  to the toplevel-shell */
	n = 0;
	XtSetArg(args[n], XtNiconPixmap,
		 XCreateBitmapFromData(XtDisplay(toplevel),
				       XtScreen(toplevel)->root,
				       icon_bits, icon_width,
				       icon_height));
	
	n++;
	XtSetValues(toplevel, args, n);


	/* build the desktop */

	command_init();

	paintboard();

	XtRealizeWidget(toplevel);	/* realize all */

	

	headline(toplevel, titlename);	/* status line */

	/* additional settings */
	popup_init();	/* initialize all command-popups */
	textup_init();	/* initialize text - popup */
	radio_init();	/* initialize radio-button-popup */
	fileselector = init_file_select(canvas); /* connect file selector box */
	init_prompt_text();	/* initialize simple text pop-up */
	init_ask_erase();	/* initialize security-queries */
	graphics_init();	/* some initializations */


	/* startup-message */
	XtPopupSpringLoaded(textup);
	headline(textup, "Welcome");

	init_text_and_position();	/* box queries */
	init_text_adjustment();	/* text queries */
	init_no_objects_sign();
	ruler_on();

	/* was there a filename, specified at the command line ? */
	/* or maybe a ? */
	if ( (argc > 1) && (*argv[1]!='-') && (*argv[1]!='+'))
		load_it(argv[1]);



	XtMainLoop();	/* wait for events; this is the standard event loop */
}



void
headline(Widget w, char *txt)
{
	/* print status-string at the top of the toplevel-shell */
	Window          win;
	Display        *disp;
	if (!XtIsRealized(w))
		return;
	disp = XtDisplay(w);
	win = XtWindow(w);
	XStoreName(disp, win, txt);
}







void
command_init()
{
	/* initializes the command-buttons and connects the bitmaps */
	Pixmap          icon;
	Display        *disp;
	Window          win;
	int             hor = 3, vert = 3, vtab;
	XtTranslations  trans_table1,trans_table2,trans_table3;
	char		cmd_str1[50]="<EnterWindow> : set() \n";
	char		cmd_str2[100]="<KeyPress>Return   : notify() unset()\n";
  	char		cmd_str3[50]="<ButtonRelease> : notify() unset()\n";
	
  
  	trans_table1 = XtParseTranslationTable(cmd_str1);
	trans_table2 = XtParseTranslationTable(cmd_str2);
	trans_table3 = XtParseTranslationTable(cmd_str3);


	vtab = cmd_btn_height + 3 * vertical_distance - 2;


	/* --- canvas database, additional settings --- */
	n = 0;
	XtSetArg(args[n], XtNwidth, (XtArgVal) x_A4_max + 220);
	n++;
	XtSetArg(args[n], XtNheight, (XtArgVal) y_A4_max + 20);
	n++;
	canvas = XtCreateManagedWidget("workspace", compositeWidgetClass, toplevel, args, n);



	/* --- line command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "Line");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, line_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;

	line_cmd = XtCreateManagedWidget("line", commandWidgetClass, canvas, args, n);
	XtAddCallback(line_cmd, XtNcallback, general_purpose_handler, &ll);
	XtOverrideTranslations(line_cmd, trans_table1);
	XtOverrideTranslations(line_cmd, trans_table2);
	XtOverrideTranslations(line_cmd, trans_table3);
	


	vert += vtab;

	/* --- vector command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "Vector");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, vector_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;

	vector_cmd = XtCreateManagedWidget("vector", commandWidgetClass, canvas, args, n);
	XtAddCallback(vector_cmd, XtNcallback, general_purpose_handler, &vv);
	XtOverrideTranslations(vector_cmd, trans_table1);
	XtOverrideTranslations(vector_cmd, trans_table2);
	XtOverrideTranslations(vector_cmd, trans_table3);
	

	vert += vtab;


	/* --- box command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "Box");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;
	XtSetArg(args[n], XtNmenuName, (XtArgVal) "BOXMENU");
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, box_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;

	box_cmd = XtCreateManagedWidget("box", menuButtonWidgetClass, canvas, args, n);
	XtOverrideTranslations(box_cmd, trans_table1);
	XtOverrideTranslations(box_cmd, trans_table2);
	XtOverrideTranslations(box_cmd, trans_table3);
	/* ------ box_cmd: widget is a popup -------- */


	vert += vtab;


	/* --- circle command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "Circle");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;
	XtSetArg(args[n], XtNmenuName, (XtArgVal) "CIRCLEMENU");
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, circle_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;
	
	
	circle_cmd = XtCreateManagedWidget("circle", menuButtonWidgetClass, canvas, args, n);
	XtOverrideTranslations(circle_cmd, trans_table1);
	XtOverrideTranslations(circle_cmd, trans_table2);
	XtOverrideTranslations(circle_cmd, trans_table3);
	/* ------ circle_cmd: widget is a popup -------*/

	vert += vtab;

	/* --- bezier command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "Bezier");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, bezier_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;

	bezier_cmd = XtCreateManagedWidget("bezier", commandWidgetClass, canvas, args, n);
	XtAddCallback(bezier_cmd, XtNcallback, general_purpose_handler, &ii);
	XtOverrideTranslations(bezier_cmd, trans_table1);
	XtOverrideTranslations(bezier_cmd, trans_table2);
	XtOverrideTranslations(bezier_cmd, trans_table3);
	
	
	vert += vtab;

	/* --- text command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "Text");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, text_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;

	text_cmd = XtCreateManagedWidget("text", commandWidgetClass, canvas, args, n);
	XtAddCallback(text_cmd, XtNcallback, general_purpose_handler, &tt);
	XtOverrideTranslations(text_cmd, trans_table1);
	XtOverrideTranslations(text_cmd, trans_table2);
	XtOverrideTranslations(text_cmd, trans_table3);
	

	vert += vtab;


	/* --- zoom command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "Area\nZoom");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, zoom_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;

	zoom_cmd = XtCreateManagedWidget("zoom", commandWidgetClass, canvas, args, n);
	XtAddCallback(zoom_cmd, XtNcallback, general_purpose_handler, &zz);
	XtOverrideTranslations(zoom_cmd, trans_table1);
	XtOverrideTranslations(zoom_cmd, trans_table2);
	XtOverrideTranslations(zoom_cmd, trans_table3);
	

	vert += vtab;


	/* --- refresh command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "Refresh\nDisplay");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, refresh_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;

	refresh_cmd = XtCreateManagedWidget("refresh", commandWidgetClass, canvas, args, n);
	XtAddCallback(refresh_cmd, XtNcallback, refresh, &point);
	XtOverrideTranslations(refresh_cmd, trans_table1);
	XtOverrideTranslations(refresh_cmd, trans_table2);
	XtOverrideTranslations(refresh_cmd, trans_table3);
	

	vert += vtab;

	/* --- copy command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "Copy");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;

	/* connect bitmap */


	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, copy_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;


	copy_cmd = XtCreateManagedWidget("copy", commandWidgetClass, canvas, args, n);
	XtAddCallback(copy_cmd, XtNcallback, general_purpose_handler, &yy);
	XtOverrideTranslations(copy_cmd, trans_table1);
	XtOverrideTranslations(copy_cmd, trans_table2);
	XtOverrideTranslations(copy_cmd, trans_table3);
	

	vert += vtab;

	/* --- erase command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "Erase");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;
	XtSetArg(args[n], XtNmenuName, (XtArgVal) "ERASEMENU");
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, erase_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;
	
	erase_cmd = XtCreateManagedWidget("erase", menuButtonWidgetClass, canvas, args, n);
	XtOverrideTranslations(erase_cmd, trans_table1);
	XtOverrideTranslations(erase_cmd, trans_table2);
	XtOverrideTranslations(erase_cmd, trans_table3);
	/*------- erase_cmd: widget is a popup ---------*/


	vert += vtab;


	/* --- edit command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "PickEdit");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;
	XtSetArg(args[n], XtNmenuName, (XtArgVal) "EDITMENU");
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, edit_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;
	
	edit_cmd = XtCreateManagedWidget("edit", menuButtonWidgetClass, canvas, args, n);
	XtOverrideTranslations(edit_cmd, trans_table1);
	XtOverrideTranslations(edit_cmd, trans_table2);
	XtOverrideTranslations(edit_cmd, trans_table3);
	
	
	vert += vtab;


	/* --- options command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "Options");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;
	XtSetArg(args[n], XtNmenuName, (XtArgVal) "OPTIONSMENU");
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, setting_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;

	options_cmd = XtCreateManagedWidget("options", commandWidgetClass, canvas, args, n);
	XtOverrideTranslations(options_cmd, trans_table1);
	XtOverrideTranslations(options_cmd, trans_table2);
	XtOverrideTranslations(options_cmd, trans_table3);
	XtAddCallback(options_cmd, XtNcallback, options, NULL);


	/* buttons top right placement */

	vert = 3;
	hor = 600+xorigin+new_section;


	/* --- file command button db, additional settings --- */
	n = 0;
	/*
	 * XtSetArg(args[n],XtNwidth,(XtArgVal)cmd_btn_width);n++;
	 * XtSetArg(args[n],XtNheight,(XtArgVal)cmd_btn_height);n++;
	 */
	XtSetArg(args[n], XtNlabel, (XtArgVal) "File\nService");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;
	XtSetArg(args[n], XtNmenuName, (XtArgVal) "FILEMENU");
	n++;

	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, file_bits, 60, 60);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;
	
	file_cmd = XtCreateManagedWidget("file", menuButtonWidgetClass, canvas, args, n);
	XtOverrideTranslations(file_cmd, trans_table1);
	XtOverrideTranslations(file_cmd, trans_table2);
	XtOverrideTranslations(file_cmd, trans_table3);
	/*------- file_cmd:  widget is a popup -----------*/


	
	vert+=vtab;

	/* --- coordinate display area--- */
	n = 0;
	XtSetArg(args[n], XtNwidth, 90);
	n++;
	XtSetArg(args[n], XtNheight, (XtArgVal) 40);
	n++;
	XtSetArg(args[n], XtNlabel, " ");
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;

	coordinates = XtCreateManagedWidget("coord", labelWidgetClass, canvas, args, n);


	vert+=vtab-new_section;

	/* up */

	n = 0;
	XtSetArg(args[n], XtNborderWidth, 0);
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;
	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, up_bits, 35,20);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;
	up=XtCreateManagedWidget("up", commandWidgetClass, canvas, args, n);
	XtAddEventHandler(up, ButtonPress,False, event_move_base, &uu);


	/* right */

	n = 0;
	XtSetArg(args[n], XtNborderWidth, 0);
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor+46);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert);
	n++;
	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, right_bits, 20,35);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;
	rgt=XtCreateManagedWidget("right", commandWidgetClass, canvas, args, n);
	XtAddEventHandler(rgt, ButtonPress,False, event_move_base, &rr);


	/* left */

	n = 0;
	XtSetArg(args[n], XtNborderWidth, 0);
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert+27);
	n++;
	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, left_bits, 20,35);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;
	lft=XtCreateManagedWidget("left", commandWidgetClass, canvas, args, n);
	XtAddEventHandler(lft, ButtonPress,False, event_move_base, &ll);


	/* down */

	n = 0;
	XtSetArg(args[n], XtNborderWidth, 0);
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor+31);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert+42);
	n++;
	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, down_bits, 35,20);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;
	dwn=XtCreateManagedWidget("down", commandWidgetClass, canvas, args, n);
	XtAddEventHandler(dwn, ButtonPress,False, event_move_base, &dd);


	/* center */

	n = 0;
	XtSetArg(args[n], XtNborderWidth, 1);
	n++;
	XtSetArg(args[n], XtNinternalWidth, 2);
	n++;
	XtSetArg(args[n], XtNx, (XtArgVal) hor+31);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) vert+27);
	n++;
	/* connect bitmap */
	disp = XtDisplay(toplevel);
	win = RootWindow(disp, DefaultScreen(disp));
	icon = XCreateBitmapFromData(disp, win, zen_bits,8,8);
	XtSetArg(args[n], XtNbitmap, icon);
	n++;
	zentr=XtCreateManagedWidget("center", commandWidgetClass, canvas, args, n);
	XtAddEventHandler(zentr, ButtonPress,False, event_move_base, &zz);

}


/*-----------------------------------------------------------*/


void
popup_init()
{/* Creating the menu's (popup) for file,box,circle and erase */

	Widget          filemenu, load, save, quit, pdir, left;	/* popup-widgets for
								 * 'file service' */
	Widget          boxmenu, framed, dashed, filled;	/* popup-widgets for
								 * 'box' */
	Widget          circlemenu, ring, oval, ball;	/* popup-widgets for
							 * 'circle' */
	Widget          erasemenu, all_erase, obj_erase;	/* popup-widgets for
								 * 'erase' */
								 
	Widget		editmenu,other;
	
	Widget 		et1,et2,et3;
	
	Arg             arg[10];
	int             n;




	/* --- popup for button: file-service --- */

	
	filemenu = XtCreatePopupShell("FILEMENU", simpleMenuWidgetClass, canvas, NULL,0);

	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Load Object");
	n++;
	
	load = XtCreateManagedWidget("load", smeBSBObjectClass, filemenu, arg, n);
	XtAddCallback(load, XtNcallback, load_handler, NULL);

	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Store Object");
	n++;
	
	save = XtCreateManagedWidget("save", smeBSBObjectClass, filemenu, arg, n);
	XtAddCallback(save, XtNcallback, trans_handler, NULL);


	XtCreateManagedWidget("line", smeLineObjectClass, filemenu, NULL,0);


	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "About...");
	n++;
	
	left = XtCreateManagedWidget("about", smeBSBObjectClass, filemenu, arg, n);
	XtAddCallback(left, XtNcallback, about_handler, NULL);


	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Terminate");
	n++;
	
	quit = XtCreateManagedWidget("quit", smeBSBObjectClass, filemenu, arg, n);
	XtAddCallback(quit, XtNcallback, quit_handler, NULL);





	/*---popup for button: box---*/

	
	
	boxmenu = XtCreatePopupShell("BOXMENU", simpleMenuWidgetClass, canvas, NULL,0);

	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Framed");
	n++;
	
	framed = XtCreateManagedWidget("fbox", smeBSBObjectClass, boxmenu, arg, n);
	XtAddCallback(framed, XtNcallback, general_purpose_handler, &nn);

	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Dashed");
	n++;
	
	dashed = XtCreateManagedWidget("dashed", smeBSBObjectClass, boxmenu, arg, n);
	XtAddCallback(dashed, XtNcallback, general_purpose_handler, &dd);

	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Filled");
	n++;
	
	filled = XtCreateManagedWidget("filled", smeBSBObjectClass, boxmenu, arg, n);
	XtAddCallback(filled, XtNcallback, general_purpose_handler, &ff);



	/*---popup for button: circle---*/


	circlemenu = XtCreatePopupShell("CIRCLEMENU", simpleMenuWidgetClass, canvas, NULL,0);

	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Normal");
	n++;
	
	ring = XtCreateManagedWidget("rcircle", smeBSBObjectClass, circlemenu, arg, n);
	XtAddCallback(ring, XtNcallback, general_purpose_handler, &cc);

	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Oval");
	n++;
	
	oval = XtCreateManagedWidget("oval", smeBSBObjectClass, circlemenu, arg, n);
	XtAddCallback(oval, XtNcallback, general_purpose_handler, &oo);

	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Filled");
	n++;
	
	ball = XtCreateManagedWidget("ball", smeBSBObjectClass, circlemenu, arg, n);
	XtAddCallback(ball, XtNcallback, general_purpose_handler, &bb);


	/*---popup for button: erase---*/

	
	erasemenu = XtCreatePopupShell("ERASEMENU", simpleMenuWidgetClass, canvas, NULL,0);

	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Pick Object");
	n++;
	
	obj_erase = XtCreateManagedWidget("oerase", smeBSBObjectClass, erasemenu, arg, n);
	XtAddCallback(obj_erase, XtNcallback, general_purpose_handler, &gg);

	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Clear Entire Database");
	n++;
	
	all_erase = XtCreateManagedWidget("aerase", smeBSBObjectClass, erasemenu, arg, n);
	XtAddCallback(all_erase, XtNcallback, general_purpose_handler, &aa);

	n = 0;
	
	
	/* --- popup for button: edit --- */
	editmenu = XtCreatePopupShell("EDITMENU", simpleMenuWidgetClass, canvas, NULL,0);

	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "Pick Object");
	n++;
	
	other = XtCreateManagedWidget("other_objects", smeBSBObjectClass, editmenu, arg, n);
	XtAddCallback(other, XtNcallback, general_purpose_handler, &pp);
	
	/* donnerbalken */
	XtCreateManagedWidget("line", smeLineObjectClass, editmenu, NULL,0);


	n = 0;

	XtSetArg(arg[n], XtNlabel, (XtArgVal) "TEXT");
	n++;
	
	et1 = XtCreateManagedWidget("tedit", smeBSBObjectClass, editmenu, arg, n);
	XtAddCallback(et1, XtNcallback, textedit, NULL);
			
	n=0;
	XtSetArg(arg[n], XtNlabel, (XtArgVal) "FRAMED-Box Text");
	n++;
	
	et2=XtCreateManagedWidget("ftext", smeBSBObjectClass, editmenu, arg, n);
	XtAddCallback(et2, XtNcallback, fb_textedit, NULL);
	
	n=0;
	XtSetArg(arg[n], XtNlabel, (XtArgVal) "DASHED-Box Text");
	n++;
	
	et3=XtCreateManagedWidget("dtext", smeBSBObjectClass, editmenu, arg, n);
	XtAddCallback(et3, XtNcallback, db_textedit, NULL);
	
	


}


/*-----------------------------------------------------------*/


void
paintboard()
{/* create the drawing area */
	n = 0;
	XtSetArg(args[n], XtNx, (XtArgVal) xorigin);
	n++;
	XtSetArg(args[n], XtNy, (XtArgVal) yorigin);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) x_A4_max);
	n++;
	XtSetArg(args[n], XtNheight, (XtArgVal) y_A4_max);
	n++;

	pboard = XtCreateManagedWidget("", labelWidgetClass, canvas, args, n);

	XtAddEventHandler(pboard, ExposureMask, False, event_refresh, NULL);
	XtAddEventHandler(pboard, PointerMotionMask, True, draw_coords, NULL);

	/* widget 'pboard' is one of the global widgets */
	/* pboard contains all the graphics */

}


/*-----------------------------------------------------------*/


void
textup_init()
{/* create a text window */
	/* used as 'welcome'-widget and report-container (watch!) */
	XtTranslations  trans_table;
	trans_table = XtParseTranslationTable("<LeaveNotify>: XtMenuPopdown()");
	/* when the pointer is moved outside of the window, it's popped down */

	textup = XtCreatePopupShell("txt", shellWidgetClass, canvas, NULL,0);

	n = 0;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 500);
	n++;
	XtSetArg(args[n], XtNheight, (XtArgVal) 200);
	n++;
	XtSetArg(args[n], XtNborderWidth, (XtArgVal) 30);
	n++;
	XtSetArg(args[n], XtNeditType, (XtArgVal) XawtextAppend);
	n++;
	XtSetArg(args[n], XtNscrollHorizontal, (XtArgVal) XawtextScrollAlways);
	n++;
	XtSetArg(args[n], XtNscrollVertical, (XtArgVal) XawtextScrollAlways);
	n++;

	


	wtext = XtCreateManagedWidget("dialog", asciiTextWidgetClass, textup, args, n);

	XtAugmentTranslations(textup, trans_table);	/* add action to
							 * translation manager */

	print_msg(wtext, "\n");
	print_msg(wtext, "This is ");
	print_msg(wtext, titlename);
	print_msg(wtext, " / \n");
	print_msg(wtext, "(c)1991,92 K. Zitzmann,\nUniversity of Koblenz, Germany\nlast change: 8/92\n\n\n");
	
	
}
/*-----------------------------------------------------------*/


void
radio_init()
{
	/* creates the 'settings'-menu */

	Widget          dummy1;
	Widget          contents, lbl, read_number, manu, din, rul, cross_curs, length_line;
	XtTranslations  trans_table, trans_table1, trans_table2;
	Position        x_rel, y_rel, x, y;
	char            temp_str[200] = "<KeyPress>Return: get_number()\n Ctrl<Key>J      : dummy() \n\
			Ctrl<Key>O      : dummy() \n Ctrl<Key>M      : dummy() \n";

	static XtActionsRec actions[150] = {{"dummy", Dummy},
	{"get_number", get_number}};
	trans_table = XtParseTranslationTable(temp_str);

	/* when the pointer is moved outside of the window, it's popped down */
	trans_table1 = XtParseTranslationTable("<LeaveWindow>: XtMenuPopdown() \n");

	trans_table2 = XtParseTranslationTable("<Btn1Down>,<Btn1Up>: toggle() notify()");

	n = 0;
	XtSetArg(args[n], XtNheight, 230);
	n++;
	XtSetArg(args[n], XtNwidth, 330);
	n++;
	
	radio_up = XtCreatePopupShell("options", transientShellWidgetClass, canvas, args, n);

	n = 0;
	XtSetArg(args[n], XtNvSpace, 10);
	n++;
	XtSetArg(args[n], XtNhSpace, 15);
	n++;
	dummy1 = XtCreateManagedWidget("dumm1", boxWidgetClass, radio_up, args, n);



	n = 0;

	XtSetArg(args[n], XtNlabel, "enable raster");
	n++;
	XtSetArg(args[n], XtNheight, 30);
	n++;
	XtSetArg(args[n], XtNwidth, 140);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	XtSetArg(args[n], XtNstate, False);
	n++;
	
	raster1 = XtCreateManagedWidget("rasteron", toggleWidgetClass, dummy1, args, n);
	XtAddCallback(raster1, XtNcallback, aus_raster, NULL);


	raster_up = XtCreatePopupShell("Raster Definition", transientShellWidgetClass, canvas, NULL, 0);
	contents = XtCreateManagedWidget("hi", boxWidgetClass, raster_up, NULL, 0);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 70);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 250);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNlabel, "Please enter the width\nof the raster !\n(Default = 40pts)");
	n++;
	
	lbl = XtCreateManagedWidget("jj", labelWidgetClass, contents, args, n);

	n = 0;
	XtSetArg(args[n], XtNheight, (XtArgVal) 20);
	n++;
	XtSetArg(args[n], XtNwidth, (XtArgVal) 250);
	n++;
	XtSetArg(args[n], XtNautoFill, False);
	n++;
	XtSetArg(args[n], XtNuseStringInPlace, True);
	n++;
	XtSetArg(args[n], XtNstring, chain);
	n++;
	XtSetArg(args[n], XtNlength, XtNumber(chain));
	n++;
	XtSetArg(args[n], XtNeditType, XawtextEdit);
	n++;
	XtSetArg(args[n], XtNresize, XawtextResizeWidth);
	n++;
	XtSetArg(args[n], XtNwrap, XawtextWrapWord);
	n++;
	
	read_number = XtCreateManagedWidget("j", asciiTextWidgetClass, contents, args, n);


	n = 0;

	XtSetArg(args[n], XtNlabel, "snap pointer");
	n++;
	XtSetArg(args[n], XtNheight, 30);
	n++;
	XtSetArg(args[n], XtNwidth, 140);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	XtSetArg(args[n], XtNstate, False);
	n++;
	
	snap1 = XtCreateManagedWidget("snapon", toggleWidgetClass, dummy1, args, n);
	XtAddCallback(snap1, XtNcallback, snap_on, NULL);

	n = 0;

	XtSetArg(args[n], XtNlabel, "unlimited slopes");
	n++;
	XtSetArg(args[n], XtNheight, 30);
	n++;
	XtSetArg(args[n], XtNwidth, 140);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	XtSetArg(args[n], XtNstate, False);
	n++;
	
	slp = XtCreateManagedWidget("slope", toggleWidgetClass, dummy1, args, n);
	XtAddCallback(slp, XtNcallback, slopes, NULL);

	n = 0;

	XtSetArg(args[n], XtNlabel, "unlimited diameters");
	n++;
	XtSetArg(args[n], XtNheight, 30);
	n++;
	XtSetArg(args[n], XtNwidth, 140);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	XtSetArg(args[n], XtNstate, False);
	n++;
	
	dias = XtCreateManagedWidget("diameters", toggleWidgetClass, dummy1, args, n);
	XtAddCallback(dias, XtNcallback, diams, NULL);

	n = 0;

	XtSetArg(args[n], XtNlabel, "auto refresh");
	n++;
	XtSetArg(args[n], XtNheight, 30);
	n++;
	XtSetArg(args[n], XtNwidth, 140);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	XtSetArg(args[n], XtNstate, True);
	n++;
	
	manu = XtCreateManagedWidget("auto_refresh", toggleWidgetClass, dummy1, args, n);
	XtAddCallback(manu, XtNcallback, automatic_refresh, NULL);

	n = 0;

	XtSetArg(args[n], XtNlabel, "center DIN A4");
	n++;
	XtSetArg(args[n], XtNheight, 30);
	n++;
	XtSetArg(args[n], XtNwidth, 140);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	XtSetArg(args[n], XtNstate, False);
	n++;
	
	din = XtCreateManagedWidget("page", toggleWidgetClass, dummy1, args, n);
	XtAddCallback(din, XtNcallback, full_dina4, NULL);

	n = 0;

	XtSetArg(args[n], XtNlabel, "enable ruler");
	n++;
	XtSetArg(args[n], XtNheight, 30);
	n++;
	XtSetArg(args[n], XtNwidth, 140);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	XtSetArg(args[n], XtNstate, True);
	n++;
	
	rul = XtCreateManagedWidget("lineal", toggleWidgetClass, dummy1, args, n);
	XtAddCallback(rul, XtNcallback, ruler_switch, NULL);

	n = 0;

	XtSetArg(args[n], XtNlabel, "cross wire");
	n++;
	XtSetArg(args[n], XtNheight, 30);
	n++;
	XtSetArg(args[n], XtNwidth, 140);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	XtSetArg(args[n], XtNstate, False);
	n++;
	
	cross_curs = XtCreateManagedWidget("cross", toggleWidgetClass, dummy1, args, n);
	XtAddCallback(cross_curs, XtNcallback, cross_switch, NULL);

	n = 0;

	XtSetArg(args[n], XtNlabel, "watch line length");
	n++;
	XtSetArg(args[n], XtNheight, 30);
	n++;
	XtSetArg(args[n], XtNwidth, 140);
	n++;
	XtSetArg(args[n], XtNjustify, XtJustifyCenter);
	n++;
	XtSetArg(args[n], XtNshapeStyle, XmuShapeOval);
	n++;
	XtSetArg(args[n], XtNstate, False);
	n++;
	
	length_line = XtCreateManagedWidget("llline", toggleWidgetClass, dummy1, args, n);
	XtAddCallback(length_line, XtNcallback, l_length_switch, NULL);
	line_length = 0;

	/* add action to translation manager */
	XtAddActions(actions, XtNumber(actions));
	XtOverrideTranslations(radio_up, trans_table1);
	XtOverrideTranslations(raster1, trans_table2);
	XtOverrideTranslations(snap1, trans_table2);
	XtOverrideTranslations(slp, trans_table2);
	XtOverrideTranslations(dias, trans_table2);
	XtOverrideTranslations(read_number, trans_table);

}
/*-----------------------------------------------------------*/

void
print_msg(Widget text_widget, char *msg)
{
	/* prints messages into the welcome and report widget */

	XawTextPosition text_pos;
	int             result;
	XawTextBlock    text_block;


	/* get current position of Cursor */
	text_pos = XawTextGetInsertionPoint(text_widget);


	/* text_block init. */
	text_block.firstPos = 0;
	text_block.length = strlen(msg);
	text_block.ptr = msg;
	text_block.format = FMT8BIT;

	XawTextReplace(text_widget, text_pos, text_pos, &text_block);

	/* display text */
	XawTextDisplay(text_widget);
}
