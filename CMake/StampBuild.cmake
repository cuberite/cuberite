# These env variables are used for configuring Travis CI builds.
if(DEFINED ENV{TRAVIS_CUBERITE_FORCE32})
	set(FORCE32 $ENV{TRAVIS_CUBERITE_FORCE32})
endif()

if(DEFINED ENV{TRAVIS_BUILD_WITH_COVERAGE})
	set(BUILD_WITH_COVERAGE $ENV{TRAVIS_BUILD_WITH_COVERAGE})
endif()

if(DEFINED ENV{CUBERITE_BUILD_ID})
	# The build info is defined by the build system (Travis / Jenkins)
	set(BUILD_ID $ENV{CUBERITE_BUILD_ID})
	set(BUILD_SERIES_NAME $ENV{CUBERITE_BUILD_SERIES_NAME})
	set(BUILD_DATETIME $ENV{CUBERITE_BUILD_DATETIME})
	if(DEFINED ENV{CUBERITE_BUILD_COMMIT_ID})
		set(BUILD_COMMIT_ID $ENV{CUBERITE_BUILD_COMMIT_ID})
	else()
		message("Commit id not set, attempting to determine id from git")
		execute_process(
			COMMAND git rev-parse HEAD
			WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
			RESULT_VARIABLE GIT_EXECUTED
			OUTPUT_VARIABLE BUILD_COMMIT_ID
		)
		string(STRIP ${BUILD_COMMIT_ID} BUILD_COMMIT_ID)
		if (NOT (GIT_EXECUTED EQUAL 0))
			message(FATAL_ERROR "Could not identifiy git commit id")
		endif()
	endif()
else()
	# This is a local build, stuff in some basic info:
	set(BUILD_ID "Unknown")
	set(BUILD_SERIES_NAME "local build")
	execute_process(
		COMMAND git rev-parse HEAD
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
		RESULT_VARIABLE GIT_EXECUTED
		OUTPUT_VARIABLE BUILD_COMMIT_ID
	)
	if (NOT(GIT_EXECUTED EQUAL 0))
		set(BUILD_COMMIT_ID "Unknown")
	endif()
	string(STRIP ${BUILD_COMMIT_ID} BUILD_COMMIT_ID)
	execute_process(
		COMMAND git log -1 --date=iso --pretty=format:%ai
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
		RESULT_VARIABLE GIT_EXECUTED
		OUTPUT_VARIABLE BUILD_DATETIME
	)
	if (NOT(GIT_EXECUTED EQUAL 0))
		set(BUILD_DATETIME "Unknown")
	endif()
	string(STRIP ${BUILD_DATETIME} BUILD_DATETIME)

	# The BUILD_COMMIT_ID and BUILD_DATETIME aren't updated on each repo pull
	# They are only updated when cmake re-configures the project
	# Therefore mark them as "approx: "
	set(BUILD_COMMIT_ID "approx: ${BUILD_COMMIT_ID}")
	set(BUILD_DATETIME "approx: ${BUILD_DATETIME}")
endif()
