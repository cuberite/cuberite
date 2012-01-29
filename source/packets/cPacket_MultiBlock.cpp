
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

bool cPacket_MultiBlock::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size + m_NumBlocks * ( sizeof(short) + 2*sizeof(char) );
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte		( (char)m_PacketID,		Message, i );
	AppendInteger	( m_ChunkX,			Message, i );
	AppendInteger	( m_ChunkZ,			Message, i );
	AppendShort		( m_NumBlocks,		Message, i );

	AppendData		( (char*)m_BlockCoordinates,sizeof(short)*m_NumBlocks, Message, i );
	AppendData		( m_BlockTypes,				sizeof(char)*m_NumBlocks, Message, i );
	AppendData		( m_BlockMetas,				sizeof(char)*m_NumBlocks, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}