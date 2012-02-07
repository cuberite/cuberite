
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_PlayerPosition.h"
#include "../cPlayer.h"





cPacket_PlayerPosition::cPacket_PlayerPosition( cPlayer* a_Player )
{
	m_PacketID = E_PLAYERPOS;

	m_PosX = a_Player->GetPosX();
	m_PosY = a_Player->GetPosY() + 1.65;
	m_PosZ = a_Player->GetPosZ();
	m_Stance = a_Player->GetStance();
	m_bFlying = a_Player->GetFlying();
}





int cPacket_PlayerPosition::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadDouble, m_PosX,    TotalBytes);
	HANDLE_PACKET_READ(ReadDouble, m_PosY,    TotalBytes);
	HANDLE_PACKET_READ(ReadDouble, m_Stance,  TotalBytes);
	HANDLE_PACKET_READ(ReadDouble, m_PosZ,    TotalBytes);
	HANDLE_PACKET_READ(ReadBool,   m_bFlying, TotalBytes);
	return TotalBytes;
}





void cPacket_PlayerPosition::Serialize(AString & a_Data) const
{
	AppendByte	 (a_Data, m_PacketID);
	AppendDouble (a_Data, m_PosX);
	AppendDouble (a_Data, m_PosY);
	AppendDouble (a_Data, m_Stance);
	AppendDouble (a_Data, m_PosZ);
	AppendBool	 (a_Data, m_bFlying);
}




