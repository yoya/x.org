/*
 * $XConsortium: types.cxx,v 1.9 94/09/14 20:31:38 matt Exp $
 */

/*
 * Copyright (c) 1992-93 Silicon Graphics, Inc.
 * Copyright (c) 1993 Fujitsu, Ltd.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names
 * of Silicon Graphics and Fujitsu may not be used in any advertising or
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

#include <X11/Fresco/Impls/action.h>
#include <X11/Fresco/Impls/charstr.h>
#include <X11/Fresco/Impls/fobjects.h>
#include <X11/Fresco/Impls/region.h>
#include <X11/Fresco/Impls/styles.h>
#include <X11/Fresco/Impls/transform.h>
#include <X11/Fresco/OS/file.h>
#include <X11/Fresco/OS/list.h>
#include <X11/Fresco/OS/math.h>
#include <X11/Fresco/OS/table.h>
#include <X11/Fresco/OS/thread.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

/*
 * Sony NEWS-OS 6.0 has conflicting prototypes for abs() in math.h
 * and stdlib.h, so you cannot include both headers in any C++ source
 * file.  Until that bug is fixed (in NEWS-OS 6.0.1), we'll have to
 * explicitly define the symbols we need from math.h on the Sony.
 * On other platforms we can just include math.h
 */

#if defined(sony) && defined(SVR4)
#define M_PI 3.14159265358979323846
extern "C" {
    double cos(double);
    double sin(double);
}
#else
#include <math.h>
#endif

/*
 * AIX lacks prototypes for strcasecmp and strncasecmp, even though they
 * are in the standard C library.
 */

#if defined(AIXV3)
extern "C" {
    int strcasecmp(const char *, const char *);
    int strncasecmp(const char *, const char *, int);
}
#endif /* AIXV3 */

CharStringImpl::CharStringImpl() {
    data_ = nil;
    length_ = 0;
    dynamic_ = false;
}

CharStringImpl::CharStringImpl(const char* s) {
    set(s, -1);
}

CharStringImpl::CharStringImpl(const char* s, Long length) {
    set(s, length);
}

CharStringImpl::CharStringImpl(CharStringRef s) {
    data_ = nil;
    dynamic_ = false;
    copy(s);
}

CharStringImpl::CharStringImpl(const CharStringImpl& s) {
    set(s.data_, s.length_);
}

CharStringImpl::~CharStringImpl() {
    free();
}

CharStringImpl* CharStringImpl::create_static(const char* str) {
    CharStringImpl* c = new CharStringImpl;
    c->data_ = (char*)str;
    c->length_ = strlen(str);
    c->dynamic_ = false;
    return c;
}

void CharStringImpl::set(const char* str, Long n) {
    length_ = (n == -1) ? strlen(str) : n;
    data_ = new char[length_ + 1];
    strncpy(data_, str, int(length_));
    data_[length_] = '\0';
    dynamic_ = true;
}

void CharStringImpl::free() {
    if (dynamic_) {
	delete data_;
    }
}

//+ CharStringImpl(FrescoObject::=object_.)
Long CharStringImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag CharStringImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void CharStringImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void CharStringImpl::disconnect() {
    object_.disconnect();
}
void CharStringImpl::notify_observers() {
    object_.notify_observers();
}
void CharStringImpl::update() {
    object_.update();
}
//+

//+ CharStringImpl(CharString::hash)
CharString::HashValue CharStringImpl::hash() {
    ULong v = 0;
    const char* q = &data_[length_];
    for (const char* p = data_; p < q; p++) {
	v = (v << 1) ^ (*p);
    }
    ULong t = v >> 10;
    t ^= (t >> 10);
    return v ^ t;
}

//+ CharStringImpl(CharString::count)
Long CharStringImpl::count() {
    return length_;
}

//+ CharStringImpl(CharString::copy)
void CharStringImpl::copy(CharString_in s) {
    free();
    length_ = s->count();
    data_ = new char[length_ + 1];
    CharString::CharData buf(length_ + 1, 0, data_);
    s->get_char_data(buf);
    data_[length_] = '\0';
    dynamic_ = true;
    /* avoid deallocating the newly-copied data */
    buf._buffer = nil;
}

//+ CharStringImpl(CharString::equal)
Boolean CharStringImpl::equal(CharString_in s) {
    if (length_ == s->count()) {
	CharStringBuffer buf(s);
	Boolean b = strncmp(data_, buf.string(), int(length_)) == 0;
	return b;
    }
    return false;
}

//+ CharStringImpl(CharString::case_insensitive_equal)
Boolean CharStringImpl::case_insensitive_equal(CharString_in s) {
    if (length_ == s->count()) {
	CharStringBuffer buf(s);
	Boolean b = strncasecmp(data_, buf.string(), int(length_)) == 0;
	return b;
    }
    return false;
}

//+ CharStringImpl(CharString::get_data)
void CharStringImpl::get_data(Data& d) {
    Long n = (length_ > d._maximum) ? d._maximum : length_;
    for (Long i = 0; i < n; i++) {
	d._buffer[i] = data_[i];
    }
}

//+ CharStringImpl(CharString::get_char_data)
void CharStringImpl::get_char_data(CharData& d) {
    Long m = d._maximum - 1;
    Long n = (length_ > m) ? m : length_;
    for (Long i = 0; i < n; i++) {
	d._buffer[i] = data_[i];
    }
    d._buffer[n] = '\0';
    d._length = n;
}

//+ CharStringImpl(CharString::put_data)
void CharStringImpl::put_data(const Data& d) {
    free();
    length_ = d._length;
    data_ = new char[length_];
    dynamic_ = true;
    for (Long i = 0; i < length_; i++) {
	data_[i] = (char)d._buffer[i];
    }
}

//+ CharStringImpl(CharString::put_char_data)
void CharStringImpl::put_char_data(const CharData& d) {
    free();
    length_ = d._length;
    data_ = new char[length_];
    dynamic_ = true;
    for (Long i = 0; i < length_; i++) {
	data_[i] = d._buffer[i];
    }
}

const char* CharStringImpl::index(const char* s, Long length, char c) {
    const char* p = s;
    const char* q = s + length;
    for (; p < q; p++) {
	if (*p == c) {
	    return p;
	}
    }
    return nil;
}

void CharStringBuffer::init(CharStringRef s) {
    Long n = s->count();
    if (n > sizeof(fixed_)) {
	buf_._buffer = new char[n + 1];
    } else {
	n = sizeof(fixed_);
	buf_._buffer = fixed_;
    }
    buf_._maximum = n;
    buf_._length = 0;
    s->get_char_data(buf_);
}

CharStringBuffer::~CharStringBuffer() {
    if (buf_._buffer == fixed_) {
	buf_._buffer = nil;
    }
}

Boolean CharStringBuffer::operator ==(const char* str) {
    return strcmp(buf_._buffer, str) == 0;
}

Boolean CharStringBuffer::operator ==(const CharStringBuffer& b) {
    return strcmp(buf_._buffer, b.buf_._buffer) == 0;
}

Boolean CharStringBuffer::operator !=(const char* str) {
    return strcmp(buf_._buffer, str) != 0;
}

Boolean CharStringBuffer::operator !=(const CharStringBuffer& b) {
    return strcmp(buf_._buffer, b.buf_._buffer) != 0;
}

ActionImpl::ActionImpl() { }
ActionImpl::~ActionImpl() { }

