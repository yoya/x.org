/* $XConsortium: synctest.c,v 1.4 94/04/17 20:46:04 dpw Exp $ */
/*

Copyright (c) 1993  X Consortium

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
/*
 * HISTORY
 * $Log:	synctest.c,v $
 * Revision 1.4  94/04/17  20:46:04  dpw
 * new X Consortium copyright/permission
 * 
 * Revision 1.3  93/09/29  19:12:42  dpw
 * added "couldn't find SERVERTIME system counter" error message
 * 
 * Revision 1.2  93/09/03  13:26:04  dpw
 * overhaul from Pete's version
 * 
 * Revision 1.1.1.2  1993/05/05  17:37:50  Pete_Snider
 * 	initial rcs submit
 *
 * 
 * $EndLog$
 */
#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xmd.h>
#include <X11/extensions/sync.h>


int total_error_count = 0;
char *errmessage;
char *testname;
int errcount;
int protocol_error;
int sync_event, sync_error;
int BadCounter, BadAlarm, AlarmNotify, CounterNotify;

void
set_error_message(s)
char *s;
{
    errmessage = s;
}


#define LAST_SET_MESSAGE errmessage

void
report_error(err)
char *err;
{
    fprintf(stderr, "Error: %s: %s\n", testname, err);
    errcount++;
}

int
protocol_error_unexpected(dpy, errevent)
Display *dpy;
XErrorEvent *errevent;
{
    char buf[80];
    XGetErrorText(dpy, errevent->error_code, buf, sizeof (buf));
    report_error(buf);
    return 1;
}

int
protocol_error_expected(dpy, errevent)
Display *dpy;
XErrorEvent *errevent;
{
    protocol_error = errevent->error_code;
    return 1;
}


Display *
begin_test(tn)
char *tn;
{
    Display *dpy;
    testname = tn;
    errcount = 0;
    printf("Start test %s\n", testname);
    dpy = XOpenDisplay("");
    if (!dpy)
    {
	report_error("Failed to open display\n");
	exit(-1);
    }
    XSynchronize(dpy, True); /* so we get errors at convenient times */
    set_error_message("Failed to find SYNC extension\n");
    XSetErrorHandler(protocol_error_unexpected);
    if (!XSyncQueryExtension(dpy, &sync_event, &sync_error))
    {
	report_error(LAST_SET_MESSAGE);
	exit(-1);
    }
    BadCounter = sync_error + XSyncBadCounter;
    BadAlarm = sync_error + XSyncBadAlarm;
    AlarmNotify = sync_event + XSyncAlarmNotify;
    CounterNotify = sync_event + XSyncCounterNotify;
    return dpy;
}

void
end_test(dpy)
Display *dpy;
{
    printf("End test %s with %d errors\n\n", testname, errcount);
    protocol_error = 0;
    total_error_count += errcount;
    XCloseDisplay(dpy);
}


XSyncCounter
get_servertime_counter(dpy)
Display *dpy;
{
    int n_counters, i;
    XSyncSystemCounter *sys_counter_list = NULL;
    XSyncCounter counter = None;

    set_error_message("Failed to list system counters\n");
    sys_counter_list = XSyncListSystemCounters(dpy, &n_counters);
	
    for (i = n_counters-1; i >= 0; i--) 
    {
	if (strcmp(sys_counter_list[i].name, "SERVERTIME")==0)
	{
	    counter = sys_counter_list[i].counter;
	    break;
	}
    }
    if (counter == None)
	report_error("couldn't find SERVERTIME system counter");

    XSyncFreeSystemCounterList(sys_counter_list);
    return counter;
}

void set_counter(dpy, counter, newval)
Display *dpy;
XSyncCounter counter;
int newval;
{
    XSyncValue v;
    XSyncIntToValue(&v, newval);
    XSetErrorHandler(protocol_error_unexpected);
    set_error_message("Failed to set counter");
    if (!XSyncSetCounter(dpy, counter, v))
    {
	report_error(LAST_SET_MESSAGE);
    }
}

void destroy_alarm(dpy, alarm)
Display *dpy;
XSyncAlarm alarm;
{
    XSetErrorHandler(protocol_error_unexpected);
    set_error_message("Failed to destroy alarm");
    if (!XSyncDestroyAlarm(dpy, alarm))
    {
	report_error(LAST_SET_MESSAGE);
    }
}

XSyncCounter create_counter(dpy, val)
Display *dpy;
int val;
{
    XSyncCounter counter;
    XSyncValue value;

    XSyncIntToValue(&value, val);
    XSetErrorHandler(protocol_error_unexpected);
    set_error_message("Failed to create counter");
    if ((counter = XSyncCreateCounter(dpy, value)) == None)
    {
	report_error(LAST_SET_MESSAGE);
    }
    return counter;
}

void expect_no_events(dpy)
Display *dpy;
{
    XEvent e;
    while(XPending(dpy))
    {
	report_error("unexpected event found");
	XNextEvent(dpy, &e);
    }
}

void
expect_alarmnotify(dpy, alarm, counterval, testval, alarmstate, newtestval,
		   checkvals)
