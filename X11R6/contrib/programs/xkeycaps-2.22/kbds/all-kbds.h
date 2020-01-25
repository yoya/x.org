/* xkeycaps, Copyright (c) 1991, 1992, 1993, 1994
 * by Jamie Zawinski <jwz@lucid.com>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation.  No representations are made about the suitability of this
 * software for any purpose.  It is provided "as is" without express or 
 * implied warranty.
 */

#ifdef _AIX
/* Prevent AIX def of XK_Reset which conflicts with the HP definition. */
# define STD_KEYSYMS
#endif

#ifndef XK_KATAKANA
/* We need the Japanese keysym names for the Xmint keyboard. */
# define XK_KATAKANA
#endif

#include <X11/keysym.h>

#include "sun2-geom.h"
#include "sun2-caps.h"
#include "sun2-codes.h"
#include "sun2-xc-map.h"

#include "sun3-geom.h"
#include "sun3-caps.h"
#include "sun3-codes.h"
#include "sun3-xc-map.h"

#include "sun4-geom.h"
#include "sun4-caps.h"
#include "sun4-codes.h"
#include "sun4-xc-map.h"
#include "sun4-ow-map.h"
#include "sun4-r6-map.h"

#include "sun4sw-geom.h"
#include "sun4sw-caps.h"
#include "sun4sw-codes.h"
#include "sun4sw-map.h"

#include "sun5-geom.h"
#include "sun5-caps.h"
#include "sun5-codes.h"
#include "sun5-xc-map.h"
#include "sun5-ow-map.h"
#include "sun5-r6-map.h"

#include "sun5swfr-geom.h"
#include "sun5swfr-caps.h"
#include "sun5swfr-codes.h"
#include "sun5swfr-ow-map.h"

#include "sun5sw-geom.h"
#include "sun5sw-caps.h"
#include "sun5sw-codes.h"
#include "sun5sw-xc-map.h"
#include "sun5sw-ow-map.h"

#include "sun5pc-caps.h"
#include "sun5pc-codes.h"
#include "sun5pc-xc-map.h"
#include "sun5pc-xc2-map.h"
#include "sun5pc-ow-map.h"
#include "sun5pc-owde-map.h"

#include "sun101a-codes.h"
#include "sun101a-xc-map.h"

#include "ncd97-geom.h"
#include "ncd97-caps.h"
#include "ncd97-codes.h"
#include "ncd97-map.h"

#include "ncd101-geom.h"
#include "ncd101-caps.h"
#include "ncd101-codes.h"
#include "ncd101-map.h"

#include "ncd102-caps.h"
#include "ncd102-codes.h"
#include "ncd102-map.h"

#include "ncd102f-caps.h"
#include "ncd102f-codes.h"
#include "ncd102f-map.h"

#include "ncd102n-caps.h"
#include "ncd102n-codes.h"
#include "ncd102n-map.h"

#include "ncd102sf-caps.h"
#include "ncd102sf-codes.h"
#include "ncd102sf-map.h"

#include "ncd108-geom.h"
#include "ncd108-caps.h"
#include "ncd108-codes.h"
#include "ncd108-map.h"

#include "ncd220-caps.h"
#include "ncd220-codes.h"
#include "ncd220-map.h"

#include "ncdpc-caps.h"
#include "ncdpc-codes.h"
#include "ncdpc-map.h"

#include "pc101-geom.h"
#include "pc101-caps.h"

#include "pc102-geom.h"

#include "sgi-caps.h"
#include "sgi-codes.h"
#include "sgi-r5-map.h"
#include "sgi-r4-map.h"

#include "nws1250-geom.h"
#include "nws1250-caps.h"
#include "nws1250-codes.h"
#include "nws1250-map.h"

#include "nws3410-geom.h"
#include "nws3410-caps.h"
#include "nws3410-codes.h"
#include "nws3410-map.h"

#include "mac-geom.h"
#include "mac-caps.h"
#include "mac-codes.h"
#include "mac-map.h"

#include "ga5k-caps.h"
#include "ga5k-codes.h"
#include "ga5k-map.h"

#include "lk201-geom.h"
#include "lk201-caps.h"
#include "lk201-codes.h"
#include "lk201-map.h"

#include "lk401-caps.h"
#include "lk401-codes.h"
#include "lk401-map.h"

#include "lk421-geom.h"
#include "lk421-caps.h"
#include "lk421-codes.h"

#include "rt-caps.h"
#include "rt-codes.h"
#include "rt-map.h"

