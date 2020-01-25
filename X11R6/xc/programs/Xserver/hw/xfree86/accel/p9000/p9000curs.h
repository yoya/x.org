/* $XConsortium: p9000curs.h,v 1.2 94/11/21 22:38:44 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/accel/p9000/p9000curs.h,v 3.0 1994/05/29 02:05:39 dawes Exp $ */

extern Bool p9000BlockCursor;
extern Bool p9000ReloadCursor;

#define BLOCK_CURSOR    p9000BlockCursor = TRUE;

#define UNBLOCK_CURSOR  { \
			    if (p9000ReloadCursor) \
			       p9000RestoreCursor(p9000savepScreen); \
			    p9000BlockCursor = FALSE; \
                        }
