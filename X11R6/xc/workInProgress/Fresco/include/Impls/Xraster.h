/*
 * Copyright (c) 1993 2001 S.A.
 * Copyright (c) 1993 Silicon Graphics, Inc.
 * Copyright (c) 1993 Fujitsu, Ltd.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names
 * of 2001 S.A., Silicon Graphics, and Fujitsu may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of 2001 S.A., Silicon Graphics, and Fujitsu.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL 2001 S.A., SILICON GRAPHICS, OR FUJITSU BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#ifndef Fresco_Impls_Xraster_h
#define Fresco_Impls_Xraster_h

#include <X11/Fresco/display.h>
#include <X11/Fresco/drawing.h>
#include <X11/Fresco/types.h>
#include <X11/Fresco/Impls/fresco-impl.h>
#include <X11/Fresco/Impls/Xlib.h>
#include <X11/Fresco/Impls/Xscreen.h>

/*
 * RasterImpl -- implementation of rasters
 */

class RasterBitmap;
class WindowImpl;

//- RasterImpl*
//+ RasterImpl : Raster
class RasterImpl : public Raster {
public:
    ~RasterImpl();
    TypeObjId _tid();
    static RasterImpl* _narrow(BaseObjectRef);
//+
    //. RasterImpl is a base class for common raster implementations
    //. on X.  It does not implement the raster data (i.e., peek and
    //. poke), but provides shared operations for mapping raster
    //. coordinates to screen coordinates.
public:
    RasterImpl(
	Raster::Index rows, Raster::Index columns,
	Raster::Index origin_row, Raster::Index origin_column,
	Coord scale
    );

    //+ FrescoObject::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    //+

    Boolean equal(Raster_in r); //+ Raster::equal
    ULong hash(); //+ Raster::hash
    Index rows(); //+ Raster::rows
    Index columns(); //+ Raster::columns
    Index origin_x(); //+ Raster::origin_x
    Index origin_y(); //+ Raster::origin_y
    void scale(Coord); //+ Raster::scale=
    Coord scale(); //+ Raster::scale?

    struct PerScreenData {
	// Validity of this image
	XDisplay* xdisplay;
	ScreenImpl* screen;
	ScreenImpl::VisualInfo* visual;
	TransformRef trans;

	// Internal representation
	Pixmap pixmap;
	Pixmap mask;
	unsigned short pwidth;
	unsigned short pheight;
	Coord origin_x;
	Coord origin_y;
	Boolean clip_output;

	// Rough estimation of the cost for the server
	unsigned long cost;

	// Last time it was used
	unsigned long time;

	PerScreenData* next;
    };

    virtual RasterBitmap* bitmap();
    virtual void invalidate();
    virtual RasterImpl::PerScreenData* lookup(WindowImpl*, TransformRef);
    virtual RasterImpl::PerScreenData* create(
	WindowImpl*, TransformRef
    ) = 0;
    virtual void read_drawable(
	XDisplay*, XDrawable drawable, int from_x, int from_y
    ) = 0;

    SharedFrescoObjectImpl object_;
    Raster::Index rows_;
    Raster::Index columns_;
    Coord origin_x_;
    Coord origin_y_;
    Coord scale_;
    unsigned long hash_;
    PerScreenData* per_screen_;

    TransformRef device_transform(TransformRef, ScreenImpl* screen);
    static void transform_bounds(
	TransformRef t,
	Raster::Index& rows, Raster::Index& columns,
	Coord& origin_x, Coord& origin_y,
	Coord& from_x, Coord& from_y,
	Coord& dx_x, Coord& dy_x, Coord& dx_y, Coord& dy_y
    );
    static RasterRef read_drawable(
	Raster::Index rows, Raster::Index columns,
	Raster::Index origin_row, Raster::Index origin_column,
	ScreenImpl*, XDrawable drawable,
	int from_x, int from_y,
	int to_x, int to_y
    );
    static unsigned long time_;
};

