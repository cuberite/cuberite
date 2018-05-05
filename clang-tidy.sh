#!/bin/sh

set -e

FIXES_FILE="tidy-fixes.yaml"
REGEX=".+/cuberite/src/[^\.].+"
# TODO: Add -quiet when Circle CI is updated to 2.0 with a newer version of Ubuntu
ARGS="-header-filter $REGEX -export-fixes $FIXES_FILE "$@" $REGEX"

mkdir -p tidy-build
cd tidy-build
cmake --target Cuberite -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
run-clang-tidy.py $ARGS || TIDY_FAILED=1

if run-clang-tidy.py $ARGS; then
	echo "clang-tidy: No violations found"
else
	echo "clang-tidy: Found violations"
	exit 1
fi
