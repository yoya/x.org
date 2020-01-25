/*
 * Copyright 1988, 1989, 1990, 1994 Network Computing Devices, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and
 * its documentation for any purpose is hereby granted without fee, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name Network Computing Devices, Inc. not be
 * used in advertising or publicity pertaining to distribution of this 
 * software without specific, written prior permission.
 * 
 * THIS SOFTWARE IS PROVIDED `AS-IS'.  NETWORK COMPUTING DEVICES, INC.,
 * DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING WITHOUT
 * LIMITATION ALL IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE, OR NONINFRINGEMENT.  IN NO EVENT SHALL NETWORK
 * COMPUTING DEVICES, INC., BE LIABLE FOR ANY DAMAGES WHATSOEVER, INCLUDING
 * SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES, INCLUDING LOSS OF USE, DATA,
 * OR PROFITS, EVEN IF ADVISED OF THE POSSIBILITY THEREOF, AND REGARDLESS OF
 * WHETHER IN AN ACTION IN CONTRACT, TORT OR NEGLIGENCE, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * $NCDId: @(#)compress_lzw.c,v 1.6 1994/09/01 17:31:04 lemke Exp $
 */
/* Copyright 1988, 1989, 1990 Network Computing Devices, Inc.  All rights reserved. */

/* $XConsortium: compress_lzw.c,v 1.7 94/12/01 20:14:02 mor Exp $ */

#include <X11/Xos.h>
#include <X11/Xfuncs.h>
#include <errno.h>
#ifdef X_NOT_STDC_ENV
extern int errno;
#endif
#include "lbxlzw.h"
#ifdef NCD
#define DEBUG
#include <ncd/assert.h>
#undef DEBUG
#else
#define assert(x)
#include <stdio.h>
#endif /* NCD */
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/param.h>
#include "lbxbufstr.h"

void LzwFree();

#ifdef LBX_STATS
int lzw_out_compressed;
int lzw_out_plain;
int lzw_in_compressed;
int lzw_in_plain;
#endif

#define BYTESTREAM_DAEMON

#define FALSE 0
#define TRUE 1

#ifndef MIN
#define MIN(_a, _b) ( ((_a) < (_b)) ? (_a) : (_b))
#endif

/* Defines for third byte of header */
#define BIT_MASK	0x1f
#define BLOCK_MASK	0x80

#define INIT_BITS	9	/* initial number of bits/code */
#define MAXBITS		14
#define HSIZE_12BITS	5003	/* 80% ((2^12)/5003)occupancy */
#define HSIZE_14BITS	18013	/* 91% ((2^14)/18013)occupancy */

/*
 * a code_int must be able to hold 2**BITS values of type int, and also -1
 */
#if MAXBITS > 15
typedef long int		code_int;
#else /* MAXBITS > 15 */
typedef short int		code_int;
#endif /* MAXBITS > 15 */
typedef	unsigned char		char_type;
typedef long int		count_int;

/* magic number in header */
#define MAGIC_1 037
#define MAGIC_2 0235

#define CHECK_GAP (2<<BITS)	/* check interval - clear tables */

/* data structure which will be overlaid ontop of bss */
struct compress_private {
    char_type * cp_inputbuf;
    char_type * cp_inputbufend;
    char_type * cp_outputbuf;
    char_type * cp_outputbufend;
    char_type * cp_packet;
    int cp_outputcount;
    int cp_n_bits;			/* number of bits/code */
    int cp_maxbits;			/* user settable max # bits/code */
    int cp_last_output_nbits;		/* compress: last known bits/code */
    code_int cp_maxcode;		/* maximum code, given n_bits */
    code_int cp_maxmaxcode;		/* should NEVER generate this code */
    code_int cp_free_ent;		/* first unused entry */
    code_int cp_save_finchar;		/* decompress: previous finchar */
    code_int cp_save_oldcode;		/* decompress: previous oldcode */
    char_type *cp_save_stackp;		/* decompress: previous stackp */
    int cp_exit_stat;
    int cp_block_compress;
    int cp_clear_flg;
    int cp_need_output_clear;		/* compress: output a CLEAR next */
    char_type cp_rmask[9];
    char_type cp_lmask[9];
    int cp_offset, cp_size;
    char_type cp_buf[MAXBITS];
    long int cp_in_count;		/* length of input */
    long int cp_bytes_out;		/* length of compressed output */
    long int cp_out_count;		/* # of codes output (for debugging) */
    count_int cp_checkpoint;
    code_int cp_hsize;			/* for dynamic table sizing */
    int cp_hshift;			/* for hash composition */
    count_int *cp_htab;			/* [HSIZE]; */
    unsigned short *cp_codetab;		/* [HSIZE]; */
};

