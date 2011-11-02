#include "cPacket_Thunderbolt.h"

bool cPacket_Thunderbolt::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte    ( (char)m_PacketID, Message, i );
	AppendInteger ( m_UniqueID, Message, i );
	AppendBool    ( m_Unknown, Message, i );
	AppendInteger ( m_xLBPos, Message, i );
	AppendInteger ( m_yLBPos, Message, i );
	AppendInteger ( m_zLBPos, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
