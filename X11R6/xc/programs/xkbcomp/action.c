/* $XConsortium: action.c,v 1.5 94/04/08 15:21:53 erik Exp $ */
/************************************************************
 Copyright (c) 1994 by Silicon Graphics Computer Systems, Inc.

 Permission to use, copy, modify, and distribute this
 software and its documentation for any purpose and without
 fee is hereby granted, provided that the above copyright
 notice appear in all copies and that both that copyright
 notice and this permission notice appear in supporting
 documentation, and that the name of Silicon Graphics not be 
 used in advertising or publicity pertaining to distribution 
 of the software without specific prior written permission.
 Silicon Graphics makes no representation about the suitability 
 of this software for any purpose. It is provided "as is"
 without any express or implied warranty.
 
 SILICON GRAPHICS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
 SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY 
 AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SILICON
 GRAPHICS BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL 
 DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, 
 DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE 
 OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION  WITH
 THE USE OR PERFORMANCE OF THIS SOFTWARE.

 ********************************************************/

#include "xkbcomp.h"
#include "xkbfile.h"
#include "tokens.h"
#include "expr.h"

#include "vmod.h"
#include "action.h"

static Bool	actionsInitialized;
static ExprDef	constTrue;
static ExprDef	constFalse;

/***====================================================================***/

Bool
stringToAction(str,type_rtrn)
    char *	str;
    unsigned *	type_rtrn;
{
    if (str==NULL)
	return False;

    if (uStrCaseCmp(str,"noaction")==0)	 	*type_rtrn= XkbSA_NoAction;
    else if (uStrCaseCmp(str,"setmods")==0)	*type_rtrn= XkbSA_SetMods;
    else if (uStrCaseCmp(str,"latchmods")==0)	*type_rtrn= XkbSA_LatchMods;
    else if (uStrCaseCmp(str,"lockmods")==0)	*type_rtrn= XkbSA_LockMods;
    else if (uStrCaseCmp(str,"setgroup")==0)	*type_rtrn= XkbSA_SetGroup;
    else if (uStrCaseCmp(str,"latchgroup")==0)	*type_rtrn= XkbSA_LatchGroup;
    else if (uStrCaseCmp(str,"lockgroup")==0)	*type_rtrn= XkbSA_LockGroup;
    else if (uStrCaseCmp(str,"moveptr")==0)	*type_rtrn= XkbSA_MovePtr;
    else if (uStrCaseCmp(str,"accelptr")==0)	*type_rtrn= XkbSA_AccelPtr;
    else if (uStrCaseCmp(str,"accelpointer")==0)*type_rtrn= XkbSA_AccelPtr;
    else if (uStrCaseCmp(str,"ptrbtn")==0)	*type_rtrn= XkbSA_PtrBtn;
    else if (uStrCaseCmp(str,"pointerbutton")==0)
						*type_rtrn= XkbSA_PtrBtn;
    else if (uStrCaseCmp(str,"clickptrbtn")==0)	*type_rtrn= XkbSA_ClickPtrBtn;
    else if (uStrCaseCmp(str,"clickpointerbutton")==0)
						*type_rtrn= XkbSA_ClickPtrBtn;
    else if (uStrCaseCmp(str,"clickptrbutton")==0)
						*type_rtrn= XkbSA_ClickPtrBtn;
    else if (uStrCaseCmp(str,"clickpointerbtn")==0)
						*type_rtrn= XkbSA_ClickPtrBtn;
    else if (uStrCaseCmp(str,"lockptrbtn")==0)	*type_rtrn= XkbSA_LockPtrBtn;
    else if (uStrCaseCmp(str,"lockpointerbutton")==0)	
						*type_rtrn= XkbSA_LockPtrBtn;
    else if (uStrCaseCmp(str,"lockptrbutton")==0)	
						*type_rtrn= XkbSA_LockPtrBtn;
    else if (uStrCaseCmp(str,"lockpointerbtn")==0)	
						*type_rtrn= XkbSA_LockPtrBtn;
    else if (uStrCaseCmp(str,"setptrdflt")==0)	*type_rtrn= XkbSA_SetPtrDflt;
    else if (uStrCaseCmp(str,"setpointerdefault")==0)	
						*type_rtrn= XkbSA_SetPtrDflt;
    else if (uStrCaseCmp(str,"isolock")==0)	*type_rtrn= XkbSA_ISOLock;
    else if (uStrCaseCmp(str,"terminate")==0)	*type_rtrn= XkbSA_Terminate;
    else if (uStrCaseCmp(str,"terminateserver")==0)
						*type_rtrn= XkbSA_Terminate;
    else if (uStrCaseCmp(str,"switchscreen")==0)*type_rtrn= XkbSA_SwitchScreen;
    else if (uStrCaseCmp(str,"setcontrols")==0)	*type_rtrn= XkbSA_SetControls;
    else if (uStrCaseCmp(str,"lockcontrols")==0)*type_rtrn= XkbSA_LockControls;
    else if (uStrCaseCmp(str,"actionmessage")==0)*type_rtrn= XkbSA_ActionMessage;
    else if (uStrCaseCmp(str,"messageaction")==0)*type_rtrn= XkbSA_ActionMessage;
    else if (uStrCaseCmp(str,"message")==0)	*type_rtrn= XkbSA_ActionMessage;
    else if (uStrCaseCmp(str,"private")==0)	*type_rtrn= PrivateAction;
    else return False;
    return True;
}

