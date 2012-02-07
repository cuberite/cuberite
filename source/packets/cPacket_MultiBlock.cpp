
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_MultiBlock.h"





cPacket_MultiBlock::cPacket_MultiBlock( const cPacket_MultiBlock & a_Copy )
{
	m_PacketID = E_MULTI_BLOCK;
	m_ChunkX = a_Copy.m_ChunkX;
	m_ChunkZ = a_Copy.m_ChunkZ;
	m_NumBlocks = a_Copy.m_NumBlocks;
	m_BlockCoordinates = new unsigned short[m_NumBlocks];
	memcpy( m_BlockCoordinates, a_Copy.m_BlockCoordinates, sizeof(short)*m_NumBlocks );
	m_BlockTypes = new char[m_NumBlocks];
	memcpy( m_BlockTypes, a_Copy.m_BlockTypes, m_NumBlocks );
	m_BlockMetas = new char[m_NumBlocks];
	memcpy( m_BlockMetas, a_Copy.m_BlockMetas, m_NumBlocks );
}





cPacket_MultiBlock::~cPacket_MultiBlock()
{
	if( m_BlockCoordinates ) delete [] m_BlockCoordinates;
	if( m_BlockTypes ) delete [] m_BlockTypes;
	if( m_BlockMetas ) delete [] m_BlockMetas;
}





void cPacket_MultiBlock::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_ChunkX);
	AppendInteger(a_Data, m_ChunkZ);
	AppendShort  (a_Data, m_NumBlocks);

	AppendData   (a_Data, (char *)m_BlockCoordinates, sizeof(short) * m_NumBlocks);
	AppendData   (a_Data, m_BlockTypes,	m_NumBlocks);
	AppendData   (a_Data, m_BlockMetas,	m_NumBlocks);
}




