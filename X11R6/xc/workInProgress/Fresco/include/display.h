/* DO NOT EDIT -- Automatically generated from Interfaces/display.idl */

#ifndef Interfaces_display_h
#define Interfaces_display_h

#include <X11/Fresco/_enter.h>
#include <X11/Fresco/drawing.h>

class Color;
typedef Color* ColorRef;
typedef ColorRef Color_in;
typedef ColorRef Color_out, Color_inout;
typedef ColorRef Color_return;
class Color_var;

class Event;
typedef Event* EventRef;
typedef EventRef Event_in;
typedef EventRef Event_out, Event_inout;
typedef EventRef Event_return;
class Event_var;

class Font;
typedef Font* FontRef;
typedef FontRef Font_in;
typedef FontRef Font_out, Font_inout;
typedef FontRef Font_return;
class Font_var;

class GlyphTraversal;
typedef GlyphTraversal* GlyphTraversalRef;
typedef GlyphTraversalRef GlyphTraversal_in;
typedef GlyphTraversalRef GlyphTraversal_out, GlyphTraversal_inout;
typedef GlyphTraversalRef GlyphTraversal_return;
class GlyphTraversal_var;

class Raster;
typedef Raster* RasterRef;
typedef RasterRef Raster_in;
typedef RasterRef Raster_out, Raster_inout;
typedef RasterRef Raster_return;
class Raster_var;

class Screen;
typedef Screen* ScreenRef;
typedef ScreenRef Screen_in;
typedef ScreenRef Screen_out, Screen_inout;
typedef ScreenRef Screen_return;
class Screen_var;

class Viewer;
typedef Viewer* ViewerRef;
typedef ViewerRef Viewer_in;
typedef ViewerRef Viewer_out, Viewer_inout;
typedef ViewerRef Viewer_return;
class Viewer_var;

class Window;
typedef Window* WindowRef;
typedef WindowRef Window_in;
typedef WindowRef Window_out, Window_inout;
typedef WindowRef Window_return;
class Window_var;

class WindowStyle;
typedef WindowStyle* WindowStyleRef;
typedef WindowStyleRef WindowStyle_in;
typedef WindowStyleRef WindowStyle_out, WindowStyle_inout;
typedef WindowStyleRef WindowStyle_return;
class WindowStyle_var;

class Cursor;
typedef Cursor* CursorRef;
typedef CursorRef Cursor_in;
typedef CursorRef Cursor_out, Cursor_inout;
typedef CursorRef Cursor_return;
class Cursor_var;

extern BaseObjectRef _XfCursorStub_create(Exchange*);

class Cursor : public FrescoObject {
protected:
    Cursor();
    virtual ~Cursor();
public:
    CursorRef _obj() { return this; };
    static CursorRef  _return_ref(Cursor_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static CursorRef _narrow(BaseObjectRef);
    static CursorRef _duplicate(CursorRef obj) {
        return (CursorRef)_BaseObject__duplicate(obj, &_XfCursorStub_create);
    }
};

class Cursor_var {
protected:
    CursorRef _obj_;
public:
    Cursor_var() { _obj_ = 0; };
    Cursor_var(CursorRef p) { _obj_ = p; }
    Cursor_var(const Cursor_var& r) {
        _obj_ = Cursor::_duplicate(r._obj_);
    }
    ~Cursor_var() { _BaseObject__release(_obj_); }

    Cursor_var& operator =(CursorRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Cursor::_duplicate(p);
        return *this;
    }
    Cursor_var& operator =(const Cursor_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Cursor::_duplicate(r._obj_);
        return *this;
    }

