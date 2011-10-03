#include "cPacket_Metadata.h"

cPacket_Metadata::cPacket_Metadata(int s, int id)
	: EMetaState( (MetaState)s )
	, m_UniqueID( id )
	, m_Type( 0 )
	, m_MetaData( 0 )
{
	m_PacketID = E_METADATA;
	FormPacket();
}

cPacket_Metadata::cPacket_Metadata()
	: EMetaState(NORMAL)
	, m_UniqueID( 0 )
	, m_Type( 0 )
	, m_MetaData( 0 )
{
	m_PacketID = E_METADATA;
	FormPacket();
}

cPacket_Metadata::~cPacket_Metadata() {
	//if( m_MetaData ) delete [] m_MetaData;
}

void cPacket_Metadata::FormPacket() {
	if( m_MetaData ) delete [] m_MetaData;
	m_MetaData = new char[3];
	m_MetaDataSize = 3;
	//m_UniqueID = GetUniqueID();
	m_MetaData[0] = 0x00;
	//m_MetaData[1] = 0x01; //Burning
	m_MetaData[2] = 0x7f;
	switch(EMetaState) {
		case NORMAL:
			m_MetaData[1] = 0x00;
			break;
		case BURNING:
			m_MetaData[1] = 0x01;
			break;
		case CROUCHED:
			m_MetaData[1] = 0x02;
			break;
		case RIDING:
			m_MetaData[1] = 0x04;
			break;
		default:
			m_MetaData[1] = 0x00;
			break;
	}
}

bool cPacket_Metadata::Send(cSocket & a_Socket) {
	unsigned int TotalSize = c_Size + m_MetaDataSize;
	char* Message = new char[TotalSize];
	unsigned int i = 0;

	AppendByte		( (char)m_PacketID,	Message, i );
	AppendInteger	( m_UniqueID,	Message, i );
	AppendData		( m_MetaData, m_MetaDataSize, Message, i );

	bool RetVal = !cSocket::IsSocketError( SendData( a_Socket, Message, TotalSize, 0 ) );
	return RetVal;
}