Display *dpy;
XSyncAlarm alarm;
int counterval, testval, alarmstate, newtestval;
Bool checkvals;
{
    XEvent e;
    XSyncAlarmNotifyEvent *pane;
    XSyncValue counter_value, test_value, new_test_value;
    XSyncAlarmAttributes aa;
    if (XPending(dpy) == 0)
    {
	report_error("Failed to find AlarmNotify event");
	return;
    }
    XNextEvent(dpy, &e);
    pane = (XSyncAlarmNotifyEvent *)&e;

    if (pane->type != AlarmNotify)
    {
	report_error("Failed to find AlarmNotify event");
	return;
    }

    XSyncIntToValue(&counter_value, counterval);
    XSyncIntToValue(&test_value, testval);
    XSyncIntToValue(&new_test_value, newtestval);

    if (pane->alarm  != alarm)
	report_error("alarm in AlarmNotify event was incorrect");

    if (pane->state  != alarmstate)
	report_error("alarm state in AlarmNotify event was incorrect");

    if (checkvals && !XSyncValueEqual(pane->alarm_value, test_value))
	report_error("alarm value in AlarmNotify event was incorrect");

    if (checkvals && !XSyncValueEqual(pane->counter_value, counter_value))
	report_error("counter value in AlarmNotify event was incorrect");

    if (checkvals && (pane->state != XSyncAlarmDestroyed))
    {
	XSyncQueryAlarm(dpy, alarm, &aa);
	if (!XSyncValueEqual(aa.trigger.wait_value, new_test_value))
	    report_error("alarm test value after AlarmNotify event was incorrect");
    }
}


void test_initialize()
{
    Display *dpy = begin_test("Initialize");
    int major_val, minor_val;

    set_error_message("Failed to initialize SYNC extension\n");
    if (!XSyncInitialize(dpy, &major_val, &minor_val))
    {
	report_error(LAST_SET_MESSAGE);
    }

    printf("XSync information \"%s\":\n", DisplayString(dpy));
    printf("  Major version:       %d\n", major_val);
    printf("  Minor version:       %d\n", minor_val);
    printf("  First event number:  %d\n", sync_event);
    printf("  First error number:  %d\n", sync_error);
    end_test(dpy);
}

void test_list_system_counters()
{
    Display *dpy = begin_test("ListSystemCounters");
    int n_counters, i;
    XSyncSystemCounter *sys_counter_list = NULL;

    set_error_message("Failed to list system counters\n");
    sys_counter_list = XSyncListSystemCounters(dpy, &n_counters);
	
    printf("Number of system counters:  %d\n", n_counters);
    for (i = 0; i < n_counters; i++)
    {
	printf("%s id 0x%x\n", sys_counter_list[i].name,
	       sys_counter_list[i].counter);
    }
	
    if (get_servertime_counter(dpy) == None)
    {
	report_error("Failed to find SERVERTIME system counter\n");
    }

    XSyncFreeSystemCounterList(sys_counter_list);

    end_test(dpy);
}

void test_create_counter()
{
    Display *dpy = begin_test("CreateCounter");
    XSyncValue create_value, read_value;
    XSyncCounter counter;

    XSyncIntsToValue(&create_value, 2, 7);
    set_error_message("Failed to create counter");
    counter = XSyncCreateCounter(dpy, create_value);

    set_error_message("Failed to query counter");
    if (!XSyncQueryCounter(dpy, counter, &read_value))
    {
	report_error(LAST_SET_MESSAGE);
    }
	
    if (!XSyncValueEqual(create_value, read_value))
    {
	report_error("initial value of created counter was incorrect");
    }
    end_test(dpy);
}

void test_query_counter()
{
    Display *dpy = begin_test("QueryCounter");
    XSyncValue read_value;
    XSyncCounter servertime;

    XSetErrorHandler(protocol_error_expected);
    XSyncQueryCounter(dpy, (XSyncCounter)~0, &read_value);
    if (protocol_error != BadCounter)
    {
	report_error("Failed to get Counter error when querying non-existent counter");
    }

    servertime = get_servertime_counter(dpy);
    XSetErrorHandler(protocol_error_unexpected);
    set_error_message("Failed to query SERVERTIME");
    XSyncQueryCounter(dpy, servertime, &read_value);

    end_test(dpy);
}

void test_set_counter()
{
    Display *dpy = begin_test("SetCounter");
    XSyncValue create_value, set_value, read_value;
    XSyncCounter counter;
    
    /* create, set, and verify a counter */

    XSyncIntToValue(&create_value, 10L);
    counter = XSyncCreateCounter(dpy, create_value);
    XSyncIntToValue(&set_value, 20L);
    set_error_message("Failed to set counter");
    if (!XSyncSetCounter(dpy, counter, set_value))
    {
	report_error(LAST_SET_MESSAGE);
    }
    set_error_message("Failed to query counter");
    if (!XSyncQueryCounter(dpy, counter, &read_value))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (!XSyncValueEqual(set_value, read_value))
    {
	report_error("counter value was not correct after SetCounter");
    }

    /* try setting non-existent counter */

    set_error_message("Failed to destroy counter");
    XSyncDestroyCounter(dpy, counter);
    XSetErrorHandler(protocol_error_expected);
    set_error_message("Failed to set non-existent counter");
    if (!XSyncSetCounter(dpy, counter, set_value))
    {
	report_error(LAST_SET_MESSAGE);
    }

    if (protocol_error != BadCounter)
    {
	report_error("Failed to get Counter error when setting non-existent counter");
    }

    /* try to set system counter */

    counter = get_servertime_counter(dpy);
    XSetErrorHandler(protocol_error_expected);
    set_error_message("Failed to set system counter");
    if (!XSyncSetCounter(dpy, counter, set_value))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (protocol_error != BadAccess)
    {
	report_error("attempting to set SERVERTIME did not result in Access error");
    }
    end_test(dpy);
}