Bool
stringToField(str,field_rtrn)
   char *	str;
   unsigned *	field_rtrn;
{

    if (str==NULL)
	return False;

    if (uStrCaseCmp(str,"clearlocks")==0)	*field_rtrn= F_ClearLocks;
    else if (uStrCaseCmp(str,"latchtolock")==0)	*field_rtrn= F_LatchToLock;
    else if (uStrCaseCmp(str,"genkeyevent")==0)	*field_rtrn= F_GenKeyEvent;
    else if (uStrCaseCmp(str,"generatekeyevent")==0)	
						*field_rtrn= F_GenKeyEvent;
    else if (uStrCaseCmp(str,"report")==0)	*field_rtrn= F_Report;
    else if (uStrCaseCmp(str,"default")==0)	*field_rtrn= F_Default;
    else if (uStrCaseCmp(str,"affect")==0)	*field_rtrn= F_Affect;
    else if (uStrCaseCmp(str,"increment")==0)	*field_rtrn= F_Increment;
    else if (uStrCaseCmp(str,"mods")==0)	*field_rtrn= F_Modifiers;
    else if (uStrCaseCmp(str,"modifiers")==0)	*field_rtrn= F_Modifiers;
    else if (uStrCaseCmp(str,"group")==0)	*field_rtrn= F_Group;
    else if (uStrCaseCmp(str,"x")==0)		*field_rtrn= F_X;
    else if (uStrCaseCmp(str,"y")==0)		*field_rtrn= F_Y;
    else if (uStrCaseCmp(str,"button")==0)	*field_rtrn= F_Button;
    else if (uStrCaseCmp(str,"value")==0)	*field_rtrn= F_Value;
    else if (uStrCaseCmp(str,"controls")==0)	*field_rtrn= F_Controls;
    else if (uStrCaseCmp(str,"type")==0)	*field_rtrn= F_Type;
    else if (uStrCaseCmp(str,"count")==0)	*field_rtrn= F_Count;
    else if (uStrCaseCmp(str,"screen")==0)	*field_rtrn= F_Screen;
    else if (uStrCaseCmp(str,"same")==0)	*field_rtrn= F_Same;
    else if (uStrCaseCmp(str,"sameserver")==0)	*field_rtrn= F_Same;
    else if (uStrCaseCmp(str,"data")==0)	*field_rtrn= F_Data;
    else return False;
    return True;
}

char *
fieldText(field)
   unsigned field;
{
static char buf[32];

    if (field==F_ClearLocks)		strcpy(buf,"clearLocks");
    else if (field==F_LatchToLock)	strcpy(buf,"latchToLock");
    else if (field==F_GenKeyEvent)	strcpy(buf,"genKeyEvent");
    else if (field==F_Report)		strcpy(buf,"report");
    else if (field==F_Default)		strcpy(buf,"default");
    else if (field==F_Affect)		strcpy(buf,"affect");
    else if (field==F_Increment)	strcpy(buf,"increment");
    else if (field==F_Modifiers)	strcpy(buf,"modifiers");
    else if (field==F_Group)		strcpy(buf,"group");
    else if (field==F_X)		strcpy(buf,"x");
    else if (field==F_Y)		strcpy(buf,"y");
    else if (field==F_Button)		strcpy(buf,"button");
    else if (field==F_Value)		strcpy(buf,"value");
    else if (field==F_Controls)		strcpy(buf,"controls");
    else if (field==F_Type)		strcpy(buf,"type");
    else if (field==F_Count)		strcpy(buf,"count");
    else if (field==F_Screen)		strcpy(buf,"screen");
    else if (field==F_Same)		strcpy(buf,"sameServer");
    else if (field==F_Data)		strcpy(buf,"data");
    else 				strcpy(buf,"Unknown");
    return buf;
}

/***====================================================================***/

static Bool
ReportMismatch(action,field,type)
    unsigned		action;
    unsigned		field;
    char *		type;
{
    uError("Value of %s field must be of type %s\n",fieldText(field),type);
    uAction("Action %s definition ignored\n",actionTypeText(action));
    return False;
}

