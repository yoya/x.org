/*
 * Test for incremental update.
 */

#include <X11/Fresco/drawing.h>
#include <X11/Fresco/figures.h>
#include <X11/Fresco/fresco.h>
#include <X11/Fresco/layouts.h>
#include <X11/Fresco/widgets.h>
#include <X11/Fresco/Impls/action.h>
#include <X11/Fresco/Impls/fresco-impl.h>
#include <X11/Fresco/Impls/glyphs.h>
#include <X11/Fresco/Impls/region.h>
#include <X11/Fresco/Impls/styles.h>
#include <X11/Fresco/Impls/transform.h>
#include <X11/Fresco/Impls/Xdrawing.h>
#include <X11/Fresco/OS/thread.h>
#include <stdio.h>

static Option options[] = {
    { "-p", "*pause", Option::value },
    { "-pause", "*pause", Option::value },
    { "-ll", "*ll", Option::implicit, "on" },
    { "-lr", "*lr", Option::implicit, "on" },
    { "-ul", "*ul", Option::implicit, "on" },
    { "-ur", "*ur", Option::implicit, "on" },
    { nil }
};

class ColorPatch : public MonoGlyph {
public:
    ColorPatch(ColorRef, GlyphRef b);
    virtual ~ColorPatch();

    virtual void traverse(GlyphTraversalRef t);
private:
    Color_var color_;
};

ColorPatch::ColorPatch(ColorRef c, GlyphRef b) {
    body(b);
    color_ = Color::_duplicate(c);
}

ColorPatch::~ColorPatch () { }

void ColorPatch::traverse (GlyphTraversalRef t) {
    if (t->op() == GlyphTraversal::draw) {
        Painter_var p = t->current_painter();
        Region_var a = t->allocation();
        Vertex lower, upper;
        a->bounds(lower, upper);

        p->begin_path();
        p->move_to(lower.x, lower.y);
        p->line_to(upper.x, lower.y);
        p->line_to(upper.x, upper.y);
        p->line_to(lower.x, upper.y);
        p->close_path();
        p->current_color(color_);
        p->fill();
    } else {
        MonoGlyph::traverse(t);
    }
}   

class MaxShaper : public MonoGlyph {
public:
    MaxShaper(Glyph::Requisition req, GlyphRef b);

    virtual void request(Glyph::Requisition& r);
private:
    Glyph::Requisition req_;
};

MaxShaper::MaxShaper (Glyph::Requisition req, GlyphRef b) {
    body(b);
    req_ = req;
}

void MaxShaper::request (Glyph::Requisition& r) {
    MonoGlyph::request(r);
    if (req_.x.natural > r.x.natural) {
        r.x.natural = req_.x.natural;
    }
    if (req_.y.natural > r.y.natural) {
        r.y.natural = req_.y.natural;
    }
}

class Barrier : public MonoGlyph {
public:
    Barrier(GlyphRef b);
    virtual void request(Glyph::Requisition&);
    virtual void need_resize();
    virtual void traverse(GlyphTraversalRef t);
private:
    Glyph::Requisition req_;
    Boolean requested_;
};

Barrier::Barrier (GlyphRef b) {
    requested_ = false;
    body(b);
}

void Barrier::request (Glyph::Requisition& r) {
    if (!requested_) {
        requested_ = true;
        MonoGlyph::request(r);
        req_ = r;
    } else {
        r = req_;
    }
}

void Barrier::traverse(GlyphTraversalRef t) {
    Region_var region = t->allocation();
    Painter_var pr = t->current_painter();
    if (is_not_nil(pr)) {
	Vertex lower, upper;
	region->bounds(lower, upper);
	pr->push_clipping();
	pr->clip_rect(lower.x, lower.y, upper.x, upper.y);
	MonoGlyph::traverse(t);
	pr->pop_clipping();
    } else {
	MonoGlyph::traverse(t);
    }        
}
    
void Barrier::need_resize() {
    need_redraw();
}    

class TimedAction : public ActionImpl {
public:
    TimedAction(StyleRef style, GlyphRef parent, FigureKitRef figures);
    ~TimedAction();

