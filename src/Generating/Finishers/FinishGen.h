
// FinishGen.h

/* Interfaces to the various finishing generators:
	- cFinishGenSnow
	- cFinishGenIce
	- cFinishGenSprinkleFoliage
	- cFinishGenLilyPads
	- cFinishGenBottomLava
	- cFinishGenPreSimulator
	- cFinishGenDeadBushes
*/





#pragma once

#include "../ComposableGenerator.h"
#include "../../Noise/Noise.h"
#include "../../ProbabDistrib.h"
#include "../../Mobs/Monster.h"





class cFinishGenSnow :
	public cFinishGen
{
protected:
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cFinishGenIce :
	public cFinishGen
{
protected:
	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cFinishGenVines :
	public cFinishGen
{
public:
	cFinishGenVines(int a_Seed, int a_Level) :
		m_Noise(a_Seed),
		m_Level(a_Level)
	{
	}

	bool IsJungleVariant(EMCSBiome a_Biome);

protected:
	cNoise m_Noise;
	int    m_Level;

	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
};





class cFinishGenSoulsandRims :
	public cFinishGen
{
public:
	cFinishGenSoulsandRims(int a_Seed) :
		m_Noise(a_Seed)
	{
	}

protected:
	cNoise m_Noise;

	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;





class cFinishGenBottomLava :
	public cFinishGen
{
public:
	cFinishGenBottomLava(int a_Level) :
		m_Level(a_Level)
	{
	}

	int GetLevel(void) const { return m_Level; }
protected:
	int m_Level;

	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;
