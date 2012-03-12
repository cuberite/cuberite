
// WGFlat.h

// Interfaces to the cWGFlat class representing the flat world generator





#pragma once

#include "cWorldGenerator.h"





class cWGFlat :
	public cWorldGenerator
{
	typedef cWorldGenerator super;
	
protected:
	int m_Height;
	
	virtual void GenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities) override;
	virtual void PostGenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ) override;
	
public:
	cWGFlat(cWorld * a_World);
} ;




