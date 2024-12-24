/** example program of a text subwindow with an appendable source. ***/
#include <X/Xlib.h>
#include <stdio.h>
#include "Toolkit.h"

Window  w;
Targ args[10];

#define MESS1 "This is a text subwindow with an appendable source.\nType away! "
char buffer [100];
TTextReplace replace;

testit()
{
	TTextPosition position[2];
	int i = 0;

	TSetArg(args[i], T_TEXT_HIGHLIGHT, 0);
	i++;
	TSetArg(args[i], 0, 0);

	TGetTextAttr(w, args); 
	printf("Highlighted text: %s \n", (char *)args[0].data);
}


main()
{
	XEvent ev, *event;
	int i = 0;

	/*** open display ***/
	if (XOpenDisplay("") == 0)
		printf("error opening display\n");

	/*** create the text subwindow ****/
	TSetArg(args[i], T_TEXT_SOURCE, TCreateAppendSource("append.out"));
	i++;
	TSetArg(args[i], T_TEXT_OPTIONS, editable | wordBreak); 
	i++;
	TSetArg(args[i], T_TEXT_NOTIFY_SYMBOLS, "\n");
	i++;
	TSetArg(args[i], T_TEXT_NOTIFY_ROUTINE, testit);
	i++;
        TSetArg(args[i], T_TEXT_SEL_TYPE, wordSelection);
	i++;
	TSetArg(args[i], T_TEXT_WIDTH, 350);
	i++;
	TSetArg(args[i], T_TEXT_HEIGHT, 200);
	i++;
	TSetArg(args[i], 0, 0);
	w = TCreateTextSW(RootWindow, args);


	/**** put some text in the window (and in the file) ******/
	i = 0;
	strcpy(buffer, MESS1);
	replace.startPos = 0;   /** on append, doesn't matter what this is */
	replace.endPos = 0;	/** on append, doesn't matter what this is */
	replace.text.length=strlen(buffer);
	replace.text.ptr = buffer;
	TSetArg(args[i], T_TEXT_REPLACE, &replace); 
	i++;
	TSetArg(args[i], 0, 0);
	TSetTextAttr(w, args);


	/*** map and handle the events ***/
	XMapWindow(w);
	event = &ev;
	while (1) {
		XNextEvent(event);
		TDispatchXEvent(event);
	}
}
