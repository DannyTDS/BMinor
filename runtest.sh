#!/bin/sh

#####################
#	Encoder tests	#
#####################
total_tests=0
failures=0
successes=0
echo "Test encoder good..."
for testfile in test/encode/good*.bminor
do
	((total_tests++))
	if ./bminor --encode $testfile > $testfile.out
	then
		((successes++))
	else
		((failures++))
		echo "$testfile failure (INCORRECT)"
	fi
done
echo "Total tests run: $total_tests Failures: $failures Successes: $successes"

total_tests=0
failures=0
successes=0
echo "Test encoder bad..."
for testfile in test/encode/bad*.bminor
do
	((total_tests++))
	if ./bminor --encode $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
		((failures++))
	else
		((successes++))
	fi
done
echo "Total tests run: $total_tests Failures: $failures Successes: $successes"

#####################
#	Scanner tests	#
#####################
total_tests=0
failures=0
successes=0
echo "Test scanner good..."
for testfile in test/scanner/good*.bminor
do
	((total_tests++))
	if ./bminor --scan $testfile > $testfile.out
	then
		((successes++))
	else
		((failures++))
		echo "$testfile failure (INCORRECT)"
	fi
done
echo "Total tests run: $total_tests Failures: $failures Successes: $successes"

total_tests=0
failures=0
successes=0
echo "Test scanner bad..."
for testfile in test/scanner/bad*.bminor
do
	((total_tests++))
	if ./bminor --scan $testfile > $testfile.out
	then
		echo "$testfile success (INCORRECT)"
		((failures++))
	else
		((successes++))
	fi
done
echo "Total tests run: $total_tests Failures: $failures Successes: $successes"