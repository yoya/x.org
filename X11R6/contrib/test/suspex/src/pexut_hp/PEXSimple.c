/* $XConsortium: PEXSimple.c,v 1.0 93/11/22 12:23:03 rws Exp $ */

/******************************************************************************/
/*                                                                            */
/*  (c) Copyright Hewlett-Packard Company, 1992, Fort Collins, Colorado       */
/*                                                                            */
/*                            All Rights Reserved                             */
/*                                                                            */
/*  Permission to use, copy, modify, and distribute this software and its     */
/*  documentation for any purpose and without fee is hereby granted,          */
/*  provided that the above copyright notices appear in all copies and that   */
/*  both the copyright notices and this permission notice appear in           */
/*  supporting documentation, and that the name of Hewlett-Packard not be     */
/*  used in advertising or publicity pertaining to distribution of the        */
/*  software without specific, written prior permission.                      */
/*                                                                            */
/*  HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD TO THIS         */
/*  SOFTWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/*  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Hewlett-Packard    */
/*  shall not be liable for errors contained herein or direct, indirect,      */
/*  special, incidental or consequential damages in connection with the       */
/*  furnishing, performance or use of this software.                          */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/* $Source: /xsrc/xc/unsupported/test/suspex/src/pexut_hp/RCS/PEXSimple.c,v $
/* $Date: 93/11/22 12:23:03 $
/* $Revision: 1.0 $
/*                                                                            */
/* Description:                                                               */
/*   The widget implementation file for the Xg/PEXSimple Motif widget         */
/*                                                                            */
/* Notes:                                                                     */
/*   This implementation file is structured for the benefit of those using    */
/*   the O'Reilly & Associates, Inc., "X Toolkit Intrinsics Programming       */
/*   Manual", in the order defined in that book's chapter on implementing     */
/*   widgets.  That order is:                                                 */
/*     - Header Files                                                         */
/*     - Resource List                                                        */
/*     - Action Function Declarations (i.e., forward references to later      */
/*       definitions)                                                         */
/*     - Actions Table                                                        */
/*     - Default Translation Table                                            */
/*     - Method Function Declarations (i.e., forward references to later      */
/*       definitions)                                                         */
/*     - Class Record Initialization                                          */
/*     - Method Function Definitions                                          */
/*     - Action Function Definitions                                          */
/*                                                                            */
/*   Additionally, the implementation follows the upward-compatibility re-    */
/*   quirements of XmResolvePartOffsets(), using the XmPartResource structure */
/*   data type and the XmPartOffset and XmField macros.  As the widget does   */
/*   not define additional constraints, it does not use XmConstraintOffset or */
/*   XmConstraintField macros.  (See the XmResolveAllPartOffsets(3X) man page */
/*   for more information.)  Some additional macros for improving the         */
/*   readability of XmField and XmConstraintField macro calls have been in-   */
/*   serted between the Header File section and the Resource List section.    */
/*                                                                            */
/*   Further, Motif convenience functions have been added after the other     */
/*   function definitions.                                                    */
/*                                                                            */
/*   Lastly, function declarations are differentiated for standards-based C   */
/*   and C++ versus Kernighan & Ritchie C using the compiler flags FUNCPROTO, */
/*   __STDC__, __cplusplus and c_plusplus.  These are grouped under a #define */
/*   NeedFunctionPrototypes in the PEXSimple.h header file.  Where "#if       */
/*   NeedFunctionPrototypes" appears, it refers to these compiler flags.      */
/*                                                                            */
/*   Additional references:                                                   */
/*     - for more information on using this widget, see                       */
/*       + the XgPEXSimple(3X) man page                                       */
/*       + on HP systems with the HP-PEX Developer Kit installed, see the     */
/*         file "/usr/lib/PEX5/utilities/README"                              */
/*     - for more information on using Motif widgets, see                     */
/*       + "Mastering OSF/Motif(tm) Widgets", by Donald L. McMinds, published */
/*         by Addison-Wesley                                                  */
/*       + the man pages for specific widgets (e.g., "man XmDrawnButton")     */
/*     - for more information on extending this widget, see                   */
/*       + the "X Toolkit Instrinsics Programming Manual", OSF/Motif Edition, */
/*         by Adrian Nye and Tim O'Reilly, published by O'Reilly & Associates */
/*       + the "OSF/Motif Programmer's Reference", from the Open Software     */
/*         Foundation, published by Prentice-Hall                             */
/*                                                                            */
/******************************************************************************/

/*******************************************************************************
*
* Header Files
*
*******************************************************************************/

/* ----- system and platform files -------------------------------------------*/
#include <stdio.h>

/* ----- Motif-specific files ------------------------------------------------*/
#include <Xm/Xm.h>
#include <Mrm/MrmPublic.h>

/* ----- PEX-specific files --------------------------------------------------*/
#include <X11/PEX5/PEXlib.h>
#include "pexutcmap.h"
#include "PEXSimpleP.h"

/*******************************************************************************
/*
/* Global Declarations
/*
*******************************************************************************/

/* ----- typedefs ------------------------------------------------------------*/
/*
** This structure type is the entry type for the SERVER_OVERLAY_VISUALS
** property.  The property contains an entry for each Visual on the screen
** of the root window where it is attached.
** 
** The fields are used as follows:
**
**	visualid		identifier of the Visual.
**
**	transparent_type	specifies the mechanism that controls
**				transparency in the Visual: one of None,
**				TransparentPixel, or TransparentMask.
**
**	value			pixel value that is transparent, if
**				the transparent_type is TransparentPixel.
**
**	layer			specifies the layer of the Visual.  Image
**				layer is 0, overlay layer is 1.
*/
	
typedef struct {
	VisualID	visualid;
	int		transparent_type;
	int		value;
	int		layer;
} overlay_visuals_type;

/*
** This is one of the possible values of the "transparent_type" element above.
*/
#ifndef TransparentPixel
#define TransparentPixel 1
#endif

/* ----- macros --------------------------------------------------------------*/
/*
*********************************************************************
** XmField macros, used in conjunction with XmResolvePartOffsets() **
*********************************************************************
**
** XmField macros for resources
*/
#define Renderer(w) XmField(w,offsets,XgPEXSimple,renderer,PEXRenderer)
#define Overlay(w) XmField(w,offsets,XgPEXSimple,overlay,Boolean)
#define Transparent(w) XmField(w,offsets,XgPEXSimple,transparent,int)
#define xgVisual(w) XmField(w,offsets,XgPEXSimple,visual,Visual *)
#define WmCmap(w) XmField(w,offsets,XgPEXSimple,wm_cmap,int)
#define RescalePolicy(w) XmField(w,offsets,XgPEXSimple,rescale_policy,int)

