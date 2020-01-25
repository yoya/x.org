/*
 * Copyright (c) 1990, 1991 Stanford University
 * Copyright (c) 1991 Silicon Graphics, Inc.
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

#include <X11/Fresco/drawing.h>
#include <X11/Fresco/fresco.h>
#include <X11/Fresco/Impls/charstr.h>
#include <X11/Fresco/Impls/styles.h>
#include <X11/Fresco/OS/file.h>
#include <X11/Fresco/OS/list.h>
#include <X11/Fresco/OS/memory.h>
#include <stdio.h>
#include <string.h>

class StylePrinter : public StyleVisitorImpl {
public:
    StylePrinter();
    virtual ~StylePrinter();

    Boolean visit_alias(CharStringRef name); //+ StyleVisitor::visit_alias
    Boolean visit_attribute(StyleValueRef a); //+ StyleVisitor::visit_attribute

    static void print(StyleRef);
};

static Option options[] = {
    { "-v", "*verbose", Option::implicit, "on" },
    { "-verbose", "*verbose", Option::implicit, "on" },
    { nil }
};

int main(int argc, char** argv) {
    Fresco* f = Fresco_open("StyleTest", argc, argv, options);
    Style_var style = f->fresco_style();
    Boolean verbose = style->is_on(Fresco::string_ref("verbose"));
    if (verbose) {
	StylePrinter::print(style);
    }
    style = _tmp(f->drawing_kit())->drawing_style();
    if (verbose) {
	StylePrinter::print(style);
    }
    char buf[1024];
    while (fgets(buf, sizeof(buf), stdin) != NULL) {
	int n = strlen(buf) - 1;
	if (buf[n] == '\n') {
	    buf[n] = '\0';
	}
	char* new_buf = new char[n + 1];
	strcpy(new_buf, buf);
	char* str = new_buf;
	Style_var root = style->new_style();
	root->merge(style);
	Style_var s = root;
	for (;;) {
	    if (verbose) {
		StylePrinter::print(s);
	    }
	    char* i = strchr(str, '.');
	    if (i == nil) {
		break;
	    }
	    Style_var child = s->new_style();
	    child->link_parent(s);
	    s = child;
	    char* names = str;
	    *i = '\0';
	    str = i + 1;
	    for (;;) {
		char* j = strrchr(names, ',');
		if (j == nil) {
		    break;
		}
		s->alias(_tmp(new CharStringImpl(j + 1)));
		*j = '\0';
	    }
	    s->alias(_tmp(new CharStringImpl(names)));
	}
	if (*str != '\0') {
	    printf("%s = ", buf);
	    StyleValue_var a = s->resolve(_tmp(new CharStringImpl(str)));
	    CharString_var v;
	    if (is_not_nil(a) && a->read_string(v._out())) {
		CharStringBuffer buf(v);
		printf("\"%.*s\"\n", buf.length(), buf.string());
	    } else {
		printf("?\n");
	    }
	}
	delete new_buf;
    }
    Fresco::unref(f);
    return 0;
}

StylePrinter::StylePrinter() { }
StylePrinter::~StylePrinter() { }

//+ StylePrinter(StyleVisitor::visit_alias)
Boolean StylePrinter::visit_alias(CharStringRef name) {
    CharStringBuffer buf(name);
    printf("alias '%.*s'\n", buf.length(), buf.string());
    return true;
}

//+ StylePrinter(StyleVisitor::visit_attribute)
Boolean StylePrinter::visit_attribute(StyleValueRef a) {
    CharStringBuffer name(a->name());
    printf("%.*s[%d] = '", name.length(), name.string(), a->priority());
    CharString_var s;
    if (a->read_string(s._out())) {
	CharStringBuffer value(s);
	printf("%.*s", value.length(), value.string());
    } else {
	printf("???");
    }
    printf("'\n");
    fflush(stdout);
    return true;
}

void StylePrinter::print(StyleRef s) {
    CharString_var name = s->name();
    if (is_nil(name)) {
	printf("Style <noname>\n");
    } else {
	CharStringBuffer buf(name);
	printf("Style %.*s\n", buf.length(), buf.string());
    }
    StyleVisitor_var p = new StylePrinter;
    s->visit_aliases(p);
    s->visit_attributes(p);
    printf("\n");
}
