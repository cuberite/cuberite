include_directories(${PROJECT_SOURCE_DIR}/src/)

set (SHARED_SRCS
        ${PROJECT_SOURCE_DIR}/src/BlockInfo.cpp
        ${PROJECT_SOURCE_DIR}/src/Defines.cpp
        ${PROJECT_SOURCE_DIR}/src/Items/ItemHandler.cpp
        ${PROJECT_SOURCE_DIR}/src/StringUtils.cpp
        ${PROJECT_SOURCE_DIR}/src/FastRandom.cpp
        ${PROJECT_SOURCE_DIR}/src/Enchantments.cpp
        ${PROJECT_SOURCE_DIR}/src/Color.cpp

        ${PROJECT_SOURCE_DIR}/src/Item.cpp
        )

set (SHARED_HDRS
        ../TestHelpers.h
        ${PROJECT_SOURCE_DIR}/src/Items/ItemHandler.h
        ${PROJECT_SOURCE_DIR}/src/StringUtils.h
        ${PROJECT_SOURCE_DIR}/src/Item.h
        )

set (SRCS
        ItemTest.cpp
        Stubs.cpp
        )


source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(Items-exe ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(Items-exe fmt::fmt jsoncpp_static mbedtls SQLiteCpp)
if (WIN32)
    target_link_libraries(Items-exe ws2_32)
endif()

add_test(NAME Items-test COMMAND Items-exe)





# Put the projects into solution folders (MSVC):
set_target_properties(
        Items-exe
        PROPERTIES FOLDER Tests
)
