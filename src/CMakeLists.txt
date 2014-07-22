cmake_minimum_required (VERSION 2.8.2)
project (MCServer)

include_directories (SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../lib/")
include_directories (SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/include")
include_directories (SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../lib/polarssl/include")

set(FOLDERS
	OSSupport HTTPServer Items Blocks Protocol Generating PolarSSL++ Bindings
	WorldStorage Mobs Entities Simulator UI BlockEntities Generating/Prefabs
)

SET (SRCS
	BiomeDef.cpp
	BlockArea.cpp
	BlockID.cpp
	BlockInfo.cpp
	BoundingBox.cpp
	ByteBuffer.cpp
	ChatColor.cpp
	Chunk.cpp
	ChunkData.cpp
	ChunkMap.cpp
	ChunkSender.cpp
	ChunkStay.cpp
	ClientHandle.cpp
	CommandOutput.cpp
	CompositeChat.cpp
	CraftingRecipes.cpp
	Cuboid.cpp
	DeadlockDetect.cpp
	Enchantments.cpp
	FastRandom.cpp
	FurnaceRecipe.cpp
	Globals.cpp
	Group.cpp
	GroupManager.cpp
	Inventory.cpp
	Item.cpp
	ItemGrid.cpp
	LightingThread.cpp
	LineBlockTracer.cpp
	LinearInterpolation.cpp
	Log.cpp
	MCLogger.cpp
	Map.cpp
	MapManager.cpp
	MobCensus.cpp
	MobFamilyCollecter.cpp
	MobProximityCounter.cpp
	MobSpawner.cpp
	MonsterConfig.cpp
	Noise.cpp
	ProbabDistrib.cpp
	RCONServer.cpp
	Root.cpp
	Scoreboard.cpp
	Server.cpp
	Statistics.cpp
	StringCompression.cpp
	StringUtils.cpp
	Tracer.cpp
	VoronoiMap.cpp
	WebAdmin.cpp
	World.cpp
	main.cpp)

SET (HDRS
	AllocationPool.h
	BiomeDef.h
	BlockArea.h
	BlockID.h
	BlockInServerPluginInterface.h
	BlockInfo.h
	BlockTracer.h
	BoundingBox.h
	ByteBuffer.h
	ChatColor.h
	Chunk.h
	ChunkData.h
	ChunkDataCallback.h
	ChunkDef.h
	ChunkMap.h
	ChunkSender.h
	ChunkStay.h
	ClientHandle.h
	CommandOutput.h
	CompositeChat.h
	CraftingRecipes.h
	Cuboid.h
	DeadlockDetect.h
	Defines.h
	Enchantments.h
	Endianness.h
	FastRandom.h
	ForEachChunkProvider.h
	FurnaceRecipe.h
	Globals.h
	Group.h
	GroupManager.h
	Inventory.h
	Item.h
	ItemGrid.h
	LeakFinder.h
	LightingThread.h
	LineBlockTracer.h
	LinearInterpolation.h
	LinearUpscale.h
	Log.h
	MCLogger.h
	Map.h
	MapManager.h
	Matrix4.h
	MemoryLeak.h
	MersenneTwister.h
	MobCensus.h
	MobFamilyCollecter.h
	MobProximityCounter.h
	MobSpawner.h
	MonsterConfig.h
	Noise.h
	ProbabDistrib.h
	RCONServer.h
	Root.h
	Scoreboard.h
	Server.h
	StackWalker.h
	Statistics.h
	StringCompression.h
	StringUtils.h
	Tracer.h
	Vector3.h
	VoronoiMap.h
	WebAdmin.h
	World.h
	XMLParser.h)

include_directories(".")

if (NOT MSVC)
	# Bindings need to reference other folders, so they are done here instead
	# lib dependencies are not included
	include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../lib/polarssl/include")

	foreach(folder ${FOLDERS})
		add_subdirectory(${folder})
	endforeach(folder)

	get_directory_property(BINDING_DEPENDENCIES DIRECTORY "Bindings" DEFINITION BINDING_DEPENDENCIES)

	#clear file
	file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/BindingDependecies.txt)
	foreach(dependecy ${BINDING_DEPENDECIES})
		#write each dependecy on a seperate line
		file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/BindingDependecies.txt "${dependecy}\n")
	endforeach()

	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "Bindings.cpp Bindings.h")

	list(APPEND SOURCE "${SRCS}")
	list(APPEND SOURCE "${HDRS}")

	# If building a windows version, but not using MSVC, add the resources directly to the makefile:
	if (WIN32)
		list(APPEND SOURCE "Resources/MCServer.rc")
	endif()
