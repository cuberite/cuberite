
// FinishGenGlowStone.h

#pragma once

#include "../ComposableGenerator.h"
#include "../../Noise/Noise.h"

class cFinishGenGlowStone :
	public cFinishGen
{
public:
	cFinishGenGlowStone(int a_Seed) :
		m_Noise(a_Seed),
		m_Seed(a_Seed)
	{
	}

protected:
	cNoise m_Noise;
	int    m_Seed;

	void TryPlaceGlowstone(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelY, int a_RelZ, int a_Size, int a_NumStrings);
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
};
