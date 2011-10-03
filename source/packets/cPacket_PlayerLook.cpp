#include "cPacket_PlayerLook.h"
#include "../cPlayer.h"

cPacket_PlayerLook::cPacket_PlayerLook( cPlayer* a_Player )
{
	m_PacketID = E_PLAYERLOOK;

	m_Rotation = a_Player->GetRotation();
	m_Pitch = a_Player->GetPitch();
	m_bFlying = a_Player->GetFlying();
}

bool cPacket_PlayerLook::Parse( cSocket & a_Socket )
{
	m_Socket = a_Socket;
	if( !ReadFloat( m_Rotation ) )  return false;
	if( !ReadFloat( m_Pitch ) )		return false;
	if( !ReadBool ( m_bFlying ) )	return false;
	return true;
}

bool cPacket_PlayerLook::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	( (char)m_PacketID,	Message, i );
	AppendFloat	( m_Rotation,	Message, i );
	AppendFloat	( m_Pitch,		Message, i );
	AppendBool	( m_bFlying,	Message, i );

	bool RetVal = !cSocket::IsSocketError( cPacket::SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}