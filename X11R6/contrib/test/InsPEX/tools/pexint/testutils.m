
/* $XConsortium: testutils.m,v 5.2 94/04/17 20:46:40 rws Exp $ */

/*****************************************************************

Copyright (c) 1989, 1990, 1991,1990  X Consortium

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall not be
used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization from the X Consortium.

Copyright (c) 1989, 1990, 1991,1990 by Sun Microsystems, Inc.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Sun Microsystems,
and the X Consortium, not be used in advertising or publicity 
pertaining to distribution of the software without specific, written 
prior permission.  

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT 
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/


void 		i_open_phigs(char*,size_t)
void 		i_open_ws(Pint)
void 		i_open_x_ws(Pint,Display**,Window*,int,int,int,int)
void 		i_fake_button(Pint,Pint,Ppoint3,int)
Ppoint3*	i_generate_n_Ppoint3(int,Pfloat,Pfloat,Pfloat)
Ppoint_list3*	i_generate_n_Ppoint_list3(int,int)
void		i_grid(Pint,Pint,Pfloat,Pfloat,Pfloat,Pfloat,Pfloat,Pfloat,Pfloat,Pint,Pint);
void		i_get_window_dim()
void		i_make_xwindow(Display**,Window*,Pconnid_x_drawable*,int,int,int,int)
Pint		i_get_ws_type(char*)
void		i_print_var(char*,VARARG) 
void		i_pause() 
void		i_wsxform_point3(Display*,Window,Pint,Ppoint3,int*,int*,int*)
void		i_viewxform_point3(Pint,Pint,Ppoint3,Ppoint3*,int*)
int		i_check_pixel3(Display*,Window,Pint,Pint,Ppoint3,int,Pint)
int		i_check_line3(Display*,Window,Pint,Pint,Ppoint3*,int,Pint)
int		i_bogus_error();
Pdyn_mod	i_get_ws_dyn_mod(Pint,char*);
Pdyn_mod	i_get_struct_dyn_mod(Pint,char*);
int		i_get_phigs_info(char*);
int		i_is_posted_struct(Pint,Pint, Pfloat*);
!void		tfprintf(char*,VARARG)/* the interpreter version */
!void		tvprintf(int,char*,VARARG)/* the interpreter version */
!void		tabort(char*,VARARG) /* the interpreter version */