static Bool
ReportIllegal(action,field)
    unsigned		action;
    unsigned		field;
{
    uError("Field %s is not defined for an action of type %s\n",
				fieldText(field),actionTypeText(action));
    uAction("Action definition ignored\n");
    return False;
}

static Bool
ReportNotArray(action,field)
    unsigned		action;
    unsigned		field;
{
    uError("The %s field in the %s action is not an array\n",
				fieldText(field),actionTypeText(action));
    uAction("Action definition ignored\n");
    return False;
}

static Bool
HandleNoAction(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
    return ReportIllegal(action->type,field);
}

static Bool
CheckLatchLockFlags(action,field,value,flags_inout)
    unsigned	action;
    unsigned	field;
    ExprDef *	value;
    unsigned *	flags_inout;
{
unsigned	tmp;
ExprResult	result;

    if (field==F_ClearLocks)		tmp= XkbSA_ClearLocks;
    else if (field==F_LatchToLock)	tmp= XkbSA_LatchToLock;
    else 				return False; /* WSGO! */
    if (!ExprResolveBoolean(value,&result,NULL,NULL))
	return ReportMismatch(action,field,"boolean");
    if (result.uval)	*flags_inout|= tmp;
    else		*flags_inout&= ~tmp;
    return True;
}

static Bool
CheckModifierField(xkb,action,value,flags_inout,mods_rtrn)
    XkbDescPtr		xkb;
    unsigned		action;
    ExprDef *		value;
    unsigned *		flags_inout;
    unsigned *		mods_rtrn;
{
ExprResult	rtrn;

    if (value->op==ExprIdent) {
	register char *valStr;
	valStr= stGetString(value->value.str);
	if (valStr&&((uStrCaseCmp(valStr,"usemodmapmods")==0)||
		     (uStrCaseCmp(valStr,"modmapmods")==0))) {
	
	    *mods_rtrn= 0;
	    *flags_inout|= XkbSA_UseModMapMods;
	    return True;
	}
    }
    if (!ExprResolveModMask(value,&rtrn,LookupVModMask,(XPointer)xkb))
	return ReportMismatch(action,F_Modifiers,"modifier mask");
    *mods_rtrn= rtrn.uval;
    *flags_inout&= ~XkbSA_UseModMapMods;
    return True;
}

static Bool
HandleSetLatchMods(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
XkbModAction *	act;
unsigned	rtrn;
unsigned	t1,t2;

    act= (XkbModAction *)action;
    if (array_ndx!=NULL) {
	switch (field) {
	    case F_ClearLocks: case F_LatchToLock:
	    case F_Modifiers:
		return ReportNotArray(action->type,field);
	}
    }
    switch (field) {
	case F_ClearLocks:
	case F_LatchToLock:
	    rtrn= act->flags;
	    if (CheckLatchLockFlags(action->type,field,value,&rtrn)) {
		act->flags= rtrn;
		return True;
	    }
	    return False;
	case F_Modifiers:
	    t1= act->flags;
	    if (CheckModifierField(xkb,action->type,value,&t1,&t2)) {
		act->flags= t1;
		act->real_mods= act->mask= (t2&0xff);
		t2= (t2>>8)&0xffff;
		XkbSetModActionVMods(act,t2);
		return True;
	    }
	    return False;
    }
    return ReportIllegal(action->type,field);
}

static Bool
HandleLockMods(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
XkbModAction *	act;
ExprResult	rtrn;
unsigned	t1,t2;

    act= (XkbModAction *)action;
    if ((array_ndx!=NULL)&&(field==F_Modifiers))
	return ReportNotArray(action->type,field);
    switch (field) {
	case F_Modifiers:
	    t1= act->flags;
	    if (CheckModifierField(xkb,action->type,value,&t1,&t2)) {
		act->flags= t1;
		act->real_mods= act->mask= (t2&0xff);
		t2= (t2>>8)&0xffff;
		XkbSetModActionVMods(act,t2);
		return True;
	    }
	    return False;
    }
    return ReportIllegal(action->type,field);
}

static LookupEntry groupNames[] = {
	{	"group1",	1	},
	{	"group2",	2	},
	{	"group3",	3	},
	{	"group4",	4	},
	{	"group5",	5	},
	{	"group6",	6	},
	{	"group7",	7	},
	{	"group8",	8	},
	{	NULL,		0	},
};

