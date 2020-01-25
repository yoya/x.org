/* $XConsortium: xkbDflts.h,v 1.4 94/05/11 18:33:27 kaleb Exp $ */
/* THIS FILE GENERATED AUTOMATICALLY BY XKBCOMP */
#ifndef DFLT_H
#define DFLT_H 1

#ifndef XKB_IN_SERVER
#define GET_ATOM(d,s)	XInternAtom(d,s,0)
#define DPYTYPE	Display *
#else
#define GET_ATOM(d,s)	MakeAtom(s,strlen(s),1)
#define DPYTYPE	char *
#endif
#define NUM_KEYS	118

#define	vmod_NumLock	0
#define	vmod_LevelThree	1
#define	vmod_Alt	2
#define	vmod_AltGr	3

#define	vmod_NumLockMask	(1<<0)
#define	vmod_LevelThreeMask	(1<<1)
#define	vmod_AltMask	(1<<2)
#define	vmod_AltGrMask	(1<<3)

/* semantics name is "default" */
static Atom lnames_ONE_LEVEL[1];

static XkbKTMapEntryRec map_TWO_LEVEL[2]= {
    { 1,       ShiftMask,      1,       ShiftMask,               0 },
    { 1,        LockMask,      0,        LockMask,               0 }
};
static XkbKTPreserveRec preserve_TWO_LEVEL[2]= {
    {                 0,               0,               0 },
    {          LockMask,        LockMask,               0 }
};
static Atom lnames_TWO_LEVEL[2];

static XkbKTMapEntryRec map_KEYPAD[3]= {
    { 1,       ShiftMask,      1,       ShiftMask,               0 },
    { 1,       ShiftMask,      1,               0, vmod_NumLockMask },
    { 1,       ShiftMask,      1,       ShiftMask, vmod_NumLockMask }
};
static Atom lnames_KEYPAD[2];

static XkbKTMapEntryRec map_THREE_LEVEL[3]= {
    { 1,       ShiftMask,      1,       ShiftMask,               0 },
    { 1,               0,      2,               0, vmod_LevelThreeMask },
    { 1,       ShiftMask,      2,       ShiftMask, vmod_LevelThreeMask }
};
static Atom lnames_THREE_LEVEL[3];

static XkbKTMapEntryRec map_PC_BREAK[1]= {
    { 1,     ControlMask,      1,     ControlMask,               0 }
};
static Atom lnames_PC_BREAK[2];

static XkbKTMapEntryRec map_PC_SYSRQ[1]= {
    { 1,        Mod1Mask,      1,        Mod1Mask,               0 }
};
static Atom lnames_PC_SYSRQ[2];

static XkbKTMapEntryRec map_CTRL_ALT[1]= {
    { 1, ControlMask|Mod1Mask,      1, ControlMask|Mod1Mask,               0 }
};
static Atom lnames_CTRL_ALT[2];

