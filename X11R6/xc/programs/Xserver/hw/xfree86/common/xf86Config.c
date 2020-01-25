/*
 * $XConsortium: xf86Config.c,v 1.6 95/01/16 13:16:57 kaleb Exp $
 * $XFree86: xc/programs/Xserver/hw/xfree86/common/xf86Config.c,v 3.34 1995/01/11 03:50:32 dawes Exp $
 *
 * Copyright 1990,91 by Thomas Roell, Dinkelscherben, Germany.
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Thomas Roell not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  Thomas Roell makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THOMAS ROELL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THOMAS ROELL BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */

#include "X.h"
#include "Xmd.h"
#include "input.h"
#include "servermd.h"
#include "scrnintstr.h"

#include "compiler.h"

#include "xf86_OSlib.h"

#include "xf86Procs.h"

#define INIT_CONFIG
#include "xf86_Config.h"

#define CONFIG_BUF_LEN     1024

static FILE * configFile   = NULL;
static int    configStart  = 0;           /* start of the current token */
static int    configPos    = 0;           /* current readers position */
static int    configLineNo = 0;           /* linenumber */
static char   *configBuf,*configRBuf;     /* buffer for lines */
static char   *configPath;                /* path to config file */
static char   *fontPath = NULL;           /* font path */
static int    pushToken = LOCK_TOKEN;
static LexRec val;                        /* global return value */

static int screenno = -100;      /* some little number ... */

extern char *getenv();
extern char *defaultFontPath;
extern char *rgbPath;

extern Bool xf86fpFlag, xf86coFlag;
extern Bool xf86ScreensOpen;

extern int defaultColorVisualClass;

char *xf86VisualNames[] = {
    "StaticGray",
    "GrayScale",
    "StaticColor",
    "PseudoColor",
    "TrueColor",
    "DirectColor"
};

static void configFilesSection(
#if NeedFunctionPrototypes
    void
#endif
);
static void configServerFlagsSection(
#if NeedFunctionPrototypes
    void
#endif
);
static void configKeyboardSection(
#if NeedFunctionPrototypes
    void
#endif
);
static void configDeviceSection(
#if NeedFunctionPrototypes
    void
#endif
);
static void configPointerSection(
#if NeedFunctionPrototypes
    void
#endif
);
static void configScreenSection(
#if NeedFunctionPrototypes
    void
#endif
);
static void configDisplaySubsection(
#if NeedFunctionPrototypes
    DispPtr disp
#endif
);
static void configMonitorSection(
#if NeedFunctionPrototypes
    void
#endif
);
static DisplayModePtr xf86PruneModes(
#if NeedFunctionPrototypes
    MonPtr monp,
    DisplayModePtr allmodes,
    char *scrname
#endif
);
static void readVerboseMode(
#if NeedFunctionPrototypes
    MonPtr monp
#endif
);
static Bool mode_fits(
#if NeedFunctionPrototypes
    DisplayModePtr dispmp,
    MonPtr monp,
    char *scrname
#endif
);


#define DIR_FILE	"/fonts.dir"

/*
 * xf86GetPathElem --
 *	Extract a single element from the font path string starting at
 *	pnt.  The font path element will be returned, and pnt will be
 *	updated to point to the start of the next element, or set to
 *	NULL if there are no more.
 */
static char *
xf86GetPathElem(pnt)
     char **pnt;
{
  char *p1;

  p1 = *pnt;
  *pnt = index(*pnt, ',');
  if (*pnt != NULL) {
    **pnt = '\0';
    *pnt += 1;
  }
  return(p1);
}

/*
 * StrToUL --
 *
 *	A portable, but restricted, version of strtoul().  It only understands
 *	hex, octal, and decimal.  But it's good enough for our needs.
 */
unsigned int StrToUL(str)
char *str;
{
  int base = 10;
  char *p = str;
  unsigned int tot = 0;

  if (*p == '0') {
    p++;
    if (*p == 'x') {
      p++;
      base = 16;
    }
    else
      base = 8;
  }
  while (*p) {
    if ((*p >= '0') && (*p <= ((base == 8)?'7':'9'))) {
      tot = tot * base + (*p - '0');
    }
    else if ((base == 16) && (*p >= 'a') && (*p <= 'f')) {
      tot = tot * base + 10 + (*p - 'a');
    }
    else if ((base == 16) && (*p >= 'A') && (*p <= 'F')) {
      tot = tot * base + 10 + (*p - 'A');
    }
    else {
      return(tot);
    }
    p++;
  }
  return(tot);
}

/*
 * xf86ValidateFontPath --
 *	Validates the user-specified font path.  Each element that
 *	begins with a '/' is checked to make sure the directory exists.
 *	If the directory exists, the existence of a file named 'fonts.dir'
 *	is checked.  If either check fails, an error is printed and the
 *	element is removed from the font path.
 */
#define CHECK_TYPE(mode, type) ((S_IFMT & (mode)) == (type))
static char *
xf86ValidateFontPath(path)
     char *path;
{
  char *tmp_path, *out_pnt, *path_elem, *next, *p1;
  struct stat stat_buf;
  int flag;

  tmp_path = (char *)Xcalloc(strlen(path)+1);
  out_pnt = tmp_path;
  path_elem = NULL;
  next = path;
  while (next != NULL) {
    path_elem = xf86GetPathElem(&next);
    if (*path_elem == '/') {
      flag = stat(path_elem, &stat_buf);
      if (flag == 0)
	if (!CHECK_TYPE(stat_buf.st_mode, S_IFDIR))
	  flag = -1;
      if (flag != 0) {
        ErrorF("Warning: The directory \"%s\" does not exist.\n", path_elem);
	ErrorF("         Entry deleted from font path.\n");
	continue;
      }
      else {
	p1 = (char *)xalloc(strlen(path_elem)+strlen(DIR_FILE)+1);
	strcpy(p1, path_elem);
	strcat(p1, DIR_FILE);
	flag = stat(p1, &stat_buf);
	if (flag == 0)
	  if (!CHECK_TYPE(stat_buf.st_mode, S_IFREG))
	    flag = -1;
	xfree(p1);
	if (flag != 0) {
	  ErrorF("Warning: 'fonts.dir' not found (or not valid) in \"%s\".\n", 
		 path_elem);
	  ErrorF("          Entry deleted from font path.\n");
	  ErrorF("          (Run 'mkfontdir' on \"%s\").\n", path_elem);
	  continue;
	}
      }
    }

    /*
     * Either an OK directory, or a font server name.  So add it to
     * the path.
     */
    if (out_pnt != tmp_path)
      *out_pnt++ = ',';
    strcat(out_pnt, path_elem);
    out_pnt += strlen(path_elem);
  }
  return(tmp_path);
}

/*
 * getToken --
 *      Read next Token form the config file. Handle the global variable
 *      pushToken.
 */
static int
getToken(tab)
     SymTabRec tab[];
{
  int          c, i;

  /*
   * First check whether pushToken has a different value than LOCK_TOKEN.
   * In this case rBuf[] contains a valid STRING/TOKEN/NUMBER. But in the other
   * case the next token must be read from the input.
   */
  if (pushToken == EOF) return(EOF);
  else if (pushToken == LOCK_TOKEN)
    {
      
      c = configBuf[configPos];
      
      /*
       * Get start of next Token. EOF is handled, whitespaces & comments are
       * skipped. 
       */
      do {
	if (!c)  {
	  if (fgets(configBuf,CONFIG_BUF_LEN-1,configFile) == NULL)
	    {
	      return( pushToken = EOF );
	    }
	  configLineNo++;
	  configStart = configPos = 0;
	}
	while (((c=configBuf[configPos++])==' ') || ( c=='\t') || ( c=='\n'));
	if (c == '#') c = '\0'; 
      } while (!c);
      
      /* GJA -- handle '-' and ',' 
       * Be careful: "-hsync" is a keyword.
       */
      if ( (c == ',') && !isalpha(configBuf[configPos]) ) {
         configStart = configPos; return COMMA;
      } else if ( (c == '-') && !isalpha(configBuf[configPos]) ) {
         configStart = configPos; return DASH;
      }

      configStart = configPos;
      /*
       * Numbers are returned immediately ...
       */
      if (isdigit(c))
	{
          extern double atof();
	  int base;

	  if (c == '0')
	    if ((configBuf[configPos] == 'x') || 
		(configBuf[configPos] == 'X'))
	      base = 16;
	    else
	      base = 8;
	  else
	    base = 10;

	  configRBuf[0] = c; i = 1;
	  while (isdigit(c = configBuf[configPos++]) || 
		 (c == '.') || (c == 'x') || 
		 ((base == 16) && (((c >= 'a') && (c <= 'f')) ||
				   ((c >= 'A') && (c <= 'F')))))
            configRBuf[i++] = c;
          configPos--; /* GJA -- one too far */
	  configRBuf[i] = '\0';
	  val.num = StrToUL(configRBuf);
          val.realnum = atof(configRBuf);
	  return(NUMBER);
	}
      
      /*
       * All Strings START with a \" ...
       */
      else if (c == '\"')
	{
	  i = -1;
	  do {
	    configRBuf[++i] = (c = configBuf[configPos++]);
	  } while ((c != '\"') && (c != '\n') && (c != '\0'));
	  configRBuf[i] = '\0';
	  val.str = (char *)xalloc(strlen(configRBuf) + 1);
	  strcpy(val.str, configRBuf);      /* private copy ! */
	  return(STRING);
	}
      
      /*
       * ... and now we MUST have a valid token. Since all tokens are handled
       * caseinsenitive, they are all lowercased internally. The search is
       * handled later along with the pushed tokens.
       */
      else
	{
	  configRBuf[0] = isupper (c) ? tolower(c) : c;
	  i = 0;
	  do {
	    c = configBuf[configPos++];
	    if (isupper (c))
	      c = tolower (c);
	    configRBuf[++i] = c;
	  } while ((c != ' ') && (c != '\t') && (c != '\n') && (c != '\0'));
	  configRBuf[i] = '\0'; i=0;
	}
      
    }
  else
    {
    
      /*
       * Here we deal with pushed tokens. Reinitialize pushToken again. If
       * the pushed token was NUMBER || STRING return them again ...
       */
      int temp = pushToken;
      pushToken = LOCK_TOKEN;
    
      if (temp == COMMA || temp == DASH) return(temp);
      if (temp == NUMBER || temp == STRING) return(temp);
    }
  
  /*
   * Joop, at last we have to lookup the token ...
   */
  if (tab)
    {
      i = 0;
      while (tab[i].token != -1)
	if (strcmp(configRBuf,tab[i].name) == 0)
	  return(tab[i].token);
	else
	  i++;
    }
  
  return(ERROR_TOKEN);       /* Error catcher */
}

/*
 * getToken --
 *	Lookup a string if it is actually a token in disguise.
 */
static int
getStringToken(tab)
     SymTabRec tab[];
{
  int i;
  char *s1p, *s2p;
  
  for ( i = 0 ; tab[i].token != -1 ; i++ ) {
    if ( ! StrCaseCmp(tab[i].name,val.str) ) return tab[i].token;
  }
  return(ERROR_TOKEN);
}

/*
 * getScreenIndex --
 *	Given the screen token, returns the index in xf86Screens, or -1 if
 *	the screen type is not applicable to this server.
 */
static int
getScreenIndex(token)
     int token;
{
  int i;

  for (i = 0; xf86ScreenNames[i] >= 0 && xf86ScreenNames[i] != token; i++)
    ;
  if (xf86ScreenNames[i] < 0)
    return(-1);
  else
    return(i);
}

