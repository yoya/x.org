/*
 * Copyright (c) 1994  Harris Computer Systems Division
 *
 * The X Consortium, and any party obtaining a copy of these files from the X
 * Consortium, directly or indirectly, is granted, free of charge, a full and
 * unrestricted irrevocable, world-wide, paid up, royalty-free, nonexclusive
 * right and license to deal in this software and documentation files (the
 * "Software"), including without limitation the rights to use, copy, modify,
 * merge, publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons who receive copies from any such party to do so.
 * This license includes without limitation a license to do the foregoing
 * actions under any patents of the party supplying this software to the X
 * Consortium.
 * 
 * BECAUSE THE PROGRAM IS LICENSED FREE OF CHARGE, THERE IS NO WARRANTY FOR
 * THE PROGRAM, TO THE EXTENT PERMITTED BY APPLICABLE LAW.  EXCEPT WHEN
 * OTHERWISE STATED IN WRITING THE COPYRIGHT HOLDERS AND/OR OTHER PARTIES
 * PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS
 * TO THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE
 * PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING,
 * REPAIR OR CORRECTION.
 * 
 * IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING WILL
 * ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MAY MODIFY AND/OR REDISTRIBUTE
 * THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
 * GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
 * USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
 * DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD
 * PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),
 * EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES.
 */

#include <stdio.h>
#include <ctype.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include "DayNameP.h"

#define DOWNCASE(c) (isupper((c)) ? tolower((c)) : (c))
#define MAX_STRING_LEN 256

/*==========================================================================*/
/*                                 Quarks:                                  */
/*==========================================================================*/
XrmQuark XtQSunday;
XrmQuark XtQMonday;
XrmQuark XtQTuesday;
XrmQuark XtQWednesday;
XrmQuark XtQThursday;
XrmQuark XtQFriday;
XrmQuark XtQSaturday;


/*==========================================================================*/
/*                         DayName Type Converter:                          */
/*==========================================================================*/
static void downcase_string(char* source, char* dest) {
   for (; *source != 0; source++, dest++) {
      *dest = DOWNCASE(*source);
   }
   *dest = 0;
}


void DayNameConverter(
   XrmValuePtr args,
   Cardinal* num_args,
   XrmValuePtr fromVal,
   XrmValuePtr toVal
) {
   static int initialized = FALSE;
   static XtDayName DayName;
   XrmQuark q;
   char lowerName[MAX_STRING_LEN];

   if (!initialized) {
      /* Create quarks the first time we're called. */
      XtQSunday = XrmStringToQuark(XtNsunday);
      XtQMonday = XrmStringToQuark(XtNmonday);
      XtQTuesday = XrmStringToQuark(XtNtuesday);
      XtQWednesday = XrmStringToQuark(XtNwednesday);
      XtQThursday = XrmStringToQuark(XtNthursday);
      XtQFriday = XrmStringToQuark(XtNfriday);
      XtQSaturday = XrmStringToQuark(XtNsaturday);
   }

   downcase_string((char*)fromVal->addr, lowerName);
   q = XrmStringToQuark(lowerName);

   toVal->size = sizeof(XtDayName);
   toVal->addr = (XtPointer) &DayName;

   if (q == XtQSunday) {
      DayName = Sunday;
   } else if (q == XtQMonday) {
      DayName = Monday;
   } else if (q == XtQTuesday) {
      DayName = Tuesday;
   } else if (q == XtQWednesday) {
      DayName = Wednesday;
   } else if (q == XtQThursday) {
      DayName = Thursday;
   } else if (q == XtQFriday) {
      DayName = Friday;
   } else if (q == XtQSaturday) {
      DayName = Saturday;
   } else {
      XtStringConversionWarning(fromVal->addr, "XtRDayName");
      toVal->addr = NULL;
      toVal->size = 0;
   }
}
