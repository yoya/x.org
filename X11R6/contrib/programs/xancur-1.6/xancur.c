/* xancur.c -- X11R{4,5,6} animation of the cursor in the root window
 * Copyright (c) 1993, 1994, Christopher Alexander North-Keys
 * 
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 */

/*
 * Thanks to Mark Lillibridge of MIT Project Athena for having written xsetroot
 * summary: cycle through a list of (cursor, mask) pairs.
 */

#ifndef lint
static char Version[]="@(#)xancur.c v1.6, Christopher North-Keys, 30.May.1994";
#endif

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <pwd.h>
#include <signal.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
;

#ifndef XANCUR_SCRIPT
#define XANCUR_SCRIPT "."
#endif

#ifndef FAIL
#define FAIL (-1)
#endif

typedef struct _CursorList
{
    char *scriptdir;
	char *image;
	char *mask;
	char *fg;
	char *bg;
	Cursor cursor;
	struct _CursorList *next;
} CursorList;

extern char *getenv();
extern char *getwd();
extern struct passwd *getpwnam();

char		*Home = NULL;
char		*ProgramName = NULL;
Display		*Dpy;
int			 Scr;
Window		 Root;
char		*Foreground = NULL;
char		*Background = NULL;
int			 Reverse = 0;
unsigned	 Microsecs = 200000;
int			 Iterations = -1;
int          Debug = 0;
CursorList	*Cursors = NULL;

void	 SetCursorFiles();
void     OpenScript(/* char *name */);
void     ExecuteData();
char	*FileNameExpand(/* char *name */);
char    *EnrootPathName(/* char *dir, char *name */);
long	 SwallowFile(/*char **addr_buffer, char *path */);
Pixmap	 ReadBitmapFile();
XColor	 NameToXColor();
Cursor 	 CreateCursorFromFiles();

void
Fatal(location, call, addition)
char *location, *call, *addition;
{
    (void)fprintf(stderr, "%s: (%s)", ProgramName, location);
    if(addition) (void)fprintf(stderr, " %s\n", addition);
    if(call)
    {
        (void)fprintf(stderr, "\t");
        perror(call);
    }
    exit(FAIL);
}

void Usage()
{
	(void)fprintf(stderr, "%s: usage error.\n", ProgramName);
	(void)fprintf(stderr, "[cursor_defs]  :cursorfile/maskfile pairs -- bitmap filenames\n");
	(void)fprintf(stderr, "-script <name> :read cursor_definitions from script <name>\n");
	(void)fprintf(stderr, "-fg <color>    :set foreground\n");
	(void)fprintf(stderr, "-bg <color>    :set background\n");
	(void)fprintf(stderr, "-rv            :reverse fg and bg\n");
	(void)fprintf(stderr, "-usleep <int>  :set frame delay in microseconds, default %d\n", Microsecs);
	(void)fprintf(stderr, "-iter <int>    :set number of iterations, default infinite\n");
	(void)fprintf(stderr, "-display <dpy> :connect to X server [dpy] (also -d [dpy])\n");
	exit (1);
	return;
}

void
AbortClean()
{
	XUndefineCursor(Dpy, Root);
	XCloseDisplay(Dpy);
	exit(0);
}