static Bool
CheckGroupField(action,value,flags_inout,grp_rtrn)
    unsigned		action;
    ExprDef *		value;
    unsigned *		flags_inout;
    int *		grp_rtrn;
{
ExprDef *	spec;
ExprResult 	rtrn;

    if ((value->op==OpNegate)||(value->op==OpUnaryPlus)) {
	*flags_inout&= ~XkbSA_GroupAbsolute;
	spec= value->value.child;
    }
    else {
	*flags_inout|= XkbSA_GroupAbsolute;
	spec= value;
    }

    if (!ExprResolveInteger(spec,&rtrn,SimpleLookup,(XPointer)groupNames))
	return ReportMismatch(action,F_Group,"integer (range 1..8)");
    if ((rtrn.ival<1)||(rtrn.ival>8)) {
	uError("Illegal group %d (must be in the range 1..8)\n",rtrn.ival);
	uAction("Action %s definition ignored\n",actionTypeText(action));
	return False;
    }
    if (value->op==OpNegate)		*grp_rtrn= -rtrn.ival;
    else if (value->op==OpUnaryPlus)	*grp_rtrn= rtrn.ival;
    else				*grp_rtrn= rtrn.ival-1;
    return True;
}

static Bool
HandleSetLatchGroup(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
XkbGroupAction *	act;
unsigned		rtrn;
unsigned		t1;
int			t2;

    act= (XkbGroupAction *)action;
    if (array_ndx!=NULL) {
	switch (field) {
	    case F_ClearLocks: case F_LatchToLock:
	    case F_Group:
		return ReportNotArray(action->type,field);
	}
    }
    switch (field) {
	case F_ClearLocks:
	case F_LatchToLock:
	    rtrn= act->flags;
	    if (CheckLatchLockFlags(action->type,field,value,&rtrn)) {
		act->flags= rtrn;
		return True;
	    }
	    return False;
	case F_Group:
	    t1= act->flags;
	    if (CheckGroupField(action->type,value,&t1,&t2)) {
		act->flags= t1;
		XkbSASetGroup(act,t2);
		return True;
	    }
	    return False;
    }
    return ReportIllegal(action->type,field);
}

static Bool
HandleLockGroup(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
XkbGroupAction *	act;
ExprResult		rtrn;
unsigned		t1;
int			t2;

    act= (XkbGroupAction *)action;
    if ((array_ndx!=NULL) && (field==F_Group))
	return ReportNotArray(action->type,field);
    if (field==F_Group) {
	t1= act->flags;
	if (CheckGroupField(action->type,value,&t1,&t2)) {
	    act->flags= t1;
	    XkbSASetGroup(act,t2);
	    return True;
	}
	return False;
    }
    return ReportIllegal(action->type,field);
}

static Bool
HandleMoveAccelPtr(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
ExprResult	rtrn;
XkbPtrAction *	act;

    act= (XkbPtrAction *)action;
    if ((array_ndx!=NULL)&&((field==F_X)||(field==F_Y)))
	return ReportNotArray(action->type,field);
    if ((field==F_X)||(field==F_Y)) {
	if (!ExprResolveInteger(value,&rtrn,NULL,NULL))
	    return ReportMismatch(action->type,field,"integer");
	if (field==F_X) {
	    XkbSetPtrActionX(act,rtrn.ival);
	}
	else {
	    XkbSetPtrActionY(act,rtrn.ival);
	}
	return True;
    }
    return ReportIllegal(action->type,field);
}

static LookupEntry btnNames[] = {
	{	"button1",	1	},
	{	"button2",	2	},
	{	"button3",	3	},
	{	"button4",	4	},
	{	"button5",	5	},
	{	"default",	0	},
	{	NULL,		0	}
};

static Bool
HandlePtrBtn(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
ExprResult		rtrn;
XkbPtrBtnAction	*	act;

    act= (XkbPtrBtnAction *)action;
    if (field==F_Button) {
	if (array_ndx!=NULL)
	    return ReportNotArray(action->type,field);
	if (!ExprResolveInteger(value,&rtrn,SimpleLookup,(XPointer)btnNames))
	    return ReportMismatch(action->type,field,"integer (range 1..5)");
	if ((rtrn.ival<0)||(rtrn.ival>5)) {
	    uError("Button must specify default or be in the range 1..5\n");
	    uAction("Illegal button value %d ignored\n",rtrn.ival);
	    return False;
	}
	act->button= rtrn.ival;
	return True;
    }
    else if ((action->type==XkbSA_ClickPtrBtn)&&(field==F_Count)) {
	if (array_ndx!=NULL)
	    return ReportNotArray(action->type,field);
	if (!ExprResolveInteger(value,&rtrn,SimpleLookup,(XPointer)btnNames))
	    return ReportMismatch(action->type,field,"integer");
	if ((rtrn.ival<1)||(rtrn.ival>255)) {
	    uError("The count field must have a value in the range 1..255\n");
	    uAction("Illegal count %d ignored\n",rtrn.ival);
	    return False;
	}
	act->count= rtrn.ival;
	return True;
    }
    return ReportIllegal(action->type,field);
}

