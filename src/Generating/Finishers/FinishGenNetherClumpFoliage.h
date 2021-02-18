// FinishGenNetherClumpFoliage.h

#pragma once

#include "../ComposableGenerator.h"
#include "../../Noise/Noise.h"

class cFinishGenNetherClumpFoliage :
	public cFinishGen
{
  public:
	cFinishGenNetherClumpFoliage(int a_Seed) :
		m_Noise(a_Seed),
		m_Seed(a_Seed)
	{
	}

  protected:
	cNoise m_Noise;
	int    m_Seed;

	void TryPlaceClump(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_Block);
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;
