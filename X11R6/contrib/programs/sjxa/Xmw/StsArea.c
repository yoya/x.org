/* $SonyId: StsArea.c,v 1.1 1994/05/30 13:05:43 makoto Exp $ */
/******************************************************************
Copyright (c) 1993, 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Sony Corporation shall
not be used in advertising or otherwise to promote the sale, use or ot
her dealings in this Software without prior written authorization from
Sony Corporation.
******************************************************************/
#include	<ctype.h>
#include	<Xmw/StsAreaP.h>
#include	<X11/Shell.h>


static XtResource	resources[] =
{
    {
	XmwNmarginWidth,
	XmwCMarginWidth, XtRDimension, sizeof(Dimension),
	XtOffset(XmwStatusAreaWidget, status_area.margin_width),
	XtRImmediate, (caddr_t)5 },
    {
	XmwNmarginHeight,
	XmwCMarginHeight, XtRDimension, sizeof(Dimension),
	XtOffset(XmwStatusAreaWidget, status_area.margin_height),
	XtRImmediate, (caddr_t)2 },
    {
	XmwNspacing,
	XmwCSpacing, XtRDimension, sizeof(Dimension),
	XtOffset(XmwStatusAreaWidget, status_area.spacing),
	XtRImmediate, (caddr_t)10 },
    {
	XmwNparentWindow,
	XmwCParentWindow, XtRWindow, sizeof(Window),
	XtOffset(XmwStatusAreaWidget, status_area.parent_window),
	XtRWindow, NULL	},
    {
	XmwNcursor,
	XtCCursor, XtRCursor, sizeof(Cursor),
	XtOffset(XmwStatusAreaWidget, status_area.cursor),
	XtRImmediate, (caddr_t)0 },
    {
	XmwNareaNeededWidth,
	XtCWidth, XtRDimension, sizeof(Dimension),
	XtOffset(XmwStatusAreaWidget, status_area.needed_width),
	XtRImmediate, (caddr_t)0 },
    {
	XmwNareaNeededHeight,
	XtCHeight, XtRDimension, sizeof(Dimension),
	XtOffset(XmwStatusAreaWidget, status_area.needed_height),
	XtRImmediate, (caddr_t)0 }
};

WidgetClass	xmwStatusAreaWidgetClass = (WidgetClass)&xmwStatusAreaClassRec;


static void
#if NeedFunctionPrototypes
Initialize( Widget request, Widget new, ArgList args, Cardinal *num_args )
#else
Initialize( request, new, args, num_args )
    Widget	request, new;
    ArgList	args;
    Cardinal	*num_args;
#endif
{
    XmwStatusAreaWidget	nw = (XmwStatusAreaWidget)new;

    if( nw->core.width == 0 )
	nw->core.width = 1;
    if( nw->core.height == 0 )
	nw->core.height = 1;
}


static void	LayoutChildren( aw )
XmwStatusAreaWidget	aw;
{
    Widget	widget;
    Dimension	max_height;
    Dimension	widget_height;
    Position	x, y;
    int		ii;

    x = aw->status_area.margin_width;
    max_height = 0;
    for( ii = 0; ii < aw->composite.num_children; ii++ ) {
	widget = aw->composite.children[ii];
	if( XtIsManaged(widget) ) {
	    y = ( aw->core.height - widget->core.height ) / 2
	      - widget->core.border_width;
	    XtMoveWidget( widget, x, y );
	    x += widget->core.width + aw->status_area.spacing;

	    widget_height = widget->core.height
			  + widget->core.border_width * 2;
	    if( widget_height > max_height )	max_height = widget_height;
	}
    }

    aw->status_area.needed_width = x;
    aw->status_area.needed_height = max_height +
				    aw->status_area.margin_height * 2;
}


static void
#if NeedFunctionPrototypes
Resize( Widget w )
#else
Resize( w )
    Widget	w;
#endif
{
    LayoutChildren( (XmwStatusAreaWidget)w );
}


static Boolean	SetValues( current, request, new )
XmwStatusAreaWidget	current, request, new;
{
    Boolean	change = False;
    Position	x, y;
    Dimension	width, height;
    Dimension	bw;

    XtVaGetValues( XtParent(request), XtNx, &x, XtNy, &y,
		   XtNwidth, &width, XtNheight, &height,
		   XtNborderWidth, &bw, NULL );
    if( height != request->core.height )
	LayoutChildren( request );
    if( x != request->core.x  ||  y != request->core.y  ||
       width != request->core.width  ||  height != request->core.height ) {
	XtConfigureWidget( XtParent(request), request->core.x, request->core.y,
			   request->core.width, request->core.height, bw );
	XMoveResizeWindow( XtDisplay(request), XtWindow(request),
			  request->core.x, request->core.y,
			  request->core.width, request->core.height );
    }

    if( current->status_area.margin_width !=
	request->status_area.margin_width  ||
        current->status_area.margin_height !=
	request->status_area.margin_height  ||
        current->status_area.spacing != request->status_area.spacing ) {
	LayoutChildren( request );
	change = True;
    }

    if( current->status_area.parent_window !=
	request->status_area.parent_window ) {
	XReparentWindow( XtDisplay(request), XtWindow(request),
			 request->status_area.parent_window,
			 request->core.x, request->core.y );
    }

    if( current->status_area.cursor != request->status_area.cursor )
	XDefineCursor( XtDisplay(request), XtWindow(request),
		      request->status_area.cursor );

    return( change );
}


