#include <Xr/xr-copyright.h>

/* $Header: types.h,v 7.0 86/11/13 08:32:54 rick Exp $ */
/* Copyright 1986, Hewlett-Packard Company */
/* Copyright 1986, Massachussetts Institute of Technology */


/*************************************<+>*************************************
 *****************************************************************************
 **
 **   File:        types.h
 **
 **   Project:     X-ray Toolbox
 **
 **   Description: 
 **
 **
 **   ------------------------ MODIFICATION RECORD   ------------------------
 *
 * $Log:	types.h,v $
 * Revision 7.0  86/11/13  08:32:54  08:32:54  rick ()
 * Final QA release
 * 
 * Revision 6.0  86/11/10  15:19:58  15:19:58  rick ()
 * QA #2 release
 * 
 * Revision 5.6  86/11/07  13:59:13  13:59:13  rick ()
 * Added the copyright message.
 * 
 * Revision 5.5  86/11/07  13:19:58  13:19:58  ed ()
 * Added borderWidth to xrMenu
 * 
 * Revision 5.4  86/11/06  12:02:51  12:02:51  ed ()
 * Added a few changes for the dialog manager
 * 
 * Revision 5.2  86/10/31  12:41:06  12:41:06  rick ()
 * Changed the id member in the xrGraphic structure to graphicId.
 * 
 * Revision 5.1  86/10/30  12:33:26  12:33:26  rick ()
 * Added extern for XrPageEdit().
 * 
 * Revision 5.0  86/10/28  08:25:52  08:25:52  rick ()
 * QA #1.1 release
 * 
 * Revision 4.4  86/10/27  13:29:33  13:29:33  rick ()
 * Externed XrInputMap().
 * 
 * Revision 4.3  86/10/26  16:34:44  16:34:44  ed ()
 * Modified panel and menu structures.
 * 
 * Revision 4.2  86/10/23  16:17:11  16:17:11  ed ()
 * panel manager changes
 * 
 * Revision 4.0  86/10/20  12:11:43  12:11:43  rick ()
 * QA 1 release
 * 
 * Revision 3.14  86/10/20  08:37:30  08:37:30  fred ()
 * Added page editor structures.
 * 
 *
 *****************************************************************************
 *************************************<+>*************************************/


/************************************************************************
 *
 *   Size typedefs
 *
 ************************************************************************/

typedef char  INT8;
typedef short INT16;
typedef long  INT32;
typedef char  * STRING8;
typedef short * STRING16;

typedef unsigned char  UINT8;
typedef unsigned short UINT16;
typedef unsigned long  UINT32;

typedef int (*xrPFI)();

/**************************************************************************
 *
 *  Graphic structure definitions
 *
 **************************************************************************/

typedef struct 
{
   INT16 x;
   INT16 y;
} POINT;

typedef struct 
{
   INT16 x;
   INT16 y;
   INT16 width;
   INT16 height;
} RECTANGLE;

typedef struct
{
   INT16 x;
   INT16 y;
   INT16 width;
   INT16 height;
   INT16 angle1;
   INT16 angle2;
} ARC;

typedef struct
{
   INT16 width;
   INT16 height;
} SIZE;

typedef struct
{
   Pixmap   pixMapId;
   SIZE     pixMapSize;
   INT32    depth;
   UINT16 * baseAddr;
} xrPixMap;


typedef struct
{
   FontInfo * fontInfo;
   INT32      ascent;
   INT32      descent;
   INT32      maxWidth;
   INT32      avgWidth;
   INT32      leading;
} xrTextInfo;


typedef struct
{
   INT16    height;
   INT16    width;
   INT8     depth;
   UINT8  * data;
   POINT    extractionLoc;
} xrPixelData;


/************************************************************************
 *
 *  The xrEvent structure needed for XrInput().
 *
 ************************************************************************/


typedef struct
{
   INT32 type;
   INT32 source;
   INT16 inputCode;
   INT8  inputType;
   INT8  value1;
   INT16 value2;
   INT16 value3;
   POINT valuePt;
   INT32 valuePtr;
} xrEvent;


