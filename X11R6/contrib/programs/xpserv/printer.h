/*
Copyright (c) 1994  Matthew Reynolds and Qualcomm Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
QUALCOMM INC., OR MATTHEW REYNOLDS BE LIABLE FOR ANY CLAIM, 
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE 
OR OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Qualcomm Inc., or Matthew 
Reynolds shall not be used in advertising or otherwise to promote the sale, 
use or other dealings in this Software without their prior written 
authorization.

*/

#ifndef _PRINTER_H_
#define _PRINTER_H_

#include <stdio.h>
#include <X11/Xlib.h>

#define DEFAULTPORT	6000
#define TRUE		1
#define FALSE		0
#define BACKLOG 	5
#define STATICMAXFD	32
#define NUM_COLORS	256
#define NUMWIN  	64
#define NUMGC   	64
#define NUMATOM   	64
#define NUMFONT		64
#define NUMCMAP   	8
#define MAXFONTS	1024
#define MAXCACHEFONTS	128
#define FONTFILE	"/usr/lib/X11/printfonts.dir"

/*
   the following structure remembers for each file descriptor its
   state.  In particular, we need to know if it is busy or free
   and if it is in use, by whom.
*/

typedef int FD;

struct FDDescriptor {
    Bool    Busy;
    int     (*InputHandler)();
};

typedef struct reply_del_ {
    unsigned char	*reply;		/* if there's reply to substitute */
    int			reply_length;
} reply_del_s, *reply_del_p;

/* this is the structure for adding in printer options. I started off with
   a flag for indexed colorspaces, because our stupid versatec doesn't
   support them... */

typedef struct print_options_ {		/* options used in the program. */
    int		index_cspace_support;
} print_options_s, *print_options_p;

extern print_options_s	PRINT_OPTIONS;	/* global ( only one of it, anyway) */

struct PrintClient {
    Window		win;
    Bool		server_win;		/* X server window or ours */
    char		*printfile;
    FILE		*printfp;
    unsigned char	*buffer;
    int			bufsize;
    int			(*bytefn)();
    int			sequence;
    int			preserve_aspect;	/* flag for aspect ratio */
    int			font_scale;		/* font scale factor */
    int			pixmap_scale;		/* pixmap scale factor */
    int			page_width;		/* page size in points */
    int			page_height;
}; 

/* window information */

typedef struct winfo_ {
    Window		parent;
    Window		win;
    XWindowAttributes	attr;
    int			created;
    int			fd;
} winfo_s, *winfo_p;

/* window table */

typedef struct winfo_table_ {
    winfo_p	win_table;
    int		num_win;
} winfo_table_s, *winfo_table_p;

/* colormap information */

typedef struct cmapinfo_ {
    XColor	colors[ NUM_COLORS];
    int		num_alloc[ NUM_COLORS];
    int		num_colors;
    int		writeable[ NUM_COLORS];
    Colormap	cmap;
    int		fd;
} cmapinfo_s, *cmapinfo_p;

/* table of colormaps */

typedef struct cmapinfo_table_ {
    cmapinfo_p	cmap_table;
    int		num_cmap;
} cmapinfo_table_s, *cmapinfo_table_p;

/* GC information */

typedef struct gcinfo_ {
    XID		gcid;
    XGCValues	attr;
    XRectangle	*clip_rects;
    int		num_clip_rects;
    char	*dashes;
    int		num_dashes;
    int		fd;
} gcinfo_s, *gcinfo_p;

/* gc table */

typedef struct gcinfo_table_ {
    gcinfo_p	gc_table;
    int		num_gc;
} gcinfo_table_s, *gcinfo_table_p;

/* for enum-string lookup tables */

typedef struct lookup_ {
    int		tag;
    char	*name;
} lookup_s, *lookup_p;

typedef struct font_ {
    FD		fd;
    Font	fid;
    Font	server_fid;
    char	*xfont_name;
    char	*post_name;
    XFontStruct	*font_info;
} font_s, *font_p;

typedef struct font_table_ {
    font_p	font_table;
    int		num_fonts;
} font_table_s, *font_table_p;

typedef struct atom_ {
    char	*name;
    int		id;
} atom_s, *atom_p;

typedef struct atom_table_ {
    atom_p	atom_table;
    int		num_atoms;
} atom_table_s, *atom_table_p;


