/*
 * $XConsortium: omronFb.h,v 1.3 94/01/11 20:47:40 rob Exp $
 *
 * Copyright 1992, 1993 Data General Corporation;
 * Copyright 1991, 1992, 1993 OMRON Corporation  
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting documentation, and
 * that neither the name OMRON or DATA GENERAL be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission of the party whose name is to be used.  Neither 
 * OMRON or DATA GENERAL make any representation about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.  
 *
 * OMRON AND DATA GENERAL EACH DISCLAIM ALL WARRANTIES WITH REGARD TO THIS
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS,
 * IN NO EVENT SHALL OMRON OR DATA GENERAL BE LIABLE FOR ANY SPECIAL, INDIRECT
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF
 * USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE
 * OF THIS SOFTWARE.
 *
 */

#ifdef	luna88k
# include <dev/fbmap.h>
#ifdef USE_VME8PLANES
#include "omronVme8p.h"
#endif
#else	/*  ~luna88k */
#ifdef uniosu
# include <sys/devmap.h>
#else /* ~uniosu */
# ifdef luna2
#  include <dev/fbmap.h>
# else /* ~luna2 */
#  include <om68kdev/fbmap.h>
# endif /* luna2 */
#endif /* uniosu */
#endif /* luna88k */

typedef struct _OmronFbProc {
    int		(*CreateProc)();
    int		(*InitProc)();
    void	(*GiveUpProc)();
} OmronFbProc;        

typedef struct _OmronFbInfo {
	int		fb_type;
	int		fb_width;
	int		fb_height;
	int		fb_depth;
	int		scr_width;
	int		scr_height;
	char 		*plane;
	int		planemapsize; /* luna-88k 24plaens only */ /* MTX */
	long		*refresh_reg;	
	int		palfd;	/* pallate */
	int		fbfd1;	/* luna-88k only (/dev/fb) */
	int		fbfd2;	/* luna-88k only (/dev/allmapfb) */
	int		palmapsize;
	char		*palmap;
	int		fbmapsize;
	char		*fbmap;
	OmronFbProc	*func;
} OmronFbInfo, *OmronFbInfoPtr;

#define	mmapfd		fbfd2	/* luna-88k 24plaens only */

/*	fb_type		*/
#define DT_BM 		0
#define DT_PLASMA 	1
#define DS_BM		2
#define FS_BM		3
#define	DT_BM8		4
#define DT_BM24         5
#define DS_VME8		6

#ifndef MACH_24_PLANE
#define MACH_24_PLANE   0
#endif

/* scr_width and scr_height */
/* DT_BM, DS_BM, FS_BM, DT_BM8 */
#define SCREEN_WIDTH				1280
#define SCREEN_HEIGHT				1024
/* DT_PLASMA */
#define PLASMA_SCREEN_WIDTH			1024
#define PLASMA_SCREEN_HEIGHT			768
/* fb_width and fb_height */
#define VME_WIDTH				16384
#define FB_WIDTH				2048
#define FB_HEIGHT				1024

#define COLOR_TV_RESOLUTION  			110 
#define MONO_TV_RESOLUTION			125   
#define PLASMA_TV_RESOLUTION			100


/*
**	color palette memory map
*/
union palette {		/* mono and 4 depth color */
    struct {
	unsigned char    addr;		/* palette addres register */
	unsigned char    coldata;	/* palette data register (R G B) */	
    } reg;
    char    pad[4096];
};

union palette_bm8 {	/* 8 depth color */
    struct {
	unsigned char	addr;		/* palette addres register */	
	unsigned char	pad11[3];	/* unuse */
	unsigned char	coldata;	/* palette data register (R G B) */
	unsigned char	pad12[3];	/* unuse */
	unsigned char	command;	/* palette control command register */
	unsigned char	pad13[3];	/* unuse */
	unsigned char	ovcdata;	/* palette overlay data (unuse) */
	unsigned char	pad14[3];	/* unuse */
    } reg;
    char    pad[4096];
};

union fs_palette {	/* 8 depth color (luna-fs)*/
    struct {
	long    addr;			/* palette addres register */
	long    control;		/* palette data register (R G B) */
	long    coldata;		/* palette control command register */
	long    ovcdata;		/* palette overlay data (unuse) */
    } reg;
    char    pad[0x1000];
};

union palette_bm24 {            /* 24 depth color  */
    struct {
        unsigned int    addr;
        unsigned int    command;
        unsigned int    coldata;
        unsigned int    mode;
    } reg;
    char   pad[4096];
};

/*
**	frame buffer memory memory map
*/
struct bm_one_data {	/* mono frame buffer */
    int         sd[1][1024][64];	/* 2048 x 1024 */
};

