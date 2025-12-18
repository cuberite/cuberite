find_package(Threads REQUIRED)
include_directories(${PROJECT_SOURCE_DIR}/src/)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/lib/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set (SHARED_SRCS
	${PROJECT_SOURCE_DIR}/src/BiomeDef.cpp
	${PROJECT_SOURCE_DIR}/src/BlockArea.cpp
	${PROJECT_SOURCE_DIR}/src/Cuboid.cpp
	${PROJECT_SOURCE_DIR}/src/ChunkData.cpp
	${PROJECT_SOURCE_DIR}/src/StringCompression.cpp
	${PROJECT_SOURCE_DIR}/src/StringUtils.cpp

	${PROJECT_SOURCE_DIR}/src/Bindings/LuaState.cpp

	${PROJECT_SOURCE_DIR}/src/Generating/ChunkDesc.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/PieceModifier.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/PiecePool.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/Prefab.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/PrefabPiecePool.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalLimit.cpp
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalStrategy.cpp

	${PROJECT_SOURCE_DIR}/src/Noise/Noise.cpp

	${PROJECT_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/Event.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/File.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/GZipFile.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp

	${PROJECT_SOURCE_DIR}/src/WorldStorage/FastNBT.cpp
	${PROJECT_SOURCE_DIR}/src/WorldStorage/SchematicFileSerializer.cpp
)

set (SHARED_HDRS
	${PROJECT_SOURCE_DIR}/src/BiomeDef.h
	${PROJECT_SOURCE_DIR}/src/BlockArea.h
	${PROJECT_SOURCE_DIR}/src/Cuboid.h
	${PROJECT_SOURCE_DIR}/src/ChunkData.h
	${PROJECT_SOURCE_DIR}/src/Globals.h
	${PROJECT_SOURCE_DIR}/src/StringCompression.h
	${PROJECT_SOURCE_DIR}/src/StringUtils.h

	${PROJECT_SOURCE_DIR}/src/Bindings/LuaState.h

	${PROJECT_SOURCE_DIR}/src/Generating/ChunkDesc.h
	${PROJECT_SOURCE_DIR}/src/Generating/PieceModifier.h
	${PROJECT_SOURCE_DIR}/src/Generating/PiecePool.h
	${PROJECT_SOURCE_DIR}/src/Generating/Prefab.h
	${PROJECT_SOURCE_DIR}/src/Generating/PrefabPiecePool.h
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalLimit.h
	${PROJECT_SOURCE_DIR}/src/Generating/VerticalStrategy.h

	${PROJECT_SOURCE_DIR}/src/Noise/Noise.h

	${PROJECT_SOURCE_DIR}/src/OSSupport/CriticalSection.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/Event.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/File.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/GZipFile.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/WinStackWalker.h

	${PROJECT_SOURCE_DIR}/src/WorldStorage/FastNBT.h
	${PROJECT_SOURCE_DIR}/src/WorldStorage/SchematicFileSerializer.h
)

set (SRCS
	LuaThreadStress.cpp
	Stubs.cpp
	LuaState_Typedefs.inc
	LuaState_Declaration.inc
	Bindings.h
)



source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
source_group("Lua files" FILES Test.lua)
add_executable(LuaThreadStress ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS} Test.lua)
target_link_libraries(LuaThreadStress fmt::fmt libdeflate lsqlite luaexpat Threads::Threads tolualib)
if (WIN32)
	target_link_libraries(LuaThreadStress ws2_32)
endif()
add_test(NAME LuaThreadStress-test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} COMMAND LuaThreadStress)

# Necessary for the test to run.
# Otherwise they crash with the error STATUS_DLL_NOT_FOUND (0xC0000135) because they can't find the lua51.dll

if(WIN32)
	add_custom_command(
			TARGET LuaThreadStress  POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_BINARY_DIR}/Server/lua51.dll # Source
			${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/) # Destination
	message("Copying lua51.dll from " ${CMAKE_BINARY_DIR}/Server/lua51.dll " to " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/)

	get_cmake_property(_variableNames VARIABLES)
	list (SORT _variableNames)
	foreach (_variableName ${_variableNames})
		message(STATUS "${_variableName}=${${_variableName}}")
	endforeach()
endif()



# Put the projects into solution folders (MSVC):
set_target_properties(
	LuaThreadStress
	PROPERTIES FOLDER Tests
)
