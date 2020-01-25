/*
 * xmodmap - program for loading keymap definitions into server
 *
 * $Source: /usr/expo/X/src/clients/xmodmap/RCS/wq.h,v $
 * $Header: wq.h,v 1.1 88/02/08 18:33:59 jim Exp $
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


/* 
 * Input is parsed and a work queue is built that is executed later.  This
 * allows us to swap keys as well as ensure that we don't mess up the keyboard
 * by doing a partial rebind.
 */

enum opcode { doKeycode, doAddModifier, doRemoveModifier, doClearModifier };

struct op_generic {
    enum opcode type;			/* oneof enum opcode */
    union op *next;			/* next element in list or NULL */
};

/*
 * keycode KEYCODE = KEYSYM
 * keysym OLDKEYSYM = NEWKEYSYM
 *
 * want to eval the OLDKEYSYM before executing the work list so that it isn't
 * effected by any assignments.
 */

struct op_keycode {
    enum opcode type;			/* doKeycode */
    union op *next;			/* next element in list or NULL */
    KeyCode target_keycode;		/* key to which we are assigning */
    int count;				/* number of new keysyms */
    KeySym *keysyms;			/* new values to insert */
};


/*
 * add MODIFIER = KEYSYM ...
 */

struct op_addmodifier {
    enum opcode type;			/* doAddModifier */
    union op *next;			/* next element in list or NULL */
    int modifier;			/* index into modifier list */
    int count;				/* number of keysyms */
    KeySym *keysyms;			/* new values to insert */
};


/*
 * remove MODIFIER = OLDKEYSYM ...
 *
 * want to eval the OLDKEYSYM before executing the work list so that it isn't
 * effected by any assignments.
 */

struct op_removemodifier {
    enum opcode type;			/* doRemoveModifier */
    union op *next;			/* next element in list or NULL */
    int modifier;			/* index into modifier list */
    int count;				/* number of keysyms */
    KeyCode *keycodes;			/* old values to remove */
};


/*
 * clear MODIFIER
 */

struct op_clearmodifier {
    enum opcode type;			/* doClearModifier */
    union op *next;			/* next element in list or NULL */
    int modifier;			/* index into modifier list */
};

union op {
    struct op_generic generic;
    struct op_keycode keycode;
    struct op_addmodifier addmodifier;
    struct op_removemodifier removemodifier;
    struct op_clearmodifier clearmodifier;
};

extern struct wq {
    union op *head;
    union op *tail;
} work_queue;


extern struct modtab {
    char *name;
    int length;
    int value;
} modifier_table[];

#define AllocStruct(s) ((s *) malloc (sizeof (s)))

#define MAXKEYSYMNAMESIZE 80		/* absurdly large */