typedef struct
{
   INT32 fd;
   INT8  type;
} xrFDInput;


typedef struct
{
   RECTANGLE windowRect;
   Pixmap    foreTile;
   Pixmap    backTile;
} xrWindowData;


typedef struct
{
   UINT32 inputType;
   INT16  inputCode;
} xrWindowEvent;


typedef struct
{
   INT8     processFlag;
   xrPFI    funct;
   INT32    instance;
   INT32    message;
   INT32    eventCount;
   xrWindowEvent * eventList;
} xrWindowFunctInfo;



/************************************************************************
 *
 *  Graphic list structure
 *
 ************************************************************************/

typedef struct _xrGraphic
{
   INT32   graphicId;
   INT8    code;
   INT32   message;
   INT8  * data;
   struct  _xrGraphic * graphicPtr;
} xrGraphic;


/************************************************************************
 *
 *  typedefs for the resource manager
 *
 ************************************************************************/

typedef struct
{
   UINT16   resourceType;
   INT32    resourceId;
   INT8     resourceState;
   INT32    resourceFile;
   INT8   * resourceObject;
} xrResourceInfo;


typedef struct _xrResource
{
   INT32    resourceId;
   INT8     resourceState;
   INT32    resourceFile;
   INT8   * resourceObject;
   struct _xrResource * left;
   struct _xrResource * right;
} xrResource;


typedef struct
{
   UINT16 resourceType;
   xrPFI  resourceHandler;
} xrResourceTypeInfo;


typedef struct
{
   xrResource * resourcePtr;
   xrPFI resourceHandler;
} xrResourceType;


typedef struct
{
   INT32 memorySize;
   INT32 discSize;
   INT32 resourceFile;
} xrResourceSizes;


/************************************************************************
 *
 *  typedefs for the resource manager resource types.
 *
 ************************************************************************/

typedef struct
{
   INT16 width;
   INT16 height;
   INT8  depth;
   UINT16 * raster;
} xrPixmap;


typedef struct
{
   INT16  width;
   INT16  height;
   Pixmap pixmapId;
} xrPixmapId;


typedef struct
{
   INT16  width;
   INT16  height;
   Bitmap bitmapId;
} xrBitmapId;


typedef struct
{
   xrPixmap cursor;
   xrPixmap mask;
   INT16 xoff;
   INT16 yoff;
   INT32 foreground;
   INT32 background;
   INT32 func;
} xrCursor;


INT32 (*xrFunction)();
   


/****************************************************************
 *
 *  typedef for the editor list structure
 *
 ***************************************************************/

typedef struct _xrEditor
{
   struct _xrEditor * (*editorFunct)();
   Window      editorWindowId;
   RECTANGLE   editorRect;
   INT8        editorState;
   INT8      * editorData;
   struct _xrEditor * nextEditor;
} xrEditor;



/****************************************************************************
 *
 * Structure used to define and editor group.
 *
 ****************************************************************************/

typedef struct _xrEditorGroup
{
   INT32       groupWindowId;
   RECTANGLE   groupRect;
   INT8        groupState;
   xrEditor  * editorList;
   xrGraphic * graphicList;
   struct _xrEditorGroup * nextGroup;
} xrEditorGroup;


typedef struct
{
   INT8 * (*newMalloc)();
   INT32  (*newFree)();
   INT8 * (*newRealloc)();
   INT8 * (*newCalloc)();
} xrAllocFuncts;
   


/***************************************************************************
 *
 * Template to overlay the first five fields of an editor Info structure
 *
 **************************************************************************/

typedef struct {
   Window    editorWindowId;
   RECTANGLE editorRect;
   INT8      editorState;
   INT32     editorFGColor;
   INT32     editorBGColor;
} xrEditorTemplate;


/***************************************************************************
 *
 * Structure used to create an instance of the page editor
 *
 **************************************************************************/

