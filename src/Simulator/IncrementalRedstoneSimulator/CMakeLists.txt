project (Cuberite)

set (SRCS
	IncrementalRedstoneSimulator.cpp
)

set (HDRS
	CommandBlockHandler.h
	DoorHandler.h
	DropSpenserHandler.h
	IncrementalRedstoneSimulator.h
	RedstoneHandler.h
	RedstoneSimulatorChunkData.h
	SolidBlockHandler.h
	RedstoneComparatorHandler.h
	RedstoneRepeaterHandler.h
	RedstoneBlockHandler.h
	RedstoneTorchHandler.h
	RedstoneWireHandler.h
	RedstoneLampHandler.h
	RedstoneToggleHandler.h
	PistonHandler.h
	SmallGateHandler.h
	NoteBlockHandler.h
	TNTHandler.h
	TrappedChestHandler.h
	TripwireHookHandler.h
	PoweredRailHandler.h
	PressurePlateHandler.h
)

add_library(IncrementalRedstoneSimulator ${SRCS} ${HDRS})
target_include_directories(IncrementalRedstoneSimulator PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" PRIVATE ../..)
target_link_libraries(IncrementalRedstoneSimulator PRIVATE fmt::fmt)
