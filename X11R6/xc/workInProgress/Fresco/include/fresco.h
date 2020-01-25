/*
 * Copyright (c) 1993 Silicon Graphics, Inc.
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

#ifndef Fresco_fresco_h
#define Fresco_fresco_h

#include <X11/Fresco/types.h>

/*
 * This include files assumes that object references are pointers,
 * so we can avoid including the interface definitions.
 */

class Display;
class DrawingKit;
class FigureKit;
class Glyph;
class LayoutKit;
class ThreadKit;
class Viewer;
class WidgetKit;

//- Option
struct Option {
    //. An option describes a possible command-line argument in terms
    //. of a name that matches the argument string, a path that
    //. defines the root-style attribute to set, a format, and
    //. an optional implicit value.  If the format is "implicit" then
    //. the attribute will be set to the implicit value.
    //. If the format is "value" then the attribute will be set
    //. to the next argument on the command-line.  If the format
    //. is "path_value" then the next argument is a string containing
    //. the attribute path and value separated by a colon (":").

    enum Format { path_value, value, implicit };

    const char* name;
    const char* path;
    Option::Format format;
    const char* implicit_value;
};

//- Fresco
class Fresco : public BaseObject {
    //. Fresco is a C++ class that defines operations for accessing
    //. other Fresco objects.  It is defined as a class
    //. rather than an interface to provide support for
    //. the C++ "char*" data type and static memory management
    //. operations.  The Fresco class also provides access
    //. to non-standard kits.
    //.
    //. Typically, there will only be one Fresco object per
    //. address space that is created when the address space
    //. is initialized.  The Fresco object will hold references
    //. to kits, which in turn may hold other references.
    //. Therefore, to avoid a reference count circularity
    //. objects should not hold a reference to the Fresco object.
    //. Instead, objects can assume that the Fresco object will
    //. live at least as long as any object.
protected:
    Fresco();
    virtual ~Fresco();
public:
    //- class_name
    virtual CharStringRef class_name() = 0;
	//. Return the class name associated with a Fresco object.
	//. This operation is normally only needed when opening
	//. a display and initializing the display's style
	//. information.

    //- argc, argv
    virtual long argc() = 0;
    virtual char** argv() = 0;
	//. Return the argument list used when Fresco object
	//. was opened.  This information is normally only
	//. needed for registration with a session manager.

    //- open_display, open_default_display
    virtual Display* open_display(CharStringRef name) = 0;
    virtual Display* open_default_display() = 0;
	//. Open a display by name or open the default display.
	//. The name of the default display is platform-specific;
	//. on POSIX it can be set by the DISPLAY environment variable.
	//.
	//. Display names are of the form N:D.S, where N is
	//. an optional network host name.  If omitted, then
	//. the display is assumed to be on the local host.
	//. D is a display number on the host, and S is
	//. the number of the screen to use on the display
	//. by default.

    //- fresco_style
    virtual StyleRef fresco_style() = 0;
	//. Return the root Fresco style.  This style will contain
	//. default and command-line attributes, but not display-specific
	//. or user-specific attributes.

    //- create_request*
    virtual RequestObjRef create_request(BaseObjectRef) = 0;
	//. Return a new request for dynamically invoking an operation
	//. on the given object.

    //- main*
    virtual void run(Viewer* v, Glyph* g) = 0;
	//. Start and run a "main loop" in the common case
	//. with a single viewer on a single display.
	//. This operation creates and maps a main viewer
	//. on the default screen of the main Fresco display
	//. (see Fresco_open or Fresco_open_display).  The main viewer
	//. uses the given viewer for input (unless the viewer is nil,
	//. in which case input is ignored) and the given glyph
	//. for output (unless the glyph is nil, in which case the
	//. viewer is used for output).  After mapping the viewer,
	//. main calls Display::run and waits for it to return.

    //- drawing_kit*
    virtual void drawing_kit(DrawingKit*) = 0;
    virtual DrawingKit* drawing_kit() = 0;
	//. Set or get the kit creating drawing objects such
	//. as colors and fonts.

