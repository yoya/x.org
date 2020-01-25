/* $XConsortium: phigsextrn.h,v 5.4 94/04/17 20:41:53 hersh Exp $ */

/***********************************************************

Copyright (c) 1989, 1990, 1991  X Consortium

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

Copyright 1989, 1990, 1991 by Sun Microsystems, Inc. 

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the name of Sun Microsystems,
not be used in advertising or publicity pertaining to distribution of 
the software without specific, written prior permission.  

SUN MICROSYSTEMS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, 
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT 
SHALL SUN MICROSYSTEMS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.
******************************************************************/

#ifndef PHIGS_EXTERN_INCLUDED
#define PHIGS_EXTERN_INCLUDED

extern void popen_phigs();
extern void popen_xphigs();
extern void pclose_phigs();
extern void popen_ws();
extern void pclose_ws();
extern void phigs_ws_type_destroy();
extern void predraw_all_structs();
extern void pupd_ws();
extern void pset_disp_upd_st();
extern void pmessage();
extern void ppolyline3();
extern void ppolyline();
extern void ppolymarker3();
extern void ppolymarker();
extern void ptext3();
extern void ptext();
extern void panno_text_rel3();
extern void panno_text_rel();
extern void pfill_area3();
extern void pfill_area();
extern void pfill_area_set3();
extern void pfill_area_set();
extern void pcell_array3();
extern void pcell_array();
extern void pgdp3();
extern void pgdp();
extern void pset_line_ind();
extern void pset_marker_ind();
extern void pset_text_ind();
extern void pset_int_ind();
extern void pset_edge_ind();
extern void pset_linetype();
extern void pset_linewidth();
extern void pset_line_colr_ind();
extern void pset_marker_type();
extern void pset_marker_size();
extern void pset_marker_colr_ind();
extern void pset_text_font();
extern void pset_text_prec();
extern void pset_char_expan();
extern void pset_char_space();
extern void pset_text_colr_ind();
extern void pset_char_ht();
extern void pset_char_up_vec();
extern void pset_text_path();
extern void pset_text_align();
extern void pset_anno_char_ht();
extern void pset_anno_char_up_vec();
extern void pset_anno_path();
extern void pset_anno_align();
extern void pset_anno_style();
extern void pset_int_style();
extern void pset_int_style_ind();
extern void pset_int_colr_ind();
extern void pset_edge_flag();
extern void pset_edgetype();
extern void pset_edgewidth();
extern void pset_edge_colr_ind();
extern void pset_pat_size();
extern void pset_pat_ref_point_vecs();
extern void pset_pat_ref_point();
extern void padd_names_set();
extern void premove_names_set();
extern void pset_indiv_asf();
extern void pset_line_rep();
extern void pset_marker_rep();
extern void pset_text_rep();
extern void pset_int_rep();
extern void pset_edge_rep();
extern void pset_pat_rep();
extern void pset_colr_rep();
extern void pset_highl_filter();
extern void pset_invis_filter();
extern void pset_colr_model();
extern void pset_hlhsr_id();
extern void pset_hlhsr_mode();
extern void pset_local_tran3();
extern void pset_local_tran();
extern void pset_global_tran3();
extern void pset_global_tran();
extern void pset_model_clip_vol3();
extern void pset_model_clip_vol();
extern void pset_model_clip_ind();
extern void prestore_model_clip_vol();
extern void pset_view_ind();
extern void pset_view_rep3();
extern void pset_view_rep();
extern void pset_view_tran_in_pri();
extern void pset_ws_win3();
extern void pset_ws_win();
extern void pset_ws_vp3();
extern void pset_ws_vp();
extern void ptranslate3();
extern void ptranslate();
extern void pscale3();
extern void pscale();
extern void protate_x();
extern void protate_y();
extern void protate_z();
extern void protate();
extern void pcompose_matrix3();
extern void pcompose_matrix();
extern void ptran_point3();
extern void ptran_point();
extern void pbuild_tran_matrix3();
extern void pbuild_tran_matrix();
extern void pcompose_tran_matrix3();
extern void pcompose_tran_matrix();
extern void peval_view_ori_matrix3();
extern void peval_view_ori_matrix();
extern void peval_view_map_matrix3();
extern void peval_view_map_matrix();
extern void popen_struct();
extern void pclose_struct();
extern void pexec_struct();
extern void plabel();
extern void pappl_data();
extern void pgse();
extern void pset_edit_mode();
extern void pcopy_all_elems_struct();
extern void pset_elem_ptr();
extern void poffset_elem_ptr();
extern void pset_elem_ptr_label();
extern void pdel_elem();
extern void pdel_elem_range();
extern void pdel_elems_labels();
extern void pempty_struct();
extern void pdel_struct();
extern void pdel_struct_net();
extern void pdel_all_structs();
extern void pchange_struct_id();
extern void pchange_struct_refs();
extern void pchange_struct_id_refs();
extern void ppost_struct();
extern void punpost_struct();
extern void punpost_all_structs();
extern void popen_ar_file();
extern void pclose_ar_file();
extern void par_structs();
extern void par_struct_nets();
extern void par_all_structs();
extern void pset_conf_res();
extern void pret_struct_ids();
extern void pret_structs();
extern void pret_struct_nets();
extern void pret_all_structs();
extern void pdel_structs_ar();
extern void pdel_struct_nets_ar();
extern void pdel_all_structs_ar();
extern void pset_pick_id();
extern void pset_pick_filter();
extern void pinit_loc3();
extern void pinit_loc();
extern void pinit_stroke3();
extern void pinit_stroke();
extern void pinit_val3();
extern void pinit_val();
extern void pinit_choice3();
extern void pinit_choice();
extern void pinit_pick3();
extern void pinit_pick();
extern void pinit_string3();
extern void pinit_string();
extern void pset_loc_mode();
extern void pset_stroke_mode();
extern void pset_val_mode();
extern void pset_choice_mode();
extern void pset_pick_mode();
extern void pset_string_mode();
extern void preq_loc3();
extern void preq_loc();
extern void preq_stroke3();
extern void preq_stroke();
extern void preq_val();
extern void preq_choice();
extern void preq_pick();
extern void preq_string();
extern void psample_loc3();
extern void psample_loc();
extern void psample_stroke3();
extern void psample_stroke();
extern void psample_val();
extern void psample_choice();
extern void psample_pick();
extern void psample_string();
extern void pawait_event();
extern void pflush_events();
extern void pget_loc3();
extern void pget_loc();
extern void pget_stroke3();
extern void pget_stroke();
extern void pget_val();
extern void pget_choice();
extern void pget_pick();
extern void pget_string();
extern void pwrite_item();
extern void pget_item_type();
extern void pread_item();
extern void pinterpret_item();
extern void pinq_sys_st();
extern void pinq_ws_st();
extern void pinq_struct_st();
extern void pinq_ar_st();
extern void pinq_list_avail_ws_types();
extern void pinq_phigs_facs();
extern void pinq_gse_facs();
extern void pinq_model_clip_facs();
extern void pinq_edit_mode();
extern void pinq_open_wss();
extern void pinq_struct_ids();
extern void pinq_ar_files();
extern void pinq_conf_res();
extern void pinq_all_conf_structs();
extern void pinq_conf_structs_net();
extern void pret_paths_ances();
extern void pret_paths_descs();
extern void pinq_more_simult_events();
extern void pinq_ws_conn_type();
extern void pinq_list_view_inds();
extern void pinq_view_rep();
extern void pinq_hlhsr_mode_facs();
extern void pinq_posted_structs();
extern void pinq_disp_upd_st();
extern void pinq_list_line_inds();
extern void pinq_line_rep();
extern void pinq_list_marker_inds();
extern void pinq_marker_rep();
extern void pinq_list_text_inds();
extern void pinq_text_rep();
extern void pinq_text_extent();
extern void pinq_list_int_inds();
extern void pinq_int_rep();
extern void pinq_list_edge_inds();
extern void pinq_edge_rep();
extern void pinq_list_pat_inds();
extern void pinq_pat_rep();
extern void pinq_colr_model();
extern void pinq_list_colr_inds();
extern void pinq_colr_rep();
extern void pinq_highl_filter();
extern void pinq_invis_filter();
extern void pinq_ws_tran3();
extern void pinq_ws_tran();
extern void pinq_loc_st3();
extern void pinq_loc_st();
extern void pinq_stroke_st3();
extern void pinq_stroke_st();
extern void pinq_val_st3();
extern void pinq_val_st();
extern void pinq_choice_st3();
extern void pinq_choice_st();
extern void pinq_pick_st3();
extern void pinq_pick_st();
extern void pinq_string_st3();
extern void pinq_string_st();
extern void pinq_ws_cat();
extern void pinq_disp_space_size3();
extern void pinq_disp_space_size();
extern void pinq_hlhsr_mode();
extern void pinq_hlhsr_id_facs();
extern void pinq_view_facs();
extern void pinq_pred_view_rep();
extern void pinq_ws_class();
extern void pinq_dyns_ws_attrs();
extern void pinq_def_disp_upd_st();
extern void pinq_line_facs();
extern void pinq_pred_line_rep();
extern void pinq_marker_facs();
extern void pinq_pred_marker_rep();
extern void pinq_text_facs();
extern void pinq_pred_text_rep();
extern void pinq_anno_facs();
extern void pinq_int_facs();
extern void pinq_pred_int_rep();
extern void pinq_edge_facs();
extern void pinq_pred_edge_rep();
extern void pinq_pat_facs();
extern void pinq_pred_pat_rep();
extern void pinq_colr_model_facs();
extern void pinq_colr_facs();
extern void pinq_pred_colr_rep();
extern void pinq_list_avail_gdp3();
extern void pinq_list_avail_gdp();
extern void pinq_gdp3();
extern void pinq_gdp();
extern void pinq_list_avail_gse();
extern void pinq_num_disp_pris();
extern void pinq_ws_st_table();
extern void pinq_dyns_structs();
extern void pinq_num_avail_in();
extern void pinq_def_loc_data3();
extern void pinq_def_loc_data();
extern void pinq_def_stroke_data3();
extern void pinq_def_stroke_data();
extern void pinq_def_val_data3();
extern void pinq_def_val_data();
extern void pinq_def_choice_data3();
extern void pinq_def_choice_data();
extern void pinq_def_pick_data3();
extern void pinq_def_pick_data();
extern void pinq_def_string_data3();
extern void pinq_def_string_data();
extern void pinq_wss_posted();
extern void pinq_open_struct();
extern void pinq_elem_ptr();
extern void pinq_cur_elem_type_size();
extern void pinq_cur_elem_content();
extern void pinq_elem_type_size();
extern void pinq_elem_content();
extern void pinq_struct_status();
extern void pinq_paths_ances();
extern void pinq_paths_descs();
extern void pelem_search();
extern void pincr_spa_search3();
extern void pincr_spa_search();
extern void pinq_in_overf();
extern void pinq_err_hand_mode();
extern void pemergency_close_phigs();
extern void perr_hand();
extern void perr_log();
extern void pset_err_hand_mode();
extern void pset_err_hand();
extern void pescape();
extern void pcreate_store();
extern void pdel_store();