int
main(argc, argv)
int argc;
char **argv;
{
	char *display_name = NULL;
	int i;
	
	ProgramName = argv[0];
	Home = getenv("HOME");
	
	for (i = 1; i < argc; i++)
	{
		if (!strcmp ("-display", argv[i]) || !strcmp ("-d", argv[i])) {
			if (++i>=argc) Usage();
			display_name = argv[i];
			continue;
		}
		if (!strcmp("-fg", argv[i]))
		{
			if (++i>=argc) Usage();
			Foreground = argv[i];
			continue;
		}
		if (!strcmp("-bg", argv[i]))
		{
			if (++i>=argc) Usage();
			Background = argv[i];
			continue;
		}
		if (!strcmp("-rv", argv[i]))
		{
			Reverse = 1;
			continue;
		}
		if (!strcmp("-usleep", argv[i]))
		{
			if (++i>=argc) Usage();
			Microsecs = (unsigned)atoi(argv[i]);
			continue;
		}
		if (!strcmp("-iter", argv[i]))
		{
			if (++i>=argc) Usage();
			Iterations = atoi(argv[i]);
			continue;
		}
		if (!strcmp("-debug", argv[i]))
		{
			Debug = 1;
			continue;
		}
		if (!strcmp("-script", argv[i]))
		{
			char *pathname[MAXPATHLEN];
			if (++i>=argc) Usage();
			/* Time to fetch a file */
			OpenScript(EnrootPathName(getwd(pathname), argv[i]));
			continue;
		}
		if (argv[i][0] != '-') 
		{	
			
			if (i+1>=argc) Usage();
			SetCursorFiles(NULL, argv[i], argv[i+1]);
			i++;
			continue;
		}
		Usage();
	}
	
	if (!Cursors)
	{
		char *name;
		if(NULL == (name = (char *)malloc(strlen(ProgramName)+1)))
            Fatal("main", "malloc", NULL);
		(void)strcat(name, ".");
		(void)strcat(name, ProgramName);
		OpenScript(EnrootPathName("~", name));
	}
	
	if (!Cursors) OpenScript(XANCUR_SCRIPT);
	
	if (!(Dpy = XOpenDisplay(display_name)))
	{
		(void)fprintf(stderr, "%s:  unable to open display '%s'\n",
					  ProgramName, XDisplayName (display_name));
		exit (-1);
	}
	Root = RootWindow(Dpy, Scr = DefaultScreen(Dpy));
	
	signal(SIGTERM, AbortClean);
	signal(SIGHUP,  AbortClean);
	signal(SIGINT,  AbortClean);
		   	
	/* handle minor cursor insanity, an endless loop */
	if (Cursors)
	{
		CursorList *cur;
		
		for(cur = Cursors ; cur ; cur = cur->next)
            cur->cursor = CreateCursorFromFiles(cur->image, cur->mask,
                                                cur->fg, cur->bg);
		
		while(Iterations < 0 ? 1 : Iterations--)
		{
			for(cur = Cursors ;
				cur ;
				cur = cur->next)
			{
				XDefineCursor(Dpy, Root, cur->cursor);
				XFlush(Dpy);
				usleep(Microsecs);
			}
		}
		
		for(cur = Cursors ; cur ; cur = cur->next)
			XFreeCursor(Dpy, cur->cursor);
	}

	AbortClean();
	return (0);
}

/* Open script file.  Should be given a full pathname. */
void
OpenScript(script)	/* RECURSIVE */
char *script;
{	
	char *data;		        /* the actual data */

    if(Debug)(void)fprintf(stderr,
                           "%s: OpenScript %s\n", ProgramName, script);
    
    if((long)0 < SwallowFile(&data, script))	/*Dense, do not free*/
        ExecuteData(data, script);
    
    return;
}


/* execute instructions contained in data just read in */
void
ExecuteData(data, script)
char *data;
char *script;    /* name of script from which data was taken. */
{
    char *p;							/* pointer into data */
    
    for(p = data ; p && *p ; /*internal*/ )	/* parse the file */
    {	/* resolve to a line */
        char *b;						/* beginning of a line */
        
        p = &p[strspn(p, " \t\n")];		/* skip white space */
        b = p;							/* found beginning */
        
        p = strpbrk(p, "\n");			/* seek to newline */
        if (p)
        {
            *p = '\0';		  		 	/* zero newline */
            if(p) p++;					/* now at beginning of new line */
        }
        
        /* analyze line ----------------------------------------*/
        /* an attempt was made to use strtok, which was munged. */

        if(*b == '#')       /* a comment */
            continue;
        
        if(*b == '-')		/* a switch */
        {
            char *s;	/* switch */
            char *a;	/* argument to switch */
            
            if(NULL == (s = strpbrk(b, " \t")))
            {
                if(!strcmp(b, "-rv"))
                {
                    char *tmp_color;
                    tmp_color = Foreground;
                    Foreground = Background;
                    Background = tmp_color;
                }
            } else {
                *s++ = '\0';                /* make first field a string */
                a = &s[strspn(s, " \t")];             /* skip whitespace */
                if(s = strpbrk(a, " \t")) *s = '\0';    /* eol already 0 */
                if(!strcmp(b, "-script"))
                {
                    char *newscr = NULL;
                    char *nsp;              /* newscript pointer */
                    if(NULL == (newscr = (char *)malloc(MAXPATHLEN)))
                        Fatal("ExecuteData", "malloc", "new script");
                    
                    (void)strcpy(newscr, script);
                    nsp = strrchr(newscr, '/');
                    (void)strcpy(++nsp, a);

                    OpenScript(newscr);
                }
                if(!strcmp(b, "-fg")) Foreground = a;
                if(!strcmp(b, "-bg")) Background = a;
            }
        } else {
            char *s;			/* ptr (eventually) to mask_file */
            
            /* look for cursor filenames */
            if(NULL == (s = strpbrk(b, " \t")))
            {
                SetCursorFiles(script, b, (char *)NULL);
            } else {
                char *m;
                *s++ = '\0';
                m = &s[strspn(s, " \t")];
                if(s = strpbrk(m, " \t")) *s = '\0';
                
                SetCursorFiles(script, b, m);
            }
        } /*else*/
    } /*for*/
    return;
}

