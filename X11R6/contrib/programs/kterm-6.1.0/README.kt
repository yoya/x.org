Kterm is an X11 terminal emulator that can handle Japanese text.
This release is based on xterm in the core part of X11R6 distribution.


Configuration
----
Kterm have some optional features and all of them are enabled by default.
Edit Imakefile and kterm.h to disable them.

In Imakefile, -DKTERM, -DSTATUSLINE and -DKEEPALIVE are set in the
DEFINES line. They mean respectively:
 o KTERM:       enables Kana, Kanji, colored text and other supports.
 o STATUSLINE:  enables status line support.
 o KEEPALIVE:   enables keepalive socket option for the server connection.
You may remove any unnecessary options. If you remove all options,
your kterm is identical with xterm.

If you define -DKTERM in Imakefile, kterm.h defines some text related
options, KTERM_MBCS, KTERM_KANJI, KTERM_KCONV, KTERM_COLOR and KTERM_MBCC.
 o KTERM_COLOR: enables colored text support.
 o KTERM_MBCS:  enables support for MBCS text including Kanji.
 o KTERM_KANJI: enables support for Kanji specific encodings like Shift-JIS
                and related menu entries. KTERM_MBCS must be defined.
 o KTERM_KCONV: enables support for Kanji text input using kinput or kinput2.
        	KTERM_MBCS must be defined.
 o KTERM_MBCC:  enables MB character class support. It is used for word
                selection. KTERM_MBCS must be defined.
You may remove some or all of them unneeded. But make sure that you have
to remove KTERM_KANJI, KTERM_KCONV and KTERM_MBCC if you remove KTERM_MBCS.


Compilation
----
In R6 environment, just type:
 % xmkmf; make

Note that this kterm cannot compile under prior releases of X11.


Installation
----
Install kterm and KTerm.ad:
 % make install

Install kterm.man manual page:
 % make install.man

If your system supports Japanese manual pages and you want to install
kterm.jman, copy it to appropriate directory/filename with appropriate
encoding by your hand. Note that kterm.jman is encoded in JIS code.

If your system does not have kterm entry in the termcap or terminfo and you
want to use the statusline or uum or some other software, install termcap.kt
or terminfo.kt by hand.


Supported Systems
-----------------
kterm-6 has been successfully built and tested on the following systems:

	BSD/386 1.1
	FreeBSD 1.1
	HP-UX 9.1
	NEWS-OS 4.2.1R
	SunOS 3.5
	SunOS 4.1.1
	SunOS 4.1.3
	SunOS 4.1.3+JLE
	SunOS 5.3J


Bugs
----
If you find a bug, please make sure that it is not an xterm bug, then report
it to kagotani@cs.titech.ac.jp. Don't forget to include your kterm version
and information about your system. "kterm -version" prints its version.

						June 4, 1994
						Hiroto Kagotani
						kagotani@cs.titech.ac.jp
