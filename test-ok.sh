#!/bin/sh

for testfn in $srcdir/tests/ok*.sql
do
	cat $testfn | ./sql
	if [ $? -ne 0 ]
	then
		echo "Failed on $testfn"
		exit 1
	fi
done