void test_change_counter()
{
    Display *dpy = begin_test("ChangeCounter");
    XSyncValue create_value, read_value, delta_value;
    Bool overflow;
    XSyncCounter counter;

    set_error_message("");
    XSyncIntToValue(&create_value, 0);
    counter = XSyncCreateCounter(dpy, create_value);

    XSyncIntToValue(&delta_value, 10);
    set_error_message("Failed to change counter");
    if (!XSyncChangeCounter(dpy, counter, delta_value))
    {
	report_error(LAST_SET_MESSAGE);
    }

    XSyncValueAdd(&create_value, create_value, delta_value, &overflow);

    set_error_message("");
    if (!XSyncQueryCounter(dpy, counter, &read_value))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (!XSyncValueEqual(create_value, read_value))
    {
	report_error("counter value was not correct after SetCounter");
    }

    /* try to change non-existent counter */

    set_error_message("Failed to destroy counter");
    if (!XSyncDestroyCounter(dpy, counter))
    {
	report_error(LAST_SET_MESSAGE);
    }

    XSetErrorHandler(protocol_error_expected);
    if (!XSyncChangeCounter(dpy, counter, delta_value))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (protocol_error != BadCounter)
    {
	report_error("attempting to change non-existent counter did not result in Counter error");
    }

    /* try to change system counter */

    counter = get_servertime_counter(dpy);
    XSetErrorHandler(protocol_error_expected);
    if (!XSyncChangeCounter(dpy, counter, delta_value))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (protocol_error != BadAccess)
    {
	report_error("attempting to change SERVERTIME did not result in Access error");
    }
    end_test(dpy);
}

void test_destroy_counter()
{
    Display *dpy = begin_test("DestroyCounter");
    XSyncCounter counter;
    XSyncValue create_value;

    XSyncIntsToValue(&create_value, 4, 987);
    set_error_message("Failed to create counter");
    counter = XSyncCreateCounter(dpy, create_value);

    set_error_message("Failed to destroy counter");
    if (!XSyncDestroyCounter(dpy, counter))
    {
	report_error(LAST_SET_MESSAGE);
    }

    /* try destroying a non-existent counter */

    XSetErrorHandler(protocol_error_expected);
    set_error_message("Failed to destroy non-existent counter");
    if (!XSyncDestroyCounter(dpy, counter))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (protocol_error != BadCounter)
    {
	report_error("Failed to get Counter error when destroying non-existent counter");
    }

    /* try destroying system counter */

    set_error_message("Failed while trying to destroy system counter");
    if (!XSyncDestroyCounter(dpy, get_servertime_counter(dpy)))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (protocol_error != BadAccess)
    {
	report_error("Failed to get Access error when destroying system counter");
    }

    end_test(dpy);
}

void compare_alarm_values(dpy, alarm, pxca_correct, mask)
Display *dpy;
XSyncAlarm alarm;
XSyncAlarmAttributes *pxca_correct;
unsigned long mask;
{
    XSyncAlarmAttributes xca_read;
    set_error_message("Failed to query alarm");
    XSetErrorHandler(protocol_error_unexpected);
    if (!XSyncQueryAlarm(dpy, alarm, &xca_read))
    {
	report_error(LAST_SET_MESSAGE);
    }

    if (mask & XSyncCACounter)
    {
	if (pxca_correct->trigger.counter != xca_read.trigger.counter)
	{
	    report_error("alarm counter values did not match");
	}
    }

    if (mask & XSyncCAValueType)
    {
	if (pxca_correct->trigger.value_type != xca_read.trigger.value_type)
	{
	    report_error("alarm value_type values did not match");
	}
    }

    if (mask & XSyncCAValue)
    {
	if (!XSyncValueEqual(pxca_correct->trigger.wait_value,
			     xca_read.trigger.wait_value))
	{
	    report_error("alarm wait_value values did not match");
	}
    }

    if (mask & XSyncCATestType)
    {
	if (pxca_correct->trigger.test_type != xca_read.trigger.test_type)
	{
	    report_error("alarm test_type values did not match");
	}
    }

    if (mask & XSyncCADelta)
    {
	if (!XSyncValueEqual(pxca_correct->delta, xca_read.delta))
	{
	    report_error("alarm delta values did not match");
	}
    }

    if (mask & XSyncCAEvents)
    {
	if (pxca_correct->events != xca_read.events)
	{
	    report_error("alarm events values did not match");
	}
    }
}

