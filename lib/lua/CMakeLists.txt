
cmake_minimum_required (VERSION 2.6)
project (lua)

include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

file(GLOB SOURCE
	"src/*.c"
)

add_library(lua ${SOURCE})