//+ ActionImpl(FrescoObject::=object_.)
Long ActionImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag ActionImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void ActionImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void ActionImpl::disconnect() {
    object_.disconnect();
}
void ActionImpl::notify_observers() {
    object_.notify_observers();
}
void ActionImpl::update() {
    object_.update();
}
//+

//+ ActionImpl(Action::execute)
void ActionImpl::execute() { }

//+ ActionImpl(Action::reversible)
Boolean ActionImpl::reversible() { return false; }

//+ ActionImpl(Action::unexecute)
void ActionImpl::unexecute() { }

RegionImpl::RegionImpl() {
    defined_ = false;
    lower_.x = lower_.y = 0;
    upper_.x = upper_.y = 0;
    xalign_ = yalign_ = zalign_ = 0;
    lower_.z = -1e6;
    upper_.z = 1e6;
}

RegionImpl::RegionImpl(RegionRef a, TransformRef t1, TransformRef t2) {
    TransformImpl cumulative;
    Boolean xf = false;
    if (is_not_nil(t1) && !t1->identity()) {
	cumulative.load(t1);
	xf = true;
    }
    if (is_not_nil(t2) && !t2->identity()) {
	cumulative.premultiply(t2);
	xf = true;
    }
    RegionImpl::copy(a);
    if (xf) {
	RegionImpl::apply_transform(&cumulative);
    }
}

RegionImpl::~RegionImpl() { }

//+ RegionImpl(FrescoObject::=object_.)
Long RegionImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag RegionImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void RegionImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void RegionImpl::disconnect() {
    object_.disconnect();
}
void RegionImpl::notify_observers() {
    object_.notify_observers();
}
void RegionImpl::update() {
    object_.update();
}
//+

//+ RegionImpl(Region::contains)
Boolean RegionImpl::contains(const Vertex& v) {
    return (
	defined_ &&
	v.x >= lower_.x && v.x <= upper_.x &&
	v.y >= lower_.y && v.y <= upper_.y &&
	v.z >= lower_.z && v.z <= upper_.z
    );
}

//+ RegionImpl(Region::contains_plane)
Boolean RegionImpl::contains_plane(const Vertex& v, Axis a) {
    Boolean b = false;
    if (defined_) {
	switch (a) {
	case X_axis:
	    b = (
		v.y >= lower_.y && v.y <= upper_.y &&
		v.z >= lower_.z && v.z <= upper_.z
	    );
	    break;
	case Y_axis:
	    b = (
		v.x >= lower_.x && v.x <= upper_.x &&
		v.z >= lower_.z && v.z <= upper_.z
	    );
	    break;
	case Z_axis:
	    b = (
		v.x >= lower_.x && v.x <= upper_.x &&
		v.y >= lower_.y && v.y <= upper_.y
	    );
	    break;
	}
    }
    return b;
}

//+ RegionImpl(Region::intersects)
Boolean RegionImpl::intersects(Region_in r) {
    Boolean b = false;
    if (defined_) {
	Vertex lower, upper;
	r->bounds(lower, upper);
	b = (
	    upper.x >= lower_.x && lower.x <= upper_.x &&
	    upper.y >= lower_.y && lower.y <= upper_.y &&
	    upper.z >= lower_.z && lower.z <= upper_.z
	);
    }
    return b;
}

//+ RegionImpl(Region::copy)
void RegionImpl::copy(Region_in r) {
    Region::BoundingSpan x, y, z;
    r->span(X_axis, x);
    r->span(Y_axis, y);
    r->span(Z_axis, z);
    defined_ = true;
    lower_.x = x.begin;
    lower_.y = y.begin;
    lower_.z = z.begin;
    upper_.x = x.end;
    upper_.y = y.end;
    upper_.z = z.end;
    xalign_ = x.align;
    yalign_ = y.align;
    zalign_ = z.align;
    notify();
}

//+ RegionImpl(Region::merge_intersect)
void RegionImpl::merge_intersect(Region_in r) {
    if (defined_) {
	Vertex lower, upper;
	r->bounds(lower, upper);
	merge_max(lower_, lower);
	merge_min(upper_, upper);
    } else {
	copy(r);
    }
    notify();
}

//+ RegionImpl(Region::merge_union)
void RegionImpl::merge_union(Region_in r) {
    if (defined_) {
	Vertex lower, upper;
	r->bounds(lower, upper);
	merge_min(lower_, lower);
	merge_max(upper_, upper);
    } else {
	copy(r);
    }
    notify();
}

//+ RegionImpl(Region::subtract)
void RegionImpl::subtract(Region_in r) {
    /* not implemented */
    notify();
}

//+ RegionImpl(Region::apply_transform)
void RegionImpl::apply_transform(Transform_in t) {
    if (defined_) {
	Vertex v[8];
	v[0] = lower_;
	v[1] = upper_;
	v[2].x = v[1].x; v[2].y = v[0].y; v[2].z = v[0].z;
	v[3].x = v[1].x; v[3].y = v[0].y; v[3].z = v[1].z;
	v[4].x = v[1].x; v[4].y = v[1].y; v[4].z = v[0].z;
	v[5].x = v[0].x; v[5].y = v[1].y; v[5].z = v[1].z;
	v[6].x = v[0].x; v[6].y = v[1].y; v[6].z = v[0].z;
	v[7].x = v[1].x; v[7].y = v[1].y; v[7].z = v[0].z;
	for (long i = 0; i < 8; i++) {
	    t->transform_vertex(v[i]);
	}
	lower_ = v[0];
	upper_ = v[0];
	for (long j = 1; j < 8; j++) {
	    merge_min(lower_, v[j]);
	    merge_max(upper_, v[j]);
	}

	Vertex o;
	origin(o);
	t->transform_vertex(o);
	xalign_ = span_align(lower_.x, upper_.x, o.x);
	yalign_ = span_align(lower_.y, upper_.y, o.y);
	zalign_ = span_align(lower_.z, upper_.z, o.z);
	notify();
    }
}

Coord RegionImpl::span_align(Coord lower, Coord upper, Coord origin) {
    Coord s;
    if (Math::equal(lower, upper, float(1e-4))) {
	s = 0.0;
    } else {
	s = (origin - lower) / (upper - lower);
    }
    return s;
}

//+ RegionImpl(Region::bounds)
void RegionImpl::bounds(Vertex& lower, Vertex& upper) {
    if (defined_) {
	lower = lower_;
	upper = upper_;
    }
    else {
	lower.x = lower.y = lower.z = 0;
	upper.x = upper.y = upper.z = 0;
    }
}

//+ RegionImpl(Region::origin)
void RegionImpl::origin(Vertex& v) {
    if (defined_) {
	v.x = span_origin(lower_.x, upper_.x, xalign_);
	v.y = span_origin(lower_.y, upper_.y, yalign_);
	v.z = span_origin(lower_.z, upper_.z, zalign_);
    }
    else {
	v.x = v.y = v.z = 0;
    }
}


Coord RegionImpl::span_origin(Coord lower, Coord upper, Coord align) {
    Coord orig;
    if (Math::equal(lower, upper, float(1e-4))) {
	orig = 0.0;
    } else {
	orig = lower + align * (upper - lower);
    }
    return orig;
}

