cmake_minimum_required (VERSION 2.6) 

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src/)
include_directories(${CMAKE_SOURCE_DIR}/lib/libevent/include)

add_definitions(-DTEST_GLOBALS=1)
add_library(Network ${CMAKE_SOURCE_DIR}/src/OSSupport/Network.cpp ${CMAKE_SOURCE_DIR}/src/OSSupport/Event.cpp ${CMAKE_SOURCE_DIR}/src/StringUtils.cpp)
target_link_libraries(Network event_core event_extra)

add_executable(Google-exe Google.cpp)
target_link_libraries(Google-exe Network)
add_test(NAME Google-test COMMAND Google-exe)
