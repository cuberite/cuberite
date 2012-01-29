
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_ItemSwitch.h"





bool cPacket_ItemSwitch::Parse( cSocket & a_Socket )
{
	m_Socket = a_Socket;

	if( !ReadShort  ( m_SlotNum ) ) return false;
	return true;
}

bool cPacket_ItemSwitch::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID, Message, i );
	AppendShort  ( m_SlotNum, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}