
// ChunkDef.h

// Interfaces to helper types for chunk definitions. Most modules want to include this instead of cChunk.h





#pragma once

#include "Vector3i.h"





/** This is really only a placeholder to be used in places where we need to "make up" a chunk's Y coord.
It will help us when the new chunk format comes out and we need to patch everything up for compatibility.
*/
#define ZERO_CHUNK_Y 0

// Used to smoothly convert to new axis ordering. One will be removed when deemed stable.
#define AXIS_ORDER_YZX 1	// Original (1.1-)
#define AXIS_ORDER_XZY 2	// New (1.2+)
#define AXIS_ORDER AXIS_ORDER_XZY





// fwd
class cBlockEntity;
class cEntity;
class cClientHandle;
class cBlockEntity;

typedef std::list<cEntity *>        cEntityList;
typedef std::list<cBlockEntity *>   cBlockEntityList;




/// The datatype used by blockdata
typedef char BLOCKTYPE;

/// The datatype used by nibbledata (meta, light, skylight)
typedef unsigned char NIBBLETYPE;

/// The type used by the heightmap
typedef unsigned char HEIGHTTYPE;





/** Biome IDs
The first batch corresponds to the clientside biomes, used by MineCraft.
BiomeIDs over 255 are used by MCServer internally and are translated to MC biomes before sending them to client
*/
enum EMCSBiome
{
	biOcean            = 0,
	biPlains           = 1,
	biDesert           = 2,
	biExtremeHills     = 3,
	biForest           = 4,
	biTaiga            = 5,
	biSwampland        = 6,
	biRiver            = 7,
	biHell             = 8,  // same as Nether
	biNether           = 8,
	biSky              = 9,
	biFrozenOcean      = 10,
	biFrozenRiver      = 11,
	biIcePlains        = 12,
	biTundra           = 12,  // same as Ice Plains
	biIceMountains     = 13,
	biMushroomIsland   = 14,
	biMushroomShore    = 15,
	biBeach            = 16,
	biDesertHills      = 17,
	biForestHills      = 18,
	biTaigaHills       = 19,
	biExtremeHillsEdge = 20,
	biJungle           = 21,
	biJungleHills      = 22,
	
	// Automatically capture the maximum biome value into biMaxBiome:
	biNumBiomes,  // True number of biomes, since they are zero-based
	biMaxBiome = biNumBiomes - 1,  // The maximum biome value
} ;





/// Constants used throughout the code, useful typedefs and utility functions
class cChunkDef
{
public:
	static const int Width = 16;
	static const int Height = 256;
	static const int NumBlocks = Width * Height * Width;
	static const int BlockDataSize = NumBlocks * 2 + (NumBlocks / 2); // 2.5 * numblocks
	
	// Offsets to individual components in the joined blockdata array
	static const int MetaOffset     = NumBlocks;
	static const int LightOffset    = MetaOffset + NumBlocks / 2;
	static const int SkyLightOffset = LightOffset + NumBlocks / 2;

	static const unsigned int INDEX_OUT_OF_RANGE = 0xffffffff;

	/// The type used for any heightmap operations and storage; idx = x + Width * z
	typedef HEIGHTTYPE HeightMap[Width * Width];
	
	/** The type used for any biomemap operations and storage inside MCServer, 
	using MCServer biomes (need not correspond to client representation!)
	idx = x + Width * z  // Need to verify this with the protocol spec, currently unknown!
	*/
	typedef EMCSBiome BiomeMap[Width * Width];
	
	/// The type used for block type operations and storage, AXIS_ORDER ordering
	typedef BLOCKTYPE BlockTypes[NumBlocks];
	
	/// The type used for block data in nibble format, AXIS_ORDER ordering
	typedef NIBBLETYPE BlockNibbles[NumBlocks / 2];


