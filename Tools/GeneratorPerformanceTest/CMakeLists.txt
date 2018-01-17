project(GeneratorPerformanceTest)

include_directories(../../src/Generating)
include_directories(../../src)
include_directories(../../lib)

add_executable(GeneratorPerformanceTest GeneratorPerformanceTest.cpp ../../src/StringUtils ../../src/MCLogger ../../src/Log ../../src/BlockID ../../src/Noise ../../src/Enchantments ../../src/BlockArea)

target_link_libraries(GeneratorPerformanceTest Generating)

