
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_SpawnMob.h"
#include "../Vector3i.h"





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
	, m_HeadYaw(0)
{
	m_PacketID = E_SPAWN_MOB;
}





cPacket_SpawnMob::cPacket_SpawnMob( const cPacket_SpawnMob & a_Clone )
{
	m_Pos = new Vector3i();

	m_PacketID = E_SPAWN_MOB;
	m_UniqueID = a_Clone.m_UniqueID;
	m_Type     = a_Clone.m_Type;
	*m_Pos     = *a_Clone.m_Pos;
	m_Yaw      = a_Clone.m_Yaw;
	m_Pitch    = a_Clone.m_Pitch;
	m_HeadYaw  = a_Clone.m_HeadYaw;

	m_MetaDataSize = a_Clone.m_MetaDataSize;
	m_MetaData = new char[m_MetaDataSize];
	memcpy( m_MetaData, a_Clone.m_MetaData, sizeof( char ) * m_MetaDataSize );
}





void cPacket_SpawnMob::Serialize(AString & a_Data) const
{
	AppendByte		(a_Data, m_PacketID);
	AppendInteger	(a_Data, m_UniqueID);
	AppendByte		(a_Data, m_Type);
	AppendInteger	(a_Data, m_Pos->x);
	AppendInteger	(a_Data, m_Pos->y);
	AppendInteger	(a_Data, m_Pos->z);
	AppendByte		(a_Data, m_Yaw);
	AppendByte		(a_Data, m_Pitch);
	AppendByte    (a_Data, m_HeadYaw);
	AppendData		(a_Data, m_MetaData, m_MetaDataSize);
}




