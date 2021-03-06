/*
 * patchlevel.h	
*/

#define PATCHLEVEL 3

/*
 * Version 1.0 
 *	Initial public release
 *	bingle@cs.purdue.edu (7/18/91)
*/

/*
 * Version 1.1
 *	Fixed some errors in some of the string manipulations
 *	bingle@cs.purdue.edu (7/23/91)
*/

/*
 * Version 1.2
 *	R5 contrib tape release
 *	non-named colors allowed as initial foreground and background
 *		(welch@graft.berkeley.edu)
 *	Imakefile changes
 *		(gildea@expo.lcs.mit.edu)
 *	added registry iso8859 to font specifications
 *		(kim@grendel.lut.fi)
 *	bingle@cs.purdue.edu (10/3/91)
*/

/*
 * Version 1.3
 *	R6 contrib tape release
 *      always uses the color names when allocating new colors
 *      	instead of using the RGB values retrieved (from
 *		the possibly conflicting) client side rgb.txt
 *	no longer complains about colors that aren't found exactly
 *	no longer exits if color in rgb.txt file isn't found on
 *		server side
 *	bingle@cs.purdue.edu (6/6/94)
*/