XSyncAlarm
create_alarm(dpy, counter, valtype, testval, testtype, delta, events)
Display *dpy;
XSyncCounter counter;
int valtype, testval, testtype, delta, events;
{
    XSyncAlarm alarm;
    XSyncAlarmAttributes xcavalues;
    unsigned long xcamask = XSyncCACounter|XSyncCAValueType|XSyncCAValue|
				XSyncCATestType|XSyncCADelta|XSyncCAEvents;
    
    xcavalues.trigger.counter = counter;
    xcavalues.trigger.value_type = valtype;
    XSyncIntToValue(&xcavalues.trigger.wait_value, testval);
    xcavalues.trigger.test_type = testtype;
    XSyncIntToValue(&xcavalues.delta, delta);
    xcavalues.events = events;
    xcamask = XSyncCACounter|XSyncCAValueType|XSyncCAValue|XSyncCATestType
		    |XSyncCADelta|XSyncCAEvents;
    XSetErrorHandler(protocol_error_unexpected);
    set_error_message("Failed to create alarm");
    if ((alarm = XSyncCreateAlarm(dpy, xcamask, &xcavalues)) == None)
    {
	report_error(LAST_SET_MESSAGE);
    }
    return alarm;
}

void test_create_alarm()
{
    Display *dpy = begin_test("CreateAlarm");
    XSyncAlarmAttributes xcavalues, xcavalues1, xcavalues_read;
    unsigned long xcamask;
    XSyncValue wv, dv, create_value;
    XSyncAlarm defaultalarm, alarm, alarm1;
    XSyncCounter counter;

    /* create default alarm */

    XSetErrorHandler(protocol_error_unexpected);
    set_error_message("Failed to create default alarm");
    defaultalarm = XSyncCreateAlarm(dpy, 0L, (XSyncAlarmAttributes *)NULL);

    /* create non-default alarm */

    XSyncIntToValue(&create_value, 5);
    set_error_message("Failed to create counter");
    counter = XSyncCreateCounter(dpy, create_value);
    xcavalues.trigger.counter = counter;
    xcavalues.trigger.value_type = XSyncAbsolute;
    XSyncIntsToValue(&wv, 100, 0);
    xcavalues.trigger.wait_value = wv;
    xcavalues.trigger.test_type = XSyncPositiveComparison;
    XSyncIntsToValue(&dv, 100, 0);
    xcavalues.delta = dv;
    xcavalues.events = False;
    xcamask = XSyncCACounter|XSyncCAValueType|XSyncCAValue|XSyncCATestType
		    |XSyncCADelta|XSyncCAEvents;
    set_error_message("Failed to create alarm");
    alarm = XSyncCreateAlarm(dpy, xcamask, &xcavalues);
    expect_no_events(dpy);

    compare_alarm_values(dpy, alarm, &xcavalues, xcamask);

    /* test for Counter error */

    xcavalues1 = xcavalues;
    xcavalues1.trigger.counter = ~0;
    XSetErrorHandler(protocol_error_expected);
    set_error_message("Failed to create alarm with bad value");
    alarm1 = XSyncCreateAlarm(dpy, xcamask, &xcavalues1);
    expect_no_events(dpy);
    if (protocol_error !=  BadCounter)
    {
	report_error("attempting to create alarm with bad counter did not result in Counter error");
    }

    /* test for Value error on trigger.value_type */

    xcavalues1 = xcavalues;
    xcavalues1.trigger.value_type  = -1;
    XSetErrorHandler(protocol_error_expected);
    alarm1 = XSyncCreateAlarm(dpy, xcamask, &xcavalues1);
    expect_no_events(dpy);
    if (protocol_error !=  BadValue)
    {
	report_error("attempting to create alarm with bad value_type did not result in Value error");
    }

    /* test for Value error on trigger.test_type */

    xcavalues1 = xcavalues;
    xcavalues1.trigger.test_type  = -1;
    XSetErrorHandler(protocol_error_expected);
    alarm1 = XSyncCreateAlarm(dpy, xcamask, &xcavalues1);
    expect_no_events(dpy);
    if (protocol_error != BadValue)
    {
	report_error("attempting to create alarm with bad test_type did not result in Value error");
    }

    /* test for Value error on events */

    xcavalues1 = xcavalues;
    xcavalues1.events = -1;
    XSetErrorHandler(protocol_error_expected);
    alarm1 = XSyncCreateAlarm(dpy, xcamask, &xcavalues1);
    expect_no_events(dpy);
    if (protocol_error != BadValue)
    {
	report_error("attempting to create alarm with bad events did not result in Value error");
    }
    end_test(dpy);
}

