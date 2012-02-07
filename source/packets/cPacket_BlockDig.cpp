
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_BlockDig.h"





void cPacket_BlockDig::Serialize(AString & a_Data) const
{
	AppendByte	 (a_Data, m_PacketID);
	AppendInteger(a_Data, m_PosX);
	AppendByte	 (a_Data, m_PosY);
	AppendInteger(a_Data, m_PosZ);
	AppendByte   (a_Data, m_Direction);
}





int cPacket_BlockDig::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadByte,	  m_Status,    TotalBytes);
	HANDLE_PACKET_READ(ReadInteger, m_PosX,      TotalBytes);
	HANDLE_PACKET_READ(ReadByte,	  m_PosY,      TotalBytes);
	HANDLE_PACKET_READ(ReadInteger, m_PosZ,      TotalBytes);
	HANDLE_PACKET_READ(ReadByte,	  m_Direction, TotalBytes);
	return TotalBytes;
}




