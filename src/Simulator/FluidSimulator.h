
#pragma once

#include "Simulator.h"


class cWorld;


enum class eDirection
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
	virtual ~cFluidSimulatorData() = default;
};





class cFluidSimulator:
	public cSimulator
{
	using Super = cSimulator;

public:

	cFluidSimulator(cWorld & a_World, BLOCKTYPE a_Fluid, BLOCKTYPE a_StationaryFluid);

	/** Returns a unit vector in the direction the fluid is flowing or a zero-vector if not flowing. */
	virtual Vector3f GetFlowingDirection(Vector3i a_Pos);

	/** Creates a ChunkData object for the simulator to use. The simulator returns the correct object type. */
	virtual std::unique_ptr<cFluidSimulatorData> CreateChunkData(void) = 0;

	bool IsFluidBlock          (BLOCKTYPE a_BlockType) const { return (a_BlockType == m_FluidBlock); }
	bool IsStationaryFluidBlock(BLOCKTYPE a_BlockType) const { return (a_BlockType == m_StationaryFluidBlock); }
	bool IsAnyFluidBlock       (BLOCKTYPE a_BlockType) const { return ((a_BlockType == m_FluidBlock) || (a_BlockType == m_StationaryFluidBlock)); }

	static bool CanWashAway(BLOCKTYPE a_BlockType);

	bool IsSolidBlock      (BLOCKTYPE a_BlockType) const;
	bool IsPassableForFluid(BLOCKTYPE a_BlockType) const;

	/** Returns true if a_Meta1 is a higher fluid than a_Meta2. Takes source blocks into account. */
	bool IsHigherMeta(NIBBLETYPE a_Meta1, NIBBLETYPE a_Meta2) const;

	static constexpr NIBBLETYPE FALLING_BIT = 0x08;

protected:

	bool IsAllowedBlock(BLOCKTYPE a_BlockType) const;

	BLOCKTYPE m_FluidBlock;            // The fluid block type that needs simulating
	BLOCKTYPE m_StationaryFluidBlock;  // The fluid block type that indicates no simulation is needed
};





