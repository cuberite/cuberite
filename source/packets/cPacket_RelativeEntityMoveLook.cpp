#include "cPacket_RelativeEntityMoveLook.h"

bool cPacket_RelativeEntityMoveLook::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendInteger( m_UniqueID,	Message, i );
	AppendByte	 ( m_MoveX,		Message, i );
	AppendByte	 ( m_MoveY,		Message, i );
	AppendByte	 ( m_MoveZ,		Message, i );
	AppendByte   ( m_Yaw,		Message, i );
	AppendByte   ( m_Pitch,		Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
