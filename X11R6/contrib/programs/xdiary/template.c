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

/*
 * template.c -- routines to read and manage a diary template.
 */


/*===========================================================================*/
/*                               Header Files                                */
/*===========================================================================*/
#include <stdio.h>
#include <ctype.h>
#include <values.h>
#include <X11/Intrinsic.h>


/*===========================================================================*/
/*                          Global Types And Macros                          */
/*===========================================================================*/
#define START_LENGTH 1000
#define GROW_LENGTH 1000

static template_length;


/*===========================================================================*/
/*                              Static Routines                              */
/*===========================================================================*/
static char* read_template(char* file_name) {
   int i = 0;
   int c = 0;
   char* char_ptr;
   long current_length = START_LENGTH;
   FILE* file;

   char_ptr = (char*)XtMalloc(current_length);

   file = fopen(file_name, "r");
   if (!file) {
      return ((char*)NULL);
   }

   /* Just read the whole thing into memory to minimize file I/O time */
   c = getc(file);
   while (c != EOF) {
      char_ptr[i++] = c;

      if (i >= current_length) {
         current_length += GROW_LENGTH;
         char_ptr = (char*)XtRealloc(char_ptr, current_length);
      }

      c = getc(file);
   }

   char_ptr[i] = NULL;   
   template_length = i;

   fclose(file);
   return(char_ptr);
}


static char* compressed_template;

static char* compress(char* uncompressed) {
   char* buffer = (char*)XtCalloc(template_length+1, sizeof(char));
   char* compressed = buffer;
   
   while (*uncompressed) {
      if (isspace(*uncompressed)) {
         uncompressed++;
      } else {
         *compressed = *uncompressed;
         compressed++;
         uncompressed++;
      }
   }

   return buffer;
}


/*===========================================================================*/
/*                              Global Routines                              */
/*===========================================================================*/
char* template;


int init_template(char* file_name) {
   template = read_template(file_name);

   if (!template) {
      /* The template specified does not exist. */
      return FALSE;
   }

   compressed_template = compress(template);
   return TRUE;
}


int entry_modified(char* entry) {
   char* temp = compressed_template;

   while(*entry && *temp) {
      if (isspace(*entry)) {
         entry++;
      } else {
         if (*entry != *temp) {
            return TRUE;
         } else {
            entry++;
            temp++;
         }
      }
   }

   if (*entry) {
      return TRUE;
   } else {
      return FALSE;
   }
}
