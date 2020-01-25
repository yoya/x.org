#!/bin/sh
# Some System V systems don't understand the #! construct.  
#       In this case replace the first line with just a colon (:)
#
# Copyright 1991 by James J. Dempsey <jjd@bbn.com>
#  Version 5.3
#  Time-stamp: <92/09/14 09:58:49 jjd>
# 
# Permission to use, copy, modify, distribute, and sell this software and its
# documentation for any purpose is hereby granted without fee, provided that
# the above copyright notice appear in all copies and that both that
# copyright notice and this permission notice appear in supporting
# documentation, James J. Dempsey makes no representations about the
# suitability of this software for any purpose.  It is provided "as is"
# without express or implied warranty.
#
# Starts a local xterm running rlogin or telnet
# Written by Stephen Gildea <gildea@expo.lcs.mit.edu> 
#     and Jim Dempsey <jjd@bbn.com>
#
# Usage: xrlogin [-l username] [-telnet] [emulator options] hostname

# Set some defaults.
progname=`basename $0`
usage="usage: $progname [-l logname] [-telnet] [emulator options] host"
netprog="rlogin"
netprogopts="-8"
termprog=xterm
termprogopts=
telnet=
user=
host=

# process arguments
while [ "$*" != "" ]; do
    case "$1" in 
      -telnet)
          telnet=t;
          ;;
      -l)
          if [ "$telnet" ]; then
              echo "$progname: Can't use -l with -telnet" 1>&2
              echo $usage 1>&2
              exit 1
          fi
          user=$2; shift;
          ;;
      *)
	  # assume anything else is an argument to terminal emulator
          #  (except the last one which is the hostname)
	  termprogopts="$termprogopts $1"
          ;;
    esac
    shift
done

if [ "$user" -a "$telnet" ]; then
    echo "$progname: Can't use -l with -telnet" 1>&2
    echo $usage 1>&2
    exit 1
fi

if [ "$telnet" ]; then
    netprog=telnet;
    netprogopts= ;
fi
if [ "$user" ]; then
    netprogopts="$netprogopts -l $user"
fi

# last item in termprogopts is the hostname by definition 
#    (arg list and man page)
item=
# xyzzy keeps shell from getting confused if option begins with '-'
set xyzzy $termprogopts
termprogopts=
shift
for i in $@
do
    if [ ! -z "${item}" ]; then
        termprogopts="$termprogopts $item"
    fi
    item=$i
done
host=$item

if [ "$XRLOGIN_TERM_EMULATOR" != "" ]; then
    termprog=$XRLOGIN_TERM_EMULATOR
else
    case "$TERM" in
      *term)
	# TERM looks like it is an X11 terminal emulator, so use it.
	# Should also look for *terminalEmulatorName resource.
        termprog=$TERM;;
    esac
fi

if [ "$termprog" = "xterm" ]; then
    termprogopts="$termprogopts -ut"
fi

# Remove domain part from hostname.
name=`echo $host | sed -e 's/\..*$//'`

# netprogopts are required to go after hostname on some SYS V hosts
exec $termprog -name $termprog-$name -title $name $termprogopts -e $netprog $host $netprogopts
