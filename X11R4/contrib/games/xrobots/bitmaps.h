/*
 * bitmaps.h  --  xrobots
 * 
 * Copyright 1989 Brian Warkentine
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the author's name not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  The author makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 * 
 * THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL THE 
 * AUTHOR BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF 
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * The author's current employer has no connection with this software, as it
 * was written before being employed at Sun.  The following address is for 
 * contacting the author only and does not imply any responsibility on the 
 * behalf of Sun Microsystems.
 * 
 * Contact the author at:
 * 	Brian Warkentine  (brianw@Sun.COM)
 * 	Sun Microsystems
 * 	2550 Garcia Avenue
 * 	Mountain View, Ca 94043-1100
 *
 */

/*----------------------------------------------------------------------*/
/* playfield images */

#define Image_width 20
#define Image_height 20

#define Image_player_width 20
#define Image_player_height 20
static char Image_player_bits[] = {
   0xc0, 0x1f, 0x00, 0x30, 0x60, 0x00, 0x08, 0x80, 0x00, 0x04, 0x00, 0x01,
   0x42, 0x10, 0x02, 0xe2, 0x38, 0x02, 0x41, 0x10, 0x04, 0x01, 0x00, 0x04,
   0x01, 0x00, 0x04, 0x11, 0x40, 0x04, 0x19, 0xc0, 0x04, 0x1d, 0xc0, 0x05,
   0x31, 0x60, 0x04, 0xe2, 0x38, 0x02, 0xc2, 0x1f, 0x02, 0x84, 0x0f, 0x01,
   0x08, 0x80, 0x00, 0x30, 0x60, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00};

#define Image_player_dead_width 20
#define Image_player_dead_height 20
static char Image_player_dead_bits[] = {
   0xc0, 0x1f, 0x00, 0x30, 0x60, 0x00, 0x08, 0x80, 0x00, 0x04, 0x00, 0x01,
   0x12, 0x45, 0x02, 0xa2, 0x28, 0x02, 0x41, 0x10, 0x04, 0xa1, 0x28, 0x04,
   0x11, 0x45, 0x04, 0x01, 0x00, 0x04, 0x01, 0x00, 0x04, 0x01, 0x00, 0x04,
   0x01, 0x00, 0x04, 0x02, 0x00, 0x02, 0xf2, 0x7f, 0x02, 0x04, 0x00, 0x01,
   0x08, 0x80, 0x00, 0x30, 0x60, 0x00, 0xc0, 0x1f, 0x00, 0x00, 0x00, 0x00};

#define Image_robot_width 20
#define Image_robot_height 20
static char Image_robot_bits[] = {
   0x00, 0x00, 0x00, 0xe0, 0x3f, 0x00, 0x10, 0x40, 0x00, 0xe8, 0xff, 0x00,
   0x04, 0x38, 0x01, 0xfa, 0xff, 0x03, 0x42, 0x15, 0x02, 0xaa, 0xaa, 0x02,
   0x4a, 0x95, 0x02, 0x8a, 0x8a, 0x02, 0x1a, 0xc5, 0x02, 0x2a, 0xa2, 0x02,
   0x2a, 0xa0, 0x02, 0x2e, 0xa2, 0x03, 0x20, 0x22, 0x00, 0x20, 0x22, 0x00,
   0x20, 0x22, 0x00, 0x20, 0x22, 0x00, 0xe0, 0x3f, 0x00, 0x00, 0x00, 0x00};

#ifdef HEAP_STYLE_1
/* your choice of how you like your dead robots to look...*/

#define Image_heap_width 20
#define Image_heap_height 20
static char Image_heap_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xe0, 0xff, 0x01, 0x20, 0x00, 0x03, 0xa0, 0xaa, 0x06, 0xff, 0x57, 0x0d,
   0x01, 0xe8, 0x0a, 0x55, 0xf5, 0x0a, 0xa9, 0xfa, 0x0e, 0x55, 0xfd, 0x0a,
   0xbf, 0xfe, 0x0e, 0x55, 0xfd, 0x0a, 0xab, 0x7a, 0x0e, 0x55, 0x75, 0x0a,
   0xff, 0xea, 0x06, 0xe0, 0x5f, 0x03, 0xa0, 0xaa, 0x01, 0xe0, 0xff, 0x00};

