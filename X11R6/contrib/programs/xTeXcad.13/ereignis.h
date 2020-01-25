
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


/* header-file ereignis.h */



#ifndef EREIGNISH
#define EREIGNISH





/* prototypes */




 void load_handler(Widget widget, XtPointer closure, XtPointer call_data);
 void trans_handler(Widget widget, XtPointer closure, XtPointer call_data);
 void about_handler(Widget widget, XtPointer closure, XtPointer call_data);
 void quit_handler(Widget cmd, caddr_t client_data, caddr_t call_data);
 void options(Widget w, caddr_t client_data, caddr_t call_data);
 void raster_on(void);
 void raster_off(void);
 void snap_on(Widget w, caddr_t client_data, caddr_t call_data);
 void general_purpose_handler(Widget w, caddr_t client_data, caddr_t call_data);
 void aus_raster(Widget w, XtPointer, XtPointer);
 void slopes(Widget w, caddr_t c, caddr_t call_data);
 void diams(Widget w, caddr_t c, caddr_t call_data);
 void answer(Widget w, XtPointer closure, XtPointer call_data);
 void get_number(Widget, XEvent *, String *, Cardinal *);
 void automatic_refresh(Widget w, caddr_t client_data, caddr_t call_data);
 void full_dina4(Widget w, caddr_t client_data, caddr_t call_data);
 void draw_coords(Widget, XtPointer, XEvent *, Boolean *);
 void ruler_on(void);
 void ruler_switch(Widget w, XtPointer, XtPointer);
 void cross_switch(Widget w, XtPointer, XtPointer);
 void l_length_switch(Widget w, caddr_t client_data, caddr_t call_data);


extern int h_cross;
extern int v_cross;



#endif





