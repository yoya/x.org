/* DO NOT EDIT -- Automatically generated from Interfaces/widgets.idl */

#ifndef Interfaces_widgets_h
#define Interfaces_widgets_h

#include <X11/Fresco/_enter.h>
#include <X11/Fresco/viewer.h>

class Adjustment;
typedef Adjustment* AdjustmentRef;
typedef AdjustmentRef Adjustment_in;
typedef AdjustmentRef Adjustment_out, Adjustment_inout;
typedef AdjustmentRef Adjustment_return;
class Adjustment_var;

extern BaseObjectRef _XfAdjustmentStub_create(Exchange*);

class Adjustment : public FrescoObject {
protected:
    Adjustment();
    virtual ~Adjustment();
public:
    AdjustmentRef _obj() { return this; };
    static AdjustmentRef  _return_ref(Adjustment_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static AdjustmentRef _narrow(BaseObjectRef);
    static AdjustmentRef _duplicate(AdjustmentRef obj) {
        return (AdjustmentRef)_BaseObject__duplicate(obj, &_XfAdjustmentStub_create);
    }
    struct Settings {
        Coord lower, upper, length;
        Coord cur_lower, cur_upper, cur_length;
    };
    virtual Coord small_scroll();
    virtual void small_scroll(Coord _p);
    virtual Coord large_scroll();
    virtual void large_scroll(Coord _p);
    virtual void get_settings(Adjustment::Settings& s);
    virtual void begin();
    virtual void commit();
    virtual void cancel();
    virtual void scroll_forward();
    virtual void scroll_backward();
    virtual void page_forward();
    virtual void page_backward();
    virtual void scroll_to(Coord lower);
    virtual void scroll_by(Coord delta);
    virtual void scale_to(Coord length);
    virtual void constrain(Coord& c);
};

class Adjustment_var {
protected:
    AdjustmentRef _obj_;
public:
    Adjustment_var() { _obj_ = 0; };
    Adjustment_var(AdjustmentRef p) { _obj_ = p; }
    Adjustment_var(const Adjustment_var& r) {
        _obj_ = Adjustment::_duplicate(r._obj_);
    }
    ~Adjustment_var() { _BaseObject__release(_obj_); }

    Adjustment_var& operator =(AdjustmentRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Adjustment::_duplicate(p);
        return *this;
    }
    Adjustment_var& operator =(const Adjustment_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Adjustment::_duplicate(r._obj_);
        return *this;
    }

    AdjustmentRef _obj() const { return _obj_; }
    AdjustmentRef& _out() { return _obj_; }
    operator AdjustmentRef() const { return _obj_; }
    AdjustmentRef operator ->() const { return _obj_; }
};

inline Adjustment_var _tmp(AdjustmentRef p) { return Adjustment_var(p); }

class AdjustmentStub : public Adjustment {
public:
    AdjustmentStub(Exchange*);
    ~AdjustmentStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class Telltale;
typedef Telltale* TelltaleRef;
typedef TelltaleRef Telltale_in;
typedef TelltaleRef Telltale_out, Telltale_inout;
typedef TelltaleRef Telltale_return;
class Telltale_var;

class Button;
typedef Button* ButtonRef;
typedef ButtonRef Button_in;
typedef ButtonRef Button_out, Button_inout;
typedef ButtonRef Button_return;
class Button_var;

extern BaseObjectRef _XfButtonStub_create(Exchange*);

class Button : public Viewer {
protected:
    Button();
    virtual ~Button();
public:
    ButtonRef _obj() { return this; };
    static ButtonRef  _return_ref(Button_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static ButtonRef _narrow(BaseObjectRef);
    static ButtonRef _duplicate(ButtonRef obj) {
        return (ButtonRef)_BaseObject__duplicate(obj, &_XfButtonStub_create);
    }
    virtual Telltale_return state();
    virtual void state(Telltale_in _p);
    virtual Action_return click_action();
    virtual void click_action(Action_in _p);
};

class Button_var {
protected:
    ButtonRef _obj_;
public:
    Button_var() { _obj_ = 0; };
    Button_var(ButtonRef p) { _obj_ = p; }
    Button_var(const Button_var& r) {
        _obj_ = Button::_duplicate(r._obj_);
    }
    ~Button_var() { _BaseObject__release(_obj_); }

    Button_var& operator =(ButtonRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Button::_duplicate(p);
        return *this;
    }
    Button_var& operator =(const Button_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Button::_duplicate(r._obj_);
        return *this;
    }

