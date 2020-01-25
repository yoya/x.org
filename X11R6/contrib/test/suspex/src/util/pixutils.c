/* $XConsortium: pixutils.c,v 1.0 93/11/22 12:25:00 rws Exp $ */

/*
        NOTICE TO USER: The source code in this file is copyrighted under
        U.S. and international laws.  SunSoft, Inc., a Sun Microsystems,
        Inc. business of Mountain View, California owns the copyright. 
        Users and possessors of this source code are hereby granted a
        nonexclusive, royalty-free copyright and design patent license
        to use this code in individual and commercial software. 

        Any use of this source code must include, in the user documentation 
        and internal comments to the code, notices to the end user as follows:

(c) Copyright 1989, 1990, 1991, 1992, 1993 SunSoft, Inc., a Sun Microsystems, Inc. business.

        SUNSOFT, INC. AND SUN MICROSYSTEMS, INC. MAKE NO REPRESENTATIONS
        ABOUT THE SUITABILITY OF THIS SOURCE CODE FOR ANY PURPOSE. IT IS
        PROVIDED "AS IS" WITHOUT EXPRESS OR IMPLIED WARRANTY OF ANY KIND. 
        SUNSOFT, INC., AND SUN MICROSYSTEMS, INC., SEVERALLY AND
        INDIVIDUALLY, DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOURCE
        CODE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
        FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL SUNSOFT, INC.
        OR SUN MICROSYSTEMS, INC., BE LIABLE FOR ANY SPECIAL, INDIRECT,
        INCIDENTAL, OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER
        RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
        OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
        OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOURCE CODE.
*/

/*
 * pixel utilities library
 */

/* 
 * header files
 */
#include "stdio.h"
#include "fcntl.h"
#include <sys/types.h>
#include <sys/file.h>
#include "X11/Xlib.h"
#include "X11/Xutil.h"
#include "logmsg.h"
#include "pixutils.h"
#include "varargs.h"

/* 
 * constants 
 */
#define PSEUDOCOLOR 1
#define TRUECOLOR 2
#define MASK24 0xffffff
#define BLUEMASK 0xff0000
#define GREENMASK 0xff00
#define REDMASK 0xff
#define MAXIMAGEDIM 400 
#define SIZEIMGENTRY 2
#define MAXIMAGEDATASIZE  (SIZEIMGENTRY * MAXIMAGEDIM * MAXIMAGEDIM + 4) 
		/* extra 4 bytes to store number of bytes of image data */
#define SIZECMAPENTRY 4
#define MAXCMAPDATASIZE  (MAXCOLOR * SIZECMAPENTRY + 2)
		/* extra 2 bytes to store number of bytes of cmap data */
#define SIZEIMGENTRY24 4 /* for 24 bit image */
#define MAXIMAGEDATASIZE24  (SIZEIMGENTRY24 * MAXIMAGEDIM * MAXIMAGEDIM + 4)
                /* extra 4 bytes to store number of bytes of image data */
#define MAXCOLOR 256
#define BUFSIZE 4096
#define CMDBUFSIZE 1024
#define MAXFILENAMELEN 256

#ifdef SYSV
#define R_OK 4 
#include "sys/time.h"
#define USLEEP(usec) { struct timeval timeout; \
        timeout.tv_sec = 0; \
        timeout.tv_usec = (usec); \
        select(0,NULL,NULL,NULL,&timeout); }
#else
#define USLEEP(usec) usleep((usec))
#endif

typedef struct {
    Unsign32 rmask;
    Unsign32 gmask;
    Unsign32 bmask;
    Unsign32 rshift;
    Unsign32 gshift;
    Unsign32 bshift;
} Pixel_info;


/* 
 * global variables 
 */
static Unsign8 imagedata[MAXIMAGEDATASIZE]; /* image data to be saved */
static Unsign8 cmapdata[MAXCMAPDATASIZE]; /* image data to be saved */
static Unsign8 imagedata24[MAXIMAGEDATASIZE24]; /* 24 bit image data to be saved*/

/*
 * extern funcitons
 */

/*
 * macros
 */

/* 
 * I_SYNC() synchronize frame buffer access, to be called before pixel read back
 * It is currently a NOP
 */
#define I_SYNC()

/*
 * table of contents
 */

/* exported routines */
void i_check_image();
Unsign8 *i_get_pixel_array();
Unsign32 *i_get_pixel_array24();
Colormap i_get_window_cmap();
void i_pixel_to_rgb();
void i_pixel_to_rgb24();
int i_get_bits_per_rgb();
void i_save_image();
void i_save_image24();
void i_compare_image_to_file();
void i_compare_image_to_file24();

/* local routines */
static void i_save_image_info();
static void i_get_file_info();
static void i_get_file_data();
static void i_get_file_data24();
static void i_write();
static void i_read();
static void i_16_to_8();
static void i_8_to_16();
static void i_32_to_8();
static void i_8_to_32();
static void i_store_image();
static void i_store_image24();
static void i_store_cmap();
static void i_get_cmap_data();
static Unsign8 i_scale_ushort_to_uchar();
static char *emalloc();

/*
 * i_check_image(imagename,display,window,x,y,w,h,text) 
 * Skip this operation if the environment variable NO_IMAGE_CHECK is set
 * All image files will have extension ".img" appended to file path
 */
