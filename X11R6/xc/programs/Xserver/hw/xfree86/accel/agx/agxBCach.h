/* $XConsortium: agxBCach.h,v 1.2 94/11/21 22:06:17 kaleb Exp $ */
/* $XFree86: xc/programs/Xserver/hw/xfree86/accel/agx/agxBCach.h,v 3.1 1994/06/26 13:04:12 dawes Exp $ */
/*
 * Jon's cache malloc rubbish
 *
 * Rewritten for the AGX by Henry A. Worth (haw30@eng.amdahl.com)
 */

#undef DEBUG_FCACHE    /* Define it to see the bugs */


#ifdef DEBUG_FCACHE
#define SHOWCACHE() showcache()
#define ERROR_F(x)  ErrorF x
#else
#define SHOWCACHE() /**/
#define ERROR_F(x)   /**/
#endif

#define FONT_ROW_LENGTH    0x10000             /* Cache Row Length - 64K */
#define CACHE_ROW_ALIGN    0x8000              /* 32K Byte aligned - min */ 
#define BLOCK_NUM_CHAR     32                  /* must be power of 2 */
#define BLOCK_NUM_SHIFT    5
#define BLOCK_IDX_MASK     0x1F
#define BLOCKS_PER_FONT    8
#define FONT_BLOCK_MASK    0x07
#define MAX_NUM_ROWS       8

/*
 * Cache rows are organized as 256 bit wide lines. Allocation is also
 * by lines.
 */  

#define CACHE_LINE_WIDTH_PIXELS		256
#define CACHE_LINE_WIDTH_PIXELS_SHIFT	8
#define CACHE_LINE_WIDTH_BYTES		32
#define CACHE_LINE_WIDTH_BYTES_SHIFT	5
#define CACHE_NUM_LINES( rowlen ) \
           ( (rowlen) >> CACHE_LINE_WIDTH_BYTES_SHIFT )
#define ROW_NUM_LINES                   2048 \
           /* FONT_ROW_LENGTH >> CACHE_LINE_WIDTH_BYTES_SHIFT */

/*
 *  8 bit fonts are store off screen in up 8 blocks of 32 chars 
 */

  typedef struct _bitMapBlock{
     unsigned short line;	       /* num lines into cache row   */
     unsigned short sizel;	       /* size of the block (lines)  */
     struct _bitMapRow *daddy;         /* parent cache row rec */
     struct _bitMapBlock *next;	       /* any following blocks */
     unsigned long lru;		       /* lru */
     pointer *reference;	       /* who is referencing us */
  } bitMapBlockRec;

typedef struct _bitMapBlock *bitMapBlockPtr;

/*
 * cache is managed as a number of rows, starting with one per display
 * plane.
 */

  typedef struct _bitMapRow{
     unsigned int offset;		/* starting byte offset in vidmem */
     unsigned int freel;	      	/* num of lines available */
     struct _bitMapBlock *blocks;       /* start of linked list of blocks */
     struct _bitMapRow *next;	        /* next row */
     struct _bitMapRow *prev;	        /* previous row */
  } bitMapRowRec;

typedef struct _bitMapRow *bitMapRowPtr;

  typedef struct _CacheFont8 {
     FontPtr font;		        /* font */
     CharInfoPtr pci[256];	        /* font infos */
     unsigned short wBytes;             /* font glyph slot width (bytes) */
     unsigned short wPix;               /* font glyph slot width (pixels) */
     unsigned short hPix;		/* font glyph slot height (pixels) */
     unsigned short gper;		/* glphs per cache line */
     unsigned int   blockSize;          /* num lines for block of glyphs */
     struct _bitMapBlock *fblock[MAX_NUM_ROWS];   /* 8 * 32 cache block chars */
     struct _CacheFont8 *next;	       /* next */
  }
CacheFont8Rec;

typedef struct _CacheFont8 *CacheFont8Ptr;

extern bitMapBlockPtr agxCGetBlock();
extern void agxCReturnBlock();


