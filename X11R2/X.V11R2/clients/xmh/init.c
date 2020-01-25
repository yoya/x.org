#ifndef lint
static char rcs_id[] = "$Header: init.c,v 2.15 88/02/23 20:05:21 swick Exp $";
#endif lint
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

/* Init.c - Handle start-up initialization. */

#include "xmh.h"

extern char* _XLowerCase();

/* Xmh-specific resources. */
static Boolean defFalse = False;
static Boolean defTrue = True;

static XtResource resources[] = {
    {"debug", "Debug", XtRBoolean, sizeof(Boolean),
	 (Cardinal)&debug, XtRBoolean, (caddr_t)&defFalse},
    {"tempdir", "tempDir", XtRString, sizeof(char *),
	 (Cardinal)&tempDir, XtRString, "/tmp"},
    {"mhpath", "MhPath", XtRString, sizeof(char *),
	 (Cardinal)&defMhPath, XtRString, "/usr/local/mh6"},
    {"initialfolder", "InitialFolder", XtRString, sizeof(char *),
	 (Cardinal)&initialFolderName, XtRString, "inbox"},
    {"initialincfile", "InitialIncFile", XtRString, sizeof(char *),
         (Cardinal)&initialIncFile, XtRString, NULL},
    {"draftsfolder", "DraftsFolder", XtRString, sizeof(char *),
	 (Cardinal)&draftsFolderName, XtRString, "drafts"},
    {"sendwidth", "SendWidth", XtRInt, sizeof(int),
	 (Cardinal)&defSendLineWidth, XtRString, "72"},
    {"sendbreakwidth", "SendBreakWidth", XtRInt, sizeof(int),
	 (Cardinal)&defBreakSendLineWidth, XtRString, "85"},
    {"printcommand", "PrintCommand", XtRString, sizeof(char *),
	 (Cardinal)&defPrintCommand, XtRString,
	 "enscript > /dev/null 2>/dev/null"},
    {"tocwidth", "TocWidth", XtRInt, sizeof(int),
	 (Cardinal)&defTocWidth, XtRString, "100"},
    {"skipdeleted", "SkipDeleted", XtRBoolean, sizeof(Boolean),
	 (Cardinal)&SkipDeleted, XtRBoolean, (caddr_t)&defTrue},
    {"skipmoved", "SkipMoved", XtRBoolean, sizeof(Boolean),
	 (Cardinal)&SkipMoved, XtRBoolean, (caddr_t)&defTrue},
    {"skipcopied", "SkipCopied", XtRBoolean, sizeof(Boolean),
	 (Cardinal)&SkipCopied, XtRBoolean, (caddr_t)&defFalse},
    {"hideboringheaders", "HideBoringHeaders", XtRBoolean, sizeof(Boolean),
	 (Cardinal)&defHideBoringHeaders, XtRBoolean, (caddr_t)&defTrue},
    {"geometry", "Geometry", XtRString, sizeof(char *),
	 (Cardinal)&defGeometry, XtRString, NULL},
    {"tocgeometry", "TocGeometry", XtRString, sizeof(char *),
	 (Cardinal)&defTocGeometry, XtRString, NULL},
    {"viewgeometry", "ViewGeometry", XtRString, sizeof(char *),
	 (Cardinal)&defViewGeometry, XtRString, NULL},
    {"compgeometry", "CompGeometry", XtRString, sizeof(char *),
	 (Cardinal)&defCompGeometry, XtRString, NULL},
    {"pickgeometry", "PickGeometry", XtRString, sizeof(char *),
	 (Cardinal)&defPickGeometry, XtRString, NULL},
    {"tocpercentage", "TocPercentage", XtRInt, sizeof(int),
	 (Cardinal)&defTocPercentage, XtRString, "33"},
    {"checknewmail", "CheckNewMail", XtRBoolean, sizeof(Boolean),
	 (Cardinal)&defNewMailCheck, XtRBoolean, (caddr_t)&defTrue},
    {"makecheckpoints", "MakeCheckPoints", XtRBoolean, sizeof(Boolean),
	 (Cardinal)&defMakeCheckpoints, XtRBoolean, (caddr_t)&defFalse},
    {"mailpath", "MailPath", XtRString, sizeof(char *),
	 (Cardinal)&mailDir, XtRString, NULL},
    {"mailwaitingflag", "MailWaitingFlag", XtRBoolean, sizeof(Boolean),
	 (Cardinal)&mailWaitingFlag, XtRBoolean, &defFalse},
};