void
i_check_image(imagename,display,window,x,y,w,h,text) 
        char *imagename;	/* name of the image file (not full path) */
	Display *display;	/* X display */
	Window window;		/* X window */
	int x;			/* origin x in dc */
	int y;			/* origin y in dc */
	int w;			/* width in dc */
	int h;			/* height in dc */
	char *text;		/* image description */
{
    static char refpath[MAXFILENAMELEN]; 
    static char curpath[MAXFILENAMELEN]; 
    static char badpath[MAXFILENAMELEN+1]; 

    char *curp = NULL;		/* pointer to previously-saved image*/
    char *refp = NULL;		/* pointer to good reference image*/
    char *badp = NULL;		/* pointer to bad reference image*/
    int cmpstatus;
    unsigned long diffcount;
    int result;
    static char cmdbuf[CMDBUFSIZE];
    XWindowAttributes attr;
    int framedepth;
    Pixel_info pinfo;

    if (nocheckpix) {
        tvprintf(0,"Warning: i_check_image: image checking disabled\n");
	return;
    }
    if ((w > MAXIMAGEDIM) || (h > MAXIMAGEDIM)) {
        tfprintf("Warning: i_check_image: image width or height > limit (%d)\n",MAXIMAGEDIM);
        tfprintf("         Image save/compare will NOT be done!!\n");
	return;
    }

    if (!XGetWindowAttributes(display,window,&attr))
    	tabort("ERROR: i_check_image: in XGetWIndowAttributes, exiting... \n");

    if (attr.depth != 8 && attr.depth != 24)
	tabort("ERROR: unsupported frame buffer depth %d, exiting...",attr.depth);
    framedepth = attr.depth;
    if (framedepth == 24) {
    	pinfo.rmask = attr.visual->red_mask;
    	pinfo.gmask = attr.visual->green_mask;
    	pinfo.bmask = attr.visual->blue_mask;
    	for(pinfo.rshift = 0; 
		! (pinfo.rmask & (1 << pinfo.rshift)); pinfo.rshift++)
			;
    	for(pinfo.gshift = 0; 
		! (pinfo.gmask & (1 << pinfo.gshift)); pinfo.gshift++)
			;
    	for(pinfo.bshift = 0; 
		! (pinfo.bmask & (1 << pinfo.bshift)); pinfo.bshift++)
			;
    }

#if 0
    USLEEP (delay);
#endif

    if (ref_image_dir == NULL)
	tabort("ERROR: REF_IMAGE_DIR is not specified, exiting...");
    if (bad_image_dir == NULL)
	tabort("ERROR: BAD_IMAGE_DIR is not specified, exiting...");
    if (cur_image_dir == NULL)
	tabort("ERROR: CUR_IMAGE_DIR is not specified, exiting...");
    sprintf(refpath,"%s/%s.img",ref_image_dir,imagename);
    sprintf(curpath,"%s/%s.img",cur_image_dir,imagename);
    sprintf(badpath,"%s/%s-.img",bad_image_dir,imagename);
    if (access(refpath, R_OK) == 0) refp = refpath;
    if (access(badpath, R_OK) == 0) badp = badpath;
    if (access(curpath, R_OK) == 0) curp = curpath;


    /* CASE 1: no reference image found -- save it for inspection */
    if (!refp) {
	tfprintf("failed: no reference image for %s (saving current image)\n",
		 imagename);
	if (framedepth == 8) {
	    i_save_image(curpath, display,window,x,y,w,h,text);
	} else if (framedepth == 24) {
	    i_save_image24(curpath, display,window,x,y,w,h,text,&pinfo);
	}
	return;
    }

    /* there is a reference image, compare */
    if (framedepth == 8) {
        i_compare_image_to_file(refpath,display,window,x,y,w,h, &cmpstatus, 
		&diffcount);
    } else if (framedepth == 24) {
        i_compare_image_to_file24(refpath,display,window,x,y,w,h, 
		&pinfo,&cmpstatus,&diffcount);
    }
    if (cmpstatus == 0) {
        /* CASE 2: image matches reference, delete current image */
	if (curp != NULL) {
	    /* a copy of this image exists in currimages - delete it */
	    if (unlink(curp) == 1) {
		tvprintf(0,"error: image %s matched, but couldn't delete current image %s\n",
		    imagename, curp);
	    }
	} 
	if (badp != NULL) {
	    /* bad reference exists - delete it */
	    if (unlink(badp) == 1) {
		tvprintf(0,"error: image %s matched, but couldn't delete bad image %s\n",
		    imagename, badp);
	    }
	} 
	return;
    } else {
	if (badp != NULL) {
	    /* there's a known bad reference - compare */
            if (framedepth == 8) {
	        i_compare_image_to_file(badp,display,window,x,y,w,h, 
					    &cmpstatus, &diffcount);
            } else if (framedepth == 24) {
	        i_compare_image_to_file24(badp,display,window,x,y,w,h, 
			&pinfo,&cmpstatus, &diffcount);
            }
	    if (cmpstatus == 0) {
    	    /* CASE 3: matches known bad reference, delete current image */
		tfprintf("failed: image comparison (%s matched known bad image)\n",
			imagename);
	        if (curp != NULL) {
	            /* a copy of this image exists in currimages - delete it */
	            if (unlink(curp) == 1) {
		        tvprintf(0,"error: image %s matched bad reference, but couldn't delete current image %s\n",
		        imagename, curp);
		    }
		}
	    } else {
    	    /* CASE 4: doesn't match bad reference, save it for inspection*/
	        tfprintf("failed: pending manual comparison of image %s\n",
			imagename);
	        if (framedepth == 8) {
	    	    i_save_image(curpath, display,window,x,y,w,h,text);
	        } else if (framedepth == 24) {
	    	    i_save_image24(curpath, display,window,x,y,w,h,text,&pinfo);
	        }
	    }
        } else {
    	    /* CASE 5: no known bad reference, save it for inspection */
		tfprintf("failed: pending manual comparison of image %s\n",
		    imagename);
	        if (framedepth == 8) {
	    	    i_save_image(curpath, display,window,x,y,w,h,text);
	        } else if (framedepth == 24) {
	    	    i_save_image24(curpath, display,window,x,y,w,h,text,&pinfo);
	        }
	}
    }
}


