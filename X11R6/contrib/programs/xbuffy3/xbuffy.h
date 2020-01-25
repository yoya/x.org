/*******************************************************************************

     Copyright (c) 1994    William Pemberton (wfp5p@virginia.edu)
     
     The X Consortium, and any party obtaining a copy of these files from
     the X Consortium, directly or indirectly, is granted, free of charge, a
     full and unrestricted irrevocable, world-wide, paid up, royalty-free,
     nonexclusive right and license to deal in this software and
     documentation files (the "Software"), including without limitation the
     rights to use, copy, modify, merge, publish, distribute, sublicense,
     and/or sell copies of the Software, and to permit persons who receive
     copies from any such party to do so.  This license includes without
     limitation a license to do the foregoing actions under any patents of
     the party supplying this software to the X Consortium.

*******************************************************************************/


#ifndef _XBUFFY_H_
#define _XBUFFY_H_

#include <X11/Intrinsic.h>
#include "libdyn/dyn.h"
#include "patchlevel.h"

#define NEWSSERVER "murdoch.acc.Virginia.EDU"

#define MAX_STRING 256

#define NEW_MSG 1
#define OLD_MSG 2
#define READ_MSG 4
#define UNKNOWN 8
#define LINEFEED (char) 10

#define X_RESOURCE_CLASS "XBuffy" 

enum BoxType_e {
  MAILBOX = 0,
  NNTPBOX,
};

typedef enum BoxType_e BoxType_t;

enum BoxNameType_e {NONE = 0, SHORT, LONG, USR, UNDEF};
typedef enum BoxNameType_e BoxNameType_t;

struct articles_s
{
	long firstNum;
	long lastNum;
};

typedef struct articles_s Articles_t;	


struct BoxInfo_s{
  Widget w;
  int boxNum;			/* the box number */
  char *box;			/* the box filename or newsgroup */
  BoxType_t type;		/* the box type (mail, news, etc) */

  DynObject articles; /* for newsgroups, the read pairs  */

  time_t box_mtime;		/* last time read */
  off_t st_size;		/* size of file on last read */
  int n;				/* the number of messages in the box */
  
  char *command;
  char *audioCmd;
  char *boxTitle;
  int last;
  int pollTime;
  int headerTime;
  int nobeep;
  int origMode;
  BoxNameType_t BoxNameType;	
  
};
typedef struct BoxInfo_s BoxInfo_t;


extern char *header_cmp();
#endif /* _XBUFFY_H_ */
