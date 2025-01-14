
// ChunkDef.h

// Interfaces to helper types for chunk definitions. Most modules want to include this instead of cChunk.h





#pragma once

#include "BiomeDef.h"

//#include "BlockType.h";

// Used to smoothly convert to new axis ordering. One will be removed when deemed stable.
#define AXIS_ORDER_YZX 1  // Original (1.1-)
#define AXIS_ORDER_XZY 2  // New (1.2+)
#define AXIS_ORDER AXIS_ORDER_XZY





// fwd
class cBlockEntity;
class cEntity;
class cClientHandle;
class cBlockEntity;
class cChunkCoords;

using OwnedEntity = std::unique_ptr<cEntity>;
using cEntityList = std::vector<OwnedEntity>;




// tolua_begin

/** The datatype used by blockdata */
// typedef unsigned char BLOCKTYPE;

/** The datatype used by nibbledata (meta, light, skylight) */
// typedef unsigned char NIBBLETYPE;

typedef unsigned char LIGHTTYPE;

/** The type used by the heightmap */
typedef unsigned char HEIGHTTYPE;

typedef unsigned int  NEWBLOCKTYPE;
// tolua_end





/** Wraps the chunk coords into a single structure. */
class cChunkCoords
{
public:
	int m_ChunkX;
	int m_ChunkZ;

	cChunkCoords(int a_ChunkX, int a_ChunkZ) : m_ChunkX(a_ChunkX), m_ChunkZ(a_ChunkZ) {}


	bool operator == (const cChunkCoords & a_Other) const
	{
		return ((m_ChunkX == a_Other.m_ChunkX) && (m_ChunkZ == a_Other.m_ChunkZ));
	}


	bool operator != (const cChunkCoords & a_Other) const
	{
		return !(operator == (a_Other));
	}


	/** Simple comparison, to support ordering. */
	bool operator < (const cChunkCoords & a_Other) const
	{
		if (a_Other.m_ChunkX == m_ChunkX)
		{
			return (m_ChunkZ < a_Other.m_ChunkZ);
		}
		else
		{
			return (m_ChunkX < a_Other.m_ChunkX);
		}
	}


	/** Returns a string that describes the chunk coords, suitable for logging. */
	AString ToString() const
	{
		return fmt::format(FMT_STRING("[{}, {}]"), m_ChunkX, m_ChunkZ);
	}
} ;





/** Implements custom fmtlib formatting for cChunkCoords. */
namespace fmt
{
	template <> struct formatter<cChunkCoords>: formatter<int>
	{
		auto format(cChunkCoords a_Coords, format_context & a_Ctx)
		{
			return format_to(a_Ctx.out(), "[{}, {}]", a_Coords.m_ChunkX, a_Coords.m_ChunkZ);
		}
	};
}





/** Constants used throughout the code, useful typedefs and utility functions */
class cChunkDef
{
public:

	// Chunk dimensions:
	static const int Width = 16;
	static const int Height = 256;
	static const int NumBlocks = Width * Height * Width;

	static const int SectionHeight = 16;
	static const size_t NumSections = (cChunkDef::Height / SectionHeight);

	/** The type used for any heightmap operations and storage; idx = x + Width * z; Height points to the highest non-air block in the column */
	using HeightMap = std::array<HEIGHTTYPE, Width * Width>;

	/** The type used for any biomemap operations and storage inside Cuberite,
	using Cuberite biomes (need not correspond to client representation!)
	idx = x + Width * z */
	using BiomeMap = std::array<EMCSBiome, Width * Width>;

	// typedef unsigned char BlockTypes[NumBlocks];
	/** The type used for block type operations and storage, AXIS_ORDER ordering */
	using BlockStates = BlockState[NumBlocks];

	/** The type used for block type operations and storage, AXIS_ORDER ordering */
	typedef NEWBLOCKTYPE BlockTypes2[NumBlocks];

	/** The type used for block data in nibble format, AXIS_ORDER ordering */
	using LightNibbles = LIGHTTYPE[NumBlocks / 2];


	/** Converts absolute block coords into relative (chunk + block) coords: */
	inline static void AbsoluteToRelative(/* in-out */ int & a_X, int & a_Y, int & a_Z, /* out */ int & a_ChunkX, int & a_ChunkZ)
	{
		UNUSED(a_Y);
		BlockToChunk(a_X, a_Z, a_ChunkX, a_ChunkZ);

		a_X = a_X - a_ChunkX * Width;
		a_Z = a_Z - a_ChunkZ * Width;
	}


