/* ********************************************************************** *\
 *         Copyright IBM Corporation 1988,1991 - All Rights Reserved      *
 *        For full copyright information see:'andrew/config/COPYRITE'     *
\* ********************************************************************** */
/* $Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/ams/libs/ms/RCS/prsdate.y,v 2.6 1991/09/13 16:46:27 bobg Exp $ */
/* $ACIS: $ */
/* $Source: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/ams/libs/ms/RCS/prsdate.y,v $ */

/*
 *  parsedate - parse date specification. Originally pdate.
 *
 */

%start date_time
%token NUMBER NUMBER4 NUMBER4YEAR NUMBER2 WEEKDAY MONTH HOUR SHOUR
%token TODAY NOW TONIGHT NEXT THIS DAY WEEK FORTNIGHT
%token EVERY FROM BEFORE THE A AT ON LAST AFTER IN OF AGO WORD_MONTH
%token NOON AMPM TIMEKEY
%token AND NWORD NTHWORD ST ND RD TH
%token CHRISTMAS NEW YEAR
%token JUNK
%token ZONE

/* Resolve shift/reduce conflicts on THE */

%left THE

%{
#include <andrewos.h> /* sys/types.h sys/time.h */
#include <setjmp.h>

/* used to include c.h, however only dependency was CERROR */

#define CERROR (-1)


# define MAXPEEP 3
# define NDTMS 10
# define NOYEAR -1901
/* PTM: indicates ptm */
# define PTM -1
/* CURRTM: indicates current time */
# define CURRTM -2
/* CURRDATE: indicates current date (time set to -1). */
# define CURRDATE -3
/* PAST, FUTURE: logical values for past and future */
# define PAST 1
# define FUTURE 0

static int junk_err;			/* junk token give error? */
static int ppf;				/* past, or future */
static struct tm scurrtm;
static struct tm *currtm = &scurrtm;	/* current time */
static char *strp;			/* current position in date string */
static int delim;			/* previous field delimiter */
static jmp_buf errbuf;			/* jump location for parse errors */
extern char *nxtarg();
extern long time();
extern struct tm *localtime();
struct stable { char *text;  int token;  int lexval; };
static int shour, tmkey;
static int zone;	/* Offset in minutes from GMT specified */
static struct tm ptm;
static int pcount;
static int result;
static char *unkstring = "unknown error";

/* Date structure for constraining dates */
struct dtm 
{ int repn; 				     /* Current representation */
  int days;
  struct tm tm;
  int count;
};

static struct dtm dtm[NDTMS];
static int dtmused[NDTMS];

/* Representations */
# define RDAYS 0
# define RTM 1

/*
 *  Month and week day names (upper case only) and other words.
 */
struct stable strings[] =
{
  { "JAN*UARY", MONTH, 0 },		/* months (0-11) */
  { "FEB*RUARY", MONTH, 1 },
  { "MAR*CH", MONTH, 2 },
  { "APR*IL", MONTH, 3 },
  { "MAY", MONTH, 4 },
  { "JUN*E", MONTH, 5 },
  { "JUL*Y", MONTH, 6 },
  { "AUG*UST", MONTH, 7 },
  { "SEP*TEMBER", MONTH, 8 },
  { "OCT*OBER", MONTH, 9 },
  { "NOV*EMBER", MONTH, 10 },
  { "DEC*EMBER", MONTH, 11 },
  { "SUN*DAY", WEEKDAY, 0 },	/* days of the week (0-6) */
  { "MON*DAY", WEEKDAY, 1 },
  { "TUE*SDAY", WEEKDAY, 2 },
  { "WED*NESDAY", WEEKDAY, 3 },
  { "THU*RSDAY", WEEKDAY, 4 },
  { "FRI*DAY", WEEKDAY, 5 },
  { "SAT*URDAY", WEEKDAY, 6 },
  { "YESTERDAY", TODAY, -1 },	/* relative to today */
  { "TODAY", TODAY, 0 },
  { "TONIGHT", TONIGHT, 0 },
  { "NOW", NOW, 0 },
  { "AGO", AGO, 0 },
  { "TOMORROW", TODAY, 1 },
  { "NEXT", NEXT, 0 },		/* keywords */
  { "THIS", THIS, 0 },
  { "DAY*S", DAY, 0 },
  { "WEEK*S", WEEK, 0 },
  { "MONTH*S", WORD_MONTH, 0 },
  { "YEAR*S", YEAR, 0 },
  { "FORTNIGHT", FORTNIGHT, 0 },	/* two weeks (Australian) */
/*  { "EVERY", EVERY, 0 }, */
  { "FROM", FROM, 0 },
  { "AFTER", AFTER, 0 },
  { "BEFORE", BEFORE, 0 },
  { "LAST", LAST, 0 },
  { "THE", THE, 0 },
/*  { "A", A, 0 }, */
  { "AT", AT, 0 },
  { "ON", ON, 0 },
  { "IN", IN, 0 },
  { "OF", OF, 0 },
/*  { "AND", AND, 0 }, */
  { "MORNING", TIMEKEY, 0 },	/* time keywords. Morning is 0:00 - 11:59 */
  { "AFTERNOON", TIMEKEY, 12 }, /* Afternoon is 12:00 - 23:59 */
  { "EVENING", TIMEKEY, 15 },	/* Evening is 15:00 - 02:59 */
  { "NIGHT", TIMEKEY, 17 },	/* Night is 17:00 - 04:59 */
  { "NOON", NOON, 12 },		/* time specifications */
  { "MIDNIGHT", NOON, 24 },
  { "ONE", NWORD, 1 },		/* numbers up to 19 */
  { "TWO", NWORD, 2 },
  { "THREE", NWORD, 3 },
  { "FOUR", NWORD, 4 },
  { "FIVE", NWORD, 5 },
  { "SIX", NWORD, 6 },
  { "SEVEN", NWORD, 7 },
  { "EIGHT", NWORD, 8 },
  { "NINE", NWORD, 9 },
  { "TEN", NWORD, 10 },
  { "ELEVEN", NWORD, 11 },
  { "TWELVE", NWORD, 12 },
  { "THIRTEEN", NWORD, 13 },
  { "FOURTEEN", NWORD, 14 },
  { "FIFTEEN", NWORD, 15 },
  { "SIXTEEN", NWORD, 16 },
  { "SEVENTEEN", NWORD, 17 },
  { "EIGHTEEN", NWORD, 18 },
  { "NINETEEN", NWORD, 19 },
  { "FIRST", NTHWORD, 1 },		/* number up to 19th */
  { "SECOND", NTHWORD, 2 },
  { "THIRD", NTHWORD, 3 },
  { "FOURTH", NTHWORD, 4 },
  { "FIFTH", NTHWORD, 5 },
  { "SIXTH", NTHWORD, 6 },
  { "SEVENTH", NTHWORD, 7 },
  { "EIGHT", NTHWORD, 8 },
  { "NINTH", NTHWORD, 9 },
  { "TENTH", NTHWORD, 10 },
  { "ELEVENTH", NTHWORD, 11 },
  { "TWELFTH", NTHWORD, 12 },
  { "THIRTEENTH", NTHWORD, 13 },
  { "FOURTEENTH", NTHWORD, 14 },
  { "FIFTEENTH", NTHWORD, 15 },
  { "SIXTEENTH", NTHWORD, 16 },
  { "SEVENTEENTH", NTHWORD, 17 },
  { "EIGHTEENTH", NTHWORD, 18 },
  { "NINETEENTH", NTHWORD, 19 },
  { "ST", ST, 0 },		/* for 1st */
  { "ND", ND, 0 },		/* 2nd */
  { "RD", RD, 0 },		/* 3rd */
  { "TH", TH, 0 },		/* nth */
  { "AM", AMPM, 0 },		/* time qualifiers */
  { "A.M.", AMPM, 0 },
  { "PM", AMPM, 12 },
  { "P.M.", AMPM, 12 },
  { "CHRISTMAS", CHRISTMAS, 1225 },	/* special dates */
  { "NEW", NEW, 101 },

	/* time zones */
  { "UT", ZONE, 0 },
  { "GMT", ZONE, 0 },
  { "UTC", ZONE, 0 },
  { "GST", ZONE, -180},
  { "GDT", ZONE, -120},
  { "NST", ZONE, -210},
  { "AST", ZONE, -240},
  { "ADT", ZONE, -180},
  { "EST", ZONE, -300 },
  { "EDT", ZONE, -240 },
  { "CST", ZONE, -360 },
  { "CDT", ZONE, -300 },
  { "MST", ZONE, -420 },
  { "MDT", ZONE, -360 },
  { "PST", ZONE, -480 },
  { "PDT", ZONE, -420 },
  { "HST", ZONE, -600 },
  { "HDT", ZONE, -540 },
  { "AHST", ZONE, -600 },
  { "AHDT", ZONE, -540 },
  { "AEST", ZONE, 600 },
  { "AESST", ZONE, 660 },
  { "ACST", ZONE, 570 },
  { "ACSST", ZONE, 630 },
  { "AWST", ZONE, 480 },
  { "AWSST", ZONE, 540 },
  { "EET", ZONE, 120 },
  { "MET", ZONE, 60 },
  { "WET", ZONE, 0 },
  { "BST", ZONE, 60 },
  { "MEZ", ZONE, 60 },
  { "MESZ", ZONE, 120 },
  { "CET", ZONE, 60 },
  { "CEST", ZONE, 120 },
	/* military time */
  { "Z", ZONE, 0 },
  { "A", ZONE, -60 },
  { "B", ZONE, -120 },
  { "C", ZONE, -180 },
  { "D", ZONE, -240 },
  { "E", ZONE, -300 },
  { "F", ZONE, -360 },
  { "G", ZONE, -420 },
  { "H", ZONE, -480 },
  { "I", ZONE, -540 },
/* No 'J' -- it's not an accident */
  { "K", ZONE, -600 },
  { "L", ZONE, -660 },
  { "M", ZONE, -720 },
  { "N", ZONE, 60 },
  { "O", ZONE, 120 },
  { "P", ZONE, 180 },
  { "Q", ZONE, 240 },
  { "R", ZONE, 300 },
  { "S", ZONE, 360 },
  { "T", ZONE, 420 },
  { "U", ZONE, 480 },
  { "V", ZONE, 540 },
  { "W", ZONE, 600 },
  { "X", ZONE, 660 },
  { "Y", ZONE, 720 },

  { 0, 0, 0 }
};

%}
%%

