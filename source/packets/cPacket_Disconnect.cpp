
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Disconnect.h"





bool cPacket_Disconnect::Parse( cSocket & a_Socket )
{
	m_Socket = a_Socket;
	if( !ReadString16(m_Reason) ) return false;
	return true;
}

bool cPacket_Disconnect::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size + m_Reason.size()*sizeof(short);
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID, Message, i );
	AppendString16 ( m_Reason,   Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