static LookupEntry ptrDflts[] = {
	{	"dfltbtn",		XkbSA_AffectDfltBtn	},
	{	"defaultbutton",	XkbSA_AffectDfltBtn	},
	{	"button",		XkbSA_AffectDfltBtn	},
	{ 	NULL,			0			}
};

static Bool
HandleSetPtrDflt(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
ExprResult		rtrn;
XkbPtrDfltAction *	act;

    act= (XkbPtrDfltAction *)action;
    if (field==F_Affect) {
	if (array_ndx!=NULL)
	    return ReportNotArray(action->type,field);
	if (!ExprResolveEnum(value,&rtrn,ptrDflts))
	    return ReportMismatch(action->type,field,"pointer component");
	act->affect= rtrn.uval;
	return True;
    }
    else if ((field==F_Button)||(field==F_Value)) {
	ExprDef *btn;
	if (array_ndx!=NULL)
	    return ReportNotArray(action->type,field);
	if ((value->op==OpNegate)||(value->op==OpUnaryPlus))  {
	    act->flags&= ~XkbSA_DfltBtnAbsolute;
	    btn= value->value.child;
	}
	else {
	    act->flags|= XkbSA_DfltBtnAbsolute;
	    btn= value;
	}

	if (!ExprResolveInteger(btn,&rtrn,SimpleLookup,(XPointer)btnNames))
	    return ReportMismatch(action->type,field,"integer (range 1..5)");
	if ((rtrn.ival<0)||(rtrn.ival>5)) {
	    uError("New default button value must be in the range 1..5\n");
	    uAction("Illegal default button value %d ignored\n",rtrn.ival);
	    return False;
	}
	if (rtrn.ival==0) {
	    uError("Cannot set default pointer button to \"default\"\n");
	    uAction("Illegal default button setting ignored\n");
	    return False;
	}
	if (value->op==OpNegate)
	     XkbSASetPtrDfltValue(act,-rtrn.ival);
	else XkbSASetPtrDfltValue(act,rtrn.ival);
	return True;
    }
    return ReportIllegal(action->type,field);
}

static LookupEntry	isoNames[] = {
	{	"mods",		XkbSA_ISONoAffectMods	},
	{	"modifiers",	XkbSA_ISONoAffectMods	},
	{	"group",	XkbSA_ISONoAffectGroup	},
	{	"groups",	XkbSA_ISONoAffectGroup	},
	{	"ptr",		XkbSA_ISONoAffectPtr	},
	{	"pointer",	XkbSA_ISONoAffectPtr	},
	{	"ctrls",	XkbSA_ISONoAffectCtrls	},
	{	"controls",	XkbSA_ISONoAffectCtrls	},
	{	"all",		~((unsigned)0)		},
	{	"none",		0			},
	{	NULL,		0			},
};

static Bool
HandleISOLock(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
ExprResult	rtrn;
XkbISOAction *	act;
unsigned	flags,mods;
int		group;

    act= (XkbISOAction *)action;
    switch (field) {
	case F_Modifiers:
	    if (array_ndx!=NULL)
		return ReportNotArray(action->type,field);
	    flags= act->flags;
	    if (CheckModifierField(xkb,action->type,value,&flags,&mods)) {
		act->flags= flags&(~XkbSA_ISODfltIsGroup);
		act->real_mods= mods&0xff;
		mods= (mods>>8)&0xff;
		XkbSetModActionVMods(act,mods);
		return True;
	    }
	    return False;
	case F_Group:
	    if (array_ndx!=NULL)
		return ReportNotArray(action->type,field);
	    flags= act->flags;
	    if (CheckGroupField(action->type,value,&flags,&group)) {
		act->flags= flags|XkbSA_ISODfltIsGroup;
		XkbSASetGroup(act,group);
		return True;
	    }
	    return False;
	case F_Affect:
	    if (array_ndx!=NULL)
		return ReportNotArray(action->type,field);
	    if (!ExprResolveMask(value,&rtrn,SimpleLookup,(XPointer)isoNames))
		return ReportMismatch(action->type,field,"keyboard component");
	    act->affect= (~rtrn.uval)&XkbSA_ISOAffectMask;
	    return True;
    }
    return ReportIllegal(action->type,field);
}