void change_alarm(dpy, alarm, pxca, mask, expect_err)
Display *dpy;
XSyncAlarm alarm;
XSyncAlarmAttributes *pxca;
unsigned long mask;
int expect_err;
{
    int status;

    set_error_message("Failed to change alarm");
    if (expect_err)
	XSetErrorHandler(protocol_error_expected);
    else
	XSetErrorHandler(protocol_error_unexpected);
    status = XSyncChangeAlarm(dpy, alarm, mask, pxca);
    if (expect_err)
    {
	if (protocol_error != expect_err)
	    report_error("Failed to get expected protocol error ChangeAlarm");
    }
    else
    {
	if (!status)
	    report_error(LAST_SET_MESSAGE);
	else
	    compare_alarm_values(dpy, alarm, pxca, mask);
    }
}


void test_change_alarm()
{
    Display *dpy = begin_test("ChangeAlarm");
    XSyncAlarmAttributes xcavalues, xcavalues1, xcavalues_read;
    unsigned long xcamask;
    XSyncValue wv, dv, create_value;
    XSyncAlarm defaultalarm, alarm, alarm1;
    XSyncCounter counter;

    /* create default alarm */

    XSetErrorHandler(protocol_error_unexpected);
    set_error_message("Failed to create default alarm");
    defaultalarm = XSyncCreateAlarm(dpy, 0L, (XSyncAlarmAttributes *)NULL);
    expect_no_events(dpy);

    /* change default alarm */

    XSyncIntToValue(&create_value, 8887);
    set_error_message("Failed to create counter");
    counter = XSyncCreateCounter(dpy, create_value);
    xcavalues.trigger.counter = counter;
    xcavalues.trigger.value_type = XSyncAbsolute;
    XSyncIntsToValue(&wv, 9000, 0);
    xcavalues.trigger.wait_value = wv;
    xcavalues.trigger.test_type = XSyncPositiveComparison;
    XSyncIntsToValue(&dv, 100, 0);
    xcavalues.delta = dv;
    xcavalues.events = False;
    xcamask = XSyncCACounter|XSyncCAValueType|XSyncCAValue|XSyncCATestType
		    |XSyncCADelta|XSyncCAEvents;
    change_alarm(dpy, defaultalarm, &xcavalues, xcamask, 0);
    expect_no_events(dpy);

    /* change alarm with non-existent counter */

    xcavalues.trigger.counter = (XSyncCounter)~0;
    change_alarm(dpy, defaultalarm, &xcavalues, XSyncCACounter, BadCounter);
    expect_no_events(dpy);
    
    /* change alarm with bad values */

    xcavalues.trigger.value_type = -1;
    change_alarm(dpy, defaultalarm, &xcavalues, XSyncCAValueType, BadValue);
    expect_no_events(dpy);

    xcavalues.trigger.test_type = -1;
    change_alarm(dpy, defaultalarm, &xcavalues, XSyncCATestType, BadValue);
    expect_no_events(dpy);

    xcavalues.trigger.value_type = -1;
    change_alarm(dpy, defaultalarm, &xcavalues, XSyncCAEvents, BadValue);
    expect_no_events(dpy);

    /* change a non-existent alarm */

    change_alarm(dpy, (XSyncAlarm)~0, &xcavalues, xcamask, BadAlarm);
    expect_no_events(dpy);

    end_test(dpy);
}


void test_query_alarm()
{
    Display *dpy = begin_test("QueryAlarm");
    XSyncAlarmAttributes xca_read;

    XSetErrorHandler(protocol_error_expected);
    XSyncQueryAlarm(dpy, (XSyncAlarm)~0, &xca_read);
    if (protocol_error != BadAlarm)
    {
	report_error("Failed to get Alarm error when querying non-existent alarm");
    }

    end_test(dpy);
}

void test_destroy_alarm()
{
    Display *dpy = begin_test("DestroyAlarm");
    XSyncAlarm alarm;
    XSyncAlarmAttributes xcavalues, xcavalues1, xcavalues_read;
    unsigned long xcamask;
    XSyncValue wv, dv, create_value;
    XSyncCounter counter;

    /* destroy a default alarm */

    set_error_message("Failed to create default alarm");
    alarm = XSyncCreateAlarm(dpy, 0L, (XSyncAlarmAttributes *)NULL);
    set_error_message("Failed to destroy alarm");
    XSyncDestroyAlarm(dpy, alarm);
    expect_alarmnotify(dpy, alarm, 0, 0, XSyncAlarmDestroyed, 0, True);

    /* destroy a non-default alarm */

    XSyncIntToValue(&create_value, 612);
    set_error_message("Failed to create counter");
    counter = XSyncCreateCounter(dpy, create_value);
    xcavalues.trigger.counter = counter;
    xcavalues.trigger.value_type = XSyncAbsolute;
    XSyncIntsToValue(&wv, 100, 0);
    xcavalues.trigger.wait_value = wv;
    xcavalues.trigger.test_type = XSyncPositiveComparison;
    XSyncIntsToValue(&dv, 100, 0);
    xcavalues.delta = dv;
    xcavalues.events = False;
    xcamask = XSyncCACounter|XSyncCAValueType|XSyncCAValue|XSyncCATestType
		    |XSyncCADelta|XSyncCAEvents;
    set_error_message("Failed to create alarm");
    alarm = XSyncCreateAlarm(dpy, xcamask, &xcavalues);
    set_error_message("Failed to destroy alarm");
    XSyncDestroyAlarm(dpy, alarm);
    expect_no_events(dpy);

    /* try to destroy a non-existent alarm */

    XSetErrorHandler(protocol_error_expected);
    set_error_message("Failed to destroy non-existent alarm");
    XSyncDestroyAlarm(dpy, alarm);
    expect_no_events(dpy);
    if (protocol_error != BadAlarm)
    {
	report_error("attempting to destroy non-existent alarm did not result in Alarm error");
    }

    end_test(dpy);
}

