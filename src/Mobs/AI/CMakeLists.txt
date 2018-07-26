
include_directories ("${PROJECT_SOURCE_DIR}/../")

SET (SRCS
	BehaviorTree.cpp
)


SET (HDRS
	BehaviorTree.h
	Blackboard.h
)

if(NOT MSVC)
	add_library(MobAI ${SRCS} ${HDRS})
	target_link_libraries(MobAI fmt::fmt)
endif()
