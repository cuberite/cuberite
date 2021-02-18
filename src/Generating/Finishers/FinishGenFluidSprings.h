
// FinishGenFluidSprings.h

#pragma once

#include "../ComposableGenerator.h"
#include "../../ProbabDistrib.h"
#include "../../Noise/Noise.h"

class cFinishGenFluidSprings :
	public cFinishGen
{
public:
	cFinishGenFluidSprings(int a_Seed, BLOCKTYPE a_Fluid, cIniFile & a_IniFile, eDimension a_Dimension);

protected:

	cNoise         m_Noise;
	cProbabDistrib m_HeightDistribution;
	BLOCKTYPE      m_Fluid;
	int            m_Chance;  ///< Chance, [0..100], that a spring will be generated in a chunk

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

	/** Tries to place a spring at the specified coords, checks neighbors. Returns true if successful. */
	bool TryPlaceSpring(cChunkDesc & a_ChunkDesc, int x, int y, int z);
} ;
