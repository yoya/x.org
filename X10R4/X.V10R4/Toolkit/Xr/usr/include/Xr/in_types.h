#include <Xr/xr-copyright.h>

/* $Header: in_types.h,v 7.0 86/11/13 08:32:40 rick Exp $ */
/* Copyright 1986, Hewlett-Packard Company */
/* Copyright 1986, Massachussetts Institute of Technology */


/*************************************<+>*************************************
 *****************************************************************************
 **
 **   File:        in_types.h
 **
 **   Project:     X-ray Toolbox
 **
 **   Description: 
 **
 **
 **   ------------------------ MODIFICATION RECORD   ------------------------
 *
 * $Log:	in_types.h,v $
 * Revision 7.0  86/11/13  08:32:40  08:32:40  rick ()
 * Final QA release
 * 
 * Revision 6.0  86/11/10  15:19:42  15:19:42  rick ()
 * QA #2 release
 * 
 * Revision 5.1  86/11/07  13:58:56  13:58:56  rick ()
 * Added the copyright message.
 * 
 * Revision 5.0  86/10/28  08:25:38  08:25:38  rick ()
 * QA #1.1 release
 * 
 * Revision 4.0  86/10/20  12:11:35  12:11:35  rick ()
 * QA 1 release
 * 
 * Revision 3.7  86/10/20  08:37:14  08:37:14  fred ()
 * Added page editor structures.
 * 
 *
 *****************************************************************************
 *************************************<+>*************************************/



/*******************************************************************************
 *
 * Structure used by multiple entity editors, to store the information
 * describing each of the individual entities.
 *
 ******************************************************************************/


typedef struct {
	RECTANGLE rectangle;
	INT8    * label;
	INT8      state;
	INT8    * value;
	POINT     labelPt;
        RECTANGLE subRectangle;
} xrItemData;



/*******************************************************************************
 *
 * Structure used by several of the multiple entity utilities, to access
 * information contained within the Data structure associated with a push
 * button, check box or radio button editor instance.  The data structure
 * associated with each of these editor types have their first 7 elements
 * laid out in the same order as in this template.
 *
 ******************************************************************************/

typedef struct {
	INT16          numFields;
        INT16          numCols;
	xrTextInfo     font;
	INT32          fGColor;
	INT32          bGColor;
        Pixmap         tileId;
	xrItemData   * fields;
        INT8        ** labels;
        INT8         * states;
} xrDataTemplate;

/*******************************************************************************
 *
 * Structure used to maintain the description of a page editor instance
 *
 ******************************************************************************/

typedef struct {
	INT8        * peBuffer;       /* The editing buffer */
        INT32         peBufferCount;  /* Number of chars in buffer */
        INT32         peBufferSize;   /* Size of editing buffer */
        INT32         peMaxSize;      /* Max # of chars to allow */
	INT8          peCursorOn;     /* TRUE, if instance is active */
	xrTextInfo    peFont;         /* Font information */
	INT32         peFGColor;      /* Instance's foreground color */
	INT32         peBGColor;      /* Instance's background color */
	Pixmap        peTileId;       /* 50 % tile id */
        INT32         peRowNum;       /* Number of rows in window */
        INT32         peColNum;       /* Number of cols in window */
	INT32         peTabWidth;     /* Width of a tab character */
        INT8        * peTopOfFrame;   /* Ptr to char at (0,0) in window */
        INT8        * peBlanks;       /* Used for clearing a line */
        INT8        * peCursorPtr;    /* Cursor position within buffer */
        INT32         peCursorRow;    /* Cursor window relative row position */
        INT32         peCursorCol;    /* Cursor window relative col position */
        INT32         peVCursorCol;   /* Used when cursoring up and down to */
                                      /* different length lines.            */
} xrPageEditData;


/*******************************************************************************
 *
 * Structure used to maintain the description of a raster edit instance
 *
 ******************************************************************************/

typedef struct {
	INT32            reFGColor;
	INT32            reBGColor;
	INT32            pixelColor;
	INT32            pixelSize;
        SIZE             viewRegion;
	POINT            viewOrigin;
	RECTANGLE        reRectangle;
        xrPixelData    * extractionData;
        xrPixmap         rasterData;
	xrEditor       * vScrollbar;
        xrSBParameters   vSBParameters;
	xrEditor       * hScrollbar;
        xrSBParameters   hSBParameters;
} xrRasterEditData;


/*******************************************************************************
 *
 * Structure used to maintain the description of a raster select instance
 *
 ******************************************************************************/

typedef struct {
        Pixmap    rasterId;
        RECTANGLE itemRect;
} xrRasterSelectItem;


typedef struct {
        INT32                rsFGColor;
        INT32                rsBGColor;
        INT32                rasterHeight;
        INT32                rasterWidth;
        INT16                rasterCount;
        INT16                colCount;
        INT16                lastActive;
        INT16              * activeRaster;
        xrRasterSelectItem * rasterData;
} xrRasterSelectData;


/*******************************************************************************
 *
 * Structures used to maintain the description of a checkbox instance
 *
 ******************************************************************************/