//+ RegionImpl(Region::span)
void RegionImpl::span(Axis a, Region::BoundingSpan& s) {
    if (defined_) {
	switch (a) {
	case X_axis:
	    s.begin = lower_.x;
	    s.end = upper_.x;
	    s.align = xalign_;
	    break;
	case Y_axis:
	    s.begin = lower_.y;
	    s.end = upper_.y;
	    s.align = yalign_;
	    break;
	case Z_axis:
	    s.begin = lower_.z;
	    s.end = upper_.z;
	    s.align = zalign_;
	    break;
	}
	s.length = s.end - s.begin;
	s.origin = s.begin + s.align * s.length;
    }
    else {
	s.begin = s.end = s.length = 0;
	s.align = 0;
	s.origin = 0;
    }
}

void RegionImpl::notify() { }

void RegionImpl::merge_min(Vertex& v0, const Vertex& v) {
    v0.x = Math::min(v0.x, v.x);
    v0.y = Math::min(v0.y, v.y);
    v0.z = Math::min(v0.z, v.z);
}

void RegionImpl::merge_max(Vertex& v0, const Vertex& v) {
    v0.x = Math::max(v0.x, v.x);
    v0.y = Math::max(v0.y, v.y);
    v0.z = Math::max(v0.z, v.z);
}

implementPtrList(StringList,CharString)

/*
 * Create an attribute.
 */

StyleValueImpl::StyleValueImpl(Fresco* f, CharStringRef name, PathName* path) {
    name_ = new CharStringImpl(name);
    path_ = path;
    uninitialized_ = true;
    value_ = nil;
    any_value_ = nil;
    object_.lock_ = _tmp(f->thread_kit())->lock();
}

/*
 * Dispose of an attribute's state.
 */

StyleValueImpl::~StyleValueImpl() {
    Fresco::unref(name_);
    delete_path(path_);
    Fresco::unref(value_);
    Fresco::unref(any_value_);
}

//+ StyleValueImpl(FrescoObject::=object_.)
Long StyleValueImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag StyleValueImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void StyleValueImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void StyleValueImpl::disconnect() {
    object_.disconnect();
}
void StyleValueImpl::notify_observers() {
    object_.notify_observers();
}
void StyleValueImpl::update() {
    object_.update();
}
//+

//+ StyleValueImpl(StyleValue::name)
CharString_return StyleValueImpl::name() {
    return CharString::_duplicate(name_);
}

//+ StyleValueImpl(StyleValue::uninitialized)
Boolean StyleValueImpl::uninitialized() {
    return uninitialized_;
}

//+ StyleValueImpl(StyleValue::priority=p)
void StyleValueImpl::priority(Long p) {
    priority_ = p;
}

//+ StyleValueImpl(StyleValue::priority?)
Long StyleValueImpl::priority() {
    long p = priority_;
    return p;
}

//+ StyleValueImpl(StyleValue::is_on)
Boolean StyleValueImpl::is_on() {
    Boolean b;
    return read_boolean(b) && b;
}

//+ StyleValueImpl(StyleValue::read_boolean)
Boolean StyleValueImpl::read_boolean(Boolean& b) {
    CharString_var s;
    if (read_string(s._out())) {
	CharString_var s1 = Fresco::string_ref("on");
	CharString_var s2 = Fresco::string_ref("true");
	b = s->case_insensitive_equal(s1) || s->case_insensitive_equal(s2);
	return true;
    }
    return false;
}

//+ StyleValueImpl(StyleValue::write_boolean)
void StyleValueImpl::write_boolean(Boolean b) {
    write_string(_tmp(Fresco::string_ref(b ? "on" : "off")));
}

//+ StyleValueImpl(StyleValue::read_coord)
Boolean StyleValueImpl::read_coord(Coord& c) {
    Boolean b = false;
    CharString_var s;
    if (read_string(s._out())) {
	CharStringBuffer buf(s);
	Coord pts = 1.0;
	char* p = buf.string();
	const char* end = p + buf.length();
	if (p < end && (*p == '-' || *p == '+')) {
	    ++p;
	}
	Boolean dot = false;
	for (; p < end; p++) {
	    if (!dot && *p == '.') {
		dot = true;
	    } else if (!isspace(*p) && !isdigit(*p)) {
		if (strcmp(p, "mm") == 0) {
		    pts = 72.0 / 25.4;
		} else if (strcmp(p, "cm") == 0) {
		    pts = 72.0 / 2.54;
		} else if (strcmp(p, "in") == 0) {
		    pts = 72.0;
		} else if (strcmp(p, "pt") != 0) {
		    return false;
		}
		*p = '\0';
		break;
	    }
	}
	float f = (float)strtod(buf.string(), &p);
	if (p != buf.string()) {
	    c = f * pts;
	    b = true;
	}
    }
    return b;
}

/*
 * Writing coords, longs, and doubles is not currently implemented.
 */

//+ StyleValueImpl(StyleValue::write_coord)
void StyleValueImpl::write_coord(Coord) { }

//+ StyleValueImpl(StyleValue::read_integer)
Boolean StyleValueImpl::read_integer(Long& i) {
    Boolean b = false;
    CharString_var s;
    if (read_string(s._out())) {
	CharStringBuffer buf(s);
	char* ptr;
	Long n = strtol(buf.string(), &ptr, 0);
	if (ptr != buf.string()) {
	    i = n;
	    b = true;
	}
    }
    return b;
}

//+ StyleValueImpl(StyleValue::write_integer)
void StyleValueImpl::write_integer(Long) { }

//+ StyleValueImpl(StyleValue::read_real)
Boolean StyleValueImpl::read_real(Double& d) {
    Boolean b = false;
    CharString_var s;
    if (read_string(s._out())) {
	CharStringBuffer buf(s);
	char* ptr;
	double r = strtod(buf.string(), &ptr);
	if (ptr != buf.string()) {
	    d = r;
	    b = true;
	}
    }
    return b;
}

//+ StyleValueImpl(StyleValue::write_real)
void StyleValueImpl::write_real(Double) { }

//+ StyleValueImpl(StyleValue::read_string)
Boolean StyleValueImpl::read_string(CharString_out& s) {
    Boolean b = false;
    if (!uninitialized_ && value_ != nil) {
	s = CharString::_duplicate(value_);
	b = true;
    }
    return b;
}

//+ StyleValueImpl(StyleValue::write_string)
void StyleValueImpl::write_string(CharString_in s) {
    uninitialized_ = false;
    value_ = parse_value(s);
    modified();
}

//+ StyleValueImpl(StyleValue::read_value)
Boolean StyleValueImpl::read_value(FrescoObject_out& s) {
    Boolean b = false;
    if (any_value_ != nil) {
	s = any_value_;
	b = true;
    }
    return b;
}

//+ StyleValueImpl(StyleValue::write_value)
void StyleValueImpl::write_value(FrescoObject_in s) {
    uninitialized_ = false;
    any_value_ = s;
    modified();
}

//+ StyleValueImpl(StyleValue::lock)
void StyleValueImpl::lock() {
    object_.lock_->acquire();
}

//+ StyleValueImpl(StyleValue::unlock)
void StyleValueImpl::unlock() {
    object_.lock_->release();
}

/*
 * Scan an attribute value, replace \<char> as follows:
 *
 *	\<newline>	nothing
 *	\n		newline
 *	\<backslash>	\
 */