/*
 Image file format:
<1 byte : flag image type, currently only support PseudoColor and TrueColor>
<2 bytes: number of bytes of ascii text to follow>
<n bytes: ascii text>
<2 bytes: width of image>
<2 bytes: height of image>
<2 bytes: number of colormap data entries>
<n bytes: colormap data of the form 
	(pixel[1 byte],red[1 byte],green[1 byte],blue[1 byte]) quadriples >
<4 bytes: number of bytes of image data>
<n bytes: image data of the form (number[1 byte],pixel[1 byte]) pairs >
*/


/*
 * i_save_image(filename,display,window,x,y,w,h,text) save a window image to a fi
le 
 */
void
i_save_image(filename,display,window,x,y,w,h,text)
	char *filename;		/* reference image file name */
	Display *display;	/* X display */
	Window window;		/* X window */
	int x;			/* origin x in dc */
	int y;			/* origin y in dc */
	int w;			/* width in dc */
	int h;			/* height in dc */
	char *text;		/* image description */
{
	int fd;
	Unsign32 ncoldata,nimgdata;
	static char pixelflags[MAXCOLOR];
	Unsign8 buf[4];

	if ((fd = open(filename,O_CREAT|O_WRONLY|O_TRUNC,0666)) == -1)
		tabort("ERROR: i_save_image: in opening \"%s\", exiting...\n",filename);
	i_save_image_info(fd,x,y,w,h,text,PSEUDOCOLOR);
	i_store_image(display,window,x,y,w,h,pixelflags,&nimgdata,imagedata);
	i_store_cmap(display,window,pixelflags,&ncoldata,cmapdata);
	/* 
	 * store color map size and data <ncoldata bytes> 
	 */
	i_write(fd,(int)ncoldata,(char*)cmapdata);
	/* 
	 * store image size and data <nimgdata bytes> 
	 */
	i_write(fd,(int)nimgdata,(char*)imagedata);
	if (close(fd) == -1)
		tabort("ERROR: i_save_image: in closing \"%s\", exiting...\n",filename);
}

/*
 * i_save_image_info(fd,x,y,w,h,text)
 * save a image information to a file
 */
static void
i_save_image_info(fd,x,y,w,h,text,type)
        int fd;                 /* file fd */
        int x;                  /* origin x in dc */
        int y;                  /* origin y in dc */
        int w;                  /* width in dc */
        int h;                  /* height in dc */
        char *text;             /* image description */
	int type;		/* visual type */
{
        Unsign8 buf[4];

        if (w > MAXIMAGEDIM)
                tabort("ERROR: i_save_image_info: cannot save image of width > limit (%d)\n",MAXIMAGEDIM);
        if (h > MAXIMAGEDIM)
                tabort("ERROR: i_save_image_info: cannot save image of height > limit (%d)\n",MAXIMAGEDIM);
        buf[0] = type;
        i_16_to_8(strlen(text),&buf[1]);
        /*
         * store display type <1 byte> and length of text <2 bytes>
         */
        i_write(fd,3,(char*)buf);
        /*
         * store text <strlen(text) bytes>
         */
        i_write(fd,strlen(text),(char*)text);
        i_16_to_8((unsigned short)w,&buf[0]);
        i_16_to_8((unsigned short)h,&buf[2]);
        /*
         * store image width and height, 4 bytes>
         */
        i_write(fd,4,(char*)buf);
}


/* 
 * i_store_image(display,window,x,y,w,h,pixelflags,ndata,imagedata)
 * Store run length encoded image data into a buffer
 * <OUT: ndata records the number of bytes of image size and data to be stored>
 * <OUT: imagedata has the format: first 4 bytes store the number bytes of 
 *       image data, followed by (number[1 byte],pixel[1 byte]) pairs >
 * <OUT: pixelflags has MAXCOLOR(0 to MAXCOLOR-1) entries, each would be set 
 *       if the corresponding pixel appears in the image>
 */
static void
i_store_image(display,window,x,y,w,h,pixelflags,ndata,imagedata)
	Display *display;	/* X display */
	Window window;		/* X window */
	int x;			/* origin x in dc */
	int y;			/* origin y in dc */
	int w;			/* width in dc */
	int h;			/* height in dc */
	char *pixelflags;	/* OUT pixel flags */
	Unsign32 *ndata;	/* OUT number of bytes of data */
	Unsign8 *imagedata; 	/* OUT saved image data */
{
	int i,j;
	Unsign8 *pixbuf,*pixp,*iptr,lastpix,count;

	iptr = &imagedata[4]; /* reserved 4 bytes for storing ndata */
	*ndata = 0;
	memset(pixelflags,0,MAXCOLOR);
	pixp = pixbuf = i_get_pixel_array(display,window,x,y,w,h);
	for (count=0,i=0;i<w*h;++i,pixp++) { 
		if (pixelflags[(int)*pixp]==0) {
			pixelflags[(int)*pixp] = 1;
		}
		if (count == 0) {
			lastpix = *pixp;
			count = 1;
		} else {
			if (*pixp == lastpix) {
				count++;
				if (count >= 255) { /* count only hold up to 255
 */
					*iptr++ = count;
					*iptr++ = lastpix;
					(*ndata) += SIZEIMGENTRY;
					count = 0;
				}
			} else { /* flush accumulated pixels */
				*iptr++ = count;
				*iptr++ = lastpix;
				(*ndata) += SIZEIMGENTRY;
				lastpix = *pixp;
				count = 1;
			}
		}

	}
	/* process remaining data */
	*iptr++ = count;
	*iptr++ = lastpix;
	(*ndata) += SIZEIMGENTRY;
	free (pixbuf);
	i_32_to_8(*ndata,&imagedata[0]);
	(*ndata) += 4;
}

