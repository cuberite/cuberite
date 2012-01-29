
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Handshake.h"





bool cPacket_Handshake::Parse(cSocket & a_Socket)
{
	//printf("Parse: NEW Handshake\n");
	m_Socket = a_Socket;
	if( !ReadString16( m_Username ) ) return false;
	return true;
}

bool cPacket_Handshake::Send(cSocket & a_Socket)
{
	//LOG("Send: NEW Handshake %s", m_Username.c_str() );
	unsigned int TotalSize = c_Size + m_Username.size() * sizeof(short);
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	( (char)m_PacketID, Message, i );
	AppendString16( m_Username, Message, i );

	bool RetVal = !cSocket::IsSocketError( cPacket::SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}