typedef struct {
	INT16          cbNumFields;
        INT16          cbNumCols;
	xrTextInfo     cbFont;
	INT32          cbFGColor;
	INT32          cbBGColor;
        Pixmap         cbTileId;
	xrItemData   * cbFields;
        INT8        ** cbLabels;
        INT8         * cbStates;
        INT8         * cbValues;
} xrCheckBoxData;


/*******************************************************************************
 *
 * Structures used to maintain the description of a radio button instance
 *
 ******************************************************************************/


typedef struct {
	INT16         rbNumFields;
        INT16         rbNumCols;
	xrTextInfo    rbFont;
	INT32         rbFGColor;
	INT32         rbBGColor;
	Pixmap        rbTileId;
	xrItemData  * rbFields;
        INT8       ** rbLabels;
        INT8        * rbStates;
	INT16         rblastActive;
	INT16       * rbActive;
} xrRadioButtonData;


/*******************************************************************************
 *
 * Structure used to maintain the description of a static text instance
 *
 ******************************************************************************/

typedef struct {
	INT8     * textString;
	INT32      textAlignment;
	xrTextInfo textFont;
	INT32      stFGColor;
	INT32      stBGColor;
} xrStaticTextData;


/*******************************************************************************
 *
 * Structure used to maintain the description of a text edit instance
 *
 ******************************************************************************/

typedef struct {
	INT8        * teLabel;
	INT8        * teString;
	INT32         teMaxChars;
	INT32         teInsertPos;
	RECTANGLE     teFrameRect;
	POINT         teTextPt;
	POINT         teLabelPt;
	INT8          teCursorOn;
        INT8          teInsertMode;
        INT8          teInsertState;
        INT8          teLabelPosition;
        INT32         teSpaceWidth;
	xrTextInfo    teFont;
	INT32         teFGColor;
	INT32         teBGColor;
	Pixmap        teTileId;
        INT8          teCellWidth;
} xrTextEditData;


/*******************************************************************************
 *
 * Structure used to maintain the description of a scrollbar instance
 *
 ******************************************************************************/

typedef struct {
	INT8           orientation;
	RECTANGLE      arrow1;
	RECTANGLE      arrow2;
	RECTANGLE      slideArea;
	RECTANGLE      slideBox;
	POINT          arrow1_vl[9];
	POINT          arrow2_vl[9];
        INT16          pointCount;
	INT32          sbFGColor;
	INT32          sbBGColor;
	Pixmap         sbTileId;
        xrSBParameters configuration;
} xrScrollBarData;


/*******************************************************************************
 *
 * Structure used to maintain the description of a titlebar instance
 *
 ******************************************************************************/

typedef struct {
	INT8        * titleName;
	RECTANGLE     gadgetRect1;
	RECTANGLE     gadgetRect2;
	RECTANGLE     titleRect;
	POINT         titleLoc;
	POINT         iconLoc1;
	POINT         iconLoc2;
	xrTextInfo    titleFont;
        INT32         tbFGColor;
        INT32         tbBGColor;
	INT8        * gadgetIcon1;
	INT8        * gadgetIcon2;
	Pixmap        titleTileId;
        INT32         titleLen;
} xrTitleBarData;


/*******************************************************************************
 *
 * Structure used to maintain the description of a static raster instance
 *
 ******************************************************************************/

typedef struct {
	Pixmap   srRasterId;
} xrStaticRasterData;


/*******************************************************************************
 *
 * Structures used to maintain the description of a push button instance
 *
 ******************************************************************************/


typedef struct {
	INT16        pbNumFields;
        INT16        pbNumCols;
	xrTextInfo   pbFont;
	INT32        pbFGColor;
	INT32        pbBGColor;
	Pixmap       pbTileId;
	xrItemData * pbFields;
        INT8      ** pbLabels;
        INT8       * pbStates;
        INT16        pbDefaultButton;
        INT16        pbActiveButton;
	INT16        pbBorder;
} xrPushButtonData;




/*
 *  Structures used internally by the input, editor, and editor groups.
 */

typedef struct _xrWindowFunct
{
   INT8  processFlag;
   xrPFI funct;
   INT32 instance;
   INT32 message;
   INT32 eventCount;
   xrWindowEvent * eventList;
   struct _xrWindowFunct * nextFunct;
} xrWindowFunct;

 
typedef struct
{
   RECTANGLE windowRect;
   Pixmap    foreTile;
   Pixmap    backTile;
   xrWindowFunct functList;
   xrEditorGroup groupList;
   xrEditorGroup * activeGroup;
} xrWindow;


typedef struct
{
   INT32            meFGColor;
   INT32            meBGColor;
   INT32            numItems;
   INT32          * itemTypes;
   INT8          ** menuStrings;
   INT32            stringHeight;
   INT32          * stringLengths;
   INT16          * keybdEquiv;
   xrEvent       ** itemEvent;
   xrPFI          * itemFunct;
   struct _xrMenu ** popupMenu;

   xrEditor       * menuTitleBar;
   xrTextInfo       menuTextInfo;
   INT32          * itemPosy;
   INT32            itemPosx;
   INT32            kbdeqPosx;
   INT32            popupPosx;
   INT32            lineThickness;
   INT32            highlightStart;
   INT32            highlightLength;
   Pixmap           menuTile;

   INT32            menuArea;
   INT32            menuItem;

} xrMenuData;