/* Grammar for legal dates. The date is returned in the dtm structure indexed
 * by result. The time is returned in ptm (and, possibly, shour).
 */

date_time :	sdate_time
			{ result = $$;
			  check ($$); }
	;

sdate_time :	stime
			{ $$ = new_dtm (CURRDATE); }
	|	full_date time
	|	full_date time year
		/* 
		 * This is a hack. To handle the strange format
		 * day month day-in-month time year
		 * It parses the date as though it were a full
		 * specification and then substitutes the year.
		 */
			{ setrep (&(dtm[$$]), RTM);
			  dtm[$$].tm.tm_year = ptm.tm_year;
			  dtm[$$].tm.tm_wday = -1;
			  dtm[$$].tm.tm_yday = -1; } 
	|	time ON full_date
			{ $$ = $3; }
	|	time full_date
			{ $$ = $2; }
	|	full_date
	|	time
			{ $$ = new_dtm (CURRDATE); }
	|	stime today
			{ $$ = new_dtm (CURRDATE);
			  incr ($$, $2); }
	|	NOW
			{ $$ = new_dtm (CURRDATE);
			  ptm.tm_hour = currtm->tm_hour;
			  ptm.tm_min = currtm->tm_min;
			  ptm.tm_sec = currtm->tm_sec; }
	;