/* PHIGS+ functions */
extern void ppolyline_set3_data();
extern void pfill_area_set3_data();
extern void ptri_strip3_data();
extern void pquad_mesh3_data();
extern void pset_of_fill_area_set3_data();
extern void pcell_array3_plus();
extern void pnuni_bsp_curv();
extern void pnuni_bsp_surf();
extern void pcomp_fill_area_set_gnorm();
extern void pset_dcue_ind();
extern void pset_refl_props();
extern void pset_back_refl_props();
extern void pset_line_shad_meth();
extern void pset_back_int_style();
extern void pset_back_int_style_ind();
extern void pset_int_shad_meth();
extern void pset_back_int_shad_meth();
extern void pset_refl_eqn();
extern void pset_back_refl_eqn();
extern void pset_light_src_state();
extern void pset_face_disting_mode();
extern void pset_face_cull_mode();
extern void pset_line_colr();
extern void pset_marker_colr();
extern void pset_text_colr();
extern void pset_int_colr();
extern void pset_back_int_colr();
extern void pset_edge_colr();
extern void pset_curve_approx();
extern void pset_surf_approx();
extern void pset_line_rep_plus();
extern void pset_marker_rep_plus();
extern void pset_text_rep_plus();
extern void pset_edge_rep_plus();
extern void pset_int_rep_plus();
extern void pset_pat_rep_plus();
extern void pset_dcue_rep();
extern void pset_light_src_rep();
extern void pset_para_surf_characs();
extern void pset_colr_map_ind();
extern void pset_colr_map_rep();
extern void pset_rendering_colr_model();
extern void pinq_line_rep_plus();
extern void pinq_marker_rep_plus();
extern void pinq_text_rep_plus();
extern void pinq_edge_rep_plus();
extern void pinq_int_rep_plus();
extern void pinq_pat_rep_plus();
extern void pinq_list_dcue_inds();
extern void pinq_dcue_rep();
extern void pinq_list_light_src_inds();
extern void pinq_light_src_rep();
extern void pinq_direct_colr_model_facs();
extern void pinq_line_facs_plus();
extern void pinq_pred_line_rep_plus();
extern void pinq_pred_marker_rep_plus();
extern void pinq_pred_text_rep_plus();
extern void pinq_pred_edge_rep_plus();
extern void pinq_int_facs_plus();
extern void pinq_pred_int_rep_plus();
extern void pinq_pred_pat_rep_plus();
extern void pinq_dcue_facs();
extern void pinq_pred_dcue_rep();
extern void pinq_light_src_facs();
extern void pinq_pred_light_src_rep();
extern void pinq_curv_surf_facs();
extern void pinq_ws_st_table_plus();
extern void pinq_dyns_ws_attrs_plus();
extern void pinq_colr_map_facs();
extern void pinq_colr_map_method_facs();
extern void pinq_colr_map_rep();
extern void pinq_colr_map_st();
extern void pinq_list_colr_map_inds();
extern void pinq_pred_colr_map_rep();
extern void pinq_rendering_colr_model_facs();

#endif
