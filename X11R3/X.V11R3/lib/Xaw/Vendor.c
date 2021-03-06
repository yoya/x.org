#ifndef lint
static char Xrcsid[] = "$XConsortium: Vendor.c,v 1.4 88/10/18 12:33:20 swick Exp $";
/* $oHeader: Vendor.c,v 1.3 88/08/19 13:11:01 asente Exp $ */
#endif lint

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

/*
 * This is a copy of Xt/Vendor.c with an additional ClassInitialize
 * procedure to register Xmu resource type converters.
 *
 */

/* Make sure all wm properties can make it out of the resource manager */

#include <pwd.h>
#include <stdio.h>
#include <sys/param.h>

#include <X11/IntrinsicP.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/ShellP.h>
#include <X11/Vendor.h>
#include <X11/VendorP.h>


/***************************************************************************
 *
 * Vendor shell class record
 *
 ***************************************************************************/

static void _VendorShellClassInitialize();
static void _VendorShellInitialize();
static Boolean _VendorShellSetValues();
static void Realize();

externaldef(vendorshellclassrec) VendorShellClassRec vendorShellClassRec = {
  {
    /* superclass         */    (WidgetClass) &wmShellClassRec,
    /* class_name         */    "VendorShell",
    /* size               */    sizeof(VendorShellRec),
    /* Class Initializer  */	_VendorShellClassInitialize,
    /* class_part_initialize*/	NULL,
    /* Class init'ed ?    */	FALSE,
    /* initialize         */    _VendorShellInitialize,
    /* initialize_notify    */	NULL,		
    /* realize            */    Realize,
    /* actions            */    NULL,
    /* num_actions        */    0,
    /* resources          */    NULL,
    /* resource_count     */	0,
    /* xrm_class          */    NULLQUARK,
    /* compress_motion    */    FALSE,
    /* compress_exposure  */    TRUE,
    /* compress_enterleave*/	FALSE,
    /* visible_interest   */    FALSE,
    /* destroy            */    NULL,
    /* resize             */    XtInheritResize,
    /* expose             */    NULL,
    /* set_values         */    _VendorShellSetValues,
    /* set_values_hook      */	NULL,			
    /* set_values_almost    */	XtInheritSetValuesAlmost,  
    /* get_values_hook      */	NULL,
    /* accept_focus       */    NULL,
    /* intrinsics version */	XtVersion,
    /* callback offsets   */    NULL,
    /* tm_table		  */	NULL,
    /* query_geometry	    */  NULL,
    /* display_accelerator  */  NULL,
    /* extension	    */  NULL
  },{
    /* geometry_manager   */    XtInheritGeometryManager,
    /* change_managed     */    XtInheritChangeManaged,
    /* insert_child	  */	XtInheritInsertChild,
    /* delete_child	  */	XtInheritDeleteChild,
    /* extension	    */  NULL
  },{
    /* extension	    */  NULL
  },{
    /* extension	    */  NULL
  },{
    /* extension	    */  NULL
  }
};

externaldef(vendorshellwidgetclass) WidgetClass vendorShellWidgetClass =
	(WidgetClass) (&vendorShellClassRec);

static void _VendorShellClassInitialize()
{
    /* %%% botch; this requires an Application Context */
    extern void XmuCvtStringToCursor();
    extern void XmuCvtStringToPixmap();
    static XtConvertArgRec screenConvertArg[] = {
        {XtBaseOffset, (caddr_t) XtOffset(Widget, core.screen),
	     sizeof(Screen *)}
    };

    XtAddConverter("String", "Cursor", XmuCvtStringToCursor,      
		   screenConvertArg, XtNumber(screenConvertArg));

    XtAddConverter("String", "Pixmap", XmuCvtStringToPixmap,
		   screenConvertArg, XtNumber(screenConvertArg));
}

/* ARGSUSED */
static void _VendorShellInitialize(req, new)
	Widget req, new;
{
}

/* ARGSUSED */
static Boolean _VendorShellSetValues(old, ref, new)
	Widget old, ref, new;
{
	return FALSE;
}

static void Realize(wid, vmask, attr)
	Widget wid;
	Mask *vmask;
	XSetWindowAttributes *attr;
{
	WidgetClass super = wmShellWidgetClass;

	/* Make my superclass do all the dirty work */

	(*super->core_class.realize) (wid, vmask, attr);
}

