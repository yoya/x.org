/*
 * $XConsortium: os.cxx,v 1.12 94/08/17 21:48:05 matt Exp $
 */

/*
 * Copyright (c) 1991 Stanford University
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

#include <X11/Fresco/Impls/fresco-impl.h>
#include <X11/Fresco/OS/directory.h>
#include <X11/Fresco/OS/file.h>
#include <X11/Fresco/OS/host.h>
#include <X11/Fresco/OS/list.h>
#include <X11/Fresco/OS/math.h>
#include <X11/Fresco/OS/memory.h>
#include <X11/Fresco/OS/table.h>
#include <X11/Fresco/OS/types.h>
#include <X11/Fresco/Ox/exchange.h>

/*
 * BSD tends to have things in <sys/dir.h>, System V uses <dirent.h>.
 * So far as I can tell, POSIX went with <dirent.h>.  Ultrix <dirent.h>
 * includes <sys/dir.h>, which is silly because <sys/dir.h>
 * needs <sys/types.h>.
 */
#ifdef apollo
#include <sys/dir.h>
#include <osfcn.h>
#else
#include <dirent.h>
#endif
#include <pwd.h>

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#if !defined(__osf__) && !defined(__hpux) && !defined(__GNUC__)
#include <sysent.h>		/* DEC, HP and g++ don't have sysent.h */
#endif
#include <unistd.h>

#ifndef S_ISDIR
#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif

/*
 * Most systems have gethostname, although not all of them
 * provide a prototype in one of the standard places.  A few
 * systems lack it altogether, in which case you have to use
 * either uname or sysinfo.  Note that POSIX says uname need
 * not return enough information to interface to the network
 * (for example, it may return a nodename of "foo" instead
 * of the canonical "foo.domain.name"), so you're better off
 * using gethostname if you can
 */

#if defined(SVR4) && (defined(sun) || defined(sony))
#define NEED_UTSNAME	/* avoid UCB compatiblity package */
#include <sys/utsname.h>
#endif

#if !defined(NEED_UTSNAME) 
#if defined(AIXV3) || defined(__osf__) || defined(__GNUC__)
extern "C" {
    int gethostname(char *, int);
}
#endif
#endif

/*
 * Buffer size for internal path name computation.
 * The path stuff should really be reimplemented
 * with variable-length strings.
 */

static const int path_buffer_size = 1024 + 1;

class DirectoryEntry {
public:
    const char* name() const;
private:
    friend class Directory;
    friend class DirectoryImpl;

    char* name_;
    struct stat* info_;
};

inline const char* DirectoryEntry::name() const { return name_; }

class DirectoryImpl {
private:
    friend class Directory;

    DirectoryImpl(DIR*, const char*);
    ~DirectoryImpl();

    char* name_;
    DIR* dir_;
    DirectoryEntry* entries_;
    int count_;
    int used_;
    Boolean filled_;

    DirectoryImpl& filled();
    void do_fill();

    static Boolean dot_slash(const char*);
    static Boolean dot_dot_slash(const char*);
    static const char* home(const char*);
    static const char* eliminate_dot(const char*);
    static Boolean collapsed_dot_dot_slash(char*, char*& start);
    static const char* eliminate_dot_dot(const char*);
    static const char* interpret_slash_slash(const char*);
    static const char* interpret_tilde(const char*);
    static const char* expand_tilde(const char*, int);
    static const char* real_path(const char*);
    static Boolean ifdir(const char*);
};

Directory::Directory() {
    impl_ = nil;
}

Directory::~Directory() {
    close();
    delete impl_;
}

Directory* Directory::current() {
    return open(".");
}

Directory* Directory::open(const char* name) {
    char* s = canonical(name);
    DIR* dir = opendir(s);
    if (dir == nil) {
	delete s;
	return nil;
    }
    Directory* d = new Directory;
    d->impl_ = new DirectoryImpl(dir, s);
    return d;
}

void Directory::close() {
    DirectoryImpl& d = *impl_;
    if (d.dir_ != nil) {
	closedir(d.dir_);
	DirectoryEntry* end = &d.entries_[d.used_];
	for (DirectoryEntry* e = &d.entries_[0]; e < end; e++) {
	    delete e->name_;
	    delete e->info_;
	}
	delete d.entries_;
	d.dir_ = nil;
    }
}

const char* Directory::path() const {
    DirectoryImpl& d = *impl_;
    return d.name_;
}

int Directory::count() const {
    DirectoryImpl& d = impl_->filled();
    return d.used_;
}

