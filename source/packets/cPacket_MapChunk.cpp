
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_MapChunk.h"
#include "../cChunk.h"

#include "zlib.h"





cPacket_MapChunk::~cPacket_MapChunk()
{
	delete [] m_CompressedData;
}





cPacket_MapChunk::cPacket_MapChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData)
{
	m_PacketID = E_MAP_CHUNK;

#if (MINECRAFT_1_2_2 == 1 )

	// ... 
	m_PosX = a_ChunkX; // Chunk coordinates now, instead of block coordinates
	m_PosZ = a_ChunkZ;

	m_bContiguous = false;
	m_BitMap1 = 0;
	m_BitMap2 = 0;

	m_UnusedInt = 0;

	
	unsigned int DataSize = (cChunk::c_ChunkHeight / 16) * (4096 + 2048 + 2048 + 2048);
	std::auto_ptr<char> AllData(new char[ DataSize ]);
	memset( AllData.get(), 0, DataSize );

	unsigned int iterator = 0;
	for ( int i = 0; i < (cChunk::c_ChunkHeight / 16); ++i )
	{
		m_BitMap1 |= (1 << i); // This tells what chunks are sent. Use this to NOT send air only chunks (right now everything is sent)
		for ( int y = 0; y < 16; ++y ) for( int z = 0; z < 16; ++z ) for( int x = 0; x < 16; ++x )
		{
			int idx = cChunk::MakeIndex(x, y + i * 16, z);
			AllData.get()[iterator] = a_BlockData[idx];
			++iterator;
		}  // for y, z, x
	}
	
	// Send block metadata:
	char * Meta = a_BlockData + cChunk::c_NumBlocks;
	for ( int i = 0; i < (cChunk::c_ChunkHeight / 16); ++i )
	{
		for ( int y = 0; y < 16; ++y ) for( int z = 0; z < 16; ++z )
		{
			for ( int x = 0; x < 8; ++x )
			{
				AllData.get()[iterator] = cChunk::GetNibble(Meta, x * 2 + 0, y + i * 16, z) | (cChunk::GetNibble(Meta, x * 2 + 1, y + i * 16, z ) << 4);
				++iterator;
			}  // for x
		}  // for y, z
	}
	
	// Send block light:
	char * Light = Meta + cChunk::c_NumBlocks / 2;
	for ( int i = 0; i < (cChunk::c_ChunkHeight / 16); ++i )
	{
		for ( int y = 0; y < 16; ++y ) for( int z = 0; z < 16; ++z )
		{
			for ( int x = 0; x < 8; ++x )
			{
				AllData.get()[iterator] = cChunk::GetNibble(Light, x * 2 + 0, y + i * 16, z ) | (cChunk::GetNibble(Light, x * 2 + 1, y + i * 16, z ) << 4);
				++iterator;
			}
		}
	}
	
	// Send sky light:
	char * SkyLight = Light + cChunk::c_NumBlocks / 2;
	for( int i = 0; i < (cChunk::c_ChunkHeight/16); ++i )
	{
		for( int y = 0; y < 16; ++y ) for( int z = 0; z < 16; ++z )
		{
			for( int x = 0; x < 8; ++x )
			{
				AllData.get()[iterator] = cChunk::GetNibble(SkyLight, x * 2 + 0, y + i * 16, z ) | (cChunk::GetNibble(SkyLight, x * 2 + 1, y + i * 16, z ) << 4);
				++iterator;
			}
		}
	}

	uLongf CompressedSize = compressBound( DataSize );
	char * CompressedBlockData = new char[CompressedSize];

	compress2( (Bytef*)CompressedBlockData, &CompressedSize, (const Bytef*)AllData.get(), DataSize, Z_DEFAULT_COMPRESSION);

	m_CompressedData = CompressedBlockData;
	m_CompressedSize = CompressedSize;

#else
	m_PosX = a_ChunkX * cChunk::c_ChunkWidth; // It has to be block coordinates
	m_PosY = (short)(a_ChunkY * cChunk::c_ChunkHeight);
	m_PosZ = a_ChunkZ * cChunk::c_ChunkWidth;

	m_SizeX = 15;
	m_SizeY = 127;
	m_SizeZ = 15;

	uLongf CompressedSize = compressBound( cChunk::c_BlockDataSize );
	char * CompressedBlockData = new char[CompressedSize];

	compress2( (Bytef*)CompressedBlockData, &CompressedSize, (const Bytef*)a_BlockData, cChunk::c_BlockDataSize, Z_DEFAULT_COMPRESSION);

	m_CompressedData = CompressedBlockData;
	m_CompressedSize = CompressedSize;
#endif
}





cPacket_MapChunk::cPacket_MapChunk( const cPacket_MapChunk & a_Copy )
{
	m_PacketID = E_MAP_CHUNK;

#if (MINECRAFT_1_2_2 == 1 )
	m_PosX = a_Copy.m_PosX;
	m_PosZ = a_Copy.m_PosZ;
	m_bContiguous = a_Copy.m_bContiguous;
	m_BitMap1 = a_Copy.m_BitMap1;
	m_BitMap2 = a_Copy.m_BitMap2;

	m_CompressedSize = a_Copy.m_CompressedSize;
	m_CompressedData = new char[m_CompressedSize];
	memcpy( m_CompressedData, a_Copy.m_CompressedData, m_CompressedSize );
#else
	m_PosX = a_Copy.m_PosX;
	m_PosY = a_Copy.m_PosY;
	m_PosZ = a_Copy.m_PosZ;
	m_SizeX = a_Copy.m_SizeX;
	m_SizeY = a_Copy.m_SizeY;
	m_SizeZ = a_Copy.m_SizeZ;

	m_CompressedSize = a_Copy.m_CompressedSize;
	m_CompressedData = new char[m_CompressedSize];
	memcpy( m_CompressedData, a_Copy.m_CompressedData, m_CompressedSize );
#endif
}





void cPacket_MapChunk::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);

#if (MINECRAFT_1_2_2 == 1 )
	AppendInteger(a_Data, m_PosX);
	AppendInteger(a_Data, m_PosZ);
	AppendBool   (a_Data, m_bContiguous);
	AppendShort  (a_Data, m_BitMap1);
	AppendShort  (a_Data, m_BitMap2);
	AppendInteger(a_Data, m_CompressedSize);
	AppendInteger(a_Data, m_UnusedInt);
	AppendData   (a_Data, m_CompressedData, m_CompressedSize);
#else
	AppendInteger(a_Data, m_PosX);
	AppendShort  (a_Data, m_PosY);
	AppendInteger(a_Data, m_PosZ);
	AppendByte   (a_Data, m_SizeX);
	AppendByte   (a_Data, m_SizeY);
	AppendByte   (a_Data, m_SizeZ);
	AppendInteger(a_Data, m_CompressedSize);
	AppendData   (a_Data, m_CompressedData, m_CompressedSize);
#endif
}




