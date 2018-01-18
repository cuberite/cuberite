
#pragma once

#include "Simulator.h"


class cWorld;


enum Direction
{
	X_PLUS,
	X_MINUS,
	Y_PLUS,
	Y_MINUS,
	Z_PLUS,
	Z_MINUS,
	NONE
};





/** This is a base class for all fluid simulator data classes.
Needed so that cChunk can properly delete instances of fluid simulator data, no matter what simulator it's using. */
class cFluidSimulatorData
{
public:
	virtual ~cFluidSimulatorData() {}
} ;





class cFluidSimulator :
	public cSimulator
{
	typedef cSimulator super;

public:
	cFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid);

	// cSimulator overrides:
	virtual bool IsAllowedBlock(BLOCKTYPE a_BlockType) override;

	/** Gets the flowing direction. If a_Over is true also the block over the current block affects the direction (standard) */
	virtual Direction GetFlowingDirection(int a_X, int a_Y, int a_Z, bool a_Over = true);

	/** Creates a ChunkData object for the simulator to use. The simulator returns the correct object type. */
	virtual cFluidSimulatorData * CreateChunkData(void) { return nullptr; }

	bool IsFluidBlock          (BLOCKTYPE a_BlockType) const { return (a_BlockType == m_FluidBlock); }
	bool IsStationaryFluidBlock(BLOCKTYPE a_BlockType) const { return (a_BlockType == m_StationaryFluidBlock); }
	bool IsAnyFluidBlock       (BLOCKTYPE a_BlockType) const { return ((a_BlockType == m_FluidBlock) || (a_BlockType == m_StationaryFluidBlock)); }

	static bool CanWashAway(BLOCKTYPE a_BlockType);

	bool IsSolidBlock      (BLOCKTYPE a_BlockType);
	bool IsPassableForFluid(BLOCKTYPE a_BlockType);

	/** Returns true if a_Meta1 is a higher fluid than a_Meta2. Takes source blocks into account. */
	bool IsHigherMeta(NIBBLETYPE a_Meta1, NIBBLETYPE a_Meta2);

protected:
	BLOCKTYPE m_FluidBlock;            // The fluid block type that needs simulating
	BLOCKTYPE m_StationaryFluidBlock;  // The fluid block type that indicates no simulation is needed
} ;