static Bool
HandleSwitchScreen(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
ExprResult		rtrn;
XkbSwitchScreenAction *	act;

    act= (XkbSwitchScreenAction *)action;
    if (field==F_Screen) {
	ExprDef *scrn;
	if (array_ndx!=NULL)
	    return ReportNotArray(action->type,field);
	if ((value->op==OpNegate)||(value->op==OpUnaryPlus)) {
	    act->flags&= ~XkbSA_SwitchAbsolute;
	    scrn= value->value.child;
	}
	else {
	    act->flags|= XkbSA_SwitchAbsolute;
	    scrn= value;
	}

	if (!ExprResolveInteger(scrn,&rtrn,NULL,NULL))
	    return ReportMismatch(action->type,field,"integer (0..255)");
	if ((rtrn.ival<0)||(rtrn.ival>255)) {
	    uError("Screen index must be in the range 1..255\n");
	    uAction("Illegal screen value %d ignored\n",rtrn.ival);
	    return False;
	}
	if (value->op==OpNegate)
	     XkbSASetScreen(act,-rtrn.ival);
	else XkbSASetScreen(act,rtrn.ival);
	return True;
    }
    else if (field==F_Same) {
	if (array_ndx!=NULL)
	    return ReportNotArray(action->type,field);
	if (!ExprResolveBoolean(value,&rtrn,NULL,NULL))
	    return ReportMismatch(action->type,field,"boolean");
	if (rtrn.uval)	act->flags&= ~XkbSA_SwitchApplication;
	else		act->flags|= XkbSA_SwitchApplication;
	return True;
    }
    return ReportIllegal(action->type,field);
}

LookupEntry	ctrlNames[]= {
	{	"repeatkeys",		XkbRepeatKeysMask	},
	{	"repeat",		XkbRepeatKeysMask	},
	{	"autorepeat",		XkbRepeatKeysMask	},
	{	"slowkeys",		XkbSlowKeysMask		},
	{	"bouncekeys",		XkbBounceKeysMask	},
	{	"stickykeys",		XkbStickyKeysMask	},
	{	"mousekeys",		XkbMouseKeysMask	},
	{	"mousekeysaccel",	XkbMouseKeysAccelMask	},
	{	"accessxkeys",		XkbAccessXKeysMask	},
	{	"accessxtimeout",	XkbAccessXTimeoutMask	},
	{	"accessxfeedback",	XkbAccessXFeedbackMask	},
	{	"groupswrap",		XkbGroupsWrapMask	},
	{	"audiblebell",		XkbAudibleBellMask	},
	{	"autoautorepeat",	XkbAutoAutorepeatMask	},
	{	"all",			XkbAllAccessXMask	},
	{	"none",			0			},
	{	NULL,			0			}
};

static Bool
HandleSetLockControls(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
ExprResult		rtrn;
XkbCtrlsAction *	act;

    act= (XkbCtrlsAction *)action;
    if (field==F_Controls) {
	if (array_ndx!=NULL)
	    return ReportNotArray(action->type,field);
	if (!ExprResolveMask(value,&rtrn,SimpleLookup,(XPointer)ctrlNames))
	    return ReportMismatch(action->type,field,"controls mask");
	XkbActionSetCtrls(act,rtrn.uval);
	return True;
    }
    return ReportIllegal(action->type,field);
}

static LookupEntry evNames[]= {
	{	"press",	XkbSA_MessageOnPress	},
	{	"keypress",	XkbSA_MessageOnPress	},
	{	"release",	XkbSA_MessageOnRelease	},
	{	"keyrelease",	XkbSA_MessageOnRelease	},
	{	"all",		XkbSA_MessageOnPress|XkbSA_MessageOnRelease },
	{	"none",		0			},
	{	NULL,		0			}
};

static Bool
HandleActionMessage(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
ExprResult		rtrn;
XkbMessageAction *	act;

    act= (XkbMessageAction *)action;
    switch (field) {
	case F_Report:
	    if (array_ndx!=NULL)
		return ReportNotArray(action->type,field);
	    if (!ExprResolveMask(value,&rtrn,SimpleLookup,(XPointer)evNames))
		return ReportMismatch(action->type,field,"key event mask");
	    act->flags&= ~(XkbSA_MessageOnPress|XkbSA_MessageOnRelease);
	    act->flags= rtrn.uval&(XkbSA_MessageOnPress|XkbSA_MessageOnRelease);
	    return True;
	case F_GenKeyEvent:
	    if (array_ndx!=NULL)
		return ReportNotArray(action->type,field);
	    if (!ExprResolveBoolean(value,&rtrn,NULL,NULL))
		return ReportMismatch(action->type,field,"boolean");
	    if (rtrn.uval)	act->flags|= XkbSA_MessageGenKeyEvent;
	    else		act->flags&= ~XkbSA_MessageGenKeyEvent;
	    return True;
	case F_Data:  
	    if (array_ndx==NULL) {
		if (!ExprResolveString(value,&rtrn,NULL,NULL))
		    return ReportMismatch(action->type,field,"string");
		else {
		    int len= strlen(rtrn.str);
		    if ((len<1)||(len>6)) {
			uWarning("An action message can hold only 6 bytes\n");
			uAction("Extra %d bytes ignored\n",len-6);
		    }
		    strncpy((char *)act->message,rtrn.str,6);
		}
		return True;
	    }
	    else {
		unsigned ndx;
		if (!ExprResolveInteger(array_ndx,&rtrn,NULL,NULL)) {
		    uError("Array subscript must be integer\n");
		    uAction("Illegal subscript ignored\n");
		    return False;
		}
		ndx= rtrn.uval;
		if (ndx>5) {
		    uError("An action message is at most 6 bytes long\n");
		    uAction("Attempt to use data[%d] ignored\n",ndx);
		    return False;
		}
		if (!ExprResolveInteger(value,&rtrn,NULL,NULL))
		    return ReportMismatch(action->type,field,"integer");
		if ((rtrn.ival<0)||(rtrn.ival>255)) {
		    uError("Message data must be in the range 0..255\n");
		    uAction("Illegal datum %d ignored\n",rtrn.ival);
		    return False;
		}
		act->message[ndx]= rtrn.uval;
	    }
	    return True;
    }
    return ReportIllegal(action->type,field);
}

