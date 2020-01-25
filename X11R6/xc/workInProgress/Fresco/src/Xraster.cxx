/*
 * $XConsortium: Xraster.cxx,v 1.5 94/04/01 16:47:49 matt Exp $
 */

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
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY
 * OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 */

#include <X11/Fresco/display.h>
#include <X11/Fresco/drawing.h>
#include <X11/Fresco/fresco.h>
#include <X11/Fresco/Impls/fresco-impl.h>
#include <X11/Fresco/Impls/region.h>
#include <X11/Fresco/Impls/transform.h>
#include <X11/Fresco/Impls/Xdisplay.h>
#include <X11/Fresco/Impls/Xdrawing.h>
#include <X11/Fresco/Impls/Xpainter.h>
#include <X11/Fresco/Impls/Xraster.h>
#include <X11/Fresco/Impls/Xwindow.h>
#include <X11/Fresco/Impls/Xlib.h>
#include <X11/Fresco/Impls/Xutil.h>
#include <X11/Fresco/OS/math.h>
#include <X11/Fresco/OS/memory.h>
#include <X11/Fresco/OS/table.h>
#include <X11/Xatom.h>
#include <float.h>

typedef unsigned char pxl;	// shorthand
typedef unsigned int uint;

unsigned long RasterImpl::time_;

// Functions to round towards minus infinity and plus infinity.
// We use "fuzz" to avoid rounding down or up when we are "fuzz" close
// to bound, i.e. round_minf(0.999) == 1, not 0  and
// round_minf(-1.001) == -1, not -2.
static const float fuzz = 0.01;
static inline int round_minf(Coord v) {
    return (v >= 0) ? int(v + fuzz) : -int(-(v) - fuzz + 1 - FLT_EPSILON);
}

// Same as above. round_pinf(1.001) == 1, not 2 and round_pinf(-1.999) == -2
// not -1.
static inline int round_pinf(Coord v) {
    return (v >= 0) ? int(v + 1 - FLT_EPSILON - fuzz) : -int(-v + fuzz);
}

RasterImpl::RasterImpl(
    Raster::Index rows, Raster::Index columns,
    Raster::Index origin_row, Raster::Index origin_column, Coord scale
) {
    rows_ = rows;
    columns_ = columns;
    origin_y_ = origin_row;
    origin_x_ = origin_column;
    scale_ = scale;
    hash_ = 0;
    per_screen_ = nil;
}

RasterImpl::~RasterImpl() {
    invalidate();
}

//+ RasterImpl(FrescoObject::ref__)
Long RasterImpl::ref__(Long references) { return object_.ref__(references); }

//+ RasterImpl(FrescoObject::attach)
Tag RasterImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}

//+ RasterImpl(FrescoObject::detach)
void RasterImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}

//+ RasterImpl(FrescoObject::disconnect)
void RasterImpl::disconnect() {
    PerScreenData** ps = &per_screen_;
    while (*ps != nil) {
	if (((*ps)->screen)->ref__(0) <= 0) {
	    PerScreenData* psd = *ps;
	    *ps = psd->next;
	} else {
	    ps = &(*ps)->next;
	}
    }
    object_.disconnect();
}

//+ RasterImpl(FrescoObject::notify_observers)
void RasterImpl::notify_observers() {
    object_.notify_observers();
}

//+ RasterImpl(FrescoObject::update)
void RasterImpl::update() {
    object_.update();
}

//+ RasterImpl(Raster::equal)
Boolean RasterImpl::equal(Raster_in r) {
    if (r == this) {
	return true;
    }
    if (rows_ != r->rows()
	|| columns_ != r->columns()
	|| origin_x_ != r->origin_x()
	|| origin_y_ != r->origin_y()
	|| scale_ != r->scale()
	|| hash_ != r->hash()) {
	return false;
    }

    // Here, derived classes are responsible for performing better
    // tests to assure equality.
    return true;
}

//+ RasterImpl(Raster::hash)
ULong RasterImpl::hash() { return hash_; }

//+ RasterImpl(Raster::rows)
Raster::Index RasterImpl::rows() { return rows_; }

//+ RasterImpl(Raster::columns)
Raster::Index RasterImpl::columns() { return columns_; }

//+ RasterImpl(Raster::origin_x)
Raster::Index RasterImpl::origin_x() { return origin_x_; }

//+ RasterImpl(Raster::origin_y)
Raster::Index RasterImpl::origin_y() { return origin_y_; }

//+ RasterImpl(Raster::scale=s)
void RasterImpl::scale(Coord s) { scale_ = s; }

//+ RasterImpl(Raster::scale?)
Coord RasterImpl::scale() { return scale_; }

RasterBitmap* RasterImpl::bitmap() { return nil; }

