#include "cPacket_PickupSpawn.h"

bool cPacket_PickupSpawn::Parse( cSocket & a_Socket )
{
	m_Socket = a_Socket;
	if( !ReadInteger( m_UniqueID ) ) return false;
	if( !ReadShort	( m_Item	 ) ) return false;
	if( !ReadByte	( m_Count	 ) ) return false;
	if( !ReadShort	( m_Health	 ) ) return false;
	if( !ReadInteger( m_PosX	 ) ) return false;
	if( !ReadInteger( m_PosY	 ) ) return false;
	if( !ReadInteger( m_PosZ	 ) ) return false;
	if( !ReadByte	( m_Rotation ) ) return false;
	if( !ReadByte	( m_Pitch	 ) ) return false;
	if( !ReadByte	( m_Roll	 ) ) return false;
	return true;
}

bool cPacket_PickupSpawn::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendInteger( m_UniqueID,	Message, i );
	AppendShort  ( m_Item,		Message, i );
	AppendByte	 ( m_Count,		Message, i );
	AppendShort	 ( m_Health,	Message, i );
	AppendInteger( m_PosX,		Message, i );
	AppendInteger( m_PosY,		Message, i );
	AppendInteger( m_PosZ,		Message, i );
	AppendByte	 ( m_Rotation,	Message, i );
	AppendByte	 ( m_Pitch,		Message, i );
	AppendByte	 ( m_Roll,		Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}