#include "rtsw-caps.h"
#include "rtsw-codes.h"
#include "rtsw-map.h"
#include "rtsw8-map.h"

#include "rs6k-caps.h"
#include "rs6k-codes.h"
#include "rs6k-aix-map.h"
#include "rs6k-xc-map.h"

#include "rs6kde-caps.h"
#include "rs6kde-codes.h"
#include "rs6kde-aix-map.h"

#include "hpitf-geom.h"
#include "hpitf-caps.h"
#include "hpitf-codes.h"
#include "hpitf-map.h"

#include "hp700rx-codes.h"
#include "hp700rx-map.h"

#include "hppc-codes.h"
#include "hppc-map.h"

#include "hppcde-codes.h"
#include "hppcde-map.h"

#include "sco110-codes.h"
#include "sco110-map.h"

#include "dell-codes.h"
#include "dell-map.h"

#include "pc101-2-geom.h"
#include "pc101-2-caps.h"
#include "linux101-2-codes.h"
#include "linux101-2-map.h"

#include "pc102-caps.h"
#include "linux102-codes.h"
#include "linux102-map.h"

#include "pc102de-caps.h"
#include "linux102de-map.h"

#include "pc102-2-geom.h"
#include "pc102-2de-caps.h"
#include "linux102-2-codes.h"
#include "linux102-2de-map.h"

#include "pc102fr-caps.h"
#include "linux102fr-map.h"

#include "tt-geom.h"
#include "tt-caps.h"
#include "tt-codes.h"
#include "tt-map.h"

#include "tek101-geom.h"
#include "tek101-caps.h"
#include "tek101-codes.h"
#include "tek101-map.h"
#include "tek101-4-map.h"

#include "tek200-geom.h"
#include "tek200-caps.h"
#include "tek200-codes.h"

#include "tek200u-da-map.h"
#include "tek200u-de-map.h"
#include "tek200u-fi-map.h"
#include "tek200u-fr-map.h"
#include "tek200u-it-map.h"
#include "tek200u-ka-map.h"
#include "tek200u-no-map.h"
#include "tek200u-sp-map.h"
#include "tek200u-sw-map.h"
#include "tek200u-swde-map.h"
#include "tek200u-uk-map.h"
#include "tek200u-us-map.h"
#include "tek200v-us-map.h"
#include "tek200xesc-us-map.h"
#include "tek200xf11-us-map.h"

#include "labtam-caps.h"
#include "labtam-codes.h"
#include "labtam-map.h"

#include "NeXT-geom.h"
#include "NeXT-caps.h"
#include "NeXT-codes.h"
#include "NeXT-map.h"

#include "mag3k-caps.h"
#include "mag3k-codes.h"
#include "mag3k-map.h"

#include "magat-geom.h"
#include "magat-caps.h"
#include "magat-codes.h"
#include "magat-map.h"

#include "rs2030-caps.h"
#include "rs2030-codes.h"
#include "rs2030-map.h"

#include "xoft2-codes.h"
#include "xoft1-map.h"
#include "xoft2-map.h"

#include "hclpc-geom.h"
#include "hclpc-caps.h"
#include "hclpc-codes.h"
#include "hclpc-map.h"

#ifdef XK_kana_NU	/* just in case some losing vendors don't ship it... */
#include "xmint-geom.h"
#include "xmint-caps.h"
#include "xmint-codes.h"
#include "xmint-map.h"
#endif

#include "aviion300-codes.h"
#include "aviion300-map.h"

#include "explorer-geom.h"
#include "explorer-caps.h"
#include "explorer-codes.h"
#include "explorer-map.h"

#undef countof
#define countof(x) (sizeof((x)) / sizeof(*(x)))

#define KBD(short_name, long_name, vendor_id, geom, caps, codes, map)	\
	{ (short_name), (long_name), (vendor_id),			\
	  (geom), (caps), (codes), (map),				\
	  countof((caps)), countof((codes)), countof((map))		\
	}

struct keyboard_instance all_kbds [] = 
{
  KBD ("Sun2", "Sun type2 (X Consortium layout)", 0,
       &sun2_geom, sun2_caps, sun2_codes, sun2_xc_map),

  KBD ("Sun3", "Sun type3 (X Consortium layout)", 0,
       &sun3_geom, sun3_caps, sun3_codes, sun3_xc_map),

  KBD ("Sun4", "Sun type4 (X Consortium R4/R5 layout)", 0,
       &sun4_geom, sun4_caps, sun4_codes, sun4_xc_map),

