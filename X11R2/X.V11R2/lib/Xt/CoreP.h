/*
* $Header: CoreP.h,v 1.5 88/02/26 12:36:07 swick Exp $
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

#ifndef XtCoreP_h
#define XtCoreP_h

#define UnspecifiedPixmap ((Pixmap)2)

typedef struct _StateRec *StatePtr;

typedef struct _TMRec {
    XtTranslations   translations; /* private to Translation Manager    */
    XtActionProc*   proc_table;         /* procedure bindings for actions    */
    StatePtr        current_state;      /* Translation Manager state ptr     */
    unsigned long   lastEventTime;
} TMRec;


/***************************************************************
 * Widget Core Data Structures
 *
 *
 **************************************************************/

typedef struct _CorePart {
    Widget	    self;		/* pointer to widget itself	     */
    WidgetClass	    widget_class;	/* pointer to Widget's ClassRec	     */
    Widget	    parent;		/* parent widget	  	     */
    String          name;		/* widget resource name		     */
    XrmName         xrm_name;		/* widget resource name quarkified   */
    Screen	    *screen;		/* window's screen		     */
    Colormap        colormap;           /* colormap                          */
    Window	    window;		/* window ID			     */
    Position        x, y;		/* window position		     */
    Dimension       width, height;	/* window dimensions		     */
    Cardinal        depth;		/* number of planes in window        */
    Dimension       border_width;	/* window border width		     */
    Pixel	    border_pixel;	/* window border pixel		     */
    Pixmap          border_pixmap;	/* window border pixmap or NULL      */
    Pixel	    background_pixel;	/* window background pixel	     */
    Pixmap          background_pixmap;	/* window background pixmap or NULL  */
    struct _XtEventRec *event_table;	/* private to event dispatcher       */
    struct _TMRec   tm;                 /* translation management            */
    caddr_t         constraints;        /* constraint record                 */
    Boolean         visible;		/* is window mapped and not occluded?*/
    Boolean	    sensitive;		/* is widget sensitive to user events*/
    Boolean         ancestor_sensitive;	/* are all ancestors sensitive?      */
    Boolean         managed;		/* is widget geometry managed?	     */
    Boolean	    mapped_when_managed;/* map window if it's managed?       */
    Boolean         being_destroyed;	/* marked for destroy		     */
    XtCallbackList  destroy_callbacks;	/* who to call when widget destroyed */
    WidgetList      popup_list;         /* list of popups                    */
    Cardinal        num_popups;         /* how many popups                   */
} CorePart;

typedef struct _WidgetRec {
    CorePart    core;
 } WidgetRec;



/******************************************************************
 *
 * Core Class Structure. Widgets, regardless of their class, will have
 * these fields.  All widgets of a given class will have the same values
 * for these fields.  Widgets of a given class may also have additional
 * common fields.  These additional fields are included in incremental
 * class structures, such as CommandClass.
 *
 * The fields that are specific to this subclass, as opposed to fields that
 * are part of the superclass, are called "subclass fields" below.  Many
 * procedures are responsible only for the subclass fields, and not for
 * any superclass fields.
 *
 ********************************************************************/

typedef struct _CoreClassPart {
    WidgetClass     superclass;		/* pointer to superclass ClassRec   */
    String          class_name;		/* widget resource class name       */
    Cardinal        widget_size;	/* size in bytes of widget record   */
    XtProc	    class_initialize;   /* class initialization proc	    */
    XtWidgetClassProc class_part_initialize; /* dynamic initialization      */
    Boolean         class_inited;       /* has class been initialized?      */
    XtInitProc      initialize;		/* initialize subclass fields       */
    XtArgsProc      initialize_hook;    /* notify that initialize called    */
    XtRealizeProc   realize;		/* XCreateWindow for widget	    */
    XtActionList    actions;		/* widget semantics name to proc map*/
    Cardinal	    num_actions;	/* number of entries in actions     */
    XtResourceList  resources;		/* resources for subclass fields    */
    Cardinal        num_resources;      /* number of entries in resources   */
    XrmClass        xrm_class;		/* resource class quarkified	    */
    Boolean         compress_motion;    /* compress MotionNotify for widget */
    Boolean         compress_exposure;  /* compress Expose events for widget*/
    Boolean         compress_enterleave;/* compress enter and leave events  */
    Boolean         visible_interest;   /* select for VisibilityNotify      */
    XtWidgetProc    destroy;		/* free data for subclass pointers  */
    XtWidgetProc    resize;		/* geom manager changed widget size */
    XtExposeProc    expose;		/* rediplay window		    */
    XtSetValuesFunc set_values;		/* set subclass resource values     */
    XtArgsFunc      set_values_hook;    /* notify that set_values called    */
    XtAlmostProc    set_values_almost;  /* set_values got "Almost" geo reply*/
    XtArgsProc      get_values_hook;    /* notify that get_values called    */
    XtWidgetProc    accept_focus;       /* assign input focus to widget     */
    XtVersionType   version;	        /* version of intrinsics used	    */
    struct _XtOffsetRec *callback_private;/* list of callback offsets       */
    String         tm_table;            /* state machine                    */
    XtGeometryHandler query_geometry;	/* return preferred geometry        */
  } CoreClassPart;

typedef struct _WidgetClassRec {
    CoreClassPart core_class;
} WidgetClassRec;

globalref WidgetClassRec widgetClassRec;

extern int _XtInheritTranslations;
#define XtInheritTranslations  ((String) &_XtInheritTranslations)
#define XtInheritRealize ((XtRealizeProc) _XtInherit)
#define XtInheritResize ((XtWidgetProc) _XtInherit)
#define XtInheritExpose ((XtExposeProc) _XtInherit)
#define XtInheritSetValuesAlmost ((XtAlmostProc) _XtInherit)
#define XtInheritAcceptFocus ((XtWidgetProc) _XtInherit)
#define XtInheritQueryGeometry ((XtGeometryHandler) _XtInherit)

#endif _XtCoreP_h
/* DON'T ADD STUFF AFTER THIS #endif */
