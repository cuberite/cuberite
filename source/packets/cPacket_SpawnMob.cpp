#include "cPacket_SpawnMob.h"
#include "../Vector3i.h"

#include "../cMCLogger.h"

cPacket_SpawnMob::~cPacket_SpawnMob()
{
	if( m_MetaData ) delete [] m_MetaData;
	delete m_Pos;
}

cPacket_SpawnMob::cPacket_SpawnMob()
	: m_UniqueID( 0 )
	, m_Type( 0 )
	, m_Pos( new Vector3i() )
    , m_Yaw( 0 )
	, m_Pitch( 0 )
	, m_MetaDataSize( 0 )
	, m_MetaData( 0 )
{
	m_PacketID = E_SPAWN_MOB;
}

cPacket_SpawnMob::cPacket_SpawnMob( const cPacket_SpawnMob & a_Clone )
{
	m_Pos = new Vector3i();

	m_PacketID = E_SPAWN_MOB;
	m_UniqueID = a_Clone.m_UniqueID;
	m_Type = a_Clone.m_Type;
	*m_Pos = *a_Clone.m_Pos;
	m_Yaw = a_Clone.m_Yaw;
	m_Pitch = a_Clone.m_Pitch;

	m_MetaDataSize = a_Clone.m_MetaDataSize;
	m_MetaData = new char[m_MetaDataSize];
	memcpy( m_MetaData, a_Clone.m_MetaData, sizeof( char ) * m_MetaDataSize );
}

bool cPacket_SpawnMob::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size + m_MetaDataSize;

	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte		( (char)m_PacketID,	Message, i );
	AppendInteger	( m_UniqueID,	Message, i );
	AppendByte		( m_Type,		Message, i );
	AppendInteger	( m_Pos->x,		Message, i );
	AppendInteger	( m_Pos->y,		Message, i );
	AppendInteger	( m_Pos->z,		Message, i );
	AppendByte		( m_Yaw,		Message, i );
	AppendByte		( m_Pitch,		Message, i );
	AppendData		( m_MetaData, m_MetaDataSize, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
