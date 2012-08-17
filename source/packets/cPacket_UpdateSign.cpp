
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_UpdateSign.h"





int cPacket_UpdateSign::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEInt,           m_PosX, TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort,         m_PosY, TotalBytes);
	HANDLE_PACKET_READ(ReadBEInt,           m_PosZ, TotalBytes);
	HANDLE_PACKET_READ(ReadBEUTF16String16, m_Line1, TotalBytes);
	HANDLE_PACKET_READ(ReadBEUTF16String16, m_Line2, TotalBytes);
	HANDLE_PACKET_READ(ReadBEUTF16String16, m_Line3, TotalBytes);
	HANDLE_PACKET_READ(ReadBEUTF16String16, m_Line4, TotalBytes);
	return TotalBytes;
}





void cPacket_UpdateSign::Serialize(AString & a_Data) const
{
	AppendByte    (a_Data, m_PacketID);
	AppendInteger (a_Data, m_PosX);
	AppendShort   (a_Data, m_PosY);
	AppendInteger (a_Data, m_PosZ);
	AppendString16(a_Data, m_Line1);
	AppendString16(a_Data, m_Line2);
	AppendString16(a_Data, m_Line3);
	AppendString16(a_Data, m_Line4);
}




