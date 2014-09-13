#pragma once
#include "Chunk.h"





// fwd:
class cBiomeGen;





/** Abstract interface for getting biome data for chunks. */
class ChunkSource
{
public:
	virtual ~ChunkSource() {}

	/** Fills the a_DestChunk with the biomes for the specified coords.
	It is expected to be thread-safe and re-entrant. Usually QThread::idealThreadCount() threads are used. */
	virtual void getChunkBiomes(int a_ChunkX, int a_ChunkZ, ChunkPtr a_DestChunk) = 0;
};






class BioGenSource :
	public ChunkSource
{
public:
	/** Constructs a new BioGenSource based on the biome generator given.
	Takes ownership of a_BiomeGen */
	BioGenSource(cBiomeGen * a_BiomeGen);

	virtual void getChunkBiomes(int a_ChunkX, int a_ChunkZ, ChunkPtr a_DestChunk) override;

protected:
	std::shared_ptr<cBiomeGen> m_BiomeGen;
	QMutex m_Mtx;
};




class AnvilSource :
	public ChunkSource
{
public:
	// TODO

	virtual void getChunkBiomes(int a_ChunkX, int a_ChunkZ, ChunkPtr a_DestChunk) override;
};