	/** Converts the specified absolute position into a relative position within its chunk.
	Use BlockToChunk to query the chunk coords. */
	inline static Vector3i AbsoluteToRelative(Vector3i a_BlockPosition)
	{
		cChunkCoords chunkPos = BlockToChunk(a_BlockPosition);
		return AbsoluteToRelative(a_BlockPosition, chunkPos);
	}


	/** Converts the absolute coords into coords relative to the specified chunk. */
	inline static Vector3i AbsoluteToRelative(Vector3i a_BlockPosition, cChunkCoords a_ChunkPos)
	{
		return { a_BlockPosition.x - a_ChunkPos.m_ChunkX * Width, a_BlockPosition.y, a_BlockPosition.z - a_ChunkPos.m_ChunkZ * Width };
	}


	/** Converts relative block coordinates into absolute coordinates with a known chunk location */
	inline static Vector3i RelativeToAbsolute(Vector3i a_RelBlockPosition, cChunkCoords a_ChunkCoords)
	{
		return Vector3i(
			a_RelBlockPosition.x + a_ChunkCoords.m_ChunkX * Width,
			a_RelBlockPosition.y,
			a_RelBlockPosition.z + a_ChunkCoords.m_ChunkZ * Width
		);
	}


	/** Validates a height-coordinate. Returns false if height-coordinate is out of height bounds */
	inline static bool IsValidHeight(Vector3i a_BlockPosition)
	{
		return ((a_BlockPosition.y >= 0) && (a_BlockPosition.y < Height));
	}


	/** Validates a height-coordinate. Returns false if height-coordinate is out of height bounds */
	inline static bool IsValidHeight(int a_BlockPosition)
	{
		return ((a_BlockPosition >= 0) && (a_BlockPosition < Height));
	}



	/** Validates a width-coordinate. Returns false if width-coordiante is out of width bounds */
	inline static bool IsValidWidth(int a_Width)
	{
		return ((a_Width >= 0) && (a_Width < Width));
	}


	/** Validates a chunk relative coordinate. Returns false if the coordiante is out of bounds for a chunk. */
	inline static bool IsValidRelPos(Vector3i a_RelPos)
	{
		return (
			IsValidWidth(a_RelPos.x) &&
			IsValidHeight(a_RelPos) &&
			IsValidWidth(a_RelPos.z)
		);
	}


	/** Converts absolute block coords to chunk coords: */
	inline static void BlockToChunk(int a_X, int a_Z, int & a_ChunkX, int & a_ChunkZ)
	{
		// This version is deprecated in favor of the vector version
		// If you're developing new code, use the other version.
		const auto ChunkCoords = BlockToChunk({ a_X, 0, a_Z });
		a_ChunkX = ChunkCoords.m_ChunkX;
		a_ChunkZ = ChunkCoords.m_ChunkZ;
	}


	/** The Y coordinate of a_Pos is ignored */
	inline static cChunkCoords BlockToChunk(const Vector3i a_Position)
	{
		return { FAST_FLOOR_DIV(a_Position.x, Width), FAST_FLOOR_DIV(a_Position.z, Width) };
	}


	inline static size_t MakeIndex(Vector3i a_Pos)
	{
		return MakeIndex(a_Pos.x, a_Pos.y, a_Pos.z);
	}


	inline static size_t MakeIndex(int x, int y, int z)
	{
		ASSERT(IsValidRelPos({ x, y, z }));

		#if AXIS_ORDER == AXIS_ORDER_XZY
			// For some reason, NOT using the Horner schema is faster. Weird.
			return static_cast<size_t>(x + (z * Width) + (y * Width * Width));   // 1.2 uses XZY
		#elif AXIS_ORDER == AXIS_ORDER_YZX
			return static_cast<size_t>(y + (z * Width) + (x * Height * Width));  // 1.1 uses YZX
		#endif
	}


	inline static Vector3i IndexToCoordinate(size_t index)
	{
		#if AXIS_ORDER == AXIS_ORDER_XZY
			return Vector3i(  // 1.2
				static_cast<int>(index % cChunkDef::Width),                       // X
				static_cast<int>(index / (cChunkDef::Width * cChunkDef::Width)),  // Y
				static_cast<int>((index / cChunkDef::Width) % cChunkDef::Width)   // Z
			);
		#elif AXIS_ORDER == AXIS_ORDER_YZX
			return Vector3i(  // 1.1
				static_cast<int>(index / (cChunkDef::Height * cChunkDef::Width)),  // X
				static_cast<int>(index % cChunkDef::Height),                       // Y
				static_cast<int>((index / cChunkDef::Height) % cChunkDef::Width)   // Z
			);
		#endif
	}


