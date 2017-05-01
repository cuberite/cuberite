 #!/usr/bin/env bash

set -e

export CUBERITE_BUILD_SERIES_NAME="Travis $CC $TRAVIS_CUBERITE_BUILD_TYPE"
export CUBERITE_BUILD_ID=$TRAVIS_JOB_NUMBER
export CUBERITE_BUILD_DATETIME=`date`

if [ "$CXX" == "g++" ]; then
	# This is a temporary workaround to allow the identification of GCC-4.8 by CMake, required for C++11 features
	# Travis Docker containers don't allow sudo, which update-alternatives needs, and it seems no alternative to this command is provided, hence:
	export CXX="/usr/bin/g++-4.8"
fi
cmake . -DBUILD_TOOLS=1 -DSELF_TEST=1;

echo "Building..."
VERBOSE=1 make -j 2;
make -j 2 test ARGS="-V";

# Create .gdbinit in home directory. Switches off the confirmation on quit
echo -e "define hook-quit\n\tset confirm off\nend\n" > ~/.gdbinit

echo "Testing..."
cd Server/;
touch apiCheckFailed.flag
if [ "$TRAVIS_CUBERITE_BUILD_TYPE" != "COVERAGE" ]; then
	gdb -return-child-result -ex run -ex "bt" -ex "info threads" -ex "thread apply all bt" -ex "quit" --args $CUBERITE_PATH << EOF
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
