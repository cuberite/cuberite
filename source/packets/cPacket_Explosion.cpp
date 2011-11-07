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

bool cPacket_Explosion::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size + (m_RecordsCount * 3);
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte    ( (char)m_PacketID, 							Message, i );
	AppendDouble	( m_PosX,													Message, i );
	AppendDouble	( m_PosY,													Message, i );
	AppendDouble	( m_PosZ,													Message, i );
	AppendFloat		( m_Radius, 											Message, i );
	AppendInteger	( m_RecordsCount,									Message, i );
	AppendData		( m_Records, (m_RecordsCount * 3),Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
