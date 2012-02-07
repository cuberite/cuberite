
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_PlayerLook.h"
#include "../cPlayer.h"





cPacket_PlayerLook::cPacket_PlayerLook( cPlayer* a_Player )
{
	m_PacketID = E_PLAYERLOOK;
	m_Rotation = a_Player->GetRotation();
	m_Pitch    = a_Player->GetPitch();
	m_bFlying  = a_Player->GetFlying();
}





int cPacket_PlayerLook::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadFloat, m_Rotation, TotalBytes);
	HANDLE_PACKET_READ(ReadFloat, m_Pitch,    TotalBytes);
	HANDLE_PACKET_READ(ReadBool,  m_bFlying,  TotalBytes);
	return TotalBytes;
}





void cPacket_PlayerLook::Serialize(AString & a_Data) const
{
	AppendByte	(a_Data, m_PacketID);
	AppendFloat	(a_Data, m_Rotation);
	AppendFloat	(a_Data, m_Pitch);
	AppendBool	(a_Data, m_bFlying);
}