char *
FileNameExpand(orig)
char *orig;
{
	char *hi;
	
	if(!orig) return orig;
	if(*orig == '/') return orig;
    if(*orig != '~') return orig;

	/* at this point we know that a username expansion is required */

    if(orig[1] == '\0') return Home;

	if(NULL == (hi = (char *)malloc(MAXPATHLEN)))
        Fatal("FileNameExpand", "malloc", "expansion buffer");
	
	if(orig[1] == '/')
	{
		(void)strcpy(hi, Home);
		(void)strcpy(hi, &orig[1]);
	} else {
		char *user, *end;
		struct passwd *tmp;
		
		if((NULL == (user = (char *)malloc(MAXPATHLEN))))
            Fatal("FileNameExpand", "malloc", "user buffer");

		(void)strcpy(user, &orig[1]);
		end = strchr(user, '/');
		*end = '\0';
		end++;
		
		if(NULL == (tmp = getpwnam(user)))
            Fatal("FileNameExpand", "getpwname", "user appears bogus");

		(void)strcpy(hi, (tmp->pw_dir));
		(void)strcat(hi, "/");
		(void)strcat(hi, end);
	}
	return hi;
}

char *
EnrootPathName(curdir, orig)
char *curdir, *orig;
{
    char *hi;
    
    curdir = FileNameExpand(curdir);
    orig = FileNameExpand(orig);
    
    if(*orig == '/') return orig;

	if(NULL == (hi = (char *)malloc(strlen(curdir)+strlen(orig)+1)))
	{
        Fatal("EnrootPathName", "malloc", Home);
	}
    (void)strcpy(hi, curdir);
    (void)strcat(hi, "/");
    (void)strcat(hi, orig);

    if(*hi != '/')
        Fatal("EnrootPathName", NULL, "enrooting failed");
    else
        return hi;
}

/* Create a new cursor, link it in, and set filenames and colors. */
void
SetCursorFiles(script, cursor_file, mask_file)
char *script;
char *cursor_file;
char *mask_file;
{
    char *dir = NULL;
	static CursorList *Tail;
	CursorList *hi;  /* say "hi" to the new boy... */
	
	if(NULL == (hi = (CursorList *)malloc(sizeof(CursorList))))
	{
		(void)fprintf(stderr, "%s:  malloc failed\n", ProgramName);
		exit(-1);
	}

    if(script)
    {
        if(NULL == (dir = (char*)malloc(MAXPATHLEN)))
            Fatal("SetCursorFiles", "malloc", "script directory");
        else
        {
            char *end;
            (void)strcpy(dir, script);
            end = strrchr(dir, '/');
            *end = '\0';
        }            
    } else {
		char *pathname[MAXPATHLEN];
        dir = getwd(pathname);
    }
    
	if(!mask_file)
	{
		if(NULL == (mask_file = (char *)malloc(strlen(cursor_file)+6)))
		{
			perror("malloc");
			exit(1);
		}
		(void)strcpy(mask_file, cursor_file);
		(void)strcat(mask_file, ".mask");
	}

	cursor_file = EnrootPathName(dir, cursor_file);
	mask_file = EnrootPathName(dir, mask_file);
	
	if(!Cursors) Cursors = hi;
	if(Tail) Tail->next = hi;
	Tail = hi;
	hi->next = NULL;
    hi->scriptdir = dir;
	hi->image = cursor_file;
	hi->mask = mask_file;
	hi->fg = Foreground;
	hi->bg = Background;
	
	return;
}