/*
** XmField macros for important inherited DrawingArea resources
*/
#define ExposeCallback(w) XmField(w,offsets,XmDrawingArea,expose_callback, \
	XtPointer)

/*
** XmField macros for important inherited Core resources
*/
#define BackgroundPixel(w) XmField(w,offsets,Core,background_pixel,Pixel)
#define Depth(w) XmField(w,offsets,Core,depth,int)
#define Colormap(w) XmField(w,offsets,Core,colormap,int)
#define Width(w) XmField(w,offsets,Core,width,Dimension)
#define Height(w) XmField(w,offsets,Core,height,Dimension)
#define xgScreen(w) XmField(w,offsets,Core,screen,Screen *)

/*
*******************
** Resource List **
*******************
**
** This widget uses the XmPartResource type, rather than the X Toolkit
** XtResource type used in O'Reilly & Associates "X Toolkit Intrinsics Program-
** ming Manual, Motif Edition" by Adrian Nye and Tim O'Reilly.  XmPartResource
** replaces XtResource to allow writing of upward-compatible applications and
** widgets.  For further explanation, see the XmResolveAllPartOffsets(3X) man
** page.
*/
static XmPartResource resources[] = {

	{XgNrenderer, XgCRenderer, XgRRenderer, sizeof(PEXRenderer),
		XmPartOffset(XgPEXSimple,renderer), XmRImmediate, None},

	{XgNoverlay, XgCOverlay, XmRBoolean, sizeof(Boolean),
		XmPartOffset(XgPEXSimple,overlay), XmRImmediate, False},

	{XgNtransparent, XgCTransparent, XmRInt, sizeof(int),
		XmPartOffset(XgPEXSimple,transparent), XmRImmediate,
		(XtPointer)-1},

	{XgNvisual, XgCVisual, XtRVisual, sizeof(Visual *),
		XmPartOffset(XgPEXSimple,visual), XmRImmediate, NULL},

	{XgNwmCmap, XgCWmCmap, XgRWmCmap, sizeof(int),
		XmPartOffset(XgPEXSimple,wm_cmap), XmRImmediate,
		(XtPointer)XgWM_CMAP_HIGH_PRIORITY},
};

/*
**********************************
** Action Function Declarations **
**********************************
**
*******************
** Actions Table **
*******************
**
*******************************
** Default Translation Table **
*******************************
**
** If this widget needed to define additional actions (for example, a different
** action for Btn1Down), action functions would be included in this implementa-
** tion file (probably after the method function definitions).  The forward
** reference declarations for those action functions, the actions table and
** the default translation table could be placed here.  XgPEXSimple doesn't
** have action functions, but action function declarations, an actions table
** and a default translation table could be added here if they were needed.
*/

/*
**********************************
** Method Function Declarations **
**********************************
*/

/*
** ANSI C or C++ [
*/
#if NeedFunctionPrototypes

static void ClassInitialize( void );

static void Initialize(
	Widget		rw,
	Widget		nw,
	ArgList		args,
	Cardinal	*num_args );

static void Realize(
	Widget			w,
	Mask			*mask,
	XSetWindowAttributes	*winattr );

static void Redisplay(
	XgPEXSimpleWidget	w,
	XEvent			*event,
	Region			region);

static void Destroy( XgPEXSimpleWidget w );

static Boolean SetValues(
	XgPEXSimpleWidget	current,
	XgPEXSimpleWidget	request,
	XgPEXSimpleWidget	new );

/*
** ] K&R C [
*/
#else

static void ClassInitialize();
static void Initialize();
static void Realize();
static void Redisplay();
static void Destroy();
static Boolean SetValues();

#endif /* ] */

/*
*********************************
** Class Record Initialization **
*********************************
*/
XgPEXSimpleClassRec xgPEXSimpleClassRec = {
	{                 /**********************
	                  ** core_class fields **
	                  **********************/
	(WidgetClass) &xmDrawingAreaClassRec, /* superclass                */
	"XgPEXSimple",                        /* class_name                */
	sizeof(XgPEXSimpleRec),               /* widget_size               */
	ClassInitialize,                      /* class_initialize          */
	NULL,                                 /* class_part_initialize     */
	False,                                /* class_inited              */
	Initialize,                           /* initialize                */
	NULL,                                 /* initialize_notify         */
	Realize,                              /* realize                   */
	NULL,                                 /* actions                   */
	0,                                    /* num_actions               */
	(XtResourceList)resources,            /* resources                 */
	XtNumber(resources),                  /* num_resources             */
	NULLQUARK,                            /* xrm_class                 */
	True,                                 /* compress_motion           */
	True,                                 /* compress_exposure         */
	True,                                 /* compress_enterleave       */
	False,                                /* visible_interest          */
	(XtWidgetProc) Destroy,               /* destroy                   */
	NULL,                                 /* resize                    */
	(XtExposeProc) Redisplay,             /* expose                    */
	(XtSetValuesFunc) SetValues,          /* set_values                */
	NULL,                                 /* set_values_hook           */
	XtInheritSetValuesAlmost,             /* set_values_almost         */
	NULL,                                 /* get_values_hook           */
	NULL,                                 /* accept_focus              */
	XtVersionDontCheck,                   /* version                   */
	NULL,                                 /* callback_private          */
	XmInheritTranslations,                /* tm_table                  */
	XtInheritQueryGeometry,               /* query_geometry            */
	NULL,                                 /* disp accelerator          */
	NULL                                  /* extension                 */
	},
	{                 /***************************
	                  ** composite_class fields **
	                  ***************************/
	XtInheritGeometryManager,             /* geometry_manager          */
	XtInheritChangeManaged,               /* change_managed            */
	XtInheritInsertChild,                 /* insert_child              */
	XtInheritDeleteChild,                 /* delete_child              */
	NULL,                                 /* extension                 */
	},
	{                 /****************************
	                  ** constraint_class fields **
	                  ****************************/
	NULL,                                 /* resource list             */
	0,                                    /* num resources             */
	0,                                    /* constraint size           */
	NULL,                                 /* init proc                 */
	NULL,                                 /* destroy proc              */
	NULL,                                 /* set values proc           */
	NULL,                                 /* extension                 */
	},
	{                 /*************************
	                  ** manager_class fields **
	                  *************************/
	XtInheritTranslations,                /* translations              */
	NULL,                                 /* syn_resources             */
	0,                                    /* num_get_resources         */
	NULL,                                 /* syn_cont_resources        */
	0,                                    /* num_get_cont_resources    */
	XmInheritParentProcess,               /* parent_process            */
	NULL,                                 /* extension                 */
	},
	{                 /******************************
	                  ** drawing_area_class fields **
	                  ******************************/
	NULL,                                 /* extension, aka. mumble    */
	},
	{                 /***************************
	                  ** pexsimple_class fields **
	                  ***************************/
	NULL,                                 /* extension                 */
	}
};