full_date :	rec_date
	|	timekey _of rec_date
			{ $$ = $3; }
	|	THE timekey _of rec_date
			{ $$ = $4; }
	|	rec_date timekey
	;

rec_date :	date
			{ check ($$); }
	;

date	:	partial_date
			{ $$ = new_dtm (CURRDATE);
			  constrain (PTM, $$, ppf, 1); }
	|	THIS relative_partial_date
			{ $$ = new_dtm (CURRDATE);
			  constrain (PTM, $$, FUTURE, 1); }
	|	NEXT relative_partial_date
			{ $$ = new_dtm (CURRDATE);
			  constrain (PTM, $$, FUTURE, 2); }
	|	LAST relative_partial_date
			{ $$ = new_dtm (CURRDATE);
			  incr ($$, -1);
			  constrain (PTM, $$, PAST, 1); }
	|	weekday WEEK
			{ $$ = new_dtm (CURRDATE);
			  constrain (PTM, $$, FUTURE, 1);
			  incr ($$, 7); }
	|	weekday FORTNIGHT
			{ $$ = new_dtm (CURRDATE);
			  constrain (PTM, $$, FUTURE, 1);
			  incr ($$, 14); }
	|	adjusted_date
	|	today
			{ $$ = new_dtm (CURRDATE);
			  incr ($$, $1); }
	|	TODAY WEEK
			{ $$ = new_dtm (CURRDATE);
			  incr ($$, $1 + 7); }
	|	TODAY FORTNIGHT
			{ $$ = new_dtm (CURRDATE);
			  incr ($$, $1 + 14); }
	|	stddate
			{ $$ = new_dtm (PTM); }
	|	weekday stddate
			{ $$ = new_dtm (PTM); }
	;

stddate	:	yearday year
	|	yearday '-' year
	|	yearday '/' year
	;

/* The following rule must precede other uses of relative_yearday and
 * nonrelative_yearday so that the preferred reduction is to yearday.
 * This, in turn, causes a 4-digit number to be preferred as a year
 * rather than military time. */

yearday :	relative_yearday
	|	nonrelative_yearday
	;

relative_partial_before : relative_partial_dtm
	|	LAST relative_partial_date
			{ $$ = new_dtm (PTM); }
	|	nth LAST weekday
			{ $$ = new_dtm (PTM);
			  dtm[$$].count = $1; }
	;

relative_partial_dtm :	relative_partial_date
			{ $$ = new_dtm (PTM); }
	|	nth weekday
			{ $$ = new_dtm (PTM);
			  dtm[$$].count = $1; }
	;

/* 
 * partial_date: Returns ptm.
 */

partial_date : relative_partial_date
	|	nonrelative_yearday
	;

relative_partial_date :	relative_yearday
	|	weekday
	|	nth
			{ ptm.tm_mday = $1; }
	|	weekday nth
			{ ptm.tm_mday = $2; }
	|	weekday relative_yearday
	;

weekday	:	WEEKDAY
			{ ptm.tm_wday = $1; }
	;

holiday :	sholiday
			{ ptm.tm_mon = $1/100-1; ptm.tm_mday = $1%100; }
	;

sholiday :	CHRISTMAS
	|	CHRISTMAS DAY
	|	NEW YEAR
	;

/* 
 * forward_rec_date: Returns index to dtm.
 */

forward_rec_date : AFTER rec_date
			{ $$ = $2; }
	|	FROM rec_date
			{ $$ = $2; }
	;

/* 
 * adjusted_date: Returns index to dtm.
 */

adjusted_date :	days forward_rec_date
			{ $$ = $2;
			  incr ($$, $1); }
	|	days BEFORE rec_date
			{ $$ = $3;
			  incr ($$, -$1); }
	|	days AGO
			{ $$ = new_dtm (CURRTM);
			  incr ($$, -$1); }
	|	months forward_rec_date
			{ $$ = $2;
			  incrmonth ($$, $1); }
	|	months BEFORE rec_date
			{ $$ = $3;
			  incrmonth ($$, -$1); }
	|	months AGO
			{ $$ = new_dtm (CURRDATE);
			  incrmonth ($$, -$1); }
	|	years forward_rec_date
			{ $$ = $2;
			  incryear ($$, $1); }
	|	years BEFORE rec_date
			{ $$ = $3;
			  incryear ($$, -$1); }
	|	years AGO
			{ $$ = new_dtm (CURRDATE);
			  incryear ($$, -$1); }
	|	rel_date
	|	THE rel_date
			{ $$ = $2; }
	;

/* 
 * rel_date: Parse date relative to another date. Returns index to
 * dtm structure containing the resolved date.
 */

rel_date :	relative_partial_dtm AFTER rec_date
			{ $$ = $3;
			  incr ($$, 1);
			  constrain ($1, $$, FUTURE, dtm[$1].count); }
	|	relative_partial_before BEFORE rec_date
			{ $$ = $3;
			  incr ($$, -1);
			  constrain ($1, $$, PAST, dtm[$1].count); }
	;

/* 
 * today: Parse an indication of the current day.
 * Side effect: sets tmkey if time of day indication is also made.
 */

today	:	TODAY
	|	TONIGHT
			{ tmkey = 17; }
	|	THIS timekey
	|	TODAY timekey
			{ if ($1 == 0) yyerror (unkstring); }
	;

/* 
 * months: Parse a number of months. Returns integer the number of months.
 */

months	:	number WORD_MONTH
			{ $$ = $1; }
	|	A WORD_MONTH
			{ $$ = 1; }
	;

/* 
 * years: Parse a number of years. Returns integer the number of years.
 */

years	:	number YEAR
			{ $$ = $1; }
	|	A YEAR
			{ $$ = 1; }
	;

/* 
 * days: Parse a number of days. Returns integer number of days.
 */