/*
 * i_store_cmap(display,window,pixelflags,ndata,cmapdata) 
 * store cmap data into buffer
 * <IN:  pixelflags has 256(0-255) entries, each would be set if the 
 *       corresponding pixel appears in the image>
 * <OUT: cmapdata has the format:  
 *       first two bytes store the number bytes of cmap data, followed by
 *       (pixel[1 byte],red[1 byte],green[1 byte],blue[1 byte]) quadriples >
 */
static void
i_store_cmap(display,window,pixelflags,ndata,cmapdata)
	Display *display;	/* X display */
	Window window;		/* X window */
	char *pixelflags;	/* pixel flags */
	Unsign32 *ndata;	/* OUT number of bytes of data */
	Unsign8 *cmapdata; 	/* OUT saved cmap data */
{
	int i;
	Unsign16 r,g,b;
	Colormap cmap;
	Unsign8 *cp;
	Unsign16 ncol=0;
	int bits_per_rgb;

	bits_per_rgb = i_get_bits_per_rgb(display);
	cp = &cmapdata[2]; /* reserved two bytes for ncol */
	cmap = i_get_window_cmap(display,window);
	for (i=0;i<MAXCOLOR;++i) {
		if (pixelflags[i]) {
			cp[0] = i;
			ncol++;
			i_pixel_to_rgb(display,cmap,(unsigned long)i,
				bits_per_rgb,&cp[1],&cp[2],&cp[3]);
			cp += SIZECMAPENTRY;
		}
	}
	i_16_to_8(ncol,&cmapdata[0]);
	*ndata = ncol*SIZECMAPENTRY + 2;
}


/*
 * i_compare_image_to_file(filename,display,window,x,y,w,h,status)
 * compares screen image to file image.
 * if success (matches), status = 0 
 * if image size different, status = 1
 * if fail (doesn't match), status = 2
 */
void
i_compare_image_to_file(filename,display,window,x,y,w,h,status,diffcount)
	char* filename;		/* file name */
	Display *display;	/* X display */
	Window *window;		/* X window */
	int x;			/* x in dc */
	int y;			/* y in dc */
	int w;			/* w in dc */
	int h;			/* h in dc */
	int *status;		/* OUT return status */
	unsigned long *diffcount;/* OUT number of different pixels */
{
	Unsign8 *pixbuf,*fimagedata;
	int i,j;
	Unsign8 npix,filepix,screenpix,*ip,*pp;
	Unsign32 npixel;
	int eqtbl[MAXCOLOR]; /*each entries maps file pixel to a screen pixel*/

	Unsign8 fcmaptbl[MAXCOLOR][3]; /* file cmap table has the format:
	  MAXCOLOR entries of red(1byte),green(1byte),blue(1byte) triples */

	Unsign8 scmaptbl[MAXCOLOR][4]; /* screen cmap table has the format:
	  MAXCOLOR entries of red(1byte),green(1byte),blue(1byte),flag(1byte)*/

	Colormap cmap;
	Unsign16 fw,fh;
	int bits_per_rgb;

	bits_per_rgb = i_get_bits_per_rgb(display);
	*status = 0;
	*diffcount = 0;

	i_get_file_data(display,filename,&fw,&fh,&fimagedata,fcmaptbl);
        pixbuf = i_get_pixel_array(display,window,x,y,w,h);
	cmap = i_get_window_cmap(display,window);
	if (w > MAXIMAGEDIM)
		tabort("ERROR: i_compare_image_to_file: cannot compare image of width > limit (%d)\n",MAXIMAGEDIM);
	if (h > MAXIMAGEDIM)
		tabort("ERROR: i_compare_image_to_file: cannot compare image of height > limit (%d)\n",MAXIMAGEDIM);
	if (w != fw || h != fh ) {
		tvprintf(1,"ERROR: i_compare_image_to_file: screenimage's dim (%d,%d) != fileimage's (%d,%d)\n", w,h,fw,fh);
		*status = 1;
		return;
	} 
	for (i=0;i<MAXCOLOR;++i) { /* initialize tables */
		scmaptbl[i][3] = 0;
		eqtbl[i] = -1;
	}
	for (npixel=0,npix=0,i=0,j=0,ip=fimagedata,pp=pixbuf;
		npixel<w*h;npixel++,npix--) {
		if (npix==0) {
			npix = *ip++;
			filepix = *ip++;
		}
		screenpix = *pp++;
		if (eqtbl[filepix] != (int) screenpix) { 
			if (scmaptbl[screenpix][3] == 0) { /* new pixel */
			    	scmaptbl[screenpix][3] = 1;
				i_pixel_to_rgb(display,cmap,(unsigned long)screenpix,bits_per_rgb,
					&(scmaptbl[screenpix][0]),
					&(scmaptbl[screenpix][1]),
					&(scmaptbl[screenpix][2]));
				
			}
			if (fcmaptbl[filepix][0] == scmaptbl[screenpix][0] &&
			    fcmaptbl[filepix][1] == scmaptbl[screenpix][1] &&
			    fcmaptbl[filepix][2] == scmaptbl[screenpix][2] ) {
				eqtbl[filepix] = (int)screenpix;
			} else {
				*status = 2;
				(*diffcount)++;
		    	}
		}
		if (++i == w) {
			i = 0;
			j++;
		}
	}
	free(fimagedata);
	free(pixbuf);
}

