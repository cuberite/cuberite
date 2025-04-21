#!/usr/bin/env bash

set -e

export CUBERITE_BUILD_SERIES_NAME="CI $CC $CI_CUBERITE_BUILD_TYPE"
export CUBERITE_BUILD_ID=$CI_JOB_NUMBER
export CUBERITE_BUILD_DATETIME=`date`

# Use ccache if available
if [ `which ccache` ]; then
	# Re-run compile on pre-processed sources on cache miss
	# "It's slower actually, but clang builds fail without it."
	export CCACHE_CPP2=true

	# Tell CMake of ccache's existence
	CACHE_ARGS="-DCMAKE_C_COMPILER_LAUNCHER=ccache -DCMAKE_CXX_COMPILER_LAUNCHER=ccache"

	echo "Using ccache installed at $(which ccache)"
	ccache --max-size=1G
	ccache --zero-stats
fi

workdir="$CC"_"$CI_CUBERITE_BUILD_TYPE"
mkdir "$workdir"
cd "$workdir"

# Work around a Clang + ccache issue with failing builds by disabling
# precompiled headers. Turn off LTO for faster build speeds
cmake .. -DCMAKE_BUILD_TYPE=${CI_CUBERITE_BUILD_TYPE} \
        -DBUILD_TOOLS=Yes \
        -DPRECOMPILE_HEADERS=No \
        -DSELF_TEST=Yes \
        -DUNITY_BUILDS=${CI_CUBERITE_UNITY_BUILDS-Yes} \
        -DWHOLE_PROGRAM_OPTIMISATION=No \
        ${CACHE_ARGS};

echo "Building..."
cmake --build . --parallel 3;

if [ `which ccache` ]; then
	echo "Built with ccache, outputting cache stats..."
	ccache --show-stats --verbose
fi

echo "Testing..."
ctest --output-on-failure --parallel 2;

cd Server/;
touch apiCheckFailed.flag
if [ "$CI_CUBERITE_BUILD_TYPE" != "COVERAGE" ]; then
	./Cuberite <<- EOF
		load APIDump
		api
		apicheck
		restart
		stop
	EOF
	if [ -f ./NewlyUndocumented.lua ]; then
		echo "ERROR: Newly undocumented API symbols found:"
		cat ./NewlyUndocumented.lua
		exit 1
	fi
	if [ -f ./DuplicateDocs.txt ]; then
		echo "ERROR: API documentation has duplicate symbol warnings:"
		cat ./DuplicateDocs.txt
		exit 1
	fi
	if [ -f ./apiCheckFailed.flag ]; then
		echo "ERROR: API check has failed with an unknown error"
		exit 1
	fi
fi
