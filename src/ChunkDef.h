
// ChunkDef.h

// Interfaces to helper types for chunk definitions. Most modules want to include this instead of cChunk.h





#pragma once





// Used to smoothly convert to new axis ordering. One will be removed when deemed stable.
#define AXIS_ORDER_YZX 1  // Original (1.1-)
#define AXIS_ORDER_XZY 2  // New (1.2+)
#define AXIS_ORDER AXIS_ORDER_XZY





// fwd
class cBlockEntity;
class cEntity;
class cClientHandle;
class cBlockEntity;

typedef std::list<cEntity *>          cEntityList;
typedef std::map<int, cBlockEntity *> cBlockEntities;




// tolua_begin

/** The datatype used by blockdata */
typedef unsigned char BLOCKTYPE;

/** The datatype used by nibbledata (meta, light, skylight) */
typedef unsigned char NIBBLETYPE;

/** The type used by the heightmap */
typedef unsigned char HEIGHTTYPE;

// tolua_end




/** Constants used throughout the code, useful typedefs and utility functions */
class cChunkDef
{
public:
	// Chunk dimensions:
	static const int Width = 16;
	static const int Height = 256;
	static const int NumBlocks = Width * Height * Width;

	/** If the data is collected into a single buffer, how large it needs to be: */
	static const int BlockDataSize = cChunkDef::NumBlocks * 2 + (cChunkDef::NumBlocks / 2);  // 2.5 * numblocks

	/** The type used for any heightmap operations and storage; idx = x + Width * z; Height points to the highest non-air block in the column */
	typedef HEIGHTTYPE HeightMap[Width * Width];

	/** The type used for any biomemap operations and storage inside Cuberite,
	using Cuberite biomes (need not correspond to client representation!)
	idx = x + Width * z */
	typedef EMCSBiome BiomeMap[Width * Width];

	/** The type used for block type operations and storage, AXIS_ORDER ordering */
	typedef BLOCKTYPE BlockTypes[NumBlocks];

	/** The type used for block data in nibble format, AXIS_ORDER ordering */
	typedef NIBBLETYPE BlockNibbles[NumBlocks / 2];

	/** The storage wrapper used for compressed blockdata residing in RAMz */
	typedef std::vector<BLOCKTYPE> COMPRESSED_BLOCKTYPE;

	/** The storage wrapper used for compressed nibbledata residing in RAMz */
	typedef std::vector<NIBBLETYPE> COMPRESSED_NIBBLETYPE;


	/** Converts absolute block coords into relative (chunk + block) coords: */
	inline static void AbsoluteToRelative(/* in-out */ int & a_X, int & a_Y, int & a_Z, /* out */ int & a_ChunkX, int & a_ChunkZ)
	{
		UNUSED(a_Y);
		BlockToChunk(a_X, a_Z, a_ChunkX, a_ChunkZ);

		a_X = a_X - a_ChunkX * Width;
		a_Z = a_Z - a_ChunkZ * Width;
	}

	inline static Vector3i AbsoluteToRelative(Vector3i a_BlockPosition)
	{
		int ChunkX, ChunkZ;
		BlockToChunk(a_BlockPosition.x, a_BlockPosition.z, ChunkX, ChunkZ);

		return {a_BlockPosition.x - ChunkX * Width, a_BlockPosition.y, a_BlockPosition.z - ChunkZ * Width};
	}

	inline static Vector3i AbsoluteToRelative(Vector3i a_BlockPosition, int a_ChunkX, int a_ChunkZ)
	{
		return {a_BlockPosition.x - a_ChunkX * Width, a_BlockPosition.y, a_BlockPosition.z - a_ChunkZ * Width};
	}

	/** Converts relative block coordinates into absolute coordinates with a known chunk location */
	inline static Vector3i RelativeToAbsolute(const Vector3i & a_RelBlockPosition, int a_ChunkX, int a_ChunkZ)
	{
		return Vector3i(a_RelBlockPosition.x + a_ChunkX * Width, a_RelBlockPosition.y, a_RelBlockPosition.z + a_ChunkZ * Width);
	}