	/// Converts absolute block coords into relative (chunk + block) coords:
	inline static void AbsoluteToRelative(/* in-out */ int & a_X, int & a_Y, int & a_Z, /* out */ int & a_ChunkX, int & a_ChunkZ )
	{
		BlockToChunk(a_X, a_Y, a_Z, a_ChunkX, a_ChunkZ);

		a_X = a_X - a_ChunkX * Width;
		a_Z = a_Z - a_ChunkZ * Width;
	}
	
	
	/// Converts absolute block coords to chunk coords:
	inline static void BlockToChunk( int a_X, int a_Y, int a_Z, int & a_ChunkX, int & a_ChunkZ )
	{
		(void)a_Y;
		a_ChunkX = a_X / Width;
		if ((a_X < 0) && (a_X % Width != 0))
		{
			a_ChunkX--;
		}
		a_ChunkZ = a_Z / cChunkDef::Width;
		if ((a_Z < 0) && (a_Z % Width != 0))
		{
			a_ChunkZ--;
		}
	}


	inline static unsigned int MakeIndex(int x, int y, int z )
	{
		if( x < cChunkDef::Width && x > -1 && y < cChunkDef::Height && y > -1 && z < cChunkDef::Width && z > -1 )
		{
			return MakeIndexNoCheck(x, y, z);
		}
		return INDEX_OUT_OF_RANGE;
	}


	inline static unsigned int MakeIndexNoCheck(int x, int y, int z)
	{
		#if AXIS_ORDER == AXIS_ORDER_XZY
			// For some reason, NOT using the Horner schema is faster. Weird.
			return x + (z * cChunkDef::Width) + (y * cChunkDef::Width * cChunkDef::Width); // 1.2 is XZY
		#elif AXIS_ORDER == AXIS_ORDER_YZX
			return y + (z * cChunkDef::Width) + (x * cChunkDef::Height * cChunkDef::Width); // 1.1 is YZX
		#endif
	}


	inline static Vector3i IndexToCoordinate( unsigned int index )
	{
		#if AXIS_ORDER == AXIS_ORDER_XZY
			return Vector3i(								// 1.2
				index % cChunkDef::Width,						// X
				index / (cChunkDef::Width * cChunkDef::Width),		// Y
				(index / cChunkDef::Width) % cChunkDef::Width		// Z
				);
		#elif AXIS_ORDER == AXIS_ORDER_YZX
			return Vector3i(								// 1.1
				index / (cChunkDef::Height * cChunkDef::Width),		// X
				index % cChunkDef::Height,						// Y
				(index / cChunkDef::Height) % cChunkDef::Width		// Z
				);
		#endif
	}


