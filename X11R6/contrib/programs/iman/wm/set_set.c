/*
 *
 * 	set_set.c
 * 	modification de la fenetre de parametrage
 *
 * 	Modification :  27/01/94
 *
 *	Copyright (c) 1993,1994 Bruno RIVAS
 *	All Rights Reserved
 *
 *
 * Permission to use, copy, modify, distribute and sell this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appears in all copies and that both that
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



#define NEED_XRM_RESOURCES


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <malloc.h>
#include <memory.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>


#include <X11/iman/widgets.h>
#include <X11/iman/windows.h>
#include <X11/iman/messages.h>


#include "iman.h"





/*
 *
 * Boucle principale
 *
 *
 */

int SET_SavePreferences()
{
 int mask;
 int ret;
 int i;


 if(xrdb!=NULL)
 {
   if(wm_info.set_decoration==True)
     XrmPutStringResource(&xrdb,options_text[0],"ON");
   else XrmPutStringResource(&xrdb,options_text[0],"OFF");

   if(wm_info.set_groups==True)
     XrmPutStringResource(&xrdb,options_text[1],"ON");
   else XrmPutStringResource(&xrdb,options_text[1],"OFF");

   if(wm_info.set_icons==True)
     XrmPutStringResource(&xrdb,options_text[2],"ON");
   else XrmPutStringResource(&xrdb,options_text[2],"OFF");

   if(wm_info.set_icontitle==True)
     XrmPutStringResource(&xrdb,options_text[3],"ON");
   else XrmPutStringResource(&xrdb,options_text[3],"OFF");

   if(wm_info.set_helpactive==True)
     XrmPutStringResource(&xrdb,options_text[4],"ON");
   else XrmPutStringResource(&xrdb,options_text[4],"OFF");

   if(wm_info.set_debug==True)
     XrmPutStringResource(&xrdb,options_text[5],"ON");
   else XrmPutStringResource(&xrdb,options_text[5],"OFF");

#ifdef DESQVIEW_X_SERVER
   XrmPutFileDatabase(xrdb,"/dvx/iman.rc");
#else
   XrmPutFileDatabase(xrdb,xrm_file);
#endif

   return 0;
 }

 return -1;
}