static Bool
HandlePrivate(xkb,action,field,array_ndx,value)
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
{
ExprResult	rtrn;

    switch (field) {
	case F_Type:
	    if (!ExprResolveInteger(value,&rtrn,NULL,NULL))
		return ReportMismatch(PrivateAction,field,"integer");
	    if ((rtrn.ival<0)||(rtrn.ival>255)) {
		uError("Private action type must be in the range 0..255\n");
		uAction("Illegal type %d ignored\n",rtrn.ival);
		return False;
	    }
	    action->type= rtrn.uval;
	    return True;
	case F_Data:  
	    if (array_ndx==NULL) {
		if (!ExprResolveString(value,&rtrn,NULL,NULL))
		    return ReportMismatch(action->type,field,"string");
		else {
		    int len= strlen(rtrn.str);
		    if ((len<1)||(len>7)) {
			uWarning("A private action has 7 data bytes\n");
			uAction("Extra %d bytes ignored\n",len-6);
			return False;
		    }
		    strncpy((char *)action->data,rtrn.str,7);
		}
		return True;
	    }
	    else {
		unsigned ndx;
		if (!ExprResolveInteger(array_ndx,&rtrn,NULL,NULL)) {
		    uError("Array subscript must be integer\n");
		    uAction("Illegal subscript ignored\n");
		    return False;
		}
		ndx= rtrn.uval;
		if (ndx>6) {
		    uError("The data for a private action is 7 bytes long\n");
		    uAction("Attempt to use data[%d] ignored\n",ndx);
		    return False;
		}
		if (!ExprResolveInteger(value,&rtrn,NULL,NULL))
		    return ReportMismatch(action->type,field,"integer");
		if ((rtrn.ival<0)||(rtrn.ival>255)) {
		    uError("All data for a private action must be 0..255\n");
		    uAction("Illegal datum %d ignored\n",rtrn.ival);
		    return False;
		}
		action->data[ndx]= rtrn.uval;
		return True;
	    }
    }
    return ReportIllegal(PrivateAction,field);
}

typedef	Bool	(*actionHandler)();

static actionHandler	handleAction[XkbSA_NumActions+1] = {
	HandleNoAction			/* NoAction	*/,
	HandleSetLatchMods		/* SetMods	*/,
	HandleSetLatchMods		/* LatchMods	*/,
	HandleLockMods			/* LockMods	*/,
	HandleSetLatchGroup		/* SetGroup	*/,
	HandleSetLatchGroup		/* LatchGroup	*/,
	HandleLockGroup			/* LockGroup	*/,
	HandleMoveAccelPtr		/* MovePtr	*/,
	HandleMoveAccelPtr		/* AccelPtr	*/,
	HandlePtrBtn			/* PtrBtn	*/,
	HandlePtrBtn			/* ClickPtrBtn	*/,
	HandlePtrBtn			/* LockPtrBtn	*/,
	HandleSetPtrDflt		/* SetPtrDflt	*/,
	HandleISOLock			/* ISOLock	*/,
	HandleNoAction			/* Terminate	*/,
	HandleSwitchScreen		/* SwitchScreen	*/,
	HandleSetLockControls		/* SetControls	*/,
	HandleSetLockControls		/* LockControls	*/,
	HandleActionMessage		/* ActionMessage*/,
	HandlePrivate			/* Private	*/
};

/***====================================================================***/

static void
ApplyActionFactoryDefaults(action)
    XkbAction *	action;
{
    if (action->type==XkbSA_ClickPtrBtn) {/* double click */
	action->btn.count= 2;
	action->btn.button= XkbSA_UseDfltButton;
    }
    else if (action->type==XkbSA_SetPtrDflt) { /* increment default button */
	action->dflt.affect= XkbSA_AffectDfltBtn;
	action->dflt.flags= 0;
	XkbSASetPtrDfltValue(&action->dflt,1);
    }
    else if (action->type==XkbSA_ISOLock) {
	action->iso.real_mods=  LockMask;
    }
    return;
}


