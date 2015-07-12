 #!/usr/bin/env bash

set -e

export MCSERVER_BUILD_SERIES_NAME="Travis $CC $TRAVIS_MCSERVER_BUILD_TYPE"
export MCSERVER_BUILD_ID=$TRAVIS_JOB_NUMBER
export MCSERVER_BUILD_DATETIME=`date`

cmake . -DBUILD_TOOLS=1 -DSELF_TEST=1;

echo "Checking basic style..."
cd src
lua CheckBasicStyle.lua
cd ..

echo "Building..."
make -j 2;
make -j 2 test ARGS="-V";
cd out/;
if [ "$TRAVIS_MCSERVER_BUILD_TYPE" != "COVERAGE" ]
	then echo stop | $MCSERVER_PATH;
fi
