
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


/* header-file oberfl.h */


#ifndef OBERFLH
#define OBERFLH



/* prototypes */


void popup_init(void);
void paintboard(void);
void textup_init(void);
void print_msg(Widget w, char *msg); /* display message in textwidget */
void radio_init(void);
void command_init(void);
void headline(Widget w, char *txt);



/* constant */

 extern const Dimension cmd_btn_width;
 extern const Dimension cmd_btn_height;
 extern const vertical_distance,new_section;

 extern const yorigin;

 #define xorigin (cmd_btn_width+2*new_section)
 
 extern const y_A4_max;
 extern const y_A4_min;
 extern const x_A4_max;
 extern const x_A4_min;

 
/* global variables */ 

 extern Widget toplevel,canvas,pboard,textup,wtext,input_up,get_up,box_me_up;
 extern Widget line_cmd,box_cmd,circle_cmd,text_cmd,file_cmd;
 extern Widget vector_cmd,zoom_cmd,refresh_cmd,erase_cmd,edit_cmd,options_cmd;
 extern Widget snap1,raster1,radio_up,slp,dias;
 extern Widget txt2,txt3,raster_up,ask_up,sign_up,sign_msg;
 extern Widget coordinates,about,pop_adjust_up,adj_txt2,fileselector,bezier_cmd;
 extern Widget lft,rgt,up,dwn,zentr;

 extern Arg args[10]; 
 extern int n,m;

 extern Boolean snap, raster; /* radio-button-options */
 extern Boolean zoomed,steigung,durchmesser,refr_auto,dina4;
 extern Boolean ruler,cross;

 extern char graph_action,shadow; /* current action : 'L'=line etc. */

 extern char filename[100];
 extern char chain[100];
 extern char titlename[200];
 extern char boxinput[256];
 extern char textinput[256];
 
 
 extern char txtpos[3];
 extern float xur,yur;
 extern GC gc,copy_gc; /* Graphic Context ID */

 extern Pixmap about_map;
 
 
 
 /* Figure Data Base */
 
 extern struct fig1 {
	float x,y,h,v;
	struct fig1 *next;
	} oval;

 extern struct fig2 {
	float x,y,h,v;
	struct fig2 *next;
	int radius;
	} filledBox,strich,pfeil,disc,kreis;

 extern struct fig3 {
	float x,y,h,v;
	char *text;
	char textpos[2];
	struct fig3 *next;
	} framedBox;

 extern struct fig4 {
	float x,y,h,v;
	char *text;
	char textpos[2];
	float dashlength;
	struct fig4 *next;
	} dashedBox;

 extern struct fig5 {
	float x,y;
	char *text;
	char textpos[2]; /* for the makebox-command */
	struct fig5 *next;
	} message;

 extern struct fig6 {
 	float ax,ay; /* startpoint */
 	float ex,ey; /* endpoint */
 	float sx,sy; /* controlpoint */
	struct fig6 *next;
 	} bezier;

 /* box,oval: first coordinates must be the upper left corner */
 /* circle: x,y determine the middlepoint, h,v are useless after creating object */

 extern struct fig2 *disc_start, *disc_curr, *disc_marker;
 extern struct fig2 *kreis_start, *kreis_curr, *kreis_marker;
 extern struct fig2 *filledBox_start, *filledBox_curr, *filledBox_marker;
 extern struct fig1 *oval_start, *oval_curr, *oval_marker;
 extern struct fig2 *strich_start, *strich_curr, *strich_marker;
 extern struct fig2 *pfeil_start, *pfeil_curr, *pfeil_marker;
 extern struct fig3 *framedBox_start, *framedBox_curr, *framedBox_marker;
 extern struct fig4 *dashedBox_start, *dashedBox_curr, *dashedBox_marker;
 extern struct fig5 *message_start, *message_curr, *message_marker;
 extern struct fig6 *bezier_start, *bezier_curr, *bezier_marker;


 extern int x_zoom_max;
 extern int x_zoom_min;
 extern int y_zoom_max;
 extern int y_zoom_min;

 extern int xmotion;
 extern int ymotion;

 /* user data */
extern char	aa;
extern char	bb;
extern char	cc;
extern char	dd;
extern char	ee;
extern char	ff;
extern char	gg;
extern char	hh;
extern char	ii;
extern char	jj;
extern char	kk;
extern char	ll;
extern char	mm;
extern char	nn;
extern char	oo;
extern char	pp;
extern char	qq;
extern char	rr;
extern char	ss;
extern char	tt;
extern char	uu;
extern char	vv;
extern char	ww;
extern char	xx;
extern char	yy;
extern char	zz;
extern char     point;
extern char     DUMMY;
extern char	minus;
extern char	ka;
extern char	kb;
extern char	kc;
extern char	kd;
extern char	ke;
extern char	kf;
extern char	kg;
extern char	kh;
extern char	ki;
extern char	kj;
extern char	klein_k;
extern char	kl;
extern char	km;
extern char	kn;
extern char	ko;
extern char	kp;
extern char	kq;
extern char	kr;
extern char	ks;
extern char	kt;
extern char	ku;
extern char	kv;
extern char	kw;
extern char	kx;
extern char	ky;
extern char	kz;

#endif
