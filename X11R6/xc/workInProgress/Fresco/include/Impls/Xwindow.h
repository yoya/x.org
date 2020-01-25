/*
 * Copyright (c) 1987-91 Stanford University
 * Copyright (c) 1991-93 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Stanford and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Stanford and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL STANFORD OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

#ifndef Fresco_Impls_Xwindow_h
#define Fresco_Impls_Xwindow_h

#include <X11/Fresco/display.h>
#include <X11/Fresco/viewer.h>
#include <X11/Fresco/Impls/fobjects.h>
#include <X11/Fresco/Impls/styles.h>
#include <X11/Fresco/Impls/Xscreen.h>
#include <X11/Fresco/OS/thread.h>

class DamageRegion;
class GlyphTraversalImpl;
class MainViewer;
class RegionImpl;
class XPainterImpl;

//+ WindowImpl : Window, Damage
class WindowImpl : public Window, public Damage {
public:
    ~WindowImpl();
    TypeObjId _tid();
    static WindowImpl* _narrow(BaseObjectRef);
//+
public:
    WindowImpl(DisplayImpl*, ScreenImpl*, ViewerRef);

    //+ Window::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Window */
    Boolean double_buffered();
    void double_buffered(Boolean _p);
    Cursor_return default_cursor();
    void default_cursor(Cursor_in _p);
    Color_return cursor_foreground();
    void cursor_foreground(Color_in _p);
    Color_return cursor_background();
    void cursor_background(Color_in _p);
    CharString_return geometry();
    void geometry(CharString_in _p);
    Window_return icon();
    void icon(Window_in _p);
    Boolean iconic();
    void iconic(Boolean _p);
    CharString_return title();
    void title(CharString_in _p);
    Long xor_pixel();
    void xor_pixel(Long _p);
    Screen_return window_screen();
    Style_return window_style();
    Viewer_return main_viewer();
    void configure(const Window::Placement& p);
    void get_configuration(Boolean position, Window::Placement& p);
    void configure_notify(Coord width, Coord height);
    void move_notify(Coord left, Coord bottom);
    void map();
    void unmap();
    Boolean is_mapped();
    void map_notify();
    void unmap_notify();
    void iconify();
    void deiconify();
    void raise();
    void lower();
    void redraw(Coord left, Coord bottom, Coord width, Coord height);
    void repair();
    void handle_event(Event_in e);
    void grab_pointer(Cursor_in c);
    void ungrab_pointer();
    //+

    //+ Damage::=
    void incur();
    void extend(Region_in r);
    Region_return current();
    //+

    virtual void default_geometry();
    virtual void compute_geometry();
    virtual void bind();
    virtual Boolean bound();
    virtual void set_attributes();
    virtual void set_props();
    virtual void do_map();
    virtual void unbind();

    virtual void need_repair();

    DisplayImpl* display();
    ScreenImpl* screen_impl();
    ScreenImpl::VisualInfo* visual();
    XDisplay* xdisplay();
    XWindow xwindow();
    PixelCoord pwidth() { return pwidth_; }
    PixelCoord pheight() { return pheight_; }
