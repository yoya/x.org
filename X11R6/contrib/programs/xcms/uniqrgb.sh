#!/bin/sh
#   Tue Jun  7 02:03:11 MET DST 1994
#   Christopher Alexander North-Keys

awk '/^[^#!\t]+/ {
    FS="\t"
	last = NF
	while ("" == $last)
		--last;
    if ("" == see[$last])
    {
        print $0
        see[$last] = "foo";
    }
}'

exit $?
#--------------------eof

