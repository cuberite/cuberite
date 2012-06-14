
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_PickupSpawn.h"





int cPacket_PickupSpawn::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadInteger, m_UniqueID, TotalBytes);
	HANDLE_PACKET_READ(ReadShort,   m_Item,     TotalBytes);
	HANDLE_PACKET_READ(ReadByte,    m_Count,    TotalBytes);
	HANDLE_PACKET_READ(ReadShort,   m_Health,   TotalBytes);
	HANDLE_PACKET_READ(ReadInteger, m_PosX,     TotalBytes);
	HANDLE_PACKET_READ(ReadInteger, m_PosY,     TotalBytes);
	HANDLE_PACKET_READ(ReadInteger, m_PosZ,     TotalBytes);
	HANDLE_PACKET_READ(ReadByte,    m_Rotation, TotalBytes);
	HANDLE_PACKET_READ(ReadByte,    m_Pitch,    TotalBytes);
	HANDLE_PACKET_READ(ReadByte,    m_Roll,     TotalBytes);
	return TotalBytes;
}





void cPacket_PickupSpawn::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
	AppendShort  (a_Data, m_Item);
	AppendByte   (a_Data, m_Count);
	AppendShort  (a_Data, m_Health);
	AppendInteger(a_Data, m_PosX);
	AppendInteger(a_Data, m_PosY);
	AppendInteger(a_Data, m_PosZ);
	AppendByte   (a_Data, m_Rotation);
	AppendByte   (a_Data, m_Pitch);
	AppendByte   (a_Data, m_Roll);
}




