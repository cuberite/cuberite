 #!/usr/bin/env bash

set -e

export CUBERITE_BUILD_SERIES_NAME="Travis $CC $TRAVIS_CUBERITE_BUILD_TYPE"
export CUBERITE_BUILD_ID=$TRAVIS_JOB_NUMBER
export CUBERITE_BUILD_DATETIME=`date`

cmake . -DBUILD_TOOLS=1 -DSELF_TEST=1;

echo "Building..."
cmake --build . -- -j 2;
ctest -j 2 -V;

# Create .gdbinit in home directory.
# * Switches off the confirmation on quit
# * Stops gdb from disabling address space layout randomization.
cat << EOF > ~/.gdbinit
define hook-quit
	set confirm off
end
set disable-randomization off
EOF

echo "Testing..."

# OSX builds don't have gdb
if [ "$TRAVIS_OS_NAME" = osx ]; then
	GDB_COMMAND=""
else
	GDB_COMMAND="gdb -return-child-result -ex run -ex \"bt\" -ex \"info threads\" -ex \"thread apply all bt\" -ex \"quit\" --args"
fi

cd Server/;
touch apiCheckFailed.flag
if [ "$TRAVIS_CUBERITE_BUILD_TYPE" != "COVERAGE" ]; then
	${GDB_COMMAND} ${CUBERITE_PATH} <<- EOF
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
