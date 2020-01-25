#! /bin/sh
for i in *
do
	egrep $i ../ch* >> findarguments
done