  KBD ("Sun4r6", "Sun type4 (X Consortium R6 layout)", 0,
       &sun4_geom, sun4_caps, sun4_codes, sun4_r6_map),

  KBD ("Sun4ow", "Sun type4 (OpenWindows layout)",
       "X11/NeWS - Sun Microsystems Inc.",
       &sun4_geom, sun4_caps, sun4_codes, sun4_ow_map),

  KBD ("Sun4SWr6", "Sun type4 (Swedish R6 layout)", 0,
       &sun4sw_geom, sun4sw_caps, sun4sw_codes, sun4sw_map),

  KBD ("Sun5", "Sun type5 (X Consortium R4/R5 layout)", 0,
       &sun5_geom, sun5_caps, sun5_codes, sun5_xc_map),

  KBD ("Sun5r6", "Sun type5 (X Consortium R6 layout)", 0,
       &sun5_geom, sun5_caps, sun5_codes, sun5_r6_map),

  KBD ("Sun5OW", "Sun type5 (OpenWindows layout)", 0,
       &sun5_geom, sun5_caps, sun5_codes, sun5_ow_map),

  KBD ("Sun5SWFROW", "Sun type5 (Swiss-French OpenWindows layout)", 0,
       &sun5swfr_geom, sun5swfr_caps, sun5swfr_codes, sun5swfr_ow_map),
  
  KBD ("Sun5SW", "Sun type5 (Swedish X Consortium layout + patch)", 0,
       &sun5sw_geom, sun5sw_caps, sun5sw_codes, sun5sw_xc_map),

  KBD ("Sun5SWOW", "Sun type5 (Swedish OpenWindows layout)", 0,
       &sun5sw_geom, sun5sw_caps, sun5sw_codes, sun5sw_ow_map),

  KBD ("Sun5PC", "Sun type5/PC (X Consortium layout)", 0,
       &sun5pc_geom, sun5pc_caps, sun5pc_codes, sun5pc_xc_map),

  KBD ("Sun5PC2", "Sun type5/PC (X Consortium layout + patch)", 0,
       &sun5pc_geom, sun5pc_caps, sun5pc_codes, sun5pc_xc_patched_map),

  KBD ("Sun5PCOW", "Sun type5/PC (OpenWindows layout)", 0,
       &sun5pc_geom, sun5pc_caps, sun5pc_codes, sun5pc_ow_map),

  KBD ("Sun5PCDEOW", "Sun type5/PC (German OpenWindows layout)", 0,
       &sun5pc_geom, sun5pc_caps, sun5pc_codes, sun5pc_owde_map),

  KBD ("Sun101A", "Sun 101A (X Consortium layout)", 0,
       &pc101_geom, pc101_caps, sun101a_codes, sun101a_xc_map),

  KBD ("NCD97", "Network Computing Devices N97",
       "DECWINDOWS (Compatibility String) Network Computing Devices Inc.",
       &ncd97_geom, ncd97_caps, ncd97_codes, ncd97_map),

  KBD ("NCD101", "Network Computing Devices N101", 0,
       &ncd101_geom, ncd101_caps, ncd101_codes, ncd101_map),

  KBD ("NCD102", "Network Computing Devices N102 (US layout)", 0,
       &pc102_geom, ncd102_caps, ncd102_codes, ncd102_map),

  KBD ("NCD102F", "Network Computing Devices N102 (French layout)", 0,
       &pc102_geom, ncd102f_caps, ncd102f_codes, ncd102f_map),

  KBD ("NCD102N", "Network Computing Devices N102 (Norwegian layout)", 0,
       &pc102_geom, ncd102n_caps, ncd102n_codes, ncd102n_map),

  KBD ("NCD102SF","Network Computing Devices N102 (Swedish/Finnish layout)", 0,
       &pc102_geom, ncd102sf_caps, ncd102sf_codes, ncd102sf_map),

  KBD ("NCD108", "Network Computing Devices N108", 0,
       &ncd108_geom, ncd108_caps, ncd108_codes, ncd108_map),

  KBD ("NCD220", "Network Computing Devices vt220", 0,
       &ncd220_geom, ncd220_caps, ncd220_codes, ncd220_map),

  KBD ("NCDPC", "Network Computing Devices PC-Xview",
       "PC-Xview for Windows, by Network Computing Devices, Inc.",
       &tek101_geom, ncdpc_caps, ncdpc_codes, ncdpc_map),

  KBD ("SGIR4", "Silicon Graphics (X11R4 layout)", 0,
       &pc101_geom, sgi_caps, sgi_codes, sgi_r4_map),

