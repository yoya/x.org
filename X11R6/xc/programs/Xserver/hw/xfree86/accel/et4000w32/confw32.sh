#!/bin/sh

# $XConsortium: confw32.sh,v 1.2 94/11/21 22:08:46 kaleb Exp $
# $XFree86: xc/programs/Xserver/hw/xfree86/accel/et4000w32/confw32.sh,v 3.0 1994/09/11 00:39:40 dawes Exp $
#
# This script generates w32Conf.c
#
# usage: confw32.sh driver1 driver2 ...
#

VGACONF=./w32Conf.c

cat > $VGACONF <<EOF
/*
 * This file is generated automatically -- DO NOT EDIT
 */

#include "xf86.h"
#include "vga.h"

extern vgaVideoChipRec
EOF
Args="`echo $* | tr '[a-z]' '[A-Z]'`"
set - $Args
while [ $# -gt 1 ]; do
  echo "        $1," >> $VGACONF
  shift
done
echo "        $1;" >> $VGACONF
cat >> $VGACONF <<EOF

vgaVideoChipPtr W32Drivers[] =
{
EOF
for i in $Args; do
  echo "        &$i," >> $VGACONF
done
echo "        NULL" >> $VGACONF
echo "};" >> $VGACONF
