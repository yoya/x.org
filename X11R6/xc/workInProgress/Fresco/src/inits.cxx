/*
 * $XConsortium: inits.cxx,v 1.3 94/04/01 16:48:00 matt Exp $
 */

/*
 * Copyright (c) 1992-93 Silicon Graphics, Inc.
 * Copyright (c) 1993 Fujitsu, Ltd.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the name of
 * Silicon Graphics and Fujitsu may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Silicon Graphics and Fujitsu.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL SILICON GRAPHICS OR FUJITSU BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

#include <X11/Fresco/display.h>
#include <X11/Fresco/drawing.h>
#include <X11/Fresco/figures.h>
#include <X11/Fresco/glyph.h>
#include <X11/Fresco/layouts.h>
#include <X11/Fresco/widgets.h>
#include <X11/Fresco/Impls/fresco-impl.h>
#include <X11/Fresco/Ox/marshal.h>
#include <X11/Fresco/Ox/request.h>
#include <X11/Fresco/Ox/schema.h>
#include <X11/Fresco/Ox/typeobjs.h>

/*
 * Type ids.  Some day we should generate this automatically,
 * but that requires a registration system and persistence ...
 */

/* Basic types */
TypeObjId TypeIdVar(Action) = 100;
TypeObjId TypeIdVar(Adjustment) = 101;
TypeObjId TypeIdVar(XfAdjustment_Settings) = 102;
TypeObjId TypeIdVar(Alignment) = 103;
TypeObjId TypeIdVar(Axis) = 104;
TypeObjId TypeIdVar(CharCode) = 105;
TypeObjId TypeIdVar(CharString) = 106;
TypeObjId TypeIdVar(Coord) = 107;
TypeObjId TypeIdVar(Cursor) = 108;
/* unused gap */
TypeObjId TypeIdVar(Damage) = 110;
TypeObjId TypeIdVar(Display) = 111;
TypeObjId TypeIdVar(XfDisplay_ScreenNumber) = 112;
TypeObjId TypeIdVar(DisplayImpl) = 113;
/* unused gap */
TypeObjId TypeIdVar(Event) = 120;
TypeObjId TypeIdVar(XfEvent_ButtonIndex) = 121;
TypeObjId TypeIdVar(XfEvent_KeyChord) = 122;
TypeObjId TypeIdVar(XfEvent_KeySym) = 123;
TypeObjId TypeIdVar(XfEvent_Modifier) = 124;
TypeObjId TypeIdVar(XfEvent_TimeStamp) = 125;
TypeObjId TypeIdVar(XfEvent_TypeId) = 126;
TypeObjId TypeIdVar(EventImpl) = 127;
/* unused gap */
TypeObjId TypeIdVar(Focus) = 130;
TypeObjId TypeIdVar(FrescoObject) = 131;
TypeObjId TypeIdVar(Glyph) = 132;
TypeObjId TypeIdVar(XfGlyph_AllocationInfo) = 133;
TypeObjId TypeIdVar(XfGlyph_AllocationInfoSeq) = 134;
TypeObjId TypeIdVar(XfGlyph_OffsetSeq) = 135;
TypeObjId TypeIdVar(XfGlyph_Requirement) = 136;
TypeObjId TypeIdVar(XfGlyph_Requisition) = 137;
TypeObjId TypeIdVar(GlyphImpl) = 138;
TypeObjId TypeIdVar(GlyphOffset) = 139;
TypeObjId TypeIdVar(GlyphTraversal) = 140;
TypeObjId TypeIdVar(XfGlyphTraversal_Operation) = 141;
/* unused gap */
TypeObjId TypeIdVar(PixelCoord) = 150;
TypeObjId TypeIdVar(Region) = 151;
TypeObjId TypeIdVar(XfRegion_BoundingSpan) = 152;
TypeObjId TypeIdVar(Screen) = 153;
TypeObjId TypeIdVar(ScreenImpl) = 154;
TypeObjId TypeIdVar(Style) = 155;
TypeObjId TypeIdVar(StyleValue) = 156;
TypeObjId TypeIdVar(StyleVisitor) = 157;
/* unused gap */
TypeObjId TypeIdVar(Tag) = 170;
TypeObjId TypeIdVar(Transform) = 171;
TypeObjId TypeIdVar(XfTransform_Matrix) = 172;
TypeObjId TypeIdVar(XfCoord_array_4) = 173;
TypeObjId TypeIdVar(XfCoord_array_4_4) = 174;
TypeObjId TypeIdVar(Vertex) = 175;
TypeObjId TypeIdVar(Viewer) = 176;
/* unused gap */
TypeObjId TypeIdVar(Window) = 180;
TypeObjId TypeIdVar(XfWindow_Placement) = 181;
TypeObjId TypeIdVar(WindowImpl) = 182;

