
#pragma once

#include "Simulator.h"


class cWorld;



enum class FluidDirection
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
};





class cFluidSimulator:
	public cSimulator
{
	using Super = cSimulator;

public:

	cFluidSimulator(cWorld & a_World, BlockType a_Fluid, unsigned char a_StationaryFlowValue);

	/** Returns a unit vector in the direction the fluid is flowing or a zero-vector if not flowing. */
	virtual Vector3f GetFlowingDirection(Vector3i a_Pos);

	/** Creates a ChunkData object for the simulator to use. The simulator returns the correct object type. */
	virtual cFluidSimulatorData * CreateChunkData(void) = 0;

	bool IsFluidBlock      (BlockType a_Block) const  { return (a_Block == m_FluidBlock); }
	bool IsFluidStationary (BlockState a_Block) const;

	static bool CanWashAway(BlockState a_Block);

	bool IsSolidBlock      (BlockState a_Block);
	bool IsPassableForFluid(BlockState a_Block);

	/** Returns true if a_Meta1 is a higher fluid than a_Meta2. Takes source blocks into account. */
	bool IsHigherMeta(unsigned char a_Falloff1, unsigned char a_Falloff2);

protected:

	bool IsAllowedBlock(BlockState a_Block);

	BlockType m_FluidBlock;
	unsigned char m_StationaryFalloffValue;
};