struct LzwInfo {
    struct compress_private compress_state;
    struct compress_private decompress_state;
    int			    txport;
    int			    compress_off;
    LzwBuffer		    inbuf;
    LzwBuffer		    outbuf;
    unsigned char	    header[LZW_PACKET_HDRLEN];
    struct iovec	    iovbuf[2];
};

/* macro access to every field of the private structure */
#define BITS (priv->cp_maxbits)
#define HSIZE (priv->cp_hsize)
#define n_bits (priv->cp_n_bits)
#define last_output_nbits (priv->cp_last_output_nbits)
#define need_output_clear (priv->cp_need_output_clear)
#define maxbits (priv->cp_maxbits)
#define maxcode (priv->cp_maxcode)
#define maxmaxcode (priv->cp_maxmaxcode)
#define htab (priv->cp_htab)
#define codetab (priv->cp_codetab)
#define free_ent (priv->cp_free_ent)
#define save_finchar (priv->cp_save_finchar)
#define save_oldcode (priv->cp_save_oldcode)
#define save_stackp (priv->cp_save_stackp)
#define exit_stat (priv->cp_exit_stat)
#define block_compress (priv->cp_block_compress)
#define clear_flg (priv->cp_clear_flg)
#define rmask (priv->cp_rmask)
#define lmask (priv->cp_lmask)
#define offset (priv->cp_offset)
#define size (priv->cp_size)
#define buf (priv->cp_buf)
#define inputbuf (priv->cp_inputbuf)
#define inputbufend (priv->cp_inputbufend)
#define outputbuf (priv->cp_outputbuf)
#define outputbufend (priv->cp_outputbufend)
#define outputcount (priv->cp_outputcount)
#define packet (priv->cp_packet)
#define in_count (priv->cp_in_count)
#define bytes_out (priv->cp_bytes_out)
#define out_count (priv->cp_out_count)
#define checkpoint (priv->cp_checkpoint)
#define hsize (priv->cp_hsize)
#define hshift (priv->cp_hshift)

#define MAXCODE(n_bits)	((1L << (n_bits)) - 1)

#define EOF (-1)
/*
 * To save much memory, we overlay the table used by compress() with those
 * used by decompress().  The tab_prefix table is the same size and type
 * as the codetab.  The tab_suffix table needs 2**BITS characters.  We
 * get this from the beginning of htab.  The output stack uses the rest
 * of htab, and contains characters.  There is plenty of room for any
 * possible stack (stack used to be 8000 characters).
 */

#define htabof(i)	htab[i]
#define codetabof(i)	codetab[i]
#define tab_prefixof(i)	codetabof(i)
#define tab_suffixof(i)	((char_type *)(htab))[i]
#define de_stack	((char_type *)&tab_suffixof(1<<BITS))

#define fWRITE(buf,len) \
{ \
    int l = len; \
    char_type * b = buf; \
    while(l--) \
	if(outputbuf < outputbufend) \
	    *outputbuf++ = *b++; \
	else \
	    outputbuf++; \
}

static code_int getcode();
static void output();
static void cl_block();
static void cl_hash();

/*
 * the next two codes should not be changed lightly, as they must not
 * lie within the contiguous general code space.
 */ 
#define FIRST	257	/* first free entry */
#define	CLEAR	256	/* table clear output code */

/* Macros for decompression I/O */
#define GETCHAR() (inputbuf < inputbufend?*inputbuf++:-1)
#define PUTCHAR(c) ((*outputbuf = (c), (++outputbuf < outputbufend) ? 0 : -1))
#define READREQD() (clear_flg > 0 || offset >= size || free_ent > maxcode)

/* Macros for compression I/O */
#define getchar_compress() GETCHAR()
#define putchar_compress(c) {char_type C=(c);fWRITE(&C,1);}
/*
 * compress stdin to stdout
 *
 * Algorithm:  use open addressing double hashing (no chaining) on the 
 * prefix code / next character combination.  We do a variant of Knuth's
 * algorithm D (vol. 3, sec. 6.4) along with G. Knott's relatively-prime
 * secondary probe.  Here, the modular division first probe is gives way
 * to a faster exclusive-or manipulation.  Also do block compression with
 * an adaptive reset, whereby the code table is cleared after a full table
 * has been used for a fixed amount of time.  The variable-length output
 * codes are re-sized at this point, and a special CLEAR code is generated
 * for the decompressor.  Late addition:  construct the table according to
 * file size for noticeable speed improvement on small files.  Please direct
 * questions about this implementation to ames!jaw.
 */

