
cmake_minimum_required (VERSION 2.6)
project (sqlite)

include_directories ("${PROJECT_SOURCE_DIR}/../")

file(GLOB SOURCE
    "*.c"
)


# add headers to MSVC project files:
if (WIN32)
	file(GLOB HEADERS "src/*.h")
	list(REMOVE_ITEM SOURCE "${PROJECT_SOURCE_DIR}/src/lua.h" "${PROJECT_SOURCE_DIR}/src/luac.h")
	set(SOURCE ${SOURCE} ${HEADERS})
	source_group("Sources" FILES ${SOURCE})
endif()


add_library(sqlite ${SOURCE})

if (UNIX)
	target_link_libraries(sqlite ${DYNAMIC_LOADER})
endif()