const char* Directory::name(int i) const {
    DirectoryImpl& d = impl_->filled();
    if (i < 0 || i >= d.count_) {
	/* raise exception -- out of range */
	return nil;
    }
    return d.entries_[i].name_;
}

int Directory::index(const char* name) const {
    DirectoryImpl& d = impl_->filled();
    int i = 0, j = d.used_ - 1;
    while (i <= j) {
	int k = (i + j) / 2;
	int cmp = strcmp(name, d.entries_[k].name_);
	if (cmp == 0) {
	    return k;
	}
	if (cmp > 0) {
	    i = k + 1;
	} else {
	    j = k - 1;
	}
    }
    return -1;
}

Boolean Directory::is_directory(int i) const {
    DirectoryImpl& d = impl_->filled();
    if (i < 0 || i >= d.count_) {
	/* raise exception -- out of range */
	return false;
    }
    DirectoryEntry& e = d.entries_[i];
    if (e.info_ == nil) {
	e.info_ = new (struct stat);
	char* tmp = new char[strlen(d.name_) + strlen(e.name_) + 2];
	sprintf(tmp, "%s/%s", d.name_, e.name_);
	stat(tmp, e.info_);
	delete tmp;
    }
    return S_ISDIR(e.info_->st_mode);
}

inline Boolean DirectoryImpl::dot_slash(const char* path) {
    return path[0] == '.' && (path[1] == '/' || path[1] == '\0');
}

inline Boolean DirectoryImpl::dot_dot_slash(const char* path) {
    return (path[0] == '.' && path[1] == '.' &&
	(path[2] == '/' || path[2] == '\0')
    );
}

char* Directory::canonical(const char* name) {
    const char* path = name;
    static char newpath[path_buffer_size];
    const char* s = DirectoryImpl::interpret_slash_slash(path);
    s = DirectoryImpl::eliminate_dot(s);
    s = DirectoryImpl::eliminate_dot_dot(s);
    s = DirectoryImpl::interpret_tilde(s);
    if (s[0] == '\0') {
	sprintf(newpath, "./");
    } else if (!DirectoryImpl::dot_slash(s) &&
	!DirectoryImpl::dot_dot_slash(s) && s[0] != '/'
    ) {
	sprintf(newpath, "./%s", s);
    } else if (DirectoryImpl::ifdir(s) && s[strlen(s) - 1] != '/') {
	sprintf(newpath, "%s/", s);
    } else {
	sprintf(newpath, "%s", s);
    }
    int n = strlen(newpath);
    char* r = new char[n + 1];
    strcpy(r, newpath);
    return r;
}

Boolean Directory::match(const char* name, const char* pattern) {
    const char* s = name;
    const char* end_s = s + strlen(name);
    const char* p = pattern;
    const char* end_p = p + strlen(pattern);
    for (; p < end_p; p++, s++) {
	if (*p == '*') {
	    const char* pp = p + 1;
	    if (pp == end_p) {
		return true;
	    }
	    for (; s < end_s && *s != *pp; s++);
	    p = pp;
	} else if (s == end_s || *p != *s) {
	    return false;
	}
    }
    return s == end_s;
}

/** class DirectoryImpl **/

DirectoryImpl::DirectoryImpl(DIR* d, const char* name) {
    int n = strlen(name);
    name_ = new char[n + 1];
    strcpy(name_, name);
    dir_ = d;
    entries_ = nil;
    count_ = 0;
    used_ = 0;
    filled_ = false;
}

DirectoryImpl::~DirectoryImpl() {
    delete name_;
}

DirectoryImpl& DirectoryImpl::filled() {
    if (!filled_) {
	do_fill();
	filled_ = true;
    }
    return *this;
}

static int compare_entries(const void* k1, const void* k2) {
    DirectoryEntry* e1 = (DirectoryEntry*)k1;
    DirectoryEntry* e2 = (DirectoryEntry*)k2;
    return strcmp(e1->name(), e2->name());
}

