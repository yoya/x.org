#include <X11/Fresco/display.h>
#include <X11/Fresco/drawing.h>
#include <X11/Fresco/figures.h>
#include <X11/Fresco/fresco.h>
#include <X11/Fresco/layouts.h>
#include <X11/Fresco/Impls/action.h>
#include <X11/Fresco/Impls/glyphs.h>
#include <X11/Fresco/OS/thread.h>

static Option options[] = {
    { "-d", "*delta", Option::value },
    { "-d1", "*zoomer1*delta", Option::value },
    { "-d2", "*zoomer2*delta", Option::value },
    { "-delta", "*delta", Option::value },
    { "-delta1", "*zoomer1*delta", Option::value },
    { "-delta2", "*zoomer2*delta", Option::value },
    { "-p", "*pause", Option::value },
    { "-p1", "*zoomer1*pause", Option::value },
    { "-p2", "*zoomer2*pause", Option::value },
    { "-pause", "*pause", Option::value },
    { "-pause1", "*zoomer1*pause", Option::value },
    { "-pause2", "*zoomer2*pause", Option::value },
    { "-v", "*verbose", Option::implicit, "on" },
    { "-verbose", "*verbose", Option::implicit, "on" },
    { nil }
};

class Zoomer : public MonoGlyph {
public:
    Zoomer(Fresco*, GlyphRef, StyleRef);
    virtual ~Zoomer();

    void zoom();

    void traverse(GlyphTraversalRef t); //+ Glyph::traverse
private:
    Transform_var matrix_;
    float scale_;
    float delta_;
    float pause_;
    ThreadObj_var zoom_;
};

declareActionCallback(Zoomer)
implementActionCallback(Zoomer)

Zoomer::Zoomer(Fresco* f, GlyphRef g, StyleRef s) {
    body(g);
    matrix_ = _tmp(f->drawing_kit())->identity_matrix();
    scale_ = 1.0;
    delta_ = 0.02;
    pause_ = 0.1;
    double value;
    StyleValue_var a = s->resolve(Fresco::tmp_string_ref("delta"));
    if (is_not_nil(a)) {
	if (a->read_real(value)) {
	    delta_ = float(value);
	}
    }
    a = s->resolve(Fresco::tmp_string_ref("pause"));
    if (is_not_nil(a)) {
	if (a->read_real(value)) {
	    pause_ = float(value);
	}
    }
    zoom_ = _tmp(f->thread_kit())->thread(
	_tmp(new ActionCallback(Zoomer)(this, &Zoomer::zoom))
    );
    zoom_->run();
}

Zoomer::~Zoomer() {
    zoom_->terminate();
}

void Zoomer::zoom() {
    for (;;) {
	if (Fresco::delay(pause_)) {
	    scale_ -= delta_;
	    if (scale_ < 0.05 || scale_ > 0.999999) {
		delta_ = -delta_;
	    }
	    need_resize();
	}
    }
}

//+ Zoomer(Glyph::traverse)
void Zoomer::traverse(GlyphTraversalRef t) {
    Painter_var p = t->current_painter();
    if (is_not_nil(p)) {
	p->push_matrix();
	Vertex v;
	v.x = scale_; v.y = scale_; v.z = scale_;
	matrix_->load_identity();
	matrix_->scale(v);
	p->premultiply(matrix_);
	MonoGlyph::traverse(t);
	p->pop_matrix();
    }
}

int main(int argc, char** argv) {
    Fresco* f = Fresco_open("MTest", argc, argv, options);
    FigureKit_var figures = f->figure_kit();
    Glyph_var g = figures->figure_root(
	figures->circle(
	    FigureKit::fill, _tmp(figures->default_style()), 0.0, 0.0, 100.0
	)
    );
    Style_var s = f->fresco_style();
    Boolean verbose = s->is_on(Fresco::tmp_string_ref("verbose"));
    if (verbose) {
	g = new DebugGlyph(g, "circle", DebugGlyph::trace_request_traverse);
    }
    Style_var s1 = s->new_style();
    s1->name(Fresco::tmp_string_ref("zoomer1"));
    s1->link_parent(s);
    Glyph_var g1 = new Zoomer(f, g, s1);
    if (verbose) {
	g1 = new DebugGlyph(
	    g1, "zoomer 1", DebugGlyph::trace_request_traverse
	);
    }
    Style_var s2 = s->new_style();
    s2->name(Fresco::tmp_string_ref("zoomer2"));
    s2->link_parent(s);
    Glyph_var g2 = new Zoomer(f, g, s2);
    if (verbose) {
	g2 = new DebugGlyph(
	    g2, "zoomer 2", DebugGlyph::trace_request_traverse
	);
    }
    LayoutKit_var layouts = f->layout_kit();
    g = layouts->hbox();
    g->append(_tmp(layouts->fixed(g1, 200, 200)));
    g->append(_tmp(layouts->hfil()));
    g->append(_tmp(layouts->fixed(g2, 200, 200)));
    if (verbose) {
	g = new DebugGlyph(g, "hbox", DebugGlyph::trace_request_traverse);
    }
    f->run(nil, g);
    Fresco::unref(f);
    return 0;
}
