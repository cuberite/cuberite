target_sources(
	${CMAKE_PROJECT_NAME} PRIVATE

	ForEachSourceCallback.cpp
	IncrementalRedstoneSimulator.cpp
	RedstoneHandler.cpp

	CommandBlockHandler.h
	DoorHandler.h
	DropSpenserHandler.h
	ForEachSourceCallback.h
	HopperHandler.h
	IncrementalRedstoneSimulator.h
	RedstoneHandler.h
	RedstoneSimulatorChunkData.h
	RedstoneComparatorHandler.h
	RedstoneDataHelper.h
	RedstoneRepeaterHandler.h
	RedstoneBlockHandler.h
	RedstoneTorchHandler.h
	RedstoneWireHandler.h
	RedstoneLampHandler.h
	RedstoneToggleHandler.h
	PistonHandler.h
	SmallGateHandler.h
	NoteBlockHandler.h
	ObserverHandler.h
	TNTHandler.h
	TrappedChestHandler.h
	TripwireHookHandler.h
	PoweredRailHandler.h
	PressurePlateHandler.h
)
