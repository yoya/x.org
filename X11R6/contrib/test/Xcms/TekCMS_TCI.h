/* $XConsortum: TekCMS_TCI.h,v 1.1 91/02/11 19:40:54 dave Exp $ */

#ifndef TEKCMS_TCI
#define TEKCMS_TCI

extern char	TekCMS_idir[BUFSIZ];
extern char	TekCMS_vdir[BUFSIZ];
extern char	TekCMS_rdir[BUFSIZ];




extern int
TCI();
extern int
TC_CompareResults();
extern int
Cmd_ListTC();
#endif
