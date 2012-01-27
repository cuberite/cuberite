#pragma once

#include "cPacket.h"

class cPacket_WindowOpen : public cPacket
{
public:
	cPacket_WindowOpen()
		: m_WindowID( 0 )
		, m_InventoryType( 0 )
		, m_NumSlots( 0 )
	{ m_PacketID = E_WINDOW_OPEN; }
	virtual cPacket* Clone() const { return new cPacket_WindowOpen(*this); }

	bool Send(cSocket & a_Socket);

	char m_WindowID;
	char m_InventoryType;
	std::string m_WindowTitle;
	char m_NumSlots;

	static const unsigned int c_Size = 1 + 1 + 1 + 2 + 1; // + sizeof(string)
};