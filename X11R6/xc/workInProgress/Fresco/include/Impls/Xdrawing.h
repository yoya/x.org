/* $XConsortium: Xdrawing.h,v 1.5 94/09/01 18:47:45 matt Exp $ */
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

#ifndef Fresco_Impls_Xdrawing_h
#define Fresco_Impls_Xdrawing_h

#include <X11/Fresco/drawing.h>
#include <X11/Fresco/Impls/fobjects.h>
#include <X11/Fresco/Impls/Xlib.h>

class DisplayImpl;
class DrawingKitColorTable;
class DrawingKitFontTable;

class DrawingKitImpl : public DrawingKit {
public:
    DrawingKitImpl(DisplayImpl*);
    ~DrawingKitImpl();

    //+ DrawingKit::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* DrawingKit */
    Style_return drawing_style();
    Brush_return simple_brush(Coord width);
    Brush_return dither_brush(Coord width, Long p);
    Brush_return patterned_brush(Coord width, const Data32& p);
    Pattern_return solid_pattern();
    Pattern_return halftone_pattern();
    Pattern_return stipple(const Data32& d);
    Pattern_return stipple_4x4(ULong p);
    Color_return color_rgb(Color::Intensity r, Color::Intensity g, Color::Intensity b);
    Color_return find_color(CharString_in name);
    Color_return resolve_color(Style_in s, CharString_in name);
    Color_return foreground(Style_in s);
    Color_return background(Style_in s);
    Font_return find_font(CharString_in fullname);
    Font_return find_font_match(CharString_in font_family, CharString_in font_style, Coord ptsize);
    Font_return resolve_font(Style_in s, CharString_in name);
    Font_return default_font(Style_in s);
    Raster_return bitmap_from_data(const Data8& d, Raster::Index rows, Raster::Index columns, Raster::Index origin_row, Raster::Index origin_column);
    Raster_return bitmap_from_char(Font_in f, CharCode c);
    Raster_return raster_open(CharString_in name);
    Transform_return transform_matrix(Transform::Matrix m);
    Transform_return identity_matrix();
    Painter_return printer(CharString_in output);
    //+
protected:
    SharedFrescoObjectImpl object_;
    DisplayImpl* display_;
    DrawingKitColorTable* colors_;
    DrawingKitFontTable* fonts_;
    CharString_var foreground_str_;
    CharString_var Foreground_str_;
    CharString_var background_str_;
    CharString_var Background_str_;
    CharString_var font_str_;
    CharString_var Font_str_;

    ColorRef resolve_colors(
	StyleRef s, CharString_in name1, CharString_in name2
    );
};

//+ BrushImpl : Brush
class BrushImpl : public Brush {
public:
    ~BrushImpl();
    TypeObjId _tid();
    static BrushImpl* _narrow(BaseObjectRef);
//+
public:
    BrushImpl(Coord width);
    BrushImpl(const Long* p, Long c, Coord w);
    BrushImpl(Long pat, Coord w);

    //+ Brush::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Brush */
    Coord width();
    Boolean equal(Brush_in b);
    ULong hash();
    //+

    void info(Coord& width, char*& dash_list, Long& dash_count);
protected:
    SharedFrescoObjectImpl object_;
    Coord width_;
    char* dash_list_;
    long dash_count_;

    void init(const Long* p, Long c, Coord w);
    void calc_dashes(Long pat, Long* dash, Long& count);
};

//+ ColorImpl : Color
class ColorImpl : public Color {
public:
    ~ColorImpl();
    TypeObjId _tid();
    static ColorImpl* _narrow(BaseObjectRef);
//+
public:
    ColorImpl(Color::Intensity r, Color::Intensity g, Color::Intensity b);

    //+ Color::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Color */
    void rgb(Intensity& r, Intensity& g, Intensity& b);
    Boolean equal(Color_in c);
    ULong hash();
    //+

    static unsigned short to_short(Color::Intensity);
    static Color::Intensity to_intensity(unsigned short);
protected:
    SharedFrescoObjectImpl object_;
    Color::Intensity red_;
    Color::Intensity green_;
    Color::Intensity blue_;
};

/*
 * The assumption here is that intensities are positive,
 * so the roundoff doesn't need to do a sign check.
 */

inline unsigned short ColorImpl::to_short(Color::Intensity i) {
    return (unsigned short)(i * float(0xffff) + 0.5);
}

inline Color::Intensity ColorImpl::to_intensity(unsigned short s) {
    return float(s) / float(0xffff);
}

//+ FontImpl : Font
class FontImpl : public Font {
public:
    ~FontImpl();
    TypeObjId _tid();
    static FontImpl* _narrow(BaseObjectRef);
//+
public:
    FontImpl(DisplayImpl*, const char* name);

    //+ Font::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Font */
    Boolean equal(Font_in f);
    ULong hash();
    CharString_return name();
    CharString_return encoding();
    Coord point_size();
    void font_info(Font::Info& i);
    void char_info(CharCode c, Font::Info& i);
    void string_info(CharString_in s, Font::Info& i);
    //+

    void load();
    XDisplay* xdisplay();
    XFontStruct* xfont();
    RasterRef bitmap(CharCode);
    Coord to_coord(PixelCoord);
    Coord average_width();
protected:
    SharedFrescoObjectImpl object_;
    CharStringRef name_;
    CharStringRef encoding_;
    Coord point_size_;
    Coord scale_;
    XDisplay* xdisplay_;
    XFontStruct* xfont_;
    RasterRef* rasters_;
};

inline XDisplay* FontImpl::xdisplay() { return xdisplay_; }
inline XFontStruct* FontImpl::xfont() { return xfont_; }

//+ PatternImpl : Pattern
class PatternImpl : public Pattern {
public:
    ~PatternImpl();
    TypeObjId _tid();
    static PatternImpl* _narrow(BaseObjectRef);
//+
public:
    PatternImpl(ULong);
    PatternImpl(const DrawingKit::Data32&);

    //+ Pattern::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Pattern */
    Boolean equal(Pattern_in p);
    ULong hash();
    void stipple(DrawingKit::Data32& d);
    //+
protected:
    SharedFrescoObjectImpl object_;
public:
    DrawingKit::Data32 stipple_;
};

#endif