static void
compress(priv)
    struct compress_private * priv;	/* local pointer to private data */
{
    register long fcode;
    register code_int i = 0;
    register int c;
    register code_int ent;
    register count_int hash_entry;
    register count_int * hash_ptr;
    register count_int * hash_base = &htab[0];
    register int disp;
    register code_int hsize_reg;
    register int Hshift;
    register int Maxbits;

#ifdef DEBUG
    if (verbose)
	printf("compress==== c=%d\n",in_count);
#endif /* DEBUG */

    ent = (in_count == 0) ? getchar_compress() : save_oldcode;

#ifdef DEBUG
    if (verbose)
	printf(" ent=%d\n",ent);
#endif /* DEBUG */

    hsize_reg = hsize;
    Hshift = hshift;
    Maxbits = maxbits;

    while ( (c = getchar_compress()) != EOF ) {
#ifdef DEBUG
	printf("<%d:%d> ",ent, c);
#endif /* DEBUG */
	fcode = (long) (((long) c << Maxbits) + ent);
 	i = (((long)c << Hshift) ^ ent);	/* xor hashing */

	hash_ptr = &htab[i];
	if ( (hash_entry = *hash_ptr) == fcode ) {
	    ent = codetabof(hash_ptr - hash_base);
	    continue;
	} else if ( (long)hash_entry < 0 )	/* empty slot */
	    goto nomatch;
	if ( i == 0 )
	    disp = 1;
	else
	    disp = hsize_reg - i;	/* secondary hash (after G. Knott) */
probe:
	if (hash_ptr - hash_base >= disp)
	    hash_ptr -= disp;
	else
	    hash_ptr += hsize_reg - disp;

	if ( (hash_entry = *hash_ptr) == fcode ) {
	    ent = codetabof(hash_ptr - hash_base);
	    continue;
	}
	if ( (long)hash_entry > 0 ) 
	    goto probe;
nomatch:
	output (priv, (code_int) ent );
	out_count++;
 	ent = c;
	if ( free_ent < maxmaxcode ) {
 	    codetabof(hash_ptr - hash_base) = free_ent++;/* code -> hashtable */
	    *hash_ptr = fcode;
	}
	else if ( block_compress )
	    cl_block (priv);
    }

    save_oldcode = ent;

#ifdef DEBUG
    if (verbose)
	printf("====compress\n");
#endif /* DEBUG */

    return;
}

static void
compress_flush(priv)
    struct compress_private * priv;	/* local pointer to private data */
{
    /*
     * Put out the final code.
     */
    output(priv, (code_int)save_oldcode );
    out_count++;
    output(priv, (code_int)-1 );

    return;
}

static void
clear_output (priv)
    struct compress_private * priv;	/* local pointer to private data */
{
 	cl_hash ( priv );
	free_ent = FIRST;
	clear_flg = 1;
	output (priv, (code_int) CLEAR );
#ifdef DEBUG
	if(verbose)
	    printf("compress: emitting CLEAR\n" );
#endif /* DEBUG */
}

static void
cl_block (priv)		/* table clear for block compress */
    struct compress_private * priv;	/* local pointer to private data */
{
    if (bytes_out > checkpoint) {
	clear_output(priv);			/* reset output codes */
	checkpoint = bytes_out + CHECK_GAP;
    }
}

static void
cl_hash(priv)		/* reset code table */
    struct compress_private * priv;	/* local pointer to private data */
{
    register count_int *htab_p = htab+hsize;
    register long i;
    register long m1 = -1;

    i = hsize - 16;
    do {				/* might use Sys V memset(3) here */
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    	*--htab_p = m1;
    } while ((i -= 16) >= 0);
    for ( i += 16; i > 0; i-- )
    	*--htab_p = m1;
}

/*
 * perform global data initializations.
 */