else ()
	# MSVC-specific handling: Put all files into one project, separate by the folders:

	source_group(Bindings FILES "Bindings/Bindings.cpp" "Bindings/Bindings.h")

	# Add all subfolders as solution-folders:
	function(includefolder PATH)
		FILE(GLOB FOLDER_FILES
			"${PATH}/*.cpp"
			"${PATH}/*.h"
			"${PATH}/*.rc"
			"${PATH}/*.pkg"
		)
		string(REPLACE "/" "\\" PROJECT_PATH ${PATH})
		source_group("${PROJECT_PATH}" FILES ${FOLDER_FILES})
	endfunction(includefolder)

	foreach(folder ${FOLDERS})
		add_subdirectory(${folder})
		includefolder(${folder})

		# Get all source files in this folder:
		get_directory_property(FOLDER_SRCS DIRECTORY ${folder} DEFINITION SRCS)
		foreach (src ${FOLDER_SRCS})
			list(APPEND SOURCE "${folder}/${src}")
		endforeach(src)

		# Get all headers in this folder:
		get_directory_property(FOLDER_HDRS DIRECTORY ${folder} DEFINITION HDRS)
		foreach (hdr ${FOLDER_HDRS})
			list(APPEND SOURCE "${folder}/${hdr}")
		endforeach(hdr)
	endforeach(folder)

	list(APPEND SOURCE "${SRCS}")
	list(APPEND SOURCE "${HDRS}")
	list(APPEND SOURCE "Bindings/AllToLua.pkg")

	includefolder("Resources")
	source_group("" FILES ${SOURCE})

	include_directories("${PROJECT_SOURCE_DIR}")

	# Precompiled headers (1st part)
	SET_SOURCE_FILES_PROPERTIES(
		Globals.cpp PROPERTIES COMPILE_FLAGS "/Yc\"Globals.h\""
	)
	# CMake cannot "remove" the precompiled header flags, so we use a dummy precompiled header compatible with just this one file:
	SET_SOURCE_FILES_PROPERTIES(
		Bindings/Bindings.cpp PROPERTIES COMPILE_FLAGS "/Yc\"string.h\" /Fp\"$(IntDir)/Bindings.pch\""
	)
	SET_SOURCE_FILES_PROPERTIES(
		"StackWalker.cpp LeakFinder.h" PROPERTIES COMPILE_FLAGS "/Yc\"Globals.h\""
	)
	list(APPEND SOURCE "Resources/MCServer.rc")

	# Make MSVC generate the PDB files even for the release build:
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
	set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   /Zi")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}    /DEBUG")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
	set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /DEBUG")
endif()


# Generate a list of all source files:
set(ALLFILES "")
foreach(folder ${FOLDERS})
	get_directory_property(FOLDER_SRCS DIRECTORY ${folder} DEFINITION SRCS)
	foreach (src ${FOLDER_SRCS})
		list(APPEND ALLFILES "${folder}/${src}")
	endforeach(src)

	get_directory_property(FOLDER_HDRS DIRECTORY ${folder} DEFINITION HDRS)
	foreach (hdr ${FOLDER_HDRS})
		list(APPEND ALLFILES "${folder}/${hdr}")
	endforeach(hdr)
endforeach(folder)
foreach(arg ${ALLFILES})
	set(ALLFILESLINES "${ALLFILESLINES}${arg}\n")
endforeach()
FILE(WRITE "AllFiles.lst" "${ALLFILESLINES}")


set(EXECUTABLE MCServer)

if (MSVC)
	get_directory_property(BINDING_OUTPUTS DIRECTORY "Bindings" DEFINITION BINDING_OUTPUTS)
	get_directory_property(BINDING_DEPENDENCIES DIRECTORY "Bindings" DEFINITION BINDING_DEPENDENCIES)

	ADD_CUSTOM_COMMAND(
		OUTPUT ${BINDING_OUTPUTS}

		# Copy the Lua DLL into the Bindings folder, so that tolua can run from there:
		COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_SOURCE_DIR}/MCServer/lua51.dll ./lua51.dll

		# Regenerate bindings:
		COMMAND tolua -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/

		# add any new generation dependencies here
		DEPENDS ${BINDING_DEPENDENCIES}
	)
endif()

add_executable(${EXECUTABLE} ${SOURCE})


# Output the executable into the $/MCServer folder, so that it has access to external resources:
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/MCServer)
SET_TARGET_PROPERTIES(${EXECUTABLE} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/MCServer
	RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/MCServer
	RUNTIME_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_SOURCE_DIR}/MCServer
	RUNTIME_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_SOURCE_DIR}/MCServer
)


# Make the debug executable have a "_debug" suffix
SET_TARGET_PROPERTIES(${EXECUTABLE} PROPERTIES DEBUG_POSTFIX "_debug")

# Make the profiled executables have a "_profile" postfix
SET_TARGET_PROPERTIES(${EXECUTABLE} PROPERTIES DEBUGPROFILE_POSTFIX "_debug_profile")
SET_TARGET_PROPERTIES(${EXECUTABLE} PROPERTIES RELEASEPROFILE_POSTFIX "_profile")


# Precompiled headers (2nd part)
if (MSVC)
	SET_TARGET_PROPERTIES(
		${EXECUTABLE} PROPERTIES COMPILE_FLAGS "/Yu\"Globals.h\""
		OBJECT_DEPENDS "$(IntDir)/$(TargetName.pch)"
	)
endif ()


if (NOT MSVC)
	target_link_libraries(${EXECUTABLE}
		OSSupport HTTPServer Bindings Items Blocks
		Protocol Generating Generating_Prefabs WorldStorage
		Mobs Entities Simulator UI BlockEntities PolarSSL++
	)
endif ()
if (WIN32)
	target_link_libraries(${EXECUTABLE} expat tolualib ws2_32.lib Psapi.lib)
endif()
target_link_libraries(${EXECUTABLE} luaexpat iniFile jsoncpp polarssl zlib sqlite lua)