days	:	number DAY
	|	nth_day
	|	the_nth_day
	|	THE DAY
			{ $$ = 1; }
	|	A DAY
			{ $$ = 1; }
	|	DAY
			{ $$ = 1; }
	|	number WEEK
			{ $$ = $1 * 7; }
	|	A WEEK
			{ $$ = 7; }
	|	WEEK
			{ $$ = 7; }
	|	A FORTNIGHT
			{ $$ = 14; }
	|	FORTNIGHT
			{ $$ = 14; }
	;

/* 
 * nonrelative_yearday: Parse day of the year.
 * Returns parsed specification in ptm.
 */

nonrelative_yearday : month_name THE nth
			{ ptm.tm_mday = $3; }
	|	stdmonthday _of month_name
	|	THE stdmonthday _of month_name
	|	the_nth_day OF month_name
			{ ptm.tm_mday = $1; }
	|	nth_day OF month_name
			{ ptm.tm_mday = $1; }
	;

/* 
 * relative_yearday: Parse day within year. Return ptm.
 */

relative_yearday : month '/' monthday
	|	month_name monthday
	|	month_name '-' monthday
	|	stdmonthday '-' month_name
	|	stdmonthday '-' month
	|	holiday
	;

/* 
 * time: Returns ptm containing the parsed time spec.
 */

time	:	ttime
	|	AT ttime
	|	AT stime
	;

stime	:	simple_time
	|	simple_time IN THE timekey
	;

simple_time :	number
			{ if ($1 < 1 || $1 > 12) yyerror (unkstring);
			  shour = $1; }
	;

timekey :	TIMEKEY
			{ tmkey = $1; }
	;

/* 12am is midnight. i.e. 0:00.  12pm is noon. */
ttime	:	hm_time
	|	hm_time IN THE timekey
	|	hour ':' min ':' sec opt_hundredths zone
	|	hour ':' min ':' sec opt_hundredths
	|	whour AMPM
			{ tmkey = $2; }
	|	hm_time AMPM
			{ tmkey = $2; }
	|	NOON
			{ ptm.tm_hour = $1; }
	|	whour NOON
			{ if ($1 != 12) yyerror (unkstring);
			  shour = -1;
			  ptm.tm_hour = $2; }
	|	hm_time NOON
			{ if (shour != 12 || ptm.tm_min != 0) yyerror (unkstring);
			  shour = -1;
			  ptm.tm_hour = $2; }
	|	number4 
			{ ptm.tm_hour = $1 / 100;
			  ptm.tm_min = $1 % 100;
			  if (ptm.tm_min > 59) yyerror (unkstring);
			  if ($1 > 2400) yyerror (unkstring); }
	;

number4 :
		NUMBER4
	|	NUMBER4YEAR
	;

opt_hundredths :	'.' number
	|
	;

hm_time	:	hour ':' min zone
	|	hour ':' min
	|	hour '.' min zone
	|	hour '.' min
	;

zone	:	ZONE
			{ zone = $1; }
	|	'-' ZONE
			{ zone = $2; }
	|	'+' NUMBER4
			{ zone = ($2/100 * 60) + ($2 % 100); }
	|	'-' NUMBER4
			{ zone = -(($2/100 * 60) + ($2 % 100)); }
	;			  

month_name :	MONTH
			{ ptm.tm_mon = $1; }
	;

/* 
 * year: Parse year specification.
 * Returns ptm.tm_year set to the year specified.
 */

year	:	NUMBER2
			{ ptm.tm_year = $1; }
	|	NUMBER4YEAR
			{ ptm.tm_year = $1 - 1900; }
	|	THIS YEAR
			{ ptm.tm_year = currtm->tm_year; }
	|	LAST YEAR
			{ ptm.tm_year = currtm->tm_year - 1; }
	|	NEXT YEAR
			{ ptm.tm_year = currtm->tm_year + 1; }
	;

/* 
 * month: Parse numeric month. Store (0 origin) in ptm.tm_mon.
 */

month	:	number
			{ ptm.tm_mon = $1 - 1; }
	;

monthday :	NWORD
			{ ptm.tm_mday = $1; }
	|	stdmonthday
	;

stdmonthday :	number
			{ ptm.tm_mday = $1; }
	|	nth
			{ ptm.tm_mday = $1; }
	;

hour	:	HOUR
			{ ptm.tm_hour = $1; }
	|	SHOUR
			{ shour = $1; }
	;

whour	:	hour
	|	NWORD
			{ shour = $1; }
	;

min	:	number
			{ ptm.tm_min = $1; }
	|	HOUR
			{ ptm.tm_min = $1; }
	|	SHOUR
			{ ptm.tm_min = $1; }
	;

sec	:	number
			{ ptm.tm_sec = $1; }
	;

number	:	NWORD
	|	NUMBER
	|	NUMBER2
	|	NUMBER4
	|	NUMBER4YEAR
	;

/* 
 * nth: Parse 1st 2nd etc or first second etc.
 * Return integer value of the number.
 */

nth	:	NTHWORD
	|	number ST
			{ if ($1 % 10 != 1 || $1 % 100 == 11) yyerror (unkstring);
			  $$ = $1; }
	|	number ND
			{ if ($1 % 10 != 2 || $1 % 100 == 12) yyerror (unkstring);
			  $$ = $1; }
	|	number RD
			{ if ($1 % 10 != 3 || $1 % 100 == 13) yyerror (unkstring);
			  $$ = $1; }
	|	number TH
			{ if (($1 + 9) % 10 <= 2 && ($1 % 100) / 10 != 1)
			    yyerror (unkstring);
			  $$ = $1; }
	;

_of	:	OF
	|
	;

the_nth_day :	THE nth_day
			{ $$ = $2; }
	;

nth_day	:	nth DAY
	;
%%
static char *prsdate_rcsid = "$Header: /afs/andrew.cmu.edu/itc/sm/releases/X.V11R5/ftp/src/ams/libs/ms/RCS/prsdate.y,v 2.6 1991/09/13 16:46:27 bobg Exp $";


