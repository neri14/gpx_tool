#!/bin/bash

function verdict_pass {
	echo -e "${GREEN}PASS${NC}: $@"
}

function verdict_fail {
	echo -e "${RED}FAIL${NC}: $@"
	score=$(($score + 1))
}

function overal_verdict {
	if [ $score -eq 0 ]; then
		echo -e "overal verdict: ${GREEN}PASS${NC} ($((count-score))/$count tests passed)"
	else
		echo -e "overal verdict: ${RED}FAIL${NC} ($((count-score))/$count tests passed)"
	fi
}

function test_retval_non0 {
	if [ $debug -eq 1 ]; then
		"$@"
	else
		"$@" > /dev/null 2>&1
	fi

	local status=$?

	if [ $status -ne 0 ]; then
		verdict_pass $FUNCNAME $@
	else
		verdict_fail $FUNCNAME $@
	fi

	count=$(($count + 1))
}

function test_retval_is0 {
	if [ $debug -eq 1 ]; then
		"$@"
	else
		"$@" > /dev/null 2>&1
	fi

	local status=$?

	if [ $status -eq 0 ]; then
		verdict_pass $FUNCNAME $@
	else
		verdict_fail $FUNCNAME $@
	fi

	count=$(($count + 1))
}

# check if deubg enabled
debug=0

while getopts ":v" opt; do
	case $opt in
	v)
		debug=1
		;;
	\?)
		echo "Invalid option: -$OPTARG"
		;;
	esac
done

# prepare variables
score=0
count=0
sut="./gpx_tool"

# prepare filesystem
mkdir -p ./bin/test_out
rm -f ./bin/test_out/*

# prepare env
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# test input files are not correct
test_retval_non0 ${sut} check ./input/test_input_1.gpx
test_retval_non0 ${sut} check ./input/test_input_2.gpx

# test expected output files are correct
test_retval_is0 ${sut} check ./input/test_output_1.gpx
test_retval_is0 ${sut} check ./input/test_output_2.gpx

# test files correction successfull
test_retval_is0 ${sut} fix ./input/test_input_1.gpx ./bin/test_out/out_1.gpx
test_retval_is0 ${sut} fix ./input/test_input_2.gpx ./bin/test_out/out_2.gpx

# test output files as expected
test_retval_is0 cmp -s ./input/test_output_1.gpx ./bin/test_out/out_1.gpx
test_retval_is0 cmp -s ./input/test_output_2.gpx ./bin/test_out/out_2.gpx

overal_verdict
exit $score