    virtual void execute();
private:
    GlyphRef parent_;
    FigureKitRef figures_;
    float pause_;
};

TimedAction::TimedAction (
    StyleRef style, GlyphRef parent, FigureKitRef figures
) {
    parent_ = Glyph::_duplicate(parent);
    figures_ = figures;
    pause_ = 0.03;
    StyleValue_var a = style->bind(Fresco::tmp_string_ref("pause"));
    if (is_not_nil(a)) {
	double d;
	if (a->read_real(d)) {
	    pause_ = float(d);
	}
    }
}

TimedAction::~TimedAction() { }

void TimedAction::execute () {
    Coord orig_size = 10.0;
    for (;;) {
        for (long i = 2; i <= 20; i++) {
	    parent_->body(
		figures_->rectangle(
		    FigureKit::stroke, figures_->default_style(), 
		    0, 0, orig_size*i, orig_size*i
		)
	    );
            parent_->need_resize();
	    Fresco::delay(pause_);
        }
        for (long j = 19; j > 0; j--) {
	    parent_->body(
		figures_->rectangle(
		    FigureKit::stroke, figures_->default_style(), 
		    0, 0, orig_size*j, orig_size*j
		)
	    );
            parent_->need_resize();
	    Fresco::delay(pause_);
        }
    }
}

int main(int argc, char** argv) {
    Fresco* f = Fresco_open("Barrier", argc, argv, options);

    Style_var s = f->fresco_style();
    FigureKit_var figures = f->figure_kit();
    LayoutKit_var layouts = f->layout_kit();
    ThreadKit_var threads = f->thread_kit();
    
    Glyph_var g = figures->rectangle(
        FigureKit::stroke, _tmp(figures->default_style()),
	0, 0, 10, 10
    );

    Coord px, py, cx, cy;
    if (s->is_on(Fresco::tmp_string_ref("ll"))) {
	px = 0.0; py = 0.0;
	cx = 0.0; cy = 0.0;
    } else if (s->is_on(Fresco::tmp_string_ref("lr"))) {
	px = 1.0; py = 0.0;
	cx = 1.0; cy = 0.0;
    } else if (s->is_on(Fresco::tmp_string_ref("ul"))) {
	px = 0.0; py = 1.0;
	cx = 0.0; cy = 1.0;
    } else if (s->is_on(Fresco::tmp_string_ref("ur"))) {
	px = 1.0; py = 1.0;
	cx = 1.0; cy = 1.0;
    } else {
	px = 0.5; py = 0.5;
	cx = 0.5; cy = 0.5;
    }
    GlyphRef t = new TransformAllocator(px, py, 0.0, cx, cy, 0.0);
    t->body(g);
    g = t;

    Coord fil = layouts->fil();
    ThreadObj_var tr = threads->thread(_tmp(new TimedAction(s, g, figures)));

    Glyph::Requisition req;
    req.x.natural = 100;
    req.x.maximum = 100;
    req.x.minimum = 100;

    req.y.natural = 75;
    req.y.maximum = 75;
    req.y.minimum = 75;

    g = new MaxShaper(req, g);

    Glyph_var patch1 = new ColorPatch(
        new ColorImpl(1.0, 0.0, 0.0), layouts->hglue(30.0, 0, fil*100)
    );

    Glyph_var patch2 = new ColorPatch(
        new ColorImpl(0.0, 0.0, 1.0), layouts->vglue(30.0, 0, fil*100)
    );
    Glyph_var hbox = layouts->hbox();
    hbox->append(patch1);
    hbox->append(g);
    hbox->append(patch1);

    Glyph_var vbox = layouts->vbox();
    vbox->append(patch2);
    vbox->append(hbox);
    vbox->append(patch2);

    g = new Barrier(vbox);
    g = layouts->margin(g, 15.0);

    if (tr == nil) {
        printf("timer thread creation failed\n");
    } else {
        tr->run();
    }
    f->run(nil, g);
    Fresco::unref(f);
    return 0;
}
