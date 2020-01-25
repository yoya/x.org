/*
 * Copyright (c) 1993 2001 S.A.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the name of
 * 2001 S.A. may not be used in any advertising or publicity relating to the
 * software without the specific, prior written permission of 2001 S.A.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL 2001 S.A. BE LIABLE FOR ANY SPECIAL, INCIDENTAL,
 * INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER OR NOT ADVISED OF
 * THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF LIABILITY, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef Xtf_Fresco_h
#define Xtf_Fresco_h

#include <X11/Fresco/Impls/fresco-impl.h>
#include <X11/Fresco/Impls/XtIntrinsic.h>

//- XtfFresco
class XtfFresco : public FrescoImpl {
    //. The XtfFresco class implements the Fresco class interface
    //. for interoperability with Xt.  An application that wishes
    //. to place a Fresco viewer or glyph in an Xt widget hierarchy
    //. should create an XtfFresco object instead of a Fresco object.
public:
    XtfFresco(
	XtAppContext app_context,
	const char* name, int& argc, char** argv, Option*
    );
    virtual ~XtfFresco();

    Display* open_display(CharStringRef name);
    Display* open_default_display();

    //- init_widget, init_widget_glyph
    void init_widget(Widget, Viewer*);
    void init_widget_glyph(Widget, Glyph*);
	//. These operations initialize an XtfViewer widget to contain
	//. a given Fresco viewer or glyph.  The XtfViewer widget should
	//. be placed in the Xt hierarchy in the normal Xt manner.
protected:
    XtAppContext app_context_;

    static XtfBoolean workproc(XtPointer);
};

/*
 * Global function for bootstrapping.
 */

//- XtfFresco_init
extern XtfFresco* XtfFresco_init(
    XtAppContext, XDisplay*,
    const char* name, 
    int& argc, char** argv, Option* options = nil
);
    //. Return an XtfFresco object suitable for placing Fresco
    //. objects in an Xt hierarchy.  XtfFresco_init must be passed
    //. an application context, display, session name, and
    //. command-line arguments.
//-

#endif