/* make a cursor of the right colors from two bitmap files. */
Cursor
CreateCursorFromFiles(cursor_file, mask_file, foreground, background)
char *cursor_file, *mask_file, *foreground, *background;
{
	Pixmap cursor_bitmap, mask_bitmap;
	unsigned int c_width, c_height, m_width, m_height;
	int x_hot, y_hot;
	Cursor cursor;
	XColor fg, bg;
	
	fg = NameToXColor(foreground, BlackPixel(Dpy, Scr));
	bg = NameToXColor(background, WhitePixel(Dpy, Scr));
	
	cursor_bitmap = ReadBitmapFile(cursor_file, &c_width, &c_height,
								   &x_hot, &y_hot);
	
    mask_bitmap = ReadBitmapFile(mask_file, &m_width, &m_height,
								 (int *)NULL, (int *)NULL);
	
	if (!cursor_bitmap || !mask_bitmap)
	{
		(void)fprintf(stderr, "%s: read failure on cursor %s, aborting\n",
					  ProgramName, cursor_file);
		exit (1);
	}
	
    if (c_width != m_width || c_height != m_height)
	{
		(void)fprintf(stderr, "%s: dimensions of bitmap and mask differ\n",
					  ProgramName);
		exit(1);
    }
	
    if ((x_hot == -1) && (y_hot == -1))
	{
		x_hot = (int)(c_width / 2);
		y_hot = (int)(c_height / 2);
		(void)fprintf(stderr, "%s: hotspot defaulting to (%d,%d) in %s\n",
					  ProgramName, x_hot, y_hot, cursor_file);
    }
    if ((x_hot < 0) || (x_hot >= c_width) ||
		(y_hot < 0) || (y_hot >= c_height))
	{
		(void)fprintf(stderr,
					  "%s: hotspot at (%d,%d) outside cursor bounds [0,%d],[0,%d]\n",
					  ProgramName, x_hot, y_hot, c_width, c_height);
		(void)fprintf(stderr, "in cursor bitmap %s\n", cursor_file);
		exit(1);
    }
	
    cursor = XCreatePixmapCursor(Dpy, cursor_bitmap, mask_bitmap, &fg, &bg,
								 (unsigned int)x_hot, (unsigned int)y_hot);
    XFreePixmap(Dpy, cursor_bitmap);
    XFreePixmap(Dpy, mask_bitmap);
	
    return(cursor);
}

/* Resolve name to actual X color. */
XColor
NameToXColor(name, pixel)
char *name;
unsigned long pixel;
{
    XColor color;
	
    if (name && *name)
	{
		if (!XParseColor(Dpy, DefaultColormap(Dpy, Scr), name, &color))
		{
			(void)fprintf(stderr, "%s: unknown color or bad color format: %s\n",
						  ProgramName, name);
			Usage();
		}
    } else {
		color.pixel = pixel;
		XQueryColor(Dpy, DefaultColormap(Dpy, Scr), &color);
	}
	
    return(color);
}

Pixmap
ReadBitmapFile(filename, width, height, x_hot, y_hot)
char *filename;
unsigned int *width, *height;
int *x_hot, *y_hot;
{
    Pixmap bitmap;
    int status;
	
    status = XReadBitmapFile(Dpy, Root, filename, width, height,
							 &bitmap, x_hot, y_hot);
	switch(status)
	{
	  case BitmapSuccess:
		return(bitmap);
		break;
	  case BitmapOpenFailed:
		(void)fprintf(stderr, "%s: can't open file: %s\n",
					  ProgramName, filename);
		break;
	  case BitmapFileInvalid:
		(void)fprintf(stderr, "%s: bad bitmap format file: %s\n",
					  ProgramName, filename);
		break;
	  default:
		(void)fprintf(stderr, "%s: insufficient memory for bitmap: %s",
					  ProgramName, filename);
		exit(-1);
	}
	return((Pixmap)0);
}

long
SwallowFile(addr_buffer, path)
char **addr_buffer, *path;
{
    struct stat statbuf;
    int         in_count;
    int         fd = -1;
	
    /* Attempt to open desired file */
    if(((fd)=open(path, O_RDONLY))==-1)
    {
        /* (void)fprintf(stderr, "SwallowFile: %s not opened\n", path);
		 */
        return (FAIL);
    }
	
    if(fstat(fd, &statbuf)==-1)
    {
        (void)fprintf(stderr, "SwallowFile: error on fstat file %s\n", path);
        return (FAIL);
    }
	
    /* Get a buffer to fit */
    if((*addr_buffer=(char *)calloc(1, statbuf.st_size+1))==NULL)
    {
        (void)fprintf(stderr, "SwallowFile: no space for calloc\n");
        return (FAIL);
    }
	
    /* Read in the file */
	/* Warning:  this will probably break on files over 64K in length */
    if((in_count = read(fd, *addr_buffer, statbuf.st_size)) != statbuf.st_size)
    {
        (void)fprintf(stdout,
                      "SwallowFile: error(?) %d/%d bytes read from %s\n",
                      in_count, statbuf.st_size, path);
        (void)free(*addr_buffer);
        return (FAIL);
    }
    return (in_count);
}
