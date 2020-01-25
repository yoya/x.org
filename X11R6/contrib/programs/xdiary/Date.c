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
#include <values.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include "DateP.h"
#include "StrTable.h"

#define DOWNCASE(c) (isupper((c)) ? tolower((c)) : (c))
#define STRING (0)
#define NUMBER (1)

static void process_digit_token(void);
static void process_alpha_token(void);
static int convert_month_string_to_number(void);
static int compare_string(char*);
static void construct_date(void);

char* default_month_names[] = {
   "january", "february", "march", "april", "may", "june", "july",
   "august", "september", "october", "november", "december"
};

static int days_in_month[] = {
   /* jan feb mar apr may jun jul aug sep oct nov dec */
   0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static StringTable month_names = default_month_names;
static int token[3];
static int state[3];
static int count;
static char* beg_ptr;
static char* cur_ptr;
static Date date;
static int european;


Date convert_string_to_date(StringTable table, char* string) {
   int in_alpha_token = FALSE;
   int in_digit_token = FALSE;
   date.month = 0;
   date.day = 0;
   date.year = 0;
   beg_ptr = string;
   cur_ptr = string;
   count = 0;
   european = FALSE;

   /* Was a string table specified? */
   if (table) {
      month_names = table;
   }

   while (*cur_ptr) {
      if (isalpha(*cur_ptr)) {
         if (in_digit_token) {
            /* Finished processing a digit token. */
            process_digit_token();
            in_digit_token = FALSE;
         } else if (in_alpha_token) {
            /* Still scanning an alpha token. */
         } else {
            /* Start processing an alpha token. */
            in_alpha_token = TRUE;
            beg_ptr = cur_ptr;
         }
      } else if (isdigit(*cur_ptr)) {
         if (in_alpha_token) {
            /* Finished processing a digit token. */
            process_digit_token();
            in_digit_token = FALSE;
         } else if (in_digit_token) {
            /* Still scannin a digit token. */
         } else {
            /* Start processing a digit token. */
            in_digit_token = TRUE;
            beg_ptr = cur_ptr;
         }
      } else {
         if (in_alpha_token) {
            /* Finished processing an alpha token. */
            process_alpha_token();
            in_alpha_token = FALSE;
         } else if (in_digit_token) {
            /* Finished processing a digit token. */
            process_digit_token();
            in_digit_token = FALSE;
         }

         /* Ignore garbage between tokens, but look for periods. */
         if (*cur_ptr == '.') {
            european = TRUE;
         }
      }
      cur_ptr++;
   }

   if (in_alpha_token) {
      process_alpha_token();
   } else if (in_digit_token) {
      process_digit_token();
   }

   construct_date();

   if (!is_valid_date(&date)) {
      date.month = 0;
      date.day = 0;
      date.year = 0;
   }

   return date;
}


static void process_alpha_token() {
   if (count < 3) {
      token[count] = convert_month_string_to_number();
      state[count] = STRING;
   }
   count++;
}


static void process_digit_token() {
   if (count < 3) {
      token[count] = atoi(beg_ptr);
      if (token[count] > MAXSHORT) {
         token[count] = 0;
      }
      state[count] = NUMBER;
   }
   count++;
}


static int convert_month_string_to_number() {
   int month;
   for (month = 0; month < 12; month ++) {
      if (compare_string(month_names[month])) {
         return month+1;
      }
   }

   /* Nothing matched. */
   return 0;
}


static int compare_string(char* string) {
   char* token_ptr = beg_ptr;
   char* string_ptr = string;

   while (token_ptr < cur_ptr) {
      if (DOWNCASE(*string_ptr) == DOWNCASE(*token_ptr)) {
         /* Letters match (case ignored) */
         token_ptr++;
         string_ptr++;
      } else {
         /* Doesn't match. */
         break;
      }
   }

   if (token_ptr == cur_ptr) {
      /* We got to the end without a mismatch. */
      return TRUE;
   } else {
      /* String and token didn't match. */
      return FALSE;
   }
}


static void construct_date() {
   date.day = 0;
   date.month = 0;
   date.year = 0;

   if (count == 1) {

      if (state[0] == NUMBER) {
         /* number, done */
      } else {
         /* string, error */
         return;
      }

      date.day = 0;
      date.month = 1;
      date.year = token[0];

   } else if (count == 2) {

      if (state[0] == STRING) {
         /* string */
         if (state[1] == NUMBER) {
            /* string number, done */
         } else {
            /* string string, error */
            return;
         }
      } else {
         /* number */
         if (state[1] == NUMBER) {
            /* number number, done */
         } else {
            /* number string, error */
            return;
         }
      }

      /* Month Year */
      date.day = 0;
      date.month = token[0];
      date.year = token[1];

   } else if (count == 3) {

      if (state[0] == STRING) {
         /* string */
         if (state[1] == NUMBER) {
            /* string number */
            if (state[2] == NUMBER) {
               /* string number number, done */
            } else {
               /* string number string, error */
               return;
            }
         } else {
            /* string string, error */
            return;
         }

         /* Month Day Year */
         date.month = token[0];
         date.day = token[1];
         date.year = token[2];

      } else {

         /* number */
         if (state[1] == STRING) {
            /* number string */
            if (state[2] == NUMBER) {
               /* number string number, done */
               /* Day Month Year */
               date.day = token[0];
               date.month = token[1];
               date.year = token[2];
            } else {
               /* number string string, error */
               return;
            }
         } else {
            /* number number */
            if (state[2] == NUMBER) {
               /* number number number, done */
               if (european) {
                  /* Day Month Year */
                  date.day = token[0];
                  date.month = token[1];
                  date.year = token[2];
               } else {
                  /* Month Day Year */
                  date.month = token[0];
                  date.day = token[1];
                  date.year = token[2];
               }
            } else {
               /* number number string, error */
               return;
            }
         }
      }
   }
}


int is_valid_date(Date* date_ptr) {
   if (date_ptr->month < 1 || date_ptr->month > 12) {
      /* None or an invalid month was specified. */
      return FALSE;
   }

   if (date_ptr->year <= 0) {
      /* No year was specified. */
      return FALSE;
   }

   if (date_ptr->year < 100) {
      /* Bump a two digit year into the 20th century. */
      date_ptr->year += 1900;
   }

   if (!date_ptr->day) {
      /* It's okay if no day is specified. */
      return TRUE;
   } else {
      if (date_ptr->day < 0 || date_ptr->day > days_in_month[date_ptr->month]) {
         /* An invalid day was specified. */
         return FALSE;
      } else {
         /* One last error check. */
         if (date_ptr->month == FEBRUARY 
             && date_ptr->day == 29 
             && !A_LEAP_YEAR(date_ptr->year)) {
            return FALSE;
         } else {
            /* A valid day was specified. */
            return TRUE;
         }
      }
   }
}
