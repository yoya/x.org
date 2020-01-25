/*

Copyright (c) 1986, 1987 by Hewlett-Packard Company
Copyright (c) 1986, 1987 by the Massachusetts Institute of Technology

Permission to use, copy, modify, and distribute this
software and its documentation for any purpose and without
fee is hereby granted, provided that the above copyright
notice appear in all copies and that both that copyright
notice and this permission notice appear in supporting
documentation, and that the name of M.I.T. not be used in
advertising or publicity pertaining to distribution of the
software without specific, written prior permission.

HEWLETT-PACKARD MAKES NO WARRANTY OF ANY KIND WITH REGARD
TO THIS SOFWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
PURPOSE.  Hewlett-Packard shall not be liable for errors 
contained herein or direct, indirect, special, incidental or 
consequential damages in connection with the furnishing, 
performance, or use of this material.

This software is not subject to any license of the American
Telephone and Telegraph Company or of the Regents of the
University of California.

*/

/*
 * TTY Functions unique to HP. Augments definitions in
 * server/include/keysymdef.h
 */

#define XK_Reset                0xFF6C  /* HP -- The shift of Break */
#define XK_System               0xFF6D  /* HP */
#define XK_User                 0xFF6E  /* HP */
#define XK_ClearLine            0xFF6F  /* HP */
#define XK_InsertLine           0xFF70  /* HP */
#define XK_DeleteLine           0xFF71  /* HP */
#define XK_InsertChar           0xFF72  /* HP */
#define XK_DeleteChar           0xFF73  /* HP */
#define XK_BackTab              0xFF74  /* HP */
#define XK_KP_BackTab           0xFF75  /* HP */
