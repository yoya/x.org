
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


/* header-file graphics.h */

#ifndef GRAPHICSH
#define GRAPHICSH


/* patch for Data General by JM , all patches by JM are marked with JM ! */
extern int black_white_patch;


/* prototypes */

void shorten(int *x, int *y);
void make_GCcontext(void);
void PointerPosition(int *x, int *y, unsigned int *mask);
void set_TM_btn1_first(Widget w, XEvent *event, String *id, Cardinal *num); 
void btn1_press_first(Widget w, XEvent *event, String *id, Cardinal *num);
void set_TM_btn1_second(void);
void btn1_press_second(Widget w, XEvent *event, String *id, Cardinal *num);
void set_first(void);
void enable_tracker(void);
void disable_tracker(void);
void action_track_it(Widget, XEvent *, String *, Cardinal *);
void track_it(int *xo, int *yo);
void leaving(Widget, XEvent *, String *, Cardinal *);
void zoomed2real(float *x, float *y);
void real2zoomed(float *x, float *y);
int ggt(int a, int b);
int valid_line_coords(int x_or, int y_or, int *x_ret, int *y_ret);
int valid_vector_coords(int x_or, int y_or, int *x_ret, int *y_ret);
void init_l_slope(void);
void l_insert(float value, int x, int y);
void init_a_slope(void);
void a_insert(float value, int x, int y);
void graphics_init(void);
void left(Widget, XEvent *, String *, Cardinal *);
void set_zoom_window(void);
void action_frame_track(Widget, XEvent *, String *, Cardinal *);
void frame_track(int *x0, int *y0);
void zoom_it(Widget, XEvent *, String *, Cardinal *);
void clear_zoom(void);
void cancel_zoom(Widget, XEvent *, String *, Cardinal *);
int sorry_msg(void);
void event_refresh(Widget, XtPointer, XEvent *, Boolean *);
void refresh(Widget cmd, XtPointer closure, XtPointer call_data);
void init_prompt_text();
void cancel_text();
void ok_text();
void manage_text_input();
void show_zoomed_objects();
void set_text_input();
void valid_length(int x_or, int y_or, int *x_ret, int *y_ret);
void draw_vector_marker(float a1, float a2, float e1, float e2);
void norm_rectangle(float *x, float *y, float *h, float *v);
void init_text_and_position(void);
void box_text(Widget cmd, XtPointer closure, XtPointer call_data);
void query_box_text_and_position(void);
void print_box_text(char *a,float b, float c, float d, float e, char *f);
void valid_kreis_coords(int x, int y, int *h, int *v,int *rad);
void valid_disc_coords(int x, int y, int *h, int *v,int *rad);
void DrawOval(int x_old,int y_old,int h_old,int v_old);
void query_ask_erase(void);
void perform_erase(void);
void init_ask_erase(void);
void valid_snap_coords(int *x, int *y);
void snapPointerPosition(int *x, int *y, unsigned int *mask);
void draw_the_shadow(void *object,char id);
void init_text_adjustment(void);
void adjust_text(Widget cmd, XtPointer closure, XtPointer call_data);
void query_text_adjustment(void);
void newline(Widget, XEvent *, String *, Cardinal *);
void print_head(char ch);
void leave_translation(void);
void action_textedit(Widget, XEvent *, String *, Cardinal *);
void textedit(Widget, XtPointer, XtPointer);
int number_of_text(void);
void correct_text_error(Widget, XEvent *, String *, Cardinal *);
void beende_textedit(Widget, XEvent *, String *, Cardinal *);
void action_fb_textedit(Widget, XEvent *, String *, Cardinal *);
void fb_textedit(Widget, XtPointer, XtPointer);
int fb_number_of_text(void);
void fb_correct_text_error(Widget, XEvent *, String *, Cardinal *);
void fb_beende_textedit(Widget, XEvent *, String *, Cardinal *);	
void fb_delete_textedit(Widget, XEvent *, String *, Cardinal *);
void action_db_textedit(Widget, XEvent *, String *, Cardinal *);
void db_textedit(Widget, XtPointer, XtPointer);
int db_number_of_text(void);
void db_correct_text_error(Widget, XEvent *, String *, Cardinal *);
void db_beende_textedit(Widget, XEvent *, String *, Cardinal *);	
void db_delete_textedit(Widget, XEvent *, String *, Cardinal *);
void print_text_text_marker(char *pos, float x, float y, char *txt);
void delete_textedit(Widget, XEvent *, String *, Cardinal *);
void event_move_base(Widget cmd, XtPointer closure, XEvent *event,
		     Boolean *continue_to_dispatch);
void move_base(Widget cmd, XtPointer closure, XtPointer call_data);
void manage_curve();
void entry_bezier();
void DrawBezier(float ax, float ay, float ex, float ey, float sx, float sy);
void set_lastBezier_press(void);
void cancel_curve(Widget, XEvent *, String *, Cardinal *);
void exit_curve();
void Dummy(Widget widget,XEvent *event,String *params,Cardinal *n_params);




extern int line_slope;
extern int slope_line;
extern int arrow_slope;
extern int slope_arrow;
extern int line_length;
extern int circle_diameter[];
extern int disc_diameter[];
extern int rast_height;
extern Boolean visible;

#endif
