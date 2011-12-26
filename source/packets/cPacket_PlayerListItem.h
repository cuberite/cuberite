#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_PlayerListItem : public cPacket
{
public:
	cPacket_PlayerListItem() { m_PacketID = E_PLAYER_LIST_ITEM; }

	bool Parse(cSocket & a_Socket);
	bool Send(cSocket & a_Socket);

	virtual cPacket* Clone() const { return new cPacket_PlayerListItem(*this); }

	std::string m_PlayerName; // Supports chat coloring, limited to 16 characters.
	bool m_Online;
	short m_Ping;

	static const unsigned int c_Size = 6; // Minimal size ( 6 + string )
};