CharStringRef StyleValueImpl::parse_value(CharStringRef v) {
    CharStringRef r;
    CharStringBuffer buf(v);
    if (CharStringImpl::index(buf.string(), buf.length(), '\\') == nil) {
	r = new CharStringImpl(v);
    } else {
	char fixed_dst[200];
	const char* src = buf.string();
	Long n = buf.length();
	const char* src_end = src + n;
	char* dst_start = n < sizeof(fixed_dst) ? fixed_dst : new char[n + 1];
	char* dst = dst_start;
	for (; src < src_end; src++) {
	    if (*src == '\\') {
		++src;
		switch (*src) {
		case '\n':
		    /* eliminate */
		    break;
		case 'n':
		    *dst++ = '\n';
		    break;
		case '\\':
		    *dst++ = *src;
		    break;
		default:
		    *dst++ = '\\';
		    *dst++ = *src;
		    break;
		}
	    } else {
		*dst++ = *src;
	    }
	}
	*dst = '\0';
	r = new CharStringImpl(dst_start, dst - dst_start);
	if (dst_start != fixed_dst) {
	    delete dst_start;
	}
    }
    return r;
}

void StyleValueImpl::delete_path(PathName* list) {
    if (list != nil) {
	for (ListItr(StringList) i(*list); i.more(); i.next()) {
	    CharStringRef s = i.cur();
	    Fresco::unref(s);
	}
	delete list;
    }
}

/*
 * Notify observers assuming that we already hold our lock.
 */

void StyleValueImpl::modified() {
    object_.SharedFrescoObjectImpl::notify_observers();
}

/* class SharedStyleImpl */

declareList(StyleList,SharedStyleImpl::Info)
implementList(StyleList,SharedStyleImpl::Info)

declarePtrList(StyleValueImplList,StyleValueImpl)
implementPtrList(StyleValueImplList,StyleValueImpl)

struct StyleValueTableEntry {
    StyleValueImplList** entries;
    long avail;
    long used;
};

declareTable(StyleValueTable,CharStringRef,StyleValueTableEntry*)
implementTable(StyleValueTable,CharStringRef,StyleValueTableEntry*)

SharedStyleImpl::SharedStyleImpl(Fresco* f) {
    fresco_ = f;
    style_ = nil;
    lock_ = nil;
    name_ = nil;
    aliases_ = nil;
    parent_ = nil;
    links_ = nil;
    unlink_ = nil;
    table_ = nil;
    children_ = nil;
    modified_ = true;
}

SharedStyleImpl::~SharedStyleImpl() {
    Fresco::unref(name_);
    if (is_not_nil(parent_)) {
	parent_->unlink_child(unlink_);
    }
    StyleValueTable* t = table_;
    if (t != nil) {
	for (TableIterator(StyleValueTable) i(*t); i.more(); i.next()) {
	    StyleValueTableEntry* e = i.cur_value();
	    for (long j = 0; j < e->used; j++) {
		StyleValueImplList* vl = e->entries[j];
		if (vl != nil) {
		    for (
			ListItr(StyleValueImplList) a(*vl); a.more(); a.next()
		    ) {
			Fresco::unref(a.cur());
		    }
		    delete vl;
		}
	    }
	    delete e->entries;
	    delete e;
	}
	delete t;
    }
    StyleValueImpl::delete_path(aliases_);
    if (children_ != nil) {
	/*
	 * Set links_ to nil to avoid having unlink_parent call
	 * unlink_child on this and run though the child list.
	 */
	links_ = nil;
	for (ListItr(StyleList) i(*children_); i.more(); i.next()) {
	    SharedStyleImpl::Info& info = i.cur_ref();
	    info.child->unlink_parent();
	}
	delete children_;
    }
}

class StyleCloner : public StyleVisitorImpl {
public:
    StyleCloner(SharedStyleImpl* s);
    virtual ~StyleCloner();

    //+ StyleVisitor::=
    Boolean visit_alias(CharString_in name);
    Boolean visit_attribute(StyleValue_in a);
    Boolean visit_style(Style_in s);
    //+
private:
    SharedStyleImpl* style_impl_;
};

StyleCloner::StyleCloner(SharedStyleImpl* s) { style_impl_ = s; }
StyleCloner::~StyleCloner() { }

//+ StyleCloner(StyleVisitor::visit_alias)
Boolean StyleCloner::visit_alias(CharString_in name) {
    style_impl_->alias(name);
    return true;
}

//+ StyleCloner(StyleVisitor::visit_attribute)
Boolean StyleCloner::visit_attribute(StyleValue_in a) {
    StyleValue_var n = style_impl_->bind(_tmp(a->name()));
    CharString_var s;
    if (a->read_string(s._out())) {
	n->write_string(s);
	n->priority(a->priority());
    }
    return true;
}

//+ StyleCloner(StyleVisitor::visit_style)
Boolean StyleCloner::visit_style(Style_in s) {
    Style_var ns = style_impl_->new_style();
    ns->merge(s);
    ns->link_parent(style_impl_->style_);
    return true;
}

//+ SharedStyleImpl(Style::new_style)
Style_return SharedStyleImpl::new_style() {
    return new StyleImpl(fresco_);
}

//+ SharedStyleImpl(Style::parent_style)
Style_return SharedStyleImpl::parent_style() {
    return Style::_duplicate(parent_);
}

/*
 * Link a style to its parent.  We don't want children to control
 * the lifetime of the parent, so we don't keep a "counted" reference
 * to the parent.
 */

//+ SharedStyleImpl(Style::link_parent)
void SharedStyleImpl::link_parent(Style_in parent) {
    if (is_not_nil(parent_)) {
	parent_->unlink_child(unlink_);
    }
    if (is_not_nil(parent)) {
	unlink_ = parent->link_child(style_);
    }
    parent_ = parent;
}

//+ SharedStyleImpl(Style::unlink_parent)
void SharedStyleImpl::unlink_parent() {
    if (is_not_nil(parent_)) {
	parent_->unlink_child(unlink_);
	parent_ = nil;
	Fresco::unref(style_);
    }
}

//+ SharedStyleImpl(Style::link_child)
Tag SharedStyleImpl::link_child(Style_in child) {
    if (children_ == nil) {
	children_ = new StyleList(5);
    }
    ++links_;
    SharedStyleImpl::Info info;
    info.child = Style::_duplicate(child);
    info.tag = links_;
    children_->append(info);
    return links_;
}

//+ SharedStyleImpl(Style::unlink_child)
void SharedStyleImpl::unlink_child(Tag link_tag) {
    if (children_ != nil && links_ != nil) {
	for (ListUpdater(StyleList) i(*children_); i.more(); i.next()) {
	    SharedStyleImpl::Info& info = i.cur_ref();
	    if (info.tag == link_tag) {
		i.remove_cur();
		break;
	    }
	}
    }
}

//+ SharedStyleImpl(Style::merge)
void SharedStyleImpl::merge(Style_in s) {
    if (name_ == nil) {
	CharString_var name = s->name();
	if (is_not_nil(name)) {
	    name_ = new CharStringImpl(name);
	}
    }
    StyleVisitor_var v = new StyleCloner(this);
    s->visit_aliases(v);
    s->visit_attributes(v);
    s->visit_styles(v);
}

//+ SharedStyleImpl(Style::name=s)
void SharedStyleImpl::name(CharString_in s) {
    name_ = CharString::_duplicate(s->_obj());
}

//+ SharedStyleImpl(Style::name?)
CharStringRef SharedStyleImpl::name() {
    return CharString::_duplicate(name_);
}

//+ SharedStyleImpl(Style::alias)
void SharedStyleImpl::alias(CharString_in s) {
    if (aliases_ == nil) {
	aliases_ = new PathName(5);
    }
    aliases_->prepend(CharString::_duplicate(s));
}

/*
 * Bind a name to an attribute.  If the name is already defined,
 * return the attribute.  Otherwise, create a new attribute.
 */

