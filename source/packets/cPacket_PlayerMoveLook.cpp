
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_PlayerMoveLook.h"
#include "../cPlayer.h"





cPacket_PlayerMoveLook::cPacket_PlayerMoveLook( cPlayer* a_Player )
{
	m_PacketID = E_PLAYERMOVELOOK;
	m_PosX     = a_Player->GetPosX();
	m_PosY     = a_Player->GetPosY() + 1.65;
	m_PosZ     = a_Player->GetPosZ();
	m_Stance   = a_Player->GetStance();
	m_Rotation = a_Player->GetRotation();
	m_Pitch    = a_Player->GetPitch();
	m_bFlying  = a_Player->GetFlying();
}





int cPacket_PlayerMoveLook::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadDouble, m_PosX,     TotalBytes);
	HANDLE_PACKET_READ(ReadDouble, m_PosY,     TotalBytes);
	HANDLE_PACKET_READ(ReadDouble, m_Stance,   TotalBytes);
	HANDLE_PACKET_READ(ReadDouble, m_PosZ,     TotalBytes);
	HANDLE_PACKET_READ(ReadFloat,  m_Rotation, TotalBytes);
	HANDLE_PACKET_READ(ReadFloat,  m_Pitch,    TotalBytes);
	HANDLE_PACKET_READ(ReadBool,   m_bFlying,  TotalBytes);
	return TotalBytes;
}





void cPacket_PlayerMoveLook::Serialize(AString & a_Data) const
{
	AppendByte	(a_Data, m_PacketID);
	AppendDouble(a_Data, m_PosX);
	AppendDouble(a_Data, m_PosY);
	AppendDouble(a_Data, m_Stance);
	AppendDouble(a_Data, m_PosZ);
	AppendFloat	(a_Data, m_Rotation);
	AppendFloat	(a_Data, m_Pitch);
	AppendBool	(a_Data, m_bFlying);
}




