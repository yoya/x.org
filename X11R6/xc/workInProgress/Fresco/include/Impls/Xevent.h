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

#ifndef Fresco_Impls_Xevent_h
#define Fresco_Impls_Xevent_h

#include <X11/Fresco/display.h>
#include <X11/Fresco/Impls/fobjects.h>
#include <X11/Fresco/Impls/Xlib.h>

class DisplayImpl;

//+ EventImpl : Event
class EventImpl : public Event {
public:
    ~EventImpl();
    TypeObjId _tid();
    static EventImpl* _narrow(BaseObjectRef);
//+
public:
    EventImpl(DisplayImpl*);

    //+ Event::*
    /* FrescoObject */
    Long ref__(Long references);
    Tag attach(FrescoObject_in observer);
    void detach(Tag attach_tag);
    void disconnect();
    void notify_observers();
    void update();
    /* Event */
    Event::TypeId type();
    Event::TimeStamp time();
    Boolean double_click(Event::TimeStamp previous);
    Boolean positional();
    Coord pointer_x();
    Coord pointer_y();
    Event::ButtonIndex pointer_button();
    Boolean button_is_down(Event::ButtonIndex b);
    Boolean modifier_is_down(Event::Modifier m);
    Event::KeySym key();
    CharCode character();
    void unread();
    //+

    XEvent xevent_;
    WindowRef window_;

    void clear();
    void locate();
    unsigned int keymask();
protected:
    SharedFrescoObjectImpl object_;
    DisplayImpl* display_;
    Coord x_, y_, root_x_, root_y_;
    Boolean location_valid_;
};

#endif
