/*
 * xmodmap - program for loading keymap definitions into server
 *
 * $Source: /usr/expo/X/src/clients/xmodmap/RCS/handle.c,v $
 * $Header: handle.c,v 1.3 88/02/14 16:59:10 jim Exp $
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

#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <ctype.h>
#include "xmodmap.h"
#include "wq.h"

static XModifierKeymap *map = NULL;


/*
 * The routines in this file manipulate a queue of intructions.  Instead of
 * executing each line as it is entered, we build up a list of actions to 
 * take and execute them all at the end.  This allows us to find all errors
 * at once, and to preserve the context in which we are looking up keysyms.
 */

struct wq work_queue = {NULL, NULL};


/*
 * common utility routines
 */

char *copy_to_scratch (s, len)
    char *s;
    int len;
{
    static char *buf = NULL;
    static int buflen = 0;

    if (len > buflen) {
	if (buf) free (buf);
	buflen = (len < 40) ? 80 : (len * 2);
	buf = (char *) malloc (buflen+1);
    }
    if (len > 0)
      strncpy (buf, s, len);
    else 
      len = 0;

    buf[len] = '\0';
    return (buf);
}

static void badmsg (what, arg)
    char *what;
    char *arg;
{
    fprintf (stderr, "%s:  %s:%d:  bad ", ProgramName, inputFilename, lineno);
    fprintf (stderr, what, arg);
    fprintf (stderr, "\n");
    return;
}

static void badmsgn (what, s, len)
    char *what;
    char *s;
    int len;
{
    badmsg (what, copy_to_scratch (s, len));
    return;
}


void initialize_map ()
{
    map = XGetModifierMapping (dpy);
    return;
}

static int do_keycode(), do_keysym(), finish_keycode(), get_keysym_list();
static int do_add(), do_remove(), do_clear();

int skip_word(), skip_space(), skip_chars();

static struct dt {
    char *command;			/* name of input command */
    int length;				/* length of command */
    int (*proc)();			/* handler */
} dispatch_table[] = {
    { "keycode", 7, do_keycode },
    { "keysym", 6, do_keysym },
    { "add", 3, do_add },
    { "remove", 6, do_remove },
    { "clear", 5, do_clear },
    { NULL, 0, NULL }};

/*
 * handle_line - this routine parses the input line (which has had all leading
 * and trailing whitespace removed) and builds up the work queue.
 */

int handle_line (line, len)
    char *line;				/* string to parse */
    int len;				/* length of line */
{
    int n;
    struct dt *dtp;
    int status;

    n = skip_chars (line, len);
    if (n < 0) {
	badmsg ("input line '%s'", line);
	return (-1);
    }

    for (dtp = dispatch_table; dtp->command != NULL; dtp++) {
	if (n == dtp->length &&
	    strncmp (line, dtp->command, dtp->length) == 0) {

	    n += skip_space (line+n, len-n);
	    line += n, len -= n;

	    status = (*(dtp->proc)) (line, len);
	    return (status);
	}
    }

    fprintf (stderr, "%s:  unknown command on line %s:%d\n",
	     ProgramName, inputFilename, lineno);
    return (-1);
}

/*
 * the following routines are useful for parsing
 */ 

int skip_word (s, len)
    register char *s;
    register int len;
{
    register int n;

    n = skip_chars (s, len);
    return (n + skip_space (s+n, len-n));
}

int skip_chars (s, len)
    register char *s;
    register int len;
{
    register int i;

    if (len <= 0 || !s || *s == '\0') return (0);

    for (i = 0; i < len; i++) {
	if (isspace(s[i])) break;
    }
    return (i);
}

int skip_space (s, len)
    register char *s;
    register int len;
{
    register int i;

    if (len <= 0 || !s || *s == '\0') return (0);

    for (i = 0; i < len; i++) {
	if (!isspace(s[i])) break;
    }
    return (i);
}


int skip_until_char (s, len, c)
    register char *s;
    register int len;
    register char c;
{
    register int i;

    for (i = 0; i < len; i++) {
	if (s[i] == c) break;
    }
    return (i);
}

int skip_until_chars (s, len, cs, cslen)
    char *s;
    int len;
    register char *cs;
    register int cslen;
{
    int i;

    for (i = 0; i < len; i++) {
	register int j;
	register char c = s[i];

	for (j = 0; j < cslen; j++) {
	    if (c == cs[j]) goto done;
	}
    }
  done:
    return (i);
}

