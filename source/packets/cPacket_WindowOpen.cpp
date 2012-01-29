
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_WindowOpen.h"





bool cPacket_WindowOpen::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size + m_WindowTitle.size() * sizeof( short );
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,		Message, i );
	AppendByte	( m_WindowID,		Message, i );
	AppendByte	( m_InventoryType,	Message, i );
	AppendString16( m_WindowTitle,	Message, i );
	AppendByte	( m_NumSlots,		Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}