    ButtonRef _obj() const { return _obj_; }
    ButtonRef& _out() { return _obj_; }
    operator ButtonRef() const { return _obj_; }
    ButtonRef operator ->() const { return _obj_; }
};

inline Button_var _tmp(ButtonRef p) { return Button_var(p); }

class ButtonStub : public Button {
public:
    ButtonStub(Exchange*);
    ~ButtonStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfTelltaleStub_create(Exchange*);

class Telltale : public FrescoObject {
protected:
    Telltale();
    virtual ~Telltale();
public:
    TelltaleRef _obj() { return this; };
    static TelltaleRef  _return_ref(Telltale_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static TelltaleRef _narrow(BaseObjectRef);
    static TelltaleRef _duplicate(TelltaleRef obj) {
        return (TelltaleRef)_BaseObject__duplicate(obj, &_XfTelltaleStub_create);
    }
    enum Flag {
        enabled, visible, active, chosen, running, stepping, choosable, 
        toggle
    };
    virtual void set(Telltale::Flag f);
    virtual void clear(Telltale::Flag f);
    virtual Boolean test(Telltale::Flag f);
    virtual Telltale_return current();
    virtual void current(Telltale_in _p);
};

class Telltale_var {
protected:
    TelltaleRef _obj_;
public:
    Telltale_var() { _obj_ = 0; };
    Telltale_var(TelltaleRef p) { _obj_ = p; }
    Telltale_var(const Telltale_var& r) {
        _obj_ = Telltale::_duplicate(r._obj_);
    }
    ~Telltale_var() { _BaseObject__release(_obj_); }

    Telltale_var& operator =(TelltaleRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Telltale::_duplicate(p);
        return *this;
    }
    Telltale_var& operator =(const Telltale_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Telltale::_duplicate(r._obj_);
        return *this;
    }

    TelltaleRef _obj() const { return _obj_; }
    TelltaleRef& _out() { return _obj_; }
    operator TelltaleRef() const { return _obj_; }
    TelltaleRef operator ->() const { return _obj_; }
};

inline Telltale_var _tmp(TelltaleRef p) { return Telltale_var(p); }

class TelltaleStub : public Telltale {
public:
    TelltaleStub(Exchange*);
    ~TelltaleStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class Menu;
typedef Menu* MenuRef;
typedef MenuRef Menu_in;
typedef MenuRef Menu_out, Menu_inout;
typedef MenuRef Menu_return;
class Menu_var;

class MenuItem;
typedef MenuItem* MenuItemRef;
typedef MenuItemRef MenuItem_in;
typedef MenuItemRef MenuItem_out, MenuItem_inout;
typedef MenuItemRef MenuItem_return;
class MenuItem_var;

extern BaseObjectRef _XfMenuItemStub_create(Exchange*);

class MenuItem : public Button {
protected:
    MenuItem();
    virtual ~MenuItem();
public:
    MenuItemRef _obj() { return this; };
    static MenuItemRef  _return_ref(MenuItem_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static MenuItemRef _narrow(BaseObjectRef);
    static MenuItemRef _duplicate(MenuItemRef obj) {
        return (MenuItemRef)_BaseObject__duplicate(obj, &_XfMenuItemStub_create);
    }
    virtual Menu_return submenu();
    virtual void submenu(Menu_in _p);
    virtual MenuItem_return next_menu();
    virtual MenuItem_return prev_menu();
    virtual void insert_menu_before(MenuItem_in i);
    virtual void insert_menu_after(MenuItem_in i);
    virtual void replace_menu(MenuItem_in i);
    virtual void remove_menu();
};

class MenuItem_var {
protected:
    MenuItemRef _obj_;
public:
    MenuItem_var() { _obj_ = 0; };
    MenuItem_var(MenuItemRef p) { _obj_ = p; }
    MenuItem_var(const MenuItem_var& r) {
        _obj_ = MenuItem::_duplicate(r._obj_);
    }
    ~MenuItem_var() { _BaseObject__release(_obj_); }

    MenuItem_var& operator =(MenuItemRef p) {
        _BaseObject__release(_obj_);
        _obj_ = MenuItem::_duplicate(p);
        return *this;
    }
    MenuItem_var& operator =(const MenuItem_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = MenuItem::_duplicate(r._obj_);
        return *this;
    }

    MenuItemRef _obj() const { return _obj_; }
    MenuItemRef& _out() { return _obj_; }
    operator MenuItemRef() const { return _obj_; }
    MenuItemRef operator ->() const { return _obj_; }
};

inline MenuItem_var _tmp(MenuItemRef p) { return MenuItem_var(p); }

class MenuItemStub : public MenuItem {
public:
    MenuItemStub(Exchange*);
    ~MenuItemStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

extern BaseObjectRef _XfMenuStub_create(Exchange*);

class Menu : public Viewer {
protected:
    Menu();
    virtual ~Menu();
public:
    MenuRef _obj() { return this; };
    static MenuRef  _return_ref(Menu_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static MenuRef _narrow(BaseObjectRef);
    static MenuRef _duplicate(MenuRef obj) {
        return (MenuRef)_BaseObject__duplicate(obj, &_XfMenuStub_create);
    }
    virtual void append_menu_item(MenuItem_in i);
    virtual void prepend_menu_item(MenuItem_in i);
    virtual MenuItem_return first_menu_item();
    virtual MenuItem_return last_menu_item();
    virtual MenuItem_return selected();
    virtual void selected(MenuItem_in _p);
};

class Menu_var {
protected:
    MenuRef _obj_;
public:
    Menu_var() { _obj_ = 0; };
    Menu_var(MenuRef p) { _obj_ = p; }
    Menu_var(const Menu_var& r) {
        _obj_ = Menu::_duplicate(r._obj_);
    }
    ~Menu_var() { _BaseObject__release(_obj_); }