/*
 * The action routines.
 *
 * This is where the real work gets done.  Each routine is responsible for
 * parsing its input (note that the command keyword has been stripped off)
 * and adding to the work queue.  They are also in charge of outputting
 * error messages and returning non-zero if there is a problem.
 *
 * The following global variables are available:
 *     dpy                the display descriptor
 *     work_queue         linked list of opcodes
 *     inputFilename      name of the file being processed
 *     lineno             line number of current line in input file
 */

add_to_work_queue (p)			/* this can become a macro someday */
    union op *p;
{
    if (work_queue.head == NULL) {	/* nothing on the list */
	work_queue.head = work_queue.tail = p;	/* head, tail, no prev */
    } else {
	work_queue.tail->generic.next = p;  /* head okay, prev */
	work_queue.tail = p;		/* tail */
    }
    p->generic.next = NULL;

    if (verbose) {
	print_opcode (p);
    }
    return;
}

char *copystring (s, len)
    char *s;
    int len;
{
    char *retval;

    retval = (char *) malloc (len+1);
    if (retval) {
	strncpy (retval, s, len);
	retval[len] = '\0';
    }
    return (retval);
}

static KeySym parse_keysym (line, n)
    char *line;
    int n;
{
    return (XStringToKeysym (copy_to_scratch (line, n)));
}


/*
 * do_keycode - parse off lines of the form
 *
 *                 "keycode" number "=" keysym ...
 *                           ^
 *
 * where number is in decimal, hex, or octal.  Any number of keysyms may be
 * listed.
 */

static int do_keycode (line, len)
    char *line;
    int len;
{
    int n;
    int dummy;
    char *fmt = "%d";
    KeyCode keycode;

    if (len < 3 || !line || *line == '\0') {  /* 5=a minimum */
	badmsg ("keycode input line", NULL);
	return (-1);
    }

    if (*line == '0') line++, len--, fmt = "%o";
    if (*line == 'x' || *line == 'X') line++, len--, fmt = "%x";

    dummy = 0;
    if (sscanf (line, fmt, &dummy) != 1 || dummy == 0) {
	badmsg ("keycode value", NULL);
	return (-1);
    }
    keycode = (KeyCode) dummy;

    return (finish_keycode (line, len, keycode));
}


/*
 * do_keysym - parse off lines of the form
 *
 *                 "keysym" keysym "=" keysym ...
 *                          ^
 *
 * The left keysyms has to be checked for validity and evaluated.
 */

static int do_keysym (line, len)
    char *line;
    int len;
{
    int n;
    KeyCode keycode;
    KeySym keysym;
    KeySym *kslist;
    struct op_keycode *opk;
    char *tmpname;

    if (len < 3 || !line || *line == '\0') {  /* a=b minimum */
	badmsg ("keysym input line", NULL);
	return (-1);
    }

    n = skip_chars (line, len);
    if (n < 1) {
	badmsg ("target keysym name", NULL);
	return (-1);
    }
    tmpname = copy_to_scratch (line, n);
    keysym = XStringToKeysym (tmpname);
    if (keysym == NoSymbol) {
	badmsg ("keysym target key symbol '%s'", tmpname);
	return (-1);
    }

    keycode = XKeysymToKeycode (dpy, keysym);
    if (verbose) {
	printf ("! Keysym %s (0x%lx) corresponds to keycode 0x%x\n", 
		tmpname, (long) keysym, keycode);
    }
    if (keycode < dpy->min_keycode || keycode > dpy->max_keycode) {
	if (!verbose) {
	    printf ("! Keysym %s (0x%lx) corresponds to keycode 0x%x\n", 
		    tmpname, (long) keysym, keycode);
	}
	badmsg ("keysym target keysym '%s', out of range", tmpname);
	return (-1);
    }

    return (finish_keycode (line, len, keycode));
}

static int finish_keycode (line, len, keycode)
    char *line;
    int len;
    KeyCode keycode;
{
    int n;
    KeySym *kslist;
    struct op_keycode *opk;
   
    n = skip_until_char (line, len, '=');
    line += n, len -= n;
    
    if (len < 2 || *line != '=') {	/* =a minimum */
	badmsg ("keycode command (missing keysym list),", NULL);
	return (-1);
    }
    line++, len--;			/* skip past the = */

    n = skip_space (line, len);
    line += n, len -= n;

    if (get_keysym_list (line, len, &n, &kslist) < 0) {
	badmsg ("keycode keysym list", NULL);
	return (-1);
    }

    opk = AllocStruct (struct op_keycode);
    if (!opk) {
	badmsg ("attempt to allocate a %ld byte keycode opcode",
		(long) sizeof (struct op_keycode));
	return (-1);
    }

    opk->type = doKeycode;
    opk->target_keycode = keycode;
    opk->count = n;
    opk->keysyms = kslist;

    add_to_work_queue (opk);

#ifdef later
    /* make sure we handle any special keys */
    check_special_keys (keycode, n, kslist);
#endif

    return (0);
}


