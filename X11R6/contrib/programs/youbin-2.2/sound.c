/*
 * Program:     $RCSfile: sound.c,v $  $Revision: 4.1 $
 *
 * Author:      H.Ariga     ariga@agusa.nuie.nagoya-u.ac.jp
 *              S.Yamamoto  yamamoto@nuie.nagoya-u.ac.jp
 *
 * Date:        1993/07/24
 * Modified:    $Date: 1994/05/31 08:44:02 $
 *
 * Copyright:   H.Ariga and S.Yamamoto  1993 - 94
 *
 * The X Consortium, and any party obtaining a copy of these files from
 * the X Consortium, directly or indirectly, is granted, free of charge,
 * a full and unrestricted irrevocable, world-wide, paid up, royalty-free,
 * nonexclusive right and license to deal in this software and documentation
 * files (the "Software"), including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons who receive copies from any such
 * party to do so. This license includes without limitation a license to do
 * the foregoing actions under any patents of the party supplying this
 * software to the X Consortium.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#define AUDIO_DEVICE    "/dev/audio"

#ifndef R_OK
#define R_OK            4                   /* Test for read permission */
#endif /* R_OK */

int
sound(sound_file_name)
char    *sound_file_name;
{
    struct stat     sbuf;
    FILE            *fin, *fout;
    int             c;
    
    if (!sound_file_name || stat(sound_file_name, &sbuf)) {
        return (1);
    } else if ((sbuf.st_mode & S_IFMT) != S_IFREG) {
        return (1);
    } else if (!(sbuf.st_mode & S_IREAD)) {
        return (1);
    } else if (stat(AUDIO_DEVICE, &sbuf)) {
        return (1);
    } else if ((fin = fopen(sound_file_name, "r")) == NULL) {
        return (1);
    }
    if ((fout = fopen(AUDIO_DEVICE, "w")) == NULL) {
        fclose(fin);
        return (1);
    }
    
    while ((c = fgetc(fin)) != EOF) {
        fputc(c, fout);
    }
    fclose(fout);
    fclose(fin);
    return (0);
}

