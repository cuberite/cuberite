
#pragma once

#include "cPacket.h"





class cPacket_PlayerListItem : public cPacket
{
public:
	cPacket_PlayerListItem() { m_PacketID = E_PLAYER_LIST_ITEM; }
	cPacket_PlayerListItem(const AString & a_PlayerName, bool a_Online, short a_Ping);

	virtual int Parse(const char * a_Data, int a_Size) override;
	virtual void Serialize(AString & a_Data) const override;

	virtual cPacket* Clone() const { return new cPacket_PlayerListItem(*this); }

	AString m_PlayerName; // Supports chat coloring, limited to 16 characters.
	bool    m_Online;
	short   m_Ping;

	static const unsigned int c_Size = 6; // Minimal size ( 6 + string )
};