	inline static void SetBlock(BlockState * a_BlockTypes, int a_X, int a_Y, int a_Z, BlockState a_Type)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Y >= 0) && (a_Y < Height));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		a_BlockTypes[MakeIndex(a_X, a_Y, a_Z)] = a_Type;
	}


	inline static void SetBlock(BlockState * a_BlockTypes, int a_Index, BlockState a_Type)
	{
		ASSERT((a_Index >= 0) && (a_Index <= NumBlocks));
		a_BlockTypes[a_Index] = a_Type;
	}


	inline static BlockState GetBlock(const BlockState * a_BlockTypes, Vector3i a_RelPos)
	{
		ASSERT(IsValidRelPos(a_RelPos));
		return a_BlockTypes[MakeIndex(a_RelPos)];
	}


	inline static BlockState GetBlock(const BlockState * a_BlockTypes, int a_X, int a_Y, int a_Z)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Y >= 0) && (a_Y < Height));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		return a_BlockTypes[MakeIndex(a_X, a_Y, a_Z)];
	}


	inline static BlockState GetBlock(const BlockState * a_BlockTypes, int a_Idx)
	{
		ASSERT((a_Idx >= 0) && (a_Idx < NumBlocks));
		return a_BlockTypes[a_Idx];
	}


	inline static HEIGHTTYPE GetHeight(const HeightMap & a_HeightMap, int a_X, int a_Z)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		return a_HeightMap[static_cast<size_t>(a_X + Width * a_Z)];
	}


	inline static void SetHeight(HeightMap & a_HeightMap, int a_X, int a_Z, HEIGHTTYPE a_Height)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		a_HeightMap[static_cast<size_t>(a_X + Width * a_Z)] = a_Height;
	}


	inline static EMCSBiome GetBiome(const BiomeMap & a_BiomeMap, int a_X, int a_Z)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		return a_BiomeMap[static_cast<size_t>(a_X + Width * a_Z)];
	}


	inline static void SetBiome(BiomeMap & a_BiomeMap, int a_X, int a_Z, EMCSBiome a_Biome)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		a_BiomeMap[static_cast<size_t>(a_X + Width * a_Z)] = a_Biome;
	}


	static LIGHTTYPE GetLightType(const LIGHTTYPE * a_Buffer, int x, int y, int z)
	{
		if ((x < Width) && (x > -1) && (y < Height) && (y > -1) && (z < Width) && (z > -1))
		{
			return ExpandLightType(a_Buffer, MakeIndex(x, y, z));
		}
		ASSERT(!"cChunkDef::GetNibble(): coords out of chunk range!");
		return 0;
	}


	inline static void PackLightType(LIGHTTYPE * const a_Buffer, const size_t a_Index, const LIGHTTYPE a_Nibble)
	{
		ASSERT((a_Nibble & 0xF) == a_Nibble);  // Only the lower bits should be set

		a_Buffer[a_Index / 2] = static_cast<LIGHTTYPE>(
			(a_Buffer[a_Index / 2] & (0xf0 >> ((a_Index & 1) * 4))) |  // The untouched nibble
			((a_Nibble & 0x0f) << ((a_Index & 1) * 4))  // The nibble being set
		);
	}

	inline static void PackLightType(const BlockState * const a_Buffer, const size_t a_Index, const BlockState a_Block)
	{
		// Do nothing - just make the compiler happy
	}


	inline static LIGHTTYPE ExpandLightType(const LIGHTTYPE * const a_Buffer, const size_t a_Index)
	{
		return (a_Buffer[a_Index / 2] >> ((a_Index & 1) * 4)) & 0x0f;
	}

	inline static LIGHTTYPE ExpandLightType(const std::byte * const a_Buffer, const size_t a_Index)
	{
		return static_cast<LIGHTTYPE>(a_Buffer[a_Index / 2] >> ((a_Index & 1) * 4)) & 0x0f;
	}

	inline static BlockState ExpandLightType(const BlockState * const a_Buffer, const size_t a_Index)
	{
		return a_Buffer[a_Index];
	}
} ;





