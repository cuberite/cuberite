
cmake_minimum_required (VERSION 2.6)

project (ProtoProxy)



macro(add_flags_cxx FLAGS)
	set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}         ${FLAGS}")
	set(CMAKE_C_FLAGS           "${CMAKE_C_FLAGS}           ${FLAGS}")
	set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   ${FLAGS}")
	set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}     ${FLAGS}")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${FLAGS}")
	set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   ${FLAGS}")
endmacro()




# Add the preprocessor macros used for distinguishing between debug and release builds (CMake does this automatically for MSVC):
if (NOT MSVC)
	set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -D_DEBUG")
	set(CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}     -D_DEBUG")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG")
	set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   -DNDEBUG")
endif()



if(MSVC)
	# Make build use multiple threads under MSVC:
	add_flags_cxx("/MP")

	# Make release builds use link-time code generation:
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")
	set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   /GL")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}    /LTCG")
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG")
	set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} /LTCG")
elseif(APPLE)
	#on os x clang adds pthread for us but we need to add it for gcc
	if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		add_flags_cxx("-pthread")
	endif()
else()
	# Let gcc / clang know that we're compiling a multi-threaded app:
	add_flags_cxx("-pthread")
endif()




# Use static CRT in MSVC builds:
if (MSVC)
	string(REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	string(REPLACE "/MD" "/MT" CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}")
	string(REPLACE "/MDd" "/MTd" CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}")
	string(REPLACE "/MDd" "/MTd" CMAKE_C_FLAGS_DEBUG     "${CMAKE_C_FLAGS_DEBUG}")
endif()




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


# Include the libraries:
file(GLOB CRYPTOPP_SRC "../../lib/CryptoPP/*.cpp")
file(GLOB CRYPTOPP_HDR "../../lib/CryptoPP/*.h")
flatten_files(CRYPTOPP_SRC)
flatten_files(CRYPTOPP_HDR)
source_group("CryptoPP" FILES ${CRYPTOPP_SRC} ${CRYPTOPP_HDR})

file(GLOB ZLIB_SRC "../../lib/zlib/*.c")
file(GLOB ZLIB_HDR "../../lib/zlib/*.h")
flatten_files(ZLIB_SRC)
flatten_files(ZLIB_HDR)
source_group("ZLib" FILES ${ZLIB_SRC} ${ZLIB_HDR})


# Include the shared files:
set(SHARED_SRC
	../../src/ByteBuffer.cpp
	../../src/StringUtils.cpp
	../../src/Log.cpp
	../../src/MCLogger.cpp
)
set(SHARED_HDR
	../../src/ByteBuffer.h
	../../src/StringUtils.h
	../../src/Log.h
	../../src/MCLogger.h
)
set(SHARED_OSS_SRC
	../../src/OSSupport/CriticalSection.cpp
	../../src/OSSupport/File.cpp
	../../src/OSSupport/IsThread.cpp
	../../src/OSSupport/Timer.cpp
)
set(SHARED_OSS_HDR
	../../src/OSSupport/CriticalSection.h
	../../src/OSSupport/File.h
	../../src/OSSupport/IsThread.h
	../../src/OSSupport/Timer.h
)
flatten_files(SHARED_SRC)
flatten_files(SHARED_HDR)
flatten_files(SHARED_OSS_SRC)
flatten_files(SHARED_OSS_HDR)
source_group("Shared" FILES ${SHARED_SRC} ${SHARED_HDR})
source_group("Shared\\OSSupport" FILES ${SHARED_OSS_SRC} ${SHARED_OSS_HDR})



# Include the main source files:
set(SOURCES
	Connection.cpp
	Globals.cpp
	ProtoProxy.cpp
	Server.cpp
)
set(HEADERS
	Connection.h
	Globals.h
	Server.h
)
source_group("" FILES ${SOURCES} ${HEADERS})

add_executable(ProtoProxy
	${SOURCES}
	${HEADERS}
	${SHARED_SRC}
	${SHARED_HDR}
	${SHARED_OSS_SRC}
	${SHARED_OSS_HDR}
	${CRYPTOPP_SRC}
	${CRYPTOPP_HDR}
	${ZLIB_SRC}
	${ZLIB_HDR}
)

