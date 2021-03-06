/*
 *
 * 	end_init.c
 * 	initialisation de la fenetre de fermeture
 *
 * 	Modification :  18/04/94
 *
 *	Copyright (c) 1993,1994 Bruno RIVAS
 *	All Rights Reserved
 *
 *
 * Permission to use, copy, modify, distribute and sell this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of Bruno RIVAS not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission.  Bruno RIVAS makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * Bruno RIVAS disclaims all warranties with regard to this software,
 * including all implied warranties of merchantability and fitness,
 * in no event shall Bruno RIVAS be liable for any special,
 * indirect or consequential damages or any damages
 * whatsoever resulting from loss of use, data or profits,
 * whether in an action of contract, negligence or other tortious
 * action, arising out of or in connection with the use 
 * or performance of this software.
 *
 *
 *	Bruno RIVAS 
 *      IMAN Window Manager version 1.2
 *
 *	Internet:       rivas@email.teaser.com
 *	Surface Mail:   Bruno RIVAS,
 *			30 avenue Marie
 *			93250 Villemomble -- FRANCE
 *	Voice phone:    (33) (1) 49.35.97.27
 *	Fax: 		(33) (1) 48.94.27.91
 *
 */





#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <malloc.h>
#include <memory.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <X11/iman/widgets.h>
#include <X11/iman/windows.h>
#include <X11/iman/messages.h>

#include "iman.h"










void END_Init()
{
 WidgetAttributes wid_attributes;
 WidgetTextDecoration wid_text;

 XSetWindowAttributes attrib;
 XGCValues xgcvalues;

 unsigned long ptr;
 int mask;
 int ret;
 int i, j, k, w;
 unsigned long win_type;

 



	     /*** Creation de la fenetre de fin ***/

 attrib.background_pixel=tk_display->dlg_colors.bg;
 attrib.border_pixel=tk_display->dlg_colors.border_inactive;
 attrib.cursor=tk_display->cursors.normal;
 attrib.event_mask=ExposureMask;
 mask=CWBackPixel|CWBorderPixel|CWCursor|CWEventMask;
																									
 wm_end_window=XCreateWindow(tk_display->display,RootWindow(tk_display->display,tk_display->screen),(DisplayWidth(tk_display->display,0)-END_BOX_WIDTH)/2,(DisplayHeight(tk_display->display,0)-END_BOX_HEIGHT-45)/2,END_BOX_WIDTH,END_BOX_HEIGHT,1,tk_display->depth,InputOutput,CopyFromParent,mask,&attrib);
 XStoreName(tk_display->display,wm_end_window,"End of Iman");

 mask=GCForeground|GCBackground;
 xgcvalues.foreground=tk_display->dlg_colors.text;
 xgcvalues.background=tk_display->dlg_colors.bg;
 end_gc=XCreateGC(tk_display->display,wm_end_window, mask, &xgcvalues);

 wid_attributes.mask=SALighting;
 wid_attributes.lighting=True;
 bn_end_ok=wid_Create(tk_display,WI_BUTTON,BN_PUSHBUTTON,wm_end_window,wm_end_window,(END_BOX_WIDTH-160)/2,END_BOX_HEIGHT-35,75,25,&wid_attributes,Unpushed);
 wid_text.mask=STText|STFont|STKey|STGravity;
 wid_text.font=tk_display->fonts.helvetica12;
 wid_text.text="End";
 wid_text.key=1;
 wid_text.gravity=CenterText;
 wid_SetTextDecoration(tk_display,bn_end_ok,&wid_text,True);

 bn_end_cancel=wid_Create(tk_display,WI_BUTTON,BN_PUSHBUTTON,wm_end_window,wm_end_window,END_BOX_WIDTH/2+5,END_BOX_HEIGHT-35,75,25,&wid_attributes,Unpushed);
 wid_text.text="Cancel";
 wid_SetTextDecoration(tk_display,bn_end_cancel,&wid_text,True);

 wid_Map(tk_display,bn_end_ok);
 wid_Map(tk_display,bn_end_cancel);

 ptr=DIALOG_BOX;
 XChangeProperty(tk_display->display,wm_end_window, tk_display->atoms._IMAN_WM_TYPE, XA_INTEGER,32, PropModeReplace,(char *)&ptr,1);
 ptr=TitleBar+Border+Unmoveable;					
 XChangeProperty(tk_display->display,wm_end_window, tk_display->atoms._IMAN_WM_TYPE,XA_INTEGER,32, PropModeAppend,(char *)&ptr,1);


}