//- RasterBitmap*
class RasterBitmap : public RasterImpl {
    //. RasterBitmap provides a raster implementation for bitmaps
    //. using X images.
public:
    RasterBitmap(
	DrawingKit::Data8 data,
	Raster::Index rows, Raster::Index columns,
	Raster::Index origin_row, Raster::Index origin_column,
	Coord scale = 0
    );
    ~RasterBitmap();

    Boolean equal(Raster_in r); //+ Raster::equal
    ULong hash(); //+ Raster::hash
    void peek(Index row, Index column, Element& e); //+ Raster::peek
    void poke(Index row, Index column, const Element& e); //+ Raster::poke

    virtual RasterBitmap* bitmap();
    virtual void invalidate();
    virtual RasterImpl::PerScreenData* create(WindowImpl*, TransformRef);
    virtual void read_drawable(
	XDisplay*, XDrawable drawable, int from_x, int from_y
    );

    Boolean compute_transform(
	TransformRef,
	Raster::Index& column, Raster::Index& row,
	Coord& x, Coord& y, unsigned char*& data
    );

    Boolean inline_peek(Raster::Index row, Raster::Index column);

    unsigned char* data_;
    ULong size_;
    ULong bytes_per_row_;
};

//- RasterChannels*
class RasterChannels : public RasterImpl {
    //. RasterChannels is an implementation of Raster supporting
    //. different numbers of channels, where 1 channels means
    //. grey, 2 means grey with alpha, 3 means RGB, and 4 means
    //. RGB with alpha.
public:
    typedef unsigned char Intensity;
    typedef unsigned char RGBA;

    RasterChannels(
	int channel_count, RasterChannels::Intensity* channels[],
	Raster::Index rows, Raster::Index columns,
	Raster::Index origin_row, Raster::Index origin_column,
	Coord scale = 0
    );
    RasterChannels(
	int channel_count,  int pad, RasterChannels::RGBA* channels,
	Raster::Index rows, Raster::Index columns,
	Raster::Index origin_row, Raster::Index origin_column,
	Coord scale = 0
    );
    ~RasterChannels();

    void peek(Raster::Index row, Raster::Index column, Raster::Element&);
    void poke(Raster::Index row, Raster::Index column, const Raster::Element& e);
    virtual RasterImpl::PerScreenData* create(WindowImpl*, TransformRef);
    virtual void read_drawable(
	XDisplay*, XDrawable drawable, int from_x, int from_y
    );

    RasterChannels::Intensity** channels_;
    int channel_count_;

    Boolean compute_transform(
	TransformRef,
	Raster::Index& column, Raster::Index& row,
	Coord& x, Coord& y,
	Pixmap& pixmap, unsigned char*& mask, WindowImpl* w
    );
    Boolean has_alpha() const;
};

//- RasterLUT*
class RasterLUT : public RasterImpl {
    //. Raster implementation using a color lookup table.
public:
    typedef ColorRef LUT[256];

    RasterLUT(
	LUT lut, unsigned char* data,
	Raster::Index rows, Raster::Index columns,
	Raster::Index origin_row, Raster::Index origin_column,
	Coord scale = 0
    );
    ~RasterLUT();

    void peek(Raster::Index row, Raster::Index column, Raster::Element&);
    void poke(Raster::Index row, Raster::Index column, const Raster::Element& e);
    virtual RasterImpl::PerScreenData* create(WindowImpl*, TransformRef);
    virtual void read_drawable(
	XDisplay*, XDrawable drawable, int from_x, int from_y
    );
    Boolean compute_transform(
	TransformRef,
	Raster::Index& column, Raster::Index& row,
	Coord& x, Coord& y,
	Pixmap& image, unsigned long x_lut[256], WindowImpl* w
    );
    unsigned char find_color(ColorRef);

    unsigned char* data_;
    LUT lut_;
};

inline Boolean RasterChannels::has_alpha() const {
    return channel_count_ == 2 || channel_count_ == 4;
}

#endif