/*
 * i_get_file_data(display,filename,w,h,imagedata,cmaptbl) 
 * get image and cmap data from a file 
 */
static void
i_get_file_data(display,filename,w,h,imagedata,cmaptbl) 
	Display *display;	/* X display */
	char *filename;		/* image file name */
        Unsign16 *w;           	/* OUT width */
        Unsign16 *h;           	/* OUT height */
        Unsign8 **imagedata; 	/* OUT image data */
        Unsign8 cmaptbl[][3]; 	/* OUT colormap structure */
{
        int fd;
	Unsign32 nbyte;
	Unsign16 ntext,ncol;
	Unsign8 buf[4];
	int type;

        if ((fd = open(filename,O_RDONLY)) == -1)
                tabort("ERROR: i_get_file_data: in opening \"%s\", exiting...\n",filename);
	i_get_file_info(fd,w,h,&type);
	if (type != PSEUDOCOLOR) /* check image file type */
                tabort("ERROR: i_get_file_data: image file type != PSEUDOCOLOR, exiting...\n");
	/* read image's size of cmap data entries */
	i_read(fd,2,(char*)buf);
	i_8_to_16(&buf[0],&ncol); 	
	if (ncol > MAXCOLOR)
		tabort("i_get_file_data: invalid data: number of colors > %d, exiting...\n",MAXCOLOR);
	/* read cmap data */
	i_read(fd,(int)(ncol*SIZECMAPENTRY),(char*)cmapdata);
	i_get_cmap_data(ncol,cmapdata,cmaptbl);
	/* read size of image data */
	i_read(fd,4,(char*)buf);
	i_8_to_32(&buf[0],&nbyte);
	*imagedata = (unsigned char*) emalloc(nbyte);
	/* read image data */
	i_read(fd,(int)nbyte,(char*)*imagedata);
        if (close(fd) == -1)
                tabort("ERROR: i_get_file_data: in closing \"%s\", exiting...\n",filename);

}

/*
 * i_get_file_info(fd,w,h,type)
 * get general image data info from an image file
 */
static void
i_get_file_info(fd,w,h,type)
        int fd;         /* file descriptor */
        Unsign16 *w;    /* image width */
        Unsign16 *h;    /* image height */
        int *type;      /* visual type */
{
        Unsign8 buf[4];
        Unsign16 ntext;
        char foo[BUFSIZE];
 
        /* read filetype and text size */
        i_read(fd,3,(char*)buf);
        *type = buf[0];
        i_8_to_16(&buf[1],&ntext);
        if (ntext > BUFSIZE)
                tabort("i_get_file_info: text too long (%d), exiting...\n",ntext);
        /* read text */
        i_read(fd,(int)ntext,(char*)foo);
        /* read image's width and height and size of cmap data entries */
        i_read(fd,4,(char*)buf);
        i_8_to_16(&buf[0],w);
        i_8_to_16(&buf[2],h);
}
 


/*
 *  i_get_cmap_data(ncol,cmapdata,cmaptbl);
 *  read cmap data from file and create cmap
 *  <IN: cmapdata has the format:  
 *       (pixel[1 byte],red[1 byte],green[1 byte],blue[1 byte]) quadriples >
 *  <OUT: cmaptbl has the form red(1byte),green(1byte),blue(1byte) triples>
 */
static void
i_get_cmap_data(ncol,cmapdata,cmaptbl)
        Unsign16 ncol;    	/* number of colors */
        Unsign8 *cmapdata;	/* cmap data */
        Unsign8 cmaptbl[][3]; 	/* OUT color mapping table */
{
	int i,j;
	Unsign8 *cp;

        for (cp=cmapdata,i=0;i<(int)ncol;++i,cp+=4) {
                j = cp[0];
                cmaptbl[j][0] = cp[1];
                cmaptbl[j][1] = cp[2];
                cmaptbl[j][2] = cp[3];
        }
}


/*
 * i_get_pixel_array(display,window,x,y,w,h) return a pixel array 
 */
Unsign8 *
i_get_pixel_array(display,window,x,y,w,h)
	Display *display;	/* X display */
        Window window;          /* X window */
        int x;                  /* x in dc */
        int y;                  /* y in dc */
        int w;                  /* w in dc */
        int h;                  /* h in dc */
{ 
	int i,j;
	XWindowAttributes attr;
	Unsign8 *buf,*bp;
	XImage *image;
	GC gc;
	unsigned long pi;

	if (!XGetWindowAttributes(display,window,&attr))
		tabort("ERROR: i_get_pixel_array: in XGetWIndowAttributes, exiting... \n");
	if (attr.visual->class != PseudoColor)
		tabort("ERROR: i_get_pixel_array: visual type != PSEUDOCOLOR, exiting...\n");
	/* 
	 * Return a pointer to the pixel array. 
	 * This routine automatically allocate memory for the pixel buffer 
	 */
	if (w<=0 || h<=0 || w>MAXIMAGEDIM || h>MAXIMAGEDIM || 
			x < 0 || y <0 || x+w >attr.width || y+h > attr.height) {
		tabort("i_get_pixel_array: invalid parameters, exiting...\n");
	}
	bp = buf = (Unsign8 *) emalloc(w*h*sizeof(char));
	XSync(display,False);
	I_SYNC();
	image = XGetImage(display,window,x,y,w,h,-1,ZPixmap);
	if (!image)
		tabort("ERROR: i_get_pixel_array: in XGetImage, exiting...\n");
	for (j=0;j<h;++j) {
		for (i=0;i<w;++i) {
			/* assuming pixel value < 256 */
			*bp++ = (unsigned char) XGetPixel(image,i,j);
		}
	}
	if (showpix) { /* draw a bounding box */
		gc = XCreateGC(display,window,0,0);
		XSetForeground(display,gc,WhitePixel(display,DefaultScreen(display)));
		XDrawRectangle(display,(Drawable)window,gc,x-1,y-1,w+1,h+1);
		XFreeGC(display,gc);
	}
	return(buf);
}