void RasterImpl::invalidate() {
    while (per_screen_ != nil) {
	PerScreenData& psd = *per_screen_;
	per_screen_ = psd.next;
	if (psd.pixmap != nil) {
	    XFreePixmap(psd.xdisplay, psd.pixmap);
	}
	if (psd.mask != nil) {
	    XFreePixmap(psd.xdisplay, psd.mask);
	}

	Fresco::unref(psd.trans);
    }
}

RasterImpl::PerScreenData* RasterImpl::lookup(WindowImpl* w, TransformRef t) {
    ScreenImpl* screen = w->screen_impl();
    ScreenImpl::VisualInfo* visual = w->visual();

    for (PerScreenData* psd = per_screen_; psd != nil; psd = psd->next) {
	if (psd->screen == screen && psd->visual == visual &&
	    psd->trans->equal(t)
	) {
	    psd->time = time_++;
	    return psd;
	}
    }
    psd = create(w, t);
    if (psd != nil) {
	psd->next = per_screen_;
	per_screen_ = psd;
    }
    return psd;
}

TransformRef RasterImpl::device_transform(TransformRef t, ScreenImpl* screen) {
    TransformRef internal_t = new TransformImpl;
    Vertex v;

    // Compute the matrix to map to device coordinates
    if (is_not_nil(t)) {
	internal_t->load(t);
    }

    if (scale_ != 0 && ! Math::equal(screen->to_coord(1), scale_, .01f)) {
	Coord to_pxl = 1/ screen->to_coord(1);
	v.x = scale_ * to_pxl;
	v.y = v.x;
	v.z = v.x;
	internal_t->scale(v);
    }
    return internal_t;
}

void RasterImpl::transform_bounds(
    TransformRef t,
    Raster::Index& rows, Raster::Index& columns,
    Coord& origin_x, Coord& origin_y,
    Coord& start_x, Coord& start_y,
    Coord& dx_x, Coord& dy_x, Coord& dx_y, Coord& dy_y
) {
    if (is_nil(t)) {
	start_x = 0;
	start_y = 0;
	dx_x = 1;
	dy_x = 0;
	dx_y = 0;
	dy_y = 1;
	return;
    }

    Vertex boundaries[4];
    Vertex lower, upper, origin;

    origin.x = origin_x;
    origin.y = origin_y;
    origin.z = 0;
    t->transform_vertex(origin);

    boundaries[0].x = 0;
    boundaries[0].y = 0;
    boundaries[0].z = 0;
    boundaries[2].x = columns;
    boundaries[2].y = rows;
    boundaries[2].z = 0;
    boundaries[1].x = boundaries[2].x;
    boundaries[1].y = 0;
    boundaries[1].z = 0;
    boundaries[3].x = 0;
    boundaries[3].y = boundaries[2].y;
    boundaries[3].z = 0;

    for (int i = 0; i < 4; i++) {
	t->transform_vertex(boundaries[i]);
    }

    lower = boundaries[0];
    upper = boundaries[0];
    for (i = 1; i < 4; i++) {
	RegionImpl::merge_min(lower, boundaries[i]);
	RegionImpl::merge_max(upper, boundaries[i]);
    }

    // Map back into bitmap
    boundaries[0] = lower;
    boundaries[2] = upper;
    boundaries[1].x = boundaries[2].x;
    boundaries[1].y = boundaries[0].y;
    boundaries[1].z = 0;
    boundaries[3].x = boundaries[0].x;
    boundaries[3].y = boundaries[2].y;
    boundaries[3].z = 0;

    for (i = 0; i < 4; i++) {
	t->inverse_transform_vertex(boundaries[i]);
    }
    
    // Change to device coordinates
    for (i = 0; i < 4; i++) {
	boundaries[i].y = rows - boundaries[i].y;
    }

    columns = round_pinf(upper.x) - round_minf(lower.x);
    rows = round_pinf(upper.y) - round_minf(lower.y);
    origin_x = lower.x - origin.x;
    origin_y = origin.y - upper.y;

    // If fact, we go from top to bottom in X format
    dx_x = (boundaries[2].x - boundaries[3].x) / columns;
    dy_x = (boundaries[2].y - boundaries[3].y) / columns;
    dx_y = (boundaries[0].x - boundaries[3].x) / rows;
    dy_y = (boundaries[0].y - boundaries[3].y) / rows;
    start_x = boundaries[3].x;
    start_y = boundaries[3].y;
}

RasterRef RasterImpl::read_drawable(
    Raster::Index /* rows */, Raster::Index /* columns */,
    Raster::Index /* origin_row */, Raster::Index /* origin_column */,
    ScreenImpl* /* screen */, XDrawable /* drawable */,
    int /* from_x */, int /* from_y */,
    int /* to_x */, int /* to_y */
) {
    return nil;
}

