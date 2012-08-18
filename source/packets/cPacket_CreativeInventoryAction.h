
#pragma once

#include "cPacket.h"
#include "../cItem.h"





class cPacket_CreativeInventoryAction :
	public cPacket
{
public:
	cPacket_CreativeInventoryAction() :
		m_SlotNum(0)
	{
		m_PacketID = E_CREATIVE_INVENTORY_ACTION;
	}
	
	cPacket_CreativeInventoryAction( const cPacket_CreativeInventoryAction & a_Copy );
	virtual cPacket* Clone() const { return new cPacket_CreativeInventoryAction(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	short m_SlotNum;
	cItem m_ClickedItem;
} ;