/*
 *  pdate - parse date specification into time structure
 *
 *  str = date specification string
 *  tmp = pointer to time structure to receive parsed fields
 *
 *      Parses the supplied date string and sets the appropriate
 *  fields into the time structure.  Fields which are not specified
 *  in the string are returned as -1 or filled according to select (below).
 *  The date string is copied into a temporary buffer before
 *  being parsed (maximum of 80 characters).
 *
 *     Select may be used to prefer dates in the past or future.
 *  true indicates past, false indicates future. Unspecified
 *  fields are filled (if possible) to give a reasonable date in the past
 *  or future (or the current date).
 *
 *  e.g.	Thursday would mean "last Thursday" if select was true,
 *		"this Thursday" if select was false.
 *
 *  Note that "today" is the date passed in the date structure.
 *
 *     Returns 0 on a successful parse, -1 on error.
 */

/* pdate: a cover function for parsedate which is not completely
 *   compatible with the original pdate().
 */

/* pdate (str, tmp)
char *str;
struct tm *tmp;
{ return (parsedate (str, tmp, 1, 0, 0));
}
*/

#define MAXSTRLEN 150

parsedate (str, tmp, settm, select, err, gmt)
    char *str;
    struct tm *tmp;
    int settm, select, err;
    long *gmt;
{
    long time (), curtim;
    char tstr[MAXSTRLEN+1];
    int i;
    register struct tm *rtm;

    junk_err = err;
    if (settm)
    { curtim = time (0);
	/* Current RT compiler didn't like this...   juo */
/*      *currtm = *localtime (&curtim); */    
        bcopy (localtime (&curtim), currtm, sizeof (struct tm) );
    }
    else
    { *currtm = *tmp;
    }

    /*  initialize lexical analyzer  */
    for (i = 0; i < NDTMS; i++)
      dtmused[i] = 0;
    strncpy(strp = tstr, str, MAXSTRLEN);
    tstr[MAXSTRLEN] = 0;
    ptm.tm_year = NOYEAR;
    ptm.tm_mon = -1;
    ptm.tm_mday = -1;
    ptm.tm_wday = -1;
    ptm.tm_yday = -1;
    ptm.tm_hour = -1;
    ptm.tm_min = -1;
    ptm.tm_sec = -1;
    pcount = 0;
    currtm->tm_yday = currtm->tm_wday = -1;
    shour = -1;
    tmkey = -1;
    zone = 0;
    delim = 0;
    ppf = select;

    if (setjmp(errbuf) == 0)
    {
	yyparse();
	rtm = &(dtm[result].tm);
	setrep (&dtm[result], RTM);
	rtm->tm_hour = ptm.tm_hour;
	rtm->tm_min = ptm.tm_min;
	rtm->tm_sec = ptm.tm_sec;
	time_def (rtm, currtm);
	/* If time is 24:00 or later, advance date to next day. */
	if (rtm->tm_hour >= 24)
	{ incr (result, rtm->tm_hour / 24);
	  setrep (&dtm[result], RTM);
	  rtm->tm_hour = rtm->tm_hour % 24;
	}
	if (dtm[result].repn == RTM && dtm[result].tm.tm_yday == -1)
	  setrep (&dtm[result], RDAYS);

#ifdef NOTDEF
	/* Fix up for time zone */
	timezone(gmt);
#endif NOTDEF

	setrep (&dtm[result], RTM);
	*tmp = dtm[result].tm;
	return(0);			/* return here on successful parse */
    }
    else
	return(CERROR);			/* return here on error */

}

/*
 *  yyerror - error routine (called by yyparse)
 *
 *     Performs a jump to the error return location established
 *  by pdate().
 */

static yyerror(errstring)
char *errstring;
{

    longjmp(errbuf, 1);

}

static int prelval;
static struct { int token; int val; } peep[MAXPEEP];
static int peepb = 0, peepe = 0;
static int lead0;

/*  yylex - return next token in date string.
 * 
 *  Obtains the tokens from nextlex() and returns them. Checks for
 *  NUMBER tokens which are really HOURs. This is done by peeping ahead.
 *  It gives more lookahead than yacc can support.
 */

static int yylex ()
{ register int ctoken, htoken;
  ctoken = nextlex();
  htoken = lead0 ? HOUR : SHOUR;
  if (((ctoken == NUMBER) || (ctoken == NUMBER2)) && yylval <= 24)
  { /* Possible hour - check it out */
    peeper (1); 			     /* Allow 1 token peeping */
    if (peep[0].token == AMPM || peep[0].token == NOON)
      return (htoken); 			     /* NN AM or NN PM or 12 NOON */
    if (peep[0].token == ':' || peep[0].token == '.')
    { peeper (2);
      if ((peep[1].token == NUMBER) || (peep[1].token == NUMBER2))
	return (htoken);		     /* NN:NN or NN.NN */
    }
  }
  return (ctoken);
}

static int nextlex ()
{ register int token;
  if (peepb < peepe)
  { token = peep[peepb].token;
    yylval = peep[peepb].val;
    peepb++;
  }
  else
  { token = prelex ();
    yylval = prelval;
  }
  return (token);
}

static peeper (n)
int n;
{ register int i;
  if (peepb != 0)
  { for (i = peepb; i < peepe; i++)
      peep[i-peepb] = peep[i];
    peepe -= peepb;
  }
  peepb = 0;
  while (peepe < n)
  { peep[peepe].token = prelex ();
    peep[peepe].val = prelval;
    peepe++;
  }
}

/*
 *  prelex - return next token in date string
 *
 *     Uses nxtarg() to parse the next field of the date string.
 *  If a non-space, tab, comma or newline delimiter terminated the
 *  previous field it is returned before the next field is parsed.
 *
 *     Returns either one of the delimiter characters " -:/.", the token
 *  from a match on the table (with the associated value in yylval), or
 *  NUMBER, or JUNK.
 *  JUNK is any unrecognized token (depending on the call arguments to
 *  parsedate - an unrecognized token may instead be returned as -1 simply
 *  terminating the parse.)
 *  NUMBER is a numeric string.  NUMBER4 is a 4-digit number.
 *  NUMBER2 is a 2-digit number.  NUMBER4YEAR is a 4-digit number >= 1970.
 *  If the numeric string commences with 0, then lead0 is true.
 *  '@' sign is treated as the token AT.
 */