    CursorRef _obj() const { return _obj_; }
    CursorRef& _out() { return _obj_; }
    operator CursorRef() const { return _obj_; }
    CursorRef operator ->() const { return _obj_; }
};

inline Cursor_var _tmp(CursorRef p) { return Cursor_var(p); }

class CursorStub : public Cursor {
public:
    CursorStub(Exchange*);
    ~CursorStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class Display;
typedef Display* DisplayRef;
typedef DisplayRef Display_in;
typedef DisplayRef Display_out, Display_inout;
typedef DisplayRef Display_return;
class Display_var;

extern BaseObjectRef _XfDisplayStub_create(Exchange*);

class Display : public FrescoObject {
protected:
    Display();
    virtual ~Display();
public:
    DisplayRef _obj() { return this; };
    static DisplayRef  _return_ref(Display_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static DisplayRef _narrow(BaseObjectRef);
    static DisplayRef _duplicate(DisplayRef obj) {
        return (DisplayRef)_BaseObject__duplicate(obj, &_XfDisplayStub_create);
    }
    typedef Long ScreenNumber;
    virtual Style_return display_style();
    virtual DrawingKit_return drawing_kit();
    virtual Screen_return display_screen(Display::ScreenNumber n);
    virtual Display::ScreenNumber number_of_screens();
    virtual Screen_return default_screen();
    virtual Cursor_return cursor_from_data(Short x, Short y, Long pat[16], Long mask[16]);
    virtual Cursor_return cursor_from_bitmap(Raster_in b, Raster_in mask);
    virtual Cursor_return cursor_from_font(Font_in f, Long pat, Long mask);
    virtual Cursor_return cursor_from_index(Long n);
    virtual void run(Boolean b);
    virtual Boolean running();
    virtual Tag add_filter(GlyphTraversal_in t);
    virtual void remove_filter(Tag add_tag);
    virtual void need_repair(Window_in w);
    virtual void repair();
    virtual void flush();
    virtual void flush_and_wait();
    virtual void ring_bell(Float pct_loudness);
    virtual void close();
    virtual Boolean auto_repeat();
    virtual void auto_repeat(Boolean _p);
    virtual Float key_click_volume();
    virtual void key_click_volume(Float _p);
    virtual Float pointer_acceleration();
    virtual void pointer_acceleration(Float _p);
    virtual Long pointer_threshold();
    virtual void pointer_threshold(Long _p);
    virtual Float pointer_double_click_threshold();
    virtual void pointer_double_click_threshold(Float _p);
};

class Display_var {
protected:
    DisplayRef _obj_;
public:
    Display_var() { _obj_ = 0; };
    Display_var(DisplayRef p) { _obj_ = p; }
    Display_var(const Display_var& r) {
        _obj_ = Display::_duplicate(r._obj_);
    }
    ~Display_var() { _BaseObject__release(_obj_); }

    Display_var& operator =(DisplayRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Display::_duplicate(p);
        return *this;
    }
    Display_var& operator =(const Display_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Display::_duplicate(r._obj_);
        return *this;
    }

    DisplayRef _obj() const { return _obj_; }
    DisplayRef& _out() { return _obj_; }
    operator DisplayRef() const { return _obj_; }
    DisplayRef operator ->() const { return _obj_; }
};

inline Display_var _tmp(DisplayRef p) { return Display_var(p); }

class DisplayStub : public Display {
public:
    DisplayStub(Exchange*);
    ~DisplayStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfScreenStub_create(Exchange*);

class Screen : public FrescoObject {
protected:
    Screen();
    virtual ~Screen();
public:
    ScreenRef _obj() { return this; };
    static ScreenRef  _return_ref(Screen_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static ScreenRef _narrow(BaseObjectRef);
    static ScreenRef _duplicate(ScreenRef obj) {
        return (ScreenRef)_BaseObject__duplicate(obj, &_XfScreenStub_create);
    }
    virtual Coord dpi();
    virtual void dpi(Coord _p);
    virtual Display_return screen_display();
    virtual Coord width();
    virtual Coord height();
    virtual PixelCoord to_pixels(Coord c);
    virtual Coord to_coord(PixelCoord p);
    virtual Coord to_pixels_coord(Coord c);
    virtual void move_pointer(Coord x, Coord y);
    virtual Window_return application(Viewer_in v);
    virtual Window_return top_level(Viewer_in v, Window_in group_leader);
    virtual Window_return transient(Viewer_in v, Window_in transient_for);
    virtual Window_return popup(Viewer_in v);
    virtual Window_return icon(Viewer_in v);
};

class Screen_var {
protected:
    ScreenRef _obj_;
public:
    Screen_var() { _obj_ = 0; };
    Screen_var(ScreenRef p) { _obj_ = p; }
    Screen_var(const Screen_var& r) {
        _obj_ = Screen::_duplicate(r._obj_);
    }
    ~Screen_var() { _BaseObject__release(_obj_); }

