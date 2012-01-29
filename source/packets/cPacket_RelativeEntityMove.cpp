
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_RelativeEntityMove.h"





bool cPacket_RelativeEntityMove::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendInteger( m_UniqueID,	Message, i );
	AppendByte	 ( m_MoveX,		Message, i );
	AppendByte	 ( m_MoveY,		Message, i );
	AppendByte	 ( m_MoveZ,		Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
