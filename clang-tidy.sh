#!/bin/sh

set -e

FIXES_FILE="tidy-fixes.yaml"
REGEX=".+/cuberite/src/[^\.].+"
# TODO: Add -quiet when Circle CI is updated to 2.0 with a newer version of Ubuntu
ARGS="-header-filter $REGEX -export-fixes $FIXES_FILE "$@" $REGEX"
TIDY_FAILED=0

mkdir -p tidy-build
cd tidy-build
cmake --target Cuberite -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
if [ -f ../run-clang-tidy.py ]; then
	../run-clang-tidy.py $ARGS || TIDY_FAILED=1
else
	run-clang-tidy.py $ARGS || TIDY_FAILED=1
fi

if [ $TIDY_FAILED -eq 0 ]; then
	echo "clang-tidy: No violations found"
else
	echo "clang-tidy: Found violations"
	exit 1
fi
