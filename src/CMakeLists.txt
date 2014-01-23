
cmake_minimum_required (VERSION 2.8.2)
project (MCServer)

include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/")
include_directories (SYSTEM "${PROJECT_SOURCE_DIR}/../lib/jsoncpp/include")

set(FOLDERS OSSupport HTTPServer Items Blocks Protocol Generating) 
set(FOLDERS ${FOLDERS} WorldStorage Mobs Entities Simulator UI BlockEntities)



if (NOT MSVC)

	#Bindings needs to reference other folders so are done here
	
	#lib dependecies are not included
	
	set(BINDING_DEPENDECIES ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/virtual_method_hooks.lua)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} ${CMAKE_CURRENT_SOURCE_DIR}/Bindings/AllToLua.pkg)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} ChunkDef.h BiomeDef.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} OSSupport/File.h Bindings/LuaFunctions.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Bindings/PluginManager.h Bindings/Plugin.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Bindings/PluginLua.h Bindings/WebPlugin.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Bindings/LuaWindow.h BlockID.h StringUtils.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Defines.h ChatColor.h ClientHandle.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Entities/Entity.h Entities/Floater.h )
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Entities/Pawn.h Entities/Player.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Entities/Pickup.h Entities/ProjectileEntity.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Entities/TNTEntity.h Entities/Effects.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Server.h World.h Inventory.h Enchantments.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Item.h ItemGrid.h BlockEntities/BlockEntity.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} BlockEntities/BlockEntityWithItems.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} BlockEntities/ChestEntity.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} BlockEntities/DropSpenserEntity.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} BlockEntities/DispenserEntity.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} BlockEntities/DropperEntity.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} BlockEntities/FurnaceEntity.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} BlockEntities/HopperEntity.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} BlockEntities/JukeboxEntity.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} BlockEntities/NoteEntity.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} BlockEntities/SignEntity.h WebAdmin.h Root.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Vector3f.h Vector3d.h Vector3i.h Matrix4f.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} Cuboid.h BoundingBox.h Tracer.h Group.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} BlockArea.h Generating/ChunkDesc.h)
	set(BINDING_DEPENDECIES ${BINDING_DEPENDECIES} CraftingRecipes.h UI/Window.h Mobs/Monster.h)
	
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
	add_library(Bindings Bindings/PluginManager Bindings/LuaState Bindings/WebPlugin Bindings/Bindings Bindings/ManualBindings Bindings/LuaWindow Bindings/Plugin Bindings/PluginLua Bindings/WebPlugin)

	target_link_libraries(Bindings lua sqlite tolualib)

	foreach(folder ${FOLDERS})
		add_subdirectory(${folder})
	endforeach(folder)

	file(GLOB SOURCE
		"*.cpp"
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

	# Generate the Bindings if they don't exist:
	if (NOT EXISTS "${PROJECT_SOURCE_DIR}/Bindings/Bindings.cpp")
		message("Bindings.cpp not found, generating now")
		set(tolua_executable ${PROJECT_SOURCE_DIR}/Bindings/tolua++.exe)
		execute_process(
			COMMAND ${tolua_executable} -L virtual_method_hooks.lua -o Bindings.cpp -H Bindings.h AllToLua.pkg
			WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/Bindings
		)
	endif()

	# Add all subfolders as solution-folders:
	list(APPEND FOLDERS "Resources")
	list(APPEND FOLDERS "Bindings")
	function(includefolder PATH)
		FILE(GLOB FOLDER_FILES
			"${PATH}/*.cpp"
			"${PATH}/*.h"
			"${PATH}/*.rc"
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
	target_link_libraries(${EXECUTABLE} Protocol Generating WorldStorage)
	target_link_libraries(${EXECUTABLE} Mobs Entities Simulator UI BlockEntities)
endif ()
if (WIN32)
	target_link_libraries(${EXECUTABLE} expat tolualib ws2_32.lib Psapi.lib)
endif()
target_link_libraries(${EXECUTABLE} md5 luaexpat iniFile jsoncpp cryptopp zlib lua sqlite)