static void
init_compresss(priv, Maxbits, Hsize)
    struct compress_private *priv;	/* local pointer to private data */
    int			    Maxbits;
    int			    Hsize;
{
    unsigned long fcode;

    maxbits = Maxbits;
    maxmaxcode = 1L << Maxbits;
    hsize = Hsize;
    outputcount = 0;
    last_output_nbits = 0;		/* last known bits/code */
    need_output_clear = 0;		/* need to dump a CLEAR code */
    hshift = 0;
    for ( fcode = (long) hsize;  fcode < 65536L; fcode *= 2L )
    	hshift++;
    hshift = 8 - hshift;		/* set hash code range bound */
    save_finchar = 0;
    save_oldcode = 0;
    exit_stat = 0;
    block_compress = BLOCK_MASK;
    clear_flg = 0;
    maxcode = MAXCODE(n_bits = INIT_BITS);
    free_ent = ((block_compress) ? FIRST : 256 );	/* first unused entry */
    rmask[0] = 0x00;
    rmask[1] = 0x01;
    rmask[2] = 0x03;
    rmask[3] = 0x07;
    rmask[4] = 0x0f;
    rmask[5] = 0x1f;
    rmask[6] = 0x3f;
    rmask[7] = 0x7f;
    rmask[8] = 0xff;

    lmask[0] = 0xff;
    lmask[1] = 0xfe;
    lmask[2] = 0xfc;
    lmask[3] = 0xf8;
    lmask[4] = 0xf0;
    lmask[5] = 0xe0;
    lmask[6] = 0xc0;
    lmask[7] = 0x80;
    lmask[8] = 0x00;

    offset = 0, size = 0;
    bzero(buf,sizeof(buf));
    in_count = 0;			/* length of input */
    bytes_out = 0;			/* length of compressed output */
    out_count = 0;			/* # of codes output (for debugging) */
    checkpoint = CHECK_GAP;
    save_stackp = de_stack;
    inputbuf = inputbufend = NULL;
    packet = NULL;
    cl_hash( priv );			/* clear hash table */
}

/*****************************************************************
 * TAG( output )
 *
 * Output the given code.
 * Inputs:
 * 	code:	A n_bits-bit integer.  If == -1, then EOF.  This assumes
 *		that n_bits =< (long)wordsize - 1.
 * Outputs:
 * 	Outputs code to the file.
 * Assumptions:
 *	Chars are 8 bits long.
 * Algorithm:
 * 	Maintain a BITS character long buffer (so that 8 codes will
 * fit in it exactly).  Use the VAX insv instruction to insert each
 * code in turn.  When the buffer fills up empty it and start over.
 */

static void
output(priv, code )
    struct compress_private * priv;	/* local pointer to private data */
    code_int  code;
{
#ifdef DEBUG
    static int col = 0;
#endif /* DEBUG */

    /*
     * On the VAX, it is important to have the register declarations
     * in exactly the order given, or the asm will break.
     */
    register int r_off = offset, bits= n_bits;
    register char_type * bp = buf;

#ifdef DEBUG
	if ( verbose )
	    printf( "%5d%c", code,
		    (col+=6) >= 74 ? (col = 0, '\n') : ' ' );
#endif /* DEBUG */
    if ( code >= 0 ) {
/* 
 * byte/bit numbering on the VAX is simulated by the following code
 */
#ifdef DEBUG
	if ((verbose) && (code == CLEAR)) {
	    printf("output:CLEAR, n_bits=%d, offset=%d\n", n_bits, offset);
	}
#endif /* DEBUG */
	/*
	 * Get to the first byte.
	 */
	bp += (r_off >> 3);
	r_off &= 7;
	/*
	 * Since code is always >= 8 bits, only need to mask the first
	 * hunk on the left.
	 */
	*bp = (*bp & rmask[r_off]) | (code << r_off) & lmask[r_off];
	bp++;
	bits -= (8 - r_off);
	code >>= 8 - r_off;
	/* Get any 8 bit parts in the middle (<=1 for up to 16 bits). */
	if ( bits >= 8 ) {
	    *bp++ = code;
	    code >>= 8;
	    bits -= 8;
	}
	/* Last bits. */
	if(bits)
	    *bp = code;
	offset += n_bits;
	if ( offset == (n_bits << 3) ) {
	    bp = buf;
	    bits = n_bits;
	    bytes_out += bits;
	    do
		putchar_compress(*bp++)
	    while(--bits);
	    offset = 0;
	}

	/*
	 * If the next entry is going to be too big for the code size,
	 * then increase it, if possible.
	 */
	if ( free_ent > maxcode || (clear_flg > 0))
	{
	    /*
	     * Write the whole buffer, because the input side won't
	     * discover the size increase until after it has read it.
	     */
	    if ( offset > 0 ) {
		fWRITE( buf, n_bits);
		bytes_out += n_bits;
	    }
	    offset = 0;

	    if ( clear_flg ) {
    	        maxcode = MAXCODE (n_bits = INIT_BITS);
	        clear_flg = 0;
	    }
	    else {
	    	n_bits++;
	    	if ( n_bits == maxbits )
		    maxcode = maxmaxcode;
	    	else
		    maxcode = MAXCODE(n_bits);
	    }
#ifdef DEBUG
	    if ( debug ) {
		printf( "\nChange to %d bits\n", n_bits );
		col = 0;
	    }
#endif /* DEBUG */
	}
    } else {
	/*
	 * At EOF, write the rest of the buffer.
	 */
	if ( offset > 0 )
	    fWRITE( buf, (offset + 7) / 8);
	bytes_out += (offset + 7) / 8;
	offset = 0;
#ifdef DEBUG
	if ( verbose )
	    printf( "\n" );
#endif /* DEBUG */
    }
}