    //- figure_kit*
    virtual void figure_kit(FigureKit*) = 0;
    virtual FigureKit* figure_kit() = 0;
	//. Set or get the kit for creating simple graphics figures.

    //- layout_kit*
    virtual void layout_kit(LayoutKit*) = 0;
    virtual LayoutKit* layout_kit() = 0;
	//. Set or get the kit for creating simple layout manager objects.

    //- thread_kit*
    virtual void thread_kit(ThreadKit*) = 0;
    virtual ThreadKit* thread_kit() = 0;
	//. Set or get the kit for creating threads and
	//. synchronization objects.

    //- widget_kit*
    virtual void widget_kit(WidgetKit*) = 0;
    virtual WidgetKit* widget_kit() = 0;
	//. Set or get the kit for creating simple widgets
	//. such as buttons, menus, and scrollbars.

    //- string_ref, tmp_string_ref, string_copy, tmp_string_copy
    static CharStringRef string_ref(const char*);
    static CharStringRef string_copy(const char*);
    static CharString_var tmp_string_ref(const char*);
    static CharString_var tmp_string_copy(const char*);
	//. Create a string from a null-terminated array
	//. of characters.  The string_ref operation simply
	//. references the data; the string_copy operation
	//. copies the data into its own storage that is freed
	//. when the string is deallocated.  The tmp_string_ref and
	//. tmp_string_copy operations return a managed rather than
	//. raw object reference.

    //- get_string, get_tmp_string
    static CharStringRef get_string(Style_in, const char* name);
    static CharString_var get_tmp_string(Style_in, const char* name);
	//. Return the string bound to the given name in
	//. the given style.  The get_tmp_string operation returns
	//. a managed rather than a raw reference.  These operations
	//. are effectively short-hand for creating a string, calling
	//. resolve on the style, and calling read_string on the
	//. result if non-nil.  Both operations return a nil string
	//. if either the name is not defined in the style.

    //- ref, unref
    static void ref(BaseObjectRef);
    static void unref(BaseObjectRef);
	//. The ref and unref operations respectively
	//. increment and decrement the reference count
	//. of a Fresco object.  If the count is not positive
	//. after an unref, then the object may be destroyed.
	//. These operations are static rather than operations
	//. on BaseObject to allow for nil references,
	//. which are ignored.

    //- delay
    static Boolean delay(Float seconds);
	//. Pause execution the given number of seconds or fraction
	//. of a second.  Return true if execution was successfully
	//. suspended the requested duration.
private:
    Fresco(const Fresco&);
    void operator =(const Fresco&);
};

//- Fresco_init
extern Fresco* Fresco_init(
    const char* name, int& argc, char** argv, Option* options = nil
);
    //. This function returns a Fresco object with which
    //. one can access other objects.  The name parameter
    //. specifies the name of the root style.  The argc and argv
    //. parameters should be the command-line parameters,
    //. the same as one would pass the function "main" in C.
    //. Unlike Fresco_open, this function does not open
    //. a display and does not create an initial drawing kit.
//-

//- Fresco_open, Fresco_open_display
extern Fresco* Fresco_open(
    const char* name, int& argc, char** argv, Option* options = nil
);
extern Fresco* Fresco_open_display(
    const char* display_name,
    const char* name, int& argc, char** argv, Option* options = nil
);
    //. These functions returns a Fresco object with which
    //. one can access other objects.  The name parameter
    //. specifies the name of the root style.  The argc and argv
    //. parameters should be the command-line parameters,
    //. the same as one would pass the function "main" in C.
    //.
    //. Both functions open a display and create an initial
    //. drawing kit from the display.  Fresco_open uses
    //. the default display name; Fresco_open_display uses
    //. the given display name.  Both functions return nil
    //. if they cannot successfully open the display.
//-

inline CharString_var Fresco::tmp_string_ref(const char* s) {
    return string_ref(s);
}

inline CharString_var Fresco::tmp_string_copy(const char* s) {
    return string_copy(s);
}

inline CharString_var Fresco::get_tmp_string(StyleRef s, const char* name) {
    return get_string(s, name);
}

#endif
