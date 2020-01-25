#! /bin/sh
# $Id: xcolorize.sh,v 6.1 94/05/25 13:53:51 dd Exp $

# Copyright (c) 1994 David DiGiacomo david@slack.com
#
# The X Consortium, and any party obtaining a copy of these files from
# the X Consortium, directly or indirectly, is granted, free of charge, a
# full and unrestricted irrevocable, world-wide, paid up, royalty-free,
# nonexclusive right and license to deal in this software and
# documentation files (the "Software"), including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons who receive
# copies from any such party to do so.  This license includes without
# limitation a license to do the foregoing actions under any patents of
# the party supplying this software to the X Consortium.

# xcolorize:
#	start X client program with pleasant background color

cfmt=xrgb	# default to rgb:rr/gg/bb format
dark=0		# default to light (pastel) colors
rv=0		# default to non-reverse video
coloronly=0	# default to printing window fg/bg arguments

while [ $# -gt 0 ]
do
	case "$1" in
	-rgb)		cfmt=xrgb shift ;;
	-on*|-c*)	coloronly=1 shift ;;
	-o*sh|-sh)	cfmt=xoldsh shift ;;
	-o*|-\#*)	cfmt=xold shift ;;
	-s*|-xv*|-dec*)	cfmt=sun shift ;;
	-d*)		dark=1 shift ;;
	-p*)		dark=0 shift ;;
	-r*)		rv=1 shift ;;
	-h*)		echo 'usage:'
			echo "	`basename $0` [options] [command args...]"
			echo ''
			echo 'options:'
			echo ''
			echo '	-dark	choose a dark color'
			echo '	-pastel	choose a light color [default]'
			echo '	-only	generate color only (no -fg/-bg)'
			echo '	-rv	reverse foreground and background'
			echo ''
			echo '	-old	use #RRGGBB color format'
			echo '	-oldsh	use \#RRGGBB color format'
			echo '	-rgb	use rgb:RR/GG/BB color format [default]'
			echo '	-sun	use XView/SunView color format'
			echo ''
			echo '	-help	print this message'
			echo ''
			echo '	default command is echo '
			exit 0
			;;
	--)		shift; break;;
	-*)		echo 'usage:' 1>&2
			echo "`basename $0`" '[-help]' \
				'[-dark] [-only] [-rv]' \
				'[-old|-oldsh|-sun]' \
				'[command args...]' 1>&2
			exit 1 ;;
	*)		break ;;
	esac
done

# HSV to RGB conversion parameters
j=100	# max sv value
k=255	# max rgb value

fg= bg=
case $coloronly in
0)
	case $cfmt.$dark.$rv in
	sun.0.0) fg=-Wg bg=-Wb ;;
	sun.1.0) fg='-Wg -Wf 255 255 255' bg=-Wb ;;
	sun.0.1) fg='-Wg -Wb' bg='-Wf 255 255 255' ;;
	sun.1.1) fg='-Wg -Wb' bg='-Wf 0 0 0' ;;
	x*.0.0) fg='-fg black' bg='-bg' ;;
	x*.1.0) fg='-fg white' bg='-bg' ;;
	x*.0.1) fg='-bg black' bg='-fg' ;;
	x*.1.1) fg='-bg white' bg='-fg' ;;
	esac
	;;
esac

case $cfmt in
xrgb)	fmt="$fg $bg rgb:%02x/%02x/%02x" ;;
xold)	fmt="$fg $bg #%02x%02x%02x" ;;
xoldsh)	fmt="$fg $bg \#%02x%02x%02x" ;;
sun)	fmt="$fg $bg %d %d %d" ;;
esac

# Recipe:
# 1. mash together date and PID
# 2. stir bits until lumpy (a real random number generator would be better)
# 3. extract "random" hue, saturation and value
# 4. convert to RGB
# 5. format as needed
#
# In theory the colors could be generated directly in TekHVC space,
# but I wasn't able to get satisfactory results that way.

color=`date '+%y %j %H %M %S' 2> /dev/null |
awk "
{
	pid = $$
	j = $j
	k = $k
	dark = $dark
	rv = $rv
"'
	time = (($1 * 365) + $2) * 86400
	time %= 65536
	time += (($3 * 60) + $4 * 60) + $5

	r = time * 49.07692307 + pid * 29.47619075
	r %= 262144

	h = r % 360
	r /= 359.3
	if (dark) {
		s = r % 11 + 50
		r /= 13
		v = r % 11 + 50
	}
	else {
		s = r % 9 + 12
		v = 100
	}
	s = s * k / j
	if (s < 0) s = 0
	if (s > k) s = k
	v = v * k / j
	if (v < 0) v = 0
	if (v > k) v = k

	if (s == 0) {
		h = 0;
		r = g = b = int(v);
	}
	else {
		i = int(h / 60);
		f = h % 60;
		p = int(v * (k - s) / k);
		q = int(v * (k - s * f / 60) / k);
		t = int(v * (k - s * (60 - f) / 60) / k);

		if (i == 0) {
			r = v; g = t; b = p;
		}
		else if (i == 1) {
			r = q; g = v; b = p;
		}
		else if (i == 2) {
			r = p; g = v; b = t;
		}
		else if (i == 3) {
			r = p; g = q; b = v;
		}
		else if (i == 4) {
			r = t; g = p; b = v;
		}
		else if (i == 5) {
			r = v; g = p; b = q;
		}
	}
	printf "'"$fmt"'\n",r,g,b
	exit
}
'`

case $# in
0)
	exec echo $color
	;;
*)
	prog="$1" shift
	exec "$prog" $color ${1+"$@"}
	;;
esac
