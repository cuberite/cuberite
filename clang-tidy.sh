#!/bin/sh

set -e

FIXES_FILE="tidy-fixes.yaml"
REGEX="/cuberite/src/\.?[^\.]"
ARGS="-header-filter $REGEX -quiet -export-fixes $FIXES_FILE "$@" $REGEX"

# Generate the compilation database
mkdir -p tidy-build
cd tidy-build
cmake --target Cuberite -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..

# Ensure LuaState_Typedefs.inc has been generated
pushd ../src/Bindings
lua BindingsProcessor.lua
popd

if run-clang-tidy $ARGS; then
	echo "clang-tidy: No violations found"
else
	echo "clang-tidy: Found violations"
	exit 1
fi
