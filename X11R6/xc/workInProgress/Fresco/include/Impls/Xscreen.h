/* $XConsortium: Xscreen.h,v 1.5 94/09/01 18:47:45 matt Exp $ */
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

#ifndef Fresco_Impls_Xscreen_h
#define Fresco_Impls_Xscreen_h

#include <X11/Fresco/display.h>
#include <X11/Fresco/Impls/fobjects.h>
#include <X11/Fresco/Impls/Xlib.h>
#include <X11/Fresco/Impls/Xutil.h>

class DisplayImpl;
class RGBTable;
class ScreenVisualList;

typedef int XCoord;

//+ ScreenImpl : Screen
class ScreenImpl : public Screen {
public:
    ~ScreenImpl();
    TypeObjId _tid();
    static ScreenImpl* _narrow(BaseObjectRef);
//+
public:
    ScreenImpl(Fresco*, DisplayImpl*, long n);

    //+ Screen::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Screen */
    Coord dpi();
    void dpi(Coord _p);
    Display_return screen_display();
    Coord width();
    Coord height();
    PixelCoord to_pixels(Coord c);
    Coord to_coord(PixelCoord p);
    Coord to_pixels_coord(Coord c);
    void move_pointer(Coord x, Coord y);
    Window_return application(Viewer_in v);
    Window_return top_level(Viewer_in v, Window_in group_leader);
    Window_return transient(Viewer_in v, Window_in transient_for);
    Window_return popup(Viewer_in v);
    Window_return icon(Viewer_in v);
    //+

    struct OverlayInfo {
	VisualID id;
	long type;
	long transparent;
	long layer;
    };

    struct VisualInfo {
	XDisplay* display;
	long screen;
	long depth;
	Visual* visual;
	XColormap cmap;
	OverlayInfo overlay;
	RGBTable* rgb_table;
	XColor* local_map;
	long local_map_size;
	unsigned long red, green, blue;
	unsigned long red_shift, green_shift, blue_shift;
	unsigned long white;
	unsigned long xor;
    };

    struct RGBTableEntry {
	unsigned short red, green, blue;
    };

    XWindow root_window();
    ScreenImpl::VisualInfo* default_visual();
    ScreenImpl::VisualInfo* find_visual(StyleRef);
    void find_color(ScreenImpl::VisualInfo&, XColor&);
protected:
    SharedFrescoObjectImpl object_;
    Fresco* fresco_;
    DisplayImpl* display_;
    long number_;
    XWindow root_;
    ScreenVisualList* visuals_;
    ScreenImpl::VisualInfo* default_visual_;
    Coord width_;
    Coord height_;
    XCoord pwidth_;
    XCoord pheight_;
    Coord pixels_;
    Coord points_;

    void set_dpi();
    ScreenImpl::VisualInfo* lookup_overlay(StyleRef);
    Boolean find_layer(StyleValueRef, Long& layer);
    Boolean find_overlay(long layer, XVisualInfo&, ScreenImpl::OverlayInfo&);
    Boolean lookup_visual_id(StyleRef, XVisualInfo&);
    Boolean lookup_visual(StyleRef, XVisualInfo&);
    Boolean find_visual_by_class_name(CharStringRef name, XVisualInfo& xinfo);
    void find_overlay(long layer, ScreenImpl::VisualInfo&);
    Boolean find_visual_by_info(XVisualInfo&, long mask);
    void init_visual(ScreenImpl::VisualInfo&);
    void init_color_tables(ScreenImpl::VisualInfo&);
    void set_shift(unsigned long mask, unsigned long& v, unsigned long& shift);
    unsigned int MSB(unsigned long);
    unsigned long xor(const ScreenImpl::VisualInfo&, StyleRef);
    void load_map(ScreenImpl::VisualInfo&);
    void search_map(
	ScreenImpl::VisualInfo&, ScreenImpl::RGBTableEntry& rgb, XColor& xc
    );
    double distance(
	unsigned short r, unsigned short g, unsigned short b, const XColor&
    );
    unsigned long rescale(
	unsigned long value, unsigned long in_scale, unsigned long out_scale
    );
};

#endif
