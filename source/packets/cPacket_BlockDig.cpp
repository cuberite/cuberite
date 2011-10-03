#include "cPacket_BlockDig.h"

bool cPacket_BlockDig::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendInteger( m_PosX,				Message, i );
	AppendByte	 ( m_PosY,				Message, i );
	AppendInteger( m_PosZ,				Message, i );
	AppendByte   ( m_Direction,			Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}

bool cPacket_BlockDig::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;
	if( !ReadByte	( m_Status ) ) return false;
	if( !ReadInteger( m_PosX ) ) return false;
	if( !ReadByte	( m_PosY ) ) return false;
	if( !ReadInteger( m_PosZ ) ) return false;
	if( !ReadByte	( m_Direction ) ) return false;
	return true;
}