	inline static void SetBlock(BLOCKTYPE * a_BlockTypes, int a_X, int a_Y, int a_Z, BLOCKTYPE a_Type)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Y >= 0) && (a_Y < Height));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		a_BlockTypes[MakeIndexNoCheck(a_X, a_Y, a_Z)] = a_Type;
	}
	
	
	inline static BLOCKTYPE GetBlock(const BLOCKTYPE * a_BlockTypes, int a_X, int a_Y, int a_Z)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Y >= 0) && (a_Y < Height));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		return a_BlockTypes[MakeIndexNoCheck(a_X, a_Y, a_Z)];
	}
	
	
	inline static int GetHeight(const HeightMap & a_HeightMap, int a_X, int a_Z)
	{
		return a_HeightMap[a_X + Width * a_Z];
	}
	
	
	inline static void SetHeight(HeightMap & a_HeightMap, int a_X, int a_Z, unsigned char a_Height)
	{
		a_HeightMap[a_X + Width * a_Z] = a_Height;
	}
	
	
	inline static EMCSBiome GetBiome(const BiomeMap & a_BiomeMap, int a_X, int a_Z)
	{
		return a_BiomeMap[a_X + Width * a_Z];
	}
	
	
	inline static void SetBiome(BiomeMap & a_BiomeMap, int a_X, int a_Z, EMCSBiome a_Biome)
	{
		a_BiomeMap[a_X + Width * a_Z] = a_Biome;
	}
	
	
	static NIBBLETYPE GetNibble(const NIBBLETYPE * a_Buffer, int a_BlockIdx)
	{
		if ((a_BlockIdx > -1) && (a_BlockIdx < cChunkDef::NumBlocks))
		{
			return (a_Buffer[a_BlockIdx / 2] >> ((a_BlockIdx & 1) * 4)) & 0x0f;
		}
		return 0;
	}
	
	
	static NIBBLETYPE GetNibble(const NIBBLETYPE * a_Buffer, int x, int y, int z)
	{
		if ((x < Width) && (x > -1) && (y < Height) && (y > -1) && (z < Width) && (z > -1))
		{
			int Index = MakeIndexNoCheck(x, y, z);
			return (a_Buffer[Index / 2] >> ((Index & 1) * 4)) & 0x0f;
		}
		return 0;
	}


	static void SetNibble(NIBBLETYPE * a_Buffer, int a_BlockIdx, NIBBLETYPE a_Nibble)
	{
		if ((a_BlockIdx > -1) && (a_BlockIdx < cChunkDef::NumBlocks))
		{
			a_Buffer[a_BlockIdx / 2] = (
				(a_Buffer[a_BlockIdx / 2] & (0xf0 >> ((a_BlockIdx & 1) * 4))) |  // The untouched nibble
				((a_Nibble & 0x0f) << ((a_BlockIdx & 1) * 4))  // The nibble being set
			);
		}
	}
	
	
	static void SetNibble(NIBBLETYPE * a_Buffer, int x, int y, int z, NIBBLETYPE a_Nibble)
	{
		if ((x < cChunkDef::Width) && (x > -1) && (y < cChunkDef::Height) && (y > -1) && (z < cChunkDef::Width) && (z > -1))
		{
			int Index = MakeIndexNoCheck(x, y, z);
			a_Buffer[Index / 2] = (
				(a_Buffer[Index / 2] & (0xf0 >> ((Index & 1) * 4))) |  // The untouched nibble
				((a_Nibble & 0x0f) << ((Index & 1) * 4))  // The nibble being set
			);
		}
	}


	inline static char GetNibble(const NIBBLETYPE * a_Buffer, const Vector3i & a_BlockPos )
	{
		return GetNibble( a_Buffer, a_BlockPos.x, a_BlockPos.y, a_BlockPos.z );
	}
	
	
	inline static void SetNibble(NIBBLETYPE * a_Buffer, const Vector3i & a_BlockPos, char a_Value )
	{
		SetNibble( a_Buffer, a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, a_Value );
	}

} ;





/** Interface class used for getting data out of a chunk using the GetAllData() function.
Implementation must use the pointers immediately and NOT store any of them for later use
The virtual methods are called in the same order as they're declared here.
*/
class cChunkDataCallback abstract
{
public:
	/** Called before any other callbacks to inform of the current coords 
	(only in processes where multiple chunks can be processed, such as cWorld::ForEachChunkInRect()). 
	If false is returned, the chunk is skipped.
	*/
	virtual bool Coords(int a_ChunkX, int a_ChunkZ) { return true; };
	
	/// Called once to provide heightmap data
	virtual void HeightMap(const cChunkDef::HeightMap * a_HeightMap) {};
	
	/// Called once to provide biome data
	virtual void BiomeData    (const cChunkDef::BiomeMap * a_BiomeMap) {};
	
	/// Called once to export block types
	virtual void BlockTypes   (const BLOCKTYPE * a_Type) {};
	
	/// Called once to export block meta
	virtual void BlockMeta    (const NIBBLETYPE * a_Meta) {};
	
	/// Called once to let know if the chunk lighting is valid. Return value is used to control if BlockLight() and BlockSkyLight() are called next (if true)
	virtual bool LightIsValid(bool a_IsLightValid) {return true; };

	/// Called once to export block light
	virtual void BlockLight   (const NIBBLETYPE * a_Meta) {};
	
	/// Called once to export sky light
	virtual void BlockSkyLight(const NIBBLETYPE * a_Meta) {};
	
	/// Called for each entity in the chunk
	virtual void Entity(cEntity * a_Entity) {};
	
	/// Called for each blockentity in the chunk
	virtual void BlockEntity(cBlockEntity * a_Entity) {};
} ;





