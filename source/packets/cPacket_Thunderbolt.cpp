#include "cPacket_Thunderbolt.h"

cPacket_Thunderbolt::cPacket_Thunderbolt( const cPacket_Thunderbolt & a_Copy )
{
        m_PacketID = E_THUNDERBOLT;
        m_UniqueID = 237183; //just a random Ent ID. I don't think this matters at all.
	m_Unknown  = true;
        m_xLBPos   = a_Copy.m_xLBPos;
        m_yLBPos   = a_Copy.m_yLBPos;
        m_zLBPos   = a_Copy.m_zLBPos;
printf("blot-packet %i %i %i\n", m_xLBPos,m_yLBPos,m_zLBPos);



}


bool cPacket_Thunderbolt::Send(cSocket & a_Socket)
{
	unsigned int TotalSize = c_Size;
	char* Message = new char[TotalSize];

	unsigned int i = 0;
	AppendByte    ( (char)m_PacketID, Message, i );
	AppendInteger ( m_UniqueID, Message, i );
	AppendBool    ( m_Unknown, Message, i );
	AppendInteger ( m_xLBPos*32, Message, i );
	AppendInteger ( m_yLBPos*32, Message, i );
	AppendInteger ( m_zLBPos*32, Message, i );

	for( unsigned int iii = 1; iii < TotalSize; ++iii ){

	printf("packetdata %i\n", (int)Message[iii]);
	}

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	delete [] Message;
	return RetVal;
}
