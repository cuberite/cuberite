#include "cPacket_InventoryProgressBar.h"

bool cPacket_InventoryProgressBar::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;

	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte		( (char)m_PacketID,		Message, i );
	AppendByte		( m_WindowID,		Message, i );
	AppendShort		( m_ProgressBar,	Message, i );
	AppendShort		( m_Value,			Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}