/*****************************************************************
 * TAG( getcode )
 *
 * Read one code from the standard input.  If EOF, return -1.
 * Inputs:
 * 	stdin
 * Outputs:
 * 	code or EOF is returned.
 */

static code_int
getcode(priv)
    struct compress_private * priv;	/* local pointer to private data */
{
    register code_int code;
    register int r_off, bits;
    register char_type *bp = buf;
#ifdef DEBUG
    static int col = 0;
#endif /* DEBUG */

    if ( clear_flg > 0 || offset >= size || free_ent > maxcode ) {
	/*
	 * If the next entry will be too big for the current code
	 * size, then we must increase the size.  This implies reading
	 * a new buffer full, too.
	 */
	if ( free_ent > maxcode ) {
	    n_bits++;
	    if ( n_bits == maxbits )
		maxcode = maxmaxcode;	/* won't get any bigger now */
	    else
		maxcode = MAXCODE(n_bits);
	}
	if ( clear_flg > 0) {
    	    maxcode = MAXCODE (n_bits = INIT_BITS);
	    clear_flg = 0;
	}
	{
	    int c;
	    int count = n_bits;
	    register char_type *bpx = buf;
	    size = 0;
	    while (count--) {
		c = GETCHAR();
		if (c == EOF)
		    break;
		*bpx++ = c;
		size++;
	    }
	}
	if ( size <= 0 )
	    return EOF;			/* end of file */
	offset = 0;
	/* Round size down to integral number of codes */
	size = (size << 3) - (n_bits - 1);
    }
    r_off = offset;
    bits = n_bits;
    /*
     * Get to the first byte.
     */
    bp += (r_off >> 3);
    r_off &= 7;
    /* Get first part (low order bits) */
    code = (*bp++ >> r_off);
    bits -= (8 - r_off);
    r_off = 8 - r_off;		/* now, offset into code word */
    /* Get any 8 bit parts in the middle (<=1 for up to 16 bits). */
    if ( bits >= 8 ) {
	code |= *bp++ << r_off;
	r_off += 8;
	bits -= 8;
    }
    /* high order bits. */
    code |= (*bp & rmask[bits]) << r_off;
    offset += n_bits;

#ifdef DEBUG
    if (verbose)
	printf("nextcode:%5d%c",
	    code, (col+=6) >= 74 ? (col = 0, '\n') : ' ' );
#endif /* DEBUG */
    return code;
}

/*
 * Decompress stdin to stdout.  This routine adapts to the codes in the
 * file building the "string" table on-the-fly; requiring no table to
 * be stored in the compressed file.  The tables used herein are shared
 * with those of the compress() routine.  See the definitions above.
 */