	/** Validates a height-coordinate. Returns false if height-coordiante is out of height bounds */
	inline static bool IsValidHeight(int a_Height)
	{
		return ((a_Height >= 0) && (a_Height < Height));
	}

	/** Validates a width-coordinate. Returns false if width-coordiante is out of width bounds */
	inline static bool IsValidWidth(int a_Width)
	{
		return ((a_Width >= 0) && (a_Width < Width));
	}

	/** Converts absolute block coords to chunk coords: */
	inline static void BlockToChunk(int a_X, int a_Z, int & a_ChunkX, int & a_ChunkZ)
	{
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


	inline static int MakeIndex(int x, int y, int z)
	{
		if (
			(x < Width)  && (x > -1) &&
			(y < Height) && (y > -1) &&
			(z < Width)  && (z > -1)
		)
		{
			return MakeIndexNoCheck(x, y, z);
		}
		LOGERROR("cChunkDef::MakeIndex(): coords out of range: {%d, %d, %d}; returning fake index 0", x, y, z);
		ASSERT(!"cChunkDef::MakeIndex(): coords out of chunk range!");
		return 0;
	}


	inline static int MakeIndexNoCheck(int x, int y, int z)
	{
		#if AXIS_ORDER == AXIS_ORDER_XZY
			// For some reason, NOT using the Horner schema is faster. Weird.
			return x + (z * cChunkDef::Width) + (y * cChunkDef::Width * cChunkDef::Width);   // 1.2 uses XZY
		#elif AXIS_ORDER == AXIS_ORDER_YZX
			return y + (z * cChunkDef::Width) + (x * cChunkDef::Height * cChunkDef::Width);  // 1.1 uses YZX
		#endif
	}


	inline static Vector3i IndexToCoordinate( unsigned int index)
	{
		#if AXIS_ORDER == AXIS_ORDER_XZY
			return Vector3i(  // 1.2
				index % cChunkDef::Width,                       // X
				index / (cChunkDef::Width * cChunkDef::Width),  // Y
				(index / cChunkDef::Width) % cChunkDef::Width   // Z
			);
		#elif AXIS_ORDER == AXIS_ORDER_YZX
			return Vector3i(  // 1.1
				index / (cChunkDef::Height * cChunkDef::Width),  // X
				index % cChunkDef::Height,                       // Y
				(index / cChunkDef::Height) % cChunkDef::Width   // Z
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


	inline static void SetBlock(BLOCKTYPE * a_BlockTypes, int a_Index, BLOCKTYPE a_Type)
	{
		ASSERT((a_Index >= 0) && (a_Index <= NumBlocks));
		a_BlockTypes[a_Index] = a_Type;
	}


	inline static BLOCKTYPE GetBlock(const BLOCKTYPE * a_BlockTypes, int a_X, int a_Y, int a_Z)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Y >= 0) && (a_Y < Height));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		return a_BlockTypes[MakeIndexNoCheck(a_X, a_Y, a_Z)];
	}


	inline static BLOCKTYPE GetBlock(const BLOCKTYPE * a_BlockTypes, int a_Idx)
	{
		ASSERT((a_Idx >= 0) && (a_Idx < NumBlocks));
		return a_BlockTypes[a_Idx];
	}