/*
 * validateGraphicsToken --
 *	If token is a graphics token, check it is in the list of validTokens
 * XXXX This needs modifying to work as it did with the old format
 */
static Bool
validateGraphicsToken(validTokens, token)
     int *validTokens;
     int token;
{
  int i;

  for (i = 0; ScreenTab[i].token >= 0 && ScreenTab[i].token != token; i++)
    ;
  if (ScreenTab[i].token < 0)
    return(FALSE);        /* Not a graphics token */

  for (i = 0; validTokens[i] >= 0 && validTokens[i] != token; i++)
    ;
  return(validTokens[i] >= 0);
}

/*
 * xf86TokenToString --
 *	returns the string corresponding to token
 */
char *
xf86TokenToString(table, token)
     SymTabPtr table;
     int token;
{
  int i;

  for (i = 0; table[i].token >= 0 && table[i].token != token; i++)
    ;
  if (table[i].token < 0)
    return("unknown");
  else
    return(table[i].name);
}
 
/*
 * xf86StringToToken --
 *	returns the string corresponding to token
 */
int
xf86StringToToken(table, string)
     SymTabPtr table;
     char *string;
{
  int i;

  for (i = 0; table[i].token >= 0 && StrCaseCmp(string, table[i].name); i++)
    ;
  return(table[i].token);
}
 
/*
 * configError --
 *      Print a READABLE ErrorMessage!!!  All information that is 
 *      interesting is printed.  Even a pointer to the erroneous place is
 *      printed.  Maybe our e-mail will be less :-)
 */
static void
configError(msg)
     char *msg;
{
  int i,j;

  ErrorF( "\nConfig Error: %s:%d\n\n%s", configPath, configLineNo, configBuf);
  for (i = 1, j = 1; i < configStart; i++, j++) 
    if (configBuf[i-1] != '\t')
      ErrorF(" ");
    else
      do
	ErrorF(" ");
      while (((j++)%8) != 0);
  for (i = configStart; i <= configPos; i++) ErrorF("^");
  ErrorF("\n%s\n", msg);
  exit(-1);                 /* simple exit ... */
}

void
xf86DeleteMode(infoptr, dispmp)
ScrnInfoPtr	infoptr;
DisplayModePtr	dispmp;
{
	if(infoptr->modes == dispmp)
		infoptr->modes = dispmp->next;

	if(dispmp->next == dispmp)
		FatalError("No valid modes found.\n");

	ErrorF("%s %s: Removing mode \"%s\" from list of valid modes.\n",
	       XCONFIG_PROBED, infoptr->name, dispmp->name);
	dispmp->prev->next = dispmp->next;
	dispmp->next->prev = dispmp->prev;

	xfree(dispmp->name);
	xfree(dispmp);
}

/*
 * findConfigFile --
 * 	Locate the XF86Config file.  Abort if not found.
 */
static void
findConfigFile(filename, fp)
      char *filename;
      FILE **fp;
{
  char           *home = NULL;
  char           *xconfig = NULL;
  char	         *xwinhome = NULL;

#define configFile (*fp)
#define configPath filename

  /*
   * First open if necessary the config file.
   * If the -xf86config flag was used, use the name supplied there (root only).
   * If $XF86CONFIG is a pathname, use it as the name of the config file (root)
   * If $XF86CONFIG is set but doesn't contain a '/', append it to 'XF86Config'
   *   and search the standard places (root only).
   * If $XF86CONFIG is not set, just search the standard places.
   */
  while (!configFile) {
    
    /*
     * First check if the -xf86config option was used.
     */
    if (getuid() == 0 && xf86ConfigFile[0]) {
      strcpy(configPath, xf86ConfigFile);
      if (configFile = fopen(configPath, "r"))
        break;
      else
        FatalError(
             "Cannot read file \"%s\" specified by the -xf86config flag\n",
             configPath);
    }
    /*
     * Check if XF86CONFIG is set.
     */
    if (getuid() == 0 && (xconfig = getenv("XF86CONFIG"))) {
      if (index(xconfig, '/')) {
        strcpy(configPath, xconfig);
        if (configFile = fopen(configPath, "r"))
          break;
        else
          FatalError(
               "Cannot read file \"%s\" specified by XF86CONFIG variable\n",
               configPath);
      }
    }
     
    /*
     * ~/XF86Config ...
     */
    if (getuid() == 0 && (home = getenv("HOME"))) {
      strcpy(configPath,home);
      strcat(configPath,"/XF86Config");
      if (xconfig) strcat(configPath,xconfig);
      if (configFile = fopen( configPath, "r" )) break;
    }
    
    /*
     * /etc/XF86Config
     */
    strcpy(configPath, "/etc/XF86Config");
    if (xconfig) strcat(configPath,xconfig);
    if (configFile = fopen( configPath, "r" )) break;
    
    /*
     * $(LIBDIR)/XF86Config.<hostname>
     */

    if (getuid() == 0 && (xwinhome = getenv("XWINHOME")) != NULL)
	sprintf(configPath, "%s/lib/X11/XF86Config", xwinhome);
    else
	strcpy(configPath, SERVER_CONFIG_FILE);
    if (getuid() == 0 && xconfig) strcat(configPath,xconfig);
    strcat(configPath, ".");
#ifdef AMOEBA
    {
      extern char *XServerHostName;

      strcat(configPath, XServerHostName);
    }
#else
    gethostname(configPath+strlen(configPath), MAXHOSTNAMELEN);
#endif
    if (configFile = fopen( configPath, "r" )) break;
    
    /*
     * $(LIBDIR)/XF86Config
     */
    if (getuid() == 0 && xwinhome)
	sprintf(configPath, "%s/lib/X11/XF86Config", xwinhome);
    else
	strcpy(configPath, SERVER_CONFIG_FILE);
    if (getuid() == 0 && xconfig) strcat(configPath,xconfig);
    if (configFile = fopen( configPath, "r" )) break;
    
    FatalError("No config file found!\n%s", getuid() == 0 ? "" :
               "Note, the X server no longer looks for XF86Config in $HOME");
  }
  if (xf86Verbose) {
    ErrorF("XF86Config: %s\n", configPath);
    ErrorF("%s stands for supplied, %s stands for probed/default values\n",
       XCONFIG_GIVEN, XCONFIG_PROBED);
  }
#undef configFile
#undef configPath
}

static DisplayModePtr pNew, pLast;
static Bool graphFound = FALSE;

/*
 * xf86GetNearestClock --
 *	Find closest clock to given frequency (in kHz).  This assumes the
 *	number of clocks is greater than zero.
 */
static int
xf86GetNearestClock(Screen, Frequency)
	ScrnInfoPtr Screen;
	int Frequency;
{
  int NearestClock = 0;
  int MinimumGap = abs(Frequency - Screen->clock[0]);
  int i;
  for (i = 1;  i < Screen->clocks;  i++)
  {
    int Gap = abs(Frequency - Screen->clock[i]);
    if (Gap < MinimumGap)
    {
      MinimumGap = Gap;
      NearestClock = i;
    }
  }
  return NearestClock;
}

/*
 * xf86Config --
 *	Fill some internal structure with userdefined setups. Many internal
 *      Structs are initialized.  The drivers are selected and initialized.
 *	if (! vtopen), XF86Config is read, but devices are not probed.
 *	if (vtopen), devices are probed (and modes resolved).
 *	The vtopen argument was added so that XF86Config information could be
 *	made available before the VT is opened.
 */
void
xf86Config (vtopen)
     int vtopen;
{
  int            token;
  int            i, j;
#if defined(SYSV) || defined(linux)
  int            xcpipe[2];
#endif

 if (!vtopen)
 {

  OFLG_ZERO(&GenericXF86ConfigFlag);
  configBuf  = (char*)xalloc(CONFIG_BUF_LEN);
  configRBuf = (char*)xalloc(CONFIG_BUF_LEN);
  configPath = (char*)xalloc(PATH_MAX);
  
  configBuf[0] = '\0';                    /* sanity ... */
  
  /*
   * Read the XF86Config file with the real uid to avoid security problems
   *
   * For SYSV we fork, and send the data back to the parent through a pipe
   */

#if defined(SYSV) || defined(linux)
  if (getuid() != 0) {
    if (pipe(xcpipe))
      FatalError("Pipe failed (%s)\n", strerror(errno));
    switch (fork()) {
      case -1:
        FatalError("Fork failed (%s)\n", strerror(errno));
        break;
      case 0:   /* child */
        close(xcpipe[0]);
        setuid(getuid());  
        findConfigFile(configPath, &configFile);
        {
          unsigned char pbuf[CONFIG_BUF_LEN];
          int nbytes;

          /* Pass the filename back as the first line */
          strcat(configPath, "\n");
          if (write(xcpipe[1], configPath, strlen(configPath)) < 0)
            FatalError("Child error writing to pipe (%s)\n", strerror(errno));
          while ((nbytes = fread(pbuf, 1, CONFIG_BUF_LEN, configFile)) > 0)
            if (write(xcpipe[1], pbuf, nbytes) < 0)
              FatalError("Child error writing to pipe (%s)\n", strerror(errno));
        }
        close(xcpipe[1]);
        fclose(configFile);
        exit(0);
        break;
      default: /* parent */
        close(xcpipe[1]);
        configFile = (FILE *)fdopen(xcpipe[0], "r");
        if (fgets(configPath, PATH_MAX, configFile) == NULL)
          FatalError("Error reading config file\n");
        configPath[strlen(configPath) - 1] = '\0';
    }
  }
  else {
    findConfigFile(configPath, &configFile);
  }
#else /* ! (SYSV || linux) */
  {
    int real_uid = getuid();

#ifdef MINIX
    setuid(getuid());
#else
#if !defined(SVR4) && !defined(__NetBSD__) && !defined(__FreeBSD__)
    setruid(0);
#endif
    seteuid(real_uid);
#endif /* MINIX */
    findConfigFile(configPath, &configFile);
#ifdef MINIX
    /* no need to resture the uid to root */
#else
    seteuid(0);
#if !defined(SVR4) && !defined(__NetBSD__) && !defined(__FreeBSD__)
    setruid(real_uid);
#endif
#endif /* MINIX */
  }
#endif /* SYSV || linux */
  xf86Info.sharedMonitor = FALSE;
  xf86Info.kbdProc = NULL;
  xf86Info.mseProc = NULL;
  xf86Info.notrapSignals = FALSE;
  xf86Info.caughtSignal = FALSE;

  
  while ((token = getToken(TopLevelTab)) != EOF) {
    if (getToken(NULL) != STRING) configError("section name string expected");

    if ( StrCaseCmp(val.str, "files") == 0 ) {
      configFilesSection();
    } else if ( StrCaseCmp(val.str, "serverflags") == 0 ) {
      configServerFlagsSection();
    } else if ( StrCaseCmp(val.str, "keyboard") == 0 ) {
      configKeyboardSection();
    } else if ( StrCaseCmp(val.str, "pointer") == 0 ) {
      configPointerSection();
    } else if ( StrCaseCmp(val.str, "device") == 0 ) {
      configDeviceSection();
    } else if ( StrCaseCmp(val.str, "monitor") == 0 ) {
      configMonitorSection();
    } else if ( StrCaseCmp(val.str, "screen") == 0 ) {
      configScreenSection();
    } else {
      configError("not a recognized section name");
    }
  }

  fclose(configFile);
  Xfree(configBuf);
  Xfree(configRBuf);
  Xfree(configPath);

#if defined(SYSV) || defined(linux)
  if (getuid() != 0) {
    /* Wait for the child */
    wait(NULL);
  }
#endif
  
  /* Try XF86Config FontPath first */
  if (!xf86fpFlag)
    if (fontPath) {
      char *f = xf86ValidateFontPath(fontPath);
      if (*f)
        defaultFontPath = f;
      else
        ErrorF(
        "Warning: FontPath is completely invalid.  Using compiled-in default.\n"
              );
      xfree(fontPath);
      fontPath = (char *)NULL;
    }
    else
      ErrorF("Warning: No FontPath specified, using compiled-in default.\n");
  else    /* Use fontpath specified with '-fp' */
  {
    OFLG_CLR (XCONFIG_FONTPATH, &GenericXF86ConfigFlag);
    if (fontPath)
    {
      xfree(fontPath);
      fontPath = (char *)NULL;
    }
  }
  if (!fontPath) {
    /* xf86ValidateFontPath will write into it's arg, but defaultFontPath
       could be static, so we make a copy. */
    char *f = (char *)xalloc(strlen(defaultFontPath) + 1);
    f[0] = '\0';
    strcpy (f, defaultFontPath);
    defaultFontPath = xf86ValidateFontPath(f);
    xfree(f);
  }
  else
    xfree(fontPath);

  /* If defaultFontPath is still empty, exit here */

  if (! *defaultFontPath)
    FatalError("No valid FontPath could be found\n");
  if (xf86Verbose)
    ErrorF("%s FontPath set to \"%s\"\n", 
      OFLG_ISSET(XCONFIG_FONTPATH, &GenericXF86ConfigFlag) ? XCONFIG_GIVEN :
      XCONFIG_PROBED, defaultFontPath);

  if (!xf86Info.kbdProc)
    FatalError("You must specify a keyboard in XF86Config");
  if (!xf86Info.mseProc)
    FatalError("You must specify a mouse in XF86Config");

  if (!graphFound)
  {
    Bool needcomma = FALSE;

    ErrorF("\nYou must provide a \"Screen\" section in XF86Config for at\n");
    ErrorF("least one of the following graphics drivers: ");
    for (i = 0; i < xf86MaxScreens; i++)
    {
      if (xf86Screens[i])
      {
        ErrorF("%s%s", needcomma ? ", " : "",
               xf86TokenToString(DriverTab, xf86ScreenNames[i]));
        needcomma = TRUE;
      }
    }
    ErrorF("\n");
    FatalError("No configured graphics devices");
  }
 }
 else	/* if (vtopen) */
 {
  /*
   * Probe all configured screens for letting them resolve their modes
   */
  xf86ScreensOpen = TRUE;
  for ( i=0; i < xf86MaxScreens; i++ )
    if (xf86Screens[i] && xf86Screens[i]->configured &&
	(xf86Screens[i]->configured = (xf86Screens[i]->Probe)()))
      xf86InitViewport(xf86Screens[i]);

  /*
   * Now sort the drivers to match the order of the ScreenNumbers
   * requested by the user. (sorry, slow bubble-sort here)
   * Note, that after this sorting the first driver that is not configured
   * can be used as last-mark for all configured ones.
   */
  for ( j = 0; j < xf86MaxScreens-1; j++)
    for ( i=0; i < xf86MaxScreens-j-1; i++ )
      if (!xf86Screens[i] || !xf86Screens[i]->configured ||
	  (xf86Screens[i+1] && xf86Screens[i+1]->configured &&
	   (xf86Screens[i+1]->tmpIndex < xf86Screens[i]->tmpIndex)))
	{
	  ScrnInfoPtr temp = xf86Screens[i+1];
	  xf86Screens[i+1] = xf86Screens[i];
	  xf86Screens[i] = temp;
	}

 }
}
    
