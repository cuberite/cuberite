
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Explosion.h"





cPacket_Explosion::~cPacket_Explosion()
{
	if( m_Records )
	{
		delete [] m_Records;
	}
}





cPacket_Explosion::cPacket_Explosion( const cPacket_Explosion & a_Copy )
{
	m_PacketID		= E_EXPLOSION;
	m_PosX				= a_Copy.m_PosX;
	m_PosY				= a_Copy.m_PosY;
	m_PosZ				= a_Copy.m_PosZ;
	m_Radius			= a_Copy.m_Radius; //might not be radius
	m_RecordsCount= a_Copy.m_RecordsCount;

	m_Records			= new char[(m_RecordsCount * 3)];
	memcpy( m_Records, a_Copy.m_Records, (m_RecordsCount * 3) );
}





void cPacket_Explosion::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendDouble (a_Data, m_PosX);
	AppendDouble (a_Data, m_PosY);
	AppendDouble (a_Data, m_PosZ);
	AppendFloat  (a_Data, m_Radius);
	AppendInteger(a_Data, m_RecordsCount);
	AppendData   (a_Data, m_Records, (m_RecordsCount * 3));
}




