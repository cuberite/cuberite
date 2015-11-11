
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





// Forward-declare the shared pointers to subgenerator classes:
class cBiomeGen;
class cTerrainShapeGen;
class cTerrainHeightGen;
class cTerrainCompositionGen;
class cFinishGen;
typedef SharedPtr<cBiomeGen>              cBiomeGenPtr;
typedef SharedPtr<cTerrainShapeGen>       cTerrainShapeGenPtr;
typedef SharedPtr<cTerrainHeightGen>      cTerrainHeightGenPtr;
typedef SharedPtr<cTerrainCompositionGen> cTerrainCompositionGenPtr;
typedef SharedPtr<cFinishGen>             cFinishGenPtr;





/** The interface that a biome generator must implement
A biome generator takes chunk coords on input and outputs an array of biome indices for that chunk on output.
The output array is sequenced in the same way as the MapChunk packet's biome data.
*/
class cBiomeGen
{
public:
	virtual ~cBiomeGen() {}  // Force a virtual destructor in descendants
	
	/** Generates biomes for the given chunk */
	virtual void GenBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) = 0;
	
	/** Reads parameters from the ini file, prepares generator for use. */
	virtual void InitializeBiomeGen(cIniFile & a_IniFile) {}

	/** Creates the correct BiomeGen descendant based on the ini file settings and the seed provided.
	a_CacheOffByDefault gets set to whether the cache should be disabled by default.
	Used in BiomeVisualiser, too.
	Implemented in BioGen.cpp! */
	static cBiomeGenPtr CreateBiomeGen(cIniFile & a_IniFile, int a_Seed, bool & a_CacheOffByDefault);
} ;





/** The interface that a terrain shape generator must implement
A terrain shape generator takes chunk coords on input and outputs a 3D array of "shape" for that chunk. The shape here
represents the distinction between air and solid; there's no representation of Water since that is added by the
composition geenrator.
The output array is indexed [y + 256 * z + 16 * 256 * x], so that it's fast to later compose a single column of the terrain,
which is the dominant operation following the shape generation.
The generator may request biome information from the underlying BiomeGen, it may even request information for
other chunks than the one it's currently generating (neighbors - for averaging)
*/
class cTerrainShapeGen
{
public:
	virtual ~cTerrainShapeGen() {}  // Force a virtual destructor in descendants
	
	/** Generates the shape for the given chunk */
	virtual void GenShape(int a_ChunkX, int a_ChunkZ, cChunkDesc::Shape & a_Shape) = 0;
	
	/** Reads parameters from the ini file, prepares generator for use. */
	virtual void InitializeShapeGen(cIniFile & a_IniFile) {}
	
	/** Creates the correct TerrainShapeGen descendant based on the ini file settings and the seed provided.
	a_BiomeGen is the underlying biome generator, some shape generators may depend on it providing additional biomes data around the chunk
	a_CacheOffByDefault gets set to whether the cache should be disabled by default
	Implemented in ShapeGen.cpp!
	*/
	static cTerrainShapeGenPtr CreateShapeGen(cIniFile & a_IniFile, cBiomeGenPtr a_BiomeGen, int a_Seed, bool & a_CacheOffByDefault);
} ;





/** The interface that is used to query terrain height from the shape generator.
Usually the structure generators require only the final heightmap, and generating the whole shape only to
consume the heightmap is wasteful, so this interface is used instead; it has a cache implemented over it so
that data is retained. */
class cTerrainHeightGen
{
public:
	virtual ~cTerrainHeightGen() {}  // Force a virtual destructor in descendants

	/** Retrieves the heightmap for the specified chunk. */
	virtual void GenHeightMap(int a_ChunkX, int a_ChunkZ, cChunkDef::HeightMap & a_HeightMap) = 0;

	/** Initializes the generator, reading its parameters from the INI file. */
	virtual void InitializeHeightGen(cIniFile & a_IniFile) {}