void test_using_alarms()
{
    Display *dpy = begin_test("Using Alarms");
    XSyncCounter counter;
    XSyncAlarm alarm, alarm2;
    Display *dpy2;
    XSyncAlarmAttributes xca;

    counter = create_counter(dpy, 0);

    /* test PositiveComparison */

    alarm = create_alarm(dpy, counter, XSyncAbsolute, /* test value */ 1,
			 XSyncPositiveComparison, /* delta */ 1, True);
    expect_no_events(dpy);
    set_counter(dpy, counter, 1); /* counter = test value */
    expect_alarmnotify(dpy, alarm, 1, 1, XSyncAlarmActive, 2, True);
    set_counter(dpy, counter, 3); /* counter > test value */
    expect_alarmnotify(dpy, alarm, 3, 2, XSyncAlarmActive, 4, True);
    destroy_alarm(dpy, alarm);
    expect_alarmnotify(dpy, alarm, 3, 4, XSyncAlarmDestroyed, 4, True);

    /* test NegativeComparison */

    alarm = create_alarm(dpy, counter, XSyncAbsolute, /* test value */ 2,
			 XSyncNegativeComparison, /* delta */ -1, True);
    expect_no_events(dpy);
    set_counter(dpy, counter, 2); /* counter = test value */
    expect_alarmnotify(dpy, alarm, 2, 2, XSyncAlarmActive, 1, True);
    set_counter(dpy, counter, 0); /* counter < test value */
    expect_alarmnotify(dpy, alarm, 0, 1, XSyncAlarmActive, -1, True);
    destroy_alarm(dpy, alarm);
    expect_alarmnotify(dpy, alarm, 0, -1, XSyncAlarmDestroyed, -1, True);

    /* test PositiveTransition */

    alarm = create_alarm(dpy, counter, XSyncAbsolute, /* test value */ 1,
			 XSyncPositiveTransition, /* delta */ 1, True);
    expect_no_events(dpy);
    set_counter(dpy, counter, 1); /* counter = test value */
    expect_alarmnotify(dpy, alarm, 1, 1, XSyncAlarmActive, 2, True);
    set_counter(dpy, counter, 4); /* counter > test value */
    expect_alarmnotify(dpy, alarm, 4, 2, XSyncAlarmActive, 3, True);
    destroy_alarm(dpy, alarm);
    expect_alarmnotify(dpy, alarm, 4, 3, XSyncAlarmDestroyed, 3, True);

    /* test NegativeTransition */

    alarm = create_alarm(dpy, counter, XSyncAbsolute, /* test value */ 3,
			 XSyncNegativeTransition, /* delta */ -1, True);
    expect_no_events(dpy);
    set_counter(dpy, counter, 3); /* counter = test value */
    expect_alarmnotify(dpy, alarm, 3, 3, XSyncAlarmActive, 2, True);
    set_counter(dpy, counter, 1); /* counter < test value */
    expect_alarmnotify(dpy, alarm, 1, 2, XSyncAlarmActive, 1, True);
    destroy_alarm(dpy, alarm);
    expect_alarmnotify(dpy, alarm, 1, 1, XSyncAlarmDestroyed, 1, True);

    /* test that events are not sent if we don't want them */

    alarm = create_alarm(dpy, counter, XSyncAbsolute, /* test value */ 3,
			 XSyncNegativeTransition, /* delta */ -1, False);
    expect_no_events(dpy);
    set_counter(dpy, counter, 3); /* counter = test value */
    expect_no_events(dpy);
    set_counter(dpy, counter, 1); /* counter < test value */
    expect_no_events(dpy);
    destroy_alarm(dpy, alarm);
    expect_no_events(dpy);

    /* test multiple clients selecting events on an alarm */

    if (!(dpy2 = XOpenDisplay("")))
    {
	report_error("could not open display a second time");
	goto nexttest;
    }
    XSynchronize(dpy2, True);

    set_counter(dpy, counter, 0); /* counter = test value */
    alarm = create_alarm(dpy, counter, XSyncAbsolute, /* test value */ 1,
			 XSyncPositiveComparison, /* delta */ 1, True);
    expect_no_events(dpy);

    xca.events = True;
    XSyncChangeAlarm(dpy2, alarm, XSyncCAEvents, &xca);

    set_counter(dpy, counter, 1); /* counter = test value */
    expect_alarmnotify(dpy, alarm, 1, 1, XSyncAlarmActive, 2, True);
    XSync(dpy2, False);
    expect_alarmnotify(dpy2, alarm, 1, 1, XSyncAlarmActive, 2, True);
    set_counter(dpy, counter, 3); /* counter > test value */
    expect_alarmnotify(dpy, alarm, 3, 2, XSyncAlarmActive, 4, True);
    XSync(dpy2, False);
    expect_alarmnotify(dpy2, alarm, 3, 2, XSyncAlarmActive, 4, True);
    destroy_alarm(dpy, alarm);
    expect_alarmnotify(dpy, alarm, 3, 4, XSyncAlarmDestroyed, 4, True);
    XSync(dpy2, False);
    expect_alarmnotify(dpy2, alarm, 3, 4, XSyncAlarmDestroyed, 4, True);

    XCloseDisplay(dpy2);

nexttest:

    /* test multiple alarms on a counter */

    if (!(dpy2 = XOpenDisplay("")))
    {
	report_error("could not open display a second time");
	goto nexttest1;
    }
    XSynchronize(dpy2, True);

    set_counter(dpy, counter, 0); /* counter = test value */
    alarm = create_alarm(dpy, counter, XSyncAbsolute, /* test value */ 1,
			 XSyncPositiveComparison, /* delta */ 1, False);
    alarm2 = create_alarm(dpy, counter, XSyncAbsolute, /* test value */ 1,
			 XSyncPositiveComparison, /* delta */ 1, False);
    expect_no_events(dpy);
    expect_no_events(dpy2);

    xca.events = True;
    XSyncChangeAlarm(dpy, alarm, XSyncCAEvents, &xca);
    XSyncChangeAlarm(dpy2, alarm2, XSyncCAEvents, &xca);

    set_counter(dpy, counter, 1); /* counter = test value */
    expect_alarmnotify(dpy, alarm, 1, 1, XSyncAlarmActive, 2, True);
    XSync(dpy2, False);
    expect_alarmnotify(dpy2, alarm2, 1, 1, XSyncAlarmActive, 2, True);
    set_counter(dpy, counter, 3); /* counter > test value */
    expect_alarmnotify(dpy, alarm, 3, 2, XSyncAlarmActive, 4, True);
    XSync(dpy2, False);
    expect_alarmnotify(dpy2, alarm2, 3, 2, XSyncAlarmActive, 4, True);
    destroy_alarm(dpy, alarm);
    expect_alarmnotify(dpy, alarm, 3, 4, XSyncAlarmDestroyed, 4, True);
    destroy_alarm(dpy2, alarm2);
    expect_alarmnotify(dpy2, alarm2, 3, 4, XSyncAlarmDestroyed, 4, True);

    XCloseDisplay(dpy2);

nexttest1:

    /* test alarm on SERVERTIME */
    {
	XSyncValue servertime;
	int testval;
	XSyncAlarmAttributes xca;

	counter = get_servertime_counter(dpy);
	XSyncQueryCounter(dpy, counter, &servertime);
	testval = XSyncValueLow32(servertime) + 10;
	alarm = create_alarm(dpy, counter, XSyncAbsolute, testval,
			     XSyncPositiveComparison, /* delta */ 0, True);
	sleep(1);
	XSync(dpy, False);
	expect_alarmnotify(dpy, alarm, 0, 0, XSyncAlarmInactive, 0,
			   False);
	destroy_alarm(dpy, alarm);
	expect_alarmnotify(dpy, alarm, 0, 0, XSyncAlarmDestroyed, 0,
			   False);

	/* test getting multiple events (delta != 0) */

	alarm = create_alarm(dpy, counter, XSyncRelative, 10,
			     XSyncPositiveComparison, /* delta */ 10, True);
	xca.events = False;
	change_alarm(dpy, alarm, &xca, XSyncCAEvents, 0);
	sleep(1);
	XSync(dpy, False);
	while (XPending(dpy))
	{
	    expect_alarmnotify(dpy, alarm, 0, 0, XSyncAlarmActive, 0,
			       False);
	}
	destroy_alarm(dpy, alarm);
    }

    end_test(dpy);
}