/* DrawingKit types */
TypeObjId TypeIdVar(DrawingKit) = 200;
TypeObjId TypeIdVar(XfDrawingKit_Data8) = 201;
TypeObjId TypeIdVar(XfDrawingKit_Data32) = 202;
TypeObjId TypeIdVar(XfLong_array_16) = 203;
TypeObjId TypeIdVar(Brush) = 204;
TypeObjId TypeIdVar(BrushImpl) = 205;
TypeObjId TypeIdVar(Color) = 206;
TypeObjId TypeIdVar(ColorImpl) = 207;
TypeObjId TypeIdVar(XfColor_Intensity) = 208;
TypeObjId TypeIdVar(Font) = 209;
TypeObjId TypeIdVar(FontImpl) = 210;
TypeObjId TypeIdVar(XfFont_Info) = 211;
TypeObjId TypeIdVar(Painter) = 212;
TypeObjId TypeIdVar(XPainterImpl) = 213;
TypeObjId TypeIdVar(Pattern) = 214;
TypeObjId TypeIdVar(PatternImpl) = 215;
TypeObjId TypeIdVar(Raster) = 216;
TypeObjId TypeIdVar(XfRaster_Element) = 217;
TypeObjId TypeIdVar(XfRaster_Index) = 218;
TypeObjId TypeIdVar(RasterImpl) = 219;

/* FigureKit types */
TypeObjId TypeIdVar(FigureKit) = 300;
TypeObjId TypeIdVar(XfFigureKit_Mode) = 301;
TypeObjId TypeIdVar(XfFigureKit_Vertices) = 302;
TypeObjId TypeIdVar(FigureStyle) = 303;

/* LayoutKit types */
TypeObjId TypeIdVar(LayoutKit) = 400;

/* WidgetKit types */
TypeObjId TypeIdVar(WidgetKit) = 500;
TypeObjId TypeIdVar(Button) = 501;
TypeObjId TypeIdVar(Menu) = 502;
TypeObjId TypeIdVar(MenuItem) = 503;
TypeObjId TypeIdVar(Telltale) = 504;
TypeObjId TypeIdVar(XfTelltale_Flag) = 505;

/*
 * Descriptors are generated in stubs.
 */

extern TypeObj_Descriptor
    TypeVar(XfBoolean),
    TypeVar(XfOctet),
    TypeVar(XfFloat),
    TypeVar(XfLong),
    TypeVar(XfULong),
    TypeVar(Action),
    TypeVar(Adjustment),
    TypeVar(Brush), TypeVar(BrushImpl),
    TypeVar(Button),
    TypeVar(CharString),
    TypeVar(Color), TypeVar(ColorImpl),
    TypeVar(Cursor),
    TypeVar(Damage),
    TypeVar(Display), TypeVar(DisplayImpl),
    TypeVar(DrawingKit),
    TypeVar(Event), TypeVar(EventImpl),
    TypeVar(FigureKit), TypeVar(FigureStyle),
    TypeVar(Focus),
    TypeVar(Font), TypeVar(FontImpl),
    TypeVar(FrescoObject),
    TypeVar(Glyph), TypeVar(GlyphImpl),
    TypeVar(GlyphOffset),
    TypeVar(GlyphTraversal),
    TypeVar(LayoutKit),
    TypeVar(Menu), TypeVar(MenuItem),
    TypeVar(Painter), TypeVar(XPainterImpl),
    TypeVar(Pattern), TypeVar(PatternImpl),
    TypeVar(Region),
    TypeVar(Raster), TypeVar(RasterImpl),
    TypeVar(Screen), TypeVar(ScreenImpl),
    TypeVar(Style), TypeVar(StyleVisitor),
    TypeVar(Telltale),
    TypeVar(Transform),
    TypeVar(Viewer),
    TypeVar(WidgetKit),
    TypeVar(Window), TypeVar(WindowImpl);

/*
 * Define types.  The interfaces are generated automatically,
 * but currently ix does not generate the typedefs.  So we must
 * do so by hand.  This should be fixed in the future.
 */