	/** Returns the height at the specified column.
	The default implementation calls GenHeightMap(), and then queries the heightmap.
	Descendants may provide a better-performing method. */
	virtual HEIGHTTYPE GetHeightAt(int a_BlockX, int a_BlockZ)
	{
		int chunkX, chunkZ;
		cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, chunkX, chunkZ);
		cChunkDef::HeightMap heightMap;
		GenHeightMap(chunkX, chunkZ, heightMap);
		return cChunkDef::GetHeight(heightMap, a_BlockX - chunkX * cChunkDef::Width, a_BlockZ - chunkZ * cChunkDef::Width);
	}

	/** Creates a cTerrainHeightGen descendant based on the INI file settings. */
	static cTerrainHeightGenPtr CreateHeightGen(cIniFile & a_IniFile, cBiomeGenPtr a_BiomeGen, int a_Seed, bool & a_CacheOffByDefault);
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
	
	/** Generates the chunk's composition into a_ChunkDesc, using the terrain shape provided in a_Shape.
	Is expected to fill a_ChunkDesc's heightmap with the data from a_Shape. */
	virtual void ComposeTerrain(cChunkDesc & a_ChunkDesc, const cChunkDesc::Shape & a_Shape) = 0;
	
	/** Reads parameters from the ini file, prepares generator for use. */
	virtual void InitializeCompoGen(cIniFile & a_IniFile) {}
	
	/** Creates the correct TerrainCompositionGen descendant based on the ini file settings and the seed provided.
	a_BiomeGen is the underlying biome generator, some composition generators may depend on it providing additional biomes around the chunk
	a_ShapeGen is the underlying shape generator, some composition generators may depend on it providing additional shape around the chunk
	*/
	static cTerrainCompositionGenPtr CreateCompositionGen(cIniFile & a_IniFile, cBiomeGenPtr a_BiomeGen, cTerrainShapeGenPtr a_ShapeGen, int a_Seed);
} ;





/** The interface that a finisher must implement
Finisher implements changes to the chunk after the rough terrain has been generated.
Examples of finishers are trees, snow, ore, lilypads and others.
Note that a worldgenerator may contain multiple finishers, chained one after another.
Also note that previously we used to distinguish between a structuregen and a finisher; this distinction is
no longer relevant, all structure generators are considered finishers now (#398)
*/
class cFinishGen
{
public:
	virtual ~cFinishGen() {}  // Force a virtual destructor in descendants
	
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) = 0;
} ;

typedef std::list<cFinishGenPtr> cFinishGenList;





class cComposableGenerator :
	public cChunkGenerator::cGenerator
{
	typedef cChunkGenerator::cGenerator super;
	
public:
	cComposableGenerator(cChunkGenerator & a_ChunkGenerator);
	
	// cChunkGenerator::cGenerator overrides:
	virtual void Initialize(cIniFile & a_IniFile) override;
	virtual void GenerateBiomes(int a_ChunkX, int a_ChunkZ, cChunkDef::BiomeMap & a_BiomeMap) override;
	virtual void DoGenerate(int a_ChunkX, int a_ChunkZ, cChunkDesc & a_ChunkDesc) override;

protected:
	// The generator's composition:
	/** The biome generator. */
	cBiomeGenPtr m_BiomeGen;

	/** The terrain shape generator. */
	cTerrainShapeGenPtr m_ShapeGen;

	/** The terrain composition generator. */
	cTerrainCompositionGenPtr m_CompositionGen;

	/** The cache for the heights of the composited terrain. */
	cTerrainHeightGenPtr m_CompositedHeightCache;

	/** The finisher generators, in the order in which they are applied. */
	cFinishGenList m_FinishGens;
	
	
	/** Reads the BiomeGen settings from the ini and initializes m_BiomeGen accordingly */
	void InitBiomeGen(cIniFile & a_IniFile);
	
	/** Reads the ShapeGen settings from the ini and initializes m_ShapeGen accordingly */
	void InitShapeGen(cIniFile & a_IniFile);
	
	/** Reads the CompositionGen settings from the ini and initializes m_CompositionGen accordingly */
	void InitCompositionGen(cIniFile & a_IniFile);
	
	/** Reads the finishers from the ini and initializes m_FinishGens accordingly */
	void InitFinishGens(cIniFile & a_IniFile);
} ;