//+ SharedStyleImpl(Style::bind)
StyleValue_return SharedStyleImpl::bind(CharString_in name) {
    PathName* path = parse_name(name);
    if (path == nil) {
	/* irrelevant attribute: A*B where A doesn't match */
	return nil;
    }

    if (table_ == nil) {
	table_ = new StyleValueTable(50);
    }

    StyleValueTableEntry* e = find_tail_entry(path, true);
    long n = path->count();
    if (e->avail <= n) {
	long new_avail = n + 5;
	StyleValueImplList** new_list = new StyleValueImplList*[new_avail];
	for (long i = 0; i < e->avail; i++) {
	    new_list[i] = e->entries[i];
	}
	for (i = e->avail; i < new_avail; i++) {
	    new_list[i] = nil;
	}
	delete e->entries;
	e->entries = new_list;
	e->avail = new_avail;
    }
    if (e->entries[n] == nil) {
	e->entries[n] = new StyleValueImplList;
    }
    e->used = Math::max(e->used, n + 1);
    StyleValueImplList* list = e->entries[n];
    for (ListItr(StyleValueImplList) i(*list); i.more(); i.next()) {
	StyleValueImpl* a = i.cur();
	if (same_path(a->path(), path)) {
	    StyleValueImpl::delete_path(path);
	    return StyleValue::_duplicate(a);
	}
    }
    StyleValueImpl* a = new StyleValueImpl(fresco_, name, path);
    list->append(a);
    return StyleValue::_duplicate(a);
}

/*
 * Parse a name of the form *A*B*C into the list of names A, B, C.
 * Strip the first name (e.g., A) if it matches the style's name
 * or an alias.
 */

PathName* SharedStyleImpl::parse_name(CharStringRef s) {
    CharStringBuffer buf(s);
    Boolean leading_star = false;
    char* p = buf.string();
    char* end = p + buf.length();
    if (p < end && *p == '*') {
	leading_star = true;
	++p;
    }
    match_priority_ = 0;
    PathName* list = new PathName;
    Boolean first = true;
    for (;;) {
	Long i = find_separator(p, end);
	if (i == -1) {
	    list->append(new CharStringImpl(p, end - p));
	    break;
	}
	CharStringRef name = new CharStringImpl(p, i);
	p += (i + 1);
	if (first) {
	    first = false;
	    Long q = match(name);
	    if (q != 0) {
		match_priority_ = (q == 1) ? 2 : 1;
		Fresco::unref(name);
		continue;
	    } else if (!leading_star) {
		Fresco::unref(name);
		StyleValueImpl::delete_path(list);
		list = nil;
		break;
	    }
	}
	list->append(name);
    }
    return list;
}

/*
 * Return the index of the next separator ("*" or ".") in the string.
 * If no separator is present, return -1.
 */

Long SharedStyleImpl::find_separator(const char* start, const char* end) {
    for (const char* p = start; p < end; p++) {
	if (*p == '*' || *p == '.') {
	    return p - start;
	}
    }
    return -1;
}

/*
 * Check to see if a given name matches the style's name
 * or any of its aliases.
 *
 * Return value:
 *     0 - no match
 *     1 - name match
 *     2 and up - index of alias match plus 2
 */

//+ SharedStyleImpl(Style::match)
Long SharedStyleImpl::match(CharString_in name) {
    long match = 0;
    if (is_not_nil(name_) && name_->equal(name)) {
	match = 1;
    } else if (aliases_ != nil) {
	long possible_match = 2;
	for (ListItr(StringList) i(*aliases_); i.more(); i.next()) {
	    if (name->equal(i.cur())) {
		match = possible_match;
		break;
	    }
	    ++possible_match;
	}
    }
    return match;
}

/*
 * Compare to lists of strings.
 */

Boolean SharedStyleImpl::same_path(PathName* p1, PathName* p2) {
    if (p1->count() != p2->count()) {
	return false;
    }
    ListItr(StringList) i1(*p1);
    ListItr(StringList) i2(*p2);
    for (; i1.more(); i1.next(), i2.next()) {
	CharStringRef c1 = i1.cur();
	CharStringRef c2 = i2.cur();
	if (!c1->equal(c2)) {
	    return false;
	}
    }
    return true;
}

void SharedStyleImpl::load_file(const char* filename, Long priority) {
    InputFile* f = InputFile::open(filename);
    if (f == nil) {
	return;
    }
    const char* start;
    Long len = f->read(start);
    if (len > 0) {
	load_list(start, len, priority);
    }
    f->close();
    delete f;
}

void SharedStyleImpl::load_list(const char* str, Long length, Long priority) {
    const char* p = str;
    const char* q = p + length;
    const char* start = p;
    for (; p < q; p++) {
	if (*p == '\n') {
	    if (p > start && *(p-1) != '\\') {
		load_property(start, p - start, priority);
		start = p + 1;
	    }
	}
    }
}

void SharedStyleImpl::load_property(
    const char* prop, Long length, Long priority
) {
    const char* p = prop;
    Long n = length;
    strip(p, n);
    if (n == 0 || *p == '!') {
	return;
    }
    const char* colon = CharStringImpl::index(p, n, ':');
    if (colon == nil) {
	missing_colon(p, n);
    } else {
	const char* name = p;
	Long name_length = colon - p;
	const char* value = colon + 1;
	Long value_length = n - name_length - 1;
	strip(name, name_length);
	strip(value, value_length);
	if (name_length == 0) {
	    bad_property_name(p, length);
	} else if (value_length == 0) {
	    bad_property_value(p, length);
	} else {
	    StyleValue_var a = bind(
		CharString_var(new CharStringImpl(name, name_length))
	    );
	    if (is_not_nil(a)) {
		long p = priority + match_priority_;
		if (a->uninitialized() || p >= a->priority()) {
		    a->write_string(
			CharString_var(new CharStringImpl(value, value_length))
		    );
		    a->priority(p);
		}
	    }
	}
    }
}

void SharedStyleImpl::strip(const char*& s, Long& length) {
    const char* p = s;
    for (; isspace(*p); p++);
    const char* q = s + length - 1;
    for (; q >= s && isspace(*q); q--);
    s = p;
    length = q - p + 1;
}

/*
 * Errors are nops for now.
 */

void SharedStyleImpl::missing_colon(const char*, Long) { }
void SharedStyleImpl::bad_property_name(const char*, Long) { }
void SharedStyleImpl::bad_property_value(const char*, Long) { }

/*
 * Test if an attribute is defined and readable as a boolean set to true.
 */

//+ SharedStyleImpl(Style::is_on)
Boolean SharedStyleImpl::is_on(CharString_in name) {
    StyleValue_var a = resolve(name);
    Boolean b;
    return is_not_nil(a) && a->read_boolean(b) && b;
}

/*
 * Find the attribute bound to a given name, if any.
 */

//+ SharedStyleImpl(Style::resolve)
StyleValue_return SharedStyleImpl::resolve(CharString_in name) {
    StyleValueTableEntry* e = find_entry(name, false);
    if (e != nil) {
	StyleValueImplList* list = e->entries[0];
	if (list != nil && list->count() != 0) {
	    return StyleValue::_duplicate(list->item(0));
	}
    }

    Style_var s = parent_style();
    for (; is_not_nil(s); s = s->parent_style()) {
	StyleValue_return a = s->resolve_wildcard(name, style_);
	if (is_not_nil(a)) {
	    return a;
	}
    }
    return nil;
}