/*
** Establish a pointer to the PEXSimpleClassRec.
**
** "externaldef()" is a macro defined in <X11/Intrinsic.h>.  Generally, it
** is set to null in a #define statement and has no actual effect in the
** following statement.  See <X11/Intrinsic.h> for more information.
*/
externaldef(xgpexsimplewidgetclass) WidgetClass xgPEXSimpleWidgetClass =
	(WidgetClass) &xgPEXSimpleClassRec;

/*
** This declaration is needed for XmResolvePartOffsets().
*/
static XmOffsetPtr offsets; /* Part Offset table for XmResolvePartOffsets */

/*
*******************
** Class Methods **
*******************
**
** -----------------------------------------------------------------------------
** ConversionWarning is not a class method itself, but is used in the routines
** CvtStringToRescalePolicy and CvtStringtoWMCmap, referenced in the class
** method ClassInitialize.
**
** ConversionWarning is simply the way that this widget reports a problem try-
** ing to convert a resource definition string into a defined value for that
** resource.
**
** _XtDefaultAppContext() is a function from the Xt library.  Although it's
** a private toolkit call, it's used in this widget so that the toolkit can
** report a problem before the XtVaAppInitialize() call has completed.
** -----------------------------------------------------------------------------
*/

#if NeedFunctionPrototypes /* ANSI C or C++ [ */

static void ConversionWarning(String string, String type)

#else /* ] K&R C [ */

static void ConversionWarning(string, type)
	String string, type;

#endif /* ] */
{
	String params[2];
	Cardinal num_params = 2;

	params[0] = string;
	params[1] = type;
	XtAppWarningMsg((XtAppContext) _XtDefaultAppContext(),
	       "conversionError", "string", "XtToolkitError",
	       "Cannot convert string \"%s\" to type %s",
		params, &num_params);

} /* end ConversionWarning */

/*
** -----------------------------------------------------------------------------
** CvtStringToRescalePolicy is not a class method, but is referenced in the
** class method ClassInitialize, defined below.  CvtStringToRescalePolicy
** converts a resource definition string for the XgNrescalePolicy resource to
** one of the defined values for that resource.
** -----------------------------------------------------------------------------
*/
#if NeedFunctionPrototypes /* ANSI C or C++ [ */

static Boolean CvtStringToRescalePolicy(
	XrmValuePtr args,
	Cardinal *num_args,
	XrmValuePtr fromVal,
	XrmValuePtr toVal)

#else /* ] K&R C [ */

static Boolean CvtStringToRescalePolicy(args, num_args, fromVal, toVal)
	XrmValuePtr args;
	Cardinal *num_args;
	XrmValuePtr fromVal;
	XrmValuePtr toVal;

#endif /* ] */
{
	static int value;

	if (!strcasecmp((char *) fromVal->addr, "RESCALE_MINOR")) {
		value = XgRESCALE_MINOR;
/*  
**  XgRESCALE_MAJOR is not supported with the PEXSimple widget.  The following
**  else clause would be uncommented if this widget were extended to support
**  this resource value.
**
**	} else if (!strcasecmp((char *) fromVal->addr, "RESCALE_MAJOR")) {
**		value = XgRESCALE_MAJOR;
*/

/*  
**  XgRESCALE_NONE is not supported with the PEXSimple widget.  The following
**  else clause would be uncommented if this widget were extended to support
**  this resource value.
**
**	} else if (!strcasecmp((char *) fromVal->addr, "RESCALE_NONE")) {
**		value = XgRESCALE_NONE;
*/

/*  
**  XgRESCALE_DISTORT is not supported with the PEXSimple widget.  The follow-
**  ing else clause would be uncommented if this widget were extended to
**  support this resource value.
**
**	} else if (!strcasecmp((char *) fromVal->addr, "RESCALE_DISTORT")) {
**		value = XgRESCALE_DISTORT;
*/

	} else {
		ConversionWarning((String) fromVal->addr, "XgRescalePolicy");
		return False;
	}
	if (toVal->addr) {
		if (toVal->size < sizeof(int)) {
			return False;
		} else {
			*((int *) toVal->addr) = value;
		}
	} else {
		toVal->addr = (XtPointer) &value;
	}
	toVal->size = sizeof(int);
	return True;

} /* end CvtStringToRescalePolicy */

/*
** -----------------------------------------------------------------------------
** CvtStringToWMCmap is not a class method, but is referenced in the class
** method ClassInitialize, defined immediately after it.  CvtStringToWMCmap
** converts a resource definition string for the XgNwmCmap resource to one of
** the defined values for that resource.
** -----------------------------------------------------------------------------
*/
#if NeedFunctionPrototypes /* ANSI C or C++ [ */

static Boolean CvtStringToWMCmap(
	XrmValuePtr args,
	Cardinal *num_args,
	XrmValuePtr fromVal,
	XrmValuePtr toVal)

#else  /* ] K&R C [ */

static Boolean CvtStringToWMCmap(args, num_args, fromVal, toVal)
	XrmValuePtr args;
	Cardinal *num_args;
	XrmValuePtr fromVal;
	XrmValuePtr toVal;

#endif /* ] */
{
	static int value;

	if (!strcasecmp((char *) fromVal->addr, "WM_CMAP_NONE")) {
		value = XgWM_CMAP_NONE;
	} else if (!strcasecmp((char *) fromVal->addr, "WM_CMAP_LOW_PRIORITY")){
		value = XgWM_CMAP_LOW_PRIORITY;
	} else if (!strcasecmp((char *) fromVal->addr,"WM_CMAP_HIGH_PRIORITY")){
		value = XgWM_CMAP_HIGH_PRIORITY;
	} else {
		ConversionWarning((String) fromVal->addr, "XgWmCmap");
		return False;
	}
	if (toVal->addr) {
		if (toVal->size < sizeof(int)) {
			return False;
		} else {
			*((int *) toVal->addr) = value;
		}
	} else {
		toVal->addr = (XtPointer) &value;
	}
	toVal->size = sizeof(int);
	return True;

} /* end CvtStringToWMCmap */

/*
********************************************************************************
** ClassInitialize class method
**
** This method is called once, when the first instance of a widget in the
** PEXSimple class is created.
**
** PEXSimple's ClassInitialize method simply calls the Motif routine to resolve
** offsets into widget class records (used to maintain upward-compatible access
** to widget instance and class records by applications and widgets), and
** registers the two additional conversion routines for this widget.
********************************************************************************
*/
#if NeedFunctionPrototypes /* ANSI C or C++ [ */

