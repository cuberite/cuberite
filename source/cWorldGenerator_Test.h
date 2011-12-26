#pragma once

#include "cWorldGenerator.h"

class cWorldGenerator_Test : public cWorldGenerator
{
protected:
	virtual void GenerateTerrain( cChunk* a_Chunk );
	virtual void GenerateFoliage( cChunk* a_Chunk );
};