static void
decompress(priv, newpacket)
    struct compress_private * priv;	/* local pointer to private data */
    int			      newpacket;
{
    register char_type *stackp;
    register int finchar;
    register code_int code, oldcode, incode;
    register int restarting = newpacket;

#ifdef DEBUG
    if (verbose)
	printf("decompress==== in_count:%d, len=%d\n",
	    in_count, inputbufend-inputbuf);
#endif /* DEBUG */
    if (in_count == 0) {
	restarting = FALSE;
	finchar = oldcode = getcode(priv);
	in_count++;
	if(oldcode == EOF)	/* EOF already? */
	    return;			/* Get out of here */
#ifdef DEBUG
	if (verbose)
	    printf ("%02x ", (char)finchar);
#endif /* DEBUG */
	if (PUTCHAR( (char)finchar ) == EOF)	/* 1st must be 8 bits = char */
		goto output_full;
    } else {
	finchar = save_finchar;
	oldcode = save_oldcode;
    }
#ifdef DEBUG
    if (verbose)
	printf(" finchar=%d, oldcode=%d\n", finchar,oldcode);
#endif /* DEBUG */
    stackp = de_stack;

    while ( (code = getcode(priv)) != EOF ) {

	if ( (code == CLEAR) && block_compress ) {
#ifdef DEBUG
	    if (verbose)
		printf("compress: received CLEAR n_bits=%d\n", n_bits );
#endif /* DEBUG */
	    for ( code = 255; code >= 0; code-- )
		tab_prefixof(code) = 0;
	    clear_flg = 1;
	    free_ent = FIRST - 1;
	    restarting = FALSE;
	    if ( (code = getcode (priv)) == EOF )	/* O, untimely death! */
		break;
	}
	incode = code;
	/*
	 * Special case for KwKwK string.
	 */
	if ( code >= free_ent ) {
            *stackp++ = finchar;
	    code = oldcode;
	}

	/*
	 * Generate output characters in reverse order
	 */
	while ( code >= 256 )
	{
	    if (stackp >= (char_type *)&htab[HSIZE]) {
		printf ("incode=0x%x, n_bits=%d\n", incode, n_bits);
#ifdef BYTESTREAM_DAEMON
		printf
#else /* BYTESTREAM_DAEMON */
		panic
#endif /* BYTESTREAM_DAEMON */
		    ("compress1: stackp overflow");
	    }
	    *stackp++ = tab_suffixof(code);
	    code = tab_prefixof(code);
	}
	if (stackp >= (char_type *)&htab[HSIZE]) {
	    printf ("incode=0x%x, n_bits=%d\n", incode, n_bits);
#ifdef BYTESTREAM_DAEMON
		printf
#else /* BYTESTREAM_DAEMON */
		panic
#endif /* BYTESTREAM_DAEMON */
		    ("compress2: stackp overflow");
	}
	*stackp++ = finchar = tab_suffixof(code);

	/*
	 * Generate the new entry.
	 */
	if (!restarting) {
	    if ( (code=free_ent) < maxmaxcode ) {
#ifdef DEBUG
		printf("code:%d, prefix:%d, suffix:%d\n",
		    code, oldcode, finchar);
#endif /* DEBUG */
		tab_prefixof(code) = (unsigned short)oldcode;
		tab_suffixof(code) = finchar;
		free_ent = code+1;
	    } 
	} else
	    restarting = FALSE;
	/*
	 * Remember previous code.
	 */
	oldcode = incode;

	/*
	 * And put them out in forward order
	 */
	do {
	    if (PUTCHAR ( *--stackp ) == EOF)
		goto output_full;
	}
	while ( stackp > de_stack );
    }

output_full:
    save_finchar = finchar;
    save_oldcode = oldcode;
    save_stackp = stackp;
#ifdef DEBUG
    if (verbose)
	printf("====decompress\n");
#endif /* DEBUG */
}

/*
 * Allocate/Deallocate space for the compression tables.
 */
static int
AllocCompTables(comp, Hsize)
    struct LzwInfo *comp;
    int		   Hsize;
{
    if ((comp->compress_state.cp_htab = 
		(count_int *)Xalloc(Hsize * sizeof(count_int))) == NULL)
	goto nomem;
    if ((comp->compress_state.cp_codetab = 
		(unsigned short *)Xalloc(Hsize * sizeof(short))) == NULL)
	goto nomem;
    if ((comp->decompress_state.cp_htab = 
		(count_int *)Xalloc(Hsize * sizeof(count_int))) == NULL)
	goto nomem;
    if ((comp->decompress_state.cp_codetab = 
		(unsigned short *)Xalloc(Hsize * sizeof(short))) == NULL)
	goto nomem;
    return 0;

nomem:
printf("Not enough memory for compression tables\n");
    if (comp->compress_state.cp_htab)
	Xfree(comp->compress_state.cp_htab);
    if (comp->compress_state.cp_codetab)
	Xfree(comp->compress_state.cp_codetab);
    if (comp->decompress_state.cp_htab)
	Xfree(comp->decompress_state.cp_htab);
    return -1;
}

static int
GetNewPacket(comp)
    struct LzwInfo *comp;
{
    register struct compress_private *priv = &comp->decompress_state;
    int				     len;
    int				     result;

    if (packet)	{	/* Free up previous packet in input buffer */
	FreeInput(&comp->inbuf, inputbufend - packet);
	packet = NULL;
    }

    if ((result = GetInputPtr(comp->txport,
			      &comp->inbuf,
			      LZW_PACKET_HDRLEN,
			      &packet)) <= 0)
	return result;
    len = LZW_GET_DATALEN(packet);
    if ((result = GetInputPtr(comp->txport,
			      &comp->inbuf,
			      len + LZW_PACKET_HDRLEN,
			      &packet)) <= 0) {
	packet = NULL;
	return result;
    }

    return len;
}

static int
NewPacketAvail(comp)
    struct LzwInfo *comp;
{
    register struct compress_private *priv = &comp->decompress_state;
    char 			     *pkt;
    int				     len;

    if (packet)	{	/* Free up previous packet in input buffer */
	FreeInput(&comp->inbuf, inputbufend - packet);
	packet = NULL;
    }

