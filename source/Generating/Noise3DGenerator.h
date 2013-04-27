
// Noise3DGenerator.h

// Generates terrain using 3D noise, rather than composing. Is a test.




#pragma once

#include "ChunkGenerator.h"
#include "ChunkDesc.h"
#include "../Noise.h"





class cNoise3DGenerator :
	public cChunkGenerator::cGenerator
{
	typedef cChunkGenerator::cGenerator super;
	
public:
	cNoise3DGenerator(cChunkGenerator & a_ChunkGenerator);
	virtual ~cNoise3DGenerator();
	
	virtual void Initialize(cWorld * a_World, cIniFile & a_IniFile) override;
	virtual void GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void DoGenerate(int a_ChunkX, int a_ChunkZ, cChunkDesc & a_ChunkDesc) override;
	
protected:
	cNoise m_Noise1;
	cNoise m_Noise2;
} ;




