/* $XConsortium: s3Cursor.h,v 1.1 94/03/28 21:14:07 dpw Exp $ */

extern Bool s3BlockCursor;
extern Bool s3ReloadCursor;

#define BLOCK_CURSOR	s3BlockCursor = TRUE;

#define UNBLOCK_CURSOR	{ \
			   if (s3ReloadCursor) \
			      s3RestoreCursor(s3savepScreen); \
			   s3BlockCursor = FALSE; \
			}