typedef struct {
	Window     editorWindowId;
	RECTANGLE  editorRect;
	INT8       editorState;
        INT32      editorFGColor;
        INT32      editorBGColor;
	FontInfo * editorFont;
        INT8     * buffer;
        INT32      bufferCount;
        INT32      bufferSize;
        INT32      maxCharCount;
        INT32      rowCount;
        INT32      colCount;
        INT32      tabWidth;
} xrPageEditInfo;

typedef struct {
        INT8  * buffer;
        INT32   bufferCount;
        INT32   bufferSize;
        INT32   maxCharCount;
} xrPageEditBufInfo;
        

/***************************************************************************
 *
 * Structure used to create an instance of the checkbox editor
 *
 **************************************************************************/

typedef struct {
	Window     editorWindowId;
	RECTANGLE  editorRect;
	INT8       editorState;
        INT32      editorFGColor;
        INT32      editorBGColor;
	FontInfo * editorFont;
	INT16      numFields;
	INT16      numCols;
	INT8    ** labels;
	INT8     * values;
	INT8     * stateFlags;
} xrCheckBoxInfo;


/***************************************************************************
 *
 * Structure used to create an instance of the push button editor
 *
 **************************************************************************/

typedef struct {
	Window     editorWindowId;
	RECTANGLE  editorRect;
	INT8       editorState;
        INT32      editorFGColor;
        INT32      editorBGColor;
	FontInfo * editorFont;
	INT16      numFields;
	INT16      numCols;
	INT16      defaultButton;
	INT16      borderWidth;
	INT8    ** labels;
	INT8     * stateFlags;
} xrPushButtonInfo;


/***************************************************************************
 *
 * Structure used to create an instance of the radio button editor
 *
 **************************************************************************/

typedef struct {
	Window     editorWindowId;
	RECTANGLE  editorRect;
	INT8       editorState;
        INT32      editorFGColor;
        INT32      editorBGColor;
	FontInfo * editorFont;
	INT16      numFields;
	INT16      numCols;
	INT8    ** labels;
	INT16    * value;
	INT8     * stateFlags;
} xrRadioButtonInfo;


/***************************************************************************
 *
 * Structure used to create an instance of the raster edit editor
 *
 **************************************************************************/

typedef struct {
	Window    editorWindowId;
	RECTANGLE editorRect;
	INT8      editorState;
        INT32     editorFGColor;
        INT32     editorBGColor;
	INT32     pixelColor;
	INT32     pixelSize;
        SIZE      viewRegion;
	xrPixmap  rasterData;
} xrRasterEditInfo;


/***************************************************************************
 *
 * Structure used to create an instance of the raster select editor
 *
 **************************************************************************/

typedef struct {
        INT16  rasterIndex;
        Pixmap pixmapId;
} newRaster;


typedef struct {
	Window    editorWindowId;
	RECTANGLE editorRect;
	INT8      editorState;
        INT32     editorFGColor;
        INT32     editorBGColor;
	Pixmap  * rasterIds;
	INT32     rasterHeight;
	INT32     rasterWidth;
	INT16     rasterCount;
	INT16     colCount;
	INT16   * activeRaster;
} xrRasterSelectInfo;


/***************************************************************************
 *
 * Structure used to create an instance of the scrollbar editor
 *
 **************************************************************************/

typedef struct {
        INT8    components;
        INT16   min;
        INT16   max;
        INT16   slidePosition;
        INT16   slideSize;
        INT32   (*handler)();
        INT16   granularity;
} xrSBParameters;

typedef struct {
	Window         editorWindowId;
	RECTANGLE      editorRect;
	INT8           editorState;
        INT32          editorFGColor;
        INT32          editorBGColor;
	INT8           orientation;
        xrSBParameters configuration;
} xrScrollBarInfo;


/***************************************************************************
 *
 * Structure used to create an instance of the static raster editor
 *
 **************************************************************************/

typedef struct {
	Window     editorWindowId;
	RECTANGLE  editorRect;
	INT8       editorState;
        INT32      editorFGColor;
        INT32      editorBGColor;
        INT32      rasterHeight;
        INT32      rasterWidth;
	Pixmap     rasterId;
} xrStaticRasterInfo;


