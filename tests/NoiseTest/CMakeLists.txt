cmake_minimum_required (VERSION 2.6) 

include_directories(${CMAKE_SOURCE_DIR}/src/)

set (SRCS
	NoiseTest.cpp
	OldNoise.cpp
	${CMAKE_SOURCE_DIR}/src/Noise/Noise.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
	${CMAKE_SOURCE_DIR}/src/Logger.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/File.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
)

set (HDRS
	OldNoise.h
	${CMAKE_SOURCE_DIR}/src/Noise/Noise.h
	${CMAKE_SOURCE_DIR}/src/StringUtils.h
	${CMAKE_SOURCE_DIR}/src/Logger.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/File.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/StackTrace.h
)

# MSVC builds require the Windows-only StackWalker source file:
if (MSVC)
	list (APPEND SRCS ${CMAKE_SOURCE_DIR}/src/StackWalker.cpp)
	list (APPEND HDRS ${CMAKE_SOURCE_DIR}/src/StackWalker.h)
endif()

# Disable warnings-as-errors in Clang:
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	add_flags_cxx("-Wno-error=conversion -Wno-error=old-style-cast")
	if ("${CLANG_VERSION}" VERSION_GREATER 3.5)
		add_flags_cxx("-Wno-error=inconsistent-missing-override")
	endif()
endif()

add_executable(NoiseTest ${SRCS} ${HDRS})