    Menu_var& operator =(MenuRef p) {
        _BaseObject__release(_obj_);
        _obj_ = Menu::_duplicate(p);
        return *this;
    }
    Menu_var& operator =(const Menu_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = Menu::_duplicate(r._obj_);
        return *this;
    }

    MenuRef _obj() const { return _obj_; }
    MenuRef& _out() { return _obj_; }
    operator MenuRef() const { return _obj_; }
    MenuRef operator ->() const { return _obj_; }
};

inline Menu_var _tmp(MenuRef p) { return Menu_var(p); }

class MenuStub : public Menu {
public:
    MenuStub(Exchange*);
    ~MenuStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};

class WidgetKit;
typedef WidgetKit* WidgetKitRef;
typedef WidgetKitRef WidgetKit_in;
typedef WidgetKitRef WidgetKit_out, WidgetKit_inout;
typedef WidgetKitRef WidgetKit_return;
class WidgetKit_var;

extern BaseObjectRef _XfWidgetKitStub_create(Exchange*);

class WidgetKit : public FrescoObject {
protected:
    WidgetKit();
    virtual ~WidgetKit();
public:
    WidgetKitRef _obj() { return this; };
    static WidgetKitRef  _return_ref(WidgetKit_return r) { return r; }

    virtual void* _this();
    virtual TypeObjId _tid();

    static WidgetKitRef _narrow(BaseObjectRef);
    static WidgetKitRef _duplicate(WidgetKitRef obj) {
        return (WidgetKitRef)_BaseObject__duplicate(obj, &_XfWidgetKitStub_create);
    }
    virtual Glyph_return inset_frame(Glyph_in g);
    virtual Glyph_return outset_frame(Glyph_in g);
    virtual Glyph_return bright_inset_frame(Glyph_in g);
    virtual Viewer_return label(CharString_in s);
    virtual Menu_return menubar();
    virtual Menu_return pulldown();
    virtual Menu_return pullright();
    virtual MenuItem_return menubar_item(Glyph_in g);
    virtual MenuItem_return menu_item(Glyph_in g);
    virtual MenuItem_return check_menu_item(Glyph_in g);
    virtual MenuItem_return radio_menu_item(Glyph_in g, Telltale_in group);
    virtual MenuItem_return menu_item_separator();
    virtual Telltale_return telltale_group();
    virtual Button_return push_button(Glyph_in g, Action_in a);
    virtual Button_return default_button(Glyph_in g, Action_in a);
    virtual Button_return palette_button(Glyph_in g, Action_in a);
    virtual Button_return check_box(Glyph_in g, Action_in a);
    virtual Button_return radio_button(Glyph_in g, Action_in a, Telltale_in group);
    virtual Adjustment_return bounded_float(Float lower, Float upper, Float initial);
    virtual Viewer_return slider(Axis a, Adjustment_in adj);
    virtual Viewer_return scroll_bar(Axis a, Adjustment_in adj);
    virtual Viewer_return panner(Adjustment_in x, Adjustment_in y);
    virtual Button_return zoomer(Coord scale, Adjustment_in x, Adjustment_in y, Adjustment_in z);
    virtual Button_return up_mover(Adjustment_in a);
    virtual Button_return down_mover(Adjustment_in a);
    virtual Button_return left_mover(Adjustment_in a);
    virtual Button_return right_mover(Adjustment_in a);
    virtual Viewer_return viewer_group(Glyph_in g);
};

class WidgetKit_var {
protected:
    WidgetKitRef _obj_;
public:
    WidgetKit_var() { _obj_ = 0; };
    WidgetKit_var(WidgetKitRef p) { _obj_ = p; }
    WidgetKit_var(const WidgetKit_var& r) {
        _obj_ = WidgetKit::_duplicate(r._obj_);
    }
    ~WidgetKit_var() { _BaseObject__release(_obj_); }

    WidgetKit_var& operator =(WidgetKitRef p) {
        _BaseObject__release(_obj_);
        _obj_ = WidgetKit::_duplicate(p);
        return *this;
    }
    WidgetKit_var& operator =(const WidgetKit_var& r) {
        _BaseObject__release(_obj_);
        _obj_ = WidgetKit::_duplicate(r._obj_);
        return *this;
    }

    WidgetKitRef _obj() const { return _obj_; }
    WidgetKitRef& _out() { return _obj_; }
    operator WidgetKitRef() const { return _obj_; }
    WidgetKitRef operator ->() const { return _obj_; }
};

inline WidgetKit_var _tmp(WidgetKitRef p) { return WidgetKit_var(p); }

class WidgetKitStub : public WidgetKit {
public:
    WidgetKitStub(Exchange*);
    ~WidgetKitStub();

    Exchange* _exchange();
protected:
    Exchange* exch_;
};


#endif
