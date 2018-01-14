 #!/usr/bin/env bash

set -e

export CUBERITE_BUILD_SERIES_NAME="Travis $CC $TRAVIS_CUBERITE_BUILD_TYPE"
export CUBERITE_BUILD_ID=$TRAVIS_JOB_NUMBER
export CUBERITE_BUILD_DATETIME=`date`

cmake . -DBUILD_TOOLS=1 -DSELF_TEST=1;

echo "Building..."
cmake --build . -- -j 2;
ctest -j 2 -V;

echo "Testing..."

cd Server/;
touch apiCheckFailed.flag
if [ "$TRAVIS_CUBERITE_BUILD_TYPE" != "COVERAGE" ]; then
	${CUBERITE_PATH} <<- EOF
		load APIDump
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