protected:
    SharedFrescoObjectImpl object_;
    LockObj_var lock_;
    DisplayImpl* display_;
    ScreenImpl* screen_;
    StyleImpl* style_;
    CursorRef cursor_;
    ColorRef cursor_foreground_;
    ColorRef cursor_background_;
    WindowRef icon_;
    ScreenImpl::VisualInfo* visual_;
    MainViewer* viewer_;
    XPainterImpl* painter_;
    LockObj_var damage_lock_;
    RegionImpl* damage_;
    GlyphTraversalImpl* draw_;
    GlyphTraversalImpl* pick_;
    Window::Placement placement_;
    XWindow xwindow_;
    XSetWindowAttributes xattrs_;
    unsigned long xattrmask_;
    unsigned int xclass_;
    PixelCoord xleft_, xtop_;
    PixelCoord pwidth_, pheight_;

    Boolean placed_ : 1;
    Boolean needs_resize_ : 1;
    Boolean resized_ : 1;
    Boolean moved_ : 1;
    Boolean unmapped_ : 1;
    Boolean wm_mapped_ : 1;
    Boolean map_pending_ : 1;

    static Boolean xbound(XWindow);
    void set_attribute(const char* name, CharStringRef value);
    CharStringRef get_attribute(const char* name);
    void clear_mapping_info();
    void check_binding();
    void check_position();
    void compute_x_position();
    void damage(Coord l, Coord b, Coord r, Coord t);
    void do_request(Glyph::Requisition&);
    void do_draw_traversal();
    void moved(PixelCoord x, PixelCoord y);
    void resize();
};

inline DisplayImpl* WindowImpl::display() { return display_; }
inline ScreenImpl* WindowImpl::screen_impl() { return screen_; }
inline ScreenImpl::VisualInfo* WindowImpl::visual() { return visual_; }
inline XDisplay* WindowImpl::xdisplay() { return display_->xdisplay(); }
inline XWindow WindowImpl::xwindow() { return xwindow_; }

class ManagedWindow : public WindowImpl {
public:
    struct HintInfo {
	XWMHints* hints;
	XDisplay* xdisplay;
	XWindow xwindow;
    };

    typedef Boolean (ManagedWindow::*HintFunction)(ManagedWindow::HintInfo&);

    ManagedWindow(DisplayImpl*, ScreenImpl*, ViewerRef);
    virtual ~ManagedWindow();

    void iconify(); //+ Window::iconify
    void deiconify(); //+ Window::deiconify

    virtual void compute_geometry();
    virtual void set_props();
protected:
    WindowRef group_leader_;
    WindowRef transient_for_;
private:
    void do_set(ManagedWindow::HintFunction);

    Boolean set_name(ManagedWindow::HintInfo&);
    Boolean set_geometry(ManagedWindow::HintInfo&);
    Boolean set_group_leader(ManagedWindow::HintInfo&);
    Boolean set_transient_for(ManagedWindow::HintInfo&);
    Boolean set_icon_name(ManagedWindow::HintInfo&);
    Boolean set_icon_geometry(ManagedWindow::HintInfo&);
    Boolean set_icon(ManagedWindow::HintInfo&);
    Boolean set_icon_bitmap(ManagedWindow::HintInfo&);
    Boolean set_icon_mask(ManagedWindow::HintInfo&);
    Boolean set_all(ManagedWindow::HintInfo&);

    void wm_normal_hints();
    void wm_name();
    void wm_class();
    void wm_protocols();
    void wm_colormap_windows();
    void wm_hints();
};

class ApplicationWindow : public ManagedWindow {
public:
    ApplicationWindow(DisplayImpl*, ScreenImpl*, ViewerRef);
    virtual ~ApplicationWindow();

    virtual void compute_geometry();
    virtual void set_props();
};

class TopLevelWindow : public ManagedWindow {
public:
    TopLevelWindow(
	DisplayImpl*, ScreenImpl*, ViewerRef, WindowRef group_leader
    );
    virtual ~TopLevelWindow();
};

class TransientWindow : public ManagedWindow {
public:
    TransientWindow(
	DisplayImpl*, ScreenImpl*, ViewerRef, WindowRef transient_for
    );
    virtual ~TransientWindow();

    virtual void set_attributes();
};

class PopupWindow : public WindowImpl {
public:
    PopupWindow(DisplayImpl*, ScreenImpl*, ViewerRef);
    virtual ~PopupWindow();

    virtual void set_attributes();
};

class IconWindow : public ManagedWindow {
public:
    IconWindow(DisplayImpl*, ScreenImpl*, ViewerRef);
    virtual ~IconWindow();

    virtual void do_map();
};

#endif