	inline static HEIGHTTYPE GetHeight(const HeightMap & a_HeightMap, int a_X, int a_Z)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		return a_HeightMap[a_X + Width * a_Z];
	}


	inline static void SetHeight(HeightMap & a_HeightMap, int a_X, int a_Z, HEIGHTTYPE a_Height)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		a_HeightMap[a_X + Width * a_Z] = a_Height;
	}


	inline static EMCSBiome GetBiome(const BiomeMap & a_BiomeMap, int a_X, int a_Z)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		return a_BiomeMap[a_X + Width * a_Z];
	}


	inline static void SetBiome(BiomeMap & a_BiomeMap, int a_X, int a_Z, EMCSBiome a_Biome)
	{
		ASSERT((a_X >= 0) && (a_X < Width));
		ASSERT((a_Z >= 0) && (a_Z < Width));
		a_BiomeMap[a_X + Width * a_Z] = a_Biome;
	}


	static NIBBLETYPE GetNibble(const COMPRESSED_NIBBLETYPE & a_Buffer, int a_BlockIdx, bool a_IsSkyLightNibble = false)
	{
		if ((a_BlockIdx > -1) && (a_BlockIdx < NumBlocks))
		{
			if (static_cast<size_t>(a_BlockIdx / 2) >= a_Buffer.size())
			{
				return (a_IsSkyLightNibble ? 0xff : 0);
			}
			return (a_Buffer[static_cast<size_t>(a_BlockIdx / 2)] >> ((a_BlockIdx & 1) * 4)) & 0x0f;
		}
		ASSERT(!"cChunkDef::GetNibble(): index out of chunk range!");
		return 0;
	}


	static NIBBLETYPE GetNibble(const COMPRESSED_NIBBLETYPE & a_Buffer, int x, int y, int z, bool a_IsSkyLightNibble = false)
	{
		if ((x < Width) && (x > -1) && (y < Height) && (y > -1) && (z < Width) && (z > -1))
		{
			size_t Index = static_cast<size_t>(MakeIndexNoCheck(x, y, z));
			if ((Index / 2) >= a_Buffer.size())
			{
				return (a_IsSkyLightNibble ? 0xff : 0);
			}
			return ExpandNibble(a_Buffer, Index);
		}
		ASSERT(!"cChunkDef::GetNibble(): coords out of chunk range!");
		return 0;
	}


	static NIBBLETYPE GetNibble(const NIBBLETYPE * a_Buffer, int x, int y, int z)
	{
		if ((x < Width) && (x > -1) && (y < Height) && (y > -1) && (z < Width) && (z > -1))
		{
			int Index = MakeIndexNoCheck(x, y, z);
			return (a_Buffer[static_cast<size_t>(Index / 2)] >> ((Index & 1) * 4)) & 0x0f;
		}
		ASSERT(!"cChunkDef::GetNibble(): coords out of chunk range!");
		return 0;
	}


	static void SetNibble(COMPRESSED_NIBBLETYPE & a_Buffer, int a_BlockIdx, NIBBLETYPE a_Nibble)
	{
		if ((a_BlockIdx < 0) || (a_BlockIdx >= NumBlocks))
		{
			ASSERT(!"cChunkDef::SetNibble(): index out of range!");
			return;
		}
		if (static_cast<size_t>(a_BlockIdx / 2) >= a_Buffer.size())
		{
			a_Buffer.resize(static_cast<size_t>((a_BlockIdx / 2) + 1));
		}
		a_Buffer[static_cast<size_t>(a_BlockIdx / 2)] = PackNibble(a_Buffer, static_cast<size_t>(a_BlockIdx), a_Nibble);
	}


	static void SetNibble(COMPRESSED_NIBBLETYPE & a_Buffer, int x, int y, int z, NIBBLETYPE a_Nibble)
	{
		if (
			(x >= Width)  || (x < 0) ||
			(y >= Height) || (y < 0) ||
			(z >= Width)  || (z < 0)
		)
		{
			ASSERT(!"cChunkDef::SetNibble(): index out of range!");
			return;
		}

		size_t Index = static_cast<size_t>(MakeIndexNoCheck(x, y, z));
		if ((Index / 2) >= a_Buffer.size())
		{
			a_Buffer.resize(((Index / 2) + 1));
		}
		a_Buffer[(Index / 2)] = PackNibble(a_Buffer, Index, a_Nibble);
	}