/*
 * i_get_window_cmap(display,window) return the colormap of the window
 */
Colormap
i_get_window_cmap(display,window)
	Display *display;	/* X display */
        Window window;          /* X window */
{
	/* Return the colormap of the window */
	XWindowAttributes attr;

	if (!XGetWindowAttributes(display,window,&attr))
		tabort("ERROR: i_get_window_cmap: in XGetWIndowAttributes, exiting...\n");
	return(attr.colormap);
}

/*
 * i_get_bits_per_rgb(display)
 */
int 
i_get_bits_per_rgb(display)
	Display *display;	/* X display */
{
	Visual *visual;
	int bits_per_rgb;

	visual = DefaultVisual(display,DefaultScreen(display));
	bits_per_rgb = visual->bits_per_rgb;
#ifdef BITS_PER_RGB_BUG
	tvprintf(4,"WARNING: test is faking inquiry of bits per rgb of visual\n");
	bits_per_rgb = 8;
#endif
	if (bits_per_rgb != 8 && bits_per_rgb != 16) 
		tabort("i_get_bits_per_rgb: unknown bits_per_rgb = %d, exiting...\n", bits_per_rgb);
	return(bits_per_rgb);
}

/*
 * i_pixel_to_rgb(display,cmap,pixel,r,g,b)
 * retrieve the the RGB components of the pixel
 */
void
i_pixel_to_rgb(display,cmap,pixel,bits_per_rgb,r,g,b)
	Display *display;	/* X display */
	Colormap cmap;		/* X color map */
	unsigned long pixel;	/* pixel */
	int bits_per_rgb;	/* bits per rgb for display */
	Unsign8 *r;		/* OUT red */
	Unsign8 *g;		/* OUT green */
	Unsign8 *b;		/* OUT blue */
{
	XColor color;

	color.pixel = pixel;
	XQueryColor(display,cmap,&color);
	*r = i_scale_ushort_to_uchar(bits_per_rgb,color.red); 
	*g = i_scale_ushort_to_uchar(bits_per_rgb,color.green); 
	*b = i_scale_ushort_to_uchar(bits_per_rgb,color.blue); 
}

/*
 *  i_scale_ushort_to_uchar(bits_per_rgb,value)
 */
static Unsign8 
i_scale_ushort_to_uchar(bits_per_rgb,value)
        int bits_per_rgb;
        unsigned short value;
{
	switch (sizeof(unsigned short)) {
	case 1:
		return( (Unsign8 )value );
	case 2:
		switch (bits_per_rgb) {
		case 8:
			return( (Unsign8) (value >> 8));
		case 16:
                	return((Unsign8)((float)value*1.0/65535*255));
		default:
			tabort("i_scale_ushort_to_uchar: unknown bits_per_rgb %d, exiting...\n",bits_per_rgb);
		}
	default:
		tabort("abort: i_scale_ushort_to_uchar: unknown size for unsigned short %d, exiting...\n",sizeof(unsigned short));
	}
}


/*
 24 bit Image file format:
<1 byte : flag image type, currently only support PseudoColor and TrueColor>
<2 bytes: number of bytes of ascii text to follow>
<n bytes: ascii text>
<2 bytes: width of image>
<2 bytes: height of image>
<4 bytes: number of bytes of image data>
<n bytes: image data of the form 
	(number[1 byte],red[1 byte],green[1 byte],blue[1 byte]) quadruples>
*/


/*
 * i_save_image24(filename,display,window,x,y,w,h,text,pinfo) 
 * save a window image to a file 
 */
void
i_save_image24(filename,display,window,x,y,w,h,text,pinfo)
	char *filename;		/* reference image file name */
	Display *display;	/* X display */
	Window window;		/* X window */
	int x;			/* origin x in dc */
	int y;			/* origin y in dc */
	int w;			/* width in dc */
	int h;			/* height in dc */
	char *text;		/* image description */
	Pixel_info *pinfo;	/* pixel conversion info */
{
	int fd;

	if ((fd = open(filename,O_CREAT|O_WRONLY|O_TRUNC,0666)) == -1)
		tabort("ERROR: i_save_image: in opening \"%s\", exiting...\n",filename);
	i_save_image_info(fd,x,y,w,h,text,TRUECOLOR);
	i_store_image24(fd,display,window,x,y,w,h,pinfo);
	if (close(fd) == -1)
		tabort("ERROR: i_save_image: in closing \"%s\", exiting...\n",filename);
}

/* 
 * i_store_image24(fd,display,window,x,y,w,h,pinfo)
 * Store run length encoded image data into a buffer
 */
