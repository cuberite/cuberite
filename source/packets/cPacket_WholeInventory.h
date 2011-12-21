#pragma once

#include "cPacket.h"
#include "PacketID.h"
#include "BlockID.h"

class cInventory;
class cWindow;
class cItem;
class cPacket_WholeInventory : public cPacket // full inventory [S -> C] ?
{
public:
	cPacket_WholeInventory( const cPacket_WholeInventory & a_Clone );
	cPacket_WholeInventory( cInventory* a_Inventory );
	cPacket_WholeInventory( cWindow* a_Window );
	~cPacket_WholeInventory();
	cPacket_WholeInventory()
		: m_WindowID( 0 )
		, m_Count( 0 )
		, m_Items( 0 )
	{ m_PacketID = E_INVENTORY_WHOLE; }


	virtual cPacket* Clone() const { return new cPacket_WholeInventory(*this); }

	bool Send(cSocket & a_Socket);

	char m_WindowID;	// WTF?
	short m_Count;	// Number of items

	cItem* m_Items; // Array of m_Count items

	static const unsigned int c_Size = 1 + 1 + 2; // Minimal size
};