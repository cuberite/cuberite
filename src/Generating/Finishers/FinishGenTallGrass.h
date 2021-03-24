
// FinishGenTallGrass.h

#pragma once

#include "../ComposableGenerator.h"
#include "../../Noise/Noise.h"

class cFinishGenTallGrass :
	public cFinishGen
{
public:
	cFinishGenTallGrass(int a_Seed) : m_Noise(a_Seed), m_Seed(a_Seed) {}

protected:
	cNoise m_Noise;
	int m_Seed;

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;

	static bool CanFernGrow(EMCSBiome a_Biome);
	static bool CanLargeFernGrow(EMCSBiome a_Biome);
	static int GetBiomeDensity(EMCSBiome a_Biome);
};