static void ClassInitialize( void )

#else  /* ] K&R C [ */

static void ClassInitialize()

#endif /* ] */
{

	XmResolvePartOffsets(xgPEXSimpleWidgetClass, &offsets);
	XtAddConverter(XtRString, XgRRescalePolicy,
		(XtConverter) CvtStringToRescalePolicy, NULL, 0);
	XtAddConverter(XtRString, XgRWmCmap,
		(XtConverter) CvtStringToWMCmap, NULL, 0);
	return;

} /* end ClassInitialize */

/*
********************************************************************************
** Initialize class method
**
** This method is called each time an instance of a widget is created using
** a call such as XtVaCreateManagedWidget.
**
** An initialize method normally checks for validity of resource values.
** However, all the PEXSimple resource values that can be specified are either
** uncheckable (the renderer), can only be converted to defined values
** (XgNwmCmap and XgNrescalePolicy), can only be set to a defined value
** (XgNoverlay) or will be checked later (XgNvisual).
**
** However, the Initialize method does check the XmNwidth and XmNheight
** resources.  If they are less than or equal to zero, Initialize resets them
** to 1.  This mimics the behavior of XgPEXSimple's superclass, XmDrawingArea.
**
** After this check, the Initalize method calls PEXInitialize().  Note that
** the PEXInitialize() call is harmless even if PEXInitialize() has been called 
** previously, as the PEXlib specification states: "PEXInitialize can be called
** multiple times; subsequent calls will result in the same return value as the
** first call."
**
********************************************************************************
*/
#if NeedFunctionPrototypes /* ANSI C or C++ [ */

static void Initialize( Widget rw,
			Widget nw,
			ArgList	args,
			Cardinal *num_args )

#else  /* ] K&R C [ */

static void Initialize( rw, nw, args, num_args )
	Widget rw;
	Widget nw;
	ArgList args;
	Cardinal *num_args;

#endif /* ] */
{
	PEXExtensionInfo	*info_return;
	char			err_string[PEXErrorStringLength];
	String			warning_params[2];
	Cardinal		num_params = 2;

	if ( Width(nw) <= 0 ) Width(nw) = 1;
	if ( Height(nw) <= 0 ) Height(nw) = 1;
	if ( PEXInitialize( XtDisplay(nw),
			    &info_return,
			    PEXErrorStringLength,
			    err_string )
	     != 0 ) {
		warning_params[0] = DisplayString( XtDisplay( nw ) );
		warning_params[1] = err_string;
		XtAppWarningMsg( XtWidgetToApplicationContext( nw ),
				 "initializationError",
				 "PEXInitializeError",
				 "XtToolkitError",
				 "Unable to initialize PEX on display \"%s\": %s",
				 warning_params,
				 &num_params );
		return;
	}

	return;

} /* end Initialize */

/*
** -----------------------------------------------------------------------------
** UpdateWmCmap() is not a class method, but is referenced by the Realize and
** SetValues class methods, defined below.  This routine sets the
** WM_COLORMAP_WINDOWS property using the XgNwmCmap resource.
**
** DESCRIPTION:
**   Only the widget id is passed into this function.  The routine goes
**   through the following steps:
**
**   Check to see if the widget is realized before setting the property.
**   (If it is not, nothing can be done, so the function exits.)
**
**   If the XgNwmCmap resource is set to XgWM_CMAP_NONE, do nothing.
**
**   If the XgNwmCmap resource is set to XgWM_CMAP_LOW_PRIORITY or
**   XgWM_CMAP_HIGH_PRIORITY, then perform the following steps.
**
**   Follow the widget's instance hierarchy until the top level widget is
**   found.
**   
**   Check for an existing X window property value.
**
**   Allocate memory for setting this X window property.
**
**   Set this X window property to the new value based on the colormap 
**   priority requested.
**
**   Free the memory allocated by this routine.
**
**
** -----------------------------------------------------------------------------
*/
#if NeedFunctionPrototypes /* ANSI C or C++ [ */

static void UpdateWmCmap( Widget w )

#else  /* ] K&R C [ */

static void UpdateWmCmap(w)
	Widget w;

#endif /* ] */
{
	Window *colormap_windows, *colormap_windows_return;
	Window two_colormap_windows[2], topwindow;
	int i, count, count_return, top_listed;
	Widget top;

	/*
	** Can't do anything unless the widget is realized, as no window
	** would exist to associate with the WM_COLORMAP_WINDOWS property.
	*/
	if (!XtIsRealized(w))
		return;

	/*
	** Update the WM_COLORMAP_WINDOWS property.
	*/
	switch(WmCmap(w)) {

	/*
	** Make no change to WM_COLORMAP_WINDOWS for XgWM_CMAP_NONE
	*/
	case XgWM_CMAP_NONE:
		break;

	/*
	** Add this widget to the bottom of WM_COLORMAP_WINDOWS for
	** XgWM_CMAP_LOW_PRIORITY
	*/
	case XgWM_CMAP_LOW_PRIORITY:
		/*
		** Find the top level widget.
		*/
		top = w;
		while (!XtIsShell(top)) {
			top = XtParent(top);
		}

		/*
		** Get the window ID of the top level widget.
		*/
		topwindow = XtWindow(top);

		/*
		** Check for an existing property.
		*/
		if (XGetWMColormapWindows(XtDisplay(w), topwindow,
		    &colormap_windows_return, &count_return)) {

			if (NULL == (colormap_windows =
			    (Window *) XtCalloc(count_return+1,
			    sizeof(Window)))) {
				XFree((char *)colormap_windows_return);
				return;
			}
			count = 0;
			for (i=0; i<count_return; i++) {
				if (colormap_windows_return[i] != XtWindow(w)) {
		    			colormap_windows[count++] =
					  colormap_windows_return[i];
				}
			}
			colormap_windows[count++] = XtWindow(w);

			XSetWMColormapWindows(XtDisplay(w), topwindow,
			  colormap_windows, count);
			XtFree((char *)colormap_windows);
			XFree((char *)colormap_windows_return);
		} else {
			two_colormap_windows[0] = XtWindow(w);
			XSetWMColormapWindows(XtDisplay(w), topwindow,
			  two_colormap_windows, 1);
		}
		break;
	/*
	** Add this widget to the top of WM_COLORMAP_WINDOWS for
	** XgWM_CMAP_LOW_PRIORITY
	*/
	case XgWM_CMAP_HIGH_PRIORITY:
		/*
		** Find the top level widget.
		*/
		top = w;
		while (!XtIsShell(top)) {
			top = XtParent(top);
		}

		/*
		** Get the window ID of the top level widget.
		*/
		topwindow = XtWindow(top);

		/*
		** Check for an existing property.
		*/
		if (XGetWMColormapWindows(XtDisplay(w), topwindow,
		    &colormap_windows_return, &count_return)) {

			if (NULL == (colormap_windows =
			    (Window *) XtCalloc(count_return+2,
			    sizeof(Window)))) {
				XFree((char *)colormap_windows_return);
				return;
			}

			top_listed = 0;
			for (i=0; i<count_return; i++) {
				if (colormap_windows_return[i] == topwindow) {
		    			top_listed = 1;
				}
			}

			colormap_windows[0] = XtWindow(w);
			count = 1;
			if (!top_listed) {
				/*
				** The top level window was implicitly of
				** higher priority than the windows listed in
				** the property.  Place it just below this
				** widget's priority in the new list.
			 	*/
				colormap_windows[count++] = topwindow;
			}
			for (i=0; i<count_return; i++) {
				if (colormap_windows_return[i] != XtWindow(w)) {
		    			colormap_windows[count++] =
					  colormap_windows_return[i];
				}
			}

			XSetWMColormapWindows(XtDisplay(w), topwindow,
			  colormap_windows, count);
			XtFree((char *)colormap_windows);
			XFree((char *)colormap_windows_return);
		} else {
			two_colormap_windows[0] = XtWindow(w);
			two_colormap_windows[1] = topwindow;
			XSetWMColormapWindows(XtDisplay(w), topwindow,
			  two_colormap_windows, 2);
		}
		break;
	}

} /* end UpdateWmCmap */