static void
configFilesSection()
{
  int            token;
  int            i, j;

  while ((token = getToken(FilesTab)) != ENDSECTION) {
    switch (token) {
    case FONTPATH:
      OFLG_SET(XCONFIG_FONTPATH,&GenericXF86ConfigFlag);
      if (getToken(NULL) != STRING) configError("Font path component expected");
      j = FALSE;
      if (fontPath == NULL)
	{
	  fontPath = (char *)xalloc(1);
	  fontPath[0] = '\0';
	  i = strlen(val.str) + 1;
	}
      else
	{
          i = strlen(fontPath) + strlen(val.str) + 1;
          if (fontPath[strlen(fontPath)-1] != ',') 
	    {
	      i++;
	      j = TRUE;
	    }
	}
      fontPath = (char *)xrealloc(fontPath, i);
      if (j)
        strcat(fontPath, ",");
      strcat(fontPath, val.str);
      xfree(val.str);
      break;
      
    case RGBPATH:
      OFLG_SET(XCONFIG_RGBPATH, &GenericXF86ConfigFlag);
      if (getToken(NULL) != STRING) configError("RGB path expected");
      if (!xf86coFlag)
        rgbPath = val.str;
      break;
    case EOF:
      FatalError("Unexpected EOF (missing EndSection?)");
      break; /* :-) */
    default:
      configError("File section keyword expected");
      break;
    }
  }
}

static void
configServerFlagsSection()
{
  int            token;
  int            i, j;
      
  xf86Info.dontZap       = FALSE;
  xf86Info.dontZoom      = FALSE;

  while ((token = getToken(ServerFlagsTab)) != ENDSECTION) {
    switch (token) {
    case NOTRAPSIGNALS:
      xf86Info.notrapSignals=TRUE;
      break;
    case DONTZAP:
      xf86Info.dontZap = TRUE;
      break;
    case DONTZOOM:
      xf86Info.dontZoom = TRUE;
      break;
    case EOF:
      FatalError("Unexpected EOF (missing EndSection?)");
      break; /* :-) */
    default:
      configError("Server flags section keyword expected");
      break;
    }
  }
}

static void
configKeyboardSection()
{
  int            token, ntoken;
  int            i, j;
      
  /* Initialize defaults */
  xf86Info.serverNumLock = FALSE;
  xf86Info.xleds         = 0L;
  xf86Info.kbdDelay      = 500;
  xf86Info.kbdRate       = 30;
  xf86Info.kbdProc       = (int (*)())NULL;
  xf86Info.vtinit        = NULL;
  xf86Info.vtSysreq      = VT_SYSREQ_DEFAULT;
  xf86Info.specialKeyMap = (int *)xalloc((RIGHTCTL - LEFTALT + 1) *
                                            sizeof(int));
  xf86Info.specialKeyMap[LEFTALT - LEFTALT] = KM_META;
  xf86Info.specialKeyMap[RIGHTALT - LEFTALT] = KM_META;
  xf86Info.specialKeyMap[SCROLLLOCK - LEFTALT] = KM_COMPOSE;
  xf86Info.specialKeyMap[RIGHTCTL - LEFTALT] = KM_CONTROL;

  while ((token = getToken(KeyboardTab)) != ENDSECTION) {
    switch (token) {
    case KPROTOCOL:
      if (getToken(NULL) != STRING)
        configError("Keyboard protocol name expected");
      if ( StrCaseCmp(val.str,"standard") == 0 ) {
         xf86Info.kbdProc    = xf86KbdProc;
#ifdef AMOEBA
         xf86Info.kbdEvents  = NULL;
#else
         xf86Info.kbdEvents  = xf86KbdEvents;
#endif
      } else if ( StrCaseCmp(val.str,"xqueue") == 0 ) {
#ifdef XQUEUE
        xf86Info.kbdProc   = xf86XqueKbdProc;
        xf86Info.kbdEvents = xf86XqueEvents;
        xf86Info.xqueSema  = 0;
        if (xf86Verbose)
          ErrorF("%s Xqueue selected for keyboard input\n",
  	         XCONFIG_GIVEN);
#endif
      } else {
        configError("Not a valid keyboard protocol name");
      }
      break;
    case AUTOREPEAT:
      if (getToken(NULL) != NUMBER) configError("Autorepeat delay expected");
      xf86Info.kbdDelay = val.num;
      if (getToken(NULL) != NUMBER) configError("Autorepeat rate expected");
      xf86Info.kbdRate = val.num;
      break;
    case SERVERNUM:
      xf86Info.serverNumLock = TRUE;
      break;

    case XLEDS:
      while ((token= getToken(NULL)) == NUMBER)
	xf86Info.xleds |= 1L << (val.num-1);
      pushToken = token;
      break;
    case LEFTALT:
    case RIGHTALT:
    case SCROLLLOCK:
    case RIGHTCTL:
      ntoken = getToken(KeyMapTab);
      if ((ntoken == EOF) || (ntoken == STRING) || (ntoken == NUMBER)) 
	configError("KeyMap type token expected");
      else {
	switch(ntoken) {
	case KM_META:
	case KM_COMPOSE:
	case KM_MODESHIFT:
	case KM_MODELOCK:
	case KM_SCROLLLOCK:
	case KM_CONTROL:
          xf86Info.specialKeyMap[token - LEFTALT] = ntoken;
	  break;
	default:
	  configError("Illegal KeyMap type");
	  break;
	}
      }
      break;
    case VTINIT:
      if (getToken(NULL) != STRING) configError("VTInit string expected");
      xf86Info.vtinit = val.str;
      if (xf86Verbose)
        ErrorF("%s VTInit: \"%s\"\n", XCONFIG_GIVEN, val.str);
      break;

    case VTSYSREQ:
#ifdef USE_VT_SYSREQ
      xf86Info.vtSysreq = TRUE;
      if (xf86Verbose && !VT_SYSREQ_DEFAULT)
        ErrorF("%s VTSysReq enabled\n", XCONFIG_GIVEN);
#else
      configError("VTSysReq not supported on this OS");
#endif
      break;

    case EOF:
      FatalError("Unexpected EOF (missing EndSection?)");
      break; /* :-) */

    default:
      configError("Keyboard section keyword expected");
      break;
    }
  }
  if (xf86Info.kbdProc == (int (*)())NULL)
  {
    configError("No keyboard device given");
  }
}
      