static void
i_store_image24(fd,display,window,x,y,w,h,pinfo)
	int fd;			/* file fd */
	Display *display;	/* X display */
	Window window;		/* X window */
	int x;			/* origin x in dc */
	int y;			/* origin y in dc */
	int w;			/* width in dc */
	int h;			/* height in dc */
	Pixel_info *pinfo;	/* pixel conversion info */
{
	int i,j;
	Unsign8 count,*iptr,r,g,b;
	Unsign32 *pixbuf,lastpix,*pixp,ndata;

	iptr = &imagedata24[4]; /* reserve 4 bytes for storing ndata */
	ndata = 0;
	pixp = pixbuf = i_get_pixel_array24(display,window,x,y,w,h);
	for (count=0,i=0;i<w*h;++i,pixp++) { 
		if (count == 0) {
			lastpix = *pixp;
			count = 1;
		} else {
			if ((*pixp & MASK24) == (lastpix & MASK24)) {
				count++;
				if (count >= 255) { /* count only hold up to 255
 */
					*iptr++ = count;
					i_pixel_to_rgb24(lastpix,pinfo,
						&r,&g,&b);
					*iptr++ = r;
					*iptr++ = g;
					*iptr++ = b;
					ndata += SIZEIMGENTRY24;
					count = 0;
				}
			} else { /* flush accumulated pixels */
				*iptr++ = count;
				i_pixel_to_rgb24(lastpix,pinfo,&r,&g,&b);
				*iptr++ = r;
				*iptr++ = g;
				*iptr++ = b;
				ndata += SIZEIMGENTRY24;
				lastpix = *pixp;
				count = 1;
			}
		}

	}
	/* process remaining data */
	*iptr++ = count;
	i_pixel_to_rgb24(lastpix, pinfo,&r,&g,&b);
	*iptr++ = r;
	*iptr++ = g;
	*iptr++ = b;
	ndata += SIZEIMGENTRY24;
	free (pixbuf);
	i_32_to_8(ndata,&imagedata24[0]);
	ndata += 4; /* extra 4 bytes to hold image size */
	/* 
	 * store image size and data <nimgdata bytes> 
	 */
	i_write(fd,(int)ndata,(char*)imagedata24);
}

/*
 * i_compare_image_to_file24(filename,display,window,x,y,w,h,pinfo,
 *	status,diffcount)
 * compares true color screen image to file image.
 * if success (matches), status = 0 
 * if image size different, status = 1
 * if fail (doesn't match), status = 2
 */
void
i_compare_image_to_file24(filename,display,window,x,y,w,h,pinfo,status,diffcount)
	char* filename;		/* file name */
	Display *display;	/* X display */
	Window window;		/* X window */
	int x;			/* x in dc */
	int y;			/* y in dc */
	int w;			/* w in dc */
	int h;			/* h in dc */
	Pixel_info *pinfo;	/* pixel conversion info */
	int *status;		/* OUT return status */
	unsigned long *diffcount;/* OUT number of different pixels */
{
	Unsign32 *pp,*pixbuf,npixel,screenpix;
	int i,j;
	Unsign8 *fimagedata,npix,filepix_r,filepix_g,filepix_b;
	Unsign8 screenpix_r,screenpix_g,screenpix_b,*ip;
	Unsign16 fw,fh;

	*status = 0;
	*diffcount = 0;

	i_get_file_data24(display,filename,&fw,&fh,&fimagedata);
        pixbuf = i_get_pixel_array24(display,window,x,y,w,h);
	if (w > MAXIMAGEDIM)
		tabort("ERROR: i_compare_image_to_file24: cannot compare image of width > limit (%d)\n",MAXIMAGEDIM);
	if (h > MAXIMAGEDIM)
		tabort("ERROR: i_compare_image_to_file24: cannot compare image of height > limit (%d)\n",MAXIMAGEDIM);
	if (w != fw || h != fh ) {
		tvprintf(1,"ERROR: i_compare_image_to_file24: screenimage's dim (%d,%d) != fileimage's (%d,%d)\n", w,h,fw,fh);
		*status = 1;
		return;
	} 
	for (npixel=0,npix=0,i=0,j=0,ip=fimagedata,pp=pixbuf;
		npixel<w*h;npixel++,npix--) {
		if (npix==0) {
			npix = *ip++;
			filepix_r = *ip++;
			filepix_g = *ip++;
			filepix_b = *ip++;
		}
		screenpix = *pp++;
		i_pixel_to_rgb24(screenpix,pinfo,
			&screenpix_r,&screenpix_g,&screenpix_b);
		if (screenpix_r != filepix_r ||
		    screenpix_g != filepix_g ||
		    screenpix_b != filepix_b) {
			*status = 2;
			(*diffcount)++;
		}
		if (++i == w) {
			i = 0;
			j++;
		}
	}
	free(fimagedata);
	free(pixbuf);
}

/*
 * i_get_file_data24(display,filename,w,h,imagedata) 
 * get image data from a true color image file 
 */
static void
i_get_file_data24(display,filename,w,h,imagedata) 
	Display *display;	/* X display */
	char *filename;		/* image file name */
        Unsign16 *w;           	/* OUT width */
        Unsign16 *h;           	/* OUT height */
        Unsign8 **imagedata; 	/* OUT image data */
{
        int fd,type;
	Unsign32 nbyte;
	Unsign8 buf[4];

        if ((fd = open(filename,O_RDONLY)) == -1)
                tabort("ERROR: i_get_file_data24: in opening \"%s\", exiting...\n",filename);
	i_get_file_info(fd,w,h,&type);
	if (type != TRUECOLOR) /* check image file type */
                tabort("ERROR: i_get_file_data24: image type != TRUECOLOUR, exiting...\n");
	i_read(fd,4,(char*)buf);
	i_8_to_32(&buf[0],&nbyte);
	*imagedata = (unsigned char*) emalloc(nbyte);
	/* read image data */
	i_read(fd,(int)nbyte,(char*)*imagedata);
        if (close(fd) == -1)
                tabort("ERROR: i_get_file_data24: in closing \"%s\", exiting...\n",filename);

}

