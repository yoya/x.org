/* $Id: CcWnnP.h,v 1.8 1991/09/30 10:55:22 ishisone Rel $ */
/*
 * Copyright (c) 1990  Software Research Associates, Inc.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Software Research Associates not be
 * used in advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.  Software Research
 * Associates makes no representations about the suitability of this software
 * for any purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * Author:  Makoto Ishisone, Software Research Associates, Inc., Japan
 */

#ifndef _CcWnnP_h
#define _CcWnnP_h

#include "InputConvP.h"

#include <jslib.h>
#undef wchar
#include "WStr.h"
#include "CcWnn.h"
#include "jclib.h"
#include "jilib.h"
#include "cconv.h"

typedef struct {
    int foo;
} CcWnnClassPart;

typedef struct _CcWnnClassRec {
    ObjectClassPart object_class;
    InputConvClassPart inputConv_class;
    CcWnnClassPart ccWnn_class;
} CcWnnClassRec;

typedef enum {
    normal_state,
    selection_s_state,	/* $B>.J8@a8uJdA*Br%b!<%I(J */
    selection_l_state,	/* $BBgJ8@a8uJdA*Br%b!<%I(J */
    symbol_state	/* $B5-9fF~NO%b!<%I(J */
} CcWnnState;

typedef struct {
    /* resources */
    int		(*confirmfunc)();
    XtPointer	confirmdata;
    String	jservername;
    String	jservername2;		/* secondary jserver */
    String	wnnenvname;
    String	wnnenvrcfile;
    Boolean	wnnoverrideenv;
    String	ccdeffile;
    struct wnn_env	*wnnenv;
    ccRule	ccrule;
    int		saveinterval;
    /* private data */
    ccBuf	ccbuf;
    jcConvBuf	*jcbuf;
    Boolean	createrule;	/* ccrule $B$r<+J,$G:n$C$?$+30$+$iM?$($i$l$?$+(J */
    Boolean	createenv;	/* wnnenv $B$r<+J,$G:n$C$?$+30$+$iM?$($i$l$?$+(J */
    CcWnnState	state;		/* $BJQ49$NFbIt>uBV(J */
    Boolean	textchanged;	/* $BJQ49%F%-%9%H$,JQ$o$C$?$+(J */
    Boolean	selectionending;/* $BA*Br%b!<%I$r=*N;$7$h$&$H$7$F$$$k$+(J */
    ICString	*symbollist;
    int		numsymbols;
    int		cursymbol;	/* $B5-9fA*Br%b!<%I$N;~!"8=:_A*Br$5$l$F$$$k5-9f(J */
    ICString	*candlist;
    int		candlistsize;
    wchar	*strdata;
    int		strdatasize;
    int		numcand;	/* $BA*Br%b!<%I$N;~!"8uJd?t(J */
    int		curcand;	/* $B8uJdA*Br%b!<%I$N;~!"8=:_A*Br$5$l$F$$$k8uJd(J */
    int		fixcount;
    enum {JIS_MODE, KUTEN_MODE, OTHERS} inputmode;
    wchar	*pendingdata;
} CcWnnPart;

typedef struct _CcWnnRec {
    ObjectPart  object;
    InputConvPart inputConv;
    CcWnnPart ccWnn;
} CcWnnRec;

#endif
