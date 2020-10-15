include_directories(${PROJECT_SOURCE_DIR}/src/)

set (SHARED_SRCS
        ${CMAKE_SOURCE_DIR}/src/WorldStorage/NamespaceSerializer.cpp
        ${PROJECT_SOURCE_DIR}/src/StringUtils.cpp
        )

set (SHARED_HDRS
        ../TestHelpers.h
        ${CMAKE_SOURCE_DIR}/src/WorldStorage/NamespaceSerializer.h
        ${PROJECT_SOURCE_DIR}/src/StringUtils.h
        )

set (SRCS
        NamespaceSerializerTest.cpp
        )


source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(NamespaceSerializerTest ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(NamespaceSerializerTest fmt::fmt)
add_test(NAME NamespaceSerializerTest COMMAND NamespaceSerializerTest)





# Put the projects into solution folders (MSVC):
set_target_properties(
        NamespaceSerializerTest
        PROPERTIES FOLDER Tests
)
