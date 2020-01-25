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

/* SGI - plays SUN Audio format - original code from 
 *       Aage Nymark : Age.Nymark@termo.unit.no
 *
 *       Works on Indigo, Indy SGI boxes.
 */

/*
 *  Include file dependencies:
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/signal.h>
#include <sys/strops.h>


#include "include/error.h"
#include "include/audio.h"
#include <audio.h>            /* SGI audio interface lib. header */
#include "include/ulaw.h"

/*
 *  Internal macro definitions:
 */

#define LEFT	1
#define RIGHT	3

/*
 *  Internal type declarations:
 */

/*
 *  Internal variable declarations:
 */

static ALport	thePort = 0;
static ALconfig theConf = 0;
static FILE    *theFile = 0;
static float	MaxVolum = 100.0;
static char     errorString[255];

/*
 * default audio device configuration
 *
 */

static long defConf[] = 
{
    AL_OUTPUT_RATE, 		8000,
    AL_INPUT_RATE, 			8000, 
    AL_LEFT_SPEAKER_GAIN, 	128, 
    AL_RIGHT_SPEAKER_GAIN, 	128
};

/*
 * gain control 
 *
 */ 

static long gainConf[] = 
{ 
    AL_LEFT_SPEAKER_GAIN, 	128, 
    AL_RIGHT_SPEAKER_GAIN, 	128,  
};


/*
 * static interface functions
 */

#if NeedFunctionPrototypes
static void audioSetConfig(void)
#else
static void audioSetConfig()
#endif
{
    /* do nothing */
}

#if NeedFunctionPrototypes
static void audioSetGain(int volume)
#else
static void audioSetGain(volume)
	int volume;
#endif
{
	/*
	 * set Volume (0-100%)
	 */

    long vol;
    vol = (long) (MaxVolum * ((float) volume / 100.0));
    
    gainConf[LEFT]  = vol;
    gainConf[RIGHT] = vol;
    ALsetparams( AL_DEFAULT_DEVICE, gainConf, 4);
}


#if NeedFunctionPrototypes
static int audioOpenPort(void)
#else
static int audioOpenPort()
#endif
{
	/*
	 * open audio
	 */

    ALsetparams(AL_DEFAULT_DEVICE, defConf, 8);
    theConf = ALnewconfig();
    ALsetchannels(theConf, AL_MONO);
    ALsetwidth(theConf, AL_SAMPLE_16);
    
    if (theConf)
    {
		thePort = ALopenport ("XBoing sounds", "w", theConf);
		if (thePort)
	    	return True; 
    }

    return False;   
}

#if NeedFunctionPrototypes
static void audioClosePort(void)
#else
static void audioClosePort()
#endif
{
	/*
	 * close audio
	 */

    ALfreeconfig(theConf);
    ALcloseport(thePort);
    theConf = 0;
    thePort = 0;
}

#if NeedFunctionPrototypes
static int audioFlushPort(void)
#else
static int audioFlushPort()
#endif
{
	/*
	 * flush audio device please.
	 */

/* This doesn't work either! 
    if (ioctl(ALgetfd(thePort), I_FLUSH, FLUSHW) < 0)
    {
        sprintf(errorString, "Unable to flush audio device.");
        WarningMessage(errorString);
        return False;
    }

	return True;
*/

    int ok;

	/* Big hack but I don't know how to do it otherwise */
    audioClosePort();
    ok = audioOpenPort();

    if (ok)
		return True;

    return False;
}


#if NeedFunctionPrototypes
static void auFileReadHeader(FILE *fp, sndHeader *hdr)
#else
static void auFileReadHeader(fp, hdr)
	FILE *fp;
	sndHeader *hdr;
#endif
{
	/*
	 * just jump over header
	 */

    rewind(fp);
    fread(hdr, sizeof(sndHeader), 1, fp);
    fseek(fp, (long)(hdr->dataOffset), SEEK_SET);
}

#if NeedFunctionPrototypes
int SetUpAudioSystem(Display *display)
#else
int SetUpAudioSystem(display)
	Display *display;
#endif
{
    int ok = audioOpenPort();
    audioSetGain(60);
    return ok;
}

#if NeedFunctionPrototypes
void FreeAudioSystem(void)
#else
void FreeAudioSystem()
#endif
{
    while (ALgetfilled(thePort)) 
		sginap(1);

    audioClosePort();
}

#if NeedFunctionPrototypes
void setNewVolume(unsigned int Volume)
#else
void setNewVolume(Volume)
	unsigned int Volume;
#endif
{
    audioSetGain(Volume);
}

#if NeedFunctionPrototypes
void playSoundFile(char *filename, int volume)
#else
void playSoundFile(filename, volume)
	char *filename;
	int volume;
#endif
{
    char	  		soundfile[1024];
    unsigned char 	frameBuff[1024];
    short	  		linBuff[1024];
    char       		*str;
    int				cnt, i;
    sndHeader		myHdr;
    
	/* Flush the sound port for the new sound please */
    if (ALgetfilled(thePort))
    {
		if (!audioFlushPort())
		{
        	ErrorMessage("Cannot flush sound system.");
	    	return;   
		}
    }

    audioSetGain(volume);

    if ((str = getenv("XBOING_SOUND_DIR")) != NULL)
    	sprintf(soundfile, "%s/%s.au", str, filename);
	else 
        sprintf(soundfile, "%s/%s.au", SOUNDS_DIR, filename);

	/* Open the sound file for reading */
    if ((theFile = fopen(soundfile, "rb")) == NULL)
    {
      	sprintf(errorString, "Cannot open sound file %s.", filename);
       	ErrorMessage(errorString);

		perror(soundfile);
		return;
    }
	
	/* Read the au file header */
    auFileReadHeader(theFile, &myHdr);

	/* Read all the sound sample */
    while ((cnt = fread(frameBuff, 1, 1024, theFile)))
    {
		/* Now do the ulaw stuff to the raw data */
		for(i = 0; i < cnt; i++) 
		{
	    	linBuff[i] = UlToLin(frameBuff[i]);
		}

		/* Write the samples out to the SGI audio device */
		ALwritesamps(thePort, linBuff, (long)cnt);
    }

	/* Close this audio file */
    fclose(theFile);
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
void SetMaximumVolume(int Volume)
#else
void SetMaximumVolume(Volume)
    int Volume;
#endif
{
    MaxVolum = (float) Volume;
}

#if NeedFunctionPrototypes
int GetMaximumVolume(void)
#else
int GetMaximumVolume()
#endif
{
    return (int) MaxVolum;
}
