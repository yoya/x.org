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
 * diary.c -- routines to read, manage, and save the diary file.
 *
 * Diary format:  newline terminated date string followed by a
 *                newline terminated quote enclosed string.
 *                The string may contain newlines and quotes, but
 *                quotes must be doubled (i.e. "" becomes ").
 *
 * The diary format is very simple so that it can easily be updated in a text
 * editor for people who want to add text to a date but are too impatient to
 * bring up the xdiary application.  Other tools should easily be able to
 * parse this format.
 */


/*---------------------------------------------------------------------------*/
/*                               Header Files                                */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <ctype.h>
#include <values.h>
#include <X11/Intrinsic.h>
#include "Date.h"


/*---------------------------------------------------------------------------*/
/*                          Global Types And Macros                          */
/*---------------------------------------------------------------------------*/
#define START_LENGTH 20000
#define GROW_LENGTH 10000
#define INIT_YEARS 10
#define NEWLINE '\n'
#define QUOTE   '\"'
#define STRING 0
#define DATE 1
#define EMPTYSTRING ""

typedef struct day_entry_type {
   char* text;
   int freeable;
} day_entry;

typedef struct month_entry_type {
   struct day_entry_type day[31];
} month_entry;

typedef struct year_entry_type {
   int year;
   struct month_entry_type* month[12];
   struct year_entry_type* next;
   struct year_entry_type* prev;
} year_entry;

static year_entry head;
static year_entry tail;


/*---------------------------------------------------------------------------*/
/*                              Static Routines                              */
/*---------------------------------------------------------------------------*/
static char* read_diary(char* file_name) {
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
   while (c != EOF) {
      c = getc(file);
      char_ptr[i++] = c;

      if (i >= current_length) {
         current_length += GROW_LENGTH;
         char_ptr = (char*)XtRealloc(char_ptr, current_length);
      }
   }

   fclose(file);
   return(char_ptr);
}


/*---------------------------------------------------------------------------*/
/*                              Global Routines                              */
/*---------------------------------------------------------------------------*/
void install(Date date, char* string, int freeable) {
   year_entry* year_ptr = &head;
   year_entry* save_ptr;
   year_entry* last_ptr;
   month_entry* month_ptr;
   day_entry* day_ptr;

   /* Find or create the year entry. */
   while (year_ptr) {
      if (year_ptr->year < date.year) {
         /* Not yet, try the next year entry. */
         last_ptr = year_ptr;
         year_ptr = year_ptr->next;
      } else if (year_ptr->year > date.year) {
         /* No year entry exists, create one. */
         save_ptr = year_ptr;
         year_ptr = (year_entry*)XtCalloc(1, sizeof(year_entry));
         last_ptr->next = year_ptr;
         year_ptr->next = save_ptr;
         year_ptr->prev = last_ptr;
         save_ptr->prev = year_ptr;
         year_ptr->year = date.year;
         break;
      } else {
         /* Found it. */
         break;
      }
   }/*while*/

   /* Find or create the corresponding month array. */
   month_ptr = year_ptr->month[date.month-1];
   if (!month_ptr) {
      month_ptr = (month_entry*)XtCalloc(1, sizeof(month_entry));
      year_ptr->month[date.month-1] = month_ptr;
   }

   day_ptr = &month_ptr->day[date.day-1];
   if (!day_ptr->text) {
      /* No old text exists. */
      day_ptr->text = string;
      day_ptr->freeable = freeable;
   } else {
      if (day_ptr->freeable) {
         XtFree(day_ptr->text);
      }
      day_ptr->text = string;
      day_ptr->freeable = freeable;
   }
}


char* retrieve(Date date) {
   year_entry* year_ptr = &head;
   month_entry* month_ptr;
   char* text_ptr;

   while (year_ptr) {
      if (year_ptr->year == date.year) {
         month_ptr = year_ptr->month[date.month-1];
         if (month_ptr) {
            text_ptr = month_ptr->day[date.day-1].text;
            if (text_ptr) {
               return (text_ptr);
            }
            break;
         }
         break;
      } else if (year_ptr->year > date.year) {
         break;
      }
      year_ptr = year_ptr->next;
   }
   return EMPTYSTRING;
}


int not_empty(char* string) {
   /* Catch the null string. */
   if (*string == NULL) {
      return FALSE;
   }

   while (*string) {
      if (!isspace(*string)) {
         return TRUE;
      }
      string++;
   }
   return FALSE;
}


Date next_entry(Date date) {
   year_entry* year_ptr = &head;
   month_entry* month_ptr;
   char* text_ptr;
   Date return_date;
   int month;
   int day;

   while (year_ptr) {
      if (year_ptr->year == date.year) {
         /* Same year as date specified. */
         for (month=date.month-1; month < 12; month++) {
            month_ptr = year_ptr->month[month];
            if (month_ptr) {
               if (month == date.month-1) {
                  /* Same month as date specified. */
                  day = date.day;
               } else {
                  day = 0;
               }
               for (; day < 31; day++) {
                  text_ptr = month_ptr->day[day].text;
                  if (text_ptr && not_empty(text_ptr)) {
                     return_date.year  = year_ptr->year;
                     return_date.month = month+1;
                     return_date.day   = day+1;
                     return(return_date);
                  }
               }
            }
         }
      } else if (year_ptr->year > date.year) {
         for (month=0; month < 12; month++) {
            month_ptr = year_ptr->month[month];
            if (month_ptr) {
               for (day=0; day < 31; day++) {
                  text_ptr = month_ptr->day[day].text;
                  if (text_ptr && not_empty(text_ptr)) {
                     return_date.year  = year_ptr->year;
                     return_date.month = month+1;
                     return_date.day   = day+1;
                     return(return_date);
                  }
               }
            }
         }
      }
      year_ptr = year_ptr->next;
   }

   /* No entry following the specified date was found. */
   return_date.year = 0;
   return_date.month = 0;
   return_date.day = 0;
   return return_date;
}


Date prev_entry(Date date) {
   year_entry* year_ptr = &tail;
   month_entry* month_ptr;
   char* text_ptr;
   Date return_date;
   int month;
   int day;

   while (year_ptr) {
      if (year_ptr->year == date.year) {
         /* Same year as date specified. */
         for (month=date.month-1; month >= 0; month--) {
            month_ptr = year_ptr->month[month];
            if (month_ptr) {
               if (month == date.month-1) {
                  /* Same month as date specified. */
                  day = date.day-2;
               } else {
                  day = 30;
               }
               for (; day >= 0; day--) {
                  text_ptr = month_ptr->day[day].text;
                  if (text_ptr && not_empty(text_ptr)) {
                     return_date.year  = year_ptr->year;
                     return_date.month = month+1;
                     return_date.day   = day+1;
                     return(return_date);
                  }
               }
            }
         }
      } else if (year_ptr->year < date.year) {
         for (month=11; month >= 0; month--) {
            month_ptr = year_ptr->month[month];
            if (month_ptr) {
               for (day=30; day >= 0; day--) {
                  text_ptr = month_ptr->day[day].text;
                  if (text_ptr && not_empty(text_ptr)) {
                     return_date.year  = year_ptr->year;
                     return_date.month = month+1;
                     return_date.day   = day+1;
                     return(return_date);
                  }
               }
            }
         }
      }
      year_ptr = year_ptr->prev;
   }

   /* No entry following the specified date was found. */
   return_date.year = 0;
   return_date.month = 0;
   return_date.day = 0;
   return return_date;
}




void save_diary(char* file_name) {
   year_entry* year_ptr = &head;
   month_entry* month_ptr;
   char* text_ptr;
   FILE* file;
   int month;
   int day;

   file = fopen(file_name, "w");
   if (!file) {
      fprintf(stderr, "xdiary: fopen of file \"%s\" failed\n", file_name);
      return;
   }

   while(year_ptr) {
      for (month=0; month < 12; month++) {
         month_ptr = year_ptr->month[month];
         if (month_ptr) {
            for (day=0; day < 31; day++) {
               text_ptr = month_ptr->day[day].text;
               if (text_ptr && not_empty(text_ptr)) {
                  fprintf(
                     file,
                     "%d/%d/%d\n",
                     month+1, day+1, year_ptr->year
                  );
                  putc(QUOTE, file);
                  while (*text_ptr) {
                     putc(*text_ptr, file);
                     if (*text_ptr == QUOTE) {
                        putc(QUOTE, file);
                     }
                     text_ptr++;
                  }
                  putc(QUOTE, file);
                  putc(NEWLINE, file);
                  putc(NEWLINE, file);
               }
            }
         }
      }
      year_ptr = year_ptr->next;
   }

   fclose(file);
}


#define parse_error(str) \
   (fprintf( \
      stderr, \
      "xdiary: file \"%s\", line %d: %s\n", \
      file_name, \
      current_line, \
      (str) \
   ))


int init_diary(char* file_name) {
   char* file = read_diary(file_name);
   char* fileptr = &file[0];
   char* start_of_string = fileptr;
   char* end_of_string = fileptr;
   int parsing_date = FALSE;
   int parsing_string = FALSE;
   int current_line = 1;
   int token_last_seen = STRING;
   Date date;

   /* Initialize the head and tail to simplify everything. */
   tail.year = MAXINT;
   tail.next = NULL;
   tail.prev = &head;

   head.year = 0;
   head.next = &tail;
   head.prev = NULL;
   
   if (!file) {
      fprintf(stderr, "xdiary: file \"%s\" will be created\n", file_name);
      return FALSE;
   }

   while ((int)*fileptr != EOF) {
      switch (*fileptr) {
      case QUOTE:
         if (parsing_string) {
            if (fileptr[1] == QUOTE) {
               /* Change two quotes to one. */
               *end_of_string = QUOTE;
               end_of_string++;
               fileptr++;
            } else {
               /* End of string. */
               *end_of_string = 0;
               parsing_string = FALSE;
               if (token_last_seen == STRING) {
                  parse_error("date missing");
               } else {
                  install(date, start_of_string, FALSE);
               }
               token_last_seen = STRING;
               start_of_string = end_of_string = fileptr + 1;
            }
         } else {
            /* Must be the start of a new string. */
            parsing_string = TRUE;
            start_of_string = end_of_string = fileptr + 1;
         }
         break;
      case NEWLINE:
         if (parsing_date) {
            /* Finished parsing a date. */
            parsing_date = FALSE;
            *end_of_string = 0;
            if (token_last_seen == DATE) {
               parse_error("text missing");
            }
            token_last_seen = DATE;
            date = convert_string_to_date(
               default_month_names,
               start_of_string
            );
            if (date.year == 0) {
               parse_error("bad date");
               token_last_seen = STRING;
            }
         } else if (parsing_string) {
            /* Newline is part of a string. */
            *end_of_string = *fileptr;
            end_of_string++;
         }
         current_line++;
         break;
      default:
         if (parsing_date || parsing_string) {
            /* Part of string. */
            *end_of_string = *fileptr;
            end_of_string++;
         } else {
            /* If character is signigicant, start parsing date. */
            if (!isspace(*fileptr)) {
               parsing_date = TRUE;
               end_of_string = start_of_string = fileptr;
               end_of_string++;
            }
         }
      }
      fileptr++;
   }
   return TRUE;
}


#ifdef DEBUG
void dump_dairy() {
   year_entry* year_ptr = &head;
   int month;
   int day;

   while(year_ptr) {
      for (month=0; month < 12; month++) {
         if (year_ptr->month[month]) {
            for (day=0; day < 31; day++) {
               if (year_ptr->month[month]->day[day].text) {
                  printf(
                     "date=%d/%d/%d\nfreeable=%d\ntext=\"%s\"\n",
                     year_ptr->year, month, day,
                     year_ptr->month[month]->day[day].freeable,
                     year_ptr->month[month]->day[day].text
                  );
               }
            }
         }
      }
      year_ptr = year_ptr->next;
   }
}
#endif