/***************************************************************************
 *
 * Structure used to create an instance of the static text editor
 *
 **************************************************************************/

typedef struct {
	Window     editorWindowId;
	RECTANGLE  editorRect;
	INT8       editorState;
        INT32      editorFGColor;
        INT32      editorBGColor;
	FontInfo * editorFont;
	INT8     * string;
	INT32      alignment;
} xrStaticTextInfo;


/***************************************************************************
 *
 * Structure used to create an instance of the text edit editor
 *
 **************************************************************************/

typedef struct {
	Window     editorWindowId;
	RECTANGLE  editorRect;
	INT8       editorState;
        INT32      editorFGColor;
        INT32      editorBGColor;
	FontInfo * editorFont;
	INT8     * label;
	INT8     * string;
	INT32      maxChars;
	INT32      insertPos;
        INT8       insertMode;
        INT8       labelPosition;
        INT8       cellWidth;
} xrTextEditInfo;


/***************************************************************************
 *
 * Structure used to create an instance of the titlebar editor
 *
 **************************************************************************/

typedef struct {
	Window     editorWindowId;
	RECTANGLE  editorRect;
	INT8       editorState;
        INT32      editorFGColor;
        INT32      editorBGColor;
	FontInfo * editorFont;
	INT8     * titleName;
        INT8     * gadgetIcon1;
        INT8     * gadgetIcon2;
} xrTitleBarInfo;

/************************************************************************
 *
 *  Panel structures
 *
 ***********************************************************************/

typedef struct {
        xrEditor     * (* editorFunct) ();
        INT8         * editorData;
        xrEditor     * editorInstance;
}xrPanelField;

typedef struct {
        Pixmap          winBackground;
        Pixmap          winForeground;
        INT16           borderWidth;
        INT32           foregroundColor;
        INT32           backgroundColor;
        FontInfo      * fontInfo;
        Cursor          cursor;
        
        INT32           showFlag;
        xrEditor     *  currentEditor;
        INT32           timer;
        INT32           (* initFunct)();
}xrPanelContext;

typedef struct {
        POINT             panelOrigin;
        SIZE              panelSize;
        Window            relativeTo;
        Window            childOf;
        xrPanelContext  * panelContext;

        xrPanelField    * fieldList;
        INT32             numFields;
        INT16             panelId;
} xrPanelInfo;

typedef struct _xrPanel {
        RECTANGLE            panelRect;
        RECTANGLE            panelZeroRect;
        Window               relativeTo;
        Window               childOf;
        xrPanelContext     * panelContext;

        xrEditor          ** editorInstance;
        INT32                numFields;
        INT16                panelId;

        Window               panelWindow;
        xrEditorGroup      * editorGroup;
        struct _xrPanel   ** subPanel;
        INT32                numSubPanels;
}xrPanel;

typedef struct
{
   POINT              messageOrigin;
   Window             relativeTo;
   xrPanelContext   * messageContext;

   INT32              rasterHeight;
   INT32              rasterWidth;
   Pixmap             rasterId;

   INT8             * messageText;

   INT8            ** messageButtons;
   INT32              numButtons;
} xrMsgBoxInfo;

typedef struct
{
   INT8            * menuTitle;
   INT8           ** menuItems;
   INT32             numItems;
   xrPanelContext  * menuContext;
   INT16             menuId;
   INT32             menuStyle;
} xrMenuInfo;

typedef struct
{
   Window           editorWindowId;
   RECTANGLE        editorRect;
   INT8             editorState;
   INT32            editorFGColor;
   INT32            editorBGColor;
   FontInfo       * editorFont;

   INT8           * menuTitle;
   INT32            numItems;
   INT8          ** menuStrings;
   INT32          * itemTypes;
   INT32          * stringLengths;
   INT16          * keybdEquiv;

} xrMenuEditor;




