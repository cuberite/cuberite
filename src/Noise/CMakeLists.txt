project (Cuberite)

SET (SRCS
	Noise.cpp
)

SET (HDRS
	InterpolNoise.h
	Noise.h
	OctavedNoise.h
	RidgedNoise.h
)

add_library(Noise ${SRCS} ${HDRS})
target_include_directories(Noise PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" PRIVATE ..)
target_link_libraries(Noise PRIVATE fmt::fmt OSSupport)