/*******************************************************************************
** ScreenNumOf() is not a class method, but is used in the Realize class
** method, defined below.  ScreenNumOf() returns the screen number for a
** specified display and screen pointer pair.  Since there is no straight-
** forward way to get from a screen pointer to a screen number, this function
** determines the screen number in a roundabout fashion.
*******************************************************************************/

#if NeedFunctionPrototypes /* ANSI C or C++ [ */

int ScreenNumOf( Display *display,
		 Screen *screen_ptr )

#else /* ] K&R C [ */

int ScreenNumOf( display, screen_ptr )
	Display *display;
	Screen *screen_ptr;

#endif /* ] */

{
	int screen_count;	/* number of screens defined for this display */
	int loop;		/* loop counter variable                      */
	int screen_num;		/* screen number, returned as function value  */

	screen_count = ScreenCount( display );
	for (loop = 0; loop < screen_count; loop++ ) {
		if ( RootWindowOfScreen( ScreenOfDisplay( display, loop ) ) ==
		     RootWindowOfScreen( screen_ptr ) ) {
			screen_num = loop;
			break;
		}
	}
	return screen_num;
} /* end ScreenNumOf */

/*******************************************************************************
** GetOverlayVisualsProperty() is not a class method, but is used in the Realize
** class method, defined below.  GetOverlayVisualsProperty() retrieves the
** SERVER_OVERLAY_VISUALS X window property list (used to check whether a visual
** supports overlays and to determine the transparent pixel value for an
** overlay).  It returns True if the property list was built successfully, False
** otherwise.
*******************************************************************************/

#if NeedFunctionPrototypes /* ANSI C or C++ [ */

Boolean GetOverlayVisualsProperty ( Display *display,
				    Screen *screen,
				    int *property_count_return,
				    overlay_visuals_type
				      **property_data_return )

#else /* ] K&R C [ */

Boolean GetOverlayVisualsProperty ( display, screen, property_count_return,
				    property_data_return )
	Display *display;
	Screen *screen;
	int *property_count_return;
	overlay_visuals_type **property_data_return;

#endif /* ] */

{
	Atom		property_atom;
	Atom	    	actual_type;
	int		actual_format;
	unsigned long   item_count;
	unsigned long   item_count_return;
	unsigned long   bytes_unread;
	int		result;

	/*
	** Get the atom from the X server.  If it doesn't exist,
	** neither does the property.
	*/

	property_atom = XInternAtom(display, "SERVER_OVERLAY_VISUALS", True);
	if ( property_atom == None ) {
	    return False;
	}


	/* 
	** The property atom exists-- fetch the property list.
	*/

	bytes_unread = 0;
	item_count = sizeof( overlay_visuals_type ) / 4;
	do {
	    item_count += ( bytes_unread + 3 ) / 4;
	    result = XGetWindowProperty( display, 
					 RootWindowOfScreen( screen ),
					 property_atom,
					 0,
					 item_count,
					 False, 
					 property_atom,
					 &actual_type,
					 &actual_format, 
					 &item_count_return,
					 &bytes_unread,
					 (unsigned char **)
					   property_data_return );

	} while ( ( result == Success ) && ( bytes_unread > 0 ) );

	if ( result != Success )
	    return False;

	*property_count_return = item_count_return /
				 ( sizeof(overlay_visuals_type ) / 4 );

	return True;

} /* end GetOverlayVisualsProperty */

/*
********************************************************************************
** Realize class method
**
** DESCRIPTION:
**  This function realizes the PEXSimple widget.
**
**  The Motif inputs of the parent widget id, widget name, argument list 
**  (which contains all resource values and additional callback functions 
**  that the application defines), and the number of arguments in this 
**  list are passed in to this function.  The following steps are followed to
**  realize the widget:
**
**  Call the PEXUt colormap and visual utilities, using the resource values
**  for XgNvisual and XgNoverlay (if specified) as soft criteria, to create a
**  colormap and visual.  Choose a visual that supports double-buffering, if
**  possible.
**
**  Use X Toolkit intrinsics to create the window.
**
**  Set the WM_COLORMAP_WINDOWS property using the XgNWmCmap resource.
**
**  If an existing renderer has not been specified for the XgNrenderer resource
**  value, create a renderer and set up a default color index table associated
**  with it.
**
********************************************************************************
*/

#if NeedFunctionPrototypes /* ANSI C or C++ [ */

static void Realize(

	Widget w,
	Mask *mask,
	XSetWindowAttributes *winattr)

#else  /* ] K&R C [ */

static void Realize(w, mask, winattr)
	Widget w;
	Mask *mask;
	XSetWindowAttributes *winattr;