RasterBitmap::RasterBitmap(
    DrawingKit::Data8 data,
    Raster::Index rows, Raster::Index columns,
    Raster::Index origin_row, Raster::Index origin_column,
    Coord scale
) : RasterImpl(rows, columns, origin_row, origin_column, scale) {
    bytes_per_row_ = (columns_ + 7) / 8;
    size_ = rows_ * bytes_per_row_;
    data_ = new unsigned char[size_];
    if (data._length == size_ && data._buffer != nil) {
	Memory::copy(data._buffer, data_, size_t(size_));
    }
}

RasterBitmap::~RasterBitmap() {
    delete [] data_;
}

//+ RasterBitmap(Raster::equal)
Boolean RasterBitmap::equal(Raster_in r) {
    if (RasterImpl::equal(r)) {
	RasterImpl* ri = RasterImpl::_narrow(r);
	if (ri != nil) {
	    RasterBitmap* i = ri->bitmap();
	    if (i != nil) {
		return Memory::compare(data_, i->data_, size_) == 0;
	    }
	}
	/*
	 * When all else fails here, we should extract the data
	 * (slow, ugh) and compare it with our data.
	 */
    }
    return false;
}

//+ RasterBitmap(Raster::hash)
ULong RasterBitmap::hash() {
    if (hash_ == 0) {
	unsigned long hash = 0;
	unsigned long* ptr = (unsigned long*) data_;
	for (unsigned long i = (size_ / sizeof(unsigned long)); i > 0; i--) {
	    hash ^= *ptr++;
	}
	hash_ = hash;
    }
    return hash_;
}

Boolean RasterBitmap::inline_peek(
    Raster::Index row, Raster::Index column
) {
    unsigned long index = row * bytes_per_row_ + (column >> 3);
    unsigned char mask = pxl(1 << (column & 7));

    return (data_[index] & mask);
}

//+ RasterBitmap(Raster::peek)
void RasterBitmap::peek(Index row, Index column, Element& e) {
    if (row >= rows_ || column >= columns_) {
	e.on = false;
	e.pixel = nil;
	e.blend = 0;
	return;
    }

    row = rows_ - row;		// bitmap is kept in X format

    if (inline_peek(row, column)) {
	e.on = true;
	e.pixel = nil;
	e.blend = 1;
    } else {
	e.on = false;
	e.pixel = nil;
	e.blend = 0;
    }
}

//+ RasterBitmap(Raster::poke)
void RasterBitmap::poke(Index row, Index column, const Element& e) {
    if (row >= rows_ || column >= columns_) {
	return;
    }

    row = rows_ - row;		// bitmap is kept in X format

    unsigned long index = (row * bytes_per_row_) + (column >> 3);
    unsigned char mask = pxl(1 << (column & 7));
    if (e.on) {
	data_[index] |= mask;
    } else {
	data_[index] &= ~mask;
    }
    invalidate();
}

RasterImpl::PerScreenData* RasterBitmap::create(
    WindowImpl* w, TransformRef t
) {
    // Compute transform to map to device coordinates
    ScreenImpl* screen = w->screen_impl();
    Transform_var internal_t = device_transform(t, screen);
    Raster::Index width = columns_, height = rows_;
    unsigned char* data = data_;
    Boolean clip_output = false;
    Coord origin_x = origin_x_;
    Coord origin_y = origin_y_;

    if (is_not_nil(internal_t) && ! internal_t->identity()) {
	clip_output = compute_transform(
	    internal_t, width, height, origin_x, origin_y, data
	);
	// Check for singular transforms
	if (width == 0 || height == 0) {
	    return nil;
	}
    } else {
	origin_y = origin_y - rows_;
    }

    RasterImpl::PerScreenData* psc = new RasterImpl::PerScreenData;
    psc->xdisplay = w->display()->xdisplay();
    psc->screen = screen;
    psc->visual = w->visual();
    Transform::Matrix m;
    t->store_matrix(m);
    psc->trans = new TransformImpl(m);
    psc->mask = nil;
    psc->pwidth = short(width);
    psc->pheight = short(height);
    psc->origin_x = origin_x;
    psc->origin_y = origin_y;
    psc->pixmap = XCreateBitmapFromData(
	psc->xdisplay, w->xwindow(),
	(char*)data, psc->pwidth, psc->pheight
    );
    psc->clip_output = clip_output;
    psc->cost = (
	((psc->pwidth/8 + sizeof(int) - 1) / sizeof(int)) *
	sizeof(int) * psc->pheight
    );
    psc->time = time_++;
    psc->next = nil;
    if (data != data_) {
	delete [] data;
    }
    return psc;
}