static XkbKeyTypeRec dflt_types[]= {
    {
	0,
	0,	0,
	1, XkbNoFreeKTStruct|XkbNoFreeKTMap|XkbNoFreeKTLevelNames,
	0,	NULL,	NULL,
	None,	lnames_ONE_LEVEL
    },
    {
	ShiftMask|LockMask,
	ShiftMask|LockMask,	0,
	2, XkbNoFreeKTStruct|XkbNoFreeKTMap|XkbNoFreeKTLevelNames|XkbNoFreeKTPreserve,
	2,	map_TWO_LEVEL,	preserve_TWO_LEVEL,
	None,	lnames_TWO_LEVEL
    },
    {
	ShiftMask,
	ShiftMask,	vmod_NumLockMask,
	2, XkbNoFreeKTStruct|XkbNoFreeKTMap|XkbNoFreeKTLevelNames,
	3,	map_KEYPAD,	NULL,
	None,	lnames_KEYPAD
    },
    {
	ShiftMask,
	ShiftMask,	vmod_LevelThreeMask,
	3, XkbNoFreeKTStruct|XkbNoFreeKTMap|XkbNoFreeKTLevelNames,
	3,	map_THREE_LEVEL,	NULL,
	None,	lnames_THREE_LEVEL
    },
    {
	ControlMask,
	ControlMask,	0,
	2, XkbNoFreeKTStruct|XkbNoFreeKTMap|XkbNoFreeKTLevelNames,
	1,	map_PC_BREAK,	NULL,
	None,	lnames_PC_BREAK
    },
    {
	Mod1Mask,
	Mod1Mask,	0,
	2, XkbNoFreeKTStruct|XkbNoFreeKTMap|XkbNoFreeKTLevelNames,
	1,	map_PC_SYSRQ,	NULL,
	None,	lnames_PC_SYSRQ
    },
    {
	ControlMask|Mod1Mask,
	ControlMask|Mod1Mask,	0,
	2, XkbNoFreeKTStruct|XkbNoFreeKTMap|XkbNoFreeKTLevelNames,
	1,	map_CTRL_ALT,	NULL,
	None,	lnames_CTRL_ALT
    }
};
#define num_dflt_types (sizeof(dflt_types)/sizeof(XkbKeyTypeRec))

static void
initTypeNames(dpy)
DPYTYPE dpy;
{
    dflt_types[0].name= GET_ATOM(dpy,"ONE_LEVEL");
    lnames_ONE_LEVEL[0]=	GET_ATOM(dpy,"Any");
    dflt_types[1].name= GET_ATOM(dpy,"TWO_LEVEL");
    lnames_TWO_LEVEL[0]=	GET_ATOM(dpy,"Base");
    lnames_TWO_LEVEL[1]=	GET_ATOM(dpy,"Shift");
    dflt_types[2].name= GET_ATOM(dpy,"KEYPAD");
    lnames_KEYPAD[0]=	GET_ATOM(dpy,"Base");
    lnames_KEYPAD[1]=	GET_ATOM(dpy,"Number");
    dflt_types[3].name= GET_ATOM(dpy,"THREE_LEVEL");
    lnames_THREE_LEVEL[0]=	GET_ATOM(dpy,"Base");
    lnames_THREE_LEVEL[1]=	GET_ATOM(dpy,"Shift");
    lnames_THREE_LEVEL[2]=	GET_ATOM(dpy,"Level3");
    dflt_types[4].name= GET_ATOM(dpy,"PC_BREAK");
    lnames_PC_BREAK[0]=	GET_ATOM(dpy,"Base");
    lnames_PC_BREAK[1]=	GET_ATOM(dpy,"Control");
    dflt_types[5].name= GET_ATOM(dpy,"PC_SYSRQ");
    lnames_PC_SYSRQ[0]=	GET_ATOM(dpy,"Base");
    lnames_PC_SYSRQ[1]=	GET_ATOM(dpy,"Alt");
    dflt_types[6].name= GET_ATOM(dpy,"CTRL+ALT");
    lnames_CTRL_ALT[0]=	GET_ATOM(dpy,"Base");
    lnames_CTRL_ALT[1]=	GET_ATOM(dpy,"Ctrl+Alt");
}

