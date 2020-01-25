/* $SonyId: SjXkey.c,v 1.1 1994/05/30 13:03:58 makoto Exp $ */
/******************************************************************

Copyright (c) 1992, 1993, 1994  Sony Corporation

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL SONY CORPORATION BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of Sony Corporation shall
not be used in advertising or otherwise to promote the sale, use or ot
her dealings in this Software without prior written authorization from
Sony Corporation.

******************************************************************/
#include	<malloc.h>
#include	<X11/Xlib.h>
#include	<X11/IntrinsicP.h>
#include	<X11/CoreP.h>
#include	<X11/keysym.h>
#include	<Xmw/TranslateI.h>
#include	"Ximp.h"
#include	"Ximcp.h"

extern Widget	RootText;

XimpKey *
#if NeedFunctionPrototypes
Action2XimpKey( void (*func)() )
#else /* NeedFunctionPrototypes */
Action2XimpKey( func )
    void	(*func)();
#endif /* NeedFunctionPrototypes */
{
    XtTM		tmRecPtr = &((CoreWidget)RootText)->core.tm;
    TMSimpleStateTree	stateTree;
    TMBranchHead	branchHead;
    TMTypeMatch		typeMatch;
    TMModifierMatch	modMatch;
    XtActionProc	*procs;
    TMBindData		bindData;
    ActionRec		*actions;
    int			ii, jj;
    XimpKey		*keys = NULL;
    int			count = 0;

    if( tmRecPtr->translations == NULL )
	return( NULL );
    stateTree = (TMSimpleStateTree)tmRecPtr->translations->stateTreeTbl[0];
    for( ii = 0; ii < stateTree->numBranchHeads; ii++ ) {
	branchHead = &stateTree->branchHeadTbl[ii];
	typeMatch = TMGetTypeMatch(branchHead->typeIndex);
	modMatch = TMGetModifierMatch(branchHead->modIndex);

	bindData = (TMBindData)tmRecPtr->proc_table;
	if( bindData->simple.isComplex ) {
	    TMComplexBindProcs	bindProcs;
	    bindProcs = TMGetComplexBindEntry( bindData, 0 );
	    procs = bindProcs->procs;
	}
	else {
	    TMSimpleBindProcs	bindProcs;
	    bindProcs = TMGetSimpleBindEntry( bindData, 0 );
	    procs = bindProcs->procs;
	}
	if( branchHead->isSimple ) {
	    static ActionRec	dummyAction;
	    dummyAction.idx = TMBranchMore(branchHead);
	    actions = &dummyAction;
	}
	else {
	    StatePtr currState = ((TMComplexStateTree)stateTree)->complexBranchHeadTbl[TMBranchMore(branchHead)];
	    actions = currState->actions;
	}

	while( actions != NULL ) {
	    if( *(procs[actions->idx]) == func  &&
		typeMatch->eventType == KeyPress ) {
		if( modMatch->modifiers == AnyModifier )
		    continue;
		for( jj = 0; jj < count; jj++ )
		    if( keys[jj].keysym == typeMatch->eventCode  &&
			keys[jj].modifier == modMatch->modifiers  &&
			keys[jj].modifier_mask == modMatch->modifierMask )
			goto next;
		count++;
		if( keys == NULL )
		    keys = (XimpKey *)malloc( sizeof(XimpKey) * 2 );
		else
		    keys = (XimpKey *)realloc( keys, sizeof(XimpKey) * (count + 1) );
		if( keys == NULL ) {
		    XtError( "memory allocation error." );
		    exit( 1 );
		}
		keys[count-1].keysym = typeMatch->eventCode;
		keys[count-1].modifier = modMatch->modifiers;
		keys[count-1].modifier_mask = modMatch->modifierMask;
		keys[count].keysym = XK_VoidSymbol;
		break;
	    }
	    actions = actions->next;
	}
next:;
    }
    if( count > 0 )
	for( count = 0; keys[count].keysym != XK_VoidSymbol; count++ )
	    if( keys[count].modifier_mask == ~0 )
		keys[count].modifier_mask = keys[count].modifier;
    return( keys );
}


#if defined(USE_XIMCP)
XIMTRIGGERKEY *
#if NeedFunctionPrototypes
Action2XimcpKey( void (*func)() )
#else /* NeedFunctionPrototypes */
Action2XimcpKey( func )
    void	(*func)();
#endif /* NeedFunctionPrototypes */
{
    XimpKey		*keys, *k;
    KeySym		 keysym;
    unsigned long	 modifier;
    unsigned long	 modifier_mask;

    keys = Action2XimpKey( func );
    if( keys == NULL ) {
	keys = (XimpKey *)malloc( sizeof(XimpKey) );
	((XIMTRIGGERKEY *)keys)->keysym = XK_VoidSymbol;
    }
    else {
	k = keys;
	do {
	    keysym = k->keysym;
	    modifier = k->modifier;
	    modifier_mask = k->modifier_mask;

	    ((XIMTRIGGERKEY *)k)->keysym = keysym;
	    ((XIMTRIGGERKEY *)k)->modifier = modifier;
	    ((XIMTRIGGERKEY *)k)->modifier_mask = modifier_mask;

	    k++;
	} while( keysym != XK_VoidSymbol );
    }

    return( (XIMTRIGGERKEY *)keys );
}
#endif
