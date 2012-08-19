#pragma once

#include "cPacket.h"





class cPacket_EntityEquipment : public cPacket
{
public:
	cPacket_EntityEquipment()
		: m_UniqueID( 0 )
		, m_SlotNum( 0 )
		, m_ItemType( 0 )
		, m_ItemDamage( 0 )
	{
		m_PacketID = E_ENTITY_EQUIPMENT;
	}
	
	cPacket_EntityEquipment( const cPacket_EntityEquipment & a_Copy );
	virtual cPacket * Clone() const { return new cPacket_EntityEquipment(*this); }

	virtual int  Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	int m_UniqueID;
	short m_SlotNum; // 0 = hold 1-4 = armor
	short m_ItemType;
	short m_ItemDamage;
};