    if (pkt = BYTES_AVAIL(&comp->inbuf, LZW_PACKET_HDRLEN)) {
	len = LZW_GET_DATALEN(pkt);
	if (BYTES_AVAIL(&comp->inbuf, len + LZW_PACKET_HDRLEN))
	    return TRUE;
    }

    return FALSE;
}

static int
PlainWrite(comp, buffer, buflen)
    struct LzwInfo *comp;
    unsigned char  *buffer;
    int            buflen;
{
    int		   retval;
    int		   lenleft = buflen;

    if ((retval = LzwFlush(comp->txport)) == 0) {
	register struct iovec *iov = comp->iovbuf;
	while (lenleft) {
	    int outlen, written;
	    if (outlen = iov[1].iov_len) {
		assert(iov[1].iov_len <= lenleft);
		iov[1].iov_base = (caddr_t) buffer;
	    }
	    else {
		outlen = MIN(lenleft, LZW_MAX_DATALEN);
		LZW_PUT_PKTHDR(comp->header, outlen, FALSE);
		iov[0].iov_base = (caddr_t) comp->header;
		iov[0].iov_len = LZW_PACKET_HDRLEN;
		iov[1].iov_base = (caddr_t) buffer;
		iov[1].iov_len = outlen;
	    }
	    if ((retval = FlushIovBuf(comp->txport, iov)) < 0)
		break;
	    written = outlen - retval;
	    lenleft -= written;
	    buffer += written;
	    if (retval)
		break;
	}
	if (lenleft == buflen)
	    return retval;
	else
	    return buflen - lenleft;
    }

    else if (retval > 0) {
	retval = -1;
	errno = EWOULDBLOCK;
    }

    return retval;
}

#define MAX_FDS	256

static struct LzwInfo	*per_fd[MAX_FDS];

/*
 * Initialize LZW compressor
 */
void *
LzwInit(txport, Maxbits)
    int txport;
    int Maxbits;
{
    register code_int		     code;
    register struct compress_private *priv;
    struct LzwInfo		     *comp;
    int				     Hsize;
    int				     ret1, ret2;

    if (Maxbits == 12)
	Hsize = HSIZE_12BITS;
    else if (Maxbits == 14)
	Hsize = HSIZE_14BITS;
    else
	return NULL;

    if ((comp = (struct LzwInfo *)Xalloc(sizeof(struct LzwInfo))) == NULL)
	return NULL;
    if (AllocCompTables(comp, Hsize) == -1) {
	Xfree(comp);
	return NULL;
    }

    ret1 = InitLzwBuffer(&comp->inbuf, INBUFFER_SIZE);
    ret2 = InitLzwBuffer(&comp->outbuf, OUTBUFFER_SIZE);
    if (ret1 < 0 || ret2 < 0) {
	LzwFree(comp);
	return NULL;
    }
    comp->compress_off = FALSE;
    comp->iovbuf[1].iov_len = 0;
    comp->txport = txport;

    init_compresss(&comp->compress_state, Maxbits, Hsize);
    init_compresss(&comp->decompress_state, Maxbits, Hsize);

    /* Initialize the first 256 entries in the table. */
    priv = &comp->decompress_state;
    for (code = 255; code >= 0; code--) {
	tab_prefixof(code) = 0;
	tab_suffixof(code) = (char_type)code;
    }

    per_fd[txport] = comp;
    return (void *)comp;
}

void
LzwFree(comp)
    struct LzwInfo *comp;
{
    if (!comp)
	return;
    per_fd[comp->txport] = 0;
    FreeLzwBuffer(&comp->inbuf);
    FreeLzwBuffer(&comp->outbuf);
    Xfree(comp->compress_state.cp_htab);
    Xfree(comp->compress_state.cp_codetab);
    Xfree(comp->decompress_state.cp_htab);
    Xfree(comp->decompress_state.cp_codetab);
    Xfree(comp);
}

int
LzwFlush(txport)
{
    struct LzwInfo *comp = per_fd[txport];
    struct compress_private *priv = &comp->compress_state;

    if (in_count) {
	int len;
	compress_flush(priv);
	len = outputbuf - (packet + LZW_PACKET_HDRLEN);
	LZW_PUT_PKTHDR(packet, len, TRUE);
#ifdef LBX_STATS
	lzw_out_compressed += len;
#endif
	CommitOutBuf(&comp->outbuf, len + LZW_PACKET_HDRLEN);
	in_count = 0;
    }

    return FlushOutBuf(comp->txport, &comp->outbuf);
}

