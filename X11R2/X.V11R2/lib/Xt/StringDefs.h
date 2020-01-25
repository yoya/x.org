/*
* $Header: StringDefs.h,v 1.31 88/02/26 12:51:19 swick Exp $
*/

/***********************************************************
Copyright 1987, 1988 by Digital Equipment Corporation, Maynard, Massachusetts,
and the Massachusetts Institute of Technology, Cambridge, Massachusetts.

                        All Rights Reserved

Permission to use, copy, modify, and distribute this software and its 
documentation for any purpose and without fee is hereby granted, 
provided that the above copyright notice appear in all copies and that
both that copyright notice and this permission notice appear in 
supporting documentation, and that the names of Digital or MIT not be
used in advertising or publicity pertaining to distribution of the
software without specific, written prior permission.  

DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING
ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL
DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR
ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
SOFTWARE.

******************************************************************/

#ifndef _XtStringDefs_h_
#define _XtStringDefs_h_

/* Resource names */

#define XtNallowHoriz		"allowHoriz"
#define XtNallowVert		"allowVert"
#define XtNancestorSensitive	"ancestorSensitive"
#define XtNbackground		"background"
#define XtNbackgroundPixmap     "backgroundPixmap"
#define XtNborder		"borderColor"
#define XtNborderColor		"borderColor"
#define XtNborderPixmap		"borderPixmap"
#define XtNborderWidth		"borderWidth"
#define XtNcallback		"callback"
#define XtNdepth		"depth"
#define XtNdestroyCallback      "destroyCallback"
#define XtNeditType		"editType"
#define XtNfont			"font"
#define XtNforceBars		"forceBars"
#define XtNforeground		"foreground"
#define XtNfunction		"function"
#define XtNgripIndent		"gripIndent"
#define XtNheight		"height"
#define XtNhSpace		"hSpace"
#define XtNindex		"index"
#define XtNinnerHeight		"innerHeight"
#define XtNinnerWidth		"innerWidth"
#define XtNinnerWindow		"innerWindow"
#define XtNinsertPosition	"insertPosition"
#define XtNinternalHeight	"internalHeight"
#define XtNinternalWidth	"internalWidth"
#define XtNjustify		"justify"
#define XtNlabel		"label"
#define XtNlength		"length"
#define XtNlowerRight		"lowerRight"
#define XtNmappedWhenManaged	"mappedWhenManaged"
#define XtNmaximumLength	"maximumLength"
#define XtNmenuEntry		"menuEntry"
#define XtNname			"name"
#define XtNnotify		"notify"
#define XtNorientation		"orientation"
#define XtNparameter		"parameter"
#define XtNpopupCallback	"popupCallback"
#define XtNpopdownCallback	"popdownCallback"
#define XtNreverseVideo		"reverseVideo"
#define XtNscrollProc		"scrollProc"
#define XtNscrollDCursor	"scrollDownCursor"
#define XtNscrollHCursor	"scrollHorizontalCursor"
#define XtNscrollLCursor	"scrollLeftCursor"
#define XtNscrollRCursor	"scrollRightCursor"
#define XtNscrollUCursor	"scrollUpCursor"
#define XtNscrollVCursor	"scrollVerticalCursor"
#define XtNselection		"selection"
#define XtNselectTypes		"selectTypes"
#define XtNsensitive		"sensitive"
#define XtNshown		"shown"
#define XtNskipAdjust		"skipAdjust"
#define XtNspace		"space"
#define XtNstring		"string"
#define XtNtextOptions		"textOptions"
#define XtNtextSink		"textSink"
#define XtNtextSource		"textSource"
#define XtNthickness		"thickness"
#define XtNthumb		"thumb"
#define XtNthumbProc		"thumbProc"
#define XtNtop			"top"
#define XtNtranslations		"translations"
#define XtNuseBottom		"useBottom"
#define XtNuseRight		"useRight"
#define XtNvalue		"value"
#define XtNvSpace		"vSpace"
#define XtNwidth		"width"
#define XtNwindow		"window"
#define XtNx			"x"
#define XtNy			"y"

