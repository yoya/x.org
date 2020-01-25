/* $XConsortium: indicators.c,v 1.1 94/04/08 15:31:39 erik Exp $ */
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

void
ClearIndicatorMapInfo(dpy,info)
    Display *		dpy;
    XkbFileLEDInfo *	info;
{
    bzero((char *)info,sizeof(XkbFileLEDInfo));
    info->name= XkbInternAtom(dpy,"default",False);
    info->indicator= XkbFileLEDNotBound;
    return;
}

XkbFileLEDInfo *
AddIndicatorMap(newLED,oldLEDs,merge,report)
    XkbFileLEDInfo *	newLED;
    XkbFileLEDInfo *	oldLEDs;
    unsigned		merge;
    Bool		report;
{
XkbFileLEDInfo *led,*last;

    last= NULL;
    for (led=oldLEDs;led!=NULL;led=led->next) {
	if (led->name==newLED->name) {
	    if ((led->real_mods==newLED->real_mods)&&
		(led->vmods==newLED->vmods)&&(led->groups==newLED->groups)&&
		(led->ctrls==newLED->ctrls)&&
		(led->which_mods==newLED->which_mods)&&
		(led->which_groups==newLED->which_groups))
		return oldLEDs;
	    if ((report)&&(warningLevel>3)) {
		uWarning("Map for indicator %s redefined\n",stText(led->name));
		uAction("Using %s definition\n",
					(merge==MergeAugment?"first":"last"));
	    }
	    if (merge!=MergeAugment) {
		XkbFileLEDInfo *next= led->next;
		*led= *newLED;
		led->next= next;
	    }
	    return oldLEDs;
	}
	if (led->next==NULL)
	    last= led;
    }
    /* new definition */
    led= uTypedAlloc(XkbFileLEDInfo);
    if (!led) {
	uInternalError("Couldn't allocate indicator map\n");
	uAction("Map for indicator %s not compiled\n",stText(newLED->name));
	return False;
    }
    *led= *newLED;
    led->next= NULL;
    if (last) {
	last->next= led;
	return oldLEDs;
    }
    return led;
}

static int
ReportNotArray(type,field,name)
    char *	type;
    char *	field;
    char *	name;
{
    uError("The %s %s field is not an array\n",type,field);
    uAction("Ignoring illegal assignment in definition of %s\n",name);
    return False;
}

static int
ReportBadType(type,field,name,wanted)
    char *	type;
    char *	field;
    char *	name;
    char *	wanted;
{
    uError("The %s %s field must be a %s\n",type,field,wanted);
    uAction("Ignoring illegal assignment in definition of %s\n",name);
    return False;
}

LookupEntry	stateComponentNames[] = {
	{	"base",		XkbIM_UseBase		},
	{	"latched",	XkbIM_UseLatched	},
	{	"locked",	XkbIM_UseLocked		},
	{	"effective",	XkbIM_UseEffective	},
	{	"compat",	XkbIM_UseCompat		},
	{	"any",		XkbIM_UseAnyState	},
	{	"none",		0			},
	{	NULL,		0			}
};

