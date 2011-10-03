#include "cPacket_Login.h"

bool cPacket_Login::Parse( cSocket & a_Socket )
{
	//printf("Parse: NEW Login\n");
	m_Socket = a_Socket;

	m_Username.clear();

	if( !ReadInteger( m_ProtocolVersion ) ) return false;
	if( !ReadString16( m_Username		) ) return false;
	if( !ReadLong	( m_MapSeed			) ) return false;
	if( !ReadInteger( m_ServerMode		) ) return false;
	if( !ReadByte	( m_Dimension		) ) return false;
	if( !ReadByte	( m_Difficulty		) ) return false;
	if( !ReadByte	( m_WorldHeight		) ) return false;
	if( !ReadByte	( m_MaxPlayers		) ) return false;
	return true;
}

bool cPacket_Login::Send( cSocket & a_Socket )
{
	//printf("Send: NEW Login\n");
	unsigned int TotalSize = c_Size + m_Username.size() * sizeof(short);
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendInteger( m_ProtocolVersion,	Message, i );
	AppendString16 ( m_Username,		Message, i );
	AppendLong	 ( m_MapSeed,			Message, i );
	AppendInteger( m_ServerMode,		Message, i );
	AppendByte	 ( m_Dimension,			Message, i );
	AppendByte	 ( m_Difficulty,		Message, i );
	AppendByte	 ( m_WorldHeight,		Message, i );
	AppendByte	 ( m_MaxPlayers,		Message, i );

	bool RetVal = !cSocket::IsSocketError( cPacket::SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}