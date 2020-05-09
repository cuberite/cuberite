# TODO these should be in the submodules
# Under Windows, we need Lua as DLL; on *nix we need it linked statically:
if (WIN32)
	target_compile_definitions(lua PRIVATE LUA_BUILD_AS_DLL)
endif()

# Let Lua use additional checks on its C API. This is only compiled into Debug builds:
target_compile_definitions(lua PRIVATE LUA_USE_APICHECK)

if(NOT MSVC AND "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm")
	# mbed TLS uses the frame pointer's register in inline assembly for its bignum implementation:
	# https://tls.mbed.org/kb/development/arm-thumb-error-r7-cannot-be-used-in-asm-here
	target_compile_options(mbedcrypto PRIVATE -fomit-frame-pointer)
endif()
