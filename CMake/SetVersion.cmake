# Copyright 2024 Gavin John
# SPDX-License-Identifier: Apache-2.0

option(OVERRIDE_VERSION "Override the auto-detected version" OFF)
if (OVERRIDE_VERSION)
    set(PROJECT_VERSION ${OVERRIDE_VERSION} CACHE STRING "User-specified version")
else()
    execute_process(
        COMMAND git rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_COMMIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE GIT_COMMIT_RESULT
    )

    execute_process(
        COMMAND git describe --tags --exact-match
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_TAG
        OUTPUT_STRIP_TRAILING_WHITESPACE
        RESULT_VARIABLE GIT_TAG_RESULT
    )

    if (GIT_TAG_RESULT EQUAL 0)
        set(PROJECT_VERSION ${GIT_TAG})
    elseif (GIT_COMMIT_RESULT EQUAL 0)
        set(PROJECT_VERSION "git-${GIT_COMMIT_HASH}")
    else()
        message(WARNING "Git is not installed or repository is not valid. Falling back to 'none'.")
        set(PROJECT_VERSION "none")
    endif()
endif()

add_compile_definitions(PROGRAM_VERSION="${PROJECT_VERSION}")
