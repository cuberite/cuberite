#!/usr/bin/env bash

if [ "$TRAVIS_MCSERVER_BUILD_TYPE" == "COVERAGE" ]
	then
	coveralls --exclude lib --exclude tests
fi
