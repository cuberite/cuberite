
cmake_minimum_required (VERSION 2.6)
project (cryptopp)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCRYPTOPP_DISABLE_ASM")
endif()
include_directories ("${PROJECT_SOURCE_DIR}/../../src/")

add_library(cryptopp rsa integer queue secblock misc randpool pch asn oids modarith nbtheory sha algparam fips140 pssr aes hrtimer cryptlib filters pubkey algebra simple pkcspad iterhash emsa2 eprecomp cpu rijndael)