void
expect_counternotify(dpy, counter, waitval, counterval, more, destroyed)
Display *dpy;
XSyncCounter counter;
int waitval, counterval, more, destroyed;
{
    XEvent e;
    XSyncCounterNotifyEvent *pcne;
    XSyncValue wait_value, counter_value;

    if (XPending(dpy) == 0)
    {
	report_error("Failed to find CounterNotify event");
	return;
    }
    XNextEvent(dpy, &e);
    pcne = (XSyncCounterNotifyEvent *)&e;

    if (pcne->type != CounterNotify)
    {
	report_error("Failed to find CounterNotify event");
	return;
    }

    XSyncIntToValue(&counter_value, counterval);
    XSyncIntToValue(&wait_value, waitval);

    if (pcne->counter  != counter)
	report_error("counter in CounterNotify event was incorrect");

    if (pcne->count != more)
	report_error("event count in CounterNotify event was incorrect");

    if (pcne->destroyed != destroyed)
	report_error("destroyed field in CounterNotify event was incorrect");

    if (!XSyncValueEqual(pcne->wait_value, wait_value))
	report_error("wait_value in CounterNotify event was incorrect");

    if (!XSyncValueEqual(pcne->counter_value, counter_value))
	report_error("counter value in AlarmNotify event was incorrect");
}



