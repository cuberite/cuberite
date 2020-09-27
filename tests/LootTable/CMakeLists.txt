find_package(Threads REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/src/)

# Define individual test executables:

add_executable(LootTableParseTest
        ${CMAKE_SOURCE_DIR}/src/LootTableParser.cpp
        ${CMAKE_SOURCE_DIR}/src/StringUtils.cpp
        ${CMAKE_SOURCE_DIR}/src/JsonUtils.cpp
        ${CMAKE_SOURCE_DIR}/src/Registries/ItemTags.cpp
        ${CMAKE_SOURCE_DIR}/src/Enchantments.cpp
        ${CMAKE_SOURCE_DIR}/src/Item.cpp
        ${CMAKE_SOURCE_DIR}/src/Item.cpp
        ${CMAKE_SOURCE_DIR}/src/Color.cpp
        ${CMAKE_SOURCE_DIR}/src/Defines.cpp
        ${CMAKE_SOURCE_DIR}/src/ItemGrid.cpp
        ${CMAKE_SOURCE_DIR}/src/BiomeDef.cpp
        ${CMAKE_SOURCE_DIR}/src/FastRandom.cpp
        ${CMAKE_SOURCE_DIR}/src/Noise/Noise.cpp
        ${CMAKE_SOURCE_DIR}/src/OSSupport/File.cpp
        ${CMAKE_SOURCE_DIR}/src/OSSupport/CriticalSection.cpp
        ${CMAKE_SOURCE_DIR}/src/WorldStorage/FireworksSerializer.cpp
        Stubs.cpp
        LootTableParseTest.cpp
        ../TestHelpers.h
        )


target_link_libraries(jsoncpp_lib)

link_dependencies(LootTableParseTest)
target_include_directories(LootTableParseTest SYSTEM PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/mbedtls/include
        ${CMAKE_SOURCE_DIR}/lib
        ${CMAKE_BINARY_DIR}/include
        ${CMAKE_SOURCE_DIR}/lib/TCLAP/include
        )

# Extra files for BlockTypePalette test:
# file (COPY
#        ../../Server/Protocol/1.13/base.btp.txt
#        ../../Server/Protocol/UpgradeBlockTypePalette.txt
#        DESTINATION ./
#        )







# Define individual tests:

add_test(NAME LootTableParseTest        COMMAND LootTableParseTest)





# Put all the tests into a solution folder (MSVC):
set_target_properties(
        LootTableParseTest
        PROPERTIES FOLDER Tests/LootTableTest
)
