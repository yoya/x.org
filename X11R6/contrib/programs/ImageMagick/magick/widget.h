
/*
  Define declarations.
*/
#define DoubleClick  250

/*
  Typedef declarations.
*/
typedef struct _XWidgetInfo
{
  int
    id;

  unsigned int
    bevel_width,
    width,
    height;
 
  int
    x,
    y,
    min_y,
    max_y;

  unsigned int
    raised,
    active,
    trough,
    highlight;

  char
    *text,
    *cursor,
    *marker;
} XWidgetInfo;

/*
  X utilities routines.
*/
extern int
  XMenuWidget _Declare((Display *,XWindows *,char *,char **,char *));

extern KeySym
  XCommandWidget _Declare((Display *,XWindows *,char *));

extern void
  XColorBrowserWidget _Declare((Display *,XWindows *,char *,char *)),
  XDialogWidget _Declare((Display *,XWindows *,char *,char *,
    char *)),
  XFileBrowserWidget _Declare((Display *,XWindows *,char *,char *)),
  XFontBrowserWidget _Declare((Display *,XWindows *,char *,char *)),
  XNoticeWidget _Declare((Display *,XWindows *,char *,char *)),
  XTextViewWidget _Declare((Display *,XWindows *,char *,char **));
