#!/bin/sh

FILE="tidy-fixes.yaml"
REGEX=".+/cuberite/src/\w+/.+"
# TODO: Add -quiet when Circle CI is updated to 2.0 with a newer version of Ubuntu
ARGS="-header-filter $REGEX -export-fixes $FILE $REGEX"

cmake --target Cuberite -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .
if [ -f ./run-clang-tidy.py ]; then
	./run-clang-tidy.py $ARGS
else
	run-clang-tidy.py $ARGS
fi

if [ $? -ne 0 ]; then
	echo "run-clang-tidy.py failed to run"
	exit 1
fi

if [ -s $FILE ]; then
	echo "Found violations, fixes can be found in $FILE"
	exit 1
else
	echo "No violations found"
fi
