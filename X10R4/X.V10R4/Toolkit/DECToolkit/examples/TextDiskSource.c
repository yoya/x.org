/* This is an example program of a text subwindow with a disk source */
#include <X/Xlib.h>
#include <stdio.h>
#include "Toolkit.h"

Window w;
Targ args[10];

main()
{
	XEvent ev, *event;
	int i = 0;

	/*** open display  ***/
	if (XOpenDisplay("") == 0)
		printf("error opening display\n");

	/*** create the text subwindow ****/
	TSetArg(args[i], T_TEXT_SOURCE, TCreateDiskSource("disksource.out"));
	i++;
	TSetArg(args[i], T_TEXT_WIDTH, 350);
	i++;
	TSetArg(args[i], T_TEXT_HEIGHT, 200);
	i++;
	TSetArg(args[i], 0, 0);
	w = TCreateTextSW(RootWindow, args);

	/*** map and handle the events ***/
	XMapWindow(w);
	event = &ev;
	while (1) {
		XNextEvent(event);
		TDispatchXEvent(event);
	}
}
