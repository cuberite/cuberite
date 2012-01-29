
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_UpdateHealth.h"





bool cPacket_UpdateHealth::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;

	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte		( (char)m_PacketID,	Message, i );
	AppendShort		( m_Health,		Message, i );
	AppendShort		( m_Food,		Message, i );
	AppendFloat		( m_Saturation, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
