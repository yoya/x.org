#include <X/Xlib.h>	  /*  X library include file          */ 
#include <Xr/defs.h>	  /*  X-ray definitions include file  */
#include <Xr/types.h>	  /*  X-ray types include file        */
#include "HelloWorld.h"
#include "smile.h"
#include "frown.h"


main (argc, argv)
int argc;
char * argv[];

{
   Display    * displayPtr;	/*  Ptr to the opened display strucuture   */
   xrWindowData windowData;	/*  Structure used to set the window data  */
   xrEvent      input;		/*  Structure used to gather input         */
   extern xrPFI DrawWindow();   /*  The redraw routine for the main window */
   Bitmap       bitmap;		/*  Used to create the raster bitmap id's  */
   Pixmap       pixmap;		/*  Used to create the raster pixmap id's  */


   /*
    *  Open the display and initialize X-ray.
    */

   if ((displayPtr = XOpenDisplay (argv[1])) == 0)
   {
      printf ("Could not open the display\n");
      exit();
   }

   if (XrInit (NULL) == FALSE)
   {
      printf ("Could not initialize X-ray\n");
      exit();
   }


   /*
    *  Create, map, register and select input for the main window.
    */

   if ((windowId = 
          XCreateWindow (RootWindow, 10, 10, 330, 120, xrBorderWidth,
                         xrWindowForeground, xrWindowBackground)) == 0)
   {
      printf ("The window create failed\n");
      exit();
   }

   XMapWindow (windowId);

   XrSetRect (&windowData.windowRect, 10, 10, 100, 100);
   windowData.foreTile = xrWindowForeground;
   windowData.backTile = xrWindowBackground;
   XrInput (windowId, MSG_ADDWINDOW, &windowData);

   XSelectInput (windowId, ButtonPressed | ButtonReleased |
                           KeyPressed | ExposeWindow);


   /*
    *  Call the function used to redraw the window to get it drawn.
    */

   DrawWindow (windowId, MSG_NEW, NULL);


   /*
    *  Set up a cursor for the window using the default cursor.
    */

   XDefineCursor (windowId, xrDefaultCursor);


   /*
    *  Set up and create the menu.
    */

   menu = XrMenu (NULL, MSG_NEW, &menuInfo);
   XrMenu (menu, MSG_ACTIVATEMENU, windowId);


   /*
    *  Set up the rasters for the two message boxes.
    */

   bitmap = XStoreBitmap (smile.width, smile.height, smile.raster);
   pixmap = XMakePixmap (bitmap, xrBackgroundColor, xrForegroundColor);
   niceMsgInfo.rasterId = pixmap;
   niceMsgInfo.rasterWidth = smile.width;
   niceMsgInfo.rasterHeight = smile.height;

   bitmap = XStoreBitmap (frown.width, frown.height, frown.raster);
   pixmap = XMakePixmap (bitmap, xrBackgroundColor, xrForegroundColor);
   meanMsgInfo.rasterId = pixmap;
   meanMsgInfo.rasterWidth = frown.width;
   meanMsgInfo.rasterHeight = frown.height;


   /*
    *  Set up and input loop.
    */

   while (1)
   {
      if (XrInput (0, MSG_BLKHOTREAD, &input) != FALSE)
      {
         if (input.type & XrXRAY && input.inputType == XrMENU)
         {
            switch (input.value3)
            {
               case 0:		/*  Show the Nice Message Box  */
                  XrMessageBox (&niceMsgInfo, MSG_EDIT, &input);
               break;

               case 1:		/*  Show the Mean Message Box  */
                  XrMessageBox (&meanMsgInfo, MSG_EDIT, &input);
               break;
        
               case 3:		/*  Exit  */
                  exit();
               break;
            }
         }
      }
   }
}


/************************************************************************
 *
 *  DrawWindow (window, message, data)
 *	This function handles the initial drawing of the window.
 *      which occurs if the message is set to MSG_DRAW.
 *      It also handles the redrawing of the window when an exposure
 *      event occurs.  Message will contain MSG_REDRAW.
 *
 ************************************************************************/

xrPFI
DrawWindow (window, message, data)
Window         window;
INT32          message;
XExposeEvent * data;

{
   xrWindowEvent exposeEvent[1];
   xrWindowFunctInfo exposeFunct;
   xrEvent nullEvent;
   

   /*
    *  Set up a switch to handle the two possible messages.
    */

   switch (message)
   {
      case MSG_NEW:

         /*
          *  Create the title bar editor.
          */

         titleBarInfo.editorWindowId = windowId;
         titleBar = XrTitleBar (NULL, MSG_NEW, &titleBarInfo);


         /*
          *  Set up the structure neccessary to have this function
          *  called automatically upon an exposure event.
          */

         exposeEvent[0].inputType = ExposeWindow;
         exposeEvent[0].inputCode = 0;

         exposeFunct.processFlag = TRUE;
         exposeFunct.funct = (xrPFI) DrawWindow;
         exposeFunct.instance = (INT32) windowId;
         exposeFunct.message = MSG_REDRAW;
         exposeFunct.eventCount = 1;
         exposeFunct.eventList = &exposeEvent[0];

         XrInput (windowId, MSG_ADDWINDOWFUNCT, &exposeFunct);


      case MSG_REDRAW:

         /*
          *  Write the text into the window.
          */

         XText (window, 20, 50, "Press the Menu Button to display the menu.",
                42, xrBaseFontInfo->id, xrForegroundColor, xrBackgroundColor);
         XText (window, 50, 50 + xrBaseFontInfo -> height + 5,
                "(Default: Right Button Down)", 28, xrBaseFontInfo->id,
                xrForegroundColor, xrBackgroundColor);


         /*
          *  Redraw the title bar.
          */

         XrTitleBar (titleBar, MSG_REDRAW, XrREDRAW_ALL);


         /*
          *  Push a null event to be returned to the application.
          */

         nullEvent.type = XrXRAY;
         nullEvent.inputType = NULL;
         XrInput (0, MSG_PUSHEVENT, &nullEvent);
      break;
   }
}
