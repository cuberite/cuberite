set (SHARED_SRCS
	${PROJECT_SOURCE_DIR}/src/UUID.cpp
	${PROJECT_SOURCE_DIR}/src/StringUtils.cpp
)

set (SHARED_HDRS
	${PROJECT_SOURCE_DIR}/src/UUID.h
	${PROJECT_SOURCE_DIR}/src/StringUtils.h
)

set (SRCS
	UUIDTest.cpp
)

source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})

add_executable(UUIDTest ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(UUIDTest mbedcrypto fmt::fmt)
target_compile_definitions(UUIDTest PRIVATE TEST_GLOBALS=1)
target_include_directories(UUIDTest PRIVATE
	${PROJECT_SOURCE_DIR}/src/
	${PROJECT_SOURCE_DIR}/lib/mbedtls/include
)

add_test(NAME UUID-test COMMAND UUIDTest)


# Put the projects into solution folders (MSVC):
set_target_properties(
	UUIDTest
	PROPERTIES FOLDER Tests
)
