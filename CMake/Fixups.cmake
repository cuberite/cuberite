function(emit_fixups)
	if(NOT MSVC AND "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm")
		# mbed TLS uses the frame pointer's register in inline assembly for its bignum implementation:
		# https://tls.mbed.org/kb/development/arm-thumb-error-r7-cannot-be-used-in-asm-here
		target_compile_options(mbedcrypto PRIVATE -fomit-frame-pointer)
	endif()

	if(UNIX)
		execute_process(COMMAND ldd OUTPUT_VARIABLE LDD_OUTPUT ERROR_VARIABLE LDD_OUTPUT)
		if (LDD_OUTPUT MATCHES musl)
			# Bring musl stack size in line with other platforms:
			target_link_options(${CMAKE_PROJECT_NAME} PRIVATE "-Wl,-z,stack-size=1048576")
		endif()
	endif()
endfunction()