/*
 * parse_modifier - convert a modifier string name to its index
 */

struct modtab modifier_table[] = {	/* keep in order so it can be index */
    { "shift", 5, 0 },
    { "lock", 4, 1 },
    { "control", 7, 2 },
    { "mod1", 4, 3 },
    { "mod2", 4, 4 },
    { "mod3", 4, 5 },
    { "mod4", 4, 6 },
    { "mod5", 4, 7 },
    { "ctrl", 4, 2 },
    { NULL, 0, 0 }};

static int parse_modifier (line, n)
    register char *line;
    register int n;
{
    register int i;
    struct modtab *mt;

    /* lowercase for comparison against table */
    for (i = 0; i < n; i++) {
	if (isupper (line[i])) line[i] = tolower (line[i]);
    }

    for (mt = modifier_table; mt->name; mt++) {
	if (n == mt->length && strncmp (line, mt->name, n) == 0)
	  return (mt->value);
    }
    return (-1);
}


/*
 * do_add - parse off lines of the form
 *
 *                 add MODIFIER = keysym ...
 *                     ^
 * where the MODIFIER is one of Shift, Lock, Control, Mod[1-5] where case
 * is not important.  There should also be an alias Ctrl for control.
 */

static int do_add (line, len)
    char *line;
    int len;
{
    int n;
    int modifier;
    KeySym *kslist;
    struct op_addmodifier *opam;

    if (len < 6 || !line || *line == '\0') {  /* Lock=a minimum */
	badmsg ("add modifier input line", NULL);
	return (-1);
    }

    n = skip_chars (line, len);
    if (n < 1) {
	badmsg ("add modifier name %s", line);
	return (-1);
    }

    modifier = parse_modifier (line, n);
    if (modifier < 0) {
	badmsgn ("add modifier name '%s', not allowed", line, n);
	return (-1);
    }

    line += n, len -= n;
    n = skip_until_char (line, len, '=');
    if (n < 0) {
	badmsg ("add modifier = keysym", NULL);
	return (-1);
    }

    n++;				/* skip = */
    n += skip_space (line+n, len-n);
    line += n, len -= n;

    if (get_keysym_list (line, len, &n, &kslist) < 0) {
	badmsg ("add modifier keysym list", NULL);
	return (-1);
    }

    opam = AllocStruct (struct op_addmodifier);
    if (!opam) {
	badmsg ("attempt to allocate %ld byte addmodifier opcode",
		(long) sizeof (struct op_addmodifier));
	return (-1);
    }

    opam->type = doAddModifier;
    opam->modifier = modifier;
    opam->count = n;
    opam->keysyms = kslist;

    add_to_work_queue (opam);
    
    return (0);
}

/*
 * make_add - stick a single add onto the queue
 */

static void make_add (modifier, keysym)
    int modifier;
    KeySym keysym;
{
    struct op_addmodifier *opam;

    opam = AllocStruct (struct op_addmodifier);
    if (!opam) {
	badmsg ("attempt to allocate %ld byte addmodifier opcode",
		(long) sizeof (struct op_addmodifier));
	return;
    }

    opam->type = doAddModifier;
    opam->modifier = modifier;
    opam->count = 1;
    opam->keysyms = (KeySym *) malloc (sizeof (KeySym));
    if (!opam->keysyms) {
	badmsg ("attempt to allocate %ld byte KeySym", (long) sizeof (KeySym));
	free ((char *) opam);
	return;
    }
    opam->keysyms[0] = keysym;

    add_to_work_queue (opam);
    return;
}


/*
 * do_remove - parse off lines of the form
 *
 *                 remove MODIFIER = oldkeysym ...
 *                        ^
 * where the MODIFIER is one of Shift, Lock, Control, Mod[1-5] where case
 * is not important.  There should also be an alias Ctrl for control.
 */