static void
configPointerSection()
{
  int            token;
  int		 mtoken;
  int            i, j;
  char *mouseType = "unknown";

  /* Set defaults */
  xf86Info.baudRate        = 1200;
  xf86Info.oldBaudRate     = -1;
  xf86Info.sampleRate      = 0;
  xf86Info.emulate3Buttons = FALSE;
  xf86Info.chordMiddle     = FALSE;
  xf86Info.mouseFlags      = 0;
  xf86Info.mseProc         = (int (*)())NULL;
  xf86Info.mseDevice       = NULL;
  xf86Info.mseType         = -1;
      
  while ((token = getToken(PointerTab)) != ENDSECTION) {
    switch (token) {

    case PROTOCOL:
      if (getToken(NULL) != STRING) configError("Mouse name expected");
#ifdef USE_OSMOUSE
      if ( StrCaseCmp(val.str,"osmouse") == 0 ) {
        if (xf86Verbose)
          ErrorF("%s OsMouse selected for mouse input\n", XCONFIG_GIVEN);
        /*
         *  allow an option to be passed to the OsMouse routines
         */
        if ((i = getToken(NULL)) != ERROR_TOKEN)
    	  xf86OsMouseOption(i, (pointer) &val);
        else
  	  pushToken = i;
        xf86Info.mseProc   = xf86OsMouseProc;
        xf86Info.mseEvents = xf86OsMouseEvents;
	break;
      }
#endif
#ifdef XQUEUE
      if ( StrCaseCmp(val.str,"xqueue") == 0 ) {
        xf86Info.mseProc   = xf86XqueMseProc;
        xf86Info.mseEvents = xf86XqueEvents;
        xf86Info.xqueSema  = 0;
        if (xf86Verbose)
          ErrorF("%s Xqueue selected for mouse input\n",
	         XCONFIG_GIVEN);
        break;
      }
#endif

#if defined(MACH) || defined(AMOEBA)
      mouseType = (char *) xalloc (strlen (val.str) + 1);
      strcpy (mouseType, val.str);
#else
      mouseType = (char *)strdup(val.str); /* GJA -- should we free this? */
#endif
      mtoken = getStringToken(MouseTab); /* Which mouse? */
#ifdef AMOEBA
      xf86Info.mseProc    = xf86MseProc;
      xf86Info.mseEvents  = NULL;
#else
      xf86Info.mseProc    = xf86MseProc;
      xf86Info.mseEvents  = xf86MseEvents;
#endif
      xf86Info.mseType    = mtoken - MICROSOFT;
      if (!xf86MouseSupported(xf86Info.mseType))
      {
        configError("Mouse type not supported by this OS");
      }
#ifdef MACH386
      /* Don't need to specify the device for MACH -- should always be this */
      xf86Info.mseDevice  = "/dev/mouse";
#endif
      break;
    case PDEVICE:
      if (getToken(NULL) != STRING) configError("Mouse device expected");
      xf86Info.mseDevice  = val.str;
      break;
    case BAUDRATE:
      if (getToken(NULL) != NUMBER) configError("Baudrate expected");
      if (xf86Info.mseType + MICROSOFT == LOGIMAN)
	{
	  /*
	   * XXXX This should be extended to other mouse types -- most
	   * support only 1200.  Should also disallow baudrate for bus mice
	   */
	  /* Moan if illegal baud rate!  [CHRIS-211092] */
	  if ((val.num != 1200) && (val.num != 9600))
	    configError("Only 1200 or 9600 Baud are supported by MouseMan");
	}
      xf86Info.baudRate = val.num;
      break;

    case SAMPLERATE:
      if (getToken(NULL) != NUMBER) configError("Sample rate expected");
      if (xf86Info.mseType + MICROSOFT == LOGIMAN)
	{
	  /* XXXX Most mice don't allow this */
	  /* Moan about illegal sample rate!  [CHRIS-211092] */
	  configError("Selection of sample rate is not supported by MouseMan");
	}
      xf86Info.sampleRate = val.num;
      break;

    case EMULATE3:
      if (xf86Info.chordMiddle)
        configError("Can't use Emulate3Buttons with ChordMiddle");
      xf86Info.emulate3Buttons = TRUE;
      break;

    case CHORDMIDDLE:
      if (xf86Info.mseType + MICROSOFT == MICROSOFT ||
          xf86Info.mseType + MICROSOFT == LOGIMAN)
      {
        if (xf86Info.emulate3Buttons)
          configError("Can't use ChordMiddle with Emulate3Buttons");
        xf86Info.chordMiddle = TRUE;
      }
      else
        configError("ChordMiddle is only supported for Microsoft and Logiman");
      break;

    case CLEARDTR:
#ifdef CLEARDTR_SUPPORT
      if (xf86Info.mseType + MICROSOFT == MOUSESYS)
        xf86Info.mouseFlags |= MF_CLEAR_DTR;
      else
        configError("ClearDTR only supported for MouseSystems mouse");
#else
      configError("ClearDTR not supported on this OS");
#endif
      break;
    case CLEARRTS:
#ifdef CLEARDTR_SUPPORT
      if (xf86Info.mseType + MICROSOFT == MOUSESYS)
        xf86Info.mouseFlags |= MF_CLEAR_RTS;
      else
        configError("ClearRTS only supported for MouseSystems mouse");
#else
      configError("ClearRTS not supported on this OS");
#endif
      break;
    case EOF:
      FatalError("Unexpected EOF (missing EndSection?)");
      break; /* :-) */
      
    default:
      configError("Pointer section keyword expected");
      break;
    }

  }
  /* Print log and make sanity checks */

  if (xf86Info.mseProc == (int (*)())NULL)
  {
    configError("No mouse protocol given");
  }
  
  /*
   * if mseProc is set and mseType isn't, then using Xqueue or OSmouse.
   * Otherwise, a mouse device is required.
   */
  if (xf86Info.mseType >= 0 && !xf86Info.mseDevice)
  {
    configError("No mouse device given");
  }

  if (xf86Verbose && xf86Info.mseType >= 0)
  {
    Bool formatFlag = FALSE;
    ErrorF("%s Mouse: type: %s, device: %s", 
       XCONFIG_GIVEN, mouseType, xf86Info.mseDevice);
    if (token != BUSMOUSE && token != PS_2)
    {
      formatFlag = TRUE;
      ErrorF(", baudrate: %d", xf86Info.baudRate);
    }
    if (xf86Info.sampleRate)
    {
      ErrorF("%ssamplerate: %d", formatFlag ? ",\n       " : ", ",
             xf86Info.sampleRate);
      formatFlag = !formatFlag;
    }
    if (xf86Info.emulate3Buttons)
      ErrorF("%s3 button emulation", formatFlag ? ",\n       " : ", ");
    if (xf86Info.chordMiddle)
      ErrorF("%sChorded middle button", formatFlag ? ",\n       " : ", ");
    ErrorF("\n");
  }
}
      
static int scr_index = 0; 

static int n_devices = 0;
static GDevPtr device_list = NULL;

static void
configDeviceSection()
{
  int            token;
  int            i, j;
  GDevPtr devp;

  /* Allocate one more device */
  if ( device_list == NULL ) {
    device_list = (GDevPtr) xalloc(sizeof(GDevRec));
  } else {
    device_list = (GDevPtr) xrealloc(device_list,
				     (n_devices+1) * sizeof(GDevRec));
  }
  devp = &(device_list[n_devices]); /* Point to the last device */
  n_devices++; 
  
  /* Pre-init the newly created device */
  devp->chipset = NULL;
  devp->ramdac = NULL;
  devp->dacSpeed = 0;
  OFLG_ZERO(&(devp->options));
  OFLG_ZERO(&(devp->xconfigFlag));
  devp->videoRam = 0;
  devp->speedup = SPEEDUP_DEFAULT;
  devp->clocks = 0;
  devp->clockprog = NULL;
  devp->textClockValue = -1;
  /* GJA -- We initialize the following fields to known values.
   * If later on we find they contain different values,
   * they might be interesting to print.
   */
  devp->IObase = 0;
  devp->DACbase = 0;
  devp->COPbase = 0;
  devp->POSbase = 0;
  devp->instance = 0;
  devp->BIOSbase = 0;
  devp->MemBase = 0;
  devp->s3Madjust = 0;
  devp->s3Nadjust = 0;

  while ((token = getToken(DeviceTab)) != ENDSECTION) {
    switch (token) {

    case IDENTIFIER:
      if (getToken(NULL) != STRING) configError("identifier name expected");
      devp->identifier = val.str;
      break;

    case VENDOR:
      if (getToken(NULL) != STRING) configError("vendor name expected");
      devp->vendor = val.str;
      break;

    case BOARD:
      if (getToken(NULL) != STRING) configError("board name expected");
      devp->board = val.str;
      break;

    case CHIPSET:
      if (getToken(NULL) != STRING) configError("Chipset string expected");
      devp->chipset = val.str;
      OFLG_SET(XCONFIG_CHIPSET,&(devp->xconfigFlag));
      break;

    case RAMDAC:
      if (getToken(NULL) != STRING) configError("RAMDAC string expected");
      devp->ramdac = val.str;
      OFLG_SET(XCONFIG_RAMDAC,&(devp->xconfigFlag));
      break;

    case DACSPEED:
      if (getToken(NULL) != NUMBER) configError("DAC speed expected");
      devp->dacSpeed = (int)(val.realnum * 1000.0 + 0.5);
      OFLG_SET(XCONFIG_DACSPEED,&(devp->xconfigFlag));
      break;

    case CLOCKCHIP:
       /* Only allow one Clock string */
       if (OFLG_ISSET(CLOCK_OPTION_PROGRAMABLE, &(devp->clockOptions)))
       {
	  configError("Only one Clock chip may be specified.");
	  break;
       }
       if (devp->clocks == 0)
       {
          if (getToken(NULL) != STRING) configError("Option string expected");
	  i = 0;
	  while (xf86_ClockOptionTab[i].token != -1)
	  {
	     if (StrCaseCmp(val.str, xf86_ClockOptionTab[i].name) == 0)
	     {
		OFLG_SET(CLOCK_OPTION_PROGRAMABLE, &(devp->clockOptions));
		OFLG_SET(xf86_ClockOptionTab[i].token,
			 &(devp->clockOptions));
		break;
	     }
	     i++;
	  }
	  if (xf86_ClockOptionTab[i].token == -1) {
	     configError("Unknown clock chip");
	     break;
	  }
       }
       else
       {
	  configError("Clocks previously specified by value");
       }
       break;

    case CLOCKS:
      OFLG_SET(XCONFIG_CLOCKS,&(devp->xconfigFlag));
      if ((token = getToken(NULL)) == STRING)
      {
	 configError("Use ClockChip to specify a programmable clock");
	 break;
      }
      if (OFLG_ISSET(CLOCK_OPTION_PROGRAMABLE, &(devp->clockOptions)))
      {
	 configError("Clock previously specified as programmable");
	 break;
      }
      for (i = devp->clocks; token == NUMBER && i < MAXCLOCKS; i++) {
	devp->clock[i] = (int)(val.realnum * 1000.0 + 0.5);
	token = getToken(NULL);
      }

      devp->clocks = i;
      pushToken = token;
      break;

    case OPTION:
      if (getToken(NULL) != STRING) configError("Option string expected");
      i = 0;
      while (xf86_OptionTab[i].token != -1) 
      {
	if (StrCaseCmp(val.str, xf86_OptionTab[i].name) == 0)
	{
          OFLG_SET(xf86_OptionTab[i].token, &(devp->options));
	  break;
	}
	i++;
      }
      if (xf86_OptionTab[i].token == -1)
        configError("Unknown option string");
      break;

    case VIDEORAM:
      OFLG_SET(XCONFIG_VIDEORAM,&(devp->xconfigFlag));
      if (getToken(NULL) != NUMBER) configError("Video RAM size expected");
      devp->videoRam = val.num;
      break;

    case SPEEDUP:
      OFLG_SET(XCONFIG_SPEEDUP,&(devp->xconfigFlag));
      if ((token = getToken(NULL)) == STRING)
	if (!strcmp(val.str,"all"))
	  devp->speedup = SPEEDUP_ALL;
	else
	  if (!strcmp(val.str,"best"))
	    devp->speedup = SPEEDUP_BEST;
	  else
	    if (!strcmp(val.str,"none"))
	      devp->speedup = 0;
            else
	      configError("Unrecognised SpeedUp option");
      else
      {
        pushToken = token;
	if ((token = getToken(NULL)) == NUMBER)
	  devp->speedup = val.num;
	else
	{
	  pushToken = token;
	  devp->speedup = SPEEDUP_ALL;
	}
      }
      break;

    case NOSPEEDUP:
      OFLG_SET(XCONFIG_SPEEDUP,&(devp->xconfigFlag));
      devp->speedup = 0;
      break;

    case CLOCKPROG:
      if (getToken(NULL) != STRING) configError("ClockProg string expected");
      if (val.str[0] != '/')
        FatalError("Full pathname must be given for ClockProg \"%s\"\n",
                   val.str);
      if (access(val.str, X_OK) < 0)
      {
        if (access(val.str, F_OK) < 0)
          FatalError("ClockProg \"%s\" does not exist\n", val.str);
        else
          FatalError("ClockProg \"%s\" is not executable\n", val.str);
      }
      {
        struct stat stat_buf;
        stat(val.str, &stat_buf);
	if (!CHECK_TYPE(stat_buf.st_mode, S_IFREG))
          FatalError("ClockProg \"%s\" is not a regular file\n", val.str);
      }
      devp->clockprog = val.str;
      if (getToken(NULL) == NUMBER)
      {
        devp->textClockValue = (int)(val.realnum * 1000.0 + 0.5);
      }
      else
      {
        pushToken = token;
      }
      break;

    case BIOSBASE:
      if (getToken(NULL) != NUMBER) configError("BIOS base address expected");
      devp->BIOSbase = val.num;
      OFLG_SET(XCONFIG_BIOSBASE, &(devp->xconfigFlag));
      break;

    case MEMBASE:
      if (getToken(NULL) != NUMBER) configError("Memory base address expected");
      devp->MemBase = val.num;
      OFLG_SET(XCONFIG_MEMBASE, &(devp->xconfigFlag));
      break;

    case IOBASE:
      if (getToken(NULL) != NUMBER)
        configError("Direct access register I/O base address expected");
      devp->IObase = val.num;
      OFLG_SET(XCONFIG_IOBASE, &(devp->xconfigFlag));
      break;

    case DACBASE:
      if (getToken(NULL) != NUMBER)
        configError("DAC base I/O address expected");
      devp->DACbase = val.num;
      OFLG_SET(XCONFIG_DACBASE, &(devp->xconfigFlag));
      break;

    case COPBASE:
      if (getToken(NULL) != NUMBER)
        configError("Coprocessor base memory address expected");
      devp->COPbase = val.num;
      OFLG_SET(XCONFIG_COPBASE, &(devp->xconfigFlag));
      break;

    case POSBASE:
      if (getToken(NULL) != NUMBER) configError("POS base address expected");
      devp->POSbase = val.num;
      OFLG_SET(XCONFIG_POSBASE, &(devp->xconfigFlag));
      break;

    case INSTANCE:
      if (getToken(NULL) != NUMBER)
        configError("Video adapter instance number expected");
      devp->instance = val.num;
      OFLG_SET(XCONFIG_INSTANCE, &(devp->xconfigFlag));
      break;

    case S3MNADJUST:
      if ((token = getToken(NULL)) == DASH) {  /* negative number */
	 token = getToken(NULL);
	 val.num = -val.num;
      }
      if (token != NUMBER || val.num<-31 || val.num>31) 
	 configError("M adjust (max. 31) expected");
        devp->s3Madjust = val.num;

      if ((token = getToken(NULL)) == DASH) {  /* negative number */
	 token = getToken(NULL);
	 val.num = -val.num;
      }
      if (token == NUMBER) {
	 if (val.num<-255 || val.num>255) 
	    configError("N adjust (max. 255) expected");
	 else
	    devp->s3Nadjust = val.num;
      }
      else pushToken = token;
      break;

    case S3MCLK:
      if (getToken(NULL) != NUMBER) configError("MCLK value in MHz expected");
      devp->s3MClk = (int)(val.realnum * 1000.0 + 0.5);
      break;

    case EOF:
      FatalError("Unexpected EOF (missing EndSection?)");
      break; /* :-) */
    default:
      configError("Device section keyword expected");
      break;
    }
  }
}

