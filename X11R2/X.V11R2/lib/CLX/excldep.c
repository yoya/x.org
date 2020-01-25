/*
 * Allegro CL dependent C helper routines for CLX
 */

#include <sys/types.h>
#include <sys/errno.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <signal.h>

#define ERROR -1
#define INTERRUPT -2

extern int errno;


int c_check_bytes(fd, howmany)
    int fd, howmany;
{
    int numavail;

    if (ioctl(fd, FIONREAD, (char *)&numavail) < 0) {
	perror("c_check_bytes");
	return (ERROR);
    }

    if (numavail >= howmany)
	return (1);
    else
	return (0);
}

	   
/*
 * Tries to read (end-start) characters into array at position start.
 * This routine may only be called when enough data is available on the socket,
 * otherwise we will block, which will prevent lisp from ever getting
 * control again.  Return ERROR on eof or error.
 */

int c_read_bytes(fd, array, start, end)
    int fd, start, end;
    unsigned char *array;

{
    int numwanted;

    numwanted = end - start;
    if (read(fd, (char *)&array[start], numwanted) < numwanted)
	return (ERROR);
    else
	return (numwanted);
}


/*
 * This is somewhat gross.  When the scheduler is not running we must provide
 * a way for the user to interrupt the read from the X socket from lisp.  So
 * we provide a separate reading function.
 */

int c_read_bytes_interruptible(fd, array, start, end)
    int fd, start, end;
    unsigned char *array;

{
    int numwanted, i, readfds;

    readfds = 1 << fd;
    numwanted = end - start;

    i = select(32, &readfds, (int *)0, (int *)0, (struct timeval *)0);
    if (i < 0)
	/* error condition */
	if (errno == EINTR)
	    return (INTERRUPT);
	else
	    return (ERROR);

    if (read(fd, (char *)&array[start], numwanted) < numwanted)
	return (ERROR);
    else
	return (numwanted);
}



#define OBSIZE 4096	/* X output buffer size */

static unsigned char output_buffer[OBSIZE];
static int obcount = 0;

/*
 * The inverse of above, which is simpler because there's no timeout. 
 * Don't need to block SIGIO's here, since the write either happens or it
 * fails, it doesn't block.
 */
int c_write_bytes(fd, array, start, end)
    int fd, start, end;
    unsigned char *array;
{
    int numwanted, i;
    void bcopy();

    numwanted = end - start;

    if (numwanted + obcount > OBSIZE)
	/* too much stuff -- we gotta flush. */
	if ((i = c_flush_bytes(fd)) < 0)
	    return (i);

    /* everything's cool, just bcopy */
    bcopy((char *)&array[start], (char *)&output_buffer[obcount],
	  numwanted);
    obcount += numwanted;

    return (numwanted);
}

int c_flush_bytes(fd)
    int fd;
{
    int i = 0, j = 0;

    while (obcount > 0) {
	i = write(fd, (char *)(&output_buffer[j]), obcount);
	if (i > 0) {
	    obcount -= i;
	    j += i;
	}
	else
	    return (ERROR);
    }

    return (i);
}
