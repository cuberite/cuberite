project (AnvilStats)

# Set include paths to the used libraries:
include_directories("../../lib")
include_directories("../../src")



function(flatten_files arg1)
	set(res "")
	foreach(f ${${arg1}})
		get_filename_component(f ${f} ABSOLUTE)
		list(APPEND res ${f})
	endforeach()
	set(${arg1} "${res}" PARENT_SCOPE)
endfunction()

add_subdirectory(../../lib/zlib ${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_FILES_DIRECTORY}/lib/zlib)

# Include the shared files:
set(SHARED_SRC
	../../src/ByteBuffer.cpp
	../../src/StringUtils.cpp
	../../src/LoggerListeners.cpp
	../../src/Logger.cpp
	../../src/WorldStorage/FastNBT.cpp
	../BiomeVisualiser/BiomeColors.cpp
)

set(SHARED_HDR
	../../src/ByteBuffer.h
	../../src/StringUtils.h
	../../src/LoggerListeners.h
	../../src/Logger.h
	../../src/WorldStorage/FastNBT.h
	../BiomeVisualiser/BiomeColors.h
)

set(SHARED_OSS_SRC
	../../src/OSSupport/CriticalSection.cpp
	../../src/OSSupport/Event.cpp
	../../src/OSSupport/File.cpp
	../../src/OSSupport/GZipFile.cpp
	../../src/OSSupport/IsThread.cpp
)

set(SHARED_OSS_HDR
	../../src/OSSupport/CriticalSection.h
	../../src/OSSupport/Event.h
	../../src/OSSupport/File.h
	../../src/OSSupport/GZipFile.h
	../../src/OSSupport/IsThread.h
)

flatten_files(SHARED_SRC)
flatten_files(SHARED_HDR)
flatten_files(SHARED_OSS_SRC)
flatten_files(SHARED_OSS_HDR)
source_group("Shared" FILES ${SHARED_SRC} ${SHARED_HDR})
source_group("Shared\\OSSupport" FILES ${SHARED_OSS_SRC} ${SHARED_OSS_HDR})



# Include the main source files:
set(SOURCES
	AnvilStats.cpp
	BiomeMap.cpp
	ChunkExtract.cpp
	Globals.cpp
	HeightBiomeMap.cpp
	HeightMap.cpp
	ImageComposingCallback.cpp
	Processor.cpp
	SpringStats.cpp
	Statistics.cpp
	Utils.cpp
)

set(HEADERS
	BiomeMap.h
	Callback.h
	ChunkExtract.h
	Globals.h
	HeightBiomeMap.h
	HeightMap.h
	ImageComposingCallback.h
	Processor.h
	SpringStats.h
	Statistics.h
	Utils.h

	AnvilStats.txt
)

source_group("" FILES ${SOURCES} ${HEADERS})

add_definitions(-DNBT_RESERVE_SIZE=10000)

add_executable(AnvilStats
	${SOURCES}
	${HEADERS}
	${SHARED_SRC}
	${SHARED_HDR}
	${SHARED_OSS_SRC}
	${SHARED_OSS_HDR}
)

target_link_libraries(AnvilStats zlib)

include(../../SetFlags.cmake)
set_exe_flags(AnvilStats)





# Under MSVC we need to enlarge the default stack size for the executable:
if (MSVC)
	get_target_property(TEMP AnvilStats LINK_FLAGS)
	if (TEMP STREQUAL "TEMP-NOTFOUND")
	  SET(TEMP "") # set to empty string
		message("LINKER_FLAGS not found")
	else ()
	  SET(TEMP "${TEMP} ") # a space to cleanly separate from existing content
		message("LINKER_FLAGS: ${LINKER_FLAGS}")
	endif ()
	# append our values
	SET(TEMP "${TEMP}/STACK:16777216")
	set_target_properties(AnvilStats PROPERTIES LINK_FLAGS ${TEMP})
endif ()