/** Interface class used for comparing clients of two chunks.
Used primarily for entity moving while both chunks are locked. */
class cClientDiffCallback
{
public:

	virtual ~cClientDiffCallback() {}

	/** Called for clients that are in Chunk1 and not in Chunk2, */
	virtual void Removed(cClientHandle * a_Client) = 0;

	/** Called for clients that are in Chunk2 and not in Chunk1. */
	virtual void Added(cClientHandle * a_Client) = 0;
} ;





struct sSetBlock
{
	int m_RelX, m_RelY, m_RelZ;
	int m_ChunkX, m_ChunkZ;
	BlockState m_Block;

	sSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BlockState a_Block):
		m_RelX(a_BlockX),
		m_RelY(a_BlockY),
		m_RelZ(a_BlockZ),
		m_Block(a_Block)
	{
		cChunkDef::AbsoluteToRelative(m_RelX, m_RelY, m_RelZ, m_ChunkX, m_ChunkZ);
	}

	sSetBlock(Vector3i a_BlockPos, BlockState a_Block) :
		sSetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z, a_Block)
	{
	}

	sSetBlock(int a_ChunkX, int a_ChunkZ, int a_RelX, int a_RelY, int a_RelZ, BlockState a_Block) :
		m_RelX(a_RelX), m_RelY(a_RelY), m_RelZ(a_RelZ),
		m_ChunkX(a_ChunkX), m_ChunkZ(a_ChunkZ),
		m_Block(a_Block)
	{
		ASSERT((a_RelX >= 0) && (a_RelX < cChunkDef::Width));
		ASSERT((a_RelZ >= 0) && (a_RelZ < cChunkDef::Width));
	}

	bool operator<(const sSetBlock & v1) const
	{
		return v1.m_RelY > this->m_RelY;
	}

	/** Returns the absolute X coord of the stored block. */
	int GetX(void) const { return m_RelX + cChunkDef::Width * m_ChunkX; }

	/** Returns the absolute Y coord of the stored block.
	Is the same as relative Y coords, because there's no Y relativization. */
	int GetY(void) const { return m_RelY; }

	/** Returns the absolute Z coord of the stored block. */
	int GetZ(void) const { return m_RelZ + cChunkDef::Width * m_ChunkZ; }

	/** Returns the absolute coords of the stored block. */
	Vector3i GetAbsolutePos() const
	{
		return Vector3i(GetX(), GetY(), GetZ());
	}

	/** Returns the relative position of the stored block within its chunk. */
	Vector3i GetRelativePos() const
	{
		return Vector3i(m_RelX, m_RelY, m_RelZ);
	}
};

typedef std::vector<sSetBlock> sSetBlockVector;

typedef std::list<cChunkCoords> cChunkCoordsList;
typedef std::vector<cChunkCoords> cChunkCoordsVector;





/** A simple hash function for chunk coords, we assume that chunk coords won't use more than 16 bits, so the hash is almost an identity.
Used for std::unordered_map<cChunkCoords, ...> */
class cChunkCoordsHash
{
public:
	size_t operator () (const cChunkCoords & a_Coords) const
	{
		return (static_cast<size_t>(a_Coords.m_ChunkX) << 16) ^ static_cast<size_t>(a_Coords.m_ChunkZ);
	}
};





/** Interface class used as a callback for operations that involve chunk coords */
class cChunkCoordCallback
{
public:

	virtual ~cChunkCoordCallback() {}

	/** Called with the chunk's coords, and an optional operation status flag for operations that support it. */
	virtual void Call(cChunkCoords a_Coords, bool a_IsSuccess) = 0;
} ;





/** Generic template that can store any kind of data together with a triplet of 3 coords */
template <typename X> class cCoordWithData
{
public:
	int x;
	int y;
	int z;
	X   Data;

	cCoordWithData(int a_X, int a_Y, int a_Z) :
		x(a_X), y(a_Y), z(a_Z), Data()
	{
	}

	cCoordWithData(int a_X, int a_Y, int a_Z, const X & a_Data) :
		x(a_X), y(a_Y), z(a_Z), Data(a_Data)
	{
	}
} ;

typedef cCoordWithData<int>        cCoordWithInt;

typedef std::list<cCoordWithInt>   cCoordWithIntList;
typedef std::vector<cCoordWithInt> cCoordWithIntVector;