static int do_remove (line, len)
    char *line;
    int len;
{
    int n;
    int nc;
    int i;
    int modifier;
    KeySym *kslist;
    KeyCode *kclist;
    struct op_removemodifier *oprm;

    if (len < 6 || !line || *line == '\0') {  /* Lock=a minimum */
	badmsg ("remove modifier input line", NULL);
	return (-1);
    }

    n = skip_chars (line, len);
    if (n < 1) {
	badmsg ("remove modifier name %s", line);
	return (-1);
    }

    modifier = parse_modifier (line, n);
    if (modifier < 0) {
	badmsgn ("remove modifier name '%s', not allowed", line, n);
	return (-1);
    }

    line += n, len -= n;
    n = skip_until_char (line, len, '=');
    if (n < 0) {
	badmsg ("remove modifier = keysym", NULL);
	return (-1);
    }

    n++;
    n += skip_space (line+n, len-n);
    line += n, len -= n;

    if (get_keysym_list (line, len, &n, &kslist) < 0) {
	badmsg ("remove modifier keysym list", NULL);
	return (-1);
    }

    /*
     * unlike the add command, we have to now evaluate the keysyms
     */

    kclist = (KeyCode *) malloc (n * sizeof (KeyCode));
    if (!kclist) {
	badmsg ("attempt to allocate %ld byte keycode list",
		(long) (n * sizeof (KeyCode)));
	free ((char *) kslist);
	return (-1);
    }

    nc = 0;
    for (i = 0; i < n; i++) {
	KeyCode kc = XKeysymToKeycode (dpy, kslist[i]);
	if (verbose) {
	    char *tmpname = XKeysymToString (kslist[i]);
	    printf ("! Keysym %s (0x%lx) corresponds to keycode 0x%x\n", 
		    tmpname ? tmpname : "?", (long) kslist[i], kc);
	}
	if (kc < dpy->min_keycode || kc > dpy->max_keycode) {
	    char *tmpname = XKeysymToString (kslist[i]);
	    printf ("! Keysym %s (0x%lx), keycode 0x%x, ",
		    tmpname ? tmpname : "?", (long) kslist[i], kc);
	    printf ("out of range [0x%x, 0x%x]\n", 
		    dpy->min_keycode, dpy->max_keycode);
	    badmsg ("keycode value 0x%lx in remove modifier list",
		    (long) kc);
	    continue;
	}
	kclist[nc++] = kc;		/* okay, add it to list */
    }

    free ((char *) kslist);		/* all done with it */

    oprm = AllocStruct (struct op_removemodifier);
    if (!oprm) {
	badmsg ("attempt to allocate %ld byte removemodifier opcode",
		(long) sizeof (struct op_removemodifier));
	return (-1);
    }

    oprm->type = doRemoveModifier;
    oprm->modifier = modifier;
    oprm->count = nc;
    oprm->keycodes = kclist;

    add_to_work_queue (oprm);
    
    return (0);
}

/*
 * make_remove - stick a single remove onto the queue
 */

static void make_remove (modifier, keycode)
    int modifier;
    KeyCode keycode;
{
    struct op_removemodifier *oprm;

    oprm = AllocStruct (struct op_removemodifier);
    if (!oprm) {
	badmsg ("attempt to allocate %ld byte removemodifier opcode",
		(long) sizeof (struct op_removemodifier));
	return;
    }

    oprm->type = doRemoveModifier;
    oprm->modifier = modifier;
    oprm->count = 1;
    oprm->keycodes = (KeyCode *) malloc (sizeof (KeyCode));
    if (!oprm->keycodes) {
	badmsg ("attempt to allocate %ld byte KeyCode",
		(long) sizeof (KeyCode));
	free ((char *) oprm);
	return;
    }
    oprm->keycodes[0] = keycode;

    add_to_work_queue (oprm);
    return;
}


/*
 * do_clear - parse off lines of the form
 *
 *                 clear MODIFIER
 *                       ^
 */

static int do_clear (line, len)
    char *line;
    int len;
{
    int n;
    int modifier;
    struct op_clearmodifier *opcm;

    if (len < 4 || !line || *line == '\0') {  /* Lock minimum */
	badmsg ("clear modifier input line", NULL);
	return (-1);
    }

    n = skip_chars (line, len);

    modifier = parse_modifier (line, n);
    if (modifier < 0) {
	badmsgn ("clear modifier name '%s'", line, n);
	return (-1);
    }
    n += skip_space (line+n, len-n);
    if (n != len) {
	badmsgn ("extra argument '%s' to clear modifier", line+n, len-n);
	/* okay to continue */
    }

    opcm = AllocStruct (struct op_clearmodifier);
    if (!opcm) {
	badmsg ("attempt to allocate %d byte clearmodifier opcode",
		(long) sizeof (struct op_clearmodifier));
	return (-1);
    }

    opcm->type = doClearModifier;
    opcm->modifier = modifier;

    add_to_work_queue (opcm);

    return (0);
}

