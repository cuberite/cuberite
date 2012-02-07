
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_NewInvalidState.h"





cPacket_NewInvalidState::cPacket_NewInvalidState( const cPacket_NewInvalidState & a_Copy )
{
	m_PacketID = E_NEW_INVALID_STATE;
	m_Reason   = a_Copy.m_Reason;
	m_GameMode = a_Copy.m_GameMode;
}





int cPacket_NewInvalidState::Parse(const char * a_Data, int a_Size)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadByte, m_Reason, TotalBytes);
	HANDLE_PACKET_READ(ReadByte, m_GameMode, TotalBytes);
	return TotalBytes;
}





void cPacket_NewInvalidState::Serialize(AString & a_Data) const
{
	AppendByte(a_Data, m_PacketID);
	AppendByte(a_Data, m_Reason);
	AppendByte(a_Data, m_GameMode);
}




