#!/usr/bin/env bash

if [ "$TRAVIS_MCSERVER_BUILD_TYPE" == "COVERAGE" ]
	then
	find tests -type f -name '*.gcda' -exec sh -c 'cp {} $(dirname {})/../$(basename {})' \;
	coveralls --exclude lib --exclude Android >/dev/null
fi


