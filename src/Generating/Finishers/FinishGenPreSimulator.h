
// FinishGenPreSimulator.h

#pragma once

class cFinishGenPreSimulator :
	public cFinishGen
{
public:
	cFinishGenPreSimulator(bool a_PreSimulateFallingBlocks, bool a_PreSimulateWater, bool a_PreSimulateLava);

protected:

	bool m_PreSimulateFallingBlocks;
	bool m_PreSimulateWater;
	bool m_PreSimulateLava;

	/** Drops hanging sand and gravel down to the ground, recalculates heightmap */
	void CollapseSandGravel(cChunkDesc & a_ChunkDesc);

	/** For each fluid block:
	- if all surroundings are of the same fluid, makes it stationary; otherwise makes it flowing (excl. top)
	- all fluid on the chunk's edge is made flowing */
	void StationarizeFluid(
		cChunkDef::BlockTypes & a_BlockTypes,    // Block types to read and change
		cChunkDef::HeightMap & a_HeightMap,      // Height map to read
		BLOCKTYPE a_Fluid,
		BLOCKTYPE a_StationaryFluid
	);

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;
