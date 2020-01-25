/*
 * Copyright 1993 by Digital Equipment Corporation, Maynard, Massachusetts.
 * 
 * Permission to use, copy, modify, distribute, and sell this software and its 
 * documentation for any purpose is hereby granted without fee, provided that 
 * the above copyright notice appear in all copies and that both that 
 * copyright notice and this permission notice appear in supporting 
 * documentation, and that the name of Digital not be used in advertising or 
 * publicity pertaining to distribution of the software without specific, 
 * written prior permission.  Digital makes no representations about the 
 * suitability of this software for any purpose.  It is provided "as is" 
 * without express or implied warranty.
 * 
 * DIGITAL DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING 
 * ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT SHALL 
 * DIGITAL BE LIABLE FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY 
 * DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN 
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF 
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "codec_tables.h"

/* Tables */
ATG atg[]={
	{-18.0, 0x37},
	{-16.0, 0x32},
	{-14.0, 0x31},
	{-12.0, 0x27},
	{-10.0, 0x22},
	{-8.0, 0x21},
	{-6.0, 0x20},
	{-4.0, 0x12},
	{-2.0, 0x11},
	{0.0, 0x10}
};
int atg_size = sizeof(atg)/sizeof(atg[0]);
GER ger[]={
	{-70.0, 0x08, 0x00},
	{-10.0, 0xaa,0xaa},
	{-9.5, 0xbb,0x9b},
	{-9.0, 0xac,0x79},
	{-8.5, 0x9a,0x09},
	{-8.0, 0x99,0x41},
	{-7.5, 0x99, 0x31},
	{-7.0, 0xde, 0x9c},
	{-6.5, 0xef, 0x9d},
	{-6.0, 0x9c, 0x74},
	{-5.5, 0x9d, 0x54},
	{-5.0, 0xae, 0x6a},
	{-4.5, 0xcd, 0xab},
	{-4.0, 0xdf, 0xab},
	{-3.5, 0x29, 0x74},
	{-3.0, 0xab, 0x64},
	{-2.5, 0xff, 0x6a},
	{-2.0, 0xbd, 0x2a},
	{-1.5, 0xef, 0xbe},
	{-1.0, 0xce, 0x5c},
	{-.5, 0xcd, 0x75},
	{0.0, 0x99, 0x00},
	{0.5, 0x4c, 0x55},
	{1.0, 0xdd, 0x43},
	{1.5, 0xdd, 0x33},
	{2.0, 0xef, 0x52},
	{2.5, 0x1b, 0x77},
	{3.0, 0x42, 0x55},
	{3.5, 0xdd, 0x41},
	{4.0, 0xdd, 0x31},
	{4.5, 0x1f, 0x44},
	{5.0, 0x1f, 0x43},
	{5.5, 0x1f, 0x33},
	{6.0, 0xdd, 0x40},
	{6.5, 0xdd, 0x11},
	{7.0, 0x0f, 0x44},
	{7.5, 0x1f, 0x41},
	{8.0, 0x1f, 0x31},
	{8.5, 0x20, 0x55},
	{9.0, 0xdd, 0x10},
	{9.5, 0x11, 0x42},
	{10.0, 0x0f, 0x41},
	{10.5, 0x1f, 0x11},
	{11.0, 0x0b, 0x60},
	{11.5, 0xdd, 0x00},
	{12.0, 0x10, 0x42},
	{12.5, 0x0f, 0x40},
	{13.0, 0x0f, 0x11},
	{13.4, 0x10, 0x22},
	{14.0, 0x00, 0x72},
	{14.5, 0x00, 0x42},
	{15.0, 0x10, 0x21},
	{15.5, 0x0f, 0x10},
	{15.9, 0x00, 0x22},
	{16.6, 0x10, 0x11},
	{16.9, 0x0b, 0x00},
	{17.5, 0x00, 0x21},
	{18.0, 0x0f, 0x00}
};
int ger_size = sizeof(ger)/sizeof(ger[0]);