/*
 * Try to match the given name against wildcard entries.
 * The start style's name and aliases are possible wildcard prefixes.
 */

//+ SharedStyleImpl(Style::resolve_wildcard)
StyleValue_return SharedStyleImpl::resolve_wildcard(CharString_in name, Style_in start) {
    StyleValueTableEntry* e = find_entry(name, false);
    if (e != nil) {
	if (e->used > 0) {
	    StyleValueRef a = wildcard_match(e, start);
	    if (is_not_nil(a)) {
		return a;
	    }
	}
	StyleValueImplList* list = e->entries[0];
	if (list != nil) {
	    return StyleValue::_duplicate(list->item(0));
	}
    }
    return nil;
}

/*
 * Short-hand for find an entry using the tail of a given path.
 * Since the tail shouldn't be stored with the path, it is removed
 * after used for the lookup.
 */

StyleValueTableEntry* SharedStyleImpl::find_tail_entry(
    PathName* path, Boolean force
) {
    long t = path->count() - 1;
    CharStringRef str = path->item(t);
    StyleValueTableEntry* e = find_entry(str, force);
    Fresco::unref(str);
    path->remove(t);
    return e;
}

/*
 * Lookup a given name in the attribute table.
 * Since there can be multiple wildcard entries with the same name,
 * we return the table entry if the name is found or nil if not.
 *
 * If the insert parameter is true, then create a new entry if the name
 * is not defined.
 */

StyleValueTableEntry* SharedStyleImpl::find_entry(
    CharStringRef name, Boolean insert
) {
    StyleValueTableEntry* e = nil;
    if (table_ == nil && insert) {
	table_ = new StyleValueTable(50);
    }
    if (table_ != nil && !table_->find(e, name) && insert) {
	e = new StyleValueTableEntry;
	e->entries = new StyleValueImplList*[3];
	e->avail = 3;
	e->used = 0;
	for (long i = 0; i < e->avail; i++) {
	    e->entries[i] = nil;
	}
	table_->insert(CharString::_duplicate(name), e);
    }
    return e;
}

/*
 * Check if the given table entry contains a match for the
 * list of styles starting from the given one.
 */

class StyleMatcher : public StyleVisitorImpl {
public:
    StyleMatcher();
    virtual ~StyleMatcher();

    void setup(
	SharedStyleImpl*, StyleValueTableEntry*,
	StyleRef start, StyleRef cur
    );

    Boolean visit_alias(CharString_in name); //+ StyleVisitor::visit_alias

    SharedStyleImpl* style_;
    StyleValueTableEntry* entry_;
    StyleRef start_;
    StyleRef cur_;
    StyleValueRef match_;
};

StyleMatcher::StyleMatcher() {
    style_ = nil;
    start_ = nil;
    cur_ = nil;
    match_ = nil;
}

StyleMatcher::~StyleMatcher() { }

void StyleMatcher::setup(
    SharedStyleImpl* style, StyleValueTableEntry* e,
    StyleRef start, StyleRef cur
) {
    style_ = style;
    entry_ = e;
    start_ = start;
    cur_ = cur;
    match_ = nil;
}

//+ StyleMatcher(StyleVisitor::visit_alias)
Boolean StyleMatcher::visit_alias(CharString_in name) {
    StyleValueRef a = style_->wildcard_match_name(name, entry_, cur_);
    if (is_not_nil(a)) {
	match_ = a;
	return false;
    }
    return true;
}

StyleValueRef SharedStyleImpl::wildcard_match(
    StyleValueTableEntry* e, StyleRef start
) {
    StyleValue_return found = nil;
    StyleMatcher matcher;
    Style_var s = Style::_duplicate(start);
    while (is_not_nil(s)) {
	CharString_var name = s->name();
	if (is_not_nil(name)) {
	    StyleValue_return a = wildcard_match_name(name, e, s);
	    if (is_not_nil(a)) {
		found = a;
		break;
	    }
	}
	matcher.setup(this, e, start, s->_obj());
	s->visit_aliases(&matcher);
	if (is_not_nil(matcher.match_)) {
	    found = matcher.match_;
	    break;
	}
	s = s->parent_style();
    }
    return found;
}

StyleValueRef SharedStyleImpl::wildcard_match_name(
    CharStringRef name, StyleValueTableEntry* e, StyleRef cur
) {
    for (long i = e->used - 1; i >= 1; i--) {
	StyleValueImplList* list = e->entries[i];
	if (list != nil) {
	    long best_match = 0;
	    StyleValueRef matched = nil;
	    for (ListItr(StyleValueImplList) a(*list); a.more(); a.next()) {
		StyleValueImpl* attr = a.cur();
		PathName* path = attr->path();
		if (name->equal(path->item(i - 1))) {
		    if (i == 1) {
			return StyleValue::_duplicate(attr);
		    } else {
			long new_match = finish_wildcard_match(
			    cur, path, i - 2
			);
			if (new_match > best_match) {
			    matched = attr;
			    best_match = new_match;
			}
		    }
		}
	    }
	    if (matched != nil) {
		return StyleValue::_duplicate(matched);
	    }
	}
    }
    return nil;
}

long SharedStyleImpl::finish_wildcard_match(
    StyleRef cur, PathName* path, long p_index
) {
    long matched = 0;
    Style_var s = Style::_duplicate(cur);
    long p_cur = p_index;
    while (p_cur >= 0 && is_not_nil(s)) {
	long m = s->match(path->item(p_cur));
	if (m != 0) {
	    --p_cur;
	    matched += m;
	}
	s = s->parent_style();
    }
    return matched;
}

//+ SharedStyleImpl(Style::unbind)
void SharedStyleImpl::unbind(CharString_in name) {
    if (table_ == nil) {
	return;
    }
    PathName* path = parse_name(name);
    if (path == nil) {
	return;
    }
    StyleValueTableEntry* e = find_tail_entry(path, false);
    long p = path->count();
    if (e != nil && e->used > p) {
	StyleValueImplList* vl = e->entries[p];
	if (vl != nil) {
	    for (ListUpdater(StyleValueImplList) i(*vl); i.more(); i.next()) {
		StyleValueImpl* a = i.cur();
		if (same_path(a->path(), path)) {
		    Fresco::unref(a);
		    i.remove_cur();
		    break;
		}
	    }
	    if (vl->count() == 0) {
		delete vl;
		e->entries[p] = nil;
	    }
	}
    }
    StyleValueImpl::delete_path(path);
}

//+ SharedStyleImpl(Style::visit_aliases)
void SharedStyleImpl::visit_aliases(StyleVisitor_in v) {
    if (aliases_ != nil) {
	long n = aliases_->count();
	for (long i = n - 1; i >= 0; i--) {
	    if (!v->visit_alias(aliases_->item(i))) {
		break;
	    }
	}
    }
}

//+ SharedStyleImpl(Style::visit_attributes)
void SharedStyleImpl::visit_attributes(StyleVisitor_in v) {
    if (table_ != nil) {
	for (TableIterator(StyleValueTable) i(*table_); i.more(); i.next()) {
	    StyleValueTableEntry* e = i.cur_value();
	    for (long j = 0; j < e->used; j++) {
		StyleValueImplList* vl = e->entries[j];
		if (vl != nil) {
		    for (
			ListItr(StyleValueImplList) a(*vl); a.more(); a.next()
		    ) {
			if (!v->visit_attribute(a.cur())) {
			    return;
			}
		    }
		}
	    }
	}
    }
}