    Screen_var& operator =(ScreenRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Screen::_duplicate(p);
        return *this;
    }
    Screen_var& operator =(const Screen_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Screen::_duplicate(r._obj_);
        return *this;
    }

    ScreenRef _obj() const { return _obj_; }
    ScreenRef& _out() { return _obj_; }
    operator ScreenRef() const { return _obj_; }
    ScreenRef operator ->() const { return _obj_; }
};

inline Screen_var _tmp(ScreenRef p) { return Screen_var(p); }

class ScreenStub : public Screen {
public:
    ScreenStub(Exchange*);
    ~ScreenStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfWindowStub_create(Exchange*);

class Window : public FrescoObject {
protected:
    Window();
    virtual ~Window();
public:
    WindowRef _obj() { return this; };
    static WindowRef  _return_ref(Window_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static WindowRef _narrow(BaseObjectRef);
    static WindowRef _duplicate(WindowRef obj) {
        return (WindowRef)_BaseObject__duplicate(obj, &_XfWindowStub_create);
    }
    struct Placement {
        Coord x, y, width, height;
        Alignment align_x, align_y;
    };
    virtual Boolean double_buffered();
    virtual void double_buffered(Boolean _p);
    virtual Cursor_return default_cursor();
    virtual void default_cursor(Cursor_in _p);
    virtual Color_return cursor_foreground();
    virtual void cursor_foreground(Color_in _p);
    virtual Color_return cursor_background();
    virtual void cursor_background(Color_in _p);
    virtual CharString_return geometry();
    virtual void geometry(CharString_in _p);
    virtual Window_return icon();
    virtual void icon(Window_in _p);
    virtual Boolean iconic();
    virtual void iconic(Boolean _p);
    virtual CharString_return title();
    virtual void title(CharString_in _p);
    virtual Long xor_pixel();
    virtual void xor_pixel(Long _p);
    virtual Screen_return window_screen();
    virtual Style_return window_style();
    virtual Viewer_return main_viewer();
    virtual void configure(const Window::Placement& p);
    virtual void get_configuration(Boolean position, Window::Placement& p);
    virtual void configure_notify(Coord width, Coord height);
    virtual void move_notify(Coord left, Coord bottom);
    virtual void map();
    virtual void unmap();
    virtual Boolean is_mapped();
    virtual void map_notify();
    virtual void unmap_notify();
    virtual void iconify();
    virtual void deiconify();
    virtual void raise();
    virtual void lower();
    virtual void redraw(Coord left, Coord bottom, Coord width, Coord height);
    virtual void repair();
    virtual void handle_event(Event_in e);
    virtual void grab_pointer(Cursor_in c);
    virtual void ungrab_pointer();
};

class Window_var {
protected:
    WindowRef _obj_;
public:
    Window_var() { _obj_ = 0; };
    Window_var(WindowRef p) { _obj_ = p; }
    Window_var(const Window_var& r) {
        _obj_ = Window::_duplicate(r._obj_);
    }
    ~Window_var() { _BaseObject__release(_obj_); }

    Window_var& operator =(WindowRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Window::_duplicate(p);
        return *this;
    }
    Window_var& operator =(const Window_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Window::_duplicate(r._obj_);
        return *this;
    }

    WindowRef _obj() const { return _obj_; }
    WindowRef& _out() { return _obj_; }
    operator WindowRef() const { return _obj_; }
    WindowRef operator ->() const { return _obj_; }
};

inline Window_var _tmp(WindowRef p) { return Window_var(p); }

class WindowStub : public Window {
public:
    WindowStub(Exchange*);
    ~WindowStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};


#endif
