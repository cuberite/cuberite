
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_PlayerListItem.h"
#include "../cChatColor.h"





cPacket_PlayerListItem::cPacket_PlayerListItem(std::string a_PlayerName, bool a_Online, short a_Ping)
{
	m_PacketID = E_PLAYER_LIST_ITEM;
	m_PlayerName = a_PlayerName;
	m_Online = a_Online;
	m_Ping = a_Ping;
}

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
	int len = m_PlayerName.length();
	int end = (len <= 16) ? len : 16;
	m_PlayerName = m_PlayerName.substr(0, end);
	if (len <= 14)
		m_PlayerName += cChatColor::White; // mistakes happen when you code late night :P

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
