/* $Header: /usr/src/unsup/bin/patch/RCS/pch.h,v 2.7 87/02/04 11:06:20 ksb Net $
 *
 * $Log:	pch.h,v $
 * Revision 2.7  87/02/04  11:06:20  ksb
 * patched to revision 2.7
 * 
 * Revision 2.0  86/12/02  14:30:49  doc
 * Initial Distributed Revision
 * 
 * Revision 2.0  86/09/17  15:39:57  lwall
 * Baseline for netwide release.
 * 
 */

EXT FILE *pfp INIT(Nullfp);		/* patch file pointer */

void re_patch();
void open_patch_file();
void set_hunkmax();
void grow_hunkmax();
bool there_is_another_patch();
int intuit_diff_type();
void next_intuit_at();
void skip_to();
bool another_hunk();
bool pch_swap();
char *pfetch();
short pch_line_len();
LINENUM pch_first();
LINENUM pch_ptrn_lines();
LINENUM pch_newfirst();
LINENUM pch_repl_lines();
LINENUM pch_end();
LINENUM pch_context();
LINENUM pch_hunk_beg();
char pch_char();
char *pfetch();
char *pgets();
void do_ed_script();