/*
 * get_keysym_list - parses the rest of the line into a keysyms assumes
 * that the = sign has been parsed off but there may be leading whitespace
 *
 *                 keysym ...
 *                 ^
 *
 * this involves getting the word containing the keysym, checking its range,
 * and adding it to the list.
 */

static int get_keysym_list (line, len, np, kslistp)
    char *line;
    int len;
    int *np;
    KeySym **kslistp;
{
    int havesofar, maxcanhave;
    KeySym *keysymlist;

    *np = 0;
    *kslistp = NULL;

    havesofar = 0;
    maxcanhave = 4;			/* most lists are small */
    keysymlist = (KeySym *) malloc (maxcanhave * sizeof (KeySym));
    if (!keysymlist) {
	badmsg ("attempt to allocate %ld byte initial keysymlist",
		(long) (maxcanhave * sizeof (KeySym)));
	return (-1);
    }

    while (len > 0) {
	KeySym keysym;
	int n;

	n = skip_space (line, len);
	line += n, len -= n;

	n = skip_chars (line, len);
	if (n < 0) {
	    badmsg ("keysym name list", NULL);
	    return (-1);
	}

	keysym = parse_keysym (line, n);
	line += n, len -= n;
	if (keysym == NoSymbol) {
	    badmsgn ("keysym name '%s' in keysym list", line, n);
	    /* do NOT return here, look for others */
	    continue;
	}

	/* grow the list bigger if necessary */
	if (havesofar >= maxcanhave) {
	    maxcanhave *= 2;
	    keysymlist = (KeySym *) realloc (keysymlist,
					     maxcanhave * sizeof (KeySym));
	    if (!keysymlist) {
		badmsg ("attempt to grow keysym list to %ld bytes",
			(long) (maxcanhave * sizeof (KeySym)));
		return (-1);
	    }
	}

	/* and add it to the list */
	keysymlist[havesofar++] = keysym;
    }

    *kslistp = keysymlist;
    *np = havesofar;
    return (0);
}


#ifdef later
/*
 * check_special_keys - run through list of keysyms and generate "add" or
 * "remove" commands for for any of the key syms that appear in the modifier
 * list.  this involves running down the modifier map which is an array of
 * 8 by map->max_keypermod keycodes.
 */

static void check_special_keys (keycode, n, kslist)
    KeyCode keycode;
    int n;
    KeySym *kslist;
{
    int i;				/* iterator variable */
    KeyCode *kcp;			/* keycode pointer */

    /*
     * walk the modifiermap array.  since its dimensions are not known at
     * compile time, we have to walk it by hand instead of indexing.  this
     * is why it is initialized outside the loop, but incremented inside the
     * second loop.
     */

    kcp = map->modifiermap;		/* start at beginning and iterate */
    for (i = 0; i < 8; i++) {		/* there are 8 modifier keys */
	int j;

	for (j = 0; j < map->max_keypermod; j++, kcp++) {
	    KeySym keysym;
	    int k;

	    if (!*kcp) continue;	/* only non-zero entries significant */

	    /*
	     * check to see if the target keycode is already a modifier; if so,
	     * then we have to remove it
	     */
	    if (keycode == *kcp) {
		make_remove (i, keycode);
	    }

	    /*
	     * now, check to see if any of the keysyms map to keycodes
	     * that are in the modifier list
	     */
	    for (k = 0; k < n; k++) {
		KeyCode kc = XKeysymToKeycode (dpy, kslist[k]);

		if (kc == *kcp) {	/* yup, found one */
		    /*
		     * have to generate a remove of the CURRENT keycode
		     * and then an add of the new KEYCODE
		     */
		    make_remove (i, kc);  /* modifier, keycode */
		    make_add (i, kslist[k]);  /* modifier, keysym */
		}
	    }
	}
    }
    return;
}
#endif

/*
 * print_work_queue - disassemble the work queue and print it on stdout
 */

void print_work_queue ()
{
    union op *op;

    printf ("! dump of work queue\n");
    for (op = work_queue.head; op; op = op->generic.next) {
	print_opcode (op);
    }
    return;
}

void print_opcode (op)
    union op *op;
{
    int i;

