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
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include "StrTable.h"

#define QUOTE     '\"'
#define BACKSLASH '\\'
#define INIT_SIZE 10
#define GROW_BY 10

/* Copy a string, replacing two consecutive double quotes with a single. */
static void string_copy(
   char* dest_string,
   char* source_string,
   int length
) {
   char* dest_ptr = dest_string;
   char* src_ptr = source_string;
   char* end_ptr = src_ptr + length;

   while (src_ptr < end_ptr) {
      if (*src_ptr == QUOTE) {
         src_ptr++;
      }
      *dest_ptr = *src_ptr;
      dest_ptr++;
      src_ptr++;
   }
   
   /* Null terminate the string */
   *dest_ptr = NULL;
}


/*
 * Convert a string containing quoted strings into a string table.
 * Two consecutive quotes inside a "string" become a single quote.
 * This algorithm just ignores all characters between each "string".
 * Not the most robust thing I ever wrote, but it'll do the job.
 */
void StringTableConverter(
   XrmValuePtr args,
   Cardinal* num_args,
   XrmValuePtr fromVal,
   XrmValuePtr toVal
) {
   static char** array;
   int num_allocated;
   int token_len;
   char* beg_ptr = (char*)fromVal->addr;
   char* cur_ptr = beg_ptr;
   int token_num = 0;
   int in_token = FALSE;

   num_allocated = INIT_SIZE;
   array = (char**)XtMalloc(num_allocated*sizeof(char*));

   while (*cur_ptr) {
      if (*cur_ptr == QUOTE) {
         if (in_token) {
            if (*(cur_ptr+sizeof(char)) == QUOTE) {
               /* Two consecutive quotes become one quote. */
               cur_ptr++;
            } else {
               /* Finished with this token, ignore final quote. */
               token_len = cur_ptr - beg_ptr;
               array[token_num] = (char*)XtMalloc((token_len+1)*sizeof(char));
               string_copy(array[token_num], beg_ptr, token_len);
               in_token = FALSE;
               token_num++;

               /* Allocate more array space if necessary. */
               if (token_num == num_allocated) {
                  num_allocated += GROW_BY;
                  array = (char**)XtRealloc(
                     (char*)array, num_allocated*sizeof(char*)
                  );
               }
            }

         } else {
            /* We've got a new token, skip leading quote */
            beg_ptr = cur_ptr;
            beg_ptr++;
            in_token = TRUE;
         }
      }
      cur_ptr++;
   }

   if (in_token) {
      /* Exited above loop still in a token. */
      fprintf(stderr,
         "StringTableConverter: Unmatched quote in resource string.\n"
      );
      toVal->addr = NULL;
      toVal->size = 0;
   } else {
      /* Null terminate the array. */
      array[token_num] = NULL;

      /* Load the return value. */
      toVal->addr = (XPointer) &array;
      toVal->size = sizeof(array);
   }
}


/* Return the number of strings in a string table. */
int StringTableNumber(StringTable table) {
   int number = -1;
   while (table[++number]);
   return number;
}


/* Dump the specified string table.  For debugging only. */
void StringTableDump(StringTable table) {
   int number = -1;
   while (table[++number]) {
      printf("%d: %s\n", number, table[number]);
   }
}


/* Create new array with old contents. */
StringTable StringTableCopy(StringTable old) {
   int i;
   int length = StringTableNumber(old) + 1;
   StringTable new = (StringTable)XtMalloc(length * sizeof(char*));

   for (i=0; i < length; i++) {   
      new[i] = old[i];
   }

   return new;
}
