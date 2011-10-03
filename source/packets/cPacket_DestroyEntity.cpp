#include "cPacket_DestroyEntity.h"
#include "cEntity.h"

cPacket_DestroyEntity::cPacket_DestroyEntity(cEntity* a_Entity)
{
	m_PacketID = E_DESTROY_ENT;

	m_UniqueID = a_Entity->GetUniqueID();
}

bool cPacket_DestroyEntity::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendInteger( m_UniqueID,	Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
