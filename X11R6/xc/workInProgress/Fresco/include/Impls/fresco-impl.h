/*
 * Copyright (c) 1992-1993 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the name of
 * Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Silicon Graphics.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

#ifndef Fresco_Impls_fresco_impl_h
#define Fresco_Impls_fresco_impl_h

#include <X11/Fresco/fresco.h>
#include <X11/Fresco/Impls/fobjects.h>

class FrescoImpl : public Fresco {
public:
    FrescoImpl(const char* name, int& argc, char** argv, Option*);
    ~FrescoImpl();

    //+ FrescoObject::=
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    //+

    CharStringRef class_name();
    long argc();
    char** argv();

    Display* open_display(CharStringRef name);
    Display* open_default_display();

    StyleRef fresco_style();
    RequestObjRef create_request(BaseObjectRef);

    void run(Viewer*, Glyph*);

    void drawing_kit(DrawingKit*);
    DrawingKit* drawing_kit();

    void figure_kit(FigureKit*);
    FigureKit* figure_kit();

    void layout_kit(LayoutKit*);
    LayoutKit* layout_kit();

    void thread_kit(ThreadKit*);
    ThreadKit* thread_kit();

    void widget_kit(WidgetKit*);
    WidgetKit* widget_kit();

    void connect(Display*);
protected:
    SharedFrescoObjectImpl object_;
    CharStringRef class_name_;
    long argc_;
    char** argv_;
    Display* display_;
    StyleRef style_;
    DrawingKit* drawing_kit_;
    FigureKit* figure_kit_;
    LayoutKit* layout_kit_;
    ThreadKit* thread_kit_;
    WidgetKit* widget_kit_;

    void create_root_style(
	const char* name, int& argc, char** argv, Option* options
    );
    void find_name(int argc, char** argv);
    void parse_args(int& argc, char** argv, Option* options);
    void extract(Option*, int& i, int argc, char** argv);
    const char* next_arg(int& i, int argc, char** argv, const char* message);
    void bad_arg(const char* message);

    FigureKit* create_figure_kit();
    LayoutKit* create_layout_kit();
    ThreadKit* create_thread_kit();
    WidgetKit* create_widget_kit();
    void init_types();
};

#endif
