/* $XConsortium: xtesttest.c,v 1.9 94/04/17 20:46:05 rws Exp $ */
/*

Copyright (c) 1992  X Consortium

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the X Consortium shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the X Consortium.

*/


#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/extensions/XTest.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/extensions/XInput.h>

char *ProgramName;

static void usage ()
{
    fprintf (stderr, "usage:  %s [-display dpy]\n", ProgramName);
    exit (1);
}

main (argc, argv)
    int argc;
    char **argv;
{
    char *displayname = NULL;
    Display *dpy;
    int i;    
    int event_base, error_base;
    int major_version, minor_version;
    unsigned long req;
    GC gc;
    XID gid;
    Window w;
    XSetWindowAttributes swa;
    int key, minkey, maxkey;
    XEvent ev, second_ev, third_ev;
    long    delta_time;
    unsigned char buttons[256];

    ProgramName = argv[0];
    for (i = 1; i < argc; i++) {
	char *arg = argv[i];

	if (arg[0] == '-') {
	    switch (arg[1]) {
	      case 'd':			/* -display dpy */
		if (++i >= argc) usage ();
		displayname = argv[i];
		continue;
	    }
	}
	usage ();
    }

    dpy = XOpenDisplay (displayname);
    if (!dpy) {
	fprintf (stderr, "%s:  unable to open display \"%s\"\n",
		 ProgramName, XDisplayName(displayname));
	exit (1);
    }

    if (!XTestQueryExtension (dpy, &event_base, &error_base, &major_version, &minor_version)) {
	fprintf (stderr, 
	 "%s:  XTest extension not supported on server \"%s\"\n",
		 ProgramName, DisplayString(dpy));
	XCloseDisplay(dpy);
	exit (1);
    }
    printf ("XTest information for server \"%s\":\n",
	    DisplayString(dpy));
    printf ("  Major version:       %d\n", major_version);
    printf ("  Minor version:       %d\n", minor_version);
    printf ("  First event number:  %d\n", event_base);
    printf ("  First error number:  %d\n", error_base);

    
    swa.override_redirect = True;
    swa.cursor = XCreateFontCursor(dpy, XC_boat);
    swa.event_mask = KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|ButtonMotionMask;
    w = XCreateWindow(dpy, DefaultRootWindow(dpy), 0, 0, 100, 100, 0, 0,
		      InputOnly, CopyFromParent,
		      CWEventMask|CWOverrideRedirect|CWCursor, &swa);
    XMapWindow(dpy, w);
    if (!XTestCompareCursorWithWindow(dpy, w, swa.cursor))
	printf("error: window cursor is not the expected one\n");
    XTestFakeMotionEvent(dpy, DefaultScreen(dpy), 10, 10, 0);
    if (!XTestCompareCurrentCursorWithWindow(dpy, w))
	printf("error: window cursor is not the displayed one\n");
    XUndefineCursor(dpy, w);
    if (!XTestCompareCursorWithWindow(dpy, w, None))
	printf("error: window cursor is not the expected None\n");
    XSync(dpy, True);
    XDisplayKeycodes(dpy, &minkey, &maxkey);
    key = XKeysymToKeycode(dpy, XK_a);
    if (!key)
	key = minkey;
    XTestFakeKeyEvent(dpy, key, True, 0);
    XNextEvent(dpy, &ev);
    if (ev.type != KeyPress ||
	ev.xkey.keycode != key ||
	ev.xkey.x_root != 10 ||
	ev.xkey.y_root != 10)
	printf("error: bad event received for key press\n");
    XTestFakeKeyEvent(dpy, key, False, 0);
    XNextEvent(dpy, &ev);
    if (ev.type != KeyRelease ||
	ev.xkey.keycode != key ||
	ev.xkey.x_root != 10 ||
	ev.xkey.y_root != 10)
	printf("error: bad event received for key release\n");
    XGetPointerMapping(dpy, buttons, sizeof(buttons));
    for (i = 0; !buttons[i] && (i < sizeof(buttons)); i++)
	;
    XTestFakeButtonEvent(dpy, i + 1, True, 0);
    XTestFakeMotionEvent(dpy, DefaultScreen(dpy), 9, 8, 1000);
    XTestFakeButtonEvent(dpy, i + 1, False, 2000);
    XNextEvent(dpy, &ev);
    if (ev.type != ButtonPress ||
	ev.xbutton.button != buttons[i] ||
	ev.xbutton.x_root != 10 ||
	ev.xbutton.y_root != 10)
	printf("error: bad event received for button press\n");
    XNextEvent(dpy, &second_ev);
    if (second_ev.type != MotionNotify ||
	second_ev.xmotion.x_root != 9 ||
	second_ev.xmotion.y_root != 8)
	printf("error: bad event received for motion\n");
    delta_time = second_ev.xmotion.time - ev.xbutton.time;
    if (delta_time > 1100 || delta_time < 900)
	printf ("Poor event spacing is %d should be %d\n", delta_time, 1000);
    XNextEvent(dpy, &third_ev);
    if (third_ev.type != ButtonRelease ||
	third_ev.xbutton.button != buttons[i] ||
	third_ev.xbutton.x_root != 9 ||
	third_ev.xbutton.y_root != 8)
	printf("error: bad event received for button release\n");
    delta_time = third_ev.xbutton.time - second_ev.xmotion.time;
    if (delta_time > 2100 || delta_time < 1900)
	printf ("Poor event spacing is %d should be %d\n", delta_time, 2000);
    gc = DefaultGC(dpy, DefaultScreen(dpy));
    req = NextRequest(dpy);
    XDrawPoint(dpy, w, gc, 0, 0);
    if (!XTestDiscard(dpy) || req != NextRequest(dpy))
	printf("error: XTestDiscard failed to discard an XDrawPoint\n");
    gid = XGContextFromGC(gc);
    XTestSetGContextOfGC(gc, 3L);
    if (XGContextFromGC(gc) != 3L)
	printf("error: XTestSetGContextOfGC failed\n");
    XTestSetGContextOfGC(gc, gid);
    if (XGContextFromGC(gc) != gid)
	printf("error: XTestSetGContextOfGC failed\n");
    test_xinput_devices(dpy, w, major_version, minor_version);
    XCloseDisplay (dpy);
    exit (0);
}

