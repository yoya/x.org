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

#ifndef Fresco_lib_style_h
#define Fresco_lib_style_h

#include <X11/Fresco/Impls/fresco-impl.h>
#include <X11/Fresco/OS/list.h>
#include <X11/Fresco/OS/thread.h>

class StyleValueImplList;
class StyleValueTable;
class StyleValueTableEntry;
class Fresco;
class StyleList;

declarePtrList(StringList,CharString)

typedef StringList PathName;

class StyleValueImpl : public StyleValue {
public:
    StyleValueImpl(Fresco*, CharStringRef name, PathName* path);
    ~StyleValueImpl();

    //+ StyleValue::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* StyleValue */
    CharString_return name();
    Boolean uninitialized();
    Long priority();
    void priority(Long _p);
    Boolean is_on();
    Boolean read_boolean(Boolean& b);
    void write_boolean(Boolean b);
    Boolean read_coord(Coord& c);
    void write_coord(Coord c);
    Boolean read_integer(Long& i);
    void write_integer(Long i);
    Boolean read_real(Double& d);
    void write_real(Double d);
    Boolean read_string(CharString_out& s);
    void write_string(CharString_in s);
    Boolean read_value(FrescoObject_out& s);
    void write_value(FrescoObject_in s);
    void lock();
    void unlock();
    //+

    PathName* path() { return path_; }
    CharStringRef parse_value(CharStringRef);

    static void delete_path(PathName*);
protected:
    LockedFrescoObjectImpl object_;
    CharStringRef name_;
    PathName* path_;
    Boolean uninitialized_;
    CharStringRef value_;
    FrescoObjectRef any_value_;
    long priority_;

    void modified();
};

/*
 * SharedStyleImpl provides an implementation of the Style operations
 * suitable for using with Style implementations (e.g., Viewer).
 */

class SharedStyleImpl {
public:
    SharedStyleImpl(Fresco*);
    ~SharedStyleImpl();

    //+ Style::=
    Style_return new_style();
    Style_return parent_style();
    void link_parent(Style_in parent);
    void unlink_parent();
    Tag link_child(Style_in child);
    void unlink_child(Tag link_tag);
    void merge(Style_in s);
    CharString_return name();
    void name(CharString_in _p);
    void alias(CharString_in s);
    Boolean is_on(CharString_in name);
    StyleValue_return bind(CharString_in name);
    void unbind(CharString_in name);
    StyleValue_return resolve(CharString_in name);
    StyleValue_return resolve_wildcard(CharString_in name, Style_in start);
    Long match(CharString_in name);
    void visit_aliases(StyleVisitor_in v);
    void visit_attributes(StyleVisitor_in v);
    void visit_styles(StyleVisitor_in v);
    void lock();
    void unlock();
    //+

    struct Info {
	StyleRef child;
	Tag tag;
    };

    Fresco* fresco_;
    StyleRef style_;
    LockObj_var lock_;

    void load_file(const char* filename, Long priority);
    void load_list(const char* str, Long length, Long priority);
    void load_property(const char* property, Long length, Long priority);

    StyleValueRef wildcard_match(StyleValueTableEntry* e, StyleRef start);
    StyleValueRef wildcard_match_name(
	CharStringRef name, StyleValueTableEntry*, StyleRef cur
    );
protected:
    CharStringRef name_;
    PathName* aliases_;
    StyleRef parent_;
    Tag links_;
    Tag unlink_;
    StyleValueTable* table_;
    StyleList* children_;
    Boolean modified_;
    long match_priority_;

    PathName* parse_name(CharStringRef name);
    Long find_separator(const char* start, const char* end);
    Boolean same_path(PathName* list1, PathName* list2);

    void strip(const char*& prop, Long& length);
    const char* index(const char* s, Long length, char c);
    void missing_colon(const char* s, Long length);
    void bad_property_name(const char* s, Long length);
    void bad_property_value(const char* s, Long length);

    StyleValueTableEntry* find_tail_entry(PathName*, Boolean force);
    StyleValueTableEntry* find_entry(CharStringRef, Boolean force);
    long finish_wildcard_match(StyleRef, PathName*, long p_index);
};

/*
 * StyleImpl is an implementation of Style that can be constructed directly.
 */

class StyleImpl : public Style {
public:
    StyleImpl(Fresco*);
    ~StyleImpl();

    //+ Style::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Style */
    Style_return new_style();
    Style_return parent_style();
    void link_parent(Style_in parent);
    void unlink_parent();
    Tag link_child(Style_in child);
    void unlink_child(Tag link_tag);
    void merge(Style_in s);
    CharString_return name();
    void name(CharString_in _p);
    void alias(CharString_in s);
    Boolean is_on(CharString_in name);
    StyleValue_return bind(CharString_in name);
    void unbind(CharString_in name);
    StyleValue_return resolve(CharString_in name);
    StyleValue_return resolve_wildcard(CharString_in name, Style_in start);
    Long match(CharString_in name);
    void visit_aliases(StyleVisitor_in v);
    void visit_attributes(StyleVisitor_in v);
    void visit_styles(StyleVisitor_in v);
    void lock();
    void unlock();
    //+
protected:
    LockedFrescoObjectImpl object_;
public:
    SharedStyleImpl impl_;
};

/*
 * Default StyleVisitor implementation cancels iteration immediately.
 *
 * Other StyleVisitor implementations can derive from StyleVisitorImpl
 * and just redefine the desired iteration method or methods.
 */

class StyleVisitorImpl : public StyleVisitor {
public:
    StyleVisitorImpl();
    ~StyleVisitorImpl();

    //+ StyleVisitor::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* StyleVisitor */
    Boolean visit_alias(CharString_in name);
    Boolean visit_attribute(StyleValue_in a);
    Boolean visit_style(Style_in s);
    //+
protected:
    SharedFrescoObjectImpl object_;
};

#endif