void RasterBitmap::read_drawable(
    XDisplay* xdpy, XDrawable drawable, int from_x, int from_y
) {
    XImage* image = XGetImage(
	xdpy, drawable,
	from_x, from_y, int(from_x + columns_), int(from_y + rows_),
	0x1, ZPixmap
    );
    pxl bit = 1;
    pxl pixel = 0;
    pxl* points = data_;

    for (int y = 0; y < rows_; y++) {
	for (int x = 0; x < columns_; x++) {
	    if (XGetPixel(image, x, y)) {
		pixel |= bit;
	    } else {
		pixel &= ~bit;
	    }
	    bit <<= 1;
	    if (bit == 0) {
		bit = 1;
		// flush
		*points = pixel;
		points++;
		pixel = 0;
	    }
	}
	if (bit != 1) {
	    // flush
	    *points = pixel;
	    points++;
	    bit = 1;
	    pixel = 0;
	}
    }
    XDestroyImage(image);
}

RasterBitmap* RasterBitmap::bitmap() { return this; }

void RasterBitmap::invalidate() {
    RasterImpl::invalidate();
    hash_ = 0;
}

static const float epsilon = 0.0001;

static inline Boolean is_zero(float d) {
    return Math::equal(d, 0, epsilon);
}

Boolean RasterBitmap::compute_transform(
    TransformRef t, 
    Raster::Index& width, Raster::Index& height,
    Coord& origin_x, Coord& origin_y, 
    unsigned char*& data
) {
    if (t->det_is_zero()) {
	width = 0;
	height = 0;
	return false;
    }
    Coord from_x, from_y, dx_x, dy_x, dx_y, dy_y;
    transform_bounds(
	t,
	height, width, origin_x, origin_y,
	from_x, from_y, dx_x, dy_x, dx_y, dy_y
    );
    if (width == 0 || height == 0) {
	return false;
    }
    uint size = uint(height * ((width + 7) / 8));

    data = new pxl[size];

    pxl bit = 1;
    pxl pixel = 0;
    pxl* points = data;
    Coord x, y;

    for (Raster::Index y_counter = height; y_counter > 0; y_counter--) {
	x = from_x;
	y = from_y;
	for (Raster::Index x_counter = width; x_counter > 0; x_counter--) {
	    int px = round_minf(x);
	    int py = round_minf(y);
	    if (px >= 0 && px < columns_
		&& py >= 0 && py < rows_) {
		if (inline_peek(py, px)) {
		    pixel |= bit;
		} else {
		    pixel &= ~bit;
		}
	    } else {
		pixel &= ~bit;
	    }

	    bit <<= 1;
	    if (bit == 0) {
		bit = 1;
		// flush
		*points = pixel;
		points++;
		pixel = 0;
	    }
	    x += dx_x;
	    y += dy_x;
	}
	from_x += dx_y;
	from_y += dy_y;

	if (bit != 1) {
	    // flush
	    *points = pixel;
	    points++;
	    bit = 1;
	    pixel = 0;
	}

    }
    // We should clip if we are not rectangular
    return !( (dx_x == 0 && dy_y == 0) || (dy_x == 0 && dx_y == 0) );
}

RasterChannels::RasterChannels(
    int channel_count, RasterChannels::Intensity* channels[],
    Raster::Index rows, Raster::Index columns,
    Raster::Index origin_row, Raster::Index origin_column, Coord scale
) : RasterImpl(rows, columns, origin_row, origin_column, scale) {
    channel_count_ = channel_count;
    if (channel_count_ == 0) {
	channel_count_ = 1;
    }
    size_t size = size_t(rows_ * columns_);
    channels_ = new RasterChannels::Intensity*[channel_count_];
    for (int i = 0; i < channel_count_; i++) {
	channels_[i] = new RasterChannels::Intensity[size];
	if (channels != nil && channels[i] != nil) {
	    Memory::copy(channels[i], channels_[i], size);
	}
    }
}

RasterChannels::RasterChannels(
    int channel_count, int pad, RasterChannels::RGBA* channels,
    Raster::Index rows, Raster::Index columns,
    Raster::Index origin_row, Raster::Index origin_column, Coord scale
) : RasterImpl(rows, columns, origin_row, origin_column, scale) {
    channel_count_ = channel_count;
    if (channel_count_ == 0) {
	channel_count_ = 1;
    }
    size_t size = size_t(rows_ * columns_);
    channels_ = new RasterChannels::Intensity*[channel_count_];
    for (int channel = 0; channel < channel_count_; channel++) {
	channels_[channel] = new RasterChannels::Intensity[size];
    }
    for (int i = 0; i < size; i++) {
        for (channel = 0; channel < channel_count_; channel++) {
	    channels_[channel][i] = *channels;
	    channels++;
	}
	channels += pad;
    }
}

RasterChannels::~RasterChannels() {
    for (int i = 0; i < channel_count_; i++) {
	delete[] channels_[i];
	channels_[i] = nil;
    }
    delete[] channels_;
}

