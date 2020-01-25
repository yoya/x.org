/*
 * XBoing - An X11 blockout style computer game
 *
 * (c) Copyright 1993, 1994, Justin C. Kibell, All Rights Reserved
 *
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge, a
 * full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and
 * documentation files (the "Software"), including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons who receive
 * copies from any such party to do so.  This license includes without
 * limitation a license to do the foregoing actions under any patents of
 * the party supplying this software to the X Consortium.
 *
 * In no event shall the author be liable to any party for direct, indirect,
 * special, incidental, or consequential damages arising out of the use of
 * this software and its documentation, even if the author has been advised
 * of the possibility of such damage.
 *
 * The author specifically disclaims any warranties, including, but not limited
 * to, the implied warranties of merchantability and fitness for a particular
 * purpose.  The software provided hereunder is on an "AS IS" basis, and the
 * author has no obligation to provide maintenance, support, updates,
 * enhancements, or modifications.
 */

/* HP Audio - .au much the same as how LINUXaudio.c is done */
/*            I`m not sure but does the HP audio device accept .au data
 *            and know what to do with the header?? Anyone know.
 *            Original by Justin Kibell - jck@citri.edu.au
 */

/*
 *  Include file dependencies:
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "include/error.h"
#include "include/audio.h"

/*
 *  Internal macro definitions:
 */

#define BUFFER_SIZE				(1024 * SBUF_SIZE * 2)	

/*
 *  Internal type declarations:
 */

/*
 *  Internal variable declarations:
 */

static char				*Audio_dev = "/dev/audio";
static int 				Audio_fd;
/* size should depend on sample_rate */
static unsigned char   	buf[BUFFER_SIZE];       
static char 			errorString[255];
static int 				parentid = 0;

#if NeedFunctionPrototypes
int SetUpAudioSystem(Display *display)
#else
int SetUpAudioSystem(display)
	Display *display;
#endif
{
	/* Try to open the audio device */
 	if (Audio_fd = open(Audio_dev, O_WRONLY))
  	{	
		/* Success - audio device opened */
  		return True;
  	}
 	else
  	{
		ErrorMessage("Cannot open audio device.");

		/* Bummer - cannot open audio device */
  		return False;
  	}
}

#if NeedFunctionPrototypes
void FreeAudioSystem(void)
#else
void FreeAudioSystem()
#endif
{
	flushAudioDevice();

	/* Close the audio device */
 	if (close(Audio_fd) < 0)
		ErrorMessage("Cannot open audio device.");
}

#if NeedFunctionPrototypes
static void flushAudioDevice(void)
#else
static void flushAudioDevice()
#endif
{
 	/* Flush any audio activity - hope this works */
#ifdef PleaseFixTheseDefines
 	if (ioctl(Audio_fd, I_FLUSH, FLUSHW) < 0)
  	{
  		sprintf(errorString, "Unable to flush audio device.");
  		WarningMessage(errorString);
  		return;
  	}
#endif
}

#if NeedFunctionPrototypes
void setNewVolume(unsigned int Volume)
#else
void setNewVolume(Volume)
	unsigned int Volume;
#endif
{
	/* Do nothing here as we don't have audio support */
}

#if NeedFunctionPrototypes
void audioDeviceEvents(void)
#else
void audioDeviceEvents()
#endif
{
    /* None to do */
}

#if NeedFunctionPrototypes
void playSoundFile(char *filename, int volume)
#else
void playSoundFile(filename, volume)
	char *filename;
	int volume;
#endif
{
	int err, cnt, ifd;
	char soundfile[1024];
	char *str;

	if (parentid) kill(parentid, 9);
	parentid = getpid();

	if (fork() != 0)
	{
		if ((str = getenv("XBOING_SOUND_DIR")) != NULL)
			sprintf(soundfile, "%s/%s.au", str, filename);
		else
			sprintf(soundfile, "%s/%s.au", SOUNDS_DIR, filename);

		/* Open the sound file for reading */
		if ((ifd = open(soundfile, O_RDONLY, 0)) < 0) 
		{
			/* Issue an error about not opening sound file */
			sprintf(errorString, "Unable to open sound file %s.", soundfile);
			WarningMessage(errorString);

			exit(0);
		}

		/* At this point, we're all ready to copy the data. */
		while ((cnt = read(ifd, (char *) buf, BUFFER_SIZE)) >= 0) 
		{
			/* If input EOF, write an eof marker */
			err = write(Audio_fd, (char *)buf, cnt);

			if (err != cnt) 
			{
				sprintf(errorString, "Problem while writing to audio device");
				WarningMessage(errorString);
				break;
			}    

			/* End of file? */
			if (cnt == 0) break;
		}

		if (cnt < 0) 
		{
			/* Some error - while reading - notify user */
			sprintf(errorString, "Problem while reading soundfile %s", 
				soundfile);
			WarningMessage(errorString);
		}
	 
		flushAudioDevice();

		/* Close the sound file */
		(void) close(ifd);

		exit(0);
	}
}

#if NeedFunctionPrototypes
void SetMaximumVolume(int Volume)
#else
void SetMaximumVolume(Volume)
    int Volume;
#endif
{
}

#if NeedFunctionPrototypes
int GetMaximumVolume(void)
#else
int GetMaximumVolume()
#endif
{
    return 0;
}
