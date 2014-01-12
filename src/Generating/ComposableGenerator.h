
// ComposableGenerator.h

// Declares the cComposableGenerator class representing the chunk generator that takes the composition approach to generating chunks

/*
Generating works by composing several algorithms:
Biome, TerrainHeight, TerrainComposition, Ores, Structures and SmallFoliage
Each algorithm may be chosen from a pool of available algorithms in the same class and combined with others,
based on user's preferences in the world.ini.
See http://forum.mc-server.org/showthread.php?tid=409 for details.
*/





#pragma once

#include "ChunkGenerator.h"
#include "ChunkDesc.h"





// fwd: Noise3DGenerator.h
class cNoise3DComposable;

// fwd: DistortedHeightmap.h
class cDistortedHeightmap;





/** The interface that a biome generator must implement
A biome generator takes chunk coords on input and outputs an array of biome indices for that chunk on output.
The output array is sequenced in the same way as the MapChunk packet's biome data.
*/
class cBiomeGen
{
public:
	virtual ~cBiomeGen() {}  // Force a virtual destructor in descendants
	
	/// Generates biomes for the given chunk
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) = 0;
	
	/// Reads parameters from the ini file, prepares generator for use.
	virtual void InitializeBiomeGen(cIniFile & a_IniFile) {}

	/// Creates the correct BiomeGen descendant based on the ini file settings and the seed provided.
	/// a_CacheOffByDefault gets set to whether the cache should be disabled by default
	/// Used in BiomeVisualiser, too.
	/// Implemented in BioGen.cpp!
	static cBiomeGen * CreateBiomeGen(cIniFile & a_IniFile, int a_Seed, bool & a_CacheOffByDefault);

} ;





/** The interface that a terrain height generator must implement
A terrain height generator takes chunk coords on input and outputs an array of terrain heights for that chunk.
The output array is sequenced in the same way as the BiomeGen's biome data.
The generator may request biome information from the underlying BiomeGen, it may even request information for
other chunks than the one it's currently generating (possibly neighbors - for averaging)
*/
class cTerrainHeightGen
{
public:
	virtual ~cTerrainHeightGen() {}  // Force a virtual destructor in descendants
	
	/// Generates heightmap for the given chunk
	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap) = 0;
	
	/// Reads parameters from the ini file, prepares generator for use.
	virtual void InitializeHeightGen(cIniFile & a_IniFile) {}
	
	/** Creates the correct TerrainHeightGen descendant based on the ini file settings and the seed provided.
	a_BiomeGen is the underlying biome generator, some height generators may depend on it to generate more biomes
	a_CacheOffByDefault gets set to whether the cache should be disabled by default
	Implemented in HeiGen.cpp!
	*/
	static cTerrainHeightGen * CreateHeightGen(cIniFile & a_IniFile, cBiomeGen & a_BiomeGen, int a_Seed, bool & a_CacheOffByDefault);
} ;





/** The interface that a terrain composition generator must implement
Terrain composition takes chunk coords on input and outputs the blockdata for that entire chunk, along with
the list of entities. It is supposed to make use of the underlying TerrainHeightGen and BiomeGen for that purpose,
but it may request information for other chunks than the one it's currently generating from them.
*/
class cTerrainCompositionGen
{
public:
	virtual ~cTerrainCompositionGen() {}  // Force a virtual destructor in descendants
	
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc) = 0;
	
	/// Reads parameters from the ini file, prepares generator for use.
	virtual void InitializeCompoGen(cIniFile & a_IniFile) {}
	
	/** Creates the correct TerrainCompositionGen descendant based on the ini file settings and the seed provided.
	a_BiomeGen is the underlying biome generator, some composition generators may depend on it to generate more biomes
	a_HeightGen is the underlying height generator, some composition generators may depend on it providing additional values
	*/
	static cTerrainCompositionGen * CreateCompositionGen(cIniFile & a_IniFile, cBiomeGen & a_BiomeGen, cTerrainHeightGen & a_HeightGen, int a_Seed);
} ;





/** The interface that a structure generator must implement
Structures are generated after the terrain composition took place. It should modify the blocktype data to account
for whatever structures the generator is generating.
Note that ores are considered structures too, at least from the interface point of view.
Also note that a worldgenerator may contain multiple structure generators, one for each type of structure
*/
class cStructureGen
{
public:
	virtual ~cStructureGen() {}  // Force a virtual destructor in descendants
	
	virtual void GenStructures(cChunkDesc & a_ChunkDesc) = 0;
} ;

typedef std::list<cStructureGen *> cStructureGenList;





/** The interface that a finisher must implement
Finisher implements small additions after all structures have been generated.
*/
class cFinishGen
{
public:
	virtual ~cFinishGen() {}  // Force a virtual destructor in descendants
	
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) = 0;
} ;

typedef std::list<cFinishGen *> cFinishGenList;





class cComposableGenerator :
	public cChunkGenerator::cGenerator
{
	typedef cChunkGenerator::cGenerator super;
	
public:
	cComposableGenerator(cChunkGenerator & a_ChunkGenerator);
	virtual ~cComposableGenerator();
	
	virtual void Initialize(cIniFile & a_IniFile) override;
	virtual void GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void DoGenerate(int a_ChunkX, int a_ChunkZ, cChunkDesc & a_ChunkDesc) override;

protected:
	// The generation composition:
	cBiomeGen *              m_BiomeGen;
	cTerrainHeightGen *      m_HeightGen;
	cTerrainCompositionGen * m_CompositionGen;
	cStructureGenList        m_StructureGens;
	cFinishGenList           m_FinishGens;
	
	// Generators underlying the caches:
	cBiomeGen *              m_UnderlyingBiomeGen;
	cTerrainHeightGen *      m_UnderlyingHeightGen;
	cTerrainCompositionGen * m_UnderlyingCompositionGen;
	
	
	/// Reads the biome gen settings from the ini and initializes m_BiomeGen accordingly
	void InitBiomeGen(cIniFile & a_IniFile);
	
	/// Reads the HeightGen settings from the ini and initializes m_HeightGen accordingly
	void InitHeightGen(cIniFile & a_IniFile);
	
	/// Reads the CompositionGen settings from the ini and initializes m_CompositionGen accordingly
	void InitCompositionGen(cIniFile & a_IniFile);
	
	/// Reads the structures to generate from the ini and initializes m_StructureGens accordingly
	void InitStructureGens(cIniFile & a_IniFile);
	
	/// Reads the finishers from the ini and initializes m_FinishGens accordingly
	void InitFinishGens(cIniFile & a_IniFile);
} ;




