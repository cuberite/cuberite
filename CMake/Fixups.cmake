function(emit_fixups)
	if(NOT MSVC AND "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "arm")
		# mbed TLS uses the frame pointer's register in inline assembly for its bignum implementation:
		# https://tls.mbed.org/kb/development/arm-thumb-error-r7-cannot-be-used-in-asm-here
		target_compile_options(mbedcrypto PRIVATE -fomit-frame-pointer)
	endif()
endfunction()
