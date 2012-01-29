
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_NewInvalidState.h"





cPacket_NewInvalidState::cPacket_NewInvalidState( const cPacket_NewInvalidState & a_Copy )
{
	m_PacketID = E_NEW_INVALID_STATE;
	m_Reason = a_Copy.m_Reason;
	m_GameMode = a_Copy.m_GameMode;
}

bool cPacket_NewInvalidState::Parse(cSocket & a_Socket) {
	m_Socket = a_Socket;
	if( !ReadByte	( m_Reason ) ) return false;
	if( !ReadByte	( m_GameMode ) ) return false;
	return true;
}

bool cPacket_NewInvalidState::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	( (char)m_PacketID,	Message, i );
	AppendByte ( m_Reason, Message, i );
	AppendByte ( m_GameMode, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
