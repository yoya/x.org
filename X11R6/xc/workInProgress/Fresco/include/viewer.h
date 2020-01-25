/* DO NOT EDIT -- Automatically generated from Interfaces/viewer.idl */

#ifndef Interfaces_viewer_h
#define Interfaces_viewer_h

#include <X11/Fresco/_enter.h>
#include <X11/Fresco/glyph.h>

class Focus;
typedef Focus* FocusRef;
typedef FocusRef Focus_in;
typedef FocusRef Focus_out, Focus_inout;
typedef FocusRef Focus_return;
class Focus_var;

class Event;
typedef Event* EventRef;
typedef EventRef Event_in;
typedef EventRef Event_out, Event_inout;
typedef EventRef Event_return;
class Event_var;

extern BaseObjectRef _XfEventStub_create(Exchange*);

class Event : public FrescoObject {
protected:
    Event();
    virtual ~Event();
public:
    EventRef _obj() { return this; };
    static EventRef  _return_ref(Event_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static EventRef _narrow(BaseObjectRef);
    static EventRef _duplicate(EventRef obj) {
        return (EventRef)_BaseObject__duplicate(obj, &_XfEventStub_create);
    }
    typedef Long ButtonIndex;
    typedef ULong KeySym;
    class KeyChord {
    public:
        long _maximum, _length; KeySym* _buffer;

        KeyChord() { _maximum = _length = 0; _buffer = 0; }
        KeyChord(long m, long n, KeySym* e) {
            _maximum = m; _length = n; _buffer = e;
        }
        KeyChord(const KeyChord& _s) { _buffer = 0; *this = _s; }
        KeyChord& operator =(const KeyChord&);
        ~KeyChord() { delete [] _buffer; }
    };
    typedef ULong TimeStamp;
    enum Modifier {
        control, shift, capslock, meta
    };
    enum TypeId {
        motion, enter, leave, down, up, key_press, key_release, focus_in, 
        focus_out, other
    };
    virtual Event::TypeId type();
    virtual Event::TimeStamp time();
    virtual Boolean double_click(Event::TimeStamp previous);
    virtual Boolean positional();
    virtual Coord pointer_x();
    virtual Coord pointer_y();
    virtual Event::ButtonIndex pointer_button();
    virtual Boolean button_is_down(Event::ButtonIndex b);
    virtual Boolean modifier_is_down(Event::Modifier m);
    virtual Event::KeySym key();
    virtual CharCode character();
    virtual void unread();
};

class Event_var {
protected:
    EventRef _obj_;
public:
    Event_var() { _obj_ = 0; };
    Event_var(EventRef p) { _obj_ = p; }
    Event_var(const Event_var& r) {
        _obj_ = Event::_duplicate(r._obj_);
    }
    ~Event_var() { _BaseObject__release(_obj_); }

    Event_var& operator =(EventRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Event::_duplicate(p);
        return *this;
    }
    Event_var& operator =(const Event_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Event::_duplicate(r._obj_);
        return *this;
    }

