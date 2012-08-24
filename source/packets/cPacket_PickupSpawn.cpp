
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_PickupSpawn.h"





int cPacket_PickupSpawn::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEInt,   m_UniqueID,   TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort, m_ItemType,   TotalBytes);
	HANDLE_PACKET_READ(ReadChar,    m_ItemCount,  TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort, m_ItemDamage, TotalBytes);
	HANDLE_PACKET_READ(ReadBEInt,   m_PosX,       TotalBytes);
	HANDLE_PACKET_READ(ReadBEInt,   m_PosY,       TotalBytes);
	HANDLE_PACKET_READ(ReadBEInt,   m_PosZ,       TotalBytes);
	HANDLE_PACKET_READ(ReadChar,    m_Rotation,   TotalBytes);
	HANDLE_PACKET_READ(ReadChar,    m_Pitch,      TotalBytes);
	HANDLE_PACKET_READ(ReadChar,    m_Roll,       TotalBytes);
	return TotalBytes;
}





void cPacket_PickupSpawn::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
	AppendShort  (a_Data, m_ItemType);
	AppendByte   (a_Data, m_ItemCount);
	AppendShort  (a_Data, m_ItemDamage);
	AppendInteger(a_Data, m_PosX);
	AppendInteger(a_Data, m_PosY);
	AppendInteger(a_Data, m_PosZ);
	AppendByte   (a_Data, m_Rotation);
	AppendByte   (a_Data, m_Pitch);
	AppendByte   (a_Data, m_Roll);
}




