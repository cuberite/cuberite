include_directories(${PROJECT_SOURCE_DIR}/src/)

set (SHARED_SRCS
	${PROJECT_SOURCE_DIR}/src/BoundingBox.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.cpp
	${PROJECT_SOURCE_DIR}/src/OSSupport/WinStackWalker.cpp
)

set (SHARED_HDRS
	${PROJECT_SOURCE_DIR}/src/BoundingBox.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/StackTrace.h
	${PROJECT_SOURCE_DIR}/src/OSSupport/WinStackWalker.h
)

set (SRCS
	BoundingBoxTest.cpp
)


source_group("Shared" FILES ${SHARED_SRCS} ${SHARED_HDRS})
source_group("Sources" FILES ${SRCS})
add_executable(BoundingBox-exe ${SRCS} ${SHARED_SRCS} ${SHARED_HDRS})
target_link_libraries(BoundingBox-exe fmt::fmt)
add_test(NAME BoundingBox-test COMMAND BoundingBox-exe)





# Put the projects into solution folders (MSVC):
set_target_properties(
	BoundingBox-exe
	PROPERTIES FOLDER Tests
)
