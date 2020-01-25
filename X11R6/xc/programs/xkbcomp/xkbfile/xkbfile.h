/* $XConsortium: xkbfile.h,v 1.1 94/04/08 15:48:50 erik Exp $ */
/************************************************************
 Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.

 Permission to use, copy, modify, and distribute this
 software and its documentation for any purpose and without
 fee is hereby granted, provided that the above copyright
 notice appear in all copies and that both that copyright
 notice and this permission notice appear in supporting
 documentation, and that the name of Silicon Graphics not be 
 used in advertising or publicity pertaining to distribution 
 of the software without specific prior written permission.
 Silicon Graphics makes no representation about the suitability 
 of this software for any purpose. It is provided "as is"
 without any express or implied warranty.
 
 SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
 SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY 
 AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
 GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
 DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, 
 DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
 THE USE OR PERFORMANCE OF THIS SOFTWARE.

 ********************************************************/

#ifndef XKBFILE_H
#define	XKBFILE_H 1

/***====================================================================***/

#define	XkbXKMFile	0
#define	XkbCFile	1
#define	XkbXKBFile	2
#define	XkbMessage	3

#define	XkbMapDefined		(1<<0)
#define	XkbStateDefined		(1<<1)
#define	XkbModMapDefined	(1<<2)
#define	XkbPerKeyRepeatDefined	(1<<3)
#define	XkbIndicatorMapsDefined	(1<<4)

#define	XkbFileLEDNotBound	0
typedef struct _XkbFileLEDInfo {
    Atom		name;
    unsigned char	indicator;
    unsigned char	flags;
    unsigned char	which_mods;
    unsigned char	real_mods;
    unsigned short	vmods;
    unsigned char	which_groups;
    unsigned char	groups;
    unsigned int	ctrls;
    struct _XkbFileLEDInfo *next;
} XkbFileLEDInfo;

typedef struct _XkbFileInfo {
    Display *		dpy;
    unsigned		type;
    unsigned		defined;
    unsigned		configs;
    XkbDescRec	 	xkb;
    XkbStateRec	 	state;
    XkbChangesRec	changes;
    XkbFileLEDInfo *	leds;
    unsigned char	modmap[XkbMaxLegalKeyCode+1];
    unsigned char	repeat[(XkbMaxLegalKeyCode/8)+1];
} XkbFileInfo;


/***====================================================================***/

extern	char *	XkbAtomText(
#if NeedFunctionPrototypes
    Display *	/* dpy */,
    Atom 	/* atm */,
    unsigned	/* format */
#endif
);

extern char *	XkbKeysymText(
#if NeedFunctionPrototypes
    KeySym	/* sym */,
    unsigned	/* format */
#endif
);

extern char *
XkbModIndexText(
#if NeedFunctionPrototypes
    unsigned	/* ndx */,
    unsigned	/* format */
#endif
);

extern char *
XkbModMaskText(
#if NeedFunctionPrototypes
    unsigned	/* mask */,
    unsigned	/* format */
#endif
);

extern char *	XkbVModIndexText(
#if NeedFunctionPrototypes
    Display *	/* dpy */,
    XkbDescPtr	/* xkb */,
    unsigned	/* ndx */,
    unsigned	/* format */
#endif
);

extern	char *	XkbVModMaskText(
#if NeedFunctionPrototypes
    Display *	/* dpy */,
    XkbDescPtr	/* xkb */,
    unsigned	/* modMask */,
    unsigned	/* mask */,
    unsigned	/* format */
#endif
);

extern char *	XkbIMWhichStateMask(
#if NeedFunctionPrototypes
    unsigned	/* use_which */,
    unsigned	/* format */
#endif
);

extern char *	XkbControlsMaskText(
#if NeedFunctionPrototypes
    unsigned	/* ctrls */,
    unsigned	/* format */
#endif
);

/***====================================================================***/

extern	char *	XkbAtomGetString(
#if NeedFunctionPrototypes
    Display *	/* dpy */,
    Atom 	/* atm */
#endif
);

extern	Atom	XkbInternAtom(
#if NeedFunctionPrototypes
    Display *	/* dpy */,
    char *	/* name */,
    Bool	/* onlyIfExists */
#endif
);

extern	Atom	XkbChangeAtomDisplay(
#if NeedFunctionPrototypes
    Display *	/* oldDpy */,
    Display *	/* newDpy */,
    Atom	/* atm */
#endif
);

extern	void	XkbInitAtoms(
#if NeedFunctionPrototypes
    Display *	/* dpy */
#endif
);

/***====================================================================***/

extern	Bool	XkbWriteCFile(
#if NeedFunctionPrototypes
    char *		/* name */,
    XkbFileInfo *	/* info */
#endif
);

extern	Bool	XkbWriteXKBFile(
#if NeedFunctionPrototypes
    char *		/* name */,
    XkbFileInfo *	/* result */
#endif
);

extern	Bool	XkbWriteXKMFile(
#if NeedFunctionPrototypes
    char *		/* name */,
    XkbFileInfo *	/* result */
#endif
);

/***====================================================================***/

extern	Bool	InitIncludePath(
#if NeedFunctionPrototypes
	void
#endif
);

extern	void	ClearIncludePath(
#if NeedFunctionPrototypes
	void
#endif
);

extern	Bool	AddDirectoryToPath(
#if NeedFunctionPrototypes
	char *	/* dir */
#endif
);

extern	FILE	*FindFileInPath(
#if NeedFunctionPrototypes
    char *	/* name */,
    char *	/* type */,
    char **	/* pathRtrn */
#endif
);

#endif /* XKBFILE_H */
