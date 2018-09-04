#!/bin/sh

if [ "$TRAVIS_CUBERITE_BUILD_TYPE" = "COVERAGE" ]
	then
	find tests -type f -name '*.gcda' -exec sh -c 'cp $1 $(dirname $1)/../$(basename $1)' _ {} \;
	coveralls --exclude lib --exclude Android >/dev/null
fi
