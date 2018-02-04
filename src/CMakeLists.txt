project (Cuberite)


include_directories (SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../lib/")
include_directories (SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../lib/jsoncpp/include")
include_directories (SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../lib/mbedtls/include")
include_directories (SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../lib/libevent/include")

set(FOLDERS
	OSSupport HTTP Items Blocks Protocol Generating mbedTLS++ Bindings
	WorldStorage Mobs Entities Simulator Simulator/IncrementalRedstoneSimulator
	BlockEntities UI Noise
)

SET (SRCS
	BiomeDef.cpp
	BlockArea.cpp
	BlockID.cpp
	BlockInfo.cpp
	BrewingRecipes.cpp
	Broadcaster.cpp
	BoundingBox.cpp
	ByteBuffer.cpp
	ChatColor.cpp
	Chunk.cpp
	ChunkData.cpp
	ChunkMap.cpp
	ChunkSender.cpp
	ChunkStay.cpp
	ClientHandle.cpp
	Color.cpp
	CommandOutput.cpp
	CompositeChat.cpp
	CraftingRecipes.cpp
	Cuboid.cpp
	DeadlockDetect.cpp
	Enchantments.cpp
	FastRandom.cpp
	FurnaceRecipe.cpp
	Globals.cpp
	IniFile.cpp
	Inventory.cpp
	Item.cpp
	ItemGrid.cpp
	LightingThread.cpp
	LineBlockTracer.cpp
	LinearInterpolation.cpp
	LoggerListeners.cpp
	Logger.cpp
	Map.cpp
	MapManager.cpp
	MemorySettingsRepository.cpp
	MobCensus.cpp
	MobFamilyCollecter.cpp
	MobProximityCounter.cpp
	MobSpawner.cpp
	MonsterConfig.cpp
	NetherPortalScanner.cpp
	OverridesSettingsRepository.cpp
	ProbabDistrib.cpp
	RankManager.cpp
	RCONServer.cpp
	Root.cpp
	Scoreboard.cpp
	Server.cpp
	SetChunkData.cpp
	SpawnPrepare.cpp
	Statistics.cpp
	StringCompression.cpp
	StringUtils.cpp
	Tracer.cpp
	UUID.cpp
	VoronoiMap.cpp
	WebAdmin.cpp
	World.cpp
	main.cpp
)

SET (HDRS
	AllocationPool.h
	BiomeDef.h
	BlockArea.h
	BlockID.h
	BlockInServerPluginInterface.h
	BlockInfo.h
	BlockTracer.h
	BrewingRecipes.h
	Broadcaster.h
	BoundingBox.h
	BuildInfo.h
	BuildInfo.h.cmake
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
	Color.h
	CommandOutput.h
	CompositeChat.h
	CraftingRecipes.h
	Cuboid.h
	DeadlockDetect.h
	Defines.h
	EffectID.h
	Enchantments.h
	Endianness.h
	FastRandom.h
	ForEachChunkProvider.h
	FurnaceRecipe.h
	FunctionRef.h
	Globals.h
	IniFile.h
	Inventory.h
	Item.h
	ItemGrid.h
	LazyArray.h
	LightingThread.h
	LineBlockTracer.h
	LinearInterpolation.h
	LinearUpscale.h
	Logger.h
	LoggerListeners.h
	LoggerSimple.h
	Map.h
	MapManager.h
	Matrix4.h
	MemorySettingsRepository.h
	MobCensus.h
	MobFamilyCollecter.h
	MobProximityCounter.h
	MobSpawner.h
	MonsterConfig.h
	NetherPortalScanner.h
	OverridesSettingsRepository.h
	ProbabDistrib.h
	RankManager.h
	RCONServer.h
	Root.h
	Scoreboard.h
	Server.h
	SetChunkData.h
	SettingsRepositoryInterface.h
	SpawnPrepare.h
	Statistics.h
	Stopwatch.h
	StringCompression.h
	StringUtils.h
	Tracer.h
	UUID.h
	Vector3.h
	VoronoiMap.h
	WebAdmin.h
	World.h
	XMLParser.h
)

include_directories(".")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../lib/sqlite")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../lib/SQLiteCpp/include")
include_directories (SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../lib/TCLAP/include")

configure_file("BuildInfo.h.cmake" "${CMAKE_CURRENT_SOURCE_DIR}/BuildInfo.h")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set_source_files_properties(BlockID.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors")
	set_source_files_properties(ByteBuffer.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors")
	set_source_files_properties(ClientHandle.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors ")
	set_source_files_properties(Statistics.cpp PROPERTIES COMPILE_FLAGS "-Wno-error=global-constructors")
endif()

if (NOT MSVC)
	# Bindings need to reference other folders, so they are done here instead
	# lib dependencies are not included
	include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../lib/mbedtls/include")

	foreach(folder ${FOLDERS})
		add_subdirectory(${folder})
	endforeach(folder)

	get_directory_property(BINDING_DEPENDENCIES DIRECTORY "Bindings" DEFINITION BINDING_DEPENDENCIES)

	#clear file
	file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/BindingDependencies.txt)
	foreach(dependency ${BINDING_DEPENDENCIES})
		#write each dependency on a seperate line
		file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/BindingDependencies.txt "${dependency}\n")
	endforeach()

	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "Bindings.cpp Bindings.h")

	list(APPEND SOURCE "${SRCS}")
	list(APPEND SOURCE "${HDRS}")

	# If building a windows version, but not using MSVC, add the resources directly to the makefile:
	if (WIN32)
		list(APPEND SOURCE "Resources/Cuberite.rc")
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

		# Include this folder's CMakeLists.txt in the project:
		list(APPEND SOURCE "${folder}/CMakeLists.txt")
		source_group("${folder}" FILES "${folder}/CMakeLists.txt")
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
	list(APPEND SOURCE "Resources/Cuberite.rc")

	# Make MSVC generate the PDB files even for the release build:
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
	set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   /Zi")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}    /DEBUG")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG")
	set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /DEBUG")
endif()


# Generate a list of all source files:
set(ALLFILES "${SRCS}" "${HDRS}")
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

if (MSVC)
	get_directory_property(BINDING_OUTPUTS DIRECTORY "Bindings" DEFINITION BINDING_OUTPUTS)
	get_directory_property(BINDING_DEPENDENCIES DIRECTORY "Bindings" DEFINITION BINDING_DEPENDENCIES)

	# The paths in BINDING_DEPENDENCIES are relative to the Bindings folder, convert them relative to this folder:
	foreach (dep ${BINDING_DEPENDENCIES})
		list (APPEND BINDINGS_DEPENDENCIES "Bindings/${dep}")
	endforeach(dep)

	if (USE_SYSTEM_LUA)
		ADD_CUSTOM_COMMAND(
			OUTPUT ${BINDING_OUTPUTS}
			COMMAND lua BindingsProcessor.lua
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/
			DEPENDS ${BINDINGS_DEPENDENCIES}
		)
	else()
		ADD_CUSTOM_COMMAND(
			OUTPUT ${BINDING_OUTPUTS}

			# Regenerate bindings:
			COMMAND luaexe BindingsProcessor.lua
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/
			DEPENDS ${BINDINGS_DEPENDENCIES} luaexe
		)
	endif()
endif()


add_executable(${CMAKE_PROJECT_NAME} ${SOURCE})

# Output the executable into the $/Server folder, so that it has access to external resources:
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY                ${CMAKE_SOURCE_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_SOURCE_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_SOURCE_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_SOURCE_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_SOURCE_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_SOURCE_DIR}/Server
)

# Make the debug executable have a "_debug" suffix
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_debug")

# Make the profiled executables have a "_profile" postfix
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES DEBUGPROFILE_POSTFIX "_debug_profile")
SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME} PROPERTIES RELEASEPROFILE_POSTFIX "_profile")


# Precompiled headers (2nd part)
if (MSVC)
	SET_TARGET_PROPERTIES(
		${CMAKE_PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/Yu\"Globals.h\""
		OBJECT_DEPENDS "$(IntDir)/$(TargetName.pch)"
	)
endif ()


if (NOT MSVC)
	target_link_libraries(${CMAKE_PROJECT_NAME}
		OSSupport HTTPServer Bindings Items Blocks Noise
		Protocol Generating WorldStorage
		Mobs Entities Simulator IncrementalRedstoneSimulator
		BlockEntities UI mbedTLS++
	)
endif ()

if (WIN32)
	target_link_libraries(${CMAKE_PROJECT_NAME} expat tolualib ws2_32.lib Psapi.lib)
endif()

if (${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
	add_flags_lnk(-L/usr/local/lib)
	add_flags_lnk(-L/usr/ports/devel)
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} luaexpat jsoncpp_lib_static mbedtls zlib sqlite lua SQLiteCpp event_core event_extra fmt::fmt)

# Create a folder for Bindings' documentation:
FILE(MAKE_DIRECTORY "Bindings/docs")