typedef struct _xrMenuIndex
{
   struct _xrMenu    * menuInstance;
   INT32               itemIndex;
   INT32               itemData;
} xrMenuIndex;

typedef struct _xrMenu
{
   xrEditor               * menuEditor;
   Window                   menuWindow;
   Cursor                   menuCursor;
   INT32                    totalWidth;        /* Filled at MSG_NEW */
   INT32                    totalHeight;
   INT16                    menuId;
   INT32                    menuStyle;
   POINT                    menuOrigin;
   INT32                    borderWidth;
   Window                   currentWindows[XrMAXMENUWINDOWS];
   INT32                    numWindows;
   struct _xrMenuIndex      currentPath[XrMAXMENU];
   INT32                    pathLength;
   Pixmap                   menuPixmap;
   INT32                    stickyMenu;
} xrMenu;





/*
 *  Extern declarations for X-ray's functions
 */

extern Window          XrInput();
extern Window          XrEditor();
extern xrEditorGroup * XrEditorGroup();
extern INT8            XrMapButton();
extern char          * XrInputMap();
extern INT8          * XrVersion();

extern xrEditor * XrScrollBar();
extern xrEditor * XrTitleBar();
extern xrEditor * XrStaticText();
extern xrEditor * XrStaticRaster();
extern xrEditor * XrPushButton();
extern xrEditor * XrCheckBox();
extern xrEditor * XrRadioButton();
extern xrEditor * XrTextEdit();
extern xrEditor * XrRasterEdit();
extern xrEditor * XrRasterSelect();
extern xrEditor * XrPageEdit();
extern xrEditor * XrMenuEdit();


/************************************************************************
 *
 *	External declaration for global variables.
 *
 ************************************************************************/

extern POINT        xrZeroPt;
extern RECTANGLE    xrZeroRect;

extern FontInfo   * xrBaseFontInfo;


/*
 *  External declarations for the allocation functions.
 */

extern char * (*xrMalloc)();
extern int    (*xrFree)();
extern char * (*xrRealloc)();
extern char * (*xrCalloc)();


/*
 *  The externs which contain the select and menu button conditions.
 */

extern INT8 xrSelect;
extern INT8 xrSelectUp;
extern INT8 xrMenuPost;
extern INT8 xrMenuItemSelect;


/*
 *  The externs for the button modifiers.
 */

extern UINT16 xrLBDMod;		/*  Left button down modifier    */
extern UINT16 xrLBUMod;		/*  Left button up modifier      */
extern UINT16 xrMBDMod;		/*  Middle button down modifier  */
extern UINT16 xrMBUMod;		/*  Middle button up modifier    */
extern UINT16 xrRBDMod;		/*  Right button down modifier   */
extern UINT16 xrRBUMod;		/*  Right button up modifier     */


/*
 *  Externs for X-ray defaults globals.
 */

extern Pixmap xrWindowForeground;
extern Pixmap xrWindowBackground;
extern INT32  xrBorderWidth;
extern INT32  xrForegroundColor;
extern INT32  xrBackgroundColor;
extern Cursor xrDefaultCursor;
extern Pixmap xrDefaultTile;
extern INT16  xrCursorImage[];
extern INT16  xrCursorMask[];


/*
 *  The externs for the menu manager.
 */

extern INT32  xrMenuSticky;


extern INT32 xr_GCList[][21];
extern INT32 xrDefaultGC;
extern INT32 xrEditorGC1;
extern INT32 xrEditorGC2;
extern INT32 xrEditorGC3;
extern INT32 xrEditorGC4;
extern INT32 xrEditorGC5;
extern INT32 xrEditorGC6;
extern INT32 xrEditorGC7;
extern INT32 xrEditorGC8;

extern UINT16 xr_bitmaps[][16];
extern Bitmap xr_bitmapIds[];

extern Vertex xr_PolyList[];

extern INT32 xrErrno;

extern xrPanelContext xrPanelContextDefaults;
extern xrPanel * XrPanel();
extern xrMenu  * XrMenu();
extern INT32     XrMsgBox();
