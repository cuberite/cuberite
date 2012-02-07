
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_EntityEquipment.h"





cPacket_EntityEquipment::cPacket_EntityEquipment( const cPacket_EntityEquipment & a_Copy )
{
	m_PacketID = E_ENTITY_EQUIPMENT;
	m_UniqueID = a_Copy.m_UniqueID;
	m_Slot	 = a_Copy.m_Slot;
	m_ItemID = a_Copy.m_ItemID;
	m_Short = 0;
}





int cPacket_EntityEquipment::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadInteger, m_UniqueID, TotalBytes);
	HANDLE_PACKET_READ(ReadShort,   m_Slot,     TotalBytes);
	HANDLE_PACKET_READ(ReadShort,   m_ItemID,   TotalBytes);
	HANDLE_PACKET_READ(ReadShort,   m_Short,    TotalBytes);
	return TotalBytes;
}





void cPacket_EntityEquipment::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
	AppendShort  (a_Data, m_Slot);
	AppendShort  (a_Data, m_ItemID);
	AppendShort  (a_Data, m_Short);
}