  KBD ("SGI", "Silicon Graphics (X11R5 layout)", "Silicon Graphics",
       &pc101_geom, sgi_caps, sgi_codes, sgi_r5_map),

  KBD ("NWS1250", "Sony Net Work Station 1250", 0,
       &nws1250_geom, nws1250_caps, nws1250_codes, nws1250_map),

  KBD ("NWS3410", "Sony Net Work Station 3410", "Sony Corporation",
       &nws3410_geom, nws3410_caps, nws3410_codes, nws3410_map),

  KBD ("Mac", "Apple Extended II", "MacX",
       &mac_geom, mac_caps, mac_codes, mac_map),

  KBD ("GA5k", "Concurrent Computer Corp GA5000", "CONCURRENT-ga5000",
       &pc101_geom, ga5k_caps, ga5k_codes, ga5k_map),

  KBD ("LK201", "Digital Equipment Corporation LK201",
       "Digital Equipment Corporation",
       &lk201_geom, lk201_caps, lk201_codes, lk201_map),

  KBD ("LK401", "Digital Equipment Corporation LK401",
       "DECWINDOWS DigitalEquipmentCorporation",
       &lk401_geom, lk401_caps, lk401_codes, lk401_map),

  KBD ("LK421", "Digital Equipment Corporation LK421", 0,
       &lk421_geom, lk421_caps, lk421_codes, lk401_map),

  KBD ("RT", "IBM PC/RT", 0,
       &pc101_geom, rt_caps, rt_codes, rt_map),

  KBD ("RTSW", "IBM PC/RT 102 (Swedish 7-bit layout)", 0,
       &pc102_geom, rtsw_caps, rtsw_codes, rtsw_map),

  KBD ("RTSW8", "IBM PC/RT 102 (Swedish 8-bit layout)", 0,
       &pc102_geom, rtsw_caps, rtsw_codes, rtsw8_map),

  KBD ("RS6k", "IBM RS/6000 (AIXWindows R4 layout)",
       "International Business Machines",
       &pc101_geom, rs6k_caps, rs6k_codes, rs6k_aix_map),

  KBD ("RS6kDE", "IBM RS/6000 (AIXWindows German R4 layout)", 0,
       &pc102_geom, rs6kde_caps, rs6kde_codes, rs6kde_aix_map),

  KBD ("RS6k-XC", "IBM RS/6000 (X Consortium R5 layout)", 0,
       &pc101_geom, rs6k_caps, rs6k_codes, rs6k_xc_map),

  KBD ("HP-ITF", "Hewlett-Packard 300/400/700 Series",
       "Hewlett-Packard Company",
       &hpitf_geom, hpitf_caps, hpitf_codes, hpitf_map),

  KBD ("HP700RX", "Hewlett Packard 700/RX X Terminal", 0,
       &pc101_geom, pc101_caps, hp700rx_codes, hp700rx_map),

  KBD ("HPPC",    "PC 101 key keyboard #1 (Hewlett-Packard PC US layout)", 0,
       &pc101_geom, pc101_caps, hppc_codes, hppc_map),

  KBD ("SCO110",  "PC 101 key keyboard #1 (Santa Cruz Operation US layout)",
       "The Santa Cruz Operation",
       &pc101_geom, pc101_caps, sco110_codes, sco110_map),

  KBD ("DELL",    "PC 101 key keyboard #1 (DELL PC US layout)", 0,
       &pc101_geom, pc101_caps, dell_codes, dell_map),

  /* The order of IBM130 and AGE101 is important because of their
     vendor ID strings - see comments in "xoft*.h" to understand... */
  KBD ("IBM130",  "PC 101 key keyboard #1 (AGE XoftWare, XStation 130 layout)",
       "AGE Logic, Inc. Release",
       &pc101_geom, rs6k_caps, rs6k_codes, xoft1_map),

  KBD ("AGE101",  "PC 101 key keyboard #1 (AGE XoftWare, 2.0 PC layout)",
       "AGE Logic, Inc.",
       &pc101_geom, pc101_caps, xoft2_codes, xoft2_map),

  KBD ("HCLPC",   "PC 101 key keyboard #1 (Hummingbird eXceed US layout)",
       "Hummingbird Communications Ltd.",
       &hclpc_geom, hclpc_caps, hclpc_codes, hclpc_map),


  KBD ("L101",    "PC 101 key keyboard #2 (Linux/XFree86 US layout)", 0,
       &pc101_2_geom, pc101_2_caps, linux101_2_codes, linux101_2_map),


