#
# CMakeLists.txt
# Copyright 2013 Google Inc. All Rights Reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 2.8.12)
project(backward CXX)

include(BackwardMacros.cmake)

###############################################################################
# COMPILER FLAGS
###############################################################################

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic-errors")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

###############################################################################
# BACKWARD OBJECT
###############################################################################

add_library(backward_object OBJECT backward.cpp)
target_compile_definitions(backward_object PRIVATE ${BACKWARD_DEFINITIONS})
target_include_directories(backward_object PRIVATE ${BACKWARD_INCLUDE_DIRS})
set(BACKWARD_ENABLE $<TARGET_OBJECTS:backward_object> CACHE STRING
	"Link with this object to setup backward automatically")


###############################################################################
# TESTS
###############################################################################

if(BACKWARD_TESTS)
	enable_testing()

	add_library(test_main SHARED test/_test_main.cpp)

	macro(backward_add_test src)
		get_filename_component(name ${src} NAME_WE)
		set(test_name "test_${name}")

		add_executable(${test_name} ${src} ${ARGN})

		set_target_properties(${test_name} PROPERTIES
			COMPILE_DEFINITIONS "${BACKWARD_DEFINITIONS}")

		target_include_directories(${test_name} PRIVATE ${BACKWARD_INCLUDE_DIRS})

		target_link_libraries(${test_name} ${BACKWARD_LIBRARIES} test_main)
		add_test(NAME ${name} COMMAND ${test_name})
	endmacro()

	# Tests without backward.cpp
	set(TESTS
		test
		stacktrace
		rectrace
		select_signals
		)

	foreach(test ${TESTS})
		backward_add_test(test/${test}.cpp)
	endforeach()

	# Tests with backward.cpp
	set(TESTS
		suicide
		)

	foreach(test ${TESTS})
		backward_add_test(test/${test}.cpp ${BACKWARD_ENABLE})
	endforeach()
endif()
