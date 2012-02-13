
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_MapChunk.h"
#include "../cChunk.h"

#include "zlib.h"





cPacket_MapChunk::~cPacket_MapChunk()
{
	delete [] m_CompressedData;
}





cPacket_MapChunk::cPacket_MapChunk(cChunk * a_Chunk)
{
	assert(a_Chunk->IsValid());
	m_PacketID = E_MAP_CHUNK;

	m_PosX = a_Chunk->GetPosX() * 16; // It has to be block coordinates
	m_PosY = (short)(a_Chunk->GetPosY() * 128);
	m_PosZ = a_Chunk->GetPosZ() * 16;

	m_SizeX = 15;
	m_SizeY = 127;
	m_SizeZ = 15;

	uLongf CompressedSize = compressBound( cChunk::c_BlockDataSize );
	char * CompressedBlockData = new char[CompressedSize];

	compress2( (Bytef*)CompressedBlockData, &CompressedSize, (const Bytef*)a_Chunk->pGetBlockData(), cChunk::c_BlockDataSize, Z_DEFAULT_COMPRESSION);

	m_CompressedData = CompressedBlockData;
	m_CompressedSize = CompressedSize;
}





cPacket_MapChunk::cPacket_MapChunk( const cPacket_MapChunk & a_Copy )
{
	m_PacketID = E_MAP_CHUNK;
	m_PosX = a_Copy.m_PosX;
	m_PosY = a_Copy.m_PosY;
	m_PosZ = a_Copy.m_PosZ;
	m_SizeX = a_Copy.m_SizeX;
	m_SizeY = a_Copy.m_SizeY;
	m_SizeZ = a_Copy.m_SizeZ;

	m_CompressedSize = a_Copy.m_CompressedSize;
	m_CompressedData = new char[m_CompressedSize];
	memcpy( m_CompressedData, a_Copy.m_CompressedData, m_CompressedSize );
}





void cPacket_MapChunk::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_PosX);
	AppendShort  (a_Data, m_PosY);
	AppendInteger(a_Data, m_PosZ);
	AppendByte   (a_Data, m_SizeX);
	AppendByte   (a_Data, m_SizeY);
	AppendByte   (a_Data, m_SizeZ);
	AppendInteger(a_Data, m_CompressedSize);
	AppendData   (a_Data, m_CompressedData, m_CompressedSize);
}