static int wasabb;			/* tabfind indicates abbrev. */

static int prelex()
{

  register int ret;			/* temp for previous delimiter */
  register char *fp;			/* current field */
  register int find, ndig;
  extern char _argbreak;		/* current delimiter */

  while (1)
  { if (ret=delim)
    {
	delim = 0;
        if (ret == '@')
	    return (AT);
        /* 
         * Ignore all but the good characters
         */
	if (ret != ':' && ret != '/' && ret != '-' && ret != '.' && ret != '+')
	    ret = 0;
        if (ret != 0)
	  return (ret);
    }
    if (*strp == 0) return (0);
    while (*strp == ' ' || *strp == '\t' || *strp == '\n')
      strp++;

    if (*strp >= '0' && *strp <= '9')
    { prelval = 0;
      ndig = 0;
      lead0 = *strp == '0';
      while (*strp >= '0' && *strp <= '9')
      { prelval = prelval * 10 + *strp - '0';
	strp++;
	ndig++;
      }
      if (ndig == 4) {
	if (prelval < 1970)
	  return (NUMBER4);
        return (NUMBER4YEAR);
      }
      if (ndig == 2)
	return (NUMBER2);
      return (NUMBER);
    }
    fp = nxtarg (&strp, " \t,-+:/.@()[]\n");
    delim = _argbreak;
    if (*fp == 0 && delim == 0) return (0);  /* End of input string */
    if (*fp != 0) 			     /* skip null tokens */
    { (void) foldup(fp, fp);
      /* Because of the embedded period, a.m. and p.m. do not work.
       * Solution is to recognize them explicitly.
       */
      if (fp[1] == '\0' && (delim == '.' || delim == ' ') &&
        (*fp == 'A' || *fp == 'P') && (*strp == 'M' || *strp == 'm') && 
        (strp[1] == '.' || strp[1] == ' ' || strp[1] == '\t' ||
          strp[1] == '\0'))
      { /* It is a.m. or p.m. */
        prelval = *fp == 'A' ? 0 : 12;
        strp += 2;    /* Skip the m. */
        delim = 0;
        return (AMPM);
      }
      if ((find = tabfind(fp, strings)) >= 0)
      {
	if (wasabb && delim == '.')	/* If tabfind found abbrev */
	  delim = 0;			/* Discard period after abbrev. */
        prelval = strings[find].lexval;
        return(strings[find].token);
      }
      if (junk_err)
        return (JUNK);
      else
	return (0);
    }
  }
}

/* subroutines useful for manipulating dates and used by
 * prsdate.y.
 * 
 * Copyright (c) Leonard G C Hamey, December 1983.
 * 
 * date_days (tm): converts the date portion of tm structure to days since
 *   1 jan 1900 + 693960. Also can be used to obtain weekday (sunday == 0)
 *   by taking modulo 7.
 *
 * days_date: converts days since 1/1/1900 + 693960 to date in tm structre.
 * 
 * tabfind: searches a table of keywords for date parsing.
 * 
 * constrain: fills in default fields under control of past/future
 * 	parameter.
 */

static int date_days (tm)
struct tm *tm;
{   /* Number of days since 1/1/1900 + 693960 */
    int dd = tm->tm_mday, mm = tm->tm_mon + 1, fullyear = tm->tm_year + 1900;
    int f;

    if (mm >= 3)
    {   f=365*fullyear+dd+31*mm-31-(4*mm+23)/10+fullyear/4-(75+(fullyear/100)*75)/100;
    }
    else
    {   f=365*fullyear+dd+31*mm-31+(fullyear-1)/4-(75+((fullyear-1)/100)*75)/100;
    }

    return (f-1);
}