void RasterChannels::peek(
    Raster::Index row, Raster::Index column, Raster::Element& e
) {
    if (row >= rows_ || column >= columns_) {
	e.on = false;
	e.blend = 0;
	e.pixel = nil;
	return;
    }

    // Assertion here:
    //  1 channel:	grey.
    //  2 channels:	grey alpha
    //  3 channels:	R G B
    //  4 channels and > :	R G B alpha

    unsigned long index = (rows_ - row) * columns_ + column;
    e.on = true;
    e.blend = 1;
    e.pixel = nil;
    switch (channel_count_) {
    case 2:
	e.blend = channels_[1][index] / 255.0;
	e.on = (e.blend != 0);
	// fall through
    case 1: {
	Color::Intensity grey = channels_[0][index] / 255.0;
	e.pixel = new ColorImpl(grey, grey, grey);
    }
	return;
    case 4:
	e.blend = channels_[3][index] / 255.0;
	e.on = (e.blend != 0);
	// fall through
    case 3: {
	Color::Intensity r = channels_[0][index] / 255.0;
	Color::Intensity g = channels_[1][index] / 255.0;
	Color::Intensity b = channels_[2][index] / 255.0;
	e.pixel = new ColorImpl(r, g, b);
    }
	return;
    }
}

void RasterChannels::poke(
    Raster::Index row, Raster::Index column, const Raster::Element& e
) {
    if (row >= rows_ || column >= columns_) {
	return;
    }

    unsigned long index = (rows_ - row) * columns_ + column;
    Color::Intensity r, g, b;
    
    switch (channel_count_) {
    case 2:
	channels_[1][index] = pxl(e.blend * 255.0);
	// fall through
    case 1:
	e.pixel->rgb(r, g, b);
	channels_[0][index] = pxl((0.299 * r + 0.587 * g + 0.114 * b) * 255);
	return;
    case 4:
	channels_[3][index] = pxl(e.blend * 255.0);
	// fall through
    case 3:
	e.pixel->rgb(r, g, b);
	channels_[0][index] = pxl(r * 255);
	channels_[1][index] = pxl(g * 255);
	channels_[2][index] = pxl(b * 255);
	return;
    }
}

RasterImpl::PerScreenData* RasterChannels::create(
    WindowImpl* w, TransformRef t
) {
    // Compute transform to map to device coordinates
    ScreenImpl* screen = w->screen_impl();
    Transform_var internal_t = device_transform(t, screen);
    Raster::Index width = columns_, height = rows_;
    Coord origin_x = origin_x_;
    Coord origin_y = origin_y_;
    Pixmap pixmap = nil;
    unsigned char* mask = nil;
    Boolean clip_output = false;
    XDisplay* xdpy = w->xdisplay();

    if (is_not_nil(internal_t) && ! internal_t->identity()) {
	clip_output = compute_transform(
	    internal_t,
	    width, height, origin_x, origin_y, 
	    pixmap, mask, w
	);
	// Check for singular transforms
	if (width == 0 || height == 0) {
	    return nil;
	}
    } else {
	ScreenImpl::VisualInfo* vi = w->visual();
	XDrawable xwindow = w->xwindow();
	XCoord pwidth = XCoord(width);
	XCoord pheight = XCoord(height);
	Long depth = vi->depth;

	origin_y = origin_y - rows_;

	pixmap =  XCreatePixmap(xdpy, xwindow, pwidth, pheight, int(depth));
	XImage* image = XGetImage(
	    xdpy, pixmap, 0, 0, pwidth, pheight, AllPlanes, ZPixmap
	);
	if (has_alpha()) {
	    uint size = uint(height * ((width + 7) / 8));
	    mask = new pxl[size];
	}
	pxl bit = 1;
	pxl pixel = 0;
	pxl* points = mask;
	long index = 0;

	// Here, we play with X order so we don't need to reverse Y coords.
	XColor xc;
	xc.pixel = 0;
	for (int y = 0; y < height; y++) {
	    for (int x = 0; x < width; x++) {
		switch (channel_count_) {
		case 2:
		    if (channels_[1][index] != 0) {
			pixel |= bit;
		    } else {
			pixel &= ~bit;
		    }
		    // fall through
		case 1:
		    xc.red = xc.green = xc.blue = channels_[0][index] << 8;
		    break;
		case 4:
		    if (channels_[1][index] != 0) {
			pixel |= bit;
		    } else {
			pixel &= ~bit;
		    }
		    // fall through
		case 3:
		    xc.red = channels_[0][index] << 8;
		    xc.green = channels_[1][index] << 8;
		    xc.blue = channels_[2][index] << 8;
		    break;
		}
		screen->find_color(*vi, xc);
		XPutPixel(image, x, y, xc.pixel);

		if (has_alpha()) {
		    bit <<= 1;
		    if (bit == 0) {
			bit = 1;
			// flush
			*points = pixel;
			points++;
			pixel = 0;
		    }
		}
		index++;
	    }
	    if (has_alpha()) {
		if (bit != 1) {
		    // flush
		    *points = pixel;
		    points++;
		    bit = 1;
		    pixel = 0;
		}
	    }
	}
	GC xgc = XCreateGC(xdpy, pixmap, 0, nil);
	XSetForeground(xdpy, xgc, 0);
	XPutImage(
	    xdpy, pixmap, xgc,
	    image, 0, 0, 0, 0, XCoord(width), XCoord(height)
	);
	XFreeGC(xdpy, xgc);
	XDestroyImage(image);
    }

    RasterImpl::PerScreenData* psc = new RasterImpl::PerScreenData;
    psc->xdisplay = xdpy;
    psc->screen = screen;
    psc->visual = w->visual();
    Transform::Matrix m;
    t->store_matrix(m);
    psc->trans = new TransformImpl(m);
    psc->pixmap = pixmap;
    psc->pwidth = short(width);
    psc->pheight = short(height);
    psc->origin_x = origin_x;
    psc->origin_y = origin_y;
    if (mask != nil) {
	psc->mask = XCreateBitmapFromData(
	    xdpy, w->xwindow(),
	    (char*)mask, psc->pwidth, psc->pheight
	);
	delete[] mask;
    } else {
	psc->mask = nil;
    }
    psc->clip_output = clip_output;
    psc->cost = psc->pwidth * psc->pheight * channel_count_;
    psc->time = time_++;
    psc->next = nil;
    return psc;
}

