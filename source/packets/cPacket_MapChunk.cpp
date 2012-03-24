
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_MapChunk.h"
#include "../ChunkDef.h"

#include "zlib.h"





cPacket_MapChunk::~cPacket_MapChunk()
{
	delete [] m_CompressedData;
}





cPacket_MapChunk::cPacket_MapChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, const BLOCKTYPE * a_BlockData, const BIOMETYPE * a_BiomeData)
{
	m_PacketID = E_MAP_CHUNK;

	m_PosX = a_ChunkX; // Chunk coordinates now, instead of block coordinates
	m_PosZ = a_ChunkZ;

	m_bContiguous = true;  // false = no biome data, true = with biome data
	m_BitMap1 = 0;
	m_BitMap2 = 0;

	m_UnusedInt = 0;

	
	const int BlockDataSize = (cChunkDef::Height / 16) * (4096 + 2048 + 2048 + 2048);
	const int BiomeDataSize = cChunkDef::Width * cChunkDef::Width;
	char AllData [ BlockDataSize + BiomeDataSize ];

#if AXIS_ORDER == AXIS_ORDER_YZX
	memset( AllData, 0, BlockDataSize );

	unsigned int iterator = 0;
	for ( int i = 0; i < (cChunkDef::Height / 16); ++i )
	{
		m_BitMap1 |= (1 << i); // This tells what chunks are sent. Use this to NOT send air only chunks (right now everything is sent)
		for ( int y = 0; y < 16; ++y ) for( int z = 0; z < 16; ++z ) for( int x = 0; x < 16; ++x )
		{
			int idx = cChunk::MakeIndex(x, y + i * 16, z);
			AllData[iterator] = a_BlockData[idx];
			++iterator;
		}  // for y, z, x
	}
	
	// Send block metadata:
	char * Meta = a_BlockData + cChunkDef::NumBlocks;
	for ( int i = 0; i < (cChunkDef::Height / 16); ++i )
	{
		for ( int y = 0; y < 16; ++y ) for( int z = 0; z < 16; ++z )
		{
			for ( int x = 0; x < 8; ++x )
			{
				AllData[iterator] = cChunk::GetNibble(Meta, x * 2 + 0, y + i * 16, z) | (cChunk::GetNibble(Meta, x * 2 + 1, y + i * 16, z ) << 4);
				++iterator;
			}  // for x
		}  // for y, z
	}
	
	// Send block light:
	char * Light = Meta + cChunkDef::NumBlocks / 2;
	for ( int i = 0; i < (cChunkDef::Height / 16); ++i )
	{
		for ( int y = 0; y < 16; ++y ) for( int z = 0; z < 16; ++z )
		{
			for ( int x = 0; x < 8; ++x )
			{
				AllData[iterator] = cChunk::GetNibble(Light, x * 2 + 0, y + i * 16, z ) | (cChunk::GetNibble(Light, x * 2 + 1, y + i * 16, z ) << 4);
				++iterator;
			}
		}
	}
	
	// Send sky light:
	char * SkyLight = Light + cChunkDef::NumBlocks / 2;
	for( int i = 0; i < (cChunkDef::Height/16); ++i )
	{
		for( int y = 0; y < 16; ++y ) for( int z = 0; z < 16; ++z )
		{
			for( int x = 0; x < 8; ++x )
			{
				AllData[iterator] = cChunk::GetNibble(SkyLight, x * 2 + 0, y + i * 16, z ) | (cChunk::GetNibble(SkyLight, x * 2 + 1, y + i * 16, z ) << 4);
				++iterator;
			}
		}
	}
	memcpy(AllData + BlockDataSize, a_BiomeData, BiomeDataSize);
#elif AXIS_ORDER == AXIS_ORDER_XZY
	for ( int i = 0; i < 16; ++i )
	{
		m_BitMap1 |= (1 << i);
	}
	memcpy(AllData, a_BlockData, BlockDataSize);
	memcpy(AllData + BlockDataSize, a_BiomeData, BiomeDataSize);
#endif  // AXIS_ORDER

	uLongf CompressedSize = compressBound( sizeof(AllData) );
	char * CompressedBlockData = new char[CompressedSize];

	compress2( (Bytef*)CompressedBlockData, &CompressedSize, (const Bytef*)AllData, sizeof(AllData), Z_DEFAULT_COMPRESSION);

	m_CompressedData = CompressedBlockData;
	m_CompressedSize = CompressedSize;
}





cPacket_MapChunk::cPacket_MapChunk( const cPacket_MapChunk & a_Copy )
{
	m_PacketID = E_MAP_CHUNK;

	m_PosX = a_Copy.m_PosX;
	m_PosZ = a_Copy.m_PosZ;
	m_bContiguous = a_Copy.m_bContiguous;
	m_BitMap1 = a_Copy.m_BitMap1;
	m_BitMap2 = a_Copy.m_BitMap2;

	m_CompressedSize = a_Copy.m_CompressedSize;
	m_CompressedData = new char[m_CompressedSize];
	memcpy( m_CompressedData, a_Copy.m_CompressedData, m_CompressedSize );
}





void cPacket_MapChunk::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);

	AppendInteger(a_Data, m_PosX);
	AppendInteger(a_Data, m_PosZ);
	AppendBool   (a_Data, m_bContiguous);
	AppendShort  (a_Data, m_BitMap1);
	AppendShort  (a_Data, m_BitMap2);
	AppendInteger(a_Data, m_CompressedSize);
	AppendInteger(a_Data, m_UnusedInt);
	AppendData   (a_Data, m_CompressedData, m_CompressedSize);
}




