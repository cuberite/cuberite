if(MSVC)
	# Make build use multiple threads under MSVC:
	add_compile_options(/MP)

	# Make build use Unicode:
	add_compile_definitions(UNICODE _UNICODE)

	# Warnings level 3 (TODO: level 4, warnings as errors):
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W3)
else()
	target_compile_options(
		${CMAKE_PROJECT_NAME} PRIVATE

		# We use a signed char (fixes #640 on RasPi)
		# TODO: specify this in code, not a compile flag:
		-fsigned-char

		# We support non-IEEE 754 FPUs so can make no guarantees about error:
		-ffast-math

		# All warnings
		# TODO: actually fix the warnings instead of disabling them
		# or at least disable on a file-level basis:
		-Wall -Wextra -Wno-unused-parameter -Wno-missing-noreturn
	)
endif()

# Allow for a forced 32-bit build under 64-bit OS:
if (FORCE_32)
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -m32)
endif()

# Have the compiler generate code specifically targeted at the current machine on Linux:
if(LINUX AND NOT NO_NATIVE_OPTIMIZATION)
	target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -march=native)
endif()
