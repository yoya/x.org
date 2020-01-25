#!/bin/sh
#   Tue Jun  7 02:03:11 MET DST 1994
#   Christopher Alexander North-Keys

trg="$1"
shift
cols="$*"

{
	echo '#	File:  ' "$trg"
	echo '#	Pwd :  ' `pwd`
	echo '#	Date:  ' `date`
	echo '#'
	echo '#	Source Files'
	for file in $cols ; do echo '#		'$file ; done
	echo '#'
	echo '#	Generated via a GNUmakefile from the aforementioned sources.'
	echo '#	Utility written by Group Talisman / North-Keys / 1993.7.5'
	echo '#	Email:  <erlkonig@gnu.ai.mit.edu> or <erlkonig@talisman.com>'
	echo '#'
	echo '#	This file is only effective if named in the XCMSDB env var.'
	echo '#'
	echo '#	WARNING:  This file has fascist syntax;  you should work with'
	echo '#	          the *.xcms source files mentioned above.'
	echo '#'
	echo 'XCMS_COLORDB_START 0.1'
	cat $cols \
    |   awk '/^[^#!\t]+/ {
            FS="\t"
			last = NF
			while ("" == $last)
				--last;
            if ("" == see[$1])
            {
                print $1"\t\t"$last;
                see[$1] = "foo";
            }
        }'
	echo 'XCMS_COLORDB_END'
}

exit $?
#--------------------eof

