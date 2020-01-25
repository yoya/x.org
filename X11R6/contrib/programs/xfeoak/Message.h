/* @(#)Message.h      6.1 94/06/01 20:04:28 FUJITSU LIMITED. */
/*

Copyright (c) 1991, 1992, 1993, 1994  FUJITSU LIMITED

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE FUJITSU LIMITED BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

Except as contained in this notice, the name of the FUJITSU LIMITED shall
not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization
from the FUJITSU LIMITED.

  Author: Takashi Fujiwara     FUJITSU LIMITED 
                               fujiwara@a80.tech.yk.fujitsu.co.jp
          Kiyoaki oya          FUJITSU LIMITED
	  Toshimitu Terazono   FUJITSU LIMITED

*/

/*
 *  xfeoak : Input Server (kana-kanji covertion IM Server)
 */

Message english_message[] = {
{  1, "Can't open display ! (display name: %s)"},
{  2, "Frontend already running."},
{  8, "Frontend terminated by SIGTERM."},
{  9, "Frontend terminated by server shutdown or KillClient."},
{ 10, "Bad color name  '%s', use default color."},
{ 11, "Bad color name, use default color."},

{ 20, "Can't open a file (%s), using %s instead."},
{ 21, "Can't open a file (%s), using built-in defaults instead."},
{ 22, "Errors found in fekeybind %s."},
{ 23, "Unknown keysym name (%s)."},
{ 25, "Unknown command argument (%s)."},

{ 30, "BadName (font dose not exist)\n (%s)"},
{ 32, "Undefined ascii/kana font ?\n"},
{ 34, "Undefined kanji font ?\n"},

{ 70, "Can't malloc : size = %d."},

#ifdef FUJITSU_SERVER_EXTENSION
{ 80,	"Can't used : Control Modifiers Extenstion."},
#endif /*FUJITSU_SERVER_EXTENSION*/

/* no. : 100 - 199 reserved for XIM Protocol */
/*     : 200 - 299 reserved for XIM Extenstion Protocol */
{100, "Undefined protocol ?"},
{101, "XIM_CREATE : Can't open."},
{102, "XIM_CREATE : Can't open ; too many user !"},
{103, "XIM_CREATE : BadStyle"},
{104, "XIM_CREATE : BadICValue"},
{111, "XIM_GET_IM_VALUES : Not connected."},
{112, "XIM_QUERY_EXTENSION : Not connected."},
{114, "XIM_CREATE_IC : Not connected."},
{120, "XIM_DESTROY_IC : Not connected."},
{121, "XIM_GET_IC_VALUES : Not connected."},
{122, "XIM_SET_IC_VALUES : Not connected."},
{123, "XIM_SET_IC_FOCUS : Not connected."},
{124, "XIM_UNSET_IC_FOCUS : Not connected."},
{125, "XIM_FORWARD_EVENT : Not connected."},
{126, "XIM_SYNC : Not connected."},
{127, "XIM_SYNC_REPLY : Not connected."},
{128, "XIM_RESET_IC : Not connected."},
{129, "XIM_TRIGGER_NOTIFY : Not connected."},
{130, "XIM_GET_IM_VALUES or XIM_GET_IC_VALUES : Invalid Attribute id = %d."},
{131, "XIM_GET_IM_VALUES or XIM_GET_IC_VALUES : Can not get value(id = %d)."},
{132, "XIM_CREATE_IC or XIM_SET_IC_VALUES : UnKnown Attribute id = %d."},
{133, "XIM_CREATE_IC or XIM_SET_IC_VALUES : Can not set value(id = %d)."},
{134, "XIM_CREATE_IC or XIM_SET_IC_VALUES : Invalid Attribute value(id = %d)."},
{135, "XIM_CREATE_IC : BadStyle = %x."},
{191, "Can not find Client(window = %x)."},

{200, "XIM_EXT_FORWARD_KEYEVENT : Not connected."},
{201, "XIM_EXT_MOVE : Not connected."},
{300, "not open client."}

};

int	max_message_no = sizeof(english_message) / sizeof(Message);
Message *message = english_message;