static XkbSymInterpretRec dfltSI[33]= {
    {    XK_ISO_Level2_Latch, 0x0000,
         XkbSI_LevelOneOnly|XkbSI_Exactly, ShiftMask,
         255,
       { XkbSA_LatchMods,  0x3, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Shift_Lock, 0x0000,
         XkbSI_AnyOf, ShiftMask|LockMask,
         255,
       { XkbSA_LockMods,  0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Num_Lock, 0x0000,
         XkbSI_AnyOf, 0xff,
         0,
       { XkbSA_LockMods,  0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0  } },
    {    XK_ISO_Lock, 0x0000,
         XkbSI_AnyOf, 0xff,
         255,
       { XkbSA_ISOLock,  0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_ISO_Level3_Shift, 0x0000,
         XkbSI_LevelOneOnly|XkbSI_AnyOf, 0xff,
         1,
       { XkbSA_SetMods,  0x1, 0x0, 0x0, 0x0, 0x2, 0x0, 0x0  } },
    {    XK_ISO_Level3_Latch, 0x0000,
         XkbSI_LevelOneOnly|XkbSI_AnyOf, 0xff,
         1,
       { XkbSA_LatchMods,  0x3, 0x0, 0x0, 0x0, 0x2, 0x0, 0x0  } },
    {    XK_Mode_switch, 0x0000,
         XkbSI_LevelOneOnly|XkbSI_AnyOfOrNone, 0xff,
         3,
       { XkbSA_SetGroup,  0x4, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_ISO_Group_Latch, 0x0000,
         XkbSI_LevelOneOnly|XkbSI_AnyOfOrNone, 0xff,
         3,
       { XkbSA_LatchGroup,  0x4, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_ISO_Next_Group, 0x0000,
         XkbSI_LevelOneOnly|XkbSI_AnyOfOrNone, 0xff,
         3,
       { XkbSA_LockGroup,  0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_ISO_Prev_Group, 0x0000,
         XkbSI_LevelOneOnly|XkbSI_AnyOfOrNone, 0xff,
         3,
       { XkbSA_LockGroup,  0x0, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Left, 0x0001,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_MovePtr,  0x0, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Right, 0x0001,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_MovePtr,  0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Up, 0x0001,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_MovePtr,  0x0, 0x0, 0x0, 0xff, 0xff, 0x0, 0x0  } },
    {    XK_Down, 0x0001,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_MovePtr,  0x0, 0x0, 0x0, 0x0, 0x1, 0x0, 0x0  } },
    {    XK_Pointer_Button_Dflt, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_PtrBtn,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_Button1, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_PtrBtn,  0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_Button2, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_PtrBtn,  0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_Button3, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_PtrBtn,  0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_DblClick_Dflt, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_ClickPtrBtn,  0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_DblClick1, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_ClickPtrBtn,  0x0, 0x2, 0x1, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_DblClick2, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_ClickPtrBtn,  0x0, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_DblClick3, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_ClickPtrBtn,  0x0, 0x2, 0x3, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_Drag_Dflt, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_LockPtrBtn,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_Drag1, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_LockPtrBtn,  0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_Drag2, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_LockPtrBtn,  0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_Drag3, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_LockPtrBtn,  0x0, 0x0, 0x3, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_EnableKeys, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_LockControls,  0x0, 0x0, 0x0, 0x0, 0x10, 0x0, 0x0  } },
    {    XK_Pointer_Accelerate, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_LockControls,  0x0, 0x0, 0x0, 0x0, 0x20, 0x0, 0x0  } },
    {    XK_Pointer_DfltBtnNext, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_SetPtrDflt,  0x0, 0x1, 0x1, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Pointer_DfltBtnPrev, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_SetPtrDflt,  0x0, 0x1, 0xff, 0x0, 0x0, 0x0, 0x0  } },
    {    XK_Terminate_Server, 0x0000,
         XkbSI_AnyOfOrNone, 0xff,
         255,
       { XkbSA_Terminate,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0  } },
#ifndef XKB_DDX_PERMANENT_LOCK
    {    NoSymbol, 0x0000,
         XkbSI_Exactly, LockMask,
         255,
       { XkbSA_LockMods,  0x0, 0x2, 0x2, 0x0, 0x0, 0x0, 0x0  } },
#endif
    {    NoSymbol, 0x0000,
         XkbSI_AnyOf, 0xff,
         255,
       { XkbSA_SetMods,  0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0  } }
};
#define num_dfltSI (sizeof(dfltSI)/sizeof(XkbSymInterpretRec))

#endif /* DFLT_H */