extern lookup_s XFN_NAMES[];
extern Display	*THE_DISPLAY;

void polyfillrectangle( FILE            *fp,            
                        Window          win,
                        XGCValues       *gcv,
                        int             nrect,
                        XRectangle      *rectangles);

void fillpoly( FILE             *fp,
               Window           win,
               XGCValues        *gcv,
               int              npoint,
               int              coordmode,
               XPoint           *points);

void polyline( FILE             *fp,
               Window           win,
               XGCValues        *gcv,
               int              npoint,
               int              coordmode,
               XPoint           *points);

void polytext8( FILE            *fp,
                Window          win,
                XGCValues       *gcv,
                int             x,
                int             y,
                int             ntext,
                unsigned char   *buffer);

void polytext16( FILE           *fp,
                 Window         win,
                 XGCValues      *gcv,
                 int            x,
                 int            y,
                 int            ntext,
                 unsigned char  *buffer);


void imagetext8( FILE            *fp,
                 Window          win,
                 XGCValues       *gcv,
                 int             x,
                 int             y,
                 int             ntext,
                 unsigned char   *buffer);

void imagetext16( FILE           *fp,
                  Window         win,
                  XGCValues      *gcv,
                  int            x,
                  int            y,
                  int            ntext,
                  unsigned char  *buffer);

short GetServerPort();

short GetScopePort();

char *OfficialName( 
    char *name
);

void Usage();

void ScanArgs( 
    int     argc,
    char    **argv
);

void SetUpPair( 
    FD client,
    FD server
);

void CloseConnection( 
    FD fd
);

FD FDPair( 
    FD fd
);

FD ClientHalf( 
    FD fd
);

FD ServerHalf( 
    FD fd
);

char *ClientName( 
    FD fd
);

int DataFromClient( 
    FD fd
);

int DataFromServer( 
    FD fd
);

int NewConnection( 
    FD fd
);

FD ConnectToClient( 
    FD ConnectionSocket
);

FD ConnectToServer( 
    Bool report
);

char *OfficialName( 
    char *name
);

void InitializeFD();

void UsingFD( 
    FD     fd,
    int    (*Handler)()
);

void NotUsingFD( 
    FD	fd
);

void EOFonFD( 
    FD	fd
);

void MainLoop();

void SetUpConnectionSocket(
    int		iport
);

long pad( 
    long n
);

long pad2( 
    long n
);

int GetConnSetupLength(
    unsigned char *buf
);

int GetRequestLength(
    unsigned char *buf
);

int GetReplyLength(
    unsigned char *buf
);

int GetConnSetupReplyLength(
    unsigned char *buf
);

void StartClientConnection(
     FD fd
);

void scrunch( 
    unsigned char *buf,
    int           start,
    int           length
);

void splice( 
    unsigned char *buf,
    unsigned char *insert,
    int           buflen,
    int           inslen
);

Bool InterceptRequest( 
    FD		  fd,
    unsigned char *buf,
    unsigned char **newmsg,
    int		  *newlen
);

void ReplaceReply( 
    FD		  fd,
    unsigned char *buf,
    int		  length
);

void ProcessRequest( 
    FD            fd,
    unsigned char *buf,
    unsigned char **sndbuf,
    int           *sendlen
);

void EatMessage( 
    FD            fd,
    int           length
);

void ProcessReply( 
    FD            fd,
    unsigned char *buf,
    unsigned char **sndbuf,
    int           *sendlen
);

char *lookup_tag( 
    int		tag,
    lookup_p	table
);

Bool ReplyExpected( 
    unsigned char *buf
);

Bool isReply( 
    unsigned char *buf
);

unsigned char *HeadDelete(
    struct PrintClient	*pcs,
    unsigned char	*buf,
    int			*sendlen
);

Bool MidDelete( 
    struct PrintClient *pcs,
    unsigned char      *p,
    unsigned char      **end,
    int                *sendlen
);

void set_curr_gc( 
    FD		fd,
    FILE	*fp,
    XGCValues	*gcvp,
    Colormap	cmap
);

/* set the values in a GC given the flags and the data buffer from a request */

void set_gcvalues( 
    FD			fd,
    FILE		*fp,
    XGCValues 		*gcvp, 
    unsigned long 	value_mask, 
    unsigned long 	*request,
    Colormap		cmap
);

