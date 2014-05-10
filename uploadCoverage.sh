#!/usr/bin/env bash

if [ "$TRAVIS_MCSERVER_BUILD_TYPE" == "COVERAGE" ]
	then
	find tests -type f -name '*.gcda' -exec cp {} $(dirname {})/../$(basename {})
	coveralls --exclude lib --exclude tests --gcov-options -q
fi


