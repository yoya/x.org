/* $XConsortium: strlist.h,v 1.2 94/11/21 21:55:13 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/reconfig/strlist.h,v 3.0 1994/09/03 02:52:19 dawes Exp $ */

/* Used in the %union, therefore to be included in the scanner. */
typedef struct {
	int count;
	char **datap;
} string_list ;

typedef struct {
	int count;
	string_list **datap;
} string_list_list;