static int monthend[] =
{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

static days_date (d, tm)
int d;
struct tm *tm;
{   /* Number of days since 1/1/1900 -> mm/dd/fullyear and weekday */
    int t = d - 693960, leap;
    int mm, fullyear;
    tm->tm_year = 0; 			     /* 1900 */
    tm->tm_mon = 0; 			     /* Jan */
    tm->tm_mday = 1;
    while (t < 0)			     /* handle dates before 1900 */
    {   tm->tm_year += t / 366 - 1;
        t = d - date_days (tm);
    }
    while (t >= 366)
    {   tm->tm_year += t / 366;
        t = d - date_days (tm);
    }
    fullyear = tm->tm_year + 1900;
    leap = yearsize (fullyear) == 366;
    if (! leap && t == 365)
    { tm->tm_year++;
      t = 0;
    }
    tm->tm_yday = t;			     /* Day in year (0-365) */
    if (! leap && t >= 59) 		     /* If after Feb non leap */
        t++; 				     /* Fudge as if leap */
    mm = t / 31; 			     /* Guess the month */
    if (t >= monthend[mm+1]) 		     /* Check the guess */
        mm++;
    tm->tm_mday = t - monthend[mm] + 1;	     /* Compute day in month */
    tm->tm_mon = mm;
    tm->tm_wday = d % 7; 		     /* Sunday = 0 */
}

static int tabfind (text, table)
char *text;
struct stable *table;
{ int tp;
  char *tep, *txp;
  int find = -1, foundstar;
  wasabb = 0;
  for (tp = 0; table[tp].text; tp++)
  { tep = table[tp].text;
    txp = text;
    foundstar = 0;
    while (1)
    { if (*tep == '*')
      { foundstar = 1;
	tep++;
      }
      if (! *txp) 			     /* If end of text */
      { if (! *tep) 			     /* If also end of table entry */
	  return (tp);			     /* then found */
	if (foundstar)
	{ if (find >= 0)
	    return (-2); 		     /* Ambiguous */
	  find = tp; 			     /* Remember partial match */
	  wasabb = 1;			     /* Was abbrev. */
	  break;
	}
      }
      if (*txp != *tep)
	break; 				     /* No match */
      tep++;  txp++;
    }
  }
  return (find);
}

/* check: check that a date is valid. each of the constraint processing
 *   routines is called in turn and if any of them do anything then the
 *   date is invalid.
 */

static check (date)
int date;
{ register int did;
  register struct dtm *d = &dtm[date];
  if (d->repn != RTM)
    return;
  did = month (d, d->tm.tm_mon, FUTURE);
  if (! did)
    did = mday (d, d->tm.tm_mday, FUTURE);
#ifdef NOTDEF
  if (! did && d->tm.tm_wday >= 0)
    did = weekday (d, d->tm.tm_wday, FUTURE);
  if (did)
    yyerror (unkstring);
#endif NOTDEF
  return;
}

/* constrain: fill in defaults info in date. con is a dtm containing the 
 *   constraints (or -1 indicating to use ptm). date is the dtm containing
 *   the date to be constrained. repeat is the loop count. */

static constrain (con, date, past, repeat)
int con, date;
int past, repeat;
{ register int n;
  register int did;
  register struct tm *c;
  register struct dtm *d = &dtm[date];
  if (con >= 0)
    c = &(dtm[con].tm);
  else
    c = &ptm;
  if (c->tm_year != NOYEAR)
    yyerror (unkstring);

  for (n = 1000, did = 0; ; did = 0)
  { if (c->tm_mon >= 0)
    { did |= month (d, c->tm_mon, past);
    }
    if (c->tm_mday >= 0)
    { did |= mday (d, c->tm_mday, past);
    }
    if (c->tm_wday >= 0)
    { did |= weekday (d, c->tm_wday, past);
    }
    if (! did)
    { if (repeat-- <= 1)
        break;
      incr (date, past ? -1 : 1);
    }
    if (--n <= 0)
      yyerror (unkstring);
  }

  if (con >= 0)
    dtmused[con] = 0;
  else
  { ptm.tm_year = NOYEAR;
    ptm.tm_mon = -1;
    ptm.tm_mday = -1;
    ptm.tm_wday = -1;
    ptm.tm_yday = -1;
  }
}

static time_def (tm, currtm)
struct tm *tm, *currtm;
{ if (shour >= 0)			/* Handle simple hour specification */
  { if (tmkey == -1)			/* and combine it with time key. */
      tmkey = 8;			/* Default is 8:00 - 19:59 */
    if (shour == 12)
      shour = 0;
    if (shour < tmkey)
      shour += 12;
    if (shour < tmkey)
      shour += 12;
    tm->tm_hour = shour;
  }
  if (tm->tm_hour >= 0)
  { /* If time specified and fields left out, assume zero. */
    if (tm->tm_min < 0)
      tm->tm_min = 0;
    if (tm->tm_sec < 0)
      tm->tm_sec = 0;
  }
}

/* date constraint processing routines.
 * 
 * These routines allow determination of the first date after/before
 * (but possibly equal to the existing date) which satisfies the given
 * constraint(s).
 */

/* The constraints are implemented by calling the appropriate routine(s)
 * which check whether the particular constraint is satisfied, and if it
 * is not, advances the date until the constraint is satisfied.
 * 
 * The date is stored in the dtm structure.
 */

/* weekday: constrains the day of the week. */

static int weekday (dtm, wkday, past)
struct dtm *dtm;
int wkday; 				     /* 0 = Sunday */
int past; 				     /* true = past */
{ int n;
  setrep (dtm, RDAYS);
  n = wkday - dtm->days % 7; 	     /* adjustment */
  if (past)
  { if (n > 0)
      n -= 7;
  }
  else
    if (n < 0)
      n += 7;
  dtm->days += n;
  return (n != 0);
}

/* month: constrains the month to the specified value. */

static int month (dtm, mon, past)
struct dtm *dtm;
int mon;
int past;
{ setrep (dtm, RTM);
  if (mon < 0 || mon > 11)
    yyerror (unkstring);
  if (dtm->tm.tm_mon != mon)
  { if (past)
    { if (dtm->tm.tm_mon < mon) 	     /* If earlier month */
	dtm->tm.tm_year--; 		     /* Back up a year */
      dtm->tm.tm_mday = monthend[mon+1] - monthend[mon];
      if (mon == 1 && yearsize (dtm->tm.tm_year+1900) == 365)  /* Feb */
	dtm->tm.tm_mday--;
    }
    else
    { if (dtm->tm.tm_mon > mon) 	     /* If later month */
	dtm->tm.tm_year++; 		     /* Advance a year */
      dtm->tm.tm_mday = 1;
    }
    dtm->tm.tm_mon = mon;
    dtm->tm.tm_wday = dtm->tm.tm_yday = -1;
    return (1);
  }
  return (0);
}

/* mday: constrains the month day to the specified value. Also
 *   checks the validity of the specified value and, if it is invalid,
 *   adjusts the month to compensate. */

static int mday (dtm, day, past)
struct dtm *dtm;
int day;
int past;
{ register int maxday;
  register int status = 0;
  setrep (dtm, RTM);
  if (dtm->tm.tm_mday != day)
  { if (past)
    { if (dtm->tm.tm_mday < day)  	     /* Earlier day */
        if (dtm->tm.tm_mon-- == 0)	     /* Back up a month */
	{ dtm->tm.tm_mon = 11;
	  dtm->tm.tm_year--;
	}
    }
    else
    { if (dtm->tm.tm_mday > day)  	     /* Later day */
	if (dtm->tm.tm_mon++ == 11)  	     /* Advance month */
	{ dtm->tm.tm_mon = 0;
	  dtm->tm.tm_year++;
	}
    }
    dtm->tm.tm_mday = day;
    dtm->tm.tm_wday = dtm->tm.tm_yday = -1;
    status = 1;
  }
  if (day >= 28)
  { maxday = monthend[dtm->tm.tm_mon+1] - monthend[dtm->tm.tm_mon];
    if (dtm->tm.tm_mon == 1 && yearsize (dtm->tm.tm_year + 1900) == 365)
      maxday--;
    if (day > maxday)
    { if (past)
        dtm->tm.tm_mday = maxday;
      else
      { dtm->tm.tm_mday = 1;
	if (dtm->tm.tm_mon++ == 11)
	{ dtm->tm.tm_mon = 0;
	  dtm->tm.tm_year++;
	}
      }
      dtm->tm.tm_wday = dtm->tm.tm_yday = -1;
      return (1);
    }
  }
  return (status);
}

/* setrep: sets the representation of the date in a dtm structure. */

static setrep (dtm, rep)
struct dtm *dtm;
int rep;
{ if (dtm->repn == rep)
    return;
  if (dtm->repn == RDAYS)
  { if (rep == RTM)
      days_date (dtm->days, &(dtm->tm));
  }
  else if (dtm->repn == RTM)
  { if (rep == RDAYS)
      dtm->days = date_days (&(dtm->tm));
  }
  dtm->repn = rep;
  return;
}

/* yearsize: returns nuber of days in year. */

yearsize (year)
int year;
{ return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) ? 366 : 365);
}