static XtGeometryResult	QueryGeometry( aw, proposed, desired )
XmwStatusAreaWidget		aw;
XtWidgetGeometry	*proposed;
XtWidgetGeometry	*desired;
{
    desired->width  = aw->core.width;
    desired->height = aw->core.height;
    desired->request_mode = CWWidth | CWHeight;

    if( (proposed->request_mode & CWWidth)  &&
       (proposed->width >= desired->width)  &&
       (proposed->request_mode & CWHeight)  &&
       (proposed->height >= desired->height) ) {
	return( XtGeometryYes );
    }

    return( XtGeometryAlmost );
}


static XtGeometryResult	GeometryManager( w, request, reply )
Widget			w;
XtWidgetGeometry	*request;
XtWidgetGeometry	*reply;
{
    if( request->request_mode & CWWidth )
	w->core.width = request->width;

    if( request->request_mode & CWHeight )
	w->core.height = request->height;

    if( request->request_mode & CWBorderWidth )
	w->core.border_width = request->border_width;

    if( request->request_mode & (CWWidth | CWHeight | CWBorderWidth) )
	LayoutChildren( (XmwStatusAreaWidget)XtParent( w ) );
	    
    return( XtGeometryYes );
}


static void	ChangeManaged( aw )
XmwStatusAreaWidget	aw;
{
    LayoutChildren( aw );
}


XmwStatusAreaClassRec	xmwStatusAreaClassRec =
{
  { /* core_class fields      */
    /* superclass             */ (WidgetClass)&constraintClassRec,
    /* class_name             */ "XmwStatusArea",
    /* widget_size            */ sizeof(XmwStatusAreaWidgetRec),
    /* class_initialize       */ NULL,
    /* class_part_initialize  */ NULL,
    /* class_inited           */ False,
    /* initialize             */ Initialize,
    /* initialize_hook        */ NULL,
    /* realize                */ XtInheritRealize,
    /* actions                */ NULL,
    /* num_actions            */ 0,
    /* resources              */ resources,
    /* num_resources          */ XtNumber(resources),
    /* xrm_class              */ NULLQUARK,
    /* compress_motion        */ True,
    /* compress_exposure      */ XtExposeCompressMaximal,
    /* compress_enterleave    */ True,
    /* visible_interest       */ False,
    /* destroy                */ NULL,
    /* resize                 */ Resize,
    /* expose                 */ XtInheritExpose,
    /* set_values             */ (XtSetValuesFunc)SetValues,
    /* set_values_hook        */ NULL,
    /* set_values_almost      */ XtInheritSetValuesAlmost,
    /* get_values_hook        */ NULL,
    /* accept_focus           */ NULL,
    /* version                */ XtVersion,
    /* callback_private       */ NULL,
    /* tm_table               */ NULL,
    /* query_geometry         */ (XtGeometryHandler)QueryGeometry,
    /* display_accelerator    */ NULL,
    /* extension              */ NULL,
  },
  { /* composite_class fields */
    /* geometry_handler       */ (XtGeometryHandler)GeometryManager,
    /* change_managed         */ (XtWidgetProc)ChangeManaged,
    /* insert_child           */ XtInheritInsertChild,
    /* delete_child           */ XtInheritDeleteChild,
    /* extension              */ NULL,
  },
  { /* constraint_class fields*/
    /* resources              */ NULL,
    /* num_resources          */ 0,
    /* constraint_size        */ 0,
    /* initialize             */ NULL,
    /* destroy                */ NULL,
    /* set_values             */ NULL,
    /* extension              */ NULL,
  },
  { /* status_area_class fields */
    /* extension              */ NULL,
  }
};


#if	NeedFunctionPrototypes
Widget	XmwCreateManagedStatusArea( Widget parent, String name, ArgList args,
				    Cardinal argc )
#else	/* NeedFunctionPrototypes */
Widget	XmwCreateManagedStatusArea( parent, name, args, argc )
    Widget	parent;
    String	name;
    ArgList	args;
    Cardinal	argc;
#endif	/* NeedFunctionPrototypes */
{
    Widget	w, override;
    Window	win;
    Arg		*merged_args;
    char	*ss, *tt;
    int		n;
    Position	x, y;

    ss = XtMalloc( strlen(name) + 3 );	/* name + "OS" + NULL */
    tt = XtMalloc( strlen(name) + 3 );
    strcpy( ss, name );
    strcat( ss, "OS" );
    ss[0] = tolower( ss[0] );
    strcpy( tt, ss );
    tt[0] = toupper( tt[0] );

    x = y = 0;
    merged_args = (ArgList)XtCalloc( argc+1, sizeof(Arg) );
    for( n = 0; n < argc; n++ ) {
	if( strcmp( args[n].name, XtNx ) == 0 )
	    x = args[n].value;
	if( strcmp( args[n].name, XtNy ) == 0 )
	    y = args[n].value;
	if( strcmp( args[n].name, XtNwidth ) == 0  &&
	   args[n].value == 0 )
	    args[n].value = 10;
	if( strcmp( args[n].name, XtNheight ) == 0  &&
	   args[n].value == 0 )
	    args[n].value = 10;
	merged_args[n].name = args[n].name;
	merged_args[n].value = args[n].value;
    }
    XtSetArg( merged_args[n], XtNmappedWhenManaged, False );	n++;

    override = XtAppCreateShell( ss, tt, overrideShellWidgetClass,
				 XtDisplay(parent), merged_args, n );
    XtFree( ss );
    XtFree( tt );
    XtFree( (char *)merged_args );

    w = XtCreateManagedWidget( name, xmwStatusAreaWidgetClass, override, args,
			       argc );
    XtRealizeWidget( override );
    if( (win = ((XmwStatusAreaWidget)w)->status_area.parent_window) != None )
	XReparentWindow( XtDisplay(w), XtWindow(w), win, x, y );
    XtAddGrab( w, False, False );
    return( w );
}
