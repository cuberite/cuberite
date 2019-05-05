
SET (SRCS
	Noise.cpp
)

SET (HDRS
	InterpolNoise.h
	Noise.h
	OctavedNoise.h
	RidgedNoise.h
)

if(NOT MSVC)
	add_library(Noise ${SRCS} ${HDRS})
	target_link_libraries(Noise fmt::fmt OSSupport)
endif()
