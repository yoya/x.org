/*
 * xmodmap - program for loading keymap definitions into server
 *
 * $Source: /usr/expo/X/src/clients/xmodmap/RCS/xmodmap.h,v $
 * $Header: xmodmap.h,v 1.1 88/02/08 18:33:57 jim Exp $
 *
 * Copyright 1988 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * Author:  Jim Fulton, MIT X Consortium
 */

extern char *ProgramName;
extern Display *dpy;
extern Bool verbose;
extern Bool dontExecute;
extern char *inputFilename;
extern int lineno;

extern void initialize_map ();
extern int process_file ();
extern int process_line ();
extern int handle_line ();
extern void print_opcode ();
extern void print_work_queue ();
extern int execute_work_queue ();
extern void print_modifier_map ();
