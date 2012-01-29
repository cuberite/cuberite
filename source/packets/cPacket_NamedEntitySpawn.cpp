
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_NamedEntitySpawn.h"





bool cPacket_NamedEntitySpawn::Send( cSocket & a_Socket )
{
	unsigned int TotalSize = c_Size + m_PlayerName.size() * sizeof( short );
	char* Message = new char[TotalSize];

	if( m_CurrentItem <= 0 ) m_CurrentItem = 0; // Fix, to make sure no invalid values are sent.
	// WARNING: HERE ITS 0, BUT IN EQUIP PACKET ITS -1 !!

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,		Message, i );
	AppendInteger( m_UniqueID,		Message, i );
	AppendString16( m_PlayerName,	Message, i );
	AppendInteger( m_PosX,			Message, i );
	AppendInteger( m_PosY,			Message, i );
	AppendInteger( m_PosZ,			Message, i );
	AppendByte   ( m_Rotation,		Message, i );
	AppendByte   ( m_Pitch,			Message, i );
	AppendShort  ( m_CurrentItem,	Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