    EventRef _obj() const { return _obj_; }
    EventRef& _out() { return _obj_; }
    operator EventRef() const { return _obj_; }
    EventRef operator ->() const { return _obj_; }
};

inline Event_var _tmp(EventRef p) { return Event_var(p); }

class EventStub : public Event {
public:
    EventStub(Exchange*);
    ~EventStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class Viewer;
typedef Viewer* ViewerRef;
typedef ViewerRef Viewer_in;
typedef ViewerRef Viewer_out, Viewer_inout;
typedef ViewerRef Viewer_return;
class Viewer_var;

extern BaseObjectRef _XfViewerStub_create(Exchange*);

class Viewer : public Glyph {
protected:
    Viewer();
    virtual ~Viewer();
public:
    ViewerRef _obj() { return this; };
    static ViewerRef  _return_ref(Viewer_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static ViewerRef _narrow(BaseObjectRef);
    static ViewerRef _duplicate(ViewerRef obj) {
        return (ViewerRef)_BaseObject__duplicate(obj, &_XfViewerStub_create);
    }
    virtual Viewer_return parent_viewer();
    virtual Viewer_return next_viewer();
    virtual Viewer_return prev_viewer();
    virtual Viewer_return first_viewer();
    virtual Viewer_return last_viewer();
    virtual void append_viewer(Viewer_in v);
    virtual void prepend_viewer(Viewer_in v);
    virtual void insert_viewer(Viewer_in v);
    virtual void replace_viewer(Viewer_in v);
    virtual void remove_viewer();
    virtual void set_viewer_links(Viewer_in parent, Viewer_in prev, Viewer_in next);
    virtual void set_first_viewer(Viewer_in v);
    virtual void set_last_viewer(Viewer_in v);
    virtual Focus_return request_focus(Viewer_in requestor, Boolean temporary);
    virtual Boolean receive_focus(Focus_in f, Boolean primary);
    virtual void lose_focus(Boolean temporary);
    virtual Boolean first_focus();
    virtual Boolean last_focus();
    virtual Boolean next_focus();
    virtual Boolean prev_focus();
    virtual Boolean handle(GlyphTraversal_in t, Event_in e);
    virtual void close();
};

class Viewer_var {
protected:
    ViewerRef _obj_;
public:
    Viewer_var() { _obj_ = 0; };
    Viewer_var(ViewerRef p) { _obj_ = p; }
    Viewer_var(const Viewer_var& r) {
        _obj_ = Viewer::_duplicate(r._obj_);
    }
    ~Viewer_var() { _BaseObject__release(_obj_); }

    Viewer_var& operator =(ViewerRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Viewer::_duplicate(p);
        return *this;
    }
    Viewer_var& operator =(const Viewer_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Viewer::_duplicate(r._obj_);
        return *this;
    }

    ViewerRef _obj() const { return _obj_; }
    ViewerRef& _out() { return _obj_; }
    operator ViewerRef() const { return _obj_; }
    ViewerRef operator ->() const { return _obj_; }
};

inline Viewer_var _tmp(ViewerRef p) { return Viewer_var(p); }

class ViewerStub : public Viewer {
public:
    ViewerStub(Exchange*);
    ~ViewerStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfFocusStub_create(Exchange*);

class Focus : public FrescoObject {
protected:
    Focus();
    virtual ~Focus();
public:
    FocusRef _obj() { return this; };
    static FocusRef  _return_ref(Focus_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static FocusRef _narrow(BaseObjectRef);
    static FocusRef _duplicate(FocusRef obj) {
        return (FocusRef)_BaseObject__duplicate(obj, &_XfFocusStub_create);
    }
    virtual void add_focus_interest(Viewer_in v);
    virtual void receive_focus_below(Viewer_in v, Boolean temporary);
    virtual void lose_focus_below(Viewer_in v, Boolean temporary);
    virtual void map_keystroke(Event::KeySym k, Action_in a);
    virtual void map_keychord(const Event::KeyChord& k, Action_in a);
};

class Focus_var {
protected:
    FocusRef _obj_;
public:
    Focus_var() { _obj_ = 0; };
    Focus_var(FocusRef p) { _obj_ = p; }
    Focus_var(const Focus_var& r) {
        _obj_ = Focus::_duplicate(r._obj_);
    }
    ~Focus_var() { _BaseObject__release(_obj_); }

    Focus_var& operator =(FocusRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Focus::_duplicate(p);
        return *this;
    }
    Focus_var& operator =(const Focus_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Focus::_duplicate(r._obj_);
        return *this;
    }

    FocusRef _obj() const { return _obj_; }
    FocusRef& _out() { return _obj_; }
    operator FocusRef() const { return _obj_; }
    FocusRef operator ->() const { return _obj_; }
};

inline Focus_var _tmp(FocusRef p) { return Focus_var(p); }

class FocusStub : public Focus {
public:
    FocusStub(Exchange*);
    ~FocusStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};


#endif
