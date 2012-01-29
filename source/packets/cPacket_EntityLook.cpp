
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_EntityLook.h"

#include "../cEntity.h"





cPacket_EntityLook::cPacket_EntityLook(cEntity* a_Entity)
{
	m_PacketID = E_ENT_LOOK;

	m_UniqueID = a_Entity->GetUniqueID();
	m_Rotation = (char)((a_Entity->GetRotation()/360.f)*256);
	m_Pitch    = (char)((a_Entity->GetPitch()/360.f)*256);
}

bool cPacket_EntityLook::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,		Message, i );
	AppendInteger( m_UniqueID,		Message, i );
	AppendByte   ( m_Rotation,		Message, i );
	AppendByte   ( m_Pitch,			Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
