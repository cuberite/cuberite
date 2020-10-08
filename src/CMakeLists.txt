target_sources(
	${CMAKE_PROJECT_NAME} PRIVATE

	Resources/Cuberite.rc

	BiomeDef.cpp
	BlockArea.cpp
	BlockInfo.cpp
	BlockState.cpp
	BlockType.cpp
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
	Defines.cpp
	Enchantments.cpp
	FastRandom.cpp
	FurnaceRecipe.cpp
	Globals.cpp
	IniFile.cpp
	Inventory.cpp
	Item.cpp
	ItemGrid.cpp
	JsonUtils.cpp
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
	UUID.cpp
	VoronoiMap.cpp
	WebAdmin.cpp
	World.cpp
	main.cpp

	AllocationPool.h
	BiomeDef.h
	BlockArea.h
	BlockInServerPluginInterface.h
	BlockInfo.h
	BlockState.h
	BlockTracer.h
	BlockType.h
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
	JsonUtils.h
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
	UUID.h
	Vector3.h
	VoronoiMap.h
	WebAdmin.h
	World.h
	XMLParser.h
	main.h
)

set(FOLDERS
	Bindings BlockEntities Blocks Entities
	Generating HTTP Items mbedTLS++ Mobs Noise
	OSSupport Physics Protocol Registries Simulator
	Simulator/IncrementalRedstoneSimulator UI WorldStorage
)

# Add all child source directories:
foreach(folder ${FOLDERS})
	add_subdirectory(${folder})
endforeach(folder)

file(WRITE "${CMAKE_BINARY_DIR}/include/Globals.h"
	"/* This file allows Globals.h to be included with an absolute path */\n#include \"${PROJECT_SOURCE_DIR}/src/Globals.h\"\n")

configure_file("BuildInfo.h.cmake" "${CMAKE_BINARY_DIR}/include/BuildInfo.h")
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}/include/" ".")

# Generate AllFiles.lst for CheckBasicStyle.lua
get_target_property(ALL_FILES ${CMAKE_PROJECT_NAME} SOURCES)
foreach(FILE ${ALL_FILES})
	# target_sources converts to absolute but CheckBasicStyle expects relative
	file(RELATIVE_PATH RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" ${FILE})

	# Convert CMake list into newline-delimited string
	set(ALL_FILES_AS_LINES "${ALL_FILES_AS_LINES}${RELATIVE}\n")
endforeach()
file(WRITE AllFiles.lst "${ALL_FILES_AS_LINES}")

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

# Create a folder for Bindings' documentation:
FILE(MAKE_DIRECTORY "Bindings/docs")
make_symlink("${CMAKE_CURRENT_SOURCE_DIR}/Bindings/docs" "${CMAKE_BINARY_DIR}/Server/BindingsDocs")