int
LzwStuffInput(txport,buffer,buflen)
    int		    txport;
    unsigned char   *buffer;
    int		    buflen;
{
    struct LzwInfo	    *comp = per_fd[txport];

    if (StuffInput (&comp->inbuf, buffer, buflen) != buflen)
	return 0;
    return 1;
}

void
LzwCompressOn(txport)
    int txport;
{
    per_fd[txport]->compress_off = FALSE;
}

void
LzwCompressOff(txport)
    int txport;
{
    per_fd[txport]->compress_off = TRUE;
}

int
LzwWriteV(txport, iov, iovcnt)
    int		   txport;
    struct iovec   *iov;
    int		   iovcnt;
{
    int	i;
    int	total = 0;
    int	this_time;	    

    for (i = 0; i < iovcnt; i++)
    {
	this_time = LzwWrite(txport, iov[i].iov_base, iov[i].iov_len);
	if (this_time > 0)
	    total += this_time;
	if (this_time != iov[i].iov_len)
	{
	    if (total)
		return total;
	    return this_time;
	}
    }
    return total;
}

int
LzwWrite(txport, buffer, buflen)
    int		   txport;
    unsigned char  *buffer;
    int            buflen;
{
    struct LzwInfo	    *comp = per_fd[txport];
    struct compress_private *priv = &comp->compress_state;
    int			    len;
    int			    lenleft = buflen;
    unsigned char  	    *p = buffer;

    if (comp->compress_off) {
	return PlainWrite(comp, buffer, buflen);
    }

    while (lenleft) {
	if (in_count == 0) {
	    packet = (char_type *)
	      ReserveOutBuf(&comp->outbuf, LZW_PACKET_HDRLEN + LZW_MAX_OUTLEN);
	    if (!packet) {
		errno = EWOULDBLOCK;
		return -1;
	    }
	    outputbuf = packet + LZW_PACKET_HDRLEN;
	    outputbufend = outputbuf + LZW_MAX_OUTLEN;
	}

	len = MIN(LZW_MAX_PLAIN - in_count, lenleft);
#ifdef LBX_STATS
	lzw_out_plain += len;
#endif
	inputbuf = p;
	inputbufend = p + len;
	compress(priv);

	p += len;
	lenleft -= len;
	if ((in_count += len) == LZW_MAX_PLAIN) {
	    if (LzwFlush(txport) < 0) {
		if (lenleft == buflen)
		    return -1;
		return buflen - lenleft;
	    }
	}
    }

    return buflen;
}

int
LzwRead(txport, buffer, buflen)
    int		   txport;
    unsigned char  *buffer;
    int		   buflen;
{
    struct LzwInfo	    *comp = per_fd[txport];
    struct compress_private *priv = &comp->decompress_state;
    char_type		    *p = buffer;
    int			    lenleft = buflen;
    int			    len;
    int			    retval;
    int			    newpacket;

    /*
     * First check if there is any stuff left in the decompression stack
     */
    while (lenleft) {
	if (save_stackp == de_stack) break;
	*p++ = *--save_stackp;
	lenleft--;
    }

    /*
     * Need to decompress some more data
     */
    outputbuf = p;
    outputbufend = p + lenleft;
    while (outputbuf != outputbufend) {
	if (inputbuf == inputbufend && READREQD()) {
	    if ((retval = GetNewPacket(comp)) <= 0)
		break;
	    inputbuf = packet + LZW_PACKET_HDRLEN;
	    inputbufend = inputbuf + retval;
	    newpacket = TRUE;
#ifdef LBX_STATS
	    if (LZW_COMPRESSED(packet))
		lzw_in_compressed += retval - LZW_PACKET_HDRLEN;
#endif
	}
	else
	    newpacket = FALSE;

	if (LZW_COMPRESSED(packet))
	    decompress(priv, newpacket);
	else {
	    len = MIN(inputbufend - inputbuf, outputbufend - outputbuf);
	    memmove(outputbuf, inputbuf, len);
	    inputbuf += len;
	    outputbuf += len;
#ifdef LBX_STATS
	    lzw_in_plain -= len;	/* gets added back in later */
#endif
	}
    }

    if ((len = outputbuf - buffer) == 0)
	return retval;
    else {
#ifdef LBX_STATS
	lzw_in_plain += len;
#endif
	return len;
    }
}

int
LzwInputAvail(txport)
    int	txport;
{
    struct LzwInfo	    *comp = per_fd[txport];
    struct compress_private *priv = &comp->decompress_state;

    if ((save_stackp == de_stack) &&
	(inputbuf == inputbufend && READREQD()) &&
	NewPacketAvail(comp) <= 0)
	return FALSE;
    else
	return TRUE;
}
