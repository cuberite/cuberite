
#pragma once

#include "cWorldGenerator.h"





class cWorldGenerator_Test :
	public cWorldGenerator
{
protected:
	virtual void GenerateTerrain( cChunkPtr a_Chunk ) override;
	virtual void GenerateFoliage( cChunkPtr a_Chunk ) override;
};