//+ SharedStyleImpl(Style::visit_styles)
void SharedStyleImpl::visit_styles(StyleVisitor_in v) {
    if (children_ != nil) {
	for (ListItr(StyleList) i(*children_); i.more(); i.next()) {
	    if (!v->visit_style(i.cur_ref().child)) {
		break;
	    }
	}
    }
}

//+ SharedStyleImpl(Style::lock)
void SharedStyleImpl::lock() {
    lock_->acquire();
}

//+ SharedStyleImpl(Style::unlock)
void SharedStyleImpl::unlock() {
    lock_->release();
}

StyleImpl::StyleImpl(Fresco* f) : impl_(f) {
    impl_.style_ = this;
    object_.lock_ = _tmp(f->thread_kit())->lock();
    impl_.lock_ = object_.lock_;
}

StyleImpl::~StyleImpl() { }

//+ StyleImpl(FrescoObject::=object_.)
Long StyleImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag StyleImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void StyleImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void StyleImpl::disconnect() {
    object_.disconnect();
}
void StyleImpl::notify_observers() {
    object_.notify_observers();
}
void StyleImpl::update() {
    object_.update();
}
//+

//+ StyleImpl(Style::=impl_.)
Style_return StyleImpl::new_style() {
    return impl_.new_style();
}
Style_return StyleImpl::parent_style() {
    return impl_.parent_style();
}
void StyleImpl::link_parent(Style_in parent) {
    impl_.link_parent(parent);
}
void StyleImpl::unlink_parent() {
    impl_.unlink_parent();
}
Tag StyleImpl::link_child(Style_in child) {
    return impl_.link_child(child);
}
void StyleImpl::unlink_child(Tag link_tag) {
    impl_.unlink_child(link_tag);
}
void StyleImpl::merge(Style_in s) {
    impl_.merge(s);
}
CharString_return StyleImpl::name() {
    return impl_.name();
}
void StyleImpl::name(CharString_in _p) {
    impl_.name(_p);
}
void StyleImpl::alias(CharString_in s) {
    impl_.alias(s);
}
Boolean StyleImpl::is_on(CharString_in name) {
    return impl_.is_on(name);
}
StyleValue_return StyleImpl::bind(CharString_in name) {
    return impl_.bind(name);
}
void StyleImpl::unbind(CharString_in name) {
    impl_.unbind(name);
}
StyleValue_return StyleImpl::resolve(CharString_in name) {
    return impl_.resolve(name);
}
StyleValue_return StyleImpl::resolve_wildcard(CharString_in name, Style_in start) {
    return impl_.resolve_wildcard(name, start);
}
Long StyleImpl::match(CharString_in name) {
    return impl_.match(name);
}
void StyleImpl::visit_aliases(StyleVisitor_in v) {
    impl_.visit_aliases(v);
}
void StyleImpl::visit_attributes(StyleVisitor_in v) {
    impl_.visit_attributes(v);
}
void StyleImpl::visit_styles(StyleVisitor_in v) {
    impl_.visit_styles(v);
}
void StyleImpl::lock() {
    impl_.lock();
}
void StyleImpl::unlock() {
    impl_.unlock();
}
//+

StyleVisitorImpl::StyleVisitorImpl() { }
StyleVisitorImpl::~StyleVisitorImpl() { }

//+ StyleVisitorImpl(FrescoObject::=object_.)
Long StyleVisitorImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag StyleVisitorImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void StyleVisitorImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void StyleVisitorImpl::disconnect() {
    object_.disconnect();
}
void StyleVisitorImpl::notify_observers() {
    object_.notify_observers();
}
void StyleVisitorImpl::update() {
    object_.update();
}
//+

Boolean StyleVisitorImpl::visit_alias(CharString_in) { return false; }
Boolean StyleVisitorImpl::visit_attribute(StyleValue_in) { return false; }
Boolean StyleVisitorImpl::visit_style(Style_in) { return false; }

static const double radians_per_degree = M_PI / 180;
static const float tolerance = 1e-4;

TransformImpl::TransformImpl() { init(); }

TransformImpl::TransformImpl(Transform::Matrix m) {
    load_matrix(m);
    identity_ = false;
    translate_only_ = false;
    xy_ = true;
    modified_ = true;
}

TransformImpl::~TransformImpl() { }

//+ TransformImpl(FrescoObject::=object_.)
Long TransformImpl::ref__(Long references) {
    return object_.ref__(references);
}
Tag TransformImpl::attach(FrescoObject_in observer) {
    return object_.attach(observer);
}
void TransformImpl::detach(Tag attach_tag) {
    object_.detach(attach_tag);
}
void TransformImpl::disconnect() {
    object_.disconnect();
}
void TransformImpl::notify_observers() {
    object_.notify_observers();
}
void TransformImpl::update() {
    object_.update();
}
//+

void TransformImpl::init() {
    mat_[0][0] = mat_[1][1] = 1;
    mat_[0][1] = mat_[1][0] = 0;
    mat_[0][2] = mat_[1][2] = 0;
    mat_[0][3] = mat_[1][3] = 0;
    mat_[2][0] = mat_[2][1] = 0;
    mat_[2][2] = mat_[2][3] = 0;
    mat_[3][0] = mat_[3][1] = 0;
    mat_[3][2] = mat_[3][3] = 0;
    identity_ = true;
    translate_only_ = true;
    xy_ = true;
    modified_ = false;
}

void TransformImpl::modified() {
    modified_ = true;
    notify_observers();
}

void TransformImpl::recompute() {
    Coord m00 = mat_[0][0];
    Coord m01 = mat_[0][1];
    Coord m10 = mat_[1][0];
    Coord m11 = mat_[1][1];
    Coord m20 = mat_[2][0];
    Coord m21 = mat_[2][1];
    Coord one_plus = 1 + tolerance, one_minus = 1 - tolerance;
    Coord zero_plus = 0 + tolerance, zero_minus = 0 - tolerance;
    identity_ = (
	m00 > one_minus && m00 < one_plus &&
	m11 > one_minus && m11 < one_plus &&
	m01 > zero_minus && m01 < zero_plus &&
	m10 > zero_minus && m10 < zero_plus &&
	m20 > zero_minus && m20 < zero_plus &&
	m21 > zero_minus && m21 < zero_plus
    );
    modified_ = false;
}

Coord TransformImpl::det() {
    return mat_[0][0] * mat_[1][1] - mat_[0][1] * mat_[1][0];
}

//+ TransformImpl(Transform::load)
void TransformImpl::load(Transform_in t) {
    Transform::Matrix m;
    t->store_matrix(m);
    load_matrix(m);
}

//+ TransformImpl(Transform::load_matrix)
void TransformImpl::load_matrix(Matrix m) {
    mat_[0][0] = m[0][0]; mat_[0][1] = m[0][1];
    mat_[0][2] = m[0][2]; mat_[0][3] = m[0][3];
    mat_[1][0] = m[1][0]; mat_[1][1] = m[1][1];
    mat_[1][2] = m[1][2]; mat_[1][3] = m[1][3];
    mat_[2][0] = m[2][0]; mat_[2][1] = m[2][1];
    mat_[2][2] = m[2][2]; mat_[2][3] = m[2][3];
    mat_[3][0] = m[3][0]; mat_[3][1] = m[3][1];
    mat_[3][2] = m[3][2]; mat_[3][3] = m[3][3];
    modified();
}

