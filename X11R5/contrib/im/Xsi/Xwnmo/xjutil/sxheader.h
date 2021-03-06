/*
 * $Id: sxheader.h,v 1.2 1991/09/16 21:35:27 ohm Exp $
 */
/*
 * Copyright 1989, 1990, 1991 by OMRON Corporation
 * Copyright 1991 by Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the names of OMRON and MIT not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  OMRON and MIT make no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * Wnn consortium is one of distributors of the official Wnn source code
 * release.  Wnn consortium also makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * OMRON, MIT AND WNN CONSORTIUM DISCLAIM ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS, IN NO EVENT SHALL OMRON, MIT OR WNN CONSORTIUM BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTUOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *	Author:	Seiji Kuwari	OMRON Corporation
 *				kuwa@omron.co.jp
 *				kuwa%omron.co.jp@uunet.uu.net
 */				
/*
 * Copyright Kyoto University Research Institute for Mathematical Sciences
 *                 1987, 1988, 1989, 1990, 1991
 * Copyright OMRON Corporation. 1987, 1988, 1989, 1990, 1991
 * Copyright ASTEC, Inc. 1987, 1988, 1989, 1990, 1991
 *
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that all of the following conditions are satisfied:
 *
 * 1) The above copyright notices appear in all copies
 * 2) Both those copyright notices and this permission notice appear
 *    in supporting documentation
 * 3) The name of "Wnn" isn't changed unless substantial modifications
 *    are made, or
 * 3') Following words followed by the above copyright notices appear
 *    in all supporting documentation of software based on "Wnn":
 *
 *   "This software is based on the original version of Wnn developed by
 *    Kyoto University Research Institute for Mathematical Sciences (KURIMS),
 *    OMRON Corporation and ASTEC Inc."
 *
 * 4) The names KURIMS, OMRON and ASTEC not be used in advertising or
 *    publicity pertaining to distribution of the software without
 *    specific, written prior permission
 *
 * KURIMS, OMRON and ASTEC make no representations about the suitability
 * of this software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * Wnn consortium is one of distributors of the official Wnn source code
 * release.  Wnn consortium also makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * KURIMS, OMRON, ASTEC AND WNN CONSORTIUM DISCLAIM ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL KURIMS, OMRON, ASTEC OR
 * WNN CONSORTIUM BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */
/*	Version 4.0
 */
/**************************
 * header of standard i/o 
 **************************/

#include "jslib.h"

extern int	max_env;
extern WnnEnv *normal_env;
extern WnnEnv *reverse_env;
extern WnnEnv *cur_normal_env;
extern WnnEnv *cur_reverse_env;
extern int save_env_id[32];
extern int save_env_reverse_id[32];

extern w_char	*input_buffer;
extern w_char *return_buf;

extern int	cur_bnst_ ;		/* current bunsetsu pointer */

extern int rubout_code;

extern char romkan_clear_tbl[TBL_CNT][TBL_SIZE];
extern int (*main_table[TBL_CNT][TBL_SIZE])();	/* holding commands */
extern 	w_char jishopath[];
extern 	w_char hindopath[];
extern w_char fuzokugopath[];

extern ClientBuf *c_b;

extern int excellent_delete;

extern int *bunsetsu;
extern int *bunsetsuend;
extern int *touroku_bnst;
extern int touroku_bnst_cnt;
extern w_char *kill_buffer;
extern w_char *remember_buf;
extern int kill_buffer_offset;

extern int touroku_comment;

extern char *uumrc_name;
extern char *envrc_name;
extern char *uumkey_name;
extern char *cvt_fun_file;
extern char *cvt_meta_file;
extern char *rkfile_name;

extern char *jserver_name;
extern char *display_name;
extern char *username;

extern int maxbunsetsu;
extern int max_ichiran_kosu;
extern int maxchg;
extern int maxlength;

extern WNN_DIC_INFO *dicinfo;
extern int dic_list_size;

extern char *prgname;

extern struct wnn_ret_buf	rb;
extern struct wnn_ret_buf	dicrb;
extern struct wnn_ret_buf	wordrb;

extern Xjutil	*xjutil;
extern JutilTextRec	*cur_text;
extern Romkan	*cur_rk;
extern RomkanTable	*cur_rk_table;
extern BoxRec	*box_list;

extern XjutilFSList    font_set_list;
extern XjutilFSRec     *cur_fs;


extern char *default_message[];
extern struct msg_cat *cd;
extern BoxRec *create_box();

extern FunctionTable *f_table;
extern XjutilFuncDataBase function_db[];
extern FunctionTable default_func_table;
