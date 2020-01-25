#include <copyright.h>

/* Copyright Massachusetts Institute of Technology 1985 */

/* dbm database, mapping color names (strings) to RGB values */
/* $Header: rgb.h,v 4.0 88/01/27 16:59:21 toddb Exp $ */

#define RGB_DB "/usr/lib/rgb"

typedef struct _rgb {unsigned short red, green, blue;} RGB;
