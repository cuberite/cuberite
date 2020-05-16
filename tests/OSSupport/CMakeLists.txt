find_package(Threads REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/src/)

# Create a single OSSupport library that contains all the OSSupport code used in the tests:
set (OSSupport_SRCS
	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp
	${CMAKE_SOURCE_DIR}/src/OSSupport/Event.cpp
	${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
)
set (OSSupport_HDRS
	${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.h
	${CMAKE_SOURCE_DIR}/src/OSSupport/Event.h
	${CMAKE_SOURCE_DIR}/src/StringUtils.h
	${CMAKE_SOURCE_DIR}/src/Globals.h
)
add_library(OSSupport
	${OSSupport_SRCS}
	${OSSupport_HDRS}
)
target_link_libraries(OSSupport PUBLIC fmt::fmt)

# Define individual tests:

# StressEvent: Stress-test the cEvent implementation:
add_executable(StressEvent-exe StressEvent.cpp)
target_link_libraries(StressEvent-exe OSSupport fmt::fmt Threads::Threads)
add_test(NAME StressEvent-test COMMAND StressEvent-exe)



# Put all the tests into a solution folder (MSVC):
set_target_properties(
	StressEvent-exe
	PROPERTIES FOLDER Tests/OSSupport
)
set_target_properties(
	OSSupport
	PROPERTIES FOLDER Tests/Libraries
)