  KBD ("HPPCDE", "PC 102 key keyboard #1 (Hewlett-Packard PC German layout)",
       0,
       &pc102_geom, pc102de_caps, hppc_de_codes, hppc_de_map),

  KBD ("L102",    "PC 102 key keyboard #1 (Linux/XFree86 US layout)", 0,
       &pc102_geom, pc102_caps,   linux102_codes, linux102_map),

  KBD ("L102DE",  "PC 102 key keyboard #1 (Linux/XFree86 German layout)", 0,
       &pc102_geom, pc102de_caps, linux102_codes, linux102de_map),

  KBD ("L102FR",  "PC 102 key keyboard #1 (Linux/XFree86 French layout)", 0,
       &pc102_geom, pc102fr_caps, linux102_codes, linux102fr_map),


  KBD ("L102DE2", "PC 102 key keyboard #2 (Linux/XFree86 German layout)", 0,
       &pc102_2_geom, pc102_2de_caps, linux102_2_codes, linux102_2de_map),


  KBD ("TT", "Atari TT", 0,
       &tt_geom, tt_caps, tt_codes, tt_map),

  KBD ("TEK101", "Tektronix XP20 101-Key (US X11R5 layout)", 0,
       &tek101_geom, tek101_caps, tek101_codes, tek101_map),

  KBD ("TEK101-4", "Tektronix XP20 101-Key (US X11R4 layout)", 0,
      &tek101_geom, tek101_caps, tek101_codes, tek101_4_map),

  KBD ("TEK200US", "Tektronix VT200 (US Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_us_map),

  KBD ("TEK200DA", "Tektronix VT200 (Danish Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_da_map),

  KBD ("TEK200DE", "Tektronix VT200 (German Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_de_map),

  KBD ("TEK200FI", "Tektronix VT200 (Finnish Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_fi_map),

  KBD ("TEK200FR", "Tektronix VT200 (French Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_fr_map),

  KBD ("TEK200IT", "Tektronix VT200 (Italian Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_it_map),

  KBD ("TEK200KA", "Tektronix VT200 (Katakana Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_ka_map),

  KBD ("TEK200NO", "Tektronix VT200 (Norwegian Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_no_map),

  KBD ("TEK200SP", "Tektronix VT200 (Spanish Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_sp_map),

  KBD ("TEK200SW", "Tektronix VT200 (Swedish Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_sw_map),

  KBD ("TEK200SWDE", "Tektronix VT200 (Swiss-German Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_swde_map),

  KBD ("TEK200UK", "Tektronix VT200 (UK Ultrix layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200u_uk_map),

  KBD ("TEK200VMS", "Tektronix VT200 (US VMS layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200v_us_map),

  KBD ("TEK200ESC", "Tektronix VT200 (US ESC layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200xesc_us_map),

  KBD ("TEK200F11", "Tektronix VT200 (US F11 layout)", 0,
       &tek200_geom, tek200_caps, tek200_codes, tek200xf11_us_map),

  KBD ("Labtam", "Labtam X Terminal MT200", "Labtam Australia Pty Ltd",
       &tek101_geom, labtam_caps, labtam_codes, labtam_map),

  KBD ("NeXT", "NeXTstation non-ADB", "Pencom",
       &NeXT_geom, NeXT_caps, NeXT_codes, NeXT_map),

  KBD ("MAG3k", "MIPS Magnum 3000", "MIPS Computer Systems, Inc.",
       &ncd97_geom, mag3k_caps, mag3k_codes, mag3k_map),

  KBD ("MAG-AT", "MIPS Magnum AT (US layout)", 0,
       &magat_geom, magat_caps, magat_codes, magat_map),

  KBD ("RS2030", "MIPS RS-2030", 0,
       &pc101_geom, rs2030_caps, rs2030_codes, rs2030_map),

  KBD ("AViiON", "AViiON 300 (PC layout)", 0,
       &pc101_geom, pc101_caps, aviion300_codes, aviion300_map),

#ifdef XK_kana_NU	/* just in case some losing vendors don't ship it... */
  KBD ("XMINT", "Takaoka Xmint X Terminal",
       "TAKAOKA Electric Manufacturing Co., Ltd.",
       &xmint_geom, xmint_caps, xmint_codes, xmint_map),
#endif

  KBD ("Explorer", "Texas Instruments Explorer", "Texas Instruments Explorer",
       &explorer_geom, explorer_caps, explorer_codes, explorer_map),

#undef KBD
  { 0 }
};
