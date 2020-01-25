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
#include "GravityP.h"

#define DOWNCASE(c) (isupper((c)) ? tolower((c)) : (c))
#define MAX_STRING_LEN 256

/*==========================================================================*/
/*                                 Quarks:                                  */
/*==========================================================================*/
XrmQuark QNorthWest;
XrmQuark QNorth;
XrmQuark QNorthEast;
XrmQuark QWest;
XrmQuark QCenter;
XrmQuark QEast;
XrmQuark QSouthWest;
XrmQuark QSouth;
XrmQuark QSouthEast;

XrmQuark QNW;
XrmQuark QN;
XrmQuark QNE;
XrmQuark QW;
XrmQuark QC;
XrmQuark QE;
XrmQuark QSW;
XrmQuark QS;
XrmQuark QSE;


/*==========================================================================*/
/*                         Gravity Type Converter:                          */
/*==========================================================================*/
static void downcase_string(char* source, char* dest) {
   for (; *source != 0; source++, dest++) {
      *dest = DOWNCASE(*source);
   }
   *dest = 0;
}


void GravityConverter(
   XrmValuePtr args,
   Cardinal *num_args,
   XrmValuePtr fromVal,
   XrmValuePtr toVal
) {
   static int initialized = FALSE;
   static Gravity gravity;
   XrmQuark q;
   char lowerName[MAX_STRING_LEN];

   if (!initialized) {
      /* Create quarks the first time we're called. */
      QNorthWest = XrmStringToQuark(NnorthWest);
      QNorth = XrmStringToQuark(Nnorth);
      QNorthEast = XrmStringToQuark(NnorthEast);
      QWest = XrmStringToQuark(Nwest);
      QCenter = XrmStringToQuark(Ncenter);
      QEast = XrmStringToQuark(Neast);
      QSouthWest = XrmStringToQuark(NsouthWest);
      QSouth = XrmStringToQuark(Nsouth);
      QSouthEast = XrmStringToQuark(NsouthEast);

      QNW = XrmStringToQuark(NnorthWest2);
      QN = XrmStringToQuark(Nnorth2);
      QNE = XrmStringToQuark(NnorthEast2);
      QW = XrmStringToQuark(Nwest2);
      QC = XrmStringToQuark(Ncenter2);
      QE = XrmStringToQuark(Neast2);
      QSW = XrmStringToQuark(NsouthWest2);
      QS = XrmStringToQuark(Nsouth2);
      QSE = XrmStringToQuark(NsouthEast2);
   }

   downcase_string((char*)fromVal->addr, lowerName);
   q = XrmStringToQuark(lowerName);

   toVal->size = sizeof(Gravity);
   toVal->addr = (XtPointer)&gravity;

   if (q == QNorthWest || q == QNW) {
      gravity = NorthWest;
   } else if (q == QNorth || q == QN) {
      gravity = North;
   } else if (q == QNorthEast || q == QNE) {
      gravity = NorthEast;
   } else if (q == QWest || q == QW) {
      gravity = West;
   } else if (q == QCenter || q == QC) {
      gravity = Center;
   } else if (q == QEast || q == QE) {
      gravity = East;
   } else if (q == QSouthWest || q == QSW) {
      gravity = SouthWest;
   } else if (q == QSouth || q == QS) {
      gravity = South;
   } else if (q == QSouthEast || q == QSE) {
      gravity = SouthEast;
   } else {
      XtStringConversionWarning(fromVal->addr, "RGravity");
      toVal->addr = NULL;
      toVal->size = 0;
   }
}
