
if(NOT TARGET polarssl)
	message("including polarssl")
	add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/polarssl/ ${CMAKE_CURRENT_BINARY_DIR}/lib/polarssl EXCLUDE_FROM_ALL )
endif()
