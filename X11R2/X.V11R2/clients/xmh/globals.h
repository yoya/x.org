/*
 *	rcs_id[] = "$Header: globals.h,v 2.10 88/02/23 20:06:29 swick Exp $";
 */

/*
 *			  COPYRIGHT 1987
 *		   DIGITAL EQUIPMENT CORPORATION
 *		       MAYNARD, MASSACHUSETTS
 *			ALL RIGHTS RESERVED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE WITHOUT NOTICE AND
 * SHOULD NOT BE CONSTRUED AS A COMMITMENT BY DIGITAL EQUIPMENT CORPORATION.
 * DIGITAL MAKES NO REPRESENTATIONS ABOUT THE SUITABILITY OF THIS SOFTWARE FOR
 * ANY PURPOSE.  IT IS SUPPLIED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY.
 *
 * IF THE SOFTWARE IS MODIFIED IN A MANNER CREATING DERIVATIVE COPYRIGHT RIGHTS,
 * APPROPRIATE LEGENDS MAY BE PLACED ON THE DERIVATIVE WORK IN ADDITION TO THAT
 * SET FORTH ABOVE.
 *
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting documentation,
 * and that the name of Digital Equipment Corporation not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.
 */

#ifdef MAIN
#define ext
#else
#define ext extern
#endif
ext int *dptr;

ext Boolean	debug;

ext Display	*theDisplay;	/* Display variable. */
ext int		theScreen;	/* Which screen we're using. */
ext Widget	toplevel;	/* The top level widget (A hack %%%). */

ext char	*progName;	/* Program name. */
ext char	*homeDir;	/* User's home directory. */
ext char	*mailDir;	/* mh's mail directory. */
ext char	*tempDir;	/* Directory to use for temporary files. */
ext char	*displayName;	/* Name of the X display. */
ext char	*defGeometry;	/* Default geometry to use for things. */
ext char	*defTocGeometry;
ext char	*defViewGeometry;
ext char	*defCompGeometry;
ext char	*defPickGeometry;
ext char	*initialFolderName; /* Initial folder to use. */
ext char	*initialIncFile;	/* -file for inc on initial folder */
ext char	*draftsFolderName;	/* Folder for drafts. */
ext char	*draftFile;		/* Filename of draft. */
ext char	*xmhDraftFile;		/* Filename for sending. */
ext char	*defMhPath;		/* Path for mh commands. */
ext int		defSendLineWidth;	/* How long to break lines on send. */
ext int		defBreakSendLineWidth;	/* Minimum length of a line before
					   we'll break it. */
ext char	*defPrintCommand; /* Printing command. */

ext int		defTocWidth;	/* How many characters wide to use in tocs */
ext int		mailWaitingFlag;	/* If true, change icon on new mail */

ext Boolean	SkipDeleted;		/* If true, skip over deleted msgs. */
ext Boolean	SkipMoved;		/* If true, skip over moved msgs. */
ext Boolean	SkipCopied;		/* If true, skip over copied msgs. */

ext Boolean	defHideBoringHeaders;

ext int		defTocPercentage;
ext Boolean	defNewMailCheck; /* Whether to check for new mail. */
ext Boolean	defMakeCheckpoints; /* Whether to create checkpoint files. */

ext Toc		*folderList;	/* Array of folders. */
ext int		numFolders;	/* Number of entries in above array. */
ext Toc		InitialFolder;	/* Toc containing initial folder. */
ext Toc		DraftsFolder;	/* Toc containing drafts. */

ext Scrn	*scrnList;	/* Array of scrns in use. */
ext int		numScrns;	/* Number of scrns in above array. */

ext Button	LastButtonPressed;

ext XtTextSource NullSource;

ext Dimension	rootwidth;	/* Dimensions of root window.  */
ext Dimension	rootheight;

ext Pixmap	NoMailPixmap;	/* Icon pixmap if no new mail. */
ext Pixmap	NewMailPixmap;	/* Icon pixmap if new mail. */