static XrmOptionDescRec table[] = {
    {"-debug",	"debug",		XrmoptionNoArg,	"on"},
    {"-flag",	"mailwaitingflag",	XrmoptionNoArg, "on"},
    {"-initial","initialfolder",	XrmoptionSepArg, NULL},
    {"-path",	"mailPath",		XrmoptionSepArg, NULL},
};

/* Tell the user how to use this program. */
Syntax(call)
    char *call;
{
    extern void exit();
    (void)fprintf(stderr, "usage: %s [-path <path>] [-initial <folder>]\n", call);
    exit(2);
}


static char *FixUpGeometry(geo, defwidth, defheight)
char *geo;
Dimension defwidth, defheight;
{
    int gbits;
    Position x, y;
    Dimension width, height;
    if (geo == NULL) geo = defGeometry;
    x = y = 0;
    gbits = XParseGeometry(geo, &x, &y, &width, &height);
    if (!(gbits & WidthValue)) {
	width = defwidth;
	gbits |= WidthValue;
    }
    if (!(gbits & HeightValue)) {
	height = defheight;
	gbits |= HeightValue;
    }
    return CreateGeometry(gbits, x, y, width, height);
}


/* All the start-up initialization goes here. */

InitializeWorld(argc, argv)
unsigned int argc;
char **argv;
{
    int l;
    FILEPTR fid;
    char str[500], str2[500], *ptr;
    Scrn scrn;
    static XtActionsRec actions[] = {
	{"open-folder", OpenFolder},
	{"open-sequence", OpenSequence},
    };

    ptr = rindex(argv[0], '/');
    if (ptr) progName = ptr + 1;
    else progName = argv[0];

    toplevel = XtInitialize(progName, "Xmh", table, XtNumber(table),
			    &argc, argv);
    if (argc > 1) Syntax(progName);
    theDisplay = XtDisplay(toplevel);
    theScreen = DefaultScreen(theDisplay);

    homeDir = MallocACopy(getenv("HOME"));

    XtGetApplicationResources( toplevel, (caddr_t) NULL,
			       resources, XtNumber(resources),
			       NULL, (Cardinal)0 );

    if (mailWaitingFlag) defNewMailCheck = True;

    (void) sprintf(str, "%s/.mh_profile", homeDir);
    fid = myfopen(str, "r");
    if (fid) {
	while (ptr = ReadLine(fid)) {
	    strncpy(str2, ptr, 5);
	    str2[5] = '\0';
	    LowerCase(str2, str2);
	    if (strcmp(str2, "path:") == 0) {
		ptr += 5;
		while (*ptr == ' ' || *ptr == '\t')
		    ptr++;
		(void) strcpy(str, ptr);
	    }
	}
	(void) myfclose(fid);
    } else {
	(void) strcpy(str, "Mail");
    }
    for (l=strlen(str) - 1; l>=0 && (str[l] == ' ' || str[l] == '\t'); l--)
	str[l] = 0;
    if (str[0] == '/')
	(void) strcpy(str2, str);
    else
	(void) sprintf(str2, "%s/%s", homeDir, str);

    (void) sprintf(str, "%s/draft", str2);
    draftFile = MallocACopy(str);
    (void) sprintf(str, "%s/xmhdraft", str2);
    xmhDraftFile = MallocACopy(str);

    if (!mailDir) mailDir = MallocACopy(str2);

    NullSource = XtCreateEDiskSource("/dev/null", FALSE);

    l = strlen(defMhPath) - 1;
    if (l > 0 && defMhPath[l] == '/')
	defMhPath[l] = 0;

    rootwidth = DisplayWidth(theDisplay, theScreen);
    rootheight = DisplayHeight(theDisplay, theScreen);

    defTocGeometry = FixUpGeometry(defTocGeometry,
				   rootwidth / 2, 3 * rootheight / 4);
    defViewGeometry = FixUpGeometry(defViewGeometry,
				    rootwidth / 2, rootheight / 2);
    defCompGeometry = FixUpGeometry(defCompGeometry,
				    rootwidth / 2, rootheight / 2);
    defPickGeometry = FixUpGeometry(defPickGeometry,
				    rootwidth / 2, rootheight / 2);

    numScrns = 0;
    scrnList = (Scrn *) XtMalloc((unsigned) 1);
    LastButtonPressed = NULL;

    TocInit();
    InitPick();
    IconInit();

    XtAddActions(actions, XtNumber(actions));

    DEBUG("Making screen ... ")

    scrn = CreateNewScrn(STtocAndView);

    DEBUG(" setting toc ... ")

    TocSetScrn(InitialFolder, scrn);

    DEBUG("done.\n");

/* if (debug) {DEBUG("Syncing ..."); ExecSyncOn(); DEBUG(" done\n");} */

    MapScrn(scrn);
}
