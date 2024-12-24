/*** example program of a label ***/
#include <X/Xlib.h>
#include <stdio.h>
#include "Toolkit.h"
Window w;
Targ args[10];

main()
{
	XEvent ev, *event;
	int i = 0;

	if (XOpenDisplay("") == 0)
		printf("error opening display\n");

	TSetArg(args[i], T_LABEL_STRING, "Hello People");
	i++;
	TSetArg(args[i], 0, 0);
	w = TCreateLabel(RootWindow, args);

	XMapWindow(w);
	event = &ev;
	while (1) {
		XNextEvent(event);
		TDispatchXEvent(event);
	}
}
