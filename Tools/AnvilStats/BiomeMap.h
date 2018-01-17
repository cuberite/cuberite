
// BiomeMap.h

// Interfaces to the cBiomeMap class representing a cCallback descendant that draws a map of biomes for the world





#pragma once

#include "Callback.h"





class cBiomeMap :
	public cCallback
{
public:
	cBiomeMap(void);

	/** Saves the last region that it was processing */
	void Finish(void);

protected:
	int m_CurrentChunkX;  // Absolute chunk coords
	int m_CurrentChunkZ;
	int m_CurrentChunkOffX;  // Chunk offset from the start of the region
	int m_CurrentChunkOffZ;
	int m_CurrentRegionX;
	int m_CurrentRegionZ;
	bool m_IsCurrentRegionValid;
	char m_Biomes[16 * 32 * 16 * 32];  // Biome map of the entire current region [x + 16 * 32 * z]

	// cCallback overrides:
	virtual bool OnNewChunk(int a_ChunkX, int a_ChunkZ) override;
	virtual bool OnHeader(int a_FileOffset, unsigned char a_NumSectors, int a_Timestamp) override { return false; }
	virtual bool OnCompressedDataSizePos(int a_CompressedDataSize, int a_DataOffset, char a_CompressionMethod) override { return false; }
	virtual bool OnDecompressedData(const char * a_DecompressedNBT, int a_DataSize) override { return false; }
	virtual bool OnRealCoords(int a_ChunkX, int a_ChunkZ) override { return false; }
	virtual bool OnLastUpdate(Int64 a_LastUpdate) override { return false; }
	virtual bool OnTerrainPopulated(bool a_Populated) override { return false; }  // We don't care about "populated", the biomes are the same
	virtual bool OnBiomes(const unsigned char * a_BiomeData) override;

	void StartNewRegion(int a_RegionX, int a_RegionZ);
} ;





class cBiomeMapFactory :
	public cCallbackFactory
{
public:
	virtual ~cBiomeMapFactory();

	virtual cCallback * CreateNewCallback(void) override
	{
		return new cBiomeMap;
	}

} ;
