/* Example program of horizontal paned windows */

#include <stdio.h>
#include <X/Xlib.h>
#include "Toolkit.h"

#define NUMPANES 5


DoIt(parent, window, request, requestBox, replyBox)
Window parent, window;
enum TGeometryRequest request;
WindowBox *requestBox, *replyBox;
{
    *replyBox = *requestBox;
    XChangeWindow(window, requestBox->w, requestBox->h);
    return REQUESTYES;
}

main()
{
    int     i;
    XEvent event;
    Window  windowframe,
            wpane[NUMPANES];
    Targ    args[10];

    if(XOpenDisplay("") == FALSE)
       printf("Error opening display\n");

/* Make the frame for the tiled window */

    windowframe = XCreateWindow(RootWindow, 100,300, 100, 200, 3,
	                        BlackPixmap, WhitePixmap);
    TSetGeometryRequest(windowframe, DoIt);
    TMakeHWindowFrame(windowframe);

/* Make the tiled panels */

    TSetArg (args[0], T_PANED_WIDTH, 100);
    TSetArg (args[1], T_PANED_MIN,    10);
    TSetArg (args[2], T_PANED_MAX,   200);
    TSetArg (args[3], 0, 0);

    for (i = 0; i < NUMPANES; i++)
	wpane[i] = TAddToHWindowFrame (windowframe, args);

    XMapWindow(windowframe);

    while (TRUE) {
	XNextEvent(&event);
	TDispatchXEvent(&event);
    }
}
