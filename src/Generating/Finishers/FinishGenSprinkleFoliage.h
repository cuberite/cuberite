
// FinishGenSprinkleFoliage.h

#pragma once

#include "../ComposableGenerator.h"

class cFinishGenSprinkleFoliage :
	public cFinishGen
{
public:
	cFinishGenSprinkleFoliage(int a_Seed, int a_MaxCactusHeight, int a_MaxSugarcaneHeight):
		m_Noise(a_Seed),
		m_Seed(a_Seed),
		m_MaxCactusHeight(a_MaxCactusHeight),
		m_MaxSugarcaneHeight(a_MaxSugarcaneHeight)
	{
	}
protected:
	cNoise m_Noise;
	int    m_Seed;
	int    m_MaxCactusHeight;
	int    m_MaxSugarcaneHeight;

	/** Tries to place sugarcane at the coords specified, returns true if successful, updates the top variable (hence the & a_RefY) */
	bool TryAddSugarcane(cChunkDesc & a_ChunkDesc, int a_RelX, HEIGHTTYPE & a_RelY, int a_RelZ);

	/** Tries to place cactus at the coords specified, returns true if successful, updates the top variable (hence the & a_RefY) */
	bool TryAddCactus(cChunkDesc & a_ChunkDesc, int a_RelX, HEIGHTTYPE & a_RelY, int a_RelZ);

	// Returns true is the specified biome is a desert or its variant
	static bool IsDesertVariant(EMCSBiome a_biome);

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;
