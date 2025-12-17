
target_sources(
		${CMAKE_PROJECT_NAME} PRIVATE
		BehaviorTree.cpp

		BehaviorTree.h
		Blackboard.h
)

add_subdirectory(Behaviours)