static int n_monitors = 0;
static MonPtr monitor_list = NULL;

static void
configMonitorSection()
{
  int            token;
  int            i, j;
  MonPtr monp;
  float multiplier;
      
  /* Allocate one more monitor */
  if ( monitor_list == NULL ) {
    monitor_list = (MonPtr) xalloc(sizeof(MonRec));
  } else {
    monitor_list = (MonPtr) xrealloc(monitor_list,
				     (n_monitors+1) * sizeof(MonRec));
  }
  monp = &(monitor_list[n_monitors]); /* Point to the new monitor */
  monp->Modes = 0;
  monp->Last = 0;
  monp->n_hsync = 0;
  monp->n_vrefresh = 0;
  n_monitors++; 
  
  while ((token = getToken(MonitorTab)) != ENDSECTION) {
    switch (token) {
    case IDENTIFIER:
      if (getToken(NULL) != STRING) configError("identifier name expected");
      monp->id = val.str;
      break;
    case VENDOR:
      if (getToken(NULL) != STRING) configError("vendor name expected");
      monp->vendor = val.str;
      break;
    case MODEL:
      if (getToken(NULL) != STRING) configError("model name expected");
      monp->model = val.str;
      break;
    case MODE:
      readVerboseMode(monp);
      break;
    case MODELINE:
      token = getToken(NULL);
      pNew = (DisplayModePtr)xalloc(sizeof(DisplayModeRec));

      if (monp->Last) 
         monp->Last->next = pNew;
      else
        monp->Modes = pNew;
          
      if (token == STRING)
        {
          pNew->name = val.str;
          if ((token = getToken(NULL)) != NUMBER)
            FatalError("Dotclock expected");
        }
      else if (monp->Last)
        {
#if defined(MACH) || defined(AMOEBA)
          pNew->name = (char *) xalloc (strlen (monp->Last->name) + 1);
          strcpy (pNew->name, monp->Last->name);
#else
          pNew->name = (char *)strdup(monp->Last->name);
#endif
        }
      else
        configError("Mode name expected");

      pNew->next = NULL;
      pNew->prev = NULL;
      pNew->Flags = 0;
      pNew->Clock = (int)(val.realnum * 1000.0 + 0.5);
      pNew->CrtcHAdjusted = FALSE;
      pNew->CrtcVAdjusted = FALSE;
      
      if (getToken(NULL) == NUMBER)
	pNew->CrtcHDisplay = pNew->HDisplay = val.num;
      else configError("Horizontal display expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcHSyncStart = pNew->HSyncStart = val.num;
      else configError("Horizontal sync start expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcHSyncEnd = pNew->HSyncEnd = val.num;
      else configError("Horizontal sync end expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcHTotal = pNew->HTotal = val.num;
      else configError("Horizontal total expected");
          
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcVDisplay = pNew->VDisplay = val.num;
      else configError("Vertical display expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcVSyncStart = pNew->VSyncStart = val.num;
      else configError("Vertical sync start expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcVSyncEnd = pNew->VSyncEnd = val.num;
      else configError("Vertical sync end expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcVTotal = pNew->VTotal = val.num;
      else configError("Vertical total expected");

      token = getToken(TimingTab);
      while ( (token == TT_INTERLACE) || (token == TT_PHSYNC) ||
              (token == TT_NHSYNC) || (token == TT_PVSYNC) ||
              (token == TT_NVSYNC) || (token == TT_CSYNC) ||
              (token == TT_PCSYNC) || (token == TT_NCSYNC) ||
              (token == TT_DBLSCAN) )
      {
        switch(token) {
              
        case TT_INTERLACE: pNew->Flags |= V_INTERLACE;  break;
        case TT_PHSYNC:    pNew->Flags |= V_PHSYNC;     break;
        case TT_NHSYNC:    pNew->Flags |= V_NHSYNC;     break;
        case TT_PVSYNC:    pNew->Flags |= V_PVSYNC;     break;
        case TT_NVSYNC:    pNew->Flags |= V_NVSYNC;     break;
        case TT_CSYNC:     pNew->Flags |= V_CSYNC;      break;
        case TT_PCSYNC:    pNew->Flags |= V_PCSYNC;     break;
        case TT_NCSYNC:    pNew->Flags |= V_NCSYNC;     break;
        case TT_DBLSCAN:   pNew->Flags |= V_DBLSCAN;    break;
        default:
          configError("bug found in config reader"); break;
        }
        token = getToken(TimingTab);
      }
      pushToken = token;
      monp->Last = pNew; /* GJA */
      break;
    case BANDWIDTH:
      if ((token = getToken(NULL)) != NUMBER)
        configError("Bandwidth number expected");
      monp->bandwidth = val.realnum;
      /* Handle optional scaler */
      token = getToken(UnitTab);
      switch ( token ) {
      case HRZ: multiplier = 1.0e-6; break;
      case KHZ: multiplier = 1.0e-3; break;
      case MHZ: multiplier = 1.0; break;
      default: multiplier = 1.0; pushToken = token;
      }
      monp->bandwidth *= multiplier;
      break;
    case HORIZSYNC:
      if ((token = getToken(NULL)) != NUMBER)
        configError("Horizontal sync value expected");
      monp->hsync[monp->n_hsync].lo = val.realnum;
      if ((token = getToken(NULL)) == DASH) {
        if ((token = getToken(NULL)) != NUMBER)
           configError("Upperbound for horizontal sync value expected");
           monp->hsync[monp->n_hsync].hi = val.realnum;
      } else {
           pushToken = token;
           monp->hsync[monp->n_hsync].hi = monp->hsync[monp->n_hsync].lo;
      }
      monp->n_hsync++;
      while ( (token = getToken(NULL)) == COMMA ) {
        if ( monp->n_hsync == MAX_HSYNC )
           configError("Sorry. Too many horizontal sync intervals.");

        if ((token = getToken(NULL)) != NUMBER)
          configError("Horizontal sync value expected");
        monp->hsync[monp->n_hsync].lo = val.realnum;
        if ((token = getToken(NULL)) == DASH) {
          if ((token = getToken(NULL)) != NUMBER)
             configError("Upperbound for horizontal sync value expected");
             monp->hsync[monp->n_hsync].hi = val.realnum;
        } else {
          pushToken = token;
          monp->hsync[monp->n_hsync].hi = monp->hsync[monp->n_hsync].lo;
        }
        monp->n_hsync++;
      }
      pushToken = token;
      /* Handle optional scaler */
      token = getToken(UnitTab);
      switch ( token ) {
      case HRZ: multiplier = 1.0e-3; break;
      case KHZ: multiplier = 1.0; break;
      case MHZ: multiplier = 1.0e3; break;
      default: multiplier = 1.0; pushToken = token;
      }
      for ( i = 0 ; i < monp->n_hsync ; i++ ) {
         monp->hsync[i].hi *= multiplier;
         monp->hsync[i].lo *= multiplier;
      }
      break;
    case VERTREFRESH:
      if ((token = getToken(NULL)) != NUMBER)
        configError("Vertical refresh value expected");
      monp->vrefresh[monp->n_vrefresh].lo = val.realnum;
      if ((token = getToken(NULL)) == DASH) {
        if ((token = getToken(NULL)) != NUMBER)
          configError("Upperbound for vertical refresh value expected");
        monp->vrefresh[monp->n_vrefresh].hi = val.realnum;
      } else {
        monp->vrefresh[monp->n_vrefresh].hi =
            monp->vrefresh[monp->n_vrefresh].lo;
        pushToken = token;
      }
      monp->n_vrefresh++;
      while ( (token = getToken(NULL)) == COMMA ) {
        if ( monp->n_vrefresh == MAX_HSYNC )
          configError("Sorry. Too many vertical refresh intervals.");

        if ((token = getToken(NULL)) != NUMBER)
          configError("Vertical refresh value expected");
        monp->vrefresh[monp->n_vrefresh].lo = val.realnum;
        if ((token = getToken(NULL)) == DASH) {
          if ((token = getToken(NULL)) != NUMBER)
            configError("Upperbound for vertical refresh value expected");
          monp->vrefresh[monp->n_vrefresh].hi = val.realnum;
        } else {
          monp->vrefresh[monp->n_vrefresh].hi =
              monp->vrefresh[monp->n_vrefresh].lo;
          pushToken = token;
        }
        monp->n_vrefresh++;
      }
      pushToken = token;
      /* Handle optional scaler */
      token = getToken(UnitTab);
      switch ( token ) {
      case HRZ: multiplier = 1.0; break;
      case KHZ: multiplier = 1.0e3; break;
      case MHZ: multiplier = 1.0e6; break;
      default: multiplier = 1.0; pushToken = token;
      }
      for ( i = 0 ; i < monp->n_vrefresh ; i++ ) {
         monp->vrefresh[i].hi *= multiplier;
         monp->vrefresh[i].lo *= multiplier;
      }
      break;
    case GAMMA: {
       char *msg = "gamma correction value(s) expected\n either one value or three r/g/b values with 0.1 <= gamma <= 10";
       if ((token = getToken(NULL)) != NUMBER || val.realnum<0.1 || val.realnum>10)
	  configError(msg);
       else {
	  int i;
	  extern double xf86rGamma, xf86gGamma, xf86bGamma;
	  extern unsigned char xf86rGammaMap[], xf86gGammaMap[], xf86bGammaMap[];
	  xf86rGamma = xf86gGamma = xf86bGamma = 1.0 / val.realnum;
	  if ((token = getToken(NULL)) == NUMBER) {
	     if (val.realnum<0.1 || val.realnum>10) configError(msg);
	     else {
		xf86gGamma = 1.0 / val.realnum;
		if ((token = getToken(NULL)) != NUMBER || val.realnum<0.1 || val.realnum>10)
		   configError(msg);
		else {
		   xf86bGamma = 1.0 / val.realnum;
		}
	     }
	  }
	  else pushToken = token;
       }
       break;
    }
    case EOF:
      FatalError("Unexpected EOF. Missing EndSection?");
      break; /* :-) */

    default:
      configError("Monitor section keyword expected");
      break;
    }
  }
#if 0
  /* DHD -- do this later so errors only show up for the monitor in use */
  /* GJA -- check the modes against the monitor specs. */
  monp->Modes = xf86PruneModes(monp,monp->Modes);
#endif
}

static void
readVerboseMode(monp)
MonPtr monp;
{
  int token, token2;
  int had_dotclock = 0, had_htimings = 0, had_vtimings = 0;

  pNew = (DisplayModePtr)xalloc(sizeof(DisplayModeRec));
  pNew->next = NULL;
  pNew->prev = NULL;
  pNew->Flags = 0;
  pNew->HDisplay = pNew->VDisplay = 0; /* Uninitialized */
  pNew->CrtcHAdjusted = pNew->CrtcVAdjusted = FALSE;

  if (monp->Last) 
    monp->Last->next = pNew;
  else
    monp->Modes = pNew;
  monp->Last = pNew;

  if ( getToken(NULL) != STRING ) {
    FatalError("Mode name expected");
  }
  pNew->name = val.str;
  while ((token = getToken(ModeTab)) != ENDMODE) {
    switch (token) {
    case DOTCLOCK:
      if ((token = getToken(NULL)) != NUMBER) {
        FatalError("Dotclock expected");
      }
      pNew->Clock = (int)(val.realnum * 1000.0 + 0.5);
      had_dotclock = 1;
      break;
    case HTIMINGS:
      if (getToken(NULL) == NUMBER)
	pNew->CrtcHDisplay = pNew->HDisplay = val.num;
      else configError("Horizontal display expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcHSyncStart = pNew->HSyncStart = val.num;
      else configError("Horizontal sync start expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcHSyncEnd = pNew->HSyncEnd = val.num;
      else configError("Horizontal sync end expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcHTotal = pNew->HTotal = val.num;
      else configError("Horizontal total expected");
      had_htimings = 1;
      break;
    case VTIMINGS:
      if (getToken(NULL) == NUMBER)
	pNew->CrtcVDisplay = pNew->VDisplay = val.num;
      else configError("Vertical display expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcVSyncStart = pNew->VSyncStart = val.num;
      else configError("Vertical sync start expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcVSyncEnd = pNew->VSyncEnd = val.num;
      else configError("Vertical sync end expected");
          
      if (getToken(NULL) == NUMBER)
	pNew->CrtcVTotal = pNew->VTotal = val.num;
      else configError("Vertical total expected");
      had_vtimings = 1;
      break;
    case FLAGS:
      token = getToken(NULL);
      if (token != STRING)
        configError("Flag string expected.  Note: flags must be in \"\"");
      while ( token == STRING ) {
        token2 = getStringToken(TimingTab);
        switch(token2) {
        case TT_INTERLACE: pNew->Flags |= V_INTERLACE;  break;
        case TT_PHSYNC:    pNew->Flags |= V_PHSYNC;     break;
        case TT_NHSYNC:    pNew->Flags |= V_NHSYNC;     break;
        case TT_PVSYNC:    pNew->Flags |= V_PVSYNC;     break;
        case TT_NVSYNC:    pNew->Flags |= V_NVSYNC;     break;
        case TT_CSYNC:     pNew->Flags |= V_CSYNC;      break;
        case TT_PCSYNC:    pNew->Flags |= V_PCSYNC;     break;
        case TT_NCSYNC:    pNew->Flags |= V_NCSYNC;     break;
        case TT_DBLSCAN:   pNew->Flags |= V_DBLSCAN;    break;
        default:
          configError("Unknown flag string"); break;
        }
        token = getToken(NULL);
      }
      pushToken = token;
      break;
    }
  }
  if ( !had_dotclock ) configError("the dotclock is missing");
  if ( !had_htimings ) configError("the horizontal timings are missing");
  if ( !had_vtimings ) configError("the vertical timings are missing");
}

static Bool dummy;

static void
configScreenSection()
{
  int i, j;
  int driverno;
  int had_monitor = 0, had_device = 0;
  int dispIndex = 0;
  int numDisps = 0;
  DispPtr dispList = NULL;
  DispPtr dispp;
      
  int token;
  DisplayModePtr pNew, pLast;
  ScrnInfoPtr screen = NULL;
  int textClockValue = -1;

  token = getToken(ScreenTab);
  if ( token != DRIVER )
	configError("The screen section must begin with the 'driver' line");

  if (getToken(NULL) != STRING) configError("Driver name expected");
  driverno = getStringToken(DriverTab);
  switch ( driverno ) {
  case SVGA:
  case VGA2:
  case MONO:
  case VGA16:
  case ACCEL:
	break;
  default:
    configError("Not a recognized driver name");
  }
  scr_index = getScreenIndex(driverno);

  dummy = scr_index < 0 || !xf86Screens[scr_index];
  if (dummy)
    screen = (ScrnInfoPtr)xalloc(sizeof(ScrnInfoRec));
  else
  {
    screen = xf86Screens[scr_index];
    screen->configured = TRUE;
    screen->tmpIndex = screenno++;
    screen->scrnIndex = scr_index;	/* scrnIndex must not be changed */
    screen->frameX0 = -1;
    screen->frameY0 = -1;
    screen->virtualX = -1;
    screen->virtualY = -1;
    screen->defaultVisual = -1;
    screen->modes = NULL;
    screen->width = 240;
    screen->height = 180;
    screen->bankedMono = FALSE;
    screen->textclock = -1;
    screen->blackColour.red = 0;
    screen->blackColour.green = 0;
    screen->blackColour.blue = 0;
    screen->whiteColour.red = 0x3F;
    screen->whiteColour.green = 0x3F;
    screen->whiteColour.blue = 0x3F;
  }
  screen->clocks = 0;

  while ((token = getToken(ScreenTab)) != ENDSECTION) {
    switch (token) {

    case SCREENNO:
      if (getToken(NULL) != NUMBER) configError("Screen number expected");
      screen->tmpIndex = val.num;
      break;

    case SUBSECTION:
      if ((getToken(NULL) != STRING) || (StrCaseCmp(val.str, "display") != 0)) {
        configError("You must say \"Display\" here");
      }
      if (dispList == NULL) {
        dispList = (DispPtr)xalloc(sizeof(DispRec));
      } else {
        dispList = (DispPtr)xrealloc(dispList,
				     (numDisps + 1) * sizeof(DispRec));
      }
      dispp = dispList + numDisps;
      numDisps++;
      dispp->depth = -1;
      dispp->weight.red = dispp->weight.green = dispp->weight.blue = 0;
      dispp->frameX0 = -1;
      dispp->frameY0 = -1;
      dispp->virtualX = -1;
      dispp->virtualY = -1;
      dispp->modes = NULL;
      dispp->whiteColour.red = dispp->whiteColour.green = 
                               dispp->whiteColour.blue = 0x3F;
      dispp->blackColour.red = dispp->blackColour.green = 
                               dispp->blackColour.blue = 0;
      dispp->defaultVisual = -1;
      OFLG_ZERO(&(dispp->options));
      OFLG_ZERO(&(dispp->xconfigFlag));

      configDisplaySubsection(dispp);
      break;

    case EOF:
      FatalError("Unexpected EOF (missing EndSection?)");
      break; /* :-) */

    case MDEVICE:
      if (getToken(NULL) != STRING) configError("Device name expected");
      for ( i = 0 ; i < n_devices ; i++ ) {
        if ( strcmp(device_list[i].identifier,val.str) == 0 ) {
          /* Copy back */
          if (!dummy && xf86Verbose) {
            ErrorF("%s %s: Graphics device ID: \"%s\"\n",
                   XCONFIG_GIVEN, screen->name, device_list[i].identifier);
          }
          screen->clocks = device_list[i].clocks;
          for ( j = 0 ; j < MAXCLOCKS ; j++ ) {
             screen->clock[j] = device_list[i].clock[j];
          }
          screen->chipset = device_list[i].chipset;
          screen->ramdac = device_list[i].ramdac;
          screen->dacSpeed = device_list[i].dacSpeed;
          screen->options = device_list[i].options;
          screen->clockOptions = device_list[i].clockOptions;
          screen->xconfigFlag = device_list[i].xconfigFlag;
          screen->videoRam = device_list[i].videoRam;
          screen->speedup = device_list[i].speedup;
          screen->clockprog = device_list[i].clockprog;
          textClockValue = device_list[i].textClockValue;
          if (OFLG_ISSET(XCONFIG_BIOSBASE, &screen->xconfigFlag))
            screen->BIOSbase = device_list[i].BIOSbase;
          if (OFLG_ISSET(XCONFIG_MEMBASE, &screen->xconfigFlag))
            screen->MemBase = device_list[i].MemBase;
          if (OFLG_ISSET(XCONFIG_IOBASE, &screen->xconfigFlag))
            screen->IObase = device_list[i].IObase;
          if (OFLG_ISSET(XCONFIG_DACBASE, &screen->xconfigFlag))
            screen->DACbase = device_list[i].DACbase;
          if (OFLG_ISSET(XCONFIG_COPBASE, &screen->xconfigFlag))
            screen->COPbase = device_list[i].COPbase;
          if (OFLG_ISSET(XCONFIG_POSBASE, &screen->xconfigFlag))
            screen->POSbase = device_list[i].POSbase;
          if (OFLG_ISSET(XCONFIG_INSTANCE, &screen->xconfigFlag))
            screen->instance = device_list[i].instance;
          screen->s3Madjust = device_list[i].s3Madjust;
          screen->s3Nadjust = device_list[i].s3Nadjust;
          break;
        }
      }
      if ( i == n_devices ) { /* Exhausted the device list */
         configError("Not a declared device");
      }
      had_device = 1;
      break;
      
    case MONITOR:
      if (getToken(NULL) != STRING) configError("Monitor name expected");
      for ( i = 0 ; i < n_monitors ; i++ ) {
        if ( strcmp(monitor_list[i].id,val.str) == 0 ) {
          if (!dummy && xf86Verbose) {
            ErrorF("%s %s: Monitor ID: \"%s\"\n",
                   XCONFIG_GIVEN, screen->name, monitor_list[i].id);
          }
	  if (!dummy) {
            monitor_list[i].Modes = xf86PruneModes(&monitor_list[i],
                                                   monitor_list[i].Modes,
                                                   screen->name);
            screen->pModes = monitor_list[i].Modes;
          }
          break;
        }
      }
      if ( i == n_monitors ) { /* Exhausted the monitor list */
         configError("Not a declared monitor");
      }
      had_monitor = 1;
      break;
      
    default:
      if (!dummy && !validateGraphicsToken(screen->validTokens, token))
      {
        configError("Screen section keyword expected");
      }
      break;
    }
  }

  if (!dummy) {
    if (dispList == NULL) {
      FatalError(
         "A \"Display\" subsection is required in each \"Screen\" section\n");
    } else {
      /* Work out which if any Display subsection to use based on depth */
      if (xf86bpp < 0) { 
        /* no -bpp option given, so take depth if only one Display subsection */
        if (numDisps == 1) {
          if (dispList[0].depth > 0) {
            xf86bpp = dispList[0].depth;
          }
          dispIndex = 0;
        } else {
          /* Look for a section which matches the driver's default depth */
          for (dispIndex = 0; dispIndex < numDisps; dispIndex++) {
            if (dispList[dispIndex].depth == screen->depth)
              break;
          }
          if (dispIndex == numDisps) {
            /* No match.  This time, allow 15/16 and 24/32 to match */
            for (dispIndex = 0; dispIndex < numDisps; dispIndex++) {
              if (screen->depth == 15 && dispList[dispIndex].depth == 16 ||
                  screen->depth == 16 && dispList[dispIndex].depth == 15 ||
                  screen->depth == 24 && dispList[dispIndex].depth == 32 ||
                  screen->depth == 32 && dispList[dispIndex].depth == 24)
                break;
            }
          }
          if (dispIndex == numDisps) {
            /* Still no match, so exit */
            FatalError("No \"Display\" subsection for default depth %d\n",
                       screen->depth);
          }
        }
      } else {
        /* xf86bpp is set */
        if (numDisps == 1 && dispList[0].depth < 0) {
          /* one Display subsection, no depth set, so use it */
          /* XXXX Maybe should only do this when xf86bpp == default depth?? */
          dispIndex = 0;
        } else {
          /* find Display subsection matching xf86bpp */
          for (dispIndex = 0; dispIndex < numDisps; dispIndex++) {
            if (dispList[dispIndex].depth == xf86bpp)
              break;
          }
          if (dispIndex == numDisps) {
            /* No match.  This time, allow 15/16 and 24/32 to match */
            for (dispIndex = 0; dispIndex < numDisps; dispIndex++) {
              if (xf86bpp == 15 && dispList[dispIndex].depth == 16 ||
                  xf86bpp == 16 && dispList[dispIndex].depth == 15 ||
                  xf86bpp == 24 && dispList[dispIndex].depth == 32 ||
                  xf86bpp == 32 && dispList[dispIndex].depth == 24)
                break;
            }
          }
          if (dispIndex == numDisps) {
            /* Still no match, so exit */
            FatalError("No \"Display\" subsection for -bpp depth %d\n",
                       xf86bpp);
          }
        }
      }
      /* Now copy the info across to the screen rec */
      dispp = dispList + dispIndex;
      if (dispp->depth > 0) screen->depth = dispp->depth;
      if (dispp->weight.red > 0) screen->weight = dispp->weight;
      screen->frameX0 = dispp->frameX0;
      screen->frameY0 = dispp->frameY0;
      screen->virtualX = dispp->virtualX;
      screen->virtualY = dispp->virtualY;
      screen->modes = dispp->modes;
      screen->whiteColour = dispp->whiteColour;
      screen->blackColour = dispp->blackColour;
      screen->defaultVisual = dispp->defaultVisual;
      /* Add any new options that might be set */
      for (i = 0; i < MAX_OFLAGS; i++) {
        if (OFLG_ISSET(i, &(dispp->options)))
          OFLG_SET(i, &(screen->options));
        if (OFLG_ISSET(i, &(dispp->xconfigFlag)))
          OFLG_SET(i, &(screen->xconfigFlag));
      }
      /* Don't need them any more */
      xfree(dispList);
    }
  
    /* Maybe these should be FatalError() instead? */
    if ( !had_monitor ) {
      configError("A screen must specify a monitor");
    }
    if ( !had_device ) {
      configError("A screen must specify a device");
    }
  }

  /* Check for information that must be specified in XF86Config */
  if (scr_index >= 0 && xf86Screens[scr_index])
  {
    ScrnInfoPtr driver = xf86Screens[scr_index];

    graphFound = TRUE;

    if (driver->clockprog && !driver->clocks)
    {
      FatalError(
        "%s: When ClockProg is specified a Clocks line is required\n",
        driver->name);
    }

    /* Find the Index of the Text Clock for the ClockProg */
    if (driver->clockprog && textClockValue > 0)
    {
      driver->textclock = xf86GetNearestClock(driver, textClockValue);
      if (abs(textClockValue - driver->clock[driver->textclock]) >
          CLOCK_TOLERANCE)
        FatalError(
          "There is no defined dot-clock matching the text clock\n");
      if (xf86Verbose)
        ErrorF("%s %s: text clock = %7.3f, clock used = %7.3f\n",
          XCONFIG_GIVEN,
          driver->name, textClockValue / 1000.0,
          driver->clock[driver->textclock] / 1000.0);
    }
    if (xf86Verbose && driver->defaultVisual > 0) {
      char *visualname;
      switch (driver->defaultVisual) {
      case StaticGray:
      case GrayScale:
      case StaticColor:
      case PseudoColor:
      case TrueColor:
      case DirectColor:
        visualname = xf86VisualNames[driver->defaultVisual];
        break;
      default:
        configError("unknown visual type");
      }
      ErrorF("%s %s: Default visual: %s\n", XCONFIG_GIVEN, driver->name,
             visualname);
    }
    if (defaultColorVisualClass < 0)
      defaultColorVisualClass = driver->defaultVisual;

    /* GJA --Moved these from the device code. Had to reorganize it
     * a bit.
     */
    if (xf86Verbose) {
       if (OFLG_ISSET(XCONFIG_IOBASE, &driver->xconfigFlag)) 
           ErrorF("%s %s: Direct Access Register I/O Base Address: %x\n",
                  XCONFIG_GIVEN, driver->name, driver->IObase);

       if (OFLG_ISSET(XCONFIG_DACBASE, &driver->xconfigFlag)) 
           ErrorF("%s %s: DAC Base I/O Address: %x\n",
                  XCONFIG_GIVEN, driver->name, driver->DACbase);

       if (OFLG_ISSET(XCONFIG_COPBASE, &driver->xconfigFlag)) 
           ErrorF("%s %s: Coprocessor Base Memory Address: %x\n",
                  XCONFIG_GIVEN, driver->name, driver->COPbase);

       if (OFLG_ISSET(XCONFIG_POSBASE, &driver->xconfigFlag)) 
           ErrorF("%s %s: POS Base Address: %x\n", XCONFIG_GIVEN, driver->name,
                driver->POSbase);

       if (OFLG_ISSET(XCONFIG_BIOSBASE, &driver->xconfigFlag)) 
          ErrorF("%s %s: BIOS Base Address: %x\n", XCONFIG_GIVEN, driver->name,
	         driver->BIOSbase);

       if (OFLG_ISSET(XCONFIG_MEMBASE, &driver->xconfigFlag)) 
          ErrorF("%s %s: Memory Base Address: %x\n", XCONFIG_GIVEN,
                 driver->name, driver->MemBase);

      /* Print clock program */
      if ( driver->clockprog ) {
        ErrorF("%s %s: ClockProg: \"%s\"", XCONFIG_GIVEN, driver->name,
               driver->clockprog);
        if ( textClockValue )
            ErrorF(", Text Clock: %7.3f\n", textClockValue / 1000.0);
        ErrorF("\n");
       }
    }
  }
}

static void
configDisplaySubsection(disp)
DispPtr disp;
{
  int token;
  int i, j;
  int driver;
      
  while ((token = getToken(DisplayTab)) != ENDSUBSECTION) {
    switch (token) {
    case DEPTH:
      if (getToken(NULL) != NUMBER) configError("Display depth expected");
      disp->depth = val.num;
      break;

    case WEIGHT:
      if (getToken(NULL) != NUMBER) configError("Display weight expected");
      if (val.num > 9) {
        disp->weight.red = (val.num / 100) % 10;
        disp->weight.green = (val.num / 10) % 10;
        disp->weight.blue = val.num % 10;
      } else {
        disp->weight.red = val.num;
        if (getToken(NULL) != NUMBER) configError("Display weight expected");
        disp->weight.green = val.num;
        if (getToken(NULL) != NUMBER) configError("Display weight expected");
        disp->weight.blue = val.num;
      }
      break;

    case VIEWPORT:
      OFLG_SET(XCONFIG_VIEWPORT,&(disp->xconfigFlag));
      if (getToken(NULL) != NUMBER) configError("Viewport X expected");
      disp->frameX0 = val.num;
      if (getToken(NULL) != NUMBER) configError("Viewport Y expected");
      disp->frameY0 = val.num;
      break;

    case VIRTUAL:
      OFLG_SET(XCONFIG_VIRTUAL,&(disp->xconfigFlag));
      if (getToken(NULL) != NUMBER) configError("Virtual X expected");
      disp->virtualX = val.num;
      if (getToken(NULL) != NUMBER) configError("Virtual Y expected");
      disp->virtualY = val.num;
      break;

    case MODES:
      for (pLast=NULL; (token = getToken(NULL)) == STRING; pLast = pNew)
	{
	  pNew = (DisplayModePtr)xalloc(sizeof(DisplayModeRec));
	  pNew->name = val.str;

	  if (pLast) 
	    {
	      pLast->next = pNew;
	      pNew->prev  = pLast;
	    }
	  else
	    disp->modes = pNew;
	}
      /* Make sure at least one mode was present */
      if (!pLast)
	 configError("Mode name expected");
      pNew->next = disp->modes;
      disp->modes->prev = pLast;
      pushToken = token;
      break;

    case BLACK:
    case WHITE:
      {
        unsigned char rgb[3];
        int i, savetoken;
        
        savetoken = token;
        for (i = 0; i < 3; i++)
        {
          if (getToken(NULL) != NUMBER) configError("RGB value expected");
          rgb[i] = val.num & 0x3F;
        }
        if (savetoken == BLACK)
        {
          disp->blackColour.red = rgb[0];
          disp->blackColour.green = rgb[1];
          disp->blackColour.blue = rgb[2];
        }
        else
        {
          disp->whiteColour.red = rgb[0];
          disp->whiteColour.green = rgb[1];
          disp->whiteColour.blue = rgb[2];
        }
      }
      break;

    case VISUAL:
      if (getToken(NULL) != STRING) configError("Visual name expected");
      token = getStringToken(VisualTab);
      if (!dummy && disp->defaultVisual >= 0)
        configError("Only one default visual may be specified");
      disp->defaultVisual = token - STATICGRAY;
      break;

    case OPTION:
      if (getToken(NULL) != STRING) configError("Option string expected");
      i = 0;
      while (xf86_OptionTab[i].token != -1) 
      {
	if (StrCaseCmp(val.str, xf86_OptionTab[i].name) == 0)
	{
          OFLG_SET(xf86_OptionTab[i].token, &(disp->options));
	  break;
	}
	i++;
      }
      if (xf86_OptionTab[i].token == -1)
        configError("Unknown option string");
      break;

    case EOF:
      FatalError("Unexpected EOF (missing EndSubSection)");
      break; /* :-) */

    default:
      configError("Display subsection keyword expected");
      break;
    }
  }
}

Bool 
xf86LookupMode(target, driver)
     DisplayModePtr target;
     ScrnInfoPtr    driver;
{
  DisplayModePtr p;
  DisplayModePtr best_mode = NULL;
  int            i, Gap;
  int            Minimum_Gap = CLOCK_TOLERANCE + 1;
  Bool           found_mode = FALSE;
  Bool           clock_too_high = FALSE;
  static Bool	 first_time = TRUE;

  if (first_time)
  {
    ErrorF("%s %s: Maximum allowed dot-clock: %1.3f MHz\n", XCONFIG_PROBED,
	   driver->name, driver->maxClock / 1000.0);
    first_time = FALSE;
  }

  for (p = driver->pModes; p != NULL; p = p->next)	/* scan list */
  {
    if (!strcmp(p->name, target->name))		/* names equal ? */
    {
      if (OFLG_ISSET(CLOCK_OPTION_PROGRAMABLE, &(driver->clockOptions)))
      {
        if (driver->clocks == 0)
        {
          /* this we know */
          driver->clock[0] = 25175;		/* 25.175Mhz */
          driver->clock[1] = 28322;		/* 28.322MHz */
          driver->clocks = 2;
        }

        if ((p->Clock / 1000) > (driver->maxClock / 1000))
          clock_too_high = TRUE;
        else
        {
          /* We fill in the the programmable clocks as we go */
          for (i=0; i < driver->clocks; i++)
            if (driver->clock[i] == p->Clock)
              break;

          if (i >= MAXCLOCKS)
          {
            ErrorF("%s %s: Too many programmable clocks used (limit %d)!\n",
                   XCONFIG_PROBED, driver->name, MAXCLOCKS);
            return FALSE;
          }

          if (i == driver->clocks)
          {
            driver->clock[i] = p->Clock;
            driver->clocks++;
          }
	       
          target->Clock = i;
          best_mode = p;
        }
      }
      else
      {
        i = xf86GetNearestClock(driver, p->Clock);
        Gap = abs(p->Clock - driver->clock[i]);
        if (Gap < Minimum_Gap)
        {
          if ((driver->clock[i] / 1000) > (driver->maxClock / 1000))
            clock_too_high = TRUE;
          else
          {
            target->Clock = i;
            best_mode = p;
            Minimum_Gap = Gap;
          }
        }
      }
      found_mode = TRUE;
    }
  }

  if (best_mode != NULL)
  {
    target->HDisplay       = best_mode->HDisplay;
    target->HSyncStart     = best_mode->HSyncStart;
    target->HSyncEnd       = best_mode->HSyncEnd;
    target->HTotal         = best_mode->HTotal;
    target->VDisplay       = best_mode->VDisplay;
    target->VSyncStart     = best_mode->VSyncStart;
    target->VSyncEnd       = best_mode->VSyncEnd;
    target->VTotal         = best_mode->VTotal;
    target->Flags          = best_mode->Flags; 
    target->CrtcHDisplay   = best_mode->CrtcHDisplay;
    target->CrtcHSyncStart = best_mode->CrtcHSyncStart;
    target->CrtcHSyncEnd   = best_mode->CrtcHSyncEnd;
    target->CrtcHTotal     = best_mode->CrtcHTotal;
    target->CrtcVDisplay   = best_mode->CrtcVDisplay;
    target->CrtcVSyncStart = best_mode->CrtcVSyncStart;
    target->CrtcVSyncEnd   = best_mode->CrtcVSyncEnd;
    target->CrtcVTotal     = best_mode->CrtcVTotal;
    target->CrtcHAdjusted  = best_mode->CrtcHAdjusted;
    target->CrtcVAdjusted  = best_mode->CrtcVAdjusted;
    if (target->Flags & V_DBLSCAN)
    {
      target->CrtcVDisplay *= 2;
      target->CrtcVSyncStart *= 2;
      target->CrtcVSyncEnd *= 2;
      target->CrtcVTotal *= 2;
      target->CrtcVAdjusted = TRUE;
    }

    /* I'm not sure if this is the best place for this in the
     * new XF86Config organization. - SRA
     */
    if (found_mode)
      if ((driver->ValidMode)(target) == FALSE)
        {
         ErrorF("%s %s: Unable to support mode \"%s\"\n",
              XCONFIG_GIVEN,driver->name, target->name );
         return(FALSE);
        }

    if (xf86Verbose)
    {
      ErrorF("%s %s: Mode \"%s\": mode clock = %7.3f",
             XCONFIG_GIVEN, driver->name, target->name,
             best_mode->Clock / 1000.0);
      if (!OFLG_ISSET(CLOCK_OPTION_PROGRAMABLE, &(driver->clockOptions)))
        ErrorF(", clock used = %7.3f", driver->clock[target->Clock] / 1000.0);       
      ErrorF("\n");
    }
  }
  else if (!found_mode)
    ErrorF("%s %s: There is no mode definition named \"%s\"\n",
	   XCONFIG_PROBED, driver->name, target->name);
  else if (clock_too_high)
    ErrorF("%s %s: Clock for mode \"%s\" %s\n\tLimit is %7.3f MHz\n",
           XCONFIG_PROBED, driver->name, target->name,
           "is too high for the configured hardware.",
           driver->maxClock / 1000.0);
  else
    ErrorF("%s %s: There is no defined dot-clock matching mode \"%s\"\n",
           XCONFIG_PROBED, driver->name, target->name);

  return (best_mode != NULL);
}

void
xf86VerifyOptions(allowedOptions, driver)
     OFlagSet      *allowedOptions;
     ScrnInfoPtr    driver;
{
  int j;

  for (j=0; xf86_OptionTab[j].token >= 0; j++)
    if ((OFLG_ISSET(xf86_OptionTab[j].token, &driver->options)))
      if (OFLG_ISSET(xf86_OptionTab[j].token, allowedOptions))
      {
	if (xf86Verbose)
	  ErrorF("%s %s: Option \"%s\"\n", XCONFIG_GIVEN,
	         driver->name, xf86_OptionTab[j].name);
      }
      else
	ErrorF("%s %s: Option flag \"%s\" is not defined for this driver\n",
	       XCONFIG_GIVEN, driver->name, xf86_OptionTab[j].name);
}

/* Note: (To keep me [GJA] from getting confused)
 * We have two mode-related datastructures:
 * 1. A doubly linked mode name list, with ends marked by self-pointers.
 * 2. A doubly linked mode structure list.
 * We are operating here on the second structure.
 * Initially this is just singly linked.
 */
static DisplayModePtr
xf86PruneModes(monp, allmodes, scrname)
MonPtr monp;			/* Monitor specification */
DisplayModePtr allmodes;	/* List to be pruned */
char *scrname;			/* Screen name for use in error messages */
{
	DisplayModePtr dispmp;	/* To walk the list */
	DisplayModePtr olddispmp; /* The one being freed. */
	DisplayModePtr remainder; /* The first one retained. */

	dispmp = allmodes;

	/* The first modes to be deleted require that the pointer to the
	 * mode list is updated. Also, they have no predecessor in the list.
	 */
	while ( dispmp && !mode_fits(dispmp, monp, scrname) ) {
		olddispmp = dispmp;
		dispmp = dispmp->next;
		xfree(olddispmp->name);
		xfree(olddispmp);
	}
	/* Now we either have a mode that fits, or no mode at all */
	if ( ! dispmp ) { /* No mode at all */
		return NULL;
	}
	remainder = dispmp;
	while ( dispmp->next ) {
		if ( !mode_fits(dispmp->next, monp, scrname) ) {
			olddispmp = dispmp->next;
			dispmp->next = dispmp->next->next;
			xfree(olddispmp->name);
			xfree(olddispmp);
		} else {
			dispmp = dispmp->next;
		}
	}
	return remainder; /* Return pointer to {the first / the list } */
}

/* Return TRUE if the mode pointed to by dispmp agrees with all constraints
 * we can make up for the monitor pointed to by monp.
 * FALSE otherwise.
 */
static Bool mode_fits(dispmp, monp, scrname)
DisplayModePtr	dispmp;
MonPtr monp;
char *scrname;
{
	int i;
	float dotclock, hsyncfreq, vrefreshrate;

#if 0
	/* max dotclock != monitor bandwidth, do exclude this check */
	dotclock = (float)(dispmp->Clock);
	if ( dotclock > monp->bandwidth * 1000.0 ) {
		ErrorF("%s %s: Mode \"%s\" needs %f MHz bandwidth. Deleted.\n",
			XCONFIG_PROBED, scrname, dispmp->name, dotclock/1000.0);
		return FALSE;
	}
#endif

	hsyncfreq = (float)(dispmp->Clock) / (float)(dispmp->HTotal);
	for ( i = 0 ; i < monp->n_hsync ; i++ ) {
		if ( monp->hsync[i].hi == monp->hsync[i].lo ) {
			if ( (hsyncfreq > 0.999 * monp->hsync[i].hi) &&
			     (hsyncfreq < 1.001 * monp->hsync[i].hi) )
			{
				break; /* Matches close enough. */
			}
		} else {
			if ( (hsyncfreq > 0.999 * monp->hsync[i].lo) &&
			     (hsyncfreq < 1.001 * monp->hsync[i].hi) )
			{
				break; /* In range. */
			}
		}
	}
	/* Now see whether we ran out of sync frequencies */
	if ( i == monp->n_hsync ) {
		ErrorF(
		  "%s %s: Mode \"%s\" needs hsync freq of %.2f kHz. Deleted.\n",
		  XCONFIG_PROBED, scrname, dispmp->name, hsyncfreq);
		return FALSE;
	}
			
	vrefreshrate = (float)(dispmp->Clock) * 1000.0 /
				((float)(dispmp->HTotal) *
					(float)(dispmp->VTotal)) ;
	if ( dispmp->Flags & V_INTERLACE ) vrefreshrate *= 2.0;
	if ( dispmp->Flags & V_DBLSCAN ) vrefreshrate /= 2.0;
	for ( i = 0 ; i < monp->n_vrefresh ; i++ ) {
		if ( monp->vrefresh[i].hi == monp->vrefresh[i].lo ) {
			if ( (vrefreshrate > 0.999 * monp->vrefresh[i].hi) &&
			     (vrefreshrate < 1.001 * monp->vrefresh[i].hi) )
			{
				break; /* Matches close enough. */
			}
		} else {
			if ( (vrefreshrate > 0.999 * monp->vrefresh[i].lo) &&
			     (vrefreshrate < 1.001 * monp->vrefresh[i].hi) )
			{
				break; /* In range. */
			}
		}
	}
	/* Now see whether we ran out of refresh rates */
	if ( i == monp->n_vrefresh ) {
		ErrorF(
		  "%s %s: Mode \"%s\" needs vert refresh rate of %.2f Hz. Deleted.\n",
		  XCONFIG_PROBED, scrname, dispmp->name, vrefreshrate);
		return FALSE;
	}

	/* Passed every test. */
	return TRUE;
}
