
#pragma once

#include "cPacket.h"

#include "../BlockID.h"





class cInventory;
class cWindow;
class cItem;





class cPacket_WholeInventory :
	public cPacket // full inventory [S -> C] ?
{
public:
	cPacket_WholeInventory(const cPacket_WholeInventory & a_Clone);
	cPacket_WholeInventory(const cInventory & a_Inventory);
	cPacket_WholeInventory(const cWindow & a_Window);
	~cPacket_WholeInventory();
	cPacket_WholeInventory()
		: m_WindowID( 0 )
		, m_Count( 0 )
		, m_Items( 0 )
	{
		m_PacketID = E_INVENTORY_WHOLE;
	}

	virtual cPacket * Clone() const { return new cPacket_WholeInventory(*this); }

	virtual void Serialize(AString & a_Data) const override;

	char    m_WindowID;
	short   m_Count;
	cItem * m_Items;
};