gcinfo_p getgcinfo( 
    unsigned long 	gcid, 
    gcinfo_table_p 	table
);

winfo_p getwinfo( 
    unsigned long	win,
    winfo_table_p	table
);

cmapinfo_p getcmapinfo( 
    unsigned long	cmap,
    cmapinfo_table_p	table
);

winfo_p getgenwinfo( 
    unsigned long 	win,
    winfo_table_p	table
);

Bool DecodeRequest( FD                  fd,
                    unsigned char       *buf,
                    unsigned char       *newmsg,
                    int                 *newlen);

void initprocs(
    struct PrintClient	*pc,
    int			fd
);

void setcolor(
    FILE *fp,
    unsigned long pixel,
    Colormap cmap
);

void setlinewidth(
    FILE *fp,
    int line_width
);

void setlinecap(
    FILE *fp,
    int cap_style
);

void setlinejoin(
    FILE *fp,
    int join_style
);

int setfont(
    FD	fd,
    FILE *fp,
    Font font_id
);

void polyfillrectangle(
    FILE *fp,
    Window win,
    XGCValues *gcv,
    int nrect,
    XRectangle *rectangles
);

void polyrectangle(
    FILE *fp,
    Window win,
    XGCValues *gcv,
    int nrect,
    XRectangle *rectangles
);

void fillpoly(
    FILE *fp,
    Window win,
    XGCValues *gcv,
    int npoint,
    int coordmode,
    XPoint *points
);

void polyline(
    FILE *fp,
    Window win,
    XGCValues *gcv,
    int npoint,
    int coordmode,
    XPoint *points
);

void polysegment(
    FILE *fp,
    Window win,
    XGCValues *gcv,
    int nseg,
    XSegment *segments
);

void polytext8(
    FILE *fp,
    Window win,
    XGCValues *gcv,
    int x,
    int y,
    int ntext,
    unsigned char *buffer
);

void polytext16(
    FILE *fp,
    Window win,
    XGCValues *gcv,
    int x,
    int y,
    int ntext,
    unsigned char *buffer
);

void imagetext8(
    FILE *fp,
    Window win,
    XGCValues *gcv,
    int x,
    int y,
    int ntext,
    unsigned char *buffer
);

void imagetext16(
    FILE *fp,
    Window win,
    XGCValues *gcv,
    int x,
    int y,
    int ntext,
    unsigned char *buffer
);

void cleararea(
    FILE *fp,
    Window win,
    XWindowAttributes *attr,
    int x,
    int y,
    int width,
    int height
);

void putimage(
    FILE 		*fp,
    Window 		win,
    XGCValues 		*gcv,
    int 		depth,
    int 		width,
    int 		height,
    int 		x,
    int 		y,
    int 		leftpad,
    int 		format,
    int 		num_colors,
    XColor 		*colors,
    unsigned char 	*data
);

void copyarea( 
       FILE             *fp,
       Drawable         src,
       Window           win,
       int              src_x,
       int              src_y,
       int              dst_x,
       int              dst_y,
       int              width,
       int              height,
       int              num_colors,
       XColor           *colors
);

extern int debug;

void copy_gcvalues( XGCValues 	 *gcvp, 
		    XGCValues	 *src_gcvp,
		    unsigned long value_mask);

char *get_post_name( char *xfont);

void init_print_clients( struct PrintClient     *pcs, 
			 int                    numclients);

void remove_client( int fd);

void removecmaps( int                   fd,
		  cmapinfo_table_p      table);

void removegcs( int             fd,
		gcinfo_table_p  table);

void removewins( int            fd,
		 winfo_table_p  table);

void delete_win( winfo_table_p  winfo_table,
                 unsigned long  win);

void delete_gc( gcinfo_table_p  gcinfo_table,
                unsigned long   gc);

void delete_cmap( cmapinfo_table_p      cmapinfo_table,
                  unsigned long         cmap);

int read_loop( int	fd,
	       char	*msg,
	       int	length);

char **read_strvec( char *filename);

void print_image_grey( unsigned char 	*image,
		       int		x,
		       int		y,
		       int		width,
		       int		height,
		       int		depth,
		       XColor		*colors,
		       int		num_colors,
		       FILE		*fp,
		       int		index_support);

#endif /* _PRINTER_H_ */
