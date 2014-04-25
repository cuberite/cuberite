cmake_minimum_required (VERSION 2.8.2)
project (MCServer)

include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/")
include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/jsoncpp/include")
include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/polarssl/include")

set(FOLDERS OSSupport HTTPServer Items Blocks Protocol Generating) 
set(FOLDERS ${FOLDERS} WorldStorage Mobs Entities Simulator UI BlockEntities Generating/Prefabs)


if (NOT MSVC)

	# Bindings need to reference other folders, so they are done here instead
	
	# lib dependencies are not included
	
	set(BINDING_DEPENDECIES
		tolua
		${CMAKE_CURRENT_SOURCE_DIR}/Bindings/virtual_method_hooks.lua
		${CMAKE_CURRENT_SOURCE_DIR}/Bindings/AllToLua.pkg
		Bindings/LuaFunctions.h
		Bindings/LuaWindow.h
		Bindings/Plugin.h
		Bindings/PluginLua.h
		Bindings/PluginManager.h
		Bindings/WebPlugin.h
		BiomeDef.h
		BlockArea.h
		BlockEntities/BlockEntity.h
		BlockEntities/BlockEntityWithItems.h
		BlockEntities/ChestEntity.h
		BlockEntities/DispenserEntity.h
		BlockEntities/DropSpenserEntity.h
		BlockEntities/DropperEntity.h
		BlockEntities/FurnaceEntity.h
		BlockEntities/HopperEntity.h
		BlockEntities/JukeboxEntity.h
		BlockEntities/NoteEntity.h
		BlockEntities/SignEntity.h
		BlockEntities/MobHeadEntity.h
		BlockEntities/FlowerPotEntity.h
		BlockID.h
		BoundingBox.h
		ChatColor.h
		ChunkDef.h
		ClientHandle.h
		CraftingRecipes.h
		Cuboid.h
		Defines.h
		Enchantments.h
		Entities/Effects.h
		Entities/Entity.h
		Entities/Floater.h
		Entities/Pawn.h
		Entities/Painting.h
		Entities/Pickup.h
		Entities/Player.h
		Entities/ProjectileEntity.h
		Entities/TNTEntity.h
		Entities/ExpOrb.h
		Entities/HangingEntity.h
		Entities/ItemFrame.h
		Generating/ChunkDesc.h
		Group.h
		Inventory.h
		Item.h
		ItemGrid.h
		Mobs/Monster.h
		OSSupport/File.h
		Root.h
		Server.h
		StringUtils.h
		Tracer.h
		UI/Window.h
		Vector3.h
		WebAdmin.h
		World.h
	)
	
	include_directories(Bindings)
	include_directories(.)
	
	ADD_CUSTOM_COMMAND(
		# add any new generated bindings here
		OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/Bindings.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/Bindings.h
	
		# command execuded to regerate bindings
		COMMAND tolua -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/
	
		# add any new generation dependencies here
		DEPENDS ${BINDING_DEPENDECIES}
	)
	#add cpp files here
	add_library(Bindings
		Bindings/Bindings
		Bindings/DeprecatedBindings
		Bindings/LuaChunkStay
		Bindings/LuaState
		Bindings/LuaWindow
		Bindings/ManualBindings
		Bindings/Plugin
		Bindings/PluginLua
		Bindings/PluginManager
		Bindings/WebPlugin
	)

	target_link_libraries(Bindings lua sqlite tolualib)

	#clear file
	file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/BindingDependecies.txt)
	foreach(dependecy ${BINDING_DEPENDECIES})
		#write each dependecy on a seperate line
		file(APPEND ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/BindingDependecies.txt "${dependecy}\n")
	endforeach()

	set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "Bindings.cpp Bindings.h")

	foreach(folder ${FOLDERS})
		add_subdirectory(${folder})
	endforeach(folder)

	file(GLOB SOURCE
		"*.cpp"
		"*.h"
	)

	list(REMOVE_ITEM SOURCE "${PROJECT_SOURCE_DIR}/StackWalker.cpp" "${PROJECT_SOURCE_DIR}/LeakFinder.cpp")

	# If building a windows version, but not using MSVC, add the resources directly to the makefile:
	if (WIN32)
		FILE(GLOB ResourceFiles
			"Resources/*.rc"
		)
		list(APPEND SOURCE "${ResourceFiles}")
	endif()


else ()
	# MSVC-specific handling: Put all files into one project, separate by the folders:

	# Generate the Bindings if they don't exist:
	if (NOT EXISTS "${PROJECT_SOURCE_DIR}/Bindings/Bindings.cpp")
		message("Bindings.cpp not found, generating now")
		set(tolua_executable ${PROJECT_SOURCE_DIR}/Bindings/tolua++.exe)
		execute_process(
			COMMAND ${tolua_executable} -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Bindings
		)
	endif()

	# Get all files in this folder:
	file(GLOB_RECURSE SOURCE
		"*.cpp"
		"*.h"
		"*.pkg"
	)
	source_group("" FILES ${SOURCE})

	# Add all subfolders as solution-folders:
	list(APPEND FOLDERS "Resources")
	list(APPEND FOLDERS "Bindings")
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
		includefolder(${folder})
	endforeach(folder)

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

set(EXECUTABLE MCServer)

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
	target_link_libraries(${EXECUTABLE} OSSupport HTTPServer Bindings Items Blocks)
	target_link_libraries(${EXECUTABLE} Protocol Generating Generating_Prefabs WorldStorage)
	target_link_libraries(${EXECUTABLE} Mobs Entities Simulator UI BlockEntities)
endif ()
if (WIN32)
	target_link_libraries(${EXECUTABLE} expat tolualib ws2_32.lib Psapi.lib)
endif()
target_link_libraries(${EXECUTABLE} md5 luaexpat iniFile jsoncpp polarssl zlib lua sqlite)