/** A simple implementation of the cChunkDataCallback interface that collects all block data into a single buffer
*/
class cChunkDataCollector :
	public cChunkDataCallback
{
public:

	// Must be char instead of BLOCKTYPE or NIBBLETYPE, because it houses both.
	char m_BlockData[cChunkDef::BlockDataSize];

protected:

	virtual void BlockTypes(const BLOCKTYPE * a_BlockTypes) override
	{
		memcpy(m_BlockData, a_BlockTypes, sizeof(cChunkDef::BlockTypes));
	}


	virtual void BlockMeta(const NIBBLETYPE * a_BlockMeta) override
	{
		memcpy(m_BlockData + cChunkDef::NumBlocks, a_BlockMeta, cChunkDef::NumBlocks / 2);
	}


	virtual void BlockLight(const NIBBLETYPE * a_BlockLight) override
	{
		memcpy(m_BlockData + 3 * cChunkDef::NumBlocks / 2, a_BlockLight, cChunkDef::NumBlocks / 2);
	}


	virtual void BlockSkyLight(const NIBBLETYPE * a_BlockSkyLight) override
	{
		memcpy(m_BlockData + 2 * cChunkDef::NumBlocks, a_BlockSkyLight, cChunkDef::NumBlocks / 2);
	}
} ;





/** A simple implementation of the cChunkDataCallback interface that collects all block data into a separate buffers
*/
class cChunkDataSeparateCollector :
	public cChunkDataCallback
{
public:

	cChunkDef::BlockTypes   m_BlockTypes;
	cChunkDef::BlockNibbles m_BlockMetas;
	cChunkDef::BlockNibbles m_BlockLight;
	cChunkDef::BlockNibbles m_BlockSkyLight;

protected:

	virtual void BlockTypes(const BLOCKTYPE * a_BlockTypes) override
	{
		memcpy(m_BlockTypes, a_BlockTypes, sizeof(m_BlockTypes));
	}


	virtual void BlockMeta(const NIBBLETYPE * a_BlockMeta) override
	{
		memcpy(m_BlockMetas, a_BlockMeta, sizeof(m_BlockMetas));
	}


	virtual void BlockLight(const NIBBLETYPE * a_BlockLight) override
	{
		memcpy(m_BlockLight, a_BlockLight, sizeof(m_BlockLight));
	}


	virtual void BlockSkyLight(const NIBBLETYPE * a_BlockSkyLight) override
	{
		memcpy(m_BlockSkyLight, a_BlockSkyLight, sizeof(m_BlockSkyLight));
	}
} ;





/** Interface class used for comparing clients of two chunks.
Used primarily for entity moving while both chunks are locked.
*/
class cClientDiffCallback
{
public:
	/// Called for clients that are in Chunk1 and not in Chunk2,
	virtual void Removed(cClientHandle * a_Client) = 0;
	
	/// Called for clients that are in Chunk2 and not in Chunk1.
	virtual void Added(cClientHandle * a_Client) = 0;
} ;





struct sSetBlock
{
	int x, y, z;
	int ChunkX, ChunkZ;
	BLOCKTYPE BlockType;
	NIBBLETYPE BlockMeta;

	sSetBlock( int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta );  // absolute block position
	sSetBlock(int a_ChunkX, int a_ChunkZ, int a_X, int a_Y, int a_Z, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) :
		ChunkX(a_ChunkX), ChunkZ(a_ChunkZ),
		x(a_X), y(a_Y), z(a_Z),
		BlockType(a_BlockType),
		BlockMeta(a_BlockMeta)
	{}
};

typedef std::list<sSetBlock> sSetBlockList;
typedef std::vector<sSetBlock> sSetBlockVector;





class cChunkCoords
{
public:
	int m_ChunkX;
	int m_ChunkY;
	int m_ChunkZ;
	
	cChunkCoords(int a_ChunkX, int a_ChunkY, int a_ChunkZ) : m_ChunkX(a_ChunkX), m_ChunkY(a_ChunkY), m_ChunkZ(a_ChunkZ) {}
	
	bool operator == (const cChunkCoords & a_Other) const
	{
		return ((m_ChunkX == a_Other.m_ChunkX) && (m_ChunkY == a_Other.m_ChunkY) && (m_ChunkZ == a_Other.m_ChunkZ));
	}
} ;

typedef std::list<cChunkCoords> cChunkCoordsList;





/// Interface class used as a callback for operations that involve chunk coords
class cChunkCoordCallback
{
public:
	virtual void Call(int a_ChunkX, int a_ChunkZ) = 0;
} ;