void RasterChannels::read_drawable(
    XDisplay* xdpy, XDrawable drawable, int from_x, int from_y
) {
    XImage* image = XGetImage(
	xdpy, drawable,
	from_x, from_y, int(from_x + columns_), int(from_y + rows_),
	0x1, ZPixmap
    );

    for (int y = 0; y < rows_; y++) {
	for (int x = 0; x < columns_; x++) {
	    ULong pixel = XGetPixel(image, x, y);
	    
	}
    }
    XDestroyImage(image);
}

Boolean RasterChannels::compute_transform(
    TransformRef t, 
    Raster::Index& width, Raster::Index& height,
    Coord& origin_x, Coord& origin_y, 
    Pixmap& pixmap, unsigned char*& mask, WindowImpl* w
) {
    if (t->det_is_zero()) {
	width = 0;
	height = 0;
	return false;
    }
    Coord from_x, from_y, dx_x, dy_x, dx_y, dy_y;
    transform_bounds(
	t,
	height, width, origin_x, origin_y,
	from_x, from_y, dx_x, dy_x, dx_y, dy_y
    );
    if (width == 0 || height == 0) {
	return false;
    }
    
    ScreenImpl* screen = w->screen_impl();
    ScreenImpl::VisualInfo* vi = w->visual();
    XDisplay* xdpy = w->xdisplay();
    XDrawable xwindow = w->xwindow();
    
    pixmap =  XCreatePixmap(
	xdpy, xwindow,
	XCoord(width), XCoord(height), int(vi->depth)
    );
    XImage* image = XGetImage(
	xdpy, pixmap,
	0, 0, XCoord(width), XCoord(height), AllPlanes, ZPixmap
    );
    
    if (has_alpha()) {
	uint size = uint(height * ((width + 7) / 8));
	mask = new pxl[size];
    }

    pxl bit = 1;
    pxl pixel = 0;
    pxl* points = mask;
    Coord x, y;

    // We move on the P0->P3 line
    XColor xc;
    xc.pixel = 0;
    for (int dest_y = 0; dest_y < height; dest_y++) {
	x = from_x;
	y = from_y;
	for (int dest_x = 0; dest_x < width; dest_x++) {
	    int px = round_minf(x); // we look at the pixel center
	    int py = round_minf(y);
	    if (px >= 0 && px < columns_ && py >= 0 && py < rows_) {
		int index = int(py * columns_ + px);

		switch (channel_count_) {
		case 2:
		    if (channels_[1][index] != 0) {
			pixel |= bit;
		    } else {
			pixel &= ~bit;
		    }
		    // fall through
		case 1:
		    xc.red = xc.green = xc.blue = channels_[0][index] << 8;
		    break;
		case 4:
		    if (channels_[1][index] != 0) {
			pixel |= bit;
		    } else {
			pixel &= ~bit;
		    }
		    // fall through
		case 3:
		    xc.red = channels_[0][index] << 8;
		    xc.green = channels_[1][index] << 8;
		    xc.blue = channels_[2][index] << 8;
		}
		screen->find_color(*vi, xc);
	    } else {
		xc.pixel = 0;
		if (has_alpha()) {
		    pixel &= ~bit;
		}
	    }
		
	    XPutPixel(image, dest_x, dest_y, xc.pixel);

	    if (has_alpha()) {
		bit <<= 1;
		if (bit == 0) {
		    bit = 1;
		    // flush
		    *points = pixel;
		    points++;
		    pixel = 0;
		}
	    }
	    x += dx_x;
	    y += dy_x;
	}
	from_x += dx_y;
	from_y += dy_y;
	if (has_alpha()) {
	    if (bit != 1) {
		// flush
		*points = pixel;
		points++;
		bit = 1;
		pixel = 0;
	    }
	}
    }
    GC xgc = XCreateGC(xdpy, pixmap, 0, nil);
    XSetForeground(xdpy, xgc, 0);
    XPutImage(
	xdpy, pixmap, xgc,
	image, 0, 0, 0, 0, XCoord(width), XCoord(height)
    );
    XFreeGC(xdpy, xgc);
    XDestroyImage(image);
    // We should clip if we are not rectangular
    return ! ((dx_x == 0 && dy_y == 0) || (dy_x == 0 && dx_y == 0));
}

