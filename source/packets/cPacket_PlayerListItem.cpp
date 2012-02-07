
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_PlayerListItem.h"
#include "../cChatColor.h"





cPacket_PlayerListItem::cPacket_PlayerListItem(const AString & a_PlayerName, bool a_Online, short a_Ping)
{
	m_PacketID = E_PLAYER_LIST_ITEM;
	m_PlayerName = a_PlayerName;
	m_Online = a_Online;
	m_Ping = a_Ping;
}





int cPacket_PlayerListItem::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadString16, m_PlayerName, TotalBytes);
	HANDLE_PACKET_READ(ReadBool,     m_Online,     TotalBytes);
	HANDLE_PACKET_READ(ReadShort,    m_Ping,       TotalBytes);
	return TotalBytes;
}





void cPacket_PlayerListItem::Serialize(AString & a_Data) const
{
	AString PlayerName(m_PlayerName);
	if (PlayerName.length() > 16)
	{
		PlayerName.erase(16);
	}
	else if (PlayerName.length() <= 14)
	{
		PlayerName += cChatColor::White;
	}

	AppendByte    (a_Data, m_PacketID);
	AppendString16(a_Data, PlayerName);
	AppendBool    (a_Data, m_Online);
	AppendShort   (a_Data, m_Ping);
}