int
SetIndicatorMapField(led,xkb,field,arrayNdx,value,merge)
    XkbFileLEDInfo *	led;
    XkbDescPtr		xkb;
    char *		field;
    ExprDef *		arrayNdx;
    ExprDef *		value;
    unsigned		merge;
{
ExprResult	rtrn;
Bool		ok;

    ok= True;
    if ((uStrCaseCmp(field,"modifiers")==0)||(uStrCaseCmp(field,"mods")==0)) {
	if (arrayNdx!=NULL)
	    return ReportNotArray("indicator map",field,stText(led->name));
	if (!ExprResolveModMask(value,&rtrn,LookupVModMask,(XPointer)xkb))
	    return ReportBadType("indicator map",field,stText(led->name),
							"modifier mask");
	led->real_mods= rtrn.uval&0xff;
	led->vmods= (rtrn.uval>>8)&0xff;
    }
    else if (uStrCaseCmp(field,"groups")==0) {
	extern LookupEntry	groupNames[];
	if (arrayNdx!=NULL)
	    return ReportNotArray("indicator map",field,stText(led->name));
	if (!ExprResolveMask(value,&rtrn,SimpleLookup,groupNames))
	    return ReportBadType("indicator map",field,stText(led->name),
							"group mask");
	led->groups= rtrn.uval;
    }
    else if ((uStrCaseCmp(field,"controls")==0)||
	     (uStrCaseCmp(field,"ctrls")==0)) {
	extern LookupEntry ctrlNames[];
	if (arrayNdx!=NULL)
	    return ReportNotArray("indicator map",field,stText(led->name));
	if (!ExprResolveMask(value,&rtrn,SimpleLookup,(XPointer)ctrlNames))
	    return ReportBadType("indicator map",field,stText(led->name),
							"controls mask");
	led->ctrls= rtrn.uval;
    }
    else if (uStrCaseCmp(field,"allowexplicit")==0) {
	if (arrayNdx!=NULL)
	    return ReportNotArray("indicator map",field,stText(led->name));
	if (!ExprResolveBoolean(value,&rtrn,NULL,NULL))
	    return ReportBadType("indicator map",field,stText(led->name),
							"boolean");
	if (rtrn.uval)	led->flags&= ~XkbIM_NoExplicit;
	else		led->flags|=  XkbIM_NoExplicit;
    }
    else if ((uStrCaseCmp(field,"whichmodstate")==0)||
	     (uStrCaseCmp(field,"whichmodifierstate")==0)) {
	if (arrayNdx!=NULL)
	    return ReportNotArray("indicator map",field,stText(led->name));
	if (!ExprResolveMask(value,&rtrn,SimpleLookup,stateComponentNames))
	    return ReportBadType("indicator map",field,stText(led->name),
						"mask of state components");
	led->which_mods= rtrn.uval;
    }
    else if (uStrCaseCmp(field,"whichgroupstate")==0) {
	if (arrayNdx!=NULL)
	    return ReportNotArray("indicator map",field,stText(led->name));
	if (!ExprResolveMask(value,&rtrn,SimpleLookup,stateComponentNames))
	    return ReportBadType("indicator map",field,stText(led->name),
						"mask of state components");
	if ((rtrn.uval&XkbIM_UseCompat)&&(warningLevel>0)) {
	    uWarning("Cannot use the compatibilty state for groups\n");
	    uAction("Not set in the %s field of the map for indicator %s\n",
						field,stText(led->name));
	    rtrn.uval&= ~XkbIM_UseCompat;
	}
	led->which_groups= rtrn.uval;
    }
    else if (uStrCaseCmp(field,"index")==0) {
	if (arrayNdx!=NULL)
	    return ReportNotArray("indicator map",field,stText(led->name));
	if (!ExprResolveInteger(value,&rtrn,NULL,NULL))
	    return ReportBadType("indicator map",field,stText(led->name),
						"indicator number");
	if ((rtrn.uval<1)||(rtrn.uval>32)) {
	    uError("Illegal indicator index %d (range 1..32)\n",rtrn.uval);
	    uAction("Index definition for %s indicator ignored\n",
						stText(led->name));
	    return False;
	}
	led->indicator= rtrn.uval;
    }
    else {
	uError("Unknown field %s in map for %s indicator\n",field,
							stText(led->name));
	uAction("Definition ignored\n");
	ok= False;
    }
    return ok;
}

XkbFileLEDInfo *
HandleIndicatorMapDef(stmt,xkb,dflt,oldLEDs,mergeMode)
    IndicatorMapDef *	stmt;
    XkbDescPtr		xkb;
    XkbFileLEDInfo *	dflt;
    XkbFileLEDInfo *	oldLEDs;
    unsigned 		mergeMode;
{
XkbFileLEDInfo		tmp,*rtrn;
VarDef *		def;
Bool			ok;
 
    if (stmt->merge!=MergeDefault) {
	if (stmt->merge==MergeReplace)
	     mergeMode= MergeOverride;
	else mergeMode= stmt->merge;
    }

    tmp= *dflt;
    tmp.name= stmt->name;

    ok= True;
    for (def= stmt->body;def!=NULL;def= (VarDef *)def->common.next) {
	ExprResult	elem,field;
	ExprDef *	arrayNdx;
	if (!ExprResolveLhs(def->name,&elem,&field,&arrayNdx)) {
	    ok= False;
	    continue;
	}
	if (elem.str!=NULL) {
	    uError("Cannot set defaults for \"%s\" element in indicator map\n",
								elem.str);
	    uAction("Assignment to %s.%s ignored\n",elem.str,field.str);
	    ok= False;
	}
	else {
	    ok= SetIndicatorMapField(&tmp,xkb,field.str,arrayNdx,def->value,
							 	mergeMode)&&ok;
	}
    }
    if (ok) {
	rtrn= AddIndicatorMap(&tmp,oldLEDs,mergeMode,True);
	return rtrn;
    }
    return NULL;
}

