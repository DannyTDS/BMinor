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
	if ./bminor --encode $testfile > $testfile.out 2>&1
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
	if ./bminor --encode $testfile > $testfile.out 2>&1
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
	if ./bminor --scan $testfile > $testfile.out 2>&1
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
	if ./bminor --scan $testfile > $testfile.out 2>&1
	then
		echo "$testfile success (INCORRECT)"
		((failures++))
	else
		((successes++))
	fi
done
echo "Total tests run: $total_tests Failures: $failures Successes: $successes"

#####################
#	Parser tests	#
#####################
total_tests=0
failures=0
successes=0
echo "Test parser good..."
for testfile in test/parser/good*.bminor
do
	((total_tests++))
	if ./bminor --parse $testfile > $testfile.out 2>&1
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
echo "Test parser bad..."
for testfile in test/parser/bad*.bminor
do
	((total_tests++))
	if ./bminor --parse $testfile > $testfile.out 2>&1
	then
		echo "$testfile success (INCORRECT)"
		((failures++))
	else
		((successes++))
	fi
done
echo "Total tests run: $total_tests Failures: $failures Successes: $successes"

#####################
#	Printer tests	#
#####################
total_tests=0
failures=0
successes=0
echo "Test printer good..."
for testfile in test/printer/good*.bminor
do
	((total_tests++))
	if ./bminor --print $testfile > $testfile.out 2>&1
	then
		# Feed printed result back through parser
		if ./bminor --parse $testfile.out > $testfile.reparse.out 2>&1
		then
			((successes++))
		else
			((failures++))
			echo "$testfile failure in re-parsing the printer output (INCORRECT)"
		fi
	else
		((failures++))
		echo "$testfile failure (INCORRECT)"
	fi
done
echo "Total tests run: $total_tests Failures: $failures Successes: $successes"

#####################
#	Resolve tests	#
#####################
total_tests=0
failures=0
successes=0
echo "Test resolve good..."
for testfile in test/resolve/good*.bminor
do
	((total_tests++))
	if ./bminor --resolve $testfile > $testfile.out 2>&1
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
echo "Test resolve bad..."
for testfile in test/resolve/bad*.bminor
do
	((total_tests++))
	if ./bminor --resolve $testfile > $testfile.out 2>&1
	then
		echo "$testfile success (INCORRECT)"
		((failures++))
	else
		((successes++))
	fi
done
echo "Total tests run: $total_tests Failures: $failures Successes: $successes"

#####################
#	Typecheck tests	#
#####################
total_tests=0
failures=0
successes=0
echo "Test typecheck good..."
for testfile in test/typecheck/good*.bminor
do
	((total_tests++))
	if ./bminor --typecheck $testfile > $testfile.out 2>&1
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
echo "Test typecheck bad..."
for testfile in test/typecheck/bad*.bminor
do
	((total_tests++))
	if ./bminor --typecheck $testfile > $testfile.out 2>&1
	then
		echo "$testfile success (INCORRECT)"
		((failures++))
	else
		((successes++))
	fi
done
echo "Total tests run: $total_tests Failures: $failures Successes: $successes"

#####################
#	Codegen tests	#
#####################
total_tests=0
failures=0
successes=0
echo "Test codegen good..."
for testfile in test/codegen/good*.bminor
do
	((total_tests++))
	if ./bminor --codegen $testfile $testfile.s > $testfile.out 2>&1 && gcc -g $testfile.s src/library.c -o $testfile.exe
	then
		((successes++))
	else
		((failures++))
		echo "$testfile failure (INCORRECT)"
	fi
done
echo "Total tests run: $total_tests Failures: $failures Successes: $successes"