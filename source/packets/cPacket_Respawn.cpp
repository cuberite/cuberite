#include "cPacket_Respawn.h"

bool cPacket_Respawn::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size + m_LevelType.size() * sizeof(short);

	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte		( (char)m_PacketID,	Message, i );
	AppendByte		( m_World, Message, i );
	AppendByte		( m_Difficulty, Message, i );
	AppendByte		( m_CreativeMode, Message, i );
	AppendShort		( m_WorldHeight, Message, i );
	AppendLong		( m_MapSeed, Message, i );
	AppendString16	( m_LevelType, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}

bool cPacket_Respawn::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;
	if( !ReadByte( m_World ) ) return false;
	if( !ReadByte( m_Difficulty ) ) return false;
	if( !ReadByte( m_CreativeMode ) ) return false;
	if( !ReadShort( m_WorldHeight ) ) return false;
	if( !ReadLong( m_MapSeed ) ) return false;
	if( !ReadString16( m_LevelType ) ) return false;
	return true;
}