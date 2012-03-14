
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




/// The datatype used by blockdata, metadata, blocklight and skylight
typedef char BLOCKTYPE;
typedef char BIOMETYPE;





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

	typedef unsigned char HeightMap[Width * Width];


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


	static BLOCKTYPE GetNibble(BLOCKTYPE * a_Buffer, int a_BlockIdx)
	{
		if ((a_BlockIdx > -1) && (a_BlockIdx < cChunkDef::NumBlocks))
		{
			return (a_Buffer[a_BlockIdx / 2] >> ((a_BlockIdx & 1) * 4)) & 0x0f;
		}
		return 0;
	}
	
	
	static BLOCKTYPE GetNibble(BLOCKTYPE * a_Buffer, int x, int y, int z)
	{
		if ((x < Width) && (x > -1) && (y < Height) && (y > -1) && (z < Width) && (z > -1))
		{
			int Index = MakeIndexNoCheck(x, y, z);
			return (a_Buffer[Index / 2] >> ((Index & 1) * 4)) & 0x0f;
		}
		return 0;
	}


	static void SetNibble(BLOCKTYPE * a_Buffer, int a_BlockIdx, BLOCKTYPE a_Nibble)
	{
		if ((a_BlockIdx > -1) && (a_BlockIdx < cChunkDef::NumBlocks))
		{
			a_Buffer[a_BlockIdx / 2] = (
				(a_Buffer[a_BlockIdx / 2] & (0xf0 >> ((a_BlockIdx & 1) * 4))) |  // The untouched nibble
				((a_Nibble & 0x0f) << ((a_BlockIdx & 1) * 4))  // The nibble being set
			);
		}
	}
	
	
	static void SetNibble(BLOCKTYPE * a_Buffer, int x, int y, int z, BLOCKTYPE a_Nibble)
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


	inline static BLOCKTYPE GetNibble(BLOCKTYPE * a_Buffer, const Vector3i & a_BlockPos )
	{
		return GetNibble( a_Buffer, a_BlockPos.x, a_BlockPos.y, a_BlockPos.z );
	}
	
	
	inline static void SetNibble(BLOCKTYPE * a_Buffer, const Vector3i & a_BlockPos, char a_Value )
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
	/// Called once to provide heightmap data
	virtual void HeightMap(const cChunkDef::HeightMap * a_HeightMap) {};
	
	/// Called once to export block types
	virtual void BlockTypes   (const BLOCKTYPE * a_Type) {};
	
	/// Called once to export block meta
	virtual void BlockMeta    (const BLOCKTYPE * a_Meta) {};
	
	/// Called once to export block light
	virtual void BlockLight   (const BLOCKTYPE * a_Meta) {};
	
	/// Called once to export sky light
	virtual void BlockSkyLight(const BLOCKTYPE * a_Meta) {};
	
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
protected:
	BLOCKTYPE m_BlockData[cChunkDef::BlockDataSize];

	virtual void BlockTypes(const BLOCKTYPE * a_BlockTypes) override
	{
		memcpy(m_BlockData, a_BlockTypes, cChunkDef::NumBlocks);
	}


	virtual void BlockMeta(const BLOCKTYPE * a_BlockMeta) override
	{
		memcpy(m_BlockData + cChunkDef::NumBlocks, a_BlockMeta, cChunkDef::NumBlocks / 2);
	}


	virtual void BlockLight(const BLOCKTYPE * a_BlockLight) override
	{
		memcpy(m_BlockData + 3 * cChunkDef::NumBlocks / 2, a_BlockLight, cChunkDef::NumBlocks / 2);
	}


	virtual void BlockSkyLight(const BLOCKTYPE * a_BlockSkyLight) override
	{
		memcpy(m_BlockData + 2 * cChunkDef::NumBlocks, a_BlockSkyLight, cChunkDef::NumBlocks / 2);
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
	char BlockType, BlockMeta;

	sSetBlock( int a_X, int a_Y, int a_Z, char a_BlockType, char a_BlockMeta );  // absolute block position
};

typedef std::list< sSetBlock > sSetBlockList;





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





