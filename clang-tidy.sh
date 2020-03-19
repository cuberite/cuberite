#!/bin/sh

set -e

FIXES_FILE="tidy-fixes.yaml"
REGEX="/cuberite/src/\.?[^\.]"
ARGS="-header-filter $REGEX -quiet -export-fixes $FIXES_FILE "$@" $REGEX"

mkdir -p tidy-build
cd tidy-build
cmake --target Cuberite -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

if run-clang-tidy.py $ARGS; then
	echo "clang-tidy: No violations found"
else
	echo "clang-tidy: Found violations"
	exit 1
fi
