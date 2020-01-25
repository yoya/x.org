/*
 * $XConsortium: client.cxx,v 1.2 94/03/23 15:52:55 matt Exp $
 */

#include "naming.h"
#include <X11/Fresco/Ox/env.h>
#include <X11/Fresco/Ox/schema.h>
#include <X11/Fresco/Ox/stub.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

static void print_error(char* s1, char* s2);
static void do_list(NamingContextRef nc, char* str);
static NamingContext::Name* string_to_name(char* s);
static int cp_paths(char* s, char*& end_str);

int main(int argc, char** argv) {
    if (argc != 2 && argc != 3) {
	printf("Usage: client dir_name [ dir_name ]\n");
	exit(1);
    }

    extern TypeObj_Descriptor TypeVar(NamingContext);
    TypeSchemaImpl::schema()->load(&TypeVar(NamingContext));
    NamingContextRef nc = new NamingContextStub(
	new RemoteExchange("localhost", 0x20000001, 0, 0)
    );
    char* cwd = getcwd(0, 128);

    char buf[256];
    if (*argv[1] == '/') {
	strcpy(buf, argv[1]);
    } else {
	strcpy(buf, cwd);
	strcat(buf, "/");
	strcat(buf, argv[1]);
    }
    do_list(nc, buf);

    if (argc == 3) {
	if (*argv[2] == '/') {
	    strcpy(buf, argv[2]);
	} else {
	    strcpy(buf, cwd);
	    strcat(buf, "/");
	    strcat(buf, argv[2]);
	}
	Env env;
	NamingContext::Name* n = string_to_name(buf);
	BaseObjectRef o = nc->resolve(*n, env);
	if (env.check_exception()) {
	    const Exception* ex = env.exception_value();
	    fprintf(stderr, "%s: ", buf);
	    switch (env.exception_value()->_hash()) {
	    case NamingContext::NotFound::_code: {
		const NamingContext::NotFound* n =
		    NamingContext::NotFound::_cast(ex);
		fprintf(stderr, "Not found (%d)\n", n->mode);
		break;
	    }
	    case NamingContext::PermissionDenied::_code:
		fprintf(stderr, "No permission\n");
		break;
	    case NamingContext::NotContext::_code:
		fprintf(stderr, "Not a context\n");
		break;
	    }
	    exit(1);
	}
	NamingContextRef nc2 = NamingContext::_narrow(o);
	if (is_nil(nc2)) {
	    fprintf(stderr, "narrow failed\n");
	} else {
	    do_list(nc2, ".");
	}
    }
    return 0;
}

static int cp_paths(char* s, char*& end_str) {
    int num_names = 0;
    while (1) {
	while (*s == '/') {
	    s++;
	}
	if (*s == 0) {
	    break;
	}
	num_names++;
	while (*s != '/' && *s != 0) {
	    *end_str++ = *s++;
	}
	*end_str++ = 0;
    }
    return num_names;
}

static NamingContext::Name* string_to_name(char* s) {
    char* buf;
    char* end_str;
    int num_names = 0;
    buf = new char[strlen(s) + 1];
    end_str = buf;
    num_names += cp_paths(s, end_str);

    NamingContext::NameComponent *nnc =
	new NamingContext::NameComponent[num_names];
    end_str = buf;
    for (long i = 0; i < num_names; i++) {
	nnc[i] = new char[strlen(end_str) + 1];
	strcpy(nnc[i], end_str);
	end_str += strlen(end_str) + 1;
    }
    NamingContext::Name* n = new NamingContext::Name;
    n->_maximum = num_names;
    n->_length = num_names;
    n->_buffer = nnc;
    return n;
}

static void do_list(NamingContextRef nc, char* str) {
    printf("** Listing %s:\n", str);
    NamingContext::BindingInfoList l = nc->list(*string_to_name(str));
#ifdef notdef
    if (_error_code != 0) {
	print_error("list", str);
    } else {
#endif
	for (int i = 0; i < l._length; i++) {
	    printf("%s\n", l._buffer[i].comp);
	}
#ifdef notdef
    }
#endif
}

#ifdef notdef
static void print_error(char *s1, char *s2) {
    switch (_error_code) {
	case naming_not_context:
	    printf("%s(%s): not context\n", s1, s2);
	    break;
	case naming_not_found:
	    printf("%s(%s): not found\n", s1, s2);
	    break;
	case naming_permission_denied:
	    printf("%s(%s): permission denied\n", s1, s2);
	    break;
    }
}
#endif
