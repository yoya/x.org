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

#ifndef _StringTableConverter_h
#define _StringTableConverter_h

/* 
 * StringTableConverter --  Convert a comma separated list of quote
 *                          enclosed strings into a StringTable resource.
 *
 * example:  xdiary*dayNames: "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
 *
 * A StringTable is an array of pointers to strings whose last entry is NULL.
 * If no strings are specified, the initial entry is NULL.  Double quotes
 * delimit the string, but two consecutive quotes become a single double quote
 * in the final string.  For example, """Wow!""" becomes "Wow!".
 *
 * int StringTableNumber(StringTable) -- Returns the number of strings in
 *                                       a specified string table.
 *
 * void StringTableDump(StringTable) -- Dumps the contents of a string table.
 *                                      Useful for debugging purposes only.
 *
 * StringTable StringTableCopy(StringTable) -- Allocates a new array, but uses
 *                                             the same string pointers.
 *                                             Be careful with this one.
 *
 */

typedef char** StringTable;

extern void StringTableConverter(
   XrmValuePtr,
   Cardinal*,
   XrmValuePtr,
   XrmValuePtr
);

extern int StringTableNumber(StringTable);
extern void StringTableDump(StringTable);
extern StringTable StringTableCopy(StringTable);

#ifndef XtRStringTable
#define XtRStringTable "StringTable"
#endif

#endif /* _StringTableConverter_h */
