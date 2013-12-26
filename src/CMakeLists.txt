
cmake_minimum_required (VERSION 2.6)
project (MCServer)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++03")
endif()

include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/")
include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/jsoncpp/include")

set(FOLDERS OSSupport HTTPServer Bindings Items Blocks Protocol Generating) 
set(FOLDERS ${FOLDERS} WorldStorage Mobs Entities Simulator UI BlockEntities)



if (NOT WIN32)
	foreach(folder ${FOLDERS})
		add_subdirectory(${folder})
	endforeach(folder)

	file(GLOB SOURCE
		"*.cpp"
	)

	list(REMOVE_ITEM SOURCE "${PROJECT_SOURCE_DIR}/StackWalker.cpp" "${PROJECT_SOURCE_DIR}/LeakFinder.cpp")

else ()

	function(includefolder PATH)
		FILE(GLOB FOLDER_FILES
			"${PATH}/*.cpp"
			"${PATH}/*.h"
		)
		source_group("${PATH}" FILES ${FOLDER_FILES})
	endfunction(includefolder)

	foreach(folder ${FOLDERS})
		includefolder(${folder})
	endforeach(folder)

	file(GLOB_RECURSE SOURCE
		"*.cpp"
		"*.h"
	)

	include_directories("${PROJECT_SOURCE_DIR}")

	source_group("" FILES ${SOURCE})

	# Precompiled headers (1st part)
	SET(PrecompiledBinary "$(IntDir)/Globals.pch")
	SET_SOURCE_FILES_PROPERTIES(
		Globals.cpp PROPERTIES COMPILE_FLAGS "/Yc\"Globals.h\" /Fp\"${PrecompiledBinary}\""
	)
endif()


if (UNIX)
	set(EXECUTABLE ../MCServer/MCServer)
else ()
	set(EXECUTABLE MCServer)
endif ()


add_executable(${EXECUTABLE} ${SOURCE})


if (WIN32)
	SET_TARGET_PROPERTIES(
		${EXECUTABLE} PROPERTIES COMPILE_FLAGS "/Yu\"Globals.h\" /FI\"${PrecompiledBinary}\" /Fp\"${PrecompiledBinary}\""
		OBJECT_DEPENDS "${PrecompiledBinary}"
	)
endif ()


if (NOT WIN32)
	target_link_libraries(${EXECUTABLE} OSSupport HTTPServer Bindings Items Blocks)
	target_link_libraries(${EXECUTABLE} Protocol Generating WorldStorage)
	target_link_libraries(${EXECUTABLE} Mobs Entities Simulator UI BlockEntities)
endif ()
target_link_libraries(${EXECUTABLE} md5 luaexpat iniFile jsoncpp cryptopp zlib lua)
