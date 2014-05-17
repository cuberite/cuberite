 #!/usr/bin/env bash

set -e

cmake . -DBUILD_TOOLS=1 -DSELF_TEST=1;
make -j 2;
make -j 2 test;
cd MCServer/;
if [ "$TRAVIS_MCSERVER_BUILD_TYPE" != "COVERAGE" ]
	then echo stop | $MCSERVER_PATH;
fi
