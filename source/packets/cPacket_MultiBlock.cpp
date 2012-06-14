
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_MultiBlock.h"





cPacket_MultiBlock::cPacket_MultiBlock( const cPacket_MultiBlock & a_Copy )
{
	m_PacketID = E_MULTI_BLOCK;
	m_ChunkX = a_Copy.m_ChunkX;
	m_ChunkZ = a_Copy.m_ChunkZ;
	m_NumBlocks = a_Copy.m_NumBlocks;
	m_DataSize = a_Copy.m_DataSize;
	m_Data = new sBlockChange[m_NumBlocks];
	memcpy( m_Data, a_Copy.m_Data, sizeof(sBlockChange)*m_NumBlocks );
}





cPacket_MultiBlock::~cPacket_MultiBlock()
{
	delete [] m_Data;
}





void cPacket_MultiBlock::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_ChunkX);
	AppendInteger(a_Data, m_ChunkZ);
	AppendShort  (a_Data, m_NumBlocks);

	AppendInteger(a_Data, m_DataSize);
	for( int i = 0; i < m_NumBlocks; ++i )
	{
		AppendInteger(a_Data, m_Data[i].Data);
	}
}