DefineType(
    XfAdjustment_Settings, Adjustment::Settings, "Adjustment::Settings"
)
DefineType(Alignment, Alignment, "Alignment")
DefineType(Axis, Axis, "Axis")
DefineType(CharCode, CharCode, "CharCode")
DefineType(XfCoord_array_4, Coord[4], "Coord[4]")
DefineType(XfCoord_array_4_4, Coord[4][4], "Coord[4][4]")
DefineType(Coord, Coord, "Coord")
DefineType(
    XfDisplay_ScreenNumber, Display::ScreenNumber, "ScreenNumber"
)
DefineType(XfEvent_ButtonIndex, Event::ButtonIndex, "ButtonIndex")
DefineType(XfEvent_KeySym, Event::KeySym, "KeySym")
DefineType(XfEvent_KeyChord, Event::KeyChord, "KeyChord")
DefineType(XfEvent_Modifier, Event::Modifier, "Modifier")
DefineType(XfEvent_TimeStamp, Event::TimeStamp, "TimeStamp")
DefineType(XfEvent_TypeId, Event::TypeId, "TypeId")
DefineType(
    XfGlyph_AllocationInfo, Glyph::AllocationInfo, "AllocationInfo"
)
DefineType(
    XfGlyph_AllocationInfoSeq, Glyph::AllocationInfoSeq, "AllocationInfoSeq"
)
DefineType(XfGlyph_OffsetSeq, Glyph::OffsetSeq, "OffsetSeq")
DefineType(XfGlyph_Requirement, Glyph::Requirement, "Requirement")
DefineType(XfGlyph_Requisition, Glyph::Requisition, "Requisition")
DefineType(XfGlyphTraversal_Operation, GlyphTraversal::Operation, "Operation")

DefineType(PixelCoord, PixelCoord, "PixelCoord")
DefineType(XfRegion_BoundingSpan, Region::BoundingSpan, "BoundingSpan")
DefineType(Tag, Tag, "Tag")
DefineType(XfTransform_Matrix, Transform::Matrix, "Matrix")
DefineType(Vertex, Vertex, "Vertex")
DefineType(XfWindow_Placement, Window::Placement, "Window_Placement")

static TypeObj_Descriptor* adjustment_settings_fields[] = {
    &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord),
    &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord),
    nil
};

static TypeObj_Descriptor* glyph_allocationinfo_fields[] = {
    &TypeVar(Region), &TypeVar(Transform), &TypeVar(Damage),
    nil
};

static TypeObj_Descriptor* glyph_requirement_fields[] = {
    &TypeVar(Boolean), &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord),
    &TypeVar(Alignment), nil
};

static TypeObj_Descriptor* glyph_requisition_fields[] = {
    &TypeVar(XfGlyph_Requirement), &TypeVar(XfGlyph_Requirement),
    &TypeVar(XfGlyph_Requirement), &TypeVar(Boolean), nil
};

static TypeObj_Descriptor* region_boundingspan_fields[] = {
    &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord),
    &TypeVar(Alignment), nil
};

static TypeObj_Descriptor* vertex_fields[] = {
    &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord), nil
};

static TypeObj_Descriptor* window_placement_fields[] = {
    &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord),
    &TypeVar(Coord), &TypeVar(Alignment), &TypeVar(Alignment), nil
};

/* Define DrawingKit types */

DefineType(XfDrawingKit_Data8, DrawingKit::Data8, "DrawingKit::Data8")
DefineType(XfDrawingKit_Data32, DrawingKit::Data32, "DrawingKit::Data32")
DefineType(XfLong_array_16, long[16], "long[16]")
DefineType(XfColor_Intensity, Color::Intensity, "Intensity")
DefineType(XfFont_Info, Font::Info, "Info")
DefineType(XfRaster_Element, Raster::Element, "Element")
DefineType(XfRaster_Index, Raster::Index, "Index")

static TypeObj_Descriptor* font_info_fields[] = {
    &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord),
    &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord), &TypeVar(Coord),
    nil
};

static TypeObj_Descriptor* raster_element_fields[] = {
    &TypeVar(Boolean), &TypeVar(Color), &TypeVar(XfFloat), nil
};

/* Define FigureKit types */

DefineType(XfFigureKit_Mode, FigureKit::Mode, "Mode")
DefineType(XfFigureKit_Vertices, FigureKit::Vertices, "Vertices")

/* Define WidgetKit types */
DefineType(XfTelltale_Flag, Telltale::Flag, "Flag")

