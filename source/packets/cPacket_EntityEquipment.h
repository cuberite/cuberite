#pragma once

#include "cPacket.h"





class cPacket_EntityEquipment : public cPacket
{
public:
	cPacket_EntityEquipment()
		: m_UniqueID( 0 )
		, m_Slot( 0 )
		, m_ItemID( 0 )
		, m_Short( 0 )
	{ m_PacketID = E_ENTITY_EQUIPMENT; m_Short = 0;  }
	cPacket_EntityEquipment( const cPacket_EntityEquipment & a_Copy );
	virtual cPacket* Clone() const { return new cPacket_EntityEquipment(*this); }

	virtual int Parse(const char * a_Data, int a_Size) override;
	virtual void Serialize(AString & a_Data) const override;

	int m_UniqueID;
	short m_Slot; // 0 = hold 1-4 = armor
	short m_ItemID;
	short m_Short;

	static const unsigned int c_Size = 1 + 4 + 2 + 2 + 2;
};