/* Class types */ 

#define XtCBackground		"Background"
#define XtCBoolean		"Boolean"
#define XtCBorderColor		"BorderColor"
#define XtCBorderWidth		"BorderWidth"
#define XtCCallback             "Callback"
#define XtCColor		"Color"
#define XtCCursor		"Cursor"
#define XtCDepth		"Depth"
#define XtCEditType		"EditType"
#define XtCFile			"File"
#define XtCFont			"Font"
#define XtCForeground		"Foreground"
#define XtCFraction		"Fraction"
#define XtCFunction		"Function"
#define XtCHeight		"Height"
#define XtCHSpace		"HSpace"
#define XtCIndex		"Index"
#define XtCInterval		"Interval"
#define XtCJustify		"Justify"
#define XtCGripIndent		"GripIndent"
#define XtCLabel		"Label"
#define XtCLength		"Length"
#define XtCMappedWhenManaged	"MappedWhenManaged"
#define XtCMargin		"Margin"
#define XtCMax			"Max"
#define XtCMenuEntry		"MenuEntry"
#define XtCMin			"Min"
#define XtCNotify		"Notify"
#define XtCOrientation		"Orientation"
#define XtCParameter		"Parameter"
#define XtCPixmap		"Pixmap"
#define XtCPosition		"Position"
#define XtCReverseVideo		"ReverseVideo"
#define XtCScrollProc		"ScrollProc"
#define XtCScrollDCursor	"ScrollDownCursor"
#define XtCScrollHCursor	"ScrollHorizontalCursor"
#define XtCScrollLCursor	"ScrollLeftCursor"
#define XtCScrollRCursor	"ScrollRightCursor"
#define XtCScrollUCursor	"ScrollUpCursor"
#define XtCScrollVCursor	"ScrollVerticalCursor"
#define XtCSelection		"Selection"
#define XtCSensitive		"Sensitive"
#define XtCSelectTypes		"SelectTypes"
#define XtCSpace		"Space"
#define XtCString		"String"
#define XtCTextOptions		"TextOptions"
#define XtCTextPosition		"TextPosition"
#define XtCTextSink		"TextSink"
#define XtCTextSource		"TextSource"
#define XtCThickness		"Thickness"
#define XtCThumb		"Thumb"
#define XtCTranslations		"Translations"
#define XtCValue		"Value"
#define XtCVSpace		"VSpace"
#define XtCWidget		"Widget"
#define XtCWidth		"Width"
#define XtCWindow		"Window"
#define XtCX			"X"
#define XtCY			"Y"

/* Representation types */

#define XtRBoolean		"Boolean"
#define XtRLongBoolean		"LongBoolean"
#define XtRCallback		"Callback"
#define XtRColor		"Color"
#define XtRCursor		"Cursor"
#define XtRDefaultColor         "DefaultColor"
#define XtRDisplay		"Display"
#define XtREditMode		"EditMode"
#define XtRFile			"File"
#define XtRFloat		"Float"
#define XtRFont			"Font"
#define XtRFontStruct		"FontStruct"
#define XtRFunction		"Function"
#define XtRGeometry		"Geometry"
#define XtRInt			"Int"
#define XtRJustify		"Justify"
#define XtROrientation		"Orientation"
#define XtRPixel		"Pixel"
#define XtRPixmap		"Pixmap"
#define XtRPointer		"Pointer"
#define XtRString		"String"
#define XtRStringTable		"StringTable"
#define XtRTranslationTable     "TranslationTable"
#define XtRWidget		"Widget"
#define XtRWindow		"Window"


/* Boolean enumeration constants */

#define XtEoff			"off"
#define XtEfalse		"false"
#define XtEno			"no"
#define XtEon			"on"
#define XtEtrue			"true"
#define XtEyes			"yes"

/* Orientation enumeration constants */

#define XtEvertical		"vertical"
#define XtEhorizontal		"horizontal"

/* text edit enumeration constants */

#define XtEtextRead		"read"
#define XtEtextAppend		"append"
#define XtEtextEdit		"edit"

#endif _XtStringDefs_h_