#endif /* ] */
{

	PEXUtVisualCriteria	vis_criteria;
	XVisualInfo		*vis_info_ptr;
	long			vinfo_mask;
	XVisualInfo		vinfo_template;
	int			number;
	unsigned long		tgt_count;
	PEXRenderingTarget	*targets;
	int			prop_count;
	overlay_visuals_type	*prop_data;
	int			loop;
	XVisualInfo		visual_info;
	XStandardColormap	colormap_info;
	PEXColorApproxEntry	color_approx_info;
	unsigned int		unmet_criteria;
	Atom			std_prop_atom;
	int			screen_num, pexut_return;
	unsigned long		renderer_mask = 0;
	PEXRendererAttributes	renderer_attrs;
	String			app_warn_msg_params[1];
	Cardinal		app_warn_msg_num_params = 1;

	/*
	** If the application supplied the visual, make sure it's usable.  Set
	** XgNvisual to NULL if the visual is not usable.  At the end, check if
	** XgNvisual is NULL.  If so, report visual rejection to the
	** application.
	*/
	if ( xgVisual(w) != NULL ) {
		/*
		** If XgNvisual is not the default visual, validate that it's
		** a valid X visual.
		*/
		if ( XVisualIDFromVisual( xgVisual(w) ) !=
		     XVisualIDFromVisual( DefaultVisualOfScreen(
					    xgScreen(w) ) ) ) {
			vinfo_mask = VisualIDMask;
			vinfo_template.visualid = XVisualIDFromVisual(
						     xgVisual(w) );
			vis_info_ptr = XGetVisualInfo( XtDisplay(w),
						       vinfo_mask,
						       &vinfo_template,
						       &number );
			if ( vis_info_ptr == NULL ) {
				xgVisual(w) = NULL;
			} else {
				XFree( (char *) vis_info_ptr );
			}
		}
		
		/*
		** If XgNvisual is still not NULL, validate that it's usable
		** with PEX.
		*/
		if ( xgVisual(w) != NULL ) {
			if ( PEXMatchRenderingTargets( XtDisplay(w),
						       RootWindowOfScreen(
							 xgScreen(w) ),
						       Depth(w),
						       PEXWindowDrawable,
						       xgVisual(w),
						       1,
						       &tgt_count,
						       &targets ) == 0 ) {
				xgVisual(w) = NULL;
			}
			if ( targets != NULL ) {
				XFree ( (char *) targets );
			}
		}
	
		/*
		** If XgNvisual is _still_ not NULL, use
		** PEXUtGetStandardColormapInfo() to obtain colormap
		** information for PEXUtVerifyColorApproximation().
		**
		** This step is performed here rather than just before the
		** PEXUtVerifyColorApproximation() call because this
		** information is automatically obtained by PEXUtFindVisual(),
		** called when XgNvisual was not set to a valid visual.  When
		** the application has specified a valid visual however,
		** colormap information must be separately obtained using
		** PEXUtGetStandardColormapInfo().
		*/
		if ( xgVisual(w) != NULL ) {
			vinfo_mask = VisualIDMask;
			vinfo_template.visualid = XVisualIDFromVisual(
						    xgVisual(w) );
			vis_info_ptr = XGetVisualInfo( XtDisplay(w),
						      vinfo_mask,
						      &vinfo_template,
						      &number );
			/*
			** Checking for a NULL return is just a sanity check,
			** since XgNvisual is either the default visual or
			** XGetVisualInfo() has already been called
			** successfully.
			*/
			if ( vis_info_ptr != NULL ) {
				if ( PEXUtGetStandardColormapInfo(
						XtDisplay(w),
						vis_info_ptr,
						&colormap_info,
						&color_approx_info,
						&std_prop_atom )
				     == PEXUtSuccess ) {
					Depth(w) = vis_info_ptr->depth;
				} else {
					xgVisual(w) = NULL;
				}
				XFree( (char *) vis_info_ptr );
			} else {
				xgVisual(w) = NULL;
			}
		}
	
		/*
		** If XgNvisual got set to NULL somewhere above, the specified
		** visual was rejected, which needs to be reported to the
		** application.
		*/
		if ( xgVisual(w) == NULL ) {
			app_warn_msg_num_params = 0;
			XtAppWarningMsg( XtWidgetToApplicationContext(w),
					 "realizeError",
					 "PEXColormapError",
					 "XtToolkitError",
					 "Cannot use specified visual",
					 app_warn_msg_params,
					 &app_warn_msg_num_params );
		}
	}

	/*
	** At this point, either XgNvisual is NULL, or it's a valid visual.
	**
	** If XgNoverlay and a valid XgNvisual were both specified, validate
	** that the visual supports overlays.  If it does not, the visual takes
	** precedence.
	*/
	if ( ( xgVisual(w) != NULL ) && ( Overlay(w) == True ) ) {
		/*
		** Use the Boolean function GetOverlayVisualsProperty(),
		** defined above, to retrieve the property list that specifies
		** which visuals support overlays.  If
		** GetOverlayVisualsProperty() cannot build the property
		** list, it returns False.
		*/
		if ( GetOverlayVisualsProperty ( XtDisplay(w),
						 xgScreen(w),
						 &prop_count,
						 &prop_data ) == True ) {
			/*
			** Loop through the property list looking for a visual
			** that matches the specified visual.
			*/
			Overlay(w) = False;
			Transparent(w) = -1;
			for ( loop = 0; loop < prop_count; loop++ ) {
				if ( prop_data[loop].visualid ==
				     XVisualIDFromVisual( xgVisual(w) ) ) {
					/*
					** The specified visual _does_ support
					** overlays.
					**/
					Overlay(w) = True;
	
					/*
					** Set XgNtransparent appropriately.
					*/
					if ( prop_data[loop].transparent_type ==
					     TransparentPixel ) {
						Transparent(w) =
						  prop_data[loop].value;
					}
					break;
				}
			}
	
			/*
			** Free any allocated resources.
			*/
			if ( prop_data != NULL ) {
				XFree( (char *) prop_data );
			}
		} else {
			/*
			** If the overlay visuals property does not exist,
			** assume that the visual cannot support overlays.
			*/
			Overlay(w) = False;
		}
	
		/*
		** If Overlay(w) got set to False, report that the visual does
		** not support overlays to the application.
		*/
		if ( Overlay(w) == False ) {
			app_warn_msg_num_params = 0;
			XtAppWarningMsg( XtWidgetToApplicationContext( w ),
					 "realizeError",
					 "PEXColormapError",
					 "XtToolkitError",
					 "Specified visual does not support overlays",
					 app_warn_msg_params,
					 &app_warn_msg_num_params );
		} /* end Overlay(w) == False */
	} /* end ( ( xgVisual(w) != NULL ) && ( Overlay(w) == True ) ) */
	
	/*
	** If a valid visual was not specified, use PEXUtFindVisual() to find
	** one.
	*/
	if ( xgVisual(w) == NULL ) {
		vis_criteria.hard_criteria_mask = 0;
		vis_criteria.soft_criteria_mask = 0;
		
		/*
		** Always specify the standard colormap property search as a
		** soft criterion.
		*/
		vis_criteria.soft_criteria_mask |=
				PEXUtStandardColormapProperty;
		vis_criteria.standard_colormap_property = True;
	
		/*
		** If Overlay(w) is True, make it a soft criterion for
		** PEXUtFindVisual().
		*/
		if ( Overlay(w) == True ) {
			vis_criteria.soft_criteria_mask |= PEXUtLayer;
			vis_criteria.layer = PEXUtOverlay;
		}
	
		/*
		** Try to find a visual that supports PEX and X doublebuffering.
		*/
		vis_criteria.hard_criteria_mask |=
				PEXUtDoubleBufferingCapability;
		vis_criteria.double_buffering_capability = PEXUtDbufferPEXAndX;
	
		/*
		** Call PEXUtFindVisual().  It requires a screen number, so get
		** that first.
		*/
		screen_num = ScreenNumOf( XtDisplay(w), xgScreen(w) );
		pexut_return = PEXUtFindVisual( XtDisplay(w),
						screen_num,
						&vis_criteria,
						&visual_info,
						&colormap_info,
						&color_approx_info,
						&unmet_criteria,
						&std_prop_atom );
		
		/*
		** If PEXUtFindVisual fails to find a visual with both PEX and
		** X doublebuffering, try to find one with just PEX double-
		** buffering.
		*/
	 	if ( pexut_return == PEXUtCriteriaFailure ) {
			vis_criteria.double_buffering_capability =
			  PEXUtDbufferPEX;
			pexut_return = PEXUtFindVisual( XtDisplay(w),
							screen_num,
							&vis_criteria,
							&visual_info,
							&colormap_info,
							&color_approx_info,
							&unmet_criteria,
							&std_prop_atom );
		}
		
		/*
		** If PEXUtFindVisual() was still unable to find a visual, try
		** again for a visual that doesn't support double-buffering.
		*/
		if ( pexut_return == PEXUtCriteriaFailure ) {
			vis_criteria.hard_criteria_mask = 0;
			pexut_return = PEXUtFindVisual( XtDisplay(w),
							screen_num,
							&vis_criteria,
							&visual_info,
							&colormap_info,
							&color_approx_info,
							&unmet_criteria,
							&std_prop_atom );
		}
	
		/*
		** If PEXUtFindVisual() was still unable to find a visual,
		** report an error and return.
		*/
		if ( ( pexut_return != PEXUtSuccess ) &&
		     ( pexut_return != PEXUtQualifiedSuccess ) ) {
			app_warn_msg_num_params = 1;
			app_warn_msg_params[0] = DisplayString( XtDisplay(w) );
			XtAppWarningMsg( XtWidgetToApplicationContext( w ),
					 "noColormap",
					 "PEXColormapError",
					 "XtToolkitError",
					 "Unable to find visual for display \"%s\"",
					 app_warn_msg_params,
					 &app_warn_msg_num_params );
			return;
		}
		
		/*
		** If PEXUtFindVisual() succeeded, set XgNvisual and XmNdepth
		** with the values returned by PEXUtFindVisual().
		*/
		xgVisual(w) = visual_info.visual;
		Depth(w) = visual_info.depth;
		
		/*
		** If XgNoverlay was True, set it to False if PEXUtLayer was
		** one of the unmet criteria returned by PEXUtFindVisual().
		** If PEXUtLayer was met, set XgNtransparent appropriately.
		*/
		if ( Overlay(w) == True )  {
			if ( ( pexut_return == PEXUtQualifiedSuccess ) &&
			     ( ( unmet_criteria & PEXUtLayer ) == PEXUtLayer ) )
			{
				Overlay(w) = False;
			} else {
				/*
				** Use the SERVER_OVERLAY_VISUALS property to
				** set Transparent(w) appropriately.
				*/
				if ( GetOverlayVisualsProperty ( XtDisplay(w),
								 xgScreen(w),
								 &prop_count,
								 &prop_data )
				     == True ) {
					/*
					** Loop through the property list
					** looking for the visual that matches
					** XgNvisual.  Setting Overlay(w) to
					** False should be superfluous, since
					** PEXUtFindVisual() says the visual
					** supports overlays, but it's left in
					** as a sanity check.
					*/
					Overlay(w) = False;
					Transparent(w) = -1;
					for ( loop = 0;
					      loop < prop_count;
					      loop++ ) {
						if ( prop_data[loop].visualid ==
						     XVisualIDFromVisual(
						       xgVisual(w) ) ) {
							/*
							** The specified visual
							** _does_ support
							** overlays.
							**/
							Overlay(w) = True;
			
							/*
							** Set XgNtransparent.
							*/
							if ( prop_data[loop].transparent_type ==
							     TransparentPixel )
							{
								Transparent(w) =
								  prop_data[loop].value;
							}
							break;
						}
					}
				} else {
					/*
					** If the overlay visuals property does
					** not exist, assume that the visual
					** cannot support overlays.  Again,
					** this is just a sanity check.
					*/
					Overlay(w) = False;
				} /* end if/else GetOverlayVisualsProperty() ==
					 True */
			} /* end if/else pexut_return == PEXUtQualifiedSuccess
				 && unmet_criteria & PEXUtLayer == PEXUtLayer */
		} /* end if Overlay(w) == True */
	} /* end if xgVisual(w) == NULL */

	/*
	** A visual has been found.  Now verify the color approximation.
	*/
	pexut_return = PEXUtVerifyColorApproximation( XtDisplay(w),
						      &color_approx_info,
						      &visual_info);
	if ( pexut_return != PEXUtSuccess ) {
		app_warn_msg_params[0] = DisplayString( XtDisplay(w) );
		XtAppWarningMsg( XtWidgetToApplicationContext( w ),
				 "noColormap",
				 "PEXColormapError",
				 "XtToolkitError",
				 "Unable to verify color approximation for display \"%s\"",
				 app_warn_msg_params,
				 &app_warn_msg_num_params );
		return;
	}

	/*
	** PEXUtFindVisual() or PEXUtGetStandardColormapInfo() may have returned
	** a colormap id from an entry in the colormap property list.  If not,
	** we need to create a colormap.
	*/
	if ( colormap_info.colormap == None ) {
		pexut_return = PEXUtCreateColormap( XtDisplay(w),
						    &visual_info,
						    &color_approx_info,
						    &(winattr->colormap) );

		if ( pexut_return != PEXUtSuccess ) {
			app_warn_msg_params[0] = DisplayString( XtDisplay(w) );
			XtAppWarningMsg( XtWidgetToApplicationContext( w ),
					 "noColormap",
					 "PEXColormapError",
					 "XtToolkitError",
					 "Unable to create colormap for display \"%s\"",
				 	app_warn_msg_params,
				 	&app_warn_msg_num_params );
			return;
		}
	} else {
		winattr->colormap = colormap_info.colormap;
	}

	/* Create the window.  Some implementations will not allow a
	** non-default visual and colormap to be specified unless the
        ** background pixel and border pixel are also specified (even if they
        ** are ignored for this particular window).
	*/
	winattr->border_pixel = 0;
	winattr->background_pixel = BackgroundPixel(w);

	/*
	**  Although it would be desirable to call XmDrawingArea's realize
	** method, for example, by specifying the following:
	**
	** #define Superclass (&xmDrawingAreaClassRec)
	** (*SuperClass->core_class.realize) (w, mask, winattr);
	**
	** this can't be used because there is no way to tell XmDrawingArea's
	** realize method to use a non-default visual.  So, use
	** XtCreateWindow() instead.
	*/
	XtCreateWindow( w,
			InputOutput,
			xgVisual(w),
			(*mask | CWBackPixel | CWColormap | CWBorderPixel),
			winattr );

	UpdateWmCmap(w);

	/*
	** If the application has not specified a renderer, set one up.  Note
	** that there is no way to check the validity of a renderer specified
	** in XgNrenderer without generating a PEX error if it's invalid, so
	** the widget assumes that a specified renderer is valid.
	*/
	if ( Renderer(w) == None ) {
		if ( ( Renderer(w) = PEXCreateRenderer( XtDisplay(w),
							XtWindow(w),
							renderer_mask,
							&renderer_attrs ) )
		     == None ) {
			app_warn_msg_params[0] = DisplayString( XtDisplay(w) );
			XtAppWarningMsg( XtWidgetToApplicationContext( w ),
					 "invalidWidget",
					 "PEXSimpleError",
					 "XtToolkitError",
					 "Unable to create renderer for display \"%s\"",
					 app_warn_msg_params,
					 &app_warn_msg_num_params );
			return;
		}
	}

} /* end Realize */

