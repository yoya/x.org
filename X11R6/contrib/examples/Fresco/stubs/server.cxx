#include "naming.h"
#include <X11/Fresco/Ox/env.h>
#include <X11/Fresco/Ox/schema.h>
#include <X11/Fresco/Ox/stub.h>
#include <X11/Fresco/Ox/transport.h>
#include <errno.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#if !sgi
/* No one except SGI seems to provide a prototype for this */
extern "C" {
    int registerrpc(u_long,u_long,u_long,void*(*)(void*),xdrproc_t,xdrproc_t);
}
#endif


class NamingContextImpl : public NamingContext {
public:
    NamingContextImpl(
	char* host, Long port, Long impl, Long o, char* root
    );
    ~NamingContextImpl();

    Exchange* _exchange();

    //+ NamingContext::=
    BaseObject_return resolve(const Name& n, Env* _env = 0);
    BindingInfoList list(const Name& n, Env* _env = 0);
    //+

    char* name_to_string(NamingContext::Name);
protected:
    RemoteExchange exch_;
    long obj_id;
    char* root;
};

static const ULong MAX_IMPLS = 10;
NamingContextImpl* impls[MAX_IMPLS];
ULong num_impls = 0;

NamingContextImpl* add_impl(char* root) {
    if (num_impls + 1 == MAX_IMPLS) {
	printf("Out of impls\n");
	exit(1);
    }
    impls[num_impls] = new NamingContextImpl(
	"localhost", 0x20000001, 0, num_impls, root
    );
    num_impls++;
    return impls[num_impls - 1];
}

static void* dispatch_call(void* data) {
    MarshalBuffer* b = (MarshalBuffer*)data;
    ULong impl_id = b->get_unsigned_long();
    ULong obj_id = b->get_unsigned_long();
    if (obj_id >= num_impls) {
	fprintf(stderr, "Bad object id %d\n", obj_id);
    } else {
	Env e;
	b->env(&e);
	b->dispatch(impls[obj_id]);
	b->env(nil);
    }
    return b;
}

int main() {
    registerrpc(
	0x20000001, 0, 1, &dispatch_call,
	xdrproc_t(&oxUnmarshal), xdrproc_t(&oxMarshal)
    );
    add_impl("");
    printf("starting server\n"); fflush(stdout);
    svc_run();
    return 0;
}

char* NamingContextImpl::name_to_string(NamingContext::Name n) {
    long len = strlen(root) + 1;
    for (long i = 0; i < n._length; i++) {
	len += strlen(n._buffer[i]) + 1;
    }
    char *str = new char[len];
    strcpy(str, root);
    if (n._length > 0) {
	strcat(str, "/");
	strcat(str, n._buffer[0]);
	for (i = 1; i < n._length; i++) {
	    strcat(str, "/");
	    strcat(str, n._buffer[i]);
	}
    }
    return str;
}

extern NamingContextImpl* add_impl(char* root);

NamingContextImpl::NamingContextImpl(
    char* host, Long port, Long impl, Long o, char* r
) : exch_(host, port, impl, o) {
    root = r;
}

NamingContextImpl::~NamingContextImpl() {
    /* unimplemented */
}

Exchange* NamingContextImpl::_exchange() { return &exch_; }

//+ NamingContextImpl(NamingContext::resolve)
BaseObject_return NamingContextImpl::resolve(const Name& n, Env* _env) {
    char* str = name_to_string(n);
    printf("NamingContextImpl::resolve: Name=%s\n", str);
    struct stat stat_buf;
    if (stat(str, &stat_buf) < 0) {
	switch (errno) {
	case ENOENT:
	case ENOTDIR: {
	    _env->set_exception(new NamingContext::NotFound(17));
	    return nil;
	}
	case EACCES:
	    _env->set_exception(new NamingContext::PermissionDenied);
	    return nil;
	}
    }
    if (S_ISDIR(stat_buf.st_mode)) {
	return add_impl(str);
    }
    _env->set_exception(new NamingContext::NotContext);
    return nil;
}

//+ NamingContextImpl(NamingContext::list)
NamingContext::BindingInfoList NamingContextImpl::list(const Name& n, Env* _env) {
    char* str = name_to_string(n);
    printf("NamingContextImpl::list: Name=%s\n", str);
    NamingContext::BindingInfoList list;
    NamingContext::BindingInfo* elems = new NamingContext::BindingInfo[500];
    long num_bindings = 0;
    DIR *dir = opendir(str);
    if (dir == 0) {
	switch (errno) {
	case ENOTDIR:
	    _env->set_exception(new NamingContext::NotContext);
	    return list;
	case ENOENT:
	    _env->set_exception(new NamingContext::NotFound);
	    return list;
	case EACCES:
	    _env->set_exception(new NamingContext::PermissionDenied);
	    return list;
	}
    }
    for (dirent* de = readdir(dir); de != 0; de = readdir(dir)) {
	long len = strlen(de->d_name);
	elems[num_bindings].comp = new char[len + 1];
	strcpy(elems[num_bindings].comp, de->d_name);
	elems[num_bindings].bt = NamingContext::regular_type;
	num_bindings++;
    }
    closedir(dir);
    list._maximum = num_bindings;
    list._length = num_bindings;
    list._buffer = elems;
    return list;
}

//+ NamingContext::%server
void _NamingContext_receive(BaseObjectRef _object, ULong _m, MarshalBuffer& _b) {
    extern TypeObjId _NamingContext_tid;
    NamingContextRef _this = (NamingContextRef)_BaseObject_tcast(_object, _NamingContext_tid);
    Env* _env = _b.env();
    switch (_m) {
        case /* resolve */ 0: {
            extern MarshalBuffer::ArgInfo _NamingContext_resolve_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            NamingContext::Name n;
            _arg[1].u_addr = &n;
            _b.receive(_NamingContext_resolve_pinfo, _arg);
            _arg[0].u_objref = _this->resolve(n, _env);
            _b.reply(_NamingContext_resolve_pinfo, _arg);
            break;
        }
        case /* list */ 1: {
            extern MarshalBuffer::ArgInfo _NamingContext_list_pinfo;
            MarshalBuffer::ArgValue _arg[2];
            NamingContext::Name n;
            _arg[1].u_addr = &n;
            _b.receive(_NamingContext_list_pinfo, _arg);
            NamingContext::BindingInfoList _result = _this->list(n, _env);
            _arg[0].u_addr = &_result;
            _b.reply(_NamingContext_list_pinfo, _arg);
            break;
        }
    }
}
//+

class NamingContextDummy {
public:
    NamingContextDummy();
};

static NamingContextDummy _NamingContext_dummy;

NamingContextDummy::NamingContextDummy() {
    extern TypeObjId TypeIdVar(NamingContext);
    extern TypeObj_Descriptor TypeVar(NamingContext);
    TypeSchemaImpl* s = TypeSchemaImpl::schema();
    s->load(&TypeVar(NamingContext));
    s->receiver(TypeIdVar(NamingContext), &_NamingContext_receive);
}