    printf ("        ");
    switch (op->generic.type) {
      case doKeycode:
	printf ("keycode 0x%lx =", (long) op->keycode.target_keycode);
	for (i = 0; i < op->keycode.count; i++) {
	    char *name = XKeysymToString (op->keycode.keysyms[i]);

	    printf (" %s", name ? name : "BADKEYSYM");
	}
	printf ("\n");
	break;
      case doAddModifier:
	printf ("add %s =", modifier_table[op->addmodifier.modifier]);
	for (i = 0; i < op->addmodifier.count; i++) {
	    char *name = XKeysymToString (op->addmodifier.keysyms[i]);
	    printf (" %s", name ? name : "BADKEYSYM");
	}
	printf ("\n");
	break;
      case doRemoveModifier:
	printf ("remove %s = ",
		modifier_table[op->removemodifier.modifier]);
	for (i = 0; i < op->removemodifier.count; i++) {
	    printf (" 0x%lx", (long) op->removemodifier.keycodes[i]);
	}
	printf ("\n");
	break;
      case doClearModifier:
	printf ("clear %s\n", modifier_table[op->clearmodifier.modifier]);
	break;
      default:
	printf ("! unknown opcode %d\n", op->generic.type);
	break;
    }				/* end switch */
    return;
}

/*
 * execute_work_queue - do the real meat and potatoes now that we know what
 * we need to do and that all of the input is correct.
 */

static int exec_keycode(), exec_add(), exec_remove(), exec_clear();

int execute_work_queue ()
{
    union op *op;
    int errors;
    Bool update_map = False;
    enum opcode lastop;

    if (verbose) {
	printf ("!\n");
	printf ("! executing work queue\n");
	printf ("!\n");
    }

    errors = 0;
    lastop = doClearModifier;				/* fake it for op test */

    for (op = work_queue.head; op; op = op->generic.next) {
	if (verbose) print_opcode (op);

	/* check to see if we have to update the keyboard mapping */
	if (lastop == doKeycode && op->generic.type != doKeycode) {
	    XSync (dpy, 0);
	    while (XEventsQueued (dpy, QueuedAlready) > 0) {
		XEvent event;
		XNextEvent (dpy, &event);
		if (event.type == MappingNotify) {
		    /* read all MappingNotify events */
		    while (XCheckTypedEvent (dpy, MappingNotify, &event)) ;
		    XRefreshKeyboardMapping (&event);
		} else {
		    fprintf (stderr, "%s:  unknown event %ld\n", 
		    	     ProgramName, (long) event.type);
		}
	    }
	}

	switch (op->generic.type) {
	  case doKeycode:
	    if (exec_keycode (op) < 0) errors++;
	    break;
	  case doAddModifier:
	    if (exec_add (op) < 0) errors++;
	    else update_map = True;
	    break;
	  case doRemoveModifier:
	    if (exec_remove (op) < 0) errors++;
	    else update_map = True;
	    break;
	  case doClearModifier:
	    if (exec_clear (op) < 0) errors++;
	    else update_map = True;
	    break;
	  default:
	    fprintf (stderr, "%s:  unknown opcode %d\n", op->generic.type);
	    break;
	}
	lastop = op->generic.type;

    }

    if (update_map) {
	if (UpdateModifierMapping (map) < 0) errors++;
    }

    return (errors > 0 ? -1 : 0);
}

static int exec_keycode (opk)
    struct op_keycode *opk;
{
    XChangeKeyboardMapping (dpy, opk->target_keycode, opk->count, 
			    opk->keysyms, 1);
    return (0);
}

static int exec_add (opam)
    struct op_addmodifier *opam;
{
    int i;
    int status;

    status = 0;
    for (i = 0; i < opam->count; i++) {
	KeyCode kc = XKeysymToKeycode (dpy, opam->keysyms[i]);

	if (AddModifier (&map, kc, opam->modifier) < 0)
	  status = -1;
    }
    return (status);
}

static int exec_remove (oprm)
    struct op_removemodifier *oprm;
{
    int i;
    int status;

    status = 0;
    for (i = 0; i < oprm->count; i++) {
	if (RemoveModifier (&map, oprm->keycodes[i], oprm->modifier) < 0)
	  status = -1;
    }
    return (status);
}

static int exec_clear (opcm)
    struct op_clearmodifier *opcm;
{
    return (ClearModifier (&map, opcm->modifier));
}


void print_modifier_map ()
{
    PrintModifierMapping (map, stdout);
    return;
}
