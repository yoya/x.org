/* fgetstr.h : get from FILE* a string not containing TERMINATORS */

/* @(#) fgetstr.h Copyright (c) 1992 Christopher Alexander North-Keys */
/* $Grueppe: Talismangrueppe $ */
/* $Anfang: Sat Aug  8 00:19:24 GMT 1992 $ */
/* $Source: /home/erlkonig/hub/Lib/fgetstr.h $ */
/* $Revision: $ */
/* $Date: Sat Aug  8 00:19:24 GMT 1992 $ */
/* $Author: erlkonig $ */

#ifndef ___fgetstr_h
#define ___fgetstr_h

#include <stdio.h>

#if defined (__STDC__)
#define ARGS(x) x
#else
#define ARGS(x) ()
#endif

extern char *Fgetstr ARGS((FILE *ifp, const char *terminators));

#endif /* ___fgetstr_h - herunten gibts nichts */
