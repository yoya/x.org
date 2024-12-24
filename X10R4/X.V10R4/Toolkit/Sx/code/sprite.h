/*
 *	$Source: /u1/Sx.new/code/RCS/sprite.h,v $
 *	$Header: sprite.h,v 1.1 86/12/03 16:12:06 swick Exp $
 */

/*
 * sprite.h --
 *
 * Common constants and type declarations for Sprite.
 *
 * Copyright (C) 1985 Regents of the University of California
 * All rights reserved.
 *
 *
 * $Header: sprite.h,v 1.1 86/12/03 16:12:06 swick Exp $ SPRITE (Berkeley)
 */

#ifndef _SPRITE
#define _SPRITE

/*
 * A boolean type is defined as an integer, not an enum.
 * This allows a boolean argument to be an expression that
 * isn't strictly 0 or 1 valued.
 */
typedef int Boolean;
#define TRUE	1
#define FALSE	0

/*
 * Functions that must return a status can return a ReturnStatus to
 * indicate success.
 */

typedef int  ReturnStatus;

/*
 * The following statuses overlap with the first 2 Generic statuses:
 *
 * SUCCESS			There was no error.
 * FAILURE			There was a general error.
 */

#define	SUCCESS			0x00000000
#define	FAILURE			0x00000001

/*
 * A nil pointer must be something that will cause an exception if 
 * referenced.  There are two nils: the kernels nil and the nil used
 * by user processes.
 */

#define NIL 	0xFFFFFFFF
#define USER_NIL 	0
#ifndef NULL
#define NULL	 	0
#endif NULL

/*
 * An address is just a pointer in C.  It is defined as a character pointer
 * so that address arithmetic will work properly, a byte at a time.
 */

typedef char *Address;

/*
 * Client Data is an uninterpreted word.  It is defined as an int so that
 * kdbx will not interpret client data as a string.  Unlike an "Address",
 * client data will generally not be used in arithmetic.
 */

typedef int *ClientData;

#endif _SPRITE