GR gr[]={
    {-70.0, 0x08, 0x90},
    {-12.0, 0xf9, 0x91},
    {-11.5, 0xc5, 0x91},
    {-11.0, 0xb6, 0x91},
    {-10.5, 0x12, 0x92},
    {-10.0, 0xa4, 0x91},
    {-9.5, 0x22, 0x92},
    {-9.0, 0x32, 0x92},
    {-8.5, 0xfb, 0x92},
    {-8.0, 0xaa, 0x92},
    {-7.5, 0x27, 0x93},
    {-7.0, 0xb3, 0x93},
    {-6.5, 0xb3, 0x94},
    {-6.0, 0x91, 0x9f},
    {-5.5, 0xea, 0x9c},
    {-5.0, 0xf9, 0x9b},
    {-4.5, 0xac, 0x9a},
    {-4.0, 0x4a, 0x9a},
    {-3.5, 0x22, 0xa2},
    {-3.0, 0xa2, 0xa2},
    {-2.5, 0x8d, 0xa6},
    {-2.0, 0xa3, 0xaa},
    {-1.5, 0x42, 0xb2},
    {-1.0, 0x52, 0xbb},
    {-0.5, 0xb2, 0xcb},
    {-0.0, 0x08, 0x08},
};
int gr_size = sizeof(gr)/sizeof(gr[0]);
GX gx[]={
	{0.0, 0x08, 0x08},
	{0.5, 0xb2, 0x4c},
	{1.0, 0xac, 0x3d},
	{1.5, 0xe5, 0x2a},
	{2.0, 0x33, 0x25},
	{2.5, 0x22, 0x22},
	{3.0, 0x22, 0x21},
	{3.5, 0xd3, 0x1f},
	{4.0, 0xa2, 0x12},
	{4.5, 0x1b, 0x12},
	{5.0, 0x3b, 0x11},
	{5.5, 0xc3, 0x0b},
	{6.0, 0xf2, 0x10},
	{6.5, 0xba, 0x03},
	{7.0, 0xca, 0x02},
	{7.5, 0x1d, 0x02},
	{8.0, 0x5a, 0x01},
	{8.5,  0x22, 0x01},
	{9.0, 0x12, 0x00},
	{9.5, 0xec, 0x00},
	{10.0, 0x32, 0x00},
	{10.5, 0x21, 0x00},
	{11.0, 0x13, 0x00},
	{11.5, 0x11, 0x00},
	{12.0, 0x0e, 0x00}
};
int gx_size = sizeof(gx)/sizeof(gx[0]);
STG stg[]={
    {-70.0, 0x08, 0x90},
    {-18.0, 0x7c, 0x8b},
    {-17.5, 0x44, 0x8b},
    {-17.0, 0x35, 0x8b},
    {-16.5, 0x2a, 0x8b},
    {-16.0, 0x24, 0x8b},
    {-15.5, 0x22, 0x8b},
    {-15.0, 0x23, 0x91},
    {-14.5, 0x2e, 0x91},
    {-14.0, 0x2a, 0x91},
    {-13.5, 0x32, 0x91},
    {-13.0, 0x3b, 0x91},
    {-12.5, 0x4b, 0x91},
    {-12.0, 0xf9, 0x91},
    {-11.5, 0xc5, 0x91},
    {-11.0, 0xb6, 0x91},
    {-10.5, 0x12, 0x92},
    {-10.0, 0xa4, 0x91},
    {-9.5, 0x22, 0x92},
    {-9.0, 0x32, 0x92},
    {-8.5, 0xfb, 0x92},
    {-8.0, 0xaa, 0x92},
    {-7.5, 0x27, 0x93},
    {-7.0, 0xb3, 0x93},
    {-6.5, 0xb3, 0x94},
    {-6.0, 0x91, 0x9f},
    {-5.5, 0xea, 0x9c},
    {-5.0, 0xf9, 0x9b},
    {-4.5, 0xac, 0x9a},
    {-4.0, 0x4a, 0x9a},
    {-3.5, 0x22, 0xa2},
    {-3.0, 0xa2, 0xa2},
    {-2.5, 0x8d, 0xa6},
    {-2.0, 0xa3, 0xaa},
    {-1.5, 0x42, 0xb2},
    {-1.0, 0x52, 0xbb},
    {-0.5, 0xb2, 0xcb},
    {0.0, 0x08, 0x08},
};
int stg_size = sizeof(stg)/sizeof(stg[0]);
STRF strf[]={
	{ 188.2,0xff},
	{ 189.0,0xfe},
	{ 189.7,0xfc},
	{ 190.5, 0xf9},
	{ 191.2, 0xf2},
	{ 192.0, 0xe4},
	{ 192.8, 0xc8},
	{ 193.6, 0x90},
	{ 194.3, 0x21},
	{ 195.1, 0x42},
	{ 195.9, 0x85},
	{ 196.7, 0x0a},
	{ 197.5, 0x14},
	{ 198.4, 0x29},
	{ 199.2, 0x53},
	{ 200.0, 0xa7},
	{ 200.8, 0x4f},
	{ 201.7, 0x9f},
	{ 202.5, 0x3e},
	{ 203.4, 0x7d},
	{ 204.3, 0xfa},
	{ 205.1, 0xf5},
	{ 206.0, 0xea},
	{ 206.9, 0xd5},
	{ 207.8, 0xaa},
	{ 208.7, 0x55},
	{ 209.6, 0xab},
	{ 210.5, 0x57},
	{ 211.5, 0xae},
	{ 212.4, 0x5c},
	{ 213.3, 0xb8},
	{ 214.3, 0x70},
	{ 215.3, 0xe0},
	{ 216.2, 0xc1},
	{ 217.2, 0x83},
	{ 218.2, 0x06},
	{ 219.2, 0x0c},
	{ 220.2, 0x18},
	{ 221.2, 0x31},
	{ 222.2, 0x62},
	{ 223.3, 0xc5},
	{ 224.3, 0x8a},
	{ 225.4, 0x15},
	{ 226.4, 0x2b},
	{ 227.5, 0x56},
	{ 228.6, 0xac},
	{ 229.7, 0x59},
	{ 230.8, 0xb3},
	{ 231.9, 0x66},
	{ 233.0, 0xcc},
	{ 234.2, 0x99},
	{ 235.3, 0x32},
	{ 236.5, 0x65},
	{ 237.6, 0xcb},
	{ 238.8, 0x97},
	{ 240.0, 0x2f},
	{ 241.2, 0x5f},
	{ 242.4, 0xbf},
	{ 243.7, 0x7e},
	{ 244.9, 0xfd},
	{ 246.2, 0xfb},
	{ 247.4, 0xf7},
	{ 248.7, 0xef},
	{ 250.0, 0xde},
	{ 251.3, 0xbc},
	{ 252.6, 0x79},
	{ 254.0, 0xf3},
	{ 255.3, 0xe6},
	{ 256.7, 0xcd},
	{ 258.1, 0x9b},
	{ 259.5, 0x37},
	{ 260.9, 0x6e},
	{ 262.3, 0xdd},
	{ 263.7, 0xbb},
	{ 265.2, 0x77},
	{ 266.7, 0xee},
	{ 268.2, 0xdc},
	{ 269.7, 0xb9},
	{ 271.2, 0x72},
	{ 272.7, 0xe5},
	{ 274.3, 0xca},
	{ 275.9, 0x95},
	{ 277.7, 0x2a},
	{ 279.1, 0x54},
	{ 280.7, 0xa9},
	{ 282.4, 0x52},
	{ 284.0, 0xa5},
	{ 285.7, 0x4a},
	{ 287.4, 0x94},
	{ 289.2, 0x28},
	{ 290.9, 0x51},
	{ 292.7, 0xa2},
	{ 294.5, 0x44},
	{ 296.3, 0x89},
	{ 298.1, 0x12},
	{ 300.0, 0x25},
	{ 301.9, 0x4b},
	{ 303.8, 0x96},
	{ 305.7, 0x2d},
	{ 307.7, 0x5a},
	{ 309.7, 0xb4},
	{ 311.7, 0x68},
	{ 313.7, 0xd1},
	{ 315.8, 0xa3},
	{ 317.9, 0x46},
	{ 320.0, 0x8c},
	{ 322.2, 0x19},
	{ 324.3, 0x33},
	{ 326.5, 0x67},
	{ 328.8, 0xce},
	{ 331.0, 0x9c},
	{ 333.3, 0x39},
	{ 335.7, 0x73},
	{ 338.0, 0xe7},
	{ 340.4, 0xcf},
	{ 342.9, 0x9e},
	{ 345.3, 0x3c},
	{ 347.8, 0x78},
	{ 350.4, 0xf1},
	{ 352.9, 0xe3},
	{ 355.6, 0xc6},
	{ 358.2, 0x8d},
	{ 360.9, 0x1b},
	{ 363.6, 0x36},
	{ 366.4, 0x6c},
	{ 369.2, 0xd8},
	{ 372.1, 0xb0},
	{ 375.0, 0x61},
	{ 378.0, 0xc2},
	{ 381.0, 0x84},
	{ 384.0, 0x08},
	{ 387.1, 0x11},
	{ 390.2, 0x22},
	{ 393.4, 0x45},
	{ 396.7, 0x8b},
	{ 400.0, 0x17},
	{ 403.4, 0x2e},
	{ 406.8, 0x5d},
	{ 410.3, 0xba},
	{ 413.8, 0x75},
	{ 417.4, 0xeb},
	{ 421.1, 0xd7},
	{ 424.8, 0xaf},
	{ 428.6, 0x5e},
	{ 432.4, 0xbd},
	{ 436.4, 0x7b},
	{ 440.4, 0xf6},
	{ 444.4, 0xed},
	{ 448.6, 0xdb},
	{ 452.8, 0xb7},
	{ 457.1, 0x6f},
	{ 461.5, 0xdf},
	{ 466.0, 0xbe},
	{ 470.6, 0x7c},
	{ 475.3, 0xf8},
	{ 480.0, 0xf0},
	{ 484.9, 0xe1},
	{ 489.8, 0xc3},
	{ 494.9, 0x86},
	{ 500.0, 0x0d},
	{ 505.3, 0x1a},
	{ 510.6, 0x34},
	{ 516.1, 0x69},
	{ 521.7, 0xd3},
	{ 527.5, 0xa6},
	{ 533.3, 0x4d},
	{ 539.3, 0x9a},
	{ 545.5, 0x35},
	{ 551.7, 0x6b},
	{ 558.1, 0xd6},
	{ 564.7, 0xad},
	{ 571.4, 0x5b},
	{ 578.3, 0xb6},
	{ 585.4, 0x6d},
	{ 592.6, 0xda},
	{ 600.0, 0xb5},
	{ 607.6, 0x6a},
	{ 615.4, 0xd4},
	{ 623.4, 0xa8},
	{ 631.6, 0x50},
	{ 640.0, 0xa0},
	{ 648.7, 0x41},
	{ 657.5, 0x82},
	{ 666.7, 0x04},
	{ 676.1, 0x09},
	{ 685.7, 0x13},
	{ 695.7, 0x27},
	{ 705.9, 0x4e},
	{ 716.4, 0x9d},
	{ 727.3, 0x3b},
	{ 738.5, 0x76},
	{ 750.0, 0xec},
	{ 761.9, 0xd9},
	{ 774.2, 0xb2},
	{ 786.9, 0x64},
	{ 800.0, 0xc9},
	{ 813.6, 0x92},
	{ 827.6, 0x24},
	{ 842.1, 0x49},
	{ 857.1, 0x93},
	{ 872.7, 0x26},
	{ 888.9, 0x4c},
	{ 905.7, 0x98},
	{ 923.1, 0x30},
	{ 941.2, 0x60},
	{ 960.0, 0xc0},
	{ 979.6, 0x81},
	{ 1000.0, 0x03},
	{ 1021.3, 0x07},
	{ 1043.5, 0x0e},
	{ 1066.7, 0x1d},
	{ 1090.9, 0x3a},
	{ 1116.3, 0x74},
	{ 1142.9, 0xe9},
	{ 1170.7, 0xd2},
	{ 1200.0, 0xa4},
	{ 1230.8, 0x48},
	{ 1263.2, 0x91},
	{ 1297.3, 0x23},
	{ 1333.3, 0x47},
	{ 1371.4, 0x8e},
	{ 1411.8, 0x1c},
	{ 1454.6, 0x38},
	{ 1500.0, 0x71},
	{ 1548.4, 0xe2},
	{ 1600.0, 0xc4},
	{ 1655.2, 0x88},
	{ 1714.3, 0x10},
	{ 1777.8, 0x20},
	{ 1846.2, 0x40},
	{ 1920.0, 0x80},
	{ 2000.0, 0x01},
	{ 2087.0, 0x02},
	{ 2181.8, 0x05},
	{ 2285.7, 0x0b},
	{ 2400.0, 0x16},
	{ 2526.3, 0x2c},
	{ 2666.7, 0x58},
	{ 2823.3, 0xb1},
	{ 3000.0, 0x63},
	{ 3200.0, 0xc7},
	{ 3428.6, 0x8f},
	{ 3692.3, 0x1e},
	{ 4000.0, 0x3d},
	{ 4363.6, 0x7a},
	{ 4800.0, 0xf4},
	{ 5333.3, 0xe8},
	{ 6000.0, 0xd0},
	{ 6857.1, 0xa1},
	{ 8000.0, 0x43},
	{ 9600.0, 0x87},
	{ 12000.0, 0x0f}
};
int strf_size = sizeof(strf)/sizeof(strf[0]);
STRG strg[]={
	{-70.0,0x00},
	{-27.0,0x60},
	{-24.0,0x70},
	{-21.0,0x80},
	{-18.0,0x90},
	{-15.0,0xa0},
	{-12.0,0xb0},
	{-9.0,0xc0},
	{-6.0,0xd0},
	{-3.0,0xe0},
	{ 0.0,0xf0},
};
int strg_size = sizeof(strg)/sizeof(strg[0]);
