char *version = "@(#)\
XDaliClock v2.03; \
Copyright (c) 1991, 1992, 1993, 1994 Jamie Zawinski (jwz@lucid.com)";

/*   7 oct 91 (v1.00)	submitted for X11r5 contrib tape.
    26 oct 91 (v1.01)	fixed Expose handling in -noseconds mode;
			access() called with too few args;
			added resource for MM/DD/YY, DD/MM/YY, etc.
     3 jan 92 (v1.02)	merged in VMS support from Daniel C. Newman
			<dan@innosoft.com>.
    16 jan 92 (v1.03)	added more intelligent visual support.
			Made it not die on fonts without per-char info.
     4 jun 92 (v1.04)   more VMS support for resource database files, from
			Tony Kennedy <adk@scri.fsu.edu>.
    10 jun 92 (v1.05)   More from Tony Kennedy: support visuals with different
			depths from default, center digits correctly in
			initial window, and merge geometry defaults in a more
			sophisticated way.  Merged in a slightly reworked
			version of more general segment-handling code from
			Dan Wallach <c169-bg@auriga.berkeley.edu>.  Added a
			second, even bigger builtin font.  Added the -root
			and -fullscreen arguments.
     3 feb 93 (v1.06)	Fixed some memory errors.
     4 sep 93 (v1.07)	Fixed shape handling; some minor portability changes.
    16 may 94 (v2.00)	Converted to use Xt instead of raw Xlib.
    21-May-94 (v2.01)	VMS and R3 support, with help from Richard L. Dyson
			<dyson@sunfish.physics.uiowa.edu>.
    31-May-94 (v2.03)	Minor tweaks to Imakefile; submitted for X11r6 contrib.
 */
