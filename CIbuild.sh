 #!/usr/bin/env bash

set -e

export CUBERITE_BUILD_SERIES_NAME="Travis $CC $TRAVIS_CUBERITE_BUILD_TYPE"
export CUBERITE_BUILD_ID=$TRAVIS_JOB_NUMBER
export CUBERITE_BUILD_DATETIME=`date`

cmake . -DBUILD_TOOLS=1 -DSELF_TEST=1;

echo "Building..."
make -j 3;
echo "Ccache statistics"
ccache -s;

make test ARGS="-V";

echo "Testing..."
cd Server/;
if [ "$TRAVIS_CUBERITE_BUILD_TYPE" != "COVERAGE" ]; then
	$CUBERITE_PATH << EOF
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
fi
