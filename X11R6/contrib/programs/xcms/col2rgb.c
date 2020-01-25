/* col2rgb.c :  Convert xcms descriptions to server rgb file */

#if ! defined(__STDC__)
#define const
#define volatile
#define register
#endif

static const char *Version[] = {
	"@(#) col2rgb.c Copyright (c) 1993, 1994 Christopher Alexander North-Keys",
	"$Grueppe: Talisman $",
	"$Anfang: Wed Jul  7 12:42:09 GMT 1993 $",
#if defined(__STDC__)
	"$Compiliert: "__DATE__" " __TIME__ " $",
#endif
	"$Source: /home/staff/erlkonig/lib/xcms/col2rgb.c $",
	"$State: Release $",
	"$Revision: 1.0 $",
	"$Date: Sat Jun  4 16:00:58 MET DST 1994 $",
	"$Author: erlkonig $",
	(const char*)0
	};

/* Copyright (c) 1994, Christopher Alexander North-Keys
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

#include <X11/Xlib.h>
#include <X11/Xcms.h>
#include <X11/Xresource.h>
#include <X11/Intrinsic.h>
#include <string.h>
#include "fgetstr.h"

char *Program = "<anon>";
char Debug = 0;

#if defined(__STDC__)
int Usage(FILE *out)
#else
int Usage(out)
FILE *out;
#endif
{
	static const char *Syntax[] = {
		"\tinput/output defaults to col-fmt -> name spec\n",
		"\t-xcmsdb 	: XCMSDB-format input, with head-/footers\n",
		"\t-rgbtxt 	: rgb.txt-format output -> r g b name\n",
		"\t-help   	: output this help\n",
		"\t-version	: output version information\n",
		"\t-debug   : enable debugging\n",
		"\t(options may be abbreviated, eg. -h -he -hel also mean -help)\n",
		(const char*)0
		};

	const char **s = &Syntax[0];
	fprintf(out,
			"syntax: %s [-help] [-version] [-xcmsdb] [-rgbtxt] [<XtOpts..>]\n",
			Program);
	while(*s) fputs(*s, out), s++;

	return 0;
}

#if defined(__STDC__)
int VersionOut(void)
#else
int VersionOut()
#endif
{
	const char **s = &Version[0];
	while(*s) puts(*s), s++;
	return 0;
}

#if defined(__STDC__)
int main(Cardinal argc, String *argv)
#else
int main(argc, argv)
Cardinal argc;
String *argv;
#endif
{	
	int argi;

	XtAppContext app_con = XtCreateApplicationContext();
	Display *dsp = XtOpenDisplay(app_con,
								 NULL,  /* use $DISPLAY */
								 "col2rgb", "Col2rgb",
								 NULL, 0,
								 &argc, argv);
	/* we'd initialize the next few immediately, but dsp may be bogus */
	int scr_no;
	Visual *vis = (Visual*)0;
	Colormap cmap;
	XcmsCCC context;

	int input_xcmsdb = 0;					   /* need to skip head/tail? */
	int output_rgbtxt = 0;

	char *line = (char*)0;

	Program = argv[0];

	if( ! dsp)
	{
		fprintf(stderr, "%s: error - no display\n", Program);
		return Usage(stderr), -1;
	}

	scr_no = DefaultScreen(dsp);
	vis = DefaultVisual(dsp, scr_no);
	cmap = DefaultColormap(dsp, scr_no);
	context = XcmsDefaultCCC(dsp, scr_no); /* yay! */ 

	/* parse remaining args */
	for(argi = 1 ; argi < argc ; ++argi)
	{
		int len = strlen(argv[argi]);
		if( ! strncmp("-xcmsdb", argv[argi], len))
			input_xcmsdb = 1;
		else if( ! strncmp("-rgbtxt", argv[argi], len))
			output_rgbtxt = 1;
		else if( ! strncmp("-debug", argv[argi], len))
			Debug = 1;
		else if( ! strncmp("-help", argv[argi], len))
			return Usage(stdout), 0;
		else if( ! strncmp("-version", argv[argi], len))
			return VersionOut(), 0;
		else
			return Usage(stderr), -1;
	}

	if(input_xcmsdb)
		for( /**/ ; line = Fgetstr(stdin, "\n") ; free(line))
		{
			static const char headmark[] = "XCMS_COLORDB_START";
			enum {headmark_null_idx = sizeof headmark / sizeof *headmark - 1};
			if( ! output_rgbtxt)
				puts(line);
			if( ! strncmp(&headmark[0], line, headmark_null_idx - 1))
			{
				free(line);
				break;
			}
		}

	for( /**/ ; line = Fgetstr(stdin, "\n") ; free(line))
	{
		/* skip forward over tab[s], next char should be start of colorspec */
		char *cp = line;
		char *colorname = (char*)0;
		char *colorspec = (char*)0;

		if(input_xcmsdb)
		{
			static const char tailmark[] = "XCMS_COLORDB_END";
			enum {tailmark_null_idx = sizeof tailmark / sizeof *tailmark - 1};
			if( ! strncmp(&tailmark[0], line, tailmark_null_idx - 1))
			{
				if( ! output_rgbtxt)
					puts(line);
				free(line);
				break;
			}
		}

		while(*cp && isspace(*cp)) ++cp; /* skip leading space */
		colorname = cp;					 /* save colorname location */ 
		/*while(*cp && ! isspace(*cp)) ++cp; /* skip colorname */
		while(*cp && ('\t' != *cp)) ++cp; /* skip colorname */
		if(*cp) *cp++ = '\0';				   /* null-term. colorname */ 
		while(*cp && isspace(*cp)) ++cp;   /* skip over space */
		colorspec = cp;					   /* save colorspec location */
		while(*cp && ! isspace(*cp)) ++cp; /* skip colorspec */
		if(*cp) *cp = '\0';				   /* null-term. colorspec */

		if(Debug)
			fprintf(stderr, "recognized name \"%s\", spec \"%s\"\n",
					colorname, colorspec);

		if(colorname && colorspec)
		{
			XcmsCCC ccc = XcmsCreateCCC(dsp, scr_no, vis,
										NULL, /* client white pt defaults */
										NULL, /* XcmsCompressionProc */
										NULL, /* data for preceding */
										NULL, /* XcmsWhiteAdjustProc */
										NULL  /* data for preceding */
										);
			if(ccc)
			{
				XcmsColor color_exact, color_avail;
				Bool      compression_flag = 0;
				int status;

				/* parse the colorspec into an XcmsColor*/
				if(XcmsFailure !=
				   XcmsLookupColor(dsp, cmap,
								   colorspec,
								   &color_exact, &color_avail,
								   XcmsUndefinedFormat))
				{
					/* convert the XcmsColor specs into RGB form */
					if(XcmsFailure !=
					   XcmsConvertColors(ccc,
										 &color_exact, /* in/out color */
										 1, /* number of preceding */
										 XcmsRGBFormat, /* trg fmt */
										 &compression_flag))
						/* output R G and B */
					{
						XcmsRGB *rgb = &color_exact.spec.RGB;

						if(output_rgbtxt)
							printf("%3d %3d %3d\t\t%s\n",
								   rgb->red   / 256,
								   rgb->green / 256,
								   rgb->blue  / 256,
								   colorname
								   );
						else
							printf("%-39s rgb:%04x/%04x/%04x\n",
								   colorname,
								   rgb->red,
								   rgb->green,
								   rgb->blue
								   );
					}
					else
					{
						fprintf(stderr, "Warning: %s not converted\n",
								colorspec);
						puts(colorspec);
					}
				}
					   
				/* cleanup for next color */
				XcmsFreeCCC(ccc);
			}
		}
	}
	
	return 0;
}


	
