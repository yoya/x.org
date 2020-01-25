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
# $XConsortium: Makefile,v 1.5 94/04/17 20:59:47 rws Exp $

#
# Makefile for building the TET and the X test suite libraries/utilities.
#
# For full details on what needs to be built and how to build, refer
# to the User Guide in file userguide.mm in the directory xtest/doc.
# This Makefile just performs some of the required steps more easily.
#
# First, work through the instructions in the section entitled 
# "Configuring the X test suite" in the User Guide. This involves 
# setting the configuration parameters correctly for your system.
# This also includes setting the TET_ROOT environment variable to the 
# directory in which this Makefile is supplied.
#
# Ensure your PATH environment variable is set to include $TET_ROOT/xtest/bin.
#
# You may then need to build the portability library, if your system is not 
# a POSIX.1 system. This is described further in the User Guide.
#
# Then, you can perform the following steps described in the User Guide
# by just typing "make" in this directory:
#
# Building the TET:
# 1. The Test Case Controller (TCC)
# 2. The API library
#
# Building the X test suite libraries and utilities:
# 1. Building the X test suite library
# 2. Building the X Protocol library
# 3. Building the X test fonts library
# 4. Building the mc utility
# 5. Building the blowup utility
#
# You then need to build and install the test fonts as described in the 
# User Guide.
#

all: install

install:
	cd tet/src/posix_c/tools; pmake install
	cd tet/src/posix_c/api; pmake install
	cd xtest/src/lib; pmake install
	cd xtest/src/libproto; pmake install
	cd xtest/src/xim; pmake install
	cd xtest/fonts; pmake install
	cd xtest/src/bin/mc ; pmake install
	cd xtest/src/pixval/blowup; pmake install

clean:
	cd tet/src/posix_c/tools; pclean
	cd tet/src/posix_c/api; pclean
	cd xtest/src/lib; pclean
	cd xtest/src/libproto; pclean
	cd xtest/src/xim; pclean
	cd xtest/fonts; pclean
	cd xtest/src/bin/mc ; pclean
	cd xtest/src/pixval/blowup; pclean
