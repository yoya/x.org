/*                            xmove
 *                            -----
 *             A Pseudoserver For Client Mobility
 *
 *   Copyright (c) 1994         Ethan Solomita
 *
 *   The X Consortium, and any party obtaining a copy of these files from
 *   the X Consortium, directly or indirectly, is granted, free of charge, a
 *   full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 *   nonexclusive right and license to deal in this software and
 *   documentation files (the "Software"), including without limitation the
 *   rights to use, copy, modify, merge, publish, distribute, sublicense,
 *   and/or sell copies of the Software, and to permit persons who receive
 *   copies from any such party to do so.  This license includes without
 *   limitation a license to do the foregoing actions under any patents of
 *   the party supplying this software to the X Consortium.
 */
/*
 * Project: XMON - An X protocol monitor
 * 
 * File: fd.c
 * 
 * Description: Support routines for file descriptors
 * 
 * All of this code is to support the handling of file descriptors.
 * The idea is to keep a table of the FDs that are in use and why.  For
 * each FD that is open for input, we keep the name of a procedure to call
 * if input arrives for that FD.  When an FD is created (by an open, pipe,
 * socket, ...) declare that by calling UsingFD.  When it is no longer in
 * use (close ...), call NotUsingFD.
 */

#include <errno.h>             /* for EINTR, EADDRINUSE, ... */

#ifdef SVR4
#include <sys/time.h>
#include <sys/resource.h>
#endif

#include "xmove.h"
#include "select_args.h"

/* function prototypes: */
/* fd.c: */

/* end function prototypes */

extern FDDescriptor				*FDD;
extern long						ReadDescriptors[];
extern long						WriteDescriptors[];
extern short					HighestFD;

Global void
InitializeFD()
{
	register short  i;

	enterprocedure("InitializeFD");
	/* get the number of file descriptors the system will let us use */

	/* allocate space for a File Descriptor Table */
	FDD = (FDDescriptor *)malloc((long)(MAXSOCKS * sizeof(FDDescriptor)));

	for (i = 0; i < MAXSOCKS; i++)
		FDD[i].InputHandler = (VoidCallback)NULL;

	CLEARBITS(ReadDescriptors);
	CLEARBITS(WriteDescriptors);
	HighestFD = 0;
}

Global FDDescriptor*
UsingFD(fd, Handler, private_data)
	int						fd;
	VoidCallback			Handler;
	Pointer					private_data;
{
	FDD[fd].InputHandler = Handler;
	FDD[fd].private_data = private_data;
	if (Handler == NULL)
		BITCLEAR(ReadDescriptors, fd);
	else
		BITSET(ReadDescriptors, fd);

	FDD[fd].pass_through = False;
	
	if (fd > HighestFD)
		HighestFD = fd;
	return &FDD[fd];
}

Global void
NotUsingFD(fd)
		int fd;
{
	FDD[fd].InputHandler = (VoidCallback)NULL;
	BITCLEAR(ReadDescriptors, fd);

	while (FDD[HighestFD].InputHandler == (VoidCallback)NULL && HighestFD > 0)
		HighestFD -= 1;
}

Global void
EOFonFD(fd)
		int fd;
{
	close(fd);
	NotUsingFD(fd);
}
