
# This script includes PolarSSL, if not already included.
# It is needed for when multiple projects reference PolarSSL.

if(NOT TARGET mbedtls)
	message("including mbedtls")
	set(ENABLE_TESTING OFF CACHE BOOL "Disable tests")
	set(ENABLE_PROGRAMS OFF CACHE BOOL "Disable programs")
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/mbedtls/ ${CMAKE_CURRENT_BINARY_DIR}/lib/mbedtls EXCLUDE_FROM_ALL)
endif()
