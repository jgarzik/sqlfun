#!/bin/sh

for testfn in $srcdir/fail*.sql
do
	cat $testfn | ./sql
	if [ $? -ne 1 ]
	then
		echo "Failed on $testfn"
		exit 1
	fi
done