struct bm_four_data {	/* 4 depth color frame buffer */
    int		sd[4][1024][64];	/* 2048 x 1024 x 4 */
};

struct bm_eight_data {	/* 8 depth color frame buffer */
    int		sd[8][1024][64];	/* 2048 x 1024 x 8 */
};

/*
** luna raster operateon hardware memory map
*/
union bm_function_set {
    long	op_cont[16];	/* function set register */
    char	pad[0x40000];
};

/*
**	luna graphic display control hardware mememory map 
*/
typedef struct dt_bm_fbmap {	/* luna and luna-2 */
    union {	
	long	reg;
	char	pad2[0x40000];
    } refresh;		/* display area control register (write only) */
    union {
	long	reg;
	char	pad3[0x40000];
    } pselect;		/* plane select register (write only) */
    /* common frame buffer (write only) */
    struct bm_one_data		cbmplane;
    /* frame buffer (read/write) */
    struct bm_eight_data	bmplane;
    /* luna raster operateon hardware (common frame buffer)  */
    union bm_function_set cplane;
    /* luna raster operateon hardware */
    union bm_function_set planes[8];
} *DtBmMapPtr;

typedef struct fs_bm_fbmap {	/* luna-fs */
    /* common frame buffer (write only) */
    struct bm_one_data cbmplane;
    struct bm_one_data pad1[6];	/* unuse */
    /* luna raster operateon hardware (common frame buffer)  */
    union fs_fset {
	long    op_cont[16];
	char    pad2[0x8000];
    } cplane;
    union fs_fset pad3[6];	/* unuse */
    union {
	struct {
	    long	addr;	/* acrtc address register */
	    long	data;	/* acrtc data register */
	} reg;
	char	pad3[0x4000];
    } acrtc;
    union {
	long	reg;
	char	pad4[0x1000];
    } pselect;	      		/* plane select register (write only) */
    union {
	long	reg;
	char	pad5[0x2000];
    } refresh;			/* display area control register (write only) */
    /* color palette */
    union fs_palette palette;
    /* frame buffer (read/write) */
    struct bm_eight_data bmplane;
    char pad5[0x600000];	/* unuse */
    /* luna raster operateon hardware */
    union {
	long    op_cont[16];
	char    pad6[0x2000];
    } planes[8];
} *FsBmMapPtr;

typedef struct R88k_bm_fbmap {	/* luna-88k frame buffer */
    /* common frame buffer (write only) */
    struct bm_one_data		cbmplane;
    /* frame buffer (read/write) */
    struct bm_eight_data	bmplane;
    /* luna raster operateon hardware (common frame buffer)  */
    union bm_function_set 	cplane;
    /* luna raster operateon hardware */
    union bm_function_set	planes[8];
} *R88kBmMapPtr;

typedef struct R88k_bm_fbreg {	/* luna-88k control */
    union {
	long	reg;
	char	pad2[0x40000];
    } refresh;	/* display area control register (write only) */
    union {
	long	reg;
	char	pad3[0x40000];
    } pselect;	/* plane select register (write only) */
} *R88kBmRegPtr;

typedef struct R88k_bm24_fbreg {        /* luna-88k 24planes control */
    unsigned  int        addr;
    unsigned  int        cnt;
} *R88kBm24RegPtr;

struct ds_bm_map {
    union {
	long	op_cont[16];
	char	pad1[0x8000];
    } planes[7];
    union {
	struct {
	    long	addr;
	    long	data;
	} reg;
	char	pad2[0x4000];
    } acrtc;
    union {
	long	reg;
	char	pad3[0x1000];
    } pselect;
    union {
	long	reg;
	char	pad4[0x2000];
    } refresh;
    union {
	struct {
	    long	addr;
	    long	control;
	    long	coldata;
	    long	ovcdata;
	} reg;
	char	pad5[0x1000];
    } palette;
};

#ifndef luna88k
# ifdef uniosu
#  ifndef luna2
extern Bool omronFsBmCreate();
extern Bool omronFsBmInit();
extern void omronFsBmGiveUp();
#  endif
# endif /* uniosu */
extern Bool omronDtBmCreate();
extern Bool omronDtBmInit();
extern void omronDtBmGiveUp();
#else /* luna88k */
extern Bool omron88kBmCreate();
extern Bool omron88kBmInit();
extern void omron88kBmGiveUp();
#ifdef USE_24PLANES
extern Bool omron88kBm24Create();
extern Bool omron88kBm24Init();
extern void omron88kBm24GiveUp();
#endif
#ifdef USE_VME8PLANES
extern Bool omronVme8pBmCreate();
extern Bool omronVme8pBmInit();
extern void omronVme8pBmGiveUp();
#endif
#endif /* luna88k */