test_xinput_devices(dpy, w, major_version, minor_version)
    Display *dpy;
    Window w;
    int major_version, minor_version;
{
    int i, j, n_devices, axes[6], evcount=0;
    XDeviceInfo *list, *slist;
    XInputClassInfo *ip;
    XAnyClassPtr any;
    XID dkp, dkr, dbp, dbr, dpi, dpo, dm;
    XEventClass dkpc, dkrc, dbpc, dbrc, dmc, dpic, dpoc, evclasses[7];
    XDevice *tdev, *keydevice=NULL, *buttondevice=NULL, *proximitydevice=NULL;
    XDevice *motiondevice=NULL;
    KeyCode keycode;
    XEvent ev, ev2;
    XDeviceKeyEvent *K;
    XDeviceButtonEvent *B, *B0;
    XDeviceMotionEvent *M;
    XProximityNotifyEvent *P;
    long    delta_time;

    if (major_version < 2 || minor_version < 2){
	printf ("XInput extension not testable on server \"%s\"\n",
		DisplayString(dpy));
	return;
    }
	
    if (!(list = XListInputDevices (dpy, &n_devices))){
	printf ("XInput extension not supported on server \"%s\"\n",
		DisplayString(dpy));
	return;
    }
    slist = list;
    keydevice = buttondevice = motiondevice = NULL;
    for (i=0; i<n_devices; i++,list++) {
	if (list->use != IsXExtensionDevice)
	    continue;
	if (!keydevice){
	    for (any=list->inputclassinfo,j=0; j<list->num_classes; j++) {
		if (any->class == KeyClass) {
		    XKeyInfo *k = (XKeyInfo *) any;
		    if (k->num_keys == 0)
			continue;
		    keycode = k->min_keycode;
		}
		any = (XAnyClassPtr) ((char *) any + any->length);
	    }
	}
	tdev = XOpenDevice(dpy, list->id);
	for (ip= tdev->classes, j=0; j<tdev->num_classes; j++, ip++) {
	    if (ip->input_class == KeyClass && !keydevice) {
		keydevice = tdev;
		DeviceKeyPress(keydevice, dkp, dkpc);
		DeviceKeyRelease(keydevice, dkr, dkrc);
		evclasses[evcount++]=dkpc;
		evclasses[evcount++]=dkrc;
	    }
	    else if (ip->input_class == ButtonClass && !buttondevice) {
		buttondevice = tdev;
		DeviceButtonPress(buttondevice, dbp, dbpc);
		DeviceButtonRelease(buttondevice, dbr, dbrc);
		evclasses[evcount++]=dbpc;
		evclasses[evcount++]=dbrc;
	    }
	    else if (ip->input_class == ProximityClass && !proximitydevice) {
		proximitydevice = tdev;
		ProximityIn(proximitydevice, dpi, dpic);
		ProximityOut(proximitydevice, dpo, dpoc);
		evclasses[evcount++]=dpic;
		evclasses[evcount++]=dpoc;
	    }
	    else if (ip->input_class == ValuatorClass && !motiondevice) {
		motiondevice = tdev;
		DeviceMotionNotify(motiondevice, dm, dmc);
		evclasses[evcount++]=dmc;
	    }
	}
    if (tdev!=keydevice && tdev!=buttondevice && tdev!=motiondevice &&
	tdev!=proximitydevice)
	XCloseDevice(dpy, tdev);
    }
    XSelectExtensionEvent (dpy, w, evclasses, evcount);
    for(i=0;i<6;i++)
	axes[i]=i;

    XTestFakeMotionEvent(dpy, DefaultScreen(dpy), 10, 10, 0);
    XSync(dpy,True);
    if (keydevice) {
	XTestFakeDeviceKeyEvent(dpy, keydevice, keycode, True, axes, 6, 0);
	XNextEvent(dpy, &ev);
	K = (XDeviceKeyEvent *) &ev;
	if (K->type != dkp ||
	    K->keycode != keycode ||
	    K->x_root != 10 ||
	    K->y_root != 10 ||
	    K->first_axis != 0 ||
	    K->axis_data[0] != 0 ||
	    K->axis_data[1] != 1 ||
	    K->axis_data[2] != 2 ||
	    K->axis_data[3] != 3 ||
	    K->axis_data[4] != 4 ||
	    K->axis_data[5] != 5)
	    printf("error: bad event received for device key press\n");

	XTestFakeDeviceKeyEvent(dpy, keydevice, keycode, False, axes, 6, 0);
	XNextEvent(dpy, &ev);
	K = (XDeviceKeyEvent *) &ev;
	if (K->type != dkr ||
	    K->keycode != keycode ||
	    K->x_root != 10 ||
	    K->y_root != 10 ||
	    K->first_axis != 0 ||
	    K->axis_data[0] != 0 ||
	    K->axis_data[1] != 1 ||
	    K->axis_data[2] != 2 ||
	    K->axis_data[3] != 3 ||
	    K->axis_data[4] != 4 ||
	    K->axis_data[5] != 5)
	    printf("error: bad event received for device key release\n");
    }

    if (buttondevice) {
	XTestFakeDeviceButtonEvent(dpy, buttondevice, 1, True, axes, 6, 0);
	XNextEvent(dpy, &ev);
	B0 = (XDeviceButtonEvent *) &ev;
	if (B0->type != dbp ||
	    B0->button != 1 ||
	    B0->x_root != 10 ||
	    B0->y_root != 10 ||
	    B0->first_axis != 0 ||
	    B0->axis_data[0] != 0 ||
	    B0->axis_data[1] != 1 ||
	    B0->axis_data[2] != 2 ||
	    B0->axis_data[3] != 3 ||
	    B0->axis_data[4] != 4 ||
	    B0->axis_data[5] != 5)
	    printf("error: bad event received for device button press\n");

	XTestFakeDeviceButtonEvent(dpy, buttondevice, 1, False, axes, 6, 1000);
	XNextEvent(dpy, &ev2);
	B = (XDeviceButtonEvent *) &ev2;
	if (B->type != dbr ||
	    B->button != 1 ||
	    B->x_root != 10 ||
	    B->y_root != 10 ||
	    B->first_axis != 0 ||
	    B->axis_data[0] != 0 ||
	    B->axis_data[1] != 1 ||
	    B->axis_data[2] != 2 ||
	    B->axis_data[3] != 3 ||
	    B->axis_data[4] != 4 ||
	    B->axis_data[5] != 5)
	    printf("error: bad event received for device button release\n");
	delta_time = B->time - B0->time;
	if (delta_time > 1100 || delta_time < 900)
	    printf("Poor event spacing is %d should be %d\n", delta_time, 1000);
	}

    if (proximitydevice) {
	XTestFakeProximityEvent(dpy, proximitydevice, True, axes, 6, 0);
	XNextEvent(dpy, &ev);
	P = (XProximityNotifyEvent *) &ev;
	if (P->type != dpi ||
	    P->x_root != 10 ||
	    P->y_root != 10 ||
	    P->first_axis != 0 ||
	    P->axis_data[0] != 0 ||
	    P->axis_data[1] != 1 ||
	    P->axis_data[2] != 2 ||
	    P->axis_data[3] != 3 ||
	    P->axis_data[4] != 4 ||
	    P->axis_data[5] != 5)
	    printf("error: bad event received for proximity in event\n");

	XTestFakeProximityEvent(dpy, proximitydevice, False, axes, 6, 0);
	XNextEvent(dpy, &ev);
	P = (XProximityNotifyEvent *) &ev;
	if (P->type != dpo ||
	    P->x_root != 10 ||
	    P->y_root != 10 ||
	    P->first_axis != 0 ||
	    P->axis_data[0] != 0 ||
	    P->axis_data[1] != 1 ||
	    P->axis_data[2] != 2 ||
	    P->axis_data[3] != 3 ||
	    P->axis_data[4] != 4 ||
	    P->axis_data[5] != 5)
	    printf("error: bad event received for proximity out event\n");
    }

    if (motiondevice) {
	XTestFakeDeviceMotionEvent(dpy, motiondevice, False, 0, axes, 6, 0);
	XNextEvent(dpy, &ev);
	M = (XDeviceMotionEvent *) &ev;
	if (M->type != dm ||
	    M->first_axis != 0 ||
	    M->axis_data[0] != 0 ||
	    M->axis_data[1] != 1 ||
	    M->axis_data[2] != 2 ||
	    M->axis_data[3] != 3 ||
	    M->axis_data[4] != 4 ||
	    M->axis_data[5] != 5)
	    printf("error: bad event received for device motion\n");
    }
}