RasterLUT::RasterLUT(
    RasterLUT::LUT lut, pxl* data,
    Raster::Index rows, Raster::Index columns,
    Raster::Index origin_row, Raster::Index origin_column, Coord scale
) : RasterImpl(rows, columns, origin_row, origin_column, scale) {
    for (int i = 0; i < 256; i++) {
	lut_[i] = lut[i];	// no need to ref
    }
    
    int size = int(rows * columns);
    data_ = new pxl[size];
    if (data != nil) {
	Memory::copy(data, data_, size);
    }
}

RasterLUT::~RasterLUT() {
    for (int i = 0; i < 256; i++) {
	Fresco::unref(lut_[i]);
	lut_[i] = nil;
    }
}

void RasterLUT::peek(
    Raster::Index row, Raster::Index column, Raster::Element& e
) {
    if (row >= rows_ || column >= columns_) {
	e.on = false;
	e.blend = 0;
	e.pixel = nil;
	return;
    }

    unsigned long index = (rows_ - row) * columns_ + column;
    e.on = true;
    e.blend = 1;
    e.pixel = lut_[data_[index]];
}

void RasterLUT::poke(
    Raster::Index row, Raster::Index column, const Raster::Element& e
) {
    if (row >= rows_ || column >= columns_ || e.pixel == nil) {
	return;
    }

    pxl color = find_color(e.pixel);
    unsigned long index = (rows_ - row) * columns_ + column;
    data_[index] = color;
}

static inline float sqr(float f) { return f*f; }

pxl RasterLUT::find_color(ColorRef color) {
    int first_free = -1;
    for (int i = 0; i < 256; i++) {
	if (lut_[i] == nil && first_free == -1) {
	    first_free = i;
	} else {
	    if (lut_[i]->equal(color)) {
		return pxl(i);
	    }
	}
    }
    if (first_free != -1) {
	lut_[first_free] = color;
	return first_free;
    }
    float min_dist = 3;
    int min_index = -1;
    Color::Intensity r0, g0, b0;
    color->rgb(r0, g0, b0);

    for (i = 0; i < 256; i++) {
	Color::Intensity r, g, b;
	if (lut_[i] != nil) {
	    lut_[i]->rgb(r, g, b);
	    float dist = sqr(r-r0)+sqr(g-g0)+sqr(b-b0);
	    if (dist > min_dist) {
		min_dist = dist;
		min_index = i;
	    }
	}
    }
    return pxl(min_index);
}

