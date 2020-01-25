
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


/* header-file io_trans.h */

#ifndef IO_TRANSH
#define IO_TRANSH





/* prototypes */

Boolean file_exists(char *fn);
void load_it(char *fn);
void get_line_info(float x, float y, float h, float v, int *x_slp, int *y_slp, float *len);
void get_vector_info(float x, float y, float h, float v, int *x_slp, int *y_slp, float *len);
float newy(float y);
float oldy(float y);
void dimension_update(float x, float y);
void trans_it(char *fn);
void manage_line(int what, FILE *fd);
void manage_vector(int what, FILE *fd);
void manage_filled(int what, FILE *fd);
void manage_framed(int what, FILE *fd);
void manage_dashed(int what, FILE *fd);
void manage_kreis(int what, FILE *fd);
void manage_disc(int what, FILE *fd);
void manage_oval(int what, FILE *fd);
void manage_text(int what, FILE *fd);
void manage_bezier(int what, FILE *fd);
void read_message(void);
void read_framebox(void);
void read_dashbox(void);
void read_filledbox(void);
void read_line(void);
void read_vector(void);
void read_circle(void);
void read_disc(void);					
void read_oval(void);
void read_bezier(void);
void compute_end_coords(float x, float y, float *h, float *v,float len);
float compute_length(float inp);
void error(char *name);


extern float l_slope[30][3];
extern float a_slope[20][3];
extern int line_slope;
extern int arrow_slope;
extern struct object_buffer buffer;
	
extern int unit;
extern float ulen;
extern float dim_x_max,dim_x_min,dim_y_max,dim_y_min;


#endif





