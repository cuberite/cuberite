#pragma once

#include "../Defines.h"
#include "ChunkDef.h"



// fwd:
class cIniFile;
class cChunkDesc;





/** The interface that all chunk generators must implement to provide the generated chunks.
Also a static factory that creates the descendants based on INI file settings.
The cChunkGeneratorThread uses this interface to generate chunks for a single world.
Ths calls to generate chunks are synchronous - they don't return until the chunk is fully generated. */
class cChunkGenerator
{
public:
	virtual ~cChunkGenerator() {}  // Force a virtual destructor

	/** Called to initialize the generator on server startup.
	Descendants should call Super::Initialize() before initializing themselves. */
	virtual void Initialize(cIniFile & a_IniFile);

	/** Generates the biomes for the specified chunk.
	Used by the world loader if biomes failed loading. */
	virtual void GenerateBiomes(cChunkCoords a_ChunkCoords, cChunkDef::BiomeMap & a_BiomeMap) = 0;

	/** Returns the biome at the specified coords.
	Used by ChunkMap if an invalid chunk is queried for biome.
	The default implementation uses GenerateBiomes(). */
	virtual EMCSBiome GetBiomeAt(int a_BlockX, int a_BlockZ);

	/** Does the actual chunk generation.
	Descendants need to override this and generate into a_ChunkDesc. */
	virtual void Generate(cChunkDesc & a_ChunkDesc) = 0;

	/** Returns the seed that was read from the INI file. */
	int GetSeed(void) const { return m_Seed; }

	/** Creates and initializes the entire generator based on the settings in the INI file.
	Initializes the generator, so that it can be used immediately after this call returns. */
	static std::unique_ptr<cChunkGenerator> CreateFromIniFile(cIniFile & a_IniFile);


protected:

	/** The main seed, read from the INI file, used for the entire generator. */
	int m_Seed;

	/** The dimension, read from the INI file. */
	eDimension m_Dimension;
};




