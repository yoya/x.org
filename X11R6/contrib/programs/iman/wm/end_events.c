/*
 *
 * 	end_events.c  
 * 	evenements de la fenetre de fermeture
 *
 * 	Modification :  18/11/93
 *
 *	Copyright (c) 1993 Bruno RIVAS
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
#include <X11/keysym.h>


#include <X11/iman/widgets.h>
#include <X11/iman/windows.h>
#include <X11/iman/messages.h>

#include "iman.h"





void END_Events()
{
 int i, j, k, w;


  switch(tk_event.ev_type)
  {

	case BN_RELEASED:

		if(tk_event.button==bn_end_ok)
		{
		  XUngrabServer(tk_display->display);
		  WM_EndOfSession();
		}
		else if(tk_event.button==bn_end_cancel)
		{
		 END_CANCEL:
		  XSync(tk_display->display,True);
		  XUngrabServer(tk_display->display);
		  WM_UnfreezeAll();
		  XUnmapWindow(tk_display->display,windows[wm_end_index].mainwindow);
		  wm_action.type=NoAction;

		  windows[wm_action.number].state.isOnTop=True;
		  WIN_GiveFocus(wm_action.number);
		  WIN_DrawActive(wm_action.number);
		  
		  wm_action.number=-1;
		  wm_action.window=0;
		  wm_action.type=NoAction;
		  WID_Unfreeze(tk_display,wm_process_window);
		  WID_Unfreeze(tk_display,wm_setup_window);
		  WID_Unfreeze(tk_display,wm_desktop_window);
		  WID_Unfreeze(tk_display,wm_clipboard_window);
		  WID_Unfreeze(tk_display,wm_colors_window);
		  WID_Unfreeze(tk_display,wm_about_window);
		  XUngrabServer(tk_display->display);
		  
		  XUnmapWindow(tk_display->display,wm_end_window);
		}
		break;




	case BN_KEYUNKNOWN:

		if((tk_event.button==bn_end_ok||tk_event.button==bn_end_cancel)&&XKeycodeToKeysym(tk_display->display,tk_event.event.xkey.keycode,0)==XK_Escape)
		{
		 	tk_event.button=bn_end_cancel;
			goto END_CANCEL;
		}
		break;





  }

}





