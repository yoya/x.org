/*
  Include declarations
*/
#include <stdio.h>
#if __STDC__ || defined(sgi) || defined(_AIX)
#include <stdlib.h>
#else
#ifndef vms
#include <malloc.h>
#include <memory.h>
#endif
#endif
#include <ctype.h>
#include <math.h>
#include <string.h>

/*
  Define declarations for the Display program.
*/
#if __STDC__ || defined(sgi) || defined(_AIX)
#define _Declare(formal_parameters) formal_parameters
#else
#define const 
#define _Declare(formal_parameters) ()
#endif
#define False  0
#define True  1
#define Warning(message,qualifier)  \
{  \
  (void) fprintf(stderr,"%s: %s",client_name,message);  \
  if (qualifier != (char *) NULL)  \
    (void) fprintf(stderr," (%s)",qualifier);  \
  (void) fprintf(stderr,".\n");  \
}

#ifndef lint
static char 
  Version[]="@(#)ImageMagick 2.1 92/10/10 cristy@dupont.com";
#endif