RasterImpl::PerScreenData* RasterLUT::create(WindowImpl* w, TransformRef t) {
    // Compute transform to map to device coordinates
    ScreenImpl* screen = w->screen_impl();
    TransformRef internal_t = device_transform(t, screen);
    Raster::Index width = columns_, height = rows_;
    Coord origin_x = origin_x_;
    Coord origin_y = origin_y_;
    Pixmap pixmap = nil;
    Boolean clip_output = false;
    XDisplay* xdpy = w->xdisplay();

    // We assume the color indexes don't change. Compute the mapping
    // from local color map to X color map on the visual/screen.
    unsigned long x_lut[256];
    ScreenImpl::VisualInfo* vi = w->visual();

    for (int i = 0; i < 256; i++) {
	XColor xc;
	Color::Intensity r, g, b;
	if (is_not_nil(lut_[i])) {
	    lut_[i]->rgb(r, g, b);
	    xc.red = (unsigned short)(r * 0xFFFF);
	    xc.green = (unsigned short)(g * 0xFFFF);
	    xc.blue = (unsigned short)(b * 0xFFFF);
	    screen->find_color(*vi, xc);
	    x_lut[i] = xc.pixel;
	} else {
	    x_lut[i] = 0;
	}
    }

    if (is_not_nil(internal_t) && ! internal_t->identity()) {
	clip_output = compute_transform(
	    internal_t, width, height, origin_x, origin_y, pixmap, x_lut, w
	);
	// Check for singular transforms
	if (width == 0 || height == 0) {
	    return nil;
	}
    } else {
	XDrawable xwindow = w->xwindow();
	XCoord pwidth = XCoord(width);
	XCoord pheight = XCoord(height);
	Long depth = vi->depth;
	
	origin_y = origin_y - rows_;

        pixmap =  XCreatePixmap(xdpy, xwindow, pwidth, pheight, int(depth));
	XImage* image = XGetImage(
	    xdpy, pixmap,
	    0, 0, pwidth, pheight, AllPlanes, ZPixmap
	);

	// Here, we play with X order so we don't need to reverse Y coords.
	pxl* pxls = data_;
	for (int y = 0; y < height; y++) {
	    for (int x = 0; x < width; x++) {
		XPutPixel(image, x, y, x_lut[*pxls]);
		pxls++;
	    }
	}
	GC xgc = XCreateGC(xdpy, pixmap, 0, nil);
	XSetForeground(xdpy, xgc, 0);
	XPutImage(
	    xdpy, pixmap, xgc,
	    image, 0, 0, 0, 0, XCoord(width), XCoord(height)
	);
	XFreeGC(xdpy, xgc);
	XDestroyImage(image);
    }

    RasterImpl::PerScreenData* psc = new RasterImpl::PerScreenData;
    psc->xdisplay = xdpy;
    psc->screen = screen;
    psc->visual = w->visual();
    Transform::Matrix m;
    t->store_matrix(m);
    psc->trans = new TransformImpl(m);
    psc->pixmap = pixmap;
    psc->mask = nil;
    psc->pwidth = short(width);
    psc->pheight = short(height);
    psc->origin_x = origin_x;
    psc->origin_y = origin_y;
    psc->clip_output = clip_output;
    psc->cost = psc->pwidth * psc->pheight;
    psc->time = time_++;
    psc->next = nil;
    return psc;
}

void RasterLUT::read_drawable(
    XDisplay* xdpy, XDrawable drawable, int from_x, int from_y
) {
    XImage* image = XGetImage(
	xdpy, drawable,
	from_x, from_y, int(from_x + columns_), int(from_y + rows_),
	0x1, ZPixmap
    );

    for (int y = 0; y < rows_; y++) {
	for (int x = 0; x < columns_; x++) {
	    ULong pixel = XGetPixel(image, x, y);
	}
    }
    XDestroyImage(image);
}


Boolean RasterLUT::compute_transform(
    TransformRef t, 
    Raster::Index& width, Raster::Index& height,
    Coord& origin_x, Coord& origin_y, 
    Pixmap& pixmap, unsigned long x_lut[256], WindowImpl* w
) {
    if (t->det_is_zero()) {
	width = 0;
	height = 0;
	return false;
    }
    Coord from_x, from_y, dx_x, dy_x, dx_y, dy_y;
    transform_bounds(
	t,
	height, width, origin_x, origin_y,
	from_x, from_y, dx_x, dy_x, dx_y, dy_y
    );
    if (width == 0 || height == 0) {
	return false;
    }

    ScreenImpl* screen = w->screen_impl();
    ScreenImpl::VisualInfo* vi = w->visual();
    XDisplay* xdpy = w->xdisplay();;
    XDrawable xwindow = w->xwindow();
    XCoord pwidth = XCoord(width);
    XCoord pheight = XCoord(height);
    Long depth = vi->depth;
    
    pixmap =  XCreatePixmap(xdpy, xwindow, pwidth, pheight, int(depth));
    XImage* image = XGetImage(
	xdpy, pixmap, 0, 0, XCoord(width), XCoord(height), AllPlanes, ZPixmap
    );
    
    Coord x, y;
    for (int dest_y = 0; dest_y < height; dest_y++) {
	x = from_x;
	y = from_y;
	for (int dest_x = 0; dest_x < width; dest_x++) {
	    unsigned long pixel;
	    int px = round_minf(x); // we look at the pixel center
	    int py = round_minf(y);
	    if (px >= 0 && px < columns_ && py >= 0 && py < rows_) {
		pixel = x_lut[data_[py * columns_ + px]];
	    } else {
		pixel = 0;
	    }

	    XPutPixel(image, dest_x, dest_y, pixel);

	    x += dx_x;
	    y += dy_x;
	}
	from_x += dx_y;
	from_y += dy_y;
    }
    GC xgc = XCreateGC(xdpy, pixmap, 0, nil);
    XSetForeground(xdpy, xgc, 0);
    XPutImage(
	xdpy, pixmap, xgc,
	image, 0, 0, 0, 0, XCoord(width), XCoord(height)
    );
    XFreeGC(xdpy, xgc);
    XDestroyImage(image);
    // We should clip if we are not rectangular
    return !( (dx_x == 0 && dy_y == 0) || (dy_x == 0 && dx_y == 0) );
}
