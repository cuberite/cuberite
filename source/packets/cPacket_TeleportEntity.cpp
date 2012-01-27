#include "cPacket_TeleportEntity.h"

#include "../cEntity.h"

cPacket_TeleportEntity::cPacket_TeleportEntity(cEntity* a_Client)
{
	m_PacketID = E_ENT_TELEPORT;

	m_UniqueID = a_Client->GetUniqueID();
	m_PosX = (int)(a_Client->GetPosX() * 32);
	m_PosY = (int)(a_Client->GetPosY() * 32);
	m_PosZ = (int)(a_Client->GetPosZ() * 32);
	m_Rotation = (char)((a_Client->GetRotation()/360.f)*256);
	m_Pitch    = (char)((a_Client->GetPitch()/360.f)*256);
}

bool cPacket_TeleportEntity::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,		Message, i );
	AppendInteger( m_UniqueID,		Message, i );
	AppendInteger( m_PosX,			Message, i );
	AppendInteger( m_PosY,			Message, i );
	AppendInteger( m_PosZ,			Message, i );
	AppendByte   ( m_Rotation,		Message, i );
	AppendByte   ( m_Pitch,			Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
