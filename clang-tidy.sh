#!/bin/sh

set -e

FIXES_FILE="tidy-fixes.yaml"
REGEX="/cuberite/src/\\.?[^\\.]"

run_clang_tidy_args () {
    run-clang-tidy.py \
	-header-filter "$REGEX" -quiet -export-fixes "$FIXES_FILE" "$1" "$REGEX"
}

mkdir -p tidy-build
cd tidy-build
cmake --target Cuberite -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

if run_clang_tidy_args "$@"; then
	echo "clang-tidy: No violations found"
else
	echo "clang-tidy: Found violations"
	exit 1
fi
