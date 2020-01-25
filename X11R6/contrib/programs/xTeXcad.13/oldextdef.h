
/* 
     Copyright (c) 1991 xtexcad  V1.3 by K. Zitzmann

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
*/


/* 
 * Former header-file extdef.h of version 1.2, now oldextdef.h,
 * contributed for compatibility.
 */


#ifndef EXTERNALH
#define EXTERNALH

#ifndef DGUX
extern double atof(char *txt);
#endif
extern double sqrt(double x);
extern int abs(int x);
extern double fabs(double x);
extern double acos(double x);
extern double sin(double x);
extern char *malloc(int l);
extern char *getenv(char *s);           
extern int chdir();                     

/* to be continued... */


#endif