//+ TransformImpl(Transform::load_identity)
void TransformImpl::load_identity() { init(); }

//+ TransformImpl(Transform::store_matrix)
void TransformImpl::store_matrix(Matrix m) {
    m[0][0] = mat_[0][0]; m[0][1] = mat_[0][1];
    m[0][2] = mat_[0][2]; m[0][3] = mat_[0][3];
    m[1][0] = mat_[1][0]; m[1][1] = mat_[1][1];
    m[1][2] = mat_[1][2]; m[1][3] = mat_[1][3];
    m[2][0] = mat_[2][0]; m[2][1] = mat_[2][1];
    m[2][2] = mat_[2][2]; m[2][3] = mat_[2][3];
    m[3][0] = mat_[3][0]; m[3][1] = mat_[3][1];
    m[3][2] = mat_[3][2]; m[3][3] = mat_[3][3];
}

//+ TransformImpl(Transform::equal)
Boolean TransformImpl::equal(Transform_in t) {
    if (modified_) {
	recompute();
    }
    if (identity_) {
	return t->identity();
    }
    if (t->identity()) {
	return false;
    }
    Transform::Matrix m;
    t->store_matrix(m);
    Coord m00 = mat_[0][0];
    Coord m01 = mat_[0][1];
    Coord m10 = mat_[1][0];
    Coord m11 = mat_[1][1];
    Coord m20 = mat_[2][0];
    Coord m21 = mat_[2][1];
    return (
	xy_ &&
	m00 < m[0][0] + tolerance && m00 > m[0][0] - tolerance &&
	m01 < m[0][1] + tolerance && m01 > m[0][1] - tolerance &&
	m10 < m[1][0] + tolerance && m10 > m[1][0] - tolerance &&
	m11 < m[1][1] + tolerance && m11 > m[1][1] - tolerance &&
	m20 < m[2][0] + tolerance && m20 > m[2][0] - tolerance &&
	m21 < m[2][1] + tolerance && m21 > m[2][1] - tolerance
    );
}

//+ TransformImpl(Transform::identity)
Boolean TransformImpl::identity() {
    if (modified_) {
	recompute();
    }
    return identity_;
}

//+ TransformImpl(Transform::det_is_zero)
Boolean TransformImpl::det_is_zero() {
    Coord d = det();
    return d < tolerance && d > -tolerance;
}

//+ TransformImpl(Transform::scale)
void TransformImpl::scale(const Vertex& v) {
    mat_[0][0] *= v.x;
    mat_[0][1] *= v.y;
    mat_[1][0] *= v.x;
    mat_[1][1] *= v.y;
    mat_[2][0] *= v.x;
    mat_[2][1] *= v.y;
    translate_only_ = false;
    modified();
}

//+ TransformImpl(Transform::rotate)
void TransformImpl::rotate(Float angle, Axis a) {
    if (a != Z_axis) {
	return;
    }
    Transform::Matrix m;
    Coord r_angle = angle * radians_per_degree;
    Coord tmp1 = cos(r_angle);
    Coord tmp2 = sin(r_angle);
    
    m[0][0] = tmp1 * mat_[0][0];
    m[0][1] = tmp2 * mat_[0][1];
    m[1][0] = tmp1 * mat_[1][0];
    m[1][1] = tmp2 * mat_[1][1];
    m[2][0] = tmp1 * mat_[2][0];
    m[2][1] = tmp2 * mat_[2][1];

    mat_[0][1] = mat_[0][0] * tmp2 + mat_[0][1] * tmp1;
    mat_[1][1] = mat_[1][0] * tmp2 + mat_[1][1] * tmp1;
    mat_[2][1] = mat_[2][0] * tmp2 + mat_[2][1] * tmp1;
    mat_[0][0] = m[0][0] - m[0][1];
    mat_[1][0] = m[1][0] - m[1][1];
    mat_[2][0] = m[2][0] - m[2][1];
    translate_only_ = false;
    modified();
}

//+ TransformImpl(Transform::translate)
void TransformImpl::translate(const Vertex& v) {
    mat_[2][0] += v.x;
    mat_[2][1] += v.y;
    modified();
}

//+ TransformImpl(Transform::premultiply)
void TransformImpl::premultiply(Transform_in t) {
    if (!t->identity()) {
	Transform::Matrix m;
	t->store_matrix(m);

	Coord tmp1 = mat_[0][0];
	Coord tmp2 = mat_[1][0];
	mat_[0][0]  = m[0][0] * tmp1 + m[0][1] * tmp2;
	mat_[1][0]  = m[1][0] * tmp1 + m[1][1] * tmp2;
	mat_[2][0] += m[2][0] * tmp1 + m[2][1] * tmp2;
     
	tmp1 = mat_[0][1];
	tmp2 = mat_[1][1];
     
	mat_[0][1]  = m[0][0] * tmp1 + m[0][1] * tmp2;
	mat_[1][1]  = m[1][0] * tmp1 + m[1][1] * tmp2;
	mat_[2][1] += m[2][0] * tmp1 + m[2][1] * tmp2;
	modified();
    }
}    

//+ TransformImpl(Transform::postmultiply)
void TransformImpl::postmultiply(Transform_in t) {
    Transform::Matrix m;
    t->store_matrix(m);

    Coord tmp = mat_[0][0] * m[0][1] + mat_[0][1] * m[1][1];
    mat_[0][0] = mat_[0][0] * m[0][0] + mat_[0][1] * m[1][0];
    mat_[0][1] = tmp;

    tmp = mat_[1][0] * m[0][1] + mat_[1][1] * m[1][1];
    mat_[1][0] = mat_[1][0] * m[0][0] + mat_[1][1] * m[1][0];
    mat_[1][1] = tmp;

    tmp = mat_[2][0] * m[0][1] + mat_[2][1] * m[1][1];
    mat_[2][0] = mat_[2][0] * m[0][0] + mat_[2][1] * m[1][0];
    mat_[2][1] = tmp;

    mat_[2][0] += m[2][0];
    mat_[2][1] += m[2][1];
    modified();
}

//+ TransformImpl(Transform::invert)
void TransformImpl::invert() {
    Coord d = det();
    if (Math::equal(d, float(0), tolerance)) {
	return;
    }
    Coord t00 = mat_[0][0];
    Coord t20 = mat_[2][0];

    mat_[2][0] = (mat_[1][0] * mat_[2][1] - mat_[1][1] * mat_[2][0]) / d;
    mat_[2][1] = (mat_[0][1] * t20 - mat_[0][0] * mat_[2][1]) / d;
    mat_[0][0] = mat_[1][1]/d;
    mat_[1][1] = t00/d;
    mat_[1][0] = -mat_[1][0]/d;
    mat_[0][1] = -mat_[0][1]/d;
    modified();
}

//+ TransformImpl(Transform::transform_vertex)
void TransformImpl::transform_vertex(Vertex& v) {
    Coord tx = v.x;
    v.x = tx * mat_[0][0] + v.y * mat_[1][0] + mat_[2][0];
    v.y = tx * mat_[0][1] + v.y * mat_[1][1] + mat_[2][1];
}

//+ TransformImpl(Transform::inverse_transform_vertex)
void TransformImpl::inverse_transform_vertex(Vertex& v) {
    Coord d = det();
    Coord a = (v.x - mat_[2][0]) / d;
    Coord b = (v.y - mat_[2][1]) / d;
    v.x = a * mat_[1][1] - b * mat_[1][0];
    v.y = b * mat_[0][0] - a * mat_[0][1];
}