static TypeObj_Descriptor* fresco_schema[] = {
    /* Basic types */
    &TypeVar(Action),
    &TypeVar(Adjustment),
    &TypeVar(CharString),
    &TypeVar(Cursor),
    &TypeVar(Damage),
    &TypeVar(Display),
    &TypeVar(DisplayImpl),
    &TypeVar(Event),
    &TypeVar(EventImpl),
    &TypeVar(Focus),
    &TypeVar(FrescoObject),
    &TypeVar(Glyph),
    &TypeVar(GlyphImpl),
    &TypeVar(GlyphOffset),
    &TypeVar(GlyphTraversal),
    &TypeVar(Region),
    &TypeVar(Screen),
    &TypeVar(ScreenImpl),
    &TypeVar(Style),
    &TypeVar(StyleVisitor),
    &TypeVar(Transform),
    &TypeVar(Viewer),
    &TypeVar(Window),
    &TypeVar(WindowImpl),

    /* DrawingKit types */
    &TypeVar(DrawingKit),
    &TypeVar(XfDrawingKit_Data8),
    &TypeVar(XfDrawingKit_Data32),
    &TypeVar(XfLong_array_16),
    &TypeVar(Brush),
    &TypeVar(BrushImpl),
    &TypeVar(Color),
    &TypeVar(ColorImpl),
    &TypeVar(XfColor_Intensity),
    &TypeVar(Font),
    &TypeVar(FontImpl),
    &TypeVar(XfFont_Info),
    &TypeVar(Painter),
    &TypeVar(XPainterImpl),
    &TypeVar(Pattern),
    &TypeVar(PatternImpl),
    &TypeVar(Raster),
    &TypeVar(XfRaster_Element),
    &TypeVar(XfRaster_Index),
    &TypeVar(RasterImpl),

    /* FigureKit types */
    &TypeVar(FigureKit),
    &TypeVar(FigureStyle),

    /* LayoutKit types */
    &TypeVar(LayoutKit),

    /* WidgetKit types */
    &TypeVar(WidgetKit),
    &TypeVar(Button),
    &TypeVar(Menu),
    &TypeVar(MenuItem),
    &TypeVar(Telltale),
    &TypeVar(XfTelltale_Flag),

    nil
};

void FrescoImpl::init_types() {
    TypeSchemaImpl::schema()->load_list(fresco_schema);

    /* Primitive types */
    new VoidTypeObj;
    new BooleanTypeObj;
    new CharTypeObj;
    new OctetTypeObj;
    new ShortTypeObj;
    new UShortTypeObj;
    new LongTypeObj;
    new ULongTypeObj;
    new LongLongTypeObj;
    new ULongLongTypeObj;
    new FloatTypeObj;
    new DoubleTypeObj;
    new StringTypeObj;

    /* Basic types */
    make_struct(XfAdjustment_Settings, adjustment_settings_fields);
    make_typedef(Alignment, XfFloat);
    make_enum(Axis, 3);
    make_typedef(CharCode, XfLong);
    make_array(XfCoord_array_4, Coord, 4);
    make_array(XfCoord_array_4_4, XfCoord_array_4, 4);
    make_typedef(Coord, XfFloat);
    make_typedef(XfDisplay_ScreenNumber, XfLong);
    make_typedef(XfEvent_ButtonIndex, XfLong);
    make_typedef(XfEvent_KeySym, XfULong);
    make_sequence(XfEvent_KeyChord, XfEvent_KeySym);
    make_enum(XfEvent_Modifier, 4);
    make_typedef(XfEvent_TimeStamp, XfULong);
    make_enum(XfEvent_TypeId, 7);
    make_struct(XfGlyph_AllocationInfo, glyph_allocationinfo_fields);
    make_sequence(XfGlyph_AllocationInfoSeq, XfGlyph_AllocationInfo);
    make_sequence(XfGlyph_OffsetSeq, XfGlyphOffset);
    make_struct(XfGlyph_Requirement, glyph_requirement_fields);
    make_struct(XfGlyph_Requisition, glyph_requisition_fields);
    make_enum(XfGlyphTraversal_Operation, 4);
    make_typedef(PixelCoord, XfLong);
    make_struct(XfRegion_BoundingSpan, region_boundingspan_fields);
    make_typedef(Tag, XfLong);
    make_typedef(XfTransform_Matrix, XfCoord_array_4_4);
    make_struct(Vertex, vertex_fields);
    make_struct(XfWindow_Placement, window_placement_fields);

    /* DrawingKit types */
    make_sequence(XfDrawingKit_Data8, XfOctet);
    make_sequence(XfDrawingKit_Data32, XfLong);
    make_array(XfLong_array_16, XfLong, 16);
    make_typedef(XfColor_Intensity, XfFloat);
    make_struct(XfFont_Info, font_info_fields);
    make_struct(XfRaster_Element, raster_element_fields);
    make_typedef(XfRaster_Index, XfLong);

    /* FigureKit types */
    make_enum(XfFigureKit_Mode, 3);
    make_sequence(XfFigureKit_Vertices, Vertex);

    /* WidgetKit types */
    make_enum(XfTelltale_Flag, 8);
}
