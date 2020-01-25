
/* $XConsortium: computils.m,v 5.2 94/04/17 20:46:30 rws Exp $ */

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


/* comparison routines */
int 		i_compare_Ppoint_list(Ppoint_list,Ppoint_list);
int 		i_compare_Ppoint_list3(Ppoint_list3,Ppoint_list3);
int 		i_compare_Ppick_path(Ppick_path,Ppick_path);
int 		i_compare_Pline_bundle_plus(Pline_bundle_plus,Pline_bundle_plus);
int 		i_compare_Pmarker_bundle_plus(Pmarker_bundle_plus,Pmarker_bundle_plus);
int 		i_compare_Ptext_bundle_plus(Ptext_bundle_plus,Ptext_bundle_plus);
int 		i_compare_Pedge_bundle_plus(Pedge_bundle_plus,Pedge_bundle_plus);
int 		i_compare_Pint_bundle_plus(Pint_bundle_plus,Pint_bundle_plus);
int 		i_compare_Ppat_rep_plus(Ppat_rep_plus,Ppat_rep_plus);
int 		i_compare_Pdcue_bundle(Pdcue_bundle,Pdcue_bundle);
int 		i_compare_Plight_src_bundle(Plight_src_bundle,Plight_src_bundle);
int 		i_compare_Pview_rep3(Pview_rep3,Pview_rep3);
int 		i_compare_Pline_bundle(Pline_bundle,Pline_bundle);
int 		i_compare_Pmarker_bundle(Pmarker_bundle,Pmarker_bundle);
int 		i_compare_Ptext_bundle(Ptext_bundle,Ptext_bundle);
int 		i_compare_Pedge_bundle(Pedge_bundle,Pedge_bundle);
int 		i_compare_Pint_bundle(Pint_bundle,Pint_bundle);
int 		i_compare_Ppat_rep(Ppat_rep,Ppat_rep);
int 		i_compare_Pcolr_rep(Pcolr_rep,Pcolr_rep);
int		i_compare_Pelem_ref_list(Pelem_ref_list,Pelem_ref_list)
int		i_compare_Pelem_ref_list_list(Pelem_ref_list_list,Pelem_ref_list_list)
int		i_compare_Pmatrix(Pmatrix,Pmatrix)
int		i_compare_Pmatrix3(Pmatrix3,Pmatrix3)
int		i_compare_Ppoint(Ppoint,Ppoint)
int		i_compare_Ppoint3(Ppoint3,Ppoint3)
int		i_compare_Pvec(Pvec,Pvec)
int		i_compare_Pvec3(Pvec3,Pvec3)
int		i_compare_Plimit3(Plimit3,Plimit3)
int		i_compare_Plimit(Plimit,Plimit)

int		i_compare_Pint_list_to_ints(Pint_list,Pint,VARARG)
int		i_compare_ordered_Pint_list(Pint_list,Pint_list)
int		i_compare_Pint_list(Pint_list,Pint_list)
int		i_compare_Par_file_list_to_ids_names(Par_file_list,Pint,VARARG)
