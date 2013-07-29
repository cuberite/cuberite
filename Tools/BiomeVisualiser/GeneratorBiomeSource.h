
// GeneratorBiomeSource.h

// Declares the cGeneratorBiomeSource that adapts a cBiomeGen into a cBiomeSource

#include "../source/Generating/BioGen.h"
#include "BiomeSource.h"





class cGeneratorBiomeSource :
	public cBiomeSource
{
public:
	cGeneratorBiomeSource(cBiomeGen * a_Generator) : m_Generator(a_Generator) {}  // Takes ownership of the generator ptr
	
	~cGeneratorBiomeSource()
	{
		delete m_Generator;
	}
	
	// cBiomeSource overrides:
	virtual eAvailability GetBiome(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_Biomes) override
	{
		m_Generator->GenBiomes(a_ChunkX, a_ChunkZ, a_Biomes);
		return baNow;
	}
	
	virtual void HintViewArea(int a_MinChunkX, int a_MaxChunkX, int a_MinChunkZ, int a_MaxChunkZ) override
	{
		// Nothing needed
	}
	
protected:
	cBiomeGen * m_Generator;
} ;




