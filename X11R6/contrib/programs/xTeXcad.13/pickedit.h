
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


/* header-file pickedit.h */

#ifndef PICKEDITH
#define PICKEDITH

int analyse_pin(struct fig2 *p, int *d, int xpos, int ypos);
int analyse_nbox(struct fig3 *p, int *d, int xpos, int ypos);
int analyse_dbox(struct fig4 *p, int *d, int xpos, int ypos);
int analyse_fbox(struct fig2 *p, int *d, int xpos, int ypos);
int analyse_ncircle(struct fig2 *p, int *d, int xpos, int ypos);
int analyse_ocircle(struct fig1 *p, int *d, int xpos, int ypos);
int analyse_fcircle(struct fig2 *p, int *d, int xpos, int ypos);
void init_pick(void);
void set_pick_stack(int x, int y);
void insert_object(void *p, int d, char c);
int nextObject(void);
void set_pick_object(void);
void set_copy_object();
void manage_pick(Widget, XEvent *, String *, Cardinal *);
int pick_move(int x, int y);
void pick_edit(void);
void erase_current_object(void);
void prepare_top_stack_object(void);
void set_pick_erase(void);
void pick_erase(Widget, XEvent *, String *, Cardinal *);
void init_no_objects_sign(void);
void pick_manager(void);
void leave_pick(char *msg);
void edit_y_line(void);
void edit_x_line(void);
void edit_y_vector(void);
void edit_x_vector(void);
void set_dash_mode(void);
void set_normal_mode(void);
void del_vector_marker(float x, float y, float h, float v);
int calc_distance(int x, int y, int h, int v);
void edit_frame(char c);
void draw_edit_marker(float x, float y, float h, float v);
void edit_circle(char c);
int valid_circle_edit(int x, int y);
void manage_copy(Widget, XEvent *, String *, Cardinal *);
void copy_manager(void);
void copy_it(int x, int y);
void edit_bezier_curveS(void);
void edit_bezier_curveE(void);
void edit_bezier_curveA(void);




#define sumobj 5

extern int maxobj;
extern char kind[sumobj];
extern int distance[sumobj];
extern void *obj[sumobj];
extern int tip;

extern int x_pick, y_pick, currIndex;

#endif
