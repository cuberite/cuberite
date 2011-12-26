#pragma once

class cChunk;
class cWorldGenerator
{
public:
	cWorldGenerator();
	~cWorldGenerator();

	virtual void GenerateChunk( cChunk* a_Chunk );

protected:

	virtual void GenerateTerrain( cChunk* a_Chunk );
	virtual void GenerateFoliage( cChunk* a_Chunk );

};