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

add_executable(NoiseTest ${SRCS} ${HDRS})