int 
HandleActionDef(def,xkb,action,mergeMode,info)
    ExprDef *		def;
    XkbDescPtr		xkb;
    XkbAnyAction *	action;
    unsigned		mergeMode;
    ActionInfo *	info;
{
ExprDef *		arg;
ExprDef *		array_ndx;
register char *		str;
unsigned 		tmp,hndlrType;

    if (!actionsInitialized)
	ActionsInit();

    if (def->op!=ExprActionDecl) {
	uError("Expected an action definition, found %s\n",exprOpText(def->op));
	return False;
    }
    str= stGetString(def->value.action.name);
    if (!str) {
	uInternalError("Missing name in action definition!!\n");
	return False;
    }
    if (!stringToAction(str,&tmp)) {
	uError("Unknown action %s\n",str);
	return False;
    }
    action->type= hndlrType= tmp;
    if (action->type!=XkbSA_NoAction) {
	ApplyActionFactoryDefaults((XkbAction *)action);
	while (info) {
	    if ((info->action==XkbSA_NoAction)||(info->action==hndlrType)) {
		if (!(*handleAction[hndlrType])(xkb,action,
						info->field,info->array_ndx,
						info->value)) {
		    return False;
		}
	    }
	    info= info->next;
	}
    }
    for (arg=def->value.action.args;arg!=NULL;arg=(ExprDef*)arg->common.next) {
	ExprDef *field,*value,*arrayRtrn;
	ExprResult elemRtrn,fieldRtrn;
	unsigned fieldNdx;

	if (arg->op==OpAssign) {
	    field= arg->value.binary.left;
	    value= arg->value.binary.right;
	}
	else {
	    if ((arg->op==OpNot)||(arg->op==OpInvert)) {
		field= arg->value.child;
		value= &constFalse;
	    }
	    else {
		field= arg;
		value= &constTrue;
	    }
	}
	if (!ExprResolveLhs(field,&elemRtrn,&fieldRtrn,&arrayRtrn))
	    return False;	/* internal error -- already reported */
	
	if (elemRtrn.str!=NULL) {
	    uError("Cannot change defaults in an action definition\n");
	    uAction("Ignoring attempt to change %s.%s\n",elemRtrn.str,
							 fieldRtrn.str);
	    return False;
	}
	if (!stringToField(fieldRtrn.str,&fieldNdx)) {
	    uError("Unknown field name %s\n",uStringText(fieldRtrn.str));
	    return False;
	}
	if (!(*handleAction[hndlrType])(xkb,action,fieldNdx,arrayRtrn,value)) {
	    return False;
	}
    }
    return True;
}

/***====================================================================***/

int 
SetActionField(xkb,elem,field,array_ndx,value,info_rtrn)
    XkbDescPtr		xkb;
    char *		elem;
    char *		field;
    ExprDef *		array_ndx;
    ExprDef *		value;
    ActionInfo **	info_rtrn;
{
ActionInfo *new,*old;

    if (!actionsInitialized)
	ActionsInit();

    new= uTypedAlloc(ActionInfo);
    if (new==NULL) {
	uInternalError("Couldn't allocate space for action default\n");
	return False;
    }
    if (uStrCaseCmp(elem,"action")==0)
	new->action= XkbSA_NoAction;
    else {
	if (!stringToAction(elem,&new->action))
	    return False;
	if (new->action==XkbSA_NoAction) {
	    uError("\"%s\" is not a valid field in a NoAction action\n",field);
	    return False;
	}
    }
    if (!stringToField(field,&new->field)) {
	uError("\"%s\" is not a legal field name\n",field);
	return False;
    }
    new->array_ndx= array_ndx;
    new->value= value;
    new->next= NULL;
    old= *info_rtrn;
    while ((old)&&(old->next))
	old= old->next;
    if (old==NULL)	*info_rtrn= new;
    else		old->next= new;
    return True;
}

/***====================================================================***/

void
ActionsInit()
{
    if (!actionsInitialized) {
	bzero((char *)&constTrue,sizeof(constTrue));
	bzero((char *)&constFalse,sizeof(constFalse));
	constTrue.common.stmtType= StmtExpr;
	constTrue.common.next= NULL;
	constTrue.op= ExprIdent;
	constTrue.type= TypeBoolean;
	constTrue.value.str= stGetToken("true");
	constFalse.common.stmtType= StmtExpr;
	constFalse.common.next= NULL;
	constFalse.op= ExprIdent;
	constFalse.type= TypeBoolean;
	constFalse.value.str= stGetToken("false");
	actionsInitialized= 1;
    }
    return;
}

