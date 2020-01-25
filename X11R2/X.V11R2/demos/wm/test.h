/* 
 * $Locker:  $ 
 */ 
static char     *rcsid_test_h = "$Header: test.h,v 1.1 87/06/17 16:14:55 swick Locked ";
#define BW 2
#define ICONWIDTH 100
#define TITLEHEIGHT 24
#define BORDERWIDTH BW
#define GROWBOXWIDTH 24
#define UPBOXWIDTH 24
#define ICONIFYWIDTH 24
#define ASSOCTABLESZ 64
#define MAXNAME 100
#define MINTITLE 32
#define MINHEIGHT (2*TITLEHEIGHT+BORDERWIDTH)
#define MINWIDTH (GROWBOXWIDTH + UPBOXWIDTH + ICONIFYWIDTH + 2*BORDERWIDTH + MINTITLE)
#define MARGIN 10

typedef struct GenericAssoc {
	Window frame;
	Window header;
	Window headerParent;
	Window client;
	Window rbox;
	Window iconify;
	Window upbox;
	Window iconWindow;
	int x,y;
	int width, height;
	int bwidth;
	int iconx;
	int icony;
	char iconname[MAXNAME];
	int iconnamelen;
	Bool iconknown;
	Bool ownIcon;
	int headerWidth;
	char name[MAXNAME];
	int namelen;
	XSizeHints sh;
	XWMHints *wmh;
	Colormap ColorMap;
	Window root;
} GenericAssoc;

typedef struct _RootInfo {
    Window      root;
    int         rootx, rooty, rootwidth, rootheight;
    Pixmap      doubleArrow;
    Pixmap      growBox;
    Pixmap      cross;
    GC          headerGC, xorGC;
    int		background;
    int		foreground;
    int		borderpixel;
}           RootInfoRec;

RootInfoRec *RootInfo;
char	*BackgroundPixelName;
char	*ForegroundPixelName;
char	*BorderPixelName;
int	BorderWidth;
char	*fontName;
