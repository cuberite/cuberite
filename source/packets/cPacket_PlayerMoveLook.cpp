
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_PlayerMoveLook.h"
#include "../cPlayer.h"





cPacket_PlayerMoveLook::cPacket_PlayerMoveLook( cPlayer* a_Player )
{
	m_PacketID = E_PLAYERMOVELOOK;


	m_PosX		= a_Player->GetPosX();
	m_PosY		= a_Player->GetPosY() + 1.65;
	m_PosZ		= a_Player->GetPosZ();
	m_Stance	= a_Player->GetStance();
	m_Rotation	= a_Player->GetRotation();
	m_Pitch		= a_Player->GetPitch();
	m_bFlying	= a_Player->GetFlying();
}

bool cPacket_PlayerMoveLook::Parse( cSocket & a_Socket )
{
	m_Socket = a_Socket;

	if( !ReadDouble( m_PosX ) ) return false;
	if( !ReadDouble( m_PosY ) ) return false;
	if( !ReadDouble( m_Stance ) ) return false;
	if( !ReadDouble( m_PosZ ) ) return false;
	if( !ReadFloat ( m_Rotation ) ) return false;
	if( !ReadFloat ( m_Pitch ) ) return false;
	if( !ReadBool  ( m_bFlying ) ) return false;
	return true;
}

bool cPacket_PlayerMoveLook::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	( (char)m_PacketID,	Message, i );
	AppendDouble( m_PosX,		Message, i );
	AppendDouble( m_PosY,		Message, i );
	AppendDouble( m_Stance,		Message, i );
	AppendDouble( m_PosZ,		Message, i );
	AppendFloat	( m_Rotation,	Message, i );
	AppendFloat	( m_Pitch,		Message, i );
	AppendBool	( m_bFlying,	Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