/* new_dtm: returns the index of a new dtm structure. If current is CURRTM,
 *   the structure will contain a copy of the current date-time, else it
 *   will contain a copy of ptm, and ptm will be reset to all -1.
 */

static int new_dtm (current)
int current;
{ register int i;
  for (i = 0; i < NDTMS; i++)
    if (! dtmused[i])
    { dtmused[i] = 1;
      if (current == CURRTM || current == CURRDATE)
      { dtm[i].tm = *currtm;
        if (current == CURRDATE)
	{ dtm[i].tm.tm_hour = -1;
	  dtm[i].tm.tm_min = -1;
	  dtm[i].tm.tm_sec = -1;
	}
	dtm[i].repn = RTM;
	dtm[i].count = 0;
      }
      else
      { dtm[i].tm = ptm;
	dtm[i].count = pcount;
	dtm[i].repn = RTM;
	ptm.tm_year = NOYEAR;
	ptm.tm_mon = -1;
	ptm.tm_mday = -1;
	ptm.tm_wday = -1;
	ptm.tm_yday = -1;
	pcount = 0;
      }
      return (i);
    }
  yyerror (unkstring);
  return(-1); /* Never reached */
}

/* incr: increment date in dtm structure. */

static incr (ndtm, days)
int ndtm;
int days;
{ setrep (&dtm[ndtm], RDAYS);
  dtm[ndtm].days += days;
}

static incryear (ndtm, years)
int ndtm;
int years;
{ setrep (&dtm[ndtm], RTM);
  dtm[ndtm].tm.tm_year += years;
  dtm[ndtm].tm.tm_wday = -1;		/* Unknown */
  dtm[ndtm].tm.tm_yday = -1;		/* Unknown */
}

/* incrmonth: increment date in dtm structure by a number of months.*/

static incrmonth (ndtm, months)
int ndtm;
int months;
{ int inc;
  inc = months > 0 ? 1 : -1;
  setrep (&dtm[ndtm], RTM);		/* Use tm structure repn */
  for ( ; months != 0; months -= inc)
  { dtm[ndtm].tm.tm_mon += inc;
    if (dtm[ndtm].tm.tm_mon < 0)
    { dtm[ndtm].tm.tm_mon = 11;
      dtm[ndtm].tm.tm_year--;
    }
    else if (dtm[ndtm].tm.tm_mon > 11)
    { dtm[ndtm].tm.tm_mon = 0;
      dtm[ndtm].tm.tm_year++;
    }
  }
  dtm[ndtm].tm.tm_wday = -1; 		/* Day of week is unknown */
  dtm[ndtm].tm.tm_yday = -1;		/* Day of year is unknown */
}

#ifdef NOTDEF

static long offset()
{
    struct timeval tp;
    struct timezone tzp;
    register struct tm *now;
    int gmtYear, gmtDay, gmtHour, gmtMin;
    static int virgin = 1;
    static long GMTDiff;	/* what to add to GMT (in minutes) to get
			local time (in minutes), or ``minutesEast'' */

    if (!virgin) return GMTDiff;
    virgin = 0;

    /*
       The zone returned by gettimeofday is static for the life of the kernel;
       it is localtime() that judges whether the local time is daylight or not.
       Get the GMT info and compare it to the localtime result to determine
       the current offset from GMT.
    */
    gettimeofday(&tp, &tzp);
    now = gmtime(&tp.tv_sec);
    gmtYear = now->tm_year;
    gmtDay = now->tm_yday;
    gmtHour = now->tm_hour;
    gmtMin = now->tm_min;
    now = localtime(&tp.tv_sec);
         if (gmtYear > now->tm_year) GMTDiff = - (60*24);
    else if (gmtYear < now->tm_year) GMTDiff = (60*24);
    else if (gmtDay > now->tm_yday) GMTDiff = - (60*24);
    else if (gmtDay < now->tm_yday) GMTDiff = (60*24);
    else GMTDiff = 0;
    GMTDiff += (now->tm_hour - gmtHour) * 60;
    GMTDiff += (now->tm_min - gmtMin);

    return GMTDiff;
}

static timezone(gmt)
    register long *gmt;
{
    long days;
    long minutes;
    struct tm *tm;

    if (gmt == 0) return;

    setrep(&dtm[result], RDAYS);

    /* Convert to 1/1/70 */
    days = dtm[result].days;
    days -= (693960 + 25567);

    minutes = days*24 + dtm[result].tm.tm_hour;
    minutes = minutes*60 + dtm[result].tm.tm_min;
    minutes += (zone - offset());
    *gmt = minutes*60 + dtm[result].tm.tm_sec;
    printf("GMT: %u, %s", *gmt, ctime(gmt));
    tm = gmtime(gmt);
    tm = localtime(gmt);
}

#endif NOTDEF
