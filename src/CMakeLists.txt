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
	BlockState.cpp
	BlockTypePalette.cpp
	BlockTypeRegistry.cpp
	BrewingRecipes.cpp
	Broadcaster.cpp
	BoundingBox.cpp
	ByteBuffer.cpp
	ChatColor.cpp
	Chunk.cpp
	ChunkData.cpp
	ChunkGeneratorThread.cpp
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
	PalettedBlockArea.cpp
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
	BlockState.h
	BlockTracer.h
	BlockTypePalette.h
	BlockTypeRegistry.h
	BrewingRecipes.h
	BoundingBox.h
	BuildInfo.h.cmake
	ByteBuffer.h
	ChatColor.h
	Chunk.h
	ChunkData.h
	ChunkDataCallback.h
	ChunkDef.h
	ChunkGeneratorThread.h
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
	OpaqueWorld.h
	OverridesSettingsRepository.h
	PalettedBlockArea.h
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

file(WRITE "${CMAKE_BINARY_DIR}/include/Globals.h"
	"/* This file allows Globals.h to be included with an absolute path */\n#include \"${PROJECT_SOURCE_DIR}/Globals.h\"\n")

include_directories("${CMAKE_BINARY_DIR}/include")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../lib/sqlite")
include_directories ("${CMAKE_CURRENT_SOURCE_DIR}/../lib/SQLiteCpp/include")
include_directories (SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../lib/TCLAP/include")

configure_file("BuildInfo.h.cmake" "${CMAKE_BINARY_DIR}/include/BuildInfo.h")

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
	RUNTIME_OUTPUT_DIRECTORY                ${CMAKE_BINARY_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_DEBUG          ${CMAKE_BINARY_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_RELEASE        ${CMAKE_BINARY_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     ${CMAKE_BINARY_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_DEBUGPROFILE   ${CMAKE_BINARY_DIR}/Server
	RUNTIME_OUTPUT_DIRECTORY_RELEASEPROFILE ${CMAKE_BINARY_DIR}/Server
)





# Create a symbolic link from ${orig} to ${link}
# If the orig and link point to the same thing, does nothing (-> in-source build)
# If ${link} already exists, does nothing.
function(make_symlink orig link)
	# If both are the same, or link already exists, bail out:
	if ("${orig}" STREQUAL "${link}")
		return()
	endif()
	if (EXISTS ${link})
		return()
	endif()

	# Create the symlink (platform-dependent):
	message("Creating symlink, orig = ${orig}; link = ${link}")
	if (CMAKE_HOST_UNIX)
		set(command ln -s ${orig} ${link})
	else()
		file(TO_NATIVE_PATH "${orig}" orig)
		file(TO_NATIVE_PATH "${link}" link)
		if (IS_DIRECTORY ${orig})
			set(command cmd.exe /c mklink /j ${link} ${orig})
		else()
			set(command cmd.exe /c mklink /h ${link} ${orig})
		endif()
	endif()

	execute_process(
		COMMAND ${command}
		RESULT_VARIABLE result
		ERROR_VARIABLE stderr
		OUTPUT_VARIABLE stdout
	)

	if (NOT ${result} EQUAL 0)
		message(FATAL_ERROR "Could not create symbolic link for: ${link} --> ${orig}: ${stderr} ${stdout}")
	endif()
endfunction(make_symlink)





# Populate the output folder with symlinks to the Server folder's internals:
set(symlinks
	Install
	Plugins
	Prefabs
	Protocol
	webadmin
	brewing.txt
	crafting.txt
	favicon.png
	furnace.txt
	items.ini
	monsters.ini
	README.txt
)
message("Creating output folder and symlinks...")
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Server")
foreach (symlink ${symlinks})
	make_symlink("${CMAKE_CURRENT_SOURCE_DIR}/../Server/${symlink}" "${CMAKE_BINARY_DIR}/Server/${symlink}")
endforeach(symlink)
make_symlink("${CMAKE_CURRENT_SOURCE_DIR}/../BACKERS"                           "${CMAKE_BINARY_DIR}/Server/BACKERS")
make_symlink("${CMAKE_CURRENT_SOURCE_DIR}/../CONTRIBUTORS"                      "${CMAKE_BINARY_DIR}/Server/CONTRIBUTORS")
make_symlink("${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE"                           "${CMAKE_BINARY_DIR}/Server/LICENSE")
make_symlink("${CMAKE_CURRENT_SOURCE_DIR}/../Server/Install/ThirdPartyLicenses" "${CMAKE_BINARY_DIR}/Server/ThirdPartyLicenses")




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
make_symlink("${CMAKE_CURRENT_SOURCE_DIR}/Bindings/docs" "${CMAKE_BINARY_DIR}/Server/BindingsDocs")


# For MSVC, set the startup project to Cuberite, and the debugger dir:
if (MSVC)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
	set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Server")
endif()
