
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_EntityEquipment.h"





cPacket_EntityEquipment::cPacket_EntityEquipment( const cPacket_EntityEquipment & a_Copy )
{
	m_PacketID   = E_ENTITY_EQUIPMENT;
	m_UniqueID   = a_Copy.m_UniqueID;
	m_SlotNum    = a_Copy.m_SlotNum;
	m_ItemType   = a_Copy.m_ItemType;
	m_ItemDamage = a_Copy.m_ItemDamage;
}





int cPacket_EntityEquipment::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEInt,   m_UniqueID,   TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort, m_SlotNum,    TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort, m_ItemType,   TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort, m_ItemDamage, TotalBytes);
	return TotalBytes;
}





void cPacket_EntityEquipment::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
	AppendShort  (a_Data, m_SlotNum);
	AppendShort  (a_Data, m_ItemType);
	AppendShort  (a_Data, m_ItemDamage);
}




