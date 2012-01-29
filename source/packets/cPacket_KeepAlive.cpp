
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_KeepAlive.h"





bool cPacket_KeepAlive::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID, Message, i );
	AppendInteger( m_KeepAliveID, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}

bool cPacket_KeepAlive::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;
	if( !ReadInteger( m_KeepAliveID ) ) return false;
	return true;
}