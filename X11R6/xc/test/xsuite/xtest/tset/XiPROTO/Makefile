#
# Copyright (c) 1990, 1991  X Consortium
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
# X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
# AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 
# Except as contained in this notice, the name of the X Consortium shall not be
# used in advertising or otherwise to promote the sale, use or other dealings
# in this Software without prior written authorization from the X Consortium.
#
# Copyright 1990, 1991 UniSoft Group Limited.
#
# Permission to use, copy, modify, distribute, and sell this software and
# its documentation for any purpose is hereby granted without fee,
# provided that the above copyright notice appear in all copies and that
# both that copyright notice and this permission notice appear in
# supporting documentation, and that the names of MIT and UniSoft not be
# used in advertising or publicity pertaining to distribution of the
# software without specific, written prior permission.  MIT and UniSoft
# make no representations about the suitability of this software for any
# purpose.  It is provided "as is" without express or implied warranty.
#
# $XConsortium: Makefile,v 1.3 94/04/17 21:13:18 rws Exp $
#

#
# This Makefile can be used to compile all the tests in this 
# section of the X test suite in such a way that they are all links to 
# a single executable file. This normally allows a considerable 
# reduction in the disc space requirements for the X test suite 
# when fully built.
#
# There are two ways this can be done:
# 1) Using the TET. Execute the command:
#     tcc -b -s link_scen xtest linkbuild
# in the directory $TET_ROOT/xtest.
# This will execute the TET build tool (which is normally pmake) 
# in the top level directory of each section of the test suite 
# (including this directory).
#
# 2) Directly without using the TET. Execute the command:
#     pmake 
# in this directory.
#
# For more details, refer to the User Guide
#

# CFLAGS - Compilation flags specific to the X Protocol tests.
#
CFLAGS=$(XP_CFLAGS)
SYSLIBS=$(XP_SYSLIBS)
LIBS=$(XP_LIBS)

# LINTFLAGS - Flags for lint specific to the X Protocol tests.
#
LINTFLAGS=$(XP_LINTFLAGS)
LINTLIBS=$(XP_LINTLIBS)

CAT=cat

ALLTESTS=Tests

# XXX misc.o is missing
TESTOFILES=\
alldvevnts.o \
chgdvctl.o \
chgdvprop.o \
chgfctl.o \
chgkbd.o \
chgptr.o \
chngdvkmap.o \
closdev.o \
dvbell.o \
getdvmmap.o \
getfctl.o \
getvers.o \
grbdev.o \
grbdvbtn.o \
grbdvky.o \
gtbtnmppn.o \
gtdvctl.o \
gtdvfcs.o \
gtdvmtnevnts.o \
gtdvprop.o \
gtkeymppn.o \
gtslctexevnt.o \
listdev.o \
opendev.o \
qrydvstat.o \
setdval.o \
setdvbmap.o \
setdvfcs.o \
setdvmmap.o \
setdvmode.o \
slctexevnt.o \
sndextevnt.o \
ungrbdev.o \
ungrbdvbtn.o \
ungrbdvky.o

LINKOFILE=linktbl.o
OFILES = $(TESTOFILES) $(LINKOFILE)

# XXX misc is missing
DIRLIST=\
alldvevnts \
chgdvctl \
chgdvprop \
chgfctl \
chgkbd \
chgptr \
chngdvkmap \
closdev \
dvbell \
getdvmmap \
getfctl \
getvers \
grbdev \
grbdvbtn \
grbdvky \
gtbtnmppn \
gtdvctl \
gtdvfcs \
gtdvmtnevnts \
gtdvprop \
gtkeymppn \
gtslctexevnt \
listdev \
opendev \
qrydvstat \
setdval \
setdvbmap \
setdvfcs \
setdvmmap \
setdvmode \
slctexevnt \
sndextevnt \
ungrbdev \
ungrbdvbtn \
ungrbdvky

all: subdirs test

test:$P $(OFILES) $(LIBS) $(TCM)
	$(CC) $(LDFLAGS) -o $@ $(OFILES) $(TCM) $(LIBS) $(SYSLIBS)
	$(CAT) test > $(ALLTESTS)
	$(RM) test

subdirs:
	if [ ! -f $(ALLTESTS) ]; then $(CAT) /dev/null > $(ALLTESTS); \
	chmod a+x $(ALLTESTS); else : ; fi
	for i in $(DIRLIST); do \
		if [ -d $$i ]; then \
			(cd $$i; echo Compiling in $$i; $(TET_BUILD_TOOL) linkexec); \
		fi; \
	done

# The xtestlib is made if it doesn't exist
$(XTESTLIB):
	cd $(XTESTROOT)/src/lib; $(TET_BUILD_TOOL) install

# The fontlib is made if it doesn't exist
$(XTESTFONTLIB):
	cd $(XTESTROOT)/fonts; $(TET_BUILD_TOOL) install

# The X Protocol test library is made if it doesn't exist
$(XSTLIB):
	cd $(XTESTROOT)/src/libproto; $(TET_BUILD_TOOL) install


clean:	cletests clesubdirs

cletests:
	$(RM) test $(OFILES) $(ALLTESTS) core

clesubdirs:
	for i in $(DIRLIST); do \
		if [ -d $$i ]; then (cd $$i; $(TET_CLEAN_TOOL) ); fi; done

clobber: clean