#else

#define Image_heap_width 20
#define Image_heap_height 20
static char Image_heap_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x1e, 0x00,
   0x00, 0x35, 0x00, 0x80, 0x5b, 0x00, 0xc0, 0xae, 0x00, 0xe0, 0xf5, 0x00,
   0xb0, 0xaa, 0x01, 0xe8, 0x6f, 0x03, 0x7c, 0xfe, 0x03, 0x00, 0x00, 0x00};

#endif

/*----------------------------------------------------------------------*/
/* cursors */

#define cursor_width 16
#define cursor_height 16

#define down_width 16
#define down_height 16
static char down_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0xb0, 0x0d,
   0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00};

#define down_left_width 16
#define down_left_height 16
static char down_left_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x01, 0x80, 0x03, 0xc0, 0x01, 0xe6, 0x00, 0x76, 0x00, 0x3e, 0x00,
   0x1e, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00, 0x00};

#define down_right_width 16
#define down_right_height 16
static char down_right_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x00, 0xc0, 0x01, 0x80, 0x03, 0x00, 0x67, 0x00, 0x6e, 0x00, 0x7c,
   0x00, 0x78, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x00};

#define left_width 16
#define left_height 16
static char left_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x18, 0x00,
   0x0c, 0x00, 0xfe, 0x07, 0xfe, 0x07, 0x0c, 0x00, 0x18, 0x00, 0x10, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define right_width 16
#define right_height 16
static char right_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x18,
   0x00, 0x30, 0xe0, 0x7f, 0xe0, 0x7f, 0x00, 0x30, 0x00, 0x18, 0x00, 0x08,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define up_width 16
#define up_height 16
static char up_bits[] = {
   0x00, 0x00, 0x80, 0x01, 0xc0, 0x03, 0xe0, 0x07, 0xb0, 0x0d, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

#define up_right_width 16
#define up_right_height 16
static char up_right_bits[] = {
   0x00, 0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x78, 0x00, 0x7c, 0x00, 0x6e,
   0x00, 0x67, 0x80, 0x03, 0xc0, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


#define up_left_width 16
#define up_left_height 16
static char up_left_bits[] = {
   0x00, 0x00, 0x7e, 0x00, 0x7e, 0x00, 0x1e, 0x00, 0x3e, 0x00, 0x76, 0x00,
   0xe6, 0x00, 0xc0, 0x01, 0x80, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


#define stay_width 16
#define stay_height 16
static char stay_bits[] = {
   0x80, 0x00, 0x80, 0x00, 0x80, 0x00, 0xe0, 0x03, 0xc0, 0x01, 0x80, 0x10,
   0x08, 0x18, 0x18, 0xfc, 0x3f, 0x18, 0x18, 0x10, 0x08, 0x01, 0x80, 0x03,
   0xc0, 0x07, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01};


#define cant_go_width 16
#define cant_go_height 16
static char cant_go_bits[] = {
   0x00, 0x00, 0xc0, 0x07, 0x20, 0x08, 0x90, 0x13, 0x50, 0x12, 0x20, 0x12,
   0x00, 0x09, 0x80, 0x04, 0x80, 0x02, 0x80, 0x02, 0x00, 0x01, 0x00, 0x01,
   0x80, 0x02, 0x80, 0x02, 0x00, 0x01, 0x00, 0x00};


#define thumbs_down_width 16
#define thumbs_down_height 16
#define thumbs_down_x_hot 7
#define thumbs_down_y_hot 15
static char thumbs_down_bits[] = {
   0x00, 0x00, 0x00, 0x00, 0xe0, 0x3f, 0x1e, 0x40, 0x00, 0x78, 0x00, 0x80,
   0x00, 0x78, 0x00, 0x80, 0x00, 0x7c, 0x0e, 0x40, 0x30, 0x3e, 0x40, 0x03,
   0x40, 0x01, 0x40, 0x01, 0x40, 0x01, 0xc0, 0x00};
