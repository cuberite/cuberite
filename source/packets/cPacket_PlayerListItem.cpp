#include "cPacket_PlayerListItem.h"

bool cPacket_PlayerListItem::Parse( cSocket & a_Socket )
{
	m_Socket = a_Socket;
	if (!ReadString(m_PlayerName)) return false;
	if (!ReadBool(m_Online)) return false;
	if (!ReadShort(m_Ping)) return false;
	return true;
}

bool cPacket_PlayerListItem::Send( cSocket & a_Socket )
{
	m_PlayerName = m_PlayerName.substr(0,16);
	unsigned int TotalSize = c_Size + m_PlayerName.size()*sizeof(short);
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte((char)m_PacketID, Message, i);
	AppendString16(m_PlayerName,   Message, i);
	AppendBool(m_Online, Message, i);
	AppendShort(m_Ping, Message, i);

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