/*
********************************************************************************
** The Destroy class method currently does nothing.
********************************************************************************
*/

#if NeedFunctionPrototypes /* ANSI C or C++ [ */

static void Destroy( XgPEXSimpleWidget w )

#else  /* ] K&R C [ */

static void Destroy( w )
	XgPEXSimpleWidget w;

#endif /* ] */
{
} /* end Destroy */

/*
********************************************************************************
** The Redisplay class method calls the callback list, then redisplays any
** gadgets which may have been obscured.  It uses a private Motif function
** _XmRedisplay gadgets, as do other Motif widgets.
********************************************************************************
*/

#if NeedFunctionPrototypes /* ANSI C or C++ [ */

static void Redisplay(
	XgPEXSimpleWidget w,
	XEvent *event,
	Region region)

#else  /* ] K&R C [ */

static void Redisplay( w, event, region )
	XgPEXSimpleWidget w;
	XEvent *event;
	Region region;

#endif /* ] */
{
	XmDrawingAreaCallbackStruct cb;

	cb.reason = XmCR_EXPOSE;
	cb.event = event;
	cb.window = XtWindow(w);

	XtCallCallbackList ( (Widget) w,
			     (XtCallbackList) ExposeCallback(w),
			     &cb );

	_XmRedisplayGadgets( (Widget) w, event, region );

} /* end Redisplay */