void test_await()
{
    Display *dpy = begin_test("Await");
    XSyncWaitCondition wc[2];
    XSyncCounter counter, counter2;
    int pid;

    /* test an await that is satisfied immediately with no CounterNotify */

    counter = create_counter(dpy, 0);

    wc[0].trigger.counter = counter;
    wc[0].trigger.value_type = XSyncAbsolute;
    XSyncIntToValue(&wc[0].trigger.wait_value, 0);
    wc[0].trigger.test_type = XSyncPositiveComparison;
    XSyncIntToValue(&wc[0].event_threshold, 1);
    XSyncAwait(dpy, wc, 1);
    expect_no_events(dpy);

    /* test an await that is satisfied immediately with CounterNotify */

    XSyncIntToValue(&wc[0].event_threshold, 1);
    XSyncIntToValue(&wc[0].trigger.wait_value, -1);
    XSyncAwait(dpy, wc, 1);
    expect_counternotify(dpy, counter, -1, 0, 0, False);

    /* test await with two clients communicating via a counter */

    if ((pid = fork()) == -1)
    {
	report_error("Failed to fork");
    }
    else if (pid)
    { /* parent */
	XSyncIntToValue(&wc[0].event_threshold, 100);
	XSyncIntToValue(&wc[0].trigger.wait_value, 1);
	/* child will set the counter to wake us up */
	XSyncAwait(dpy, wc, 1);
	/* this should wake up the child */
	XSyncDestroyCounter(dpy, counter);
    }
    else
    { /* child */
	Display *dpy = XOpenDisplay("");
	if (!dpy)
	{
	    report_error("child failed to open display");
	    exit(-1);
	}
	XSynchronize(dpy, True);
	/* this should wake up the parent */
	set_counter(dpy, counter, 5);
	XSyncIntToValue(&wc[0].event_threshold, 100);
	XSyncIntToValue(&wc[0].trigger.wait_value, 10);
	/* parent will destroy the counter which will wake us up */
	XSyncAwait(dpy, wc, 1);
	XCloseDisplay(dpy);
	exit(0);
    }

    /* test an await on SERVERTIME */

    wc[0].trigger.counter = get_servertime_counter(dpy);
    wc[0].trigger.value_type = XSyncRelative;
    XSyncIntToValue(&wc[0].trigger.wait_value, 20);
    XSyncIntToValue(&wc[0].event_threshold, 1000);
    XSyncAwait(dpy, wc, 1);
    expect_no_events(dpy);
    
    end_test(dpy);
}

void test_get_priority()
{
    Display *dpy = begin_test("GetPriority");
    int priority;
    XSyncCounter counter;
    XSyncValue create_value;
    GC gc;

    set_error_message("Failed to get priority");
    if (!XSyncGetPriority(dpy, (XID)None, &priority))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (priority != 0)
    {
	report_error("default priority was not 0");
    }

    /* get priority using one of the client's resources */

    gc = DefaultGC(dpy,0);
    if (!XSyncGetPriority(dpy, XGContextFromGC(gc), &priority))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (priority != 0)
    {
	report_error("default priority was not 0");
    }

    /* get priority of an invalid client */

    XSetErrorHandler(protocol_error_expected);
    XSyncGetPriority(dpy, (XID)~0, &priority);
    if (protocol_error != BadMatch)
    {
	report_error("getting priority of invalid client did not result in BadMatch");
    }

    end_test(dpy);
}

void test_set_priority()
{
    Display *dpy = begin_test("SetPriority");
    int priority_set, priority_get;
    GC gc;

    priority_set = 100;
    set_error_message("Failed to set priority");
    if (!XSyncSetPriority(dpy, (XID)None, priority_set))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (!XSyncGetPriority(dpy, (XID)None, &priority_get))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (priority_get != priority_set)
    {
	report_error("priority read back was incorrect");
    }

    /* set priority using one of the client's resources */

    gc = DefaultGC(dpy,0);

    priority_set = 200;
    if (!XSyncSetPriority(dpy, XGContextFromGC(gc), priority_set))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (!XSyncGetPriority(dpy, XGContextFromGC(gc), &priority_get))
    {
	report_error(LAST_SET_MESSAGE);
    }
    if (priority_get != priority_set)
    {
	report_error("priority read back was incorrect");
    }

    /* set priority of an invalid client */

    XSetErrorHandler(protocol_error_expected);
    XSyncSetPriority(dpy, (XID)~0, priority_set);
    if (protocol_error != BadMatch)
    {
	report_error("setting priority of invalid client did not result in BadMatch");
    }

    end_test(dpy);
}


int main(argc, argv)
int argc;
char **argv;
{
    test_initialize();
    test_list_system_counters();
    test_create_counter();
    test_set_counter();
    test_change_counter();
    test_query_counter();
    test_destroy_counter();
    test_create_alarm();
    test_change_alarm();
    test_query_alarm();
    test_destroy_alarm();
    test_using_alarms();
    test_await();
    test_get_priority();
    test_set_priority();

    printf("SYNC total errors: %d\n", total_error_count);
    return total_error_count;
}
