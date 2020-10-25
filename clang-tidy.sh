#!/bin/sh

set -e

FIXES_FILE="tidy-fixes.yaml"
REGEX="/cuberite/src/\.?[^\.]"
ARGS="-header-filter $REGEX -quiet -export-fixes $FIXES_FILE "$@" $REGEX"

# Generate the compilation database
mkdir -p tidy-build
cd tidy-build

# Disable precompiled headers since they aren't generated during linting which causes an error
# Disable unity builds since clang-tidy needs the full list of compiled files to check each one
cmake --target Cuberite -DCMAKE_EXPORT_COMPILE_COMMANDS=Yes -DPRECOMPILE_HEADERS=No -DUNITY_BUILDS=No ..

# Ensure LuaState_Typedefs.inc has been generated
(cd ../src/Bindings && lua BindingsProcessor.lua)

if run-clang-tidy $ARGS; then
	echo "clang-tidy: No violations found"
else
	echo "clang-tidy: Found violations"
	exit 1
fi