private:


	inline static NIBBLETYPE PackNibble(const COMPRESSED_NIBBLETYPE & a_Buffer, size_t a_Index, NIBBLETYPE a_Nibble)
	{
		return static_cast<NIBBLETYPE>(
			(a_Buffer[a_Index / 2] & (0xf0 >> ((a_Index & 1) * 4))) |  // The untouched nibble
			((a_Nibble & 0x0f) << ((a_Index & 1) * 4))  // The nibble being set
		);
	}


	inline static NIBBLETYPE ExpandNibble(const COMPRESSED_NIBBLETYPE & a_Buffer, size_t a_Index)
	{
		return (a_Buffer[a_Index / 2] >> ((a_Index & 1) * 4)) & 0x0f;
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
	BLOCKTYPE m_BlockType;
	NIBBLETYPE m_BlockMeta;

	sSetBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta);

	sSetBlock(int a_ChunkX, int a_ChunkZ, int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) :
		m_RelX(a_RelX), m_RelY(a_RelY), m_RelZ(a_RelZ),
		m_ChunkX(a_ChunkX), m_ChunkZ(a_ChunkZ),
		m_BlockType(a_BlockType),
		m_BlockMeta(a_BlockMeta)
	{
		ASSERT((a_RelX >= 0) && (a_RelX < cChunkDef::Width));
		ASSERT((a_RelZ >= 0) && (a_RelZ < cChunkDef::Width));
	}

	/** Returns the absolute X coord of the stored block. */
	int GetX(void) const { return m_RelX + cChunkDef::Width * m_ChunkX; }

	/** Returns the absolute Y coord of the stored block.
	Is the same as relative Y coords, because there's no Y relativization. */
	int GetY(void) const { return m_RelY; }

	/** Returns the absolute Z coord of the stored block. */
	int GetZ(void) const { return m_RelZ + cChunkDef::Width * m_ChunkZ; }

	/** Returns the absolute position of the stored block. */
	Vector3i GetPos(void) const { return Vector3i(GetX(), GetY(), GetZ()); }
};

typedef std::list<sSetBlock> sSetBlockList;
typedef std::vector<sSetBlock> sSetBlockVector;





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
} ;

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





class cChunkCoordsWithBool
{
public:
	int m_ChunkX;
	int m_ChunkZ;
	bool m_ForceGenerate;

	cChunkCoordsWithBool(int a_ChunkX, int a_ChunkZ, bool a_ForceGenerate) : m_ChunkX(a_ChunkX), m_ChunkZ(a_ChunkZ), m_ForceGenerate(a_ForceGenerate){}

	bool operator == (const cChunkCoordsWithBool & a_Other) const
	{
		return ((m_ChunkX == a_Other.m_ChunkX) && (m_ChunkZ == a_Other.m_ChunkZ) && (m_ForceGenerate == a_Other.m_ForceGenerate));
	}
};

typedef std::list<cChunkCoordsWithBool> cChunkCoordsWithBoolList;





/** Interface class used as a callback for operations that involve chunk coords */
class cChunkCoordCallback
{
public:

	virtual ~cChunkCoordCallback() {}

	/** Called with the chunk's coords, and an optional operation status flag for operations that support it. */
	virtual void Call(int a_ChunkX, int a_ChunkZ, bool a_IsSuccess) = 0;
} ;





/** Provides storage for a set of chunk coords together with a callback.
Used for chunk queues that notify about processed items. */
class cChunkCoordsWithCallback
{
public:
	cChunkCoordsWithCallback(int a_ChunkX, int a_ChunkZ, cChunkCoordCallback * a_Callback):
		m_ChunkX(a_ChunkX),
		m_ChunkZ(a_ChunkZ),
		m_Callback(a_Callback)
	{
	}

	int m_ChunkX;
	int m_ChunkZ;
	cChunkCoordCallback * m_Callback;
};





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
typedef cCoordWithData<BLOCKTYPE>  cCoordWithBlock;

typedef std::list<cCoordWithInt>   cCoordWithIntList;
typedef std::vector<cCoordWithInt> cCoordWithIntVector;
