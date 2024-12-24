/*** example program of a text subwindow with a string source ***/
#include <X/Xlib.h>
#include <stdio.h>
#include "Toolkit.h"

#define MESS1 "This is a text subwindow with a string source. \nWhen you hit return the application will write another message in this window."
#define MESS2 "The application wrote out this text and also performed the hightlighting that you see."

Window w;
Targ args[20];


testit()
{
	TTextReplace replace;
	TTextPosition position[2];
	char buffer [400];
	int i = 0;

	sprintf(buffer, "%-400s", MESS2);
	replace.startPos = 0;
	replace.endPos = replace.startPos + strlen(buffer);
	replace.text.length=strlen(buffer);
	replace.text.ptr = buffer;
	TSetArg(args[i], T_TEXT_REPLACE, &replace); 

	i++;
	position[0] = 4;
	position[1] = 14;
	TSetArg(args[i], T_TEXT_HIGHLIGHT, position); 

	i++;
	TSetArg(args[i], T_TEXT_INSERT_POS, strlen(MESS2));

	i++;
	TSetArg(args[i], 0, 0);
	TSetTextAttr(w, args);
}


main()
{
	char buffer[400];
	XEvent ev, *event;
	int i = 0;

	/*** open display  ***/
	if (XOpenDisplay("") == 0)
		printf("error opening display\n");

	/*** create the text subwindow ****/
	sprintf(buffer, "%-400s", MESS1);
	TSetArg(args[i], T_TEXT_SOURCE, TCreateStringSource(buffer, 400)); 
	i++;
	TSetArg(args[i], T_TEXT_OPTIONS,  editable | wordBreak); 
	i++;
	TSetArg(args[i], T_TEXT_NOTIFY_SYMBOLS, "\n");
	i++;
	TSetArg(args[i], T_TEXT_NOTIFY_ROUTINE, testit);
	i++;
        TSetArg(args[i], T_TEXT_SEL_TYPE, wordSelection);
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
