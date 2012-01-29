
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_EntityEquipment.h"





cPacket_EntityEquipment::cPacket_EntityEquipment( const cPacket_EntityEquipment & a_Copy )
{
	m_PacketID = E_ENTITY_EQUIPMENT;
	m_UniqueID = a_Copy.m_UniqueID;
	m_Slot	 = a_Copy.m_Slot;
	m_ItemID = a_Copy.m_ItemID;
	m_Short = 0;
}

bool cPacket_EntityEquipment::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;
	if( !ReadInteger( m_UniqueID ) ) return false;
	if( !ReadShort	( m_Slot ) ) return false;
	if( !ReadShort	( m_ItemID ) ) return false;
	if( !ReadShort	( m_Short ) ) return false;
	return true;
}

bool cPacket_EntityEquipment::Send(cSocket & a_Socket)
{
	//LOG("InventoryChange:");
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	if( m_ItemID <= 0 ) m_ItemID = -1; // Fix, to make sure no invalid values are sent.
                        // WARNING: HERE ITS -1, BUT IN NAMED ENTITY SPAWN PACKET ITS 0 !!
	//LOG("cPacket_EntityEquipment: Sending equipped item ID: %i", m_ItemID );

	unsigned int i = 0;
	AppendByte	 ( (char)m_PacketID,	Message, i );
	AppendInteger( m_UniqueID,	Message, i );
	AppendShort  ( m_Slot,		Message, i );
	AppendShort  ( m_ItemID,	Message, i );
	AppendShort	 ( m_Short,		Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}