Bool
CopyIndicatorMapDefs(result,leds)
    XkbFileInfo *	result;
    XkbFileLEDInfo *	leds;
{
XkbFileLEDInfo *led,*next,*last;
XkbDescPtr	xkb;

    xkb= &result->xkb;
    result->defined|= XkbIndicatorMapsDefined;
    if (!XkbAllocNames(xkb,XkbIndicatorNamesMask)) {
	uInternalError("Couldn't allocate names\n");
	uAction("Indicator names may be incorrect\n");
    }
    if (xkb->indicators==NULL) {
	xkb->indicators= uTypedAlloc(XkbIndicatorRec);
	if (xkb->indicators==NULL) {
	    uInternalError("Can't allocate \"indicators\"\n");
	    uAction("Indicator map definitions may be lost\n");
	}
	bzero((char *)xkb->indicators,sizeof(XkbIndicatorRec));
    }
    for (last=result->leds;(last!=NULL)&&(last->next!=NULL);last=last->next) {
	/* conditional does the work */;
    }
    for (led=leds;led!=NULL;led=next) {
	next= led->next;
	if ((led->groups!=0)&&(led->which_groups==0)) 
	    led->which_groups= XkbIM_UseEffective;
	if ((led->which_mods==0)&&((led->real_mods!=0)||(led->vmods!=0)))
	    led->which_mods= XkbIM_UseEffective;
	if ((led->indicator==XkbFileLEDNotBound)||(!xkb->indicators)) {
	    led->next= NULL;
	    if (last)
		 last->next= led;
	    else result->leds= led;
	    last= led;
	}
	else {
	    register XkbIndicatorMapPtr imap;
	    imap= &xkb->indicators->maps[led->indicator-1];
	    imap->flags= led->flags;
	    imap->which_groups= led->which_groups;
	    imap->groups= led->groups;
	    imap->which_mods= led->which_mods;
	    imap->mask= led->real_mods;
	    imap->real_mods= led->real_mods;
	    imap->vmods= led->vmods;
	    imap->ctrls= led->ctrls;
	    if (xkb->names!=NULL)
		xkb->names->indicators[led->indicator-1]= led->name;
	    uFree(led);
	}
    }
    return True;
}

Bool
BindIndicators(result,force)
    XkbFileInfo *	result;
    Bool		force;
{
XkbDescPtr			xkb;
register int 			i;
register XkbFileLEDInfo *	led,*next,*last;

    xkb= &result->xkb;
    if (xkb->names!=NULL) {
	for (led=result->leds;led!=NULL;led= led->next) {
	    if (led->indicator==XkbFileLEDNotBound) {
		for (i=0;i<XkbNumIndicators;i++) {
		    if (xkb->names->indicators[i]==led->name) {
			led->indicator= i+1;
			break;
		    }
		}
	    }
	    if ((led->indicator==XkbFileLEDNotBound)&&force) {
		for (i=0;i<XkbNumIndicators;i++) {
		    if (xkb->names->indicators[i]==None) {
			xkb->names->indicators[i]= led->name;
			led->indicator= i+1;
			break;
		    }
		}
		if (led->indicator==XkbFileLEDNotBound) {
		    uError("Too many indicator maps\n");
		    uAction("Some indicator maps will be lost\n");
		    continue;
		}
	    }
	}
    }
    for (last=NULL,led=result->leds;led!=NULL;led= next) {
	next= led->next;
	if (led->indicator==XkbFileLEDNotBound) {
	    if (last)
		 last->next= led;
	    else result->leds= led;
	    last= led;
	}
	else {
	    if ((xkb->names!=NULL)&&
		(xkb->names->indicators[led->indicator-1]!=led->name)) {
		Atom old= xkb->names->indicators[led->indicator-1];
		uError("Multiple names bound to indicator %d\n",led->indicator);
		uAction("Using %s, ignoring %s\n",
				XkbAtomGetString(result->dpy,old),
				XkbAtomGetString(result->dpy,led->name));
		led->indicator= XkbFileLEDNotBound;
		if (last)
		     last->next= led;
		else result->leds= led;
		last= led;
	    }
	    else {
		XkbIndicatorMapPtr map;
		map= &xkb->indicators->maps[led->indicator-1];
		map->flags= led->flags;
		map->which_groups= led->which_groups;
		map->groups= led->groups;
		map->which_mods= led->which_mods;
		map->mask= led->real_mods;
		map->real_mods= led->real_mods;
		map->vmods= led->vmods;
		map->ctrls= led->ctrls;
		last->next= next;
		led->next= NULL;
		uFree(led);
	    }
	}
    }
    return True;
}