/*
********************************************************************************
** The SetValues class method checks resource values that the application has
** requested be changed.  Settable resources are checked for valid values and
** left unchanged if the value is not valid.  Resources that are read-only
** after the widget has been realized are left unchanged if a change has been
** requested.  The SetValues method returns True if a redraw is required, False
** if it is not.  Note again that the renderer cannot be checked without
** generating a PEX error, so it is assumed to be valid.  The application is
** responsible for ensuring the renderer's validity.
********************************************************************************
*/

#if NeedFunctionPrototypes /* ANSI C or C++ [ */

static Boolean SetValues(
	XgPEXSimpleWidget current,
	XgPEXSimpleWidget request,
	XgPEXSimpleWidget new)

#else  /* ] K&R C [ */

static Boolean SetValues( current, request, new )
	XgPEXSimpleWidget current;
	XgPEXSimpleWidget request;
	XgPEXSimpleWidget new;

#endif /* ] */
{
	Boolean redraw = False;

	/* Range check settable resources. */
	switch( WmCmap(request) ) {
		case XgWM_CMAP_NONE:
		case XgWM_CMAP_LOW_PRIORITY:
		case XgWM_CMAP_HIGH_PRIORITY:
			break;
		default:
			/*
			** For an invalid value, leave XgNwmCmap unchanged.
			*/
			WmCmap(new) = WmCmap(current);
			break;
	}

	switch( RescalePolicy(request) ) {
		case XgRESCALE_MINOR:
			break;
		default:
			/*
			** For an invalid value, leave XgNrescalePolicy
			** unchanged.
			*/
			RescalePolicy(new) = RescalePolicy(current);
			break;
	}

	if ( XtIsRealized(current) ) {
		/*
		** Refuse any changes to these resources after the widget is
		** realized.  They are read-only after widget realization.
		*/
		Overlay(new) = Overlay(current);
		Transparent(new) = Transparent(current);
		xgVisual(new) = xgVisual(current);
	}

	if ( WmCmap(new) != WmCmap(current) ) {
		UpdateWmCmap( (Widget) new );
	}

	/*
	** Check for any conditions that would prompt a redraw of the window.
	*/
	if ( Renderer(new) != Renderer(current) ) {
		redraw = True;
	}

	return (redraw);

} /* end SetValues */

/*
********************************************************************************
* XgCreatePEXSimple is a convenience routine used by Uil/Mrm.
********************************************************************************
*/

#if NeedFunctionPrototypes /* ANSI C or C++ [ */

Widget XgCreatePEXSimple(
	Widget parent,
	String name,
	ArgList arglist,
	Cardinal nargs)

#else  /* ] K&R C [ */

Widget XgCreatePEXSimple( parent, name, arglist, nargs )
	Widget parent;
	String name;
	ArgList arglist;
	Cardinal nargs;

#endif /* ] */
{
	return( XtCreateWidget( name,
				xgPEXSimpleWidgetClass,
				parent,
				arglist,
				nargs ) );
} /* end XgCreatePEXSimple */

/*
********************************************************************************
* PEXSimpleMrmInitialize registers the PEXSimple widget class with Mrm.
********************************************************************************
*/

#if NeedFunctionPrototypes /* ANSI C or C++ [ */

int XgPEXSimpleMrmInitialize(void)

#else  /* ] K&R C [ */

int XgPEXSimpleMrmInitialize()

#endif /* ] */
{
	return( MrmRegisterClass ( MrmwcUnknown,
				   "PEXSimple",
				   "XgCreatePEXSimple",
				   XgCreatePEXSimple,
				   (WidgetClass) &xgPEXSimpleClassRec ) );
} /* end XgPEXSimpleMrmInitialize */