void DirectoryImpl::do_fill() {
    unsigned int overflows = 0;
#ifdef apollo
    for (struct direct* d = readdir(dir_); d != nil; d = readdir(dir_)) {
#else
    for (struct dirent* d = readdir(dir_); d != nil; d = readdir(dir_)) {
#endif
	if (used_ >= count_) {
	    ++overflows;
	    int new_count = count_ + 50*overflows;
	    DirectoryEntry* new_entries = new DirectoryEntry[new_count];
	    Memory::copy(
		entries_, new_entries, count_ * sizeof(DirectoryEntry)
	    );
	    delete entries_;
	    entries_ = new_entries;
	    count_ = new_count;
	}
	DirectoryEntry& e = entries_[used_];
	int n = strlen(d->d_name);
	e.name_ = new char[n + 1];
	strcpy(e.name_, d->d_name);
	e.info_ = nil;
	++used_;
    }
    qsort(entries_, used_, sizeof(DirectoryEntry), &compare_entries);
}

const char* DirectoryImpl::home(const char* name) {
    struct passwd* pw;
    if (name == nil) {
	pw = getpwuid(getuid());
    } else {
	pw = getpwnam(name);
    }
    return (pw == nil) ? nil : pw->pw_dir;
}

const char* DirectoryImpl::eliminate_dot(const char* path) {
    static char newpath[path_buffer_size];
    const char* src;
    char* dest = newpath;

    const char* end = &path[strlen(path)];
    for (src = path; src < end; src++) {
	if (dot_slash(src) && dest > newpath && *(dest - 1) == '/') {
	    src++;
	} else {
	    *dest++ = *src;
	}
    }
    *dest = '\0';
    return newpath;
}

Boolean DirectoryImpl::collapsed_dot_dot_slash(char* path, char*& start) {
    if (path == start || *(start - 1) != '/') {
	return false;
    }
    if (path == start - 1 && *path == '/') {
	return true;
    }
    if (path == start - 2) {	/* doesn't handle double-slash correctly */
	start = path;
	return *start != '.';
    }
    if (path < start - 2 && !dot_dot_slash(start - 3)) {
	for (start -= 2; path <= start; --start) {
	    if (*start == '/') {
		++start;
		return true;
	    }
	}
	start = path;
	return true;
    }
    return false;
}

const char* DirectoryImpl::eliminate_dot_dot(const char* path) {
    static char newpath[path_buffer_size];
    const char* src;
    char* dest = newpath;

    const char* end = &path[strlen(path)];
    for (src = path; src < end; src++) {
	if (dot_dot_slash(src) && collapsed_dot_dot_slash(newpath, dest)) {
	    src += 2;
	} else {
	    *dest++ = *src;
	}
    }
    *dest = '\0';
    return newpath;
}

const char* DirectoryImpl::interpret_slash_slash(const char* path) {
    for (int i = strlen(path) - 1; i > 0; --i) {
	if (path[i] == '/' && path[i - 1] == '/') {
	    return &path[i];
	}
    }
    return path;
}

const char* DirectoryImpl::interpret_tilde(const char* path) {
    static char realpath[path_buffer_size];
    const char* beg = strrchr(path, '~');
    Boolean valid = (beg != nil && (beg == path || *(beg - 1) == '/'));
    if (valid) {
	const char* end = strchr(beg, '/');
	int length = (end == nil) ? strlen(beg) : (end - beg);
	const char* expanded = expand_tilde(beg, length);
	if (expanded == nil) {
	    valid = false;
	} else {
	    strcpy(realpath, expanded);
	    if (end != nil) {
		strcat(realpath, end);
	    }
	}
    }
    return valid ? realpath : path;
}

const char* DirectoryImpl::expand_tilde(const char* tilde, int length) {
    const char* name = nil;
    if (length > 1) {
	static char buf[path_buffer_size];
	strncpy(buf, tilde + 1, length - 1);
	buf[length - 1] = '\0';
	name = buf;
    }
    return home(name);
}

const char* DirectoryImpl::real_path(const char* path) {
    const char* realpath;
    if (*path == '\0') {
	realpath = "./";
    } else {
	realpath = interpret_tilde(interpret_slash_slash(path));
    }
    return realpath;
}

Boolean DirectoryImpl::ifdir(const char* path) {
    struct stat st;
    return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

/* class File */

#ifdef __sgi
#include <sys/mman.h>
#endif

/* apollo doesn't put read in one of the standard places? */
extern "C" {
#if defined(apollo)
    extern long read(int, void*, unsigned int);
#endif
}

class FileInfo {
public:
    char* name_;
    int fd_;
    char* map_;
    struct stat info_;
    off_t pos_;
    char* buf_;
    unsigned int limit_;

    FileInfo(const char*, int fd);
};

FileInfo::FileInfo(const char* s, int fd) {
    int n = strlen(s);
    name_ = new char[n + 1];
    strcpy(name_, s);
    fd_ = fd;
    pos_ = 0;
    limit_ = 0;
    map_ = nil;
    buf_ = nil;
}

File::File(FileInfo* i) {
    rep_ = i;
}

File::~File() {
    close();
    delete rep_->name_;
    delete rep_;
}

const char* File::name() const {
    return rep_->name_;
}

long File::length() const {
    return rep_->info_.st_size;
}

void File::close() {
    FileInfo* i = rep_;
    if (i->fd_ >= 0) {
	if (i->map_ != nil) {
#ifdef __sgi
	    munmap(i->map_, int(i->info_.st_size));
#endif
	}
	if (i->buf_ != nil) {
	    delete i->buf_;
	}
	::close(i->fd_);
	i->fd_ = -1;
    }
}

void File::limit(unsigned int buffersize) {
    rep_->limit_ = buffersize;
}

FileInfo* File::rep() const { return rep_; }

/* class InputFile */

InputFile::InputFile(FileInfo* i) : File(i) { }
InputFile::~InputFile() { }

InputFile* InputFile::open(const char* name) {
    /* cast to workaround DEC C++ prototype bug */
    int fd = ::open((char*)name, O_RDONLY);
    if (fd < 0) {
	return nil;
    }
    FileInfo* i = new FileInfo(name, fd);
    if (fstat(fd, &i->info_) < 0) {
	delete i;
	return nil;
    }
    return new InputFile(i);
}

long InputFile::read(const char*& start) {
    FileInfo* i = rep();
    long len = i->info_.st_size;
    if (i->pos_ >= len) {
	return 0;
    }
    if (i->limit_ != 0 && len > i->limit_) {
	len = i->limit_;
    }
#ifdef __sgi
    i->map_ = (char*)mmap(
	0, (int)len, PROT_READ, MAP_PRIVATE, i->fd_, i->pos_
    );
    if ((int)(i->map_) == -1) {
	return -1;
    }
    start = i->map_;
#else
    if (i->buf_ == nil) {
	i->buf_ = new char[len];
    }
    start = i->buf_;
    len = ::read(i->fd_, i->buf_, size_t(len));
#endif
    i->pos_ += len;
    return len;
}

/* class StdInput */

StdInput::StdInput() : InputFile(new FileInfo("-stdin", 0)) { }
StdInput::~StdInput() { }

long StdInput::length() const { return -1; }

long StdInput::read(const char*& start) {
    FileInfo* i = rep();
    if (i->buf_ == nil) {
	if (i->limit_ == 0) {
	    i->limit_ = BUFSIZ;
	}
	i->buf_ = new char[i->limit_];
    }
    long nbytes = ::read(i->fd_, (char*)i->buf_, i->limit_);
    if (nbytes > 0) {
	start = (const char*)(i->buf_);
    }
    return nbytes;
}

/* class Host */

char Host::name_[100];

const char* Host::name() {
    if (name_[0] == '\0') {
#ifdef NEED_UTSNAME
	struct utsname name;
	uname(&name);
	int len = strlen(name.nodename);
	if (len >= sizeof(name_)) {
	    len = sizeof(name_) - 1;
	}
	strncpy(name_, name.nodename, len);
	name_[len] = '\0';
#else
	gethostname(name_, sizeof(name_));
#endif
    }
    return name_;
}

/*
 * template<T> class List<T>
 */

implementList(__AnyPtrList,__AnyPtr)

static long ListImpl_best_new_sizes[] = {
    48, 112, 240, 496, 1008, 2032, 4080, 8176,
    16368, 32752, 65520, 131056, 262128, 524272, 1048560,
    2097136, 4194288, 8388592, 16777200, 33554416, 67108848
};

long ListImpl_best_new_count(long count, unsigned int size) {
    for (int i = 0; i < sizeof(ListImpl_best_new_sizes)/sizeof(long); i++) {
        if (count * size < ListImpl_best_new_sizes[i]) {
            return ListImpl_best_new_sizes[i] / size;
        }
    }
    return count;
}

void ListImpl_range_error(long i) {
    fprintf(stderr, "internal error: list index %d out of range\n", i);
    abort();
}

/* class Math */

/* would that these lived in a standard place ... */
extern "C" {
    extern double fabs(double);
}

int Math::abs(int x) { return x >= 0 ? x : -x; }
long Math::abs(long x) { return x >= 0 ? x : -x; }
double Math::abs(double x) { return ::fabs(x); }

/* class Memory */

void Memory::copy(const void* from, void* to, unsigned long nbytes) {
#if defined(sun) && !defined(SVR4)
    bcopy(from, to, nbytes);
#else
    memmove(to, from, size_t(nbytes));
#endif
}

int Memory::compare(const void* b1, const void* b2, unsigned long nbytes) {
    return memcmp(b1, b2, size_t(nbytes)) != 0;
}

void Memory::zero(void* b, unsigned long nbytes) {
    memset(b, 0, size_t(nbytes));
}
