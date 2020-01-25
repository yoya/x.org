/* $Header: /usr/src/unsup/bin/patch/RCS/inp.h,v 2.7 87/02/04 11:05:41 ksb Net $
 *
 * $Log:	inp.h,v $
 * Revision 2.7  87/02/04  11:05:41  ksb
 * patched to revision 2.7
 * 
 * Revision 2.0  86/12/02  14:30:33  doc
 * Initial Distributed Revision
 * 
 * Revision 2.0  86/09/17  15:37:25  lwall
 * Baseline for netwide release.
 * 
 */

EXT LINENUM input_lines INIT(0);	/* how long is input file in lines */
EXT LINENUM last_frozen_line INIT(0);	/* how many input lines have been */
					/* irretractibly output */

bool rev_in_string();
void scan_input();
bool plan_a();			/* returns false if insufficient memory */
void plan_b();
char *ifetch();