/*
 * i_get_pixel_array24(display,window,x,y,w,h) 
 *	return a pixel array for 24 bit frame buffers
 */
Unsign32 *
i_get_pixel_array24(display,window,x,y,w,h)
	Display *display;	/* X display */
        Window window;          /* X window */
        int x;                  /* x in dc */
        int y;                  /* y in dc */
        int w;                  /* w in dc */
        int h;                  /* h in dc */
{ 
	int i,j;
	XWindowAttributes attr;
	Unsign32 *buf,*bp;
	XImage *image;
	GC gc;

	if (!XGetWindowAttributes(display,window,&attr))
		tabort("ERROR: i_get_pixel_array: in XGetWIndowAttributes, exiting... \n");
/*
	if (attr.visual->class != PseudoColor)
		tabort("ERROR: i_get_pixel_array: inpex currently supports only PSEUDOCOLOR, exiting...\n");
*/
	/* 
	 * Return a pointer to the pixel array. 
	 * This routine automatically allocate memory for the pixel buffer 
	 */
	if (w<=0 || h<=0 || w>MAXIMAGEDIM || h>MAXIMAGEDIM || 
			x < 0 || y <0 || x+w >attr.width || y+h > attr.height) {
		tabort("i_get_pixel_array24: invalid parameters, exiting...\n");
	}
	bp = buf = (Unsign32 *) emalloc(w*h*sizeof(Unsign32));
	XSync(display,False);
	I_SYNC();
	image = XGetImage(display,window,x,y,w,h,-1,ZPixmap);
	if (!image)
		tabort("ERROR: i_get_pixel_array24: in XGetImage, exiting...\n");
	for (j=0;j<h;++j) {
		for (i=0;i<w;++i) {
			*bp++ = XGetPixel(image,i,j);
		}
	}
	if (showpix) {
		gc = XCreateGC(display,window,0,0);
		XSetForeground(display,gc,0xffffff);
		XDrawRectangle(display,(Drawable)window,gc,x-1,y-1,w+1,h+1);
		XFreeGC(display,gc);
	}
	return(buf);
}

/*
 * i_pixel_to_rgb24(pixel,pinfo,r,g,b)
 * retrieve the the RGB components of the true color pixel
 */
void
i_pixel_to_rgb24(pixel,pinfo,r,g,b)
	unsigned long pixel;	/* pixel */
	Pixel_info *pinfo;	/* pixel conversion info */
	Unsign8 *r;		/* OUT red */
	Unsign8 *g;		/* OUT green */
	Unsign8 *b;		/* OUT blue */
{
	*r = (pixel & pinfo->rmask) >> pinfo->rshift;
	*g = (pixel & pinfo->gmask) >> pinfo->gshift;
	*b = (pixel & pinfo->bmask) >> pinfo->bshift;
}

/*
 * i_16_to_8(n,buf) convert 1 16-bit to 2 8-bit data
 */
static void
i_16_to_8(n,buf)
	Unsign16 n;
	Unsign8 *buf; 	/* buffer */
{
	buf[0] = (Unsign8) n;		/* extract lower byte */
	buf[1] = (Unsign8) (n >> 8);	/* extract higher byte */
}

/*
 * i_8_to_16(buf,n) convert 2 8-bit to 1 16-bit data
 */
static void
i_8_to_16(buf,n)
        Unsign8 *buf;     /* buffer */
        Unsign16 *n;
{
	/* add up low & high byte */
	*n = (Unsign16 ) buf[0] + (((Unsign16 ) buf[1]) << 8); 
}

/*
 * i_32_to_8(n,buf) convert 1 32-bit to 4 8-bit data
 */
static void
i_32_to_8(n,buf)
	Unsign32 n;
	Unsign8 *buf; 	/* buffer */
{
	buf[0] = (Unsign8) n;		
	buf[1] = (Unsign8) (n >> 8);	
	buf[2] = (Unsign8) (n >> 16);
	buf[3] = (Unsign8) (n >> 24);
}

/*
 * i_8_to_32(buf,n) convert 4 8-bit to 1 32-bit data
 */
static void
i_8_to_32(buf,n)
        Unsign8 *buf;     /* buffer */
        Unsign32 *n;
{
	/* add up all bytes */
	*n = (Unsign32) buf[0] + (((Unsign32) buf[1]) << 8) +
	     (((Unsign32) buf[2]) << 16) + (((Unsign32) buf[3]) << 24)
; 
}


/*
 * i_write(fd,n,buf) write n bytes of data in user buffer
 */
static void
i_write(fd,n,buf)
	int fd;		/* file descriptor */
	int n;		/* number of bytes */
	char *buf; 	/* data */
{
	if (write(fd,buf,n) != n)
		tabort("ERROR: i_write: in write(), exiting...\n");
}


/*
 * i_read(fd,n,buf)  read n bytes of data to buffer
 */
static void
i_read(fd,n,buf) 
	int fd;		/* file descritor */
	int n;		/* number of bytes */
	char *buf; 	/* OUT value */
{
	if (read(fd,buf,n)!= n)
               	tabort("ERROR: i_read: in read(), exiting...\n");
}


/*
 * emalloc(size) allocate memory.
 */
static char *
emalloc(size)
        int size;
{
        char *p; 
 
        if (! (p=(char*)malloc(size)))
                tabort("ERROR: emalloc: in malloc(), exiting...\n");
        return(p);
}

