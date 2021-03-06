/*
 * Copyright (c) 1991-1994  Sony Corporation
 * 
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * 
 * Except as contained in this notice, the name of Sony Corporation
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * from Sony Corporation.
 *
 */

/*
 * $SonyRCSfile: file.c,v $  
 * $SonyRevision: 1.1 $ 
 * $SonyDate: 1994/06/03 08:00:34 $
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

char	*Malloc();

typedef	struct filelist {
	FILE	*fp;
	char	*fname;
	struct filelist *next;
} FileList;

static	FileList *flist = NULL;

static	char	*get_fname(fp)
FILE	*fp;
{
	FileList *p;

	for (p = flist ; p ; p = p -> next)
		if (p -> fp == fp) return p -> fname;
	return "some file";
}

FILE	*Fopen(filename, type)
char	*filename, *type;
{
	FILE	*fp;
	char	*p;
	FileList *fl;

	if (!(fp = fopen(filename, type))) {
		fprintf(stderr, "Can't open %s mode \"%s\"\n", filename, type);
		exit(1);
	}

	fl = (FileList *)Malloc(sizeof(*fl));
	fl -> fname = Malloc(strlen(filename) + 1);
	strcpy(fl -> fname, filename);
	fl -> fp = fp;
	fl -> next = flist;

	return fp;
}

Fclose(fp)
FILE	*fp;
{
	FileList *p, *q;

	if (fclose(fp) == EOF) {
		fprintf(stderr, "Close error in %s\n", get_fname(fp));
		exit(1);
	}

	for (p = flist, q = NULL ; p ; q = p, p = p -> next) {
		if (p -> fp == fp) {
			if (q)
				q -> next = p -> next;
			else
				flist = p -> next;
			free(p -> fname);
			free(p);
			break;
		}
	}
}

Fseek(fp, ofs, ptr)
FILE	*fp;
long	ofs;
int	ptr;
{
	if (fseek(fp, ofs, ptr) == 0) return 0;

	fprintf(stderr, "Seek error in %s\n", get_fname(fp));
	exit(1);
}

Ftell(fp)
FILE	*fp;
{
	return ftell(fp);
}

Fsize(filename)
char	*filename;
{
	struct	stat	buf;

	if (stat(filename, &buf) == 0) return buf.st_size;

	fprintf(stderr, "Stat error in %s\n", filename);
	exit(1);
}

Fread(p, s, n, fp)
char	*p;
int	s, n;
FILE	*fp;
{
	if (fread(p, s, n, fp) == n) return n;

	fprintf(stderr, "Read error in %s\n", get_fname(fp));
	exit(1);
}

Fwrite(p, s, n, fp)
char	*p;
int	s, n;
FILE	*fp;
{
	if (fwrite(p, s, n, fp) == n) return n;

	fprintf(stderr, "Write error in %s\n", get_fname(fp));
	exit(1);
}

Fgetc(fp)
FILE	*fp;
{
	return fgetc(fp);
}

Fflush(fp)
FILE    *fp;
{
        fflush(fp);
}    
