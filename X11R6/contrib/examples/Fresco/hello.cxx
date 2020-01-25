#include <X11/Fresco/drawing.h>
#include <X11/Fresco/figures.h>
#include <X11/Fresco/fresco.h>

static Option options[] = {
    { "-s", "*string", Option::value, "on" },
    { "-string", "*string", Option::value, "on" },
    { nil }
};

int main(int argc, char** argv) {
    Fresco* f = Fresco_open("Test", argc, argv, options);
    CharString_var hello = Fresco::get_string(
	_tmp(f->fresco_style()), "string"
    );
    if (is_nil(hello)) {
	hello = Fresco::tmp_string_ref("hello goodbye");
    }
    FigureKit_var figures = f->figure_kit();
    f->run(nil, figures->label(_tmp(figures->default_style()), hello));
    Fresco::unref(f);
    return 0;
}
