
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_Metadata.h"





cPacket_Metadata::cPacket_Metadata(int s, int id)
	: m_EMetaData( (cPawn::MetaData)s )
	, m_UniqueID( id )
	, m_Type( 0 )
	, m_MetaData( 0 )
{
	m_PacketID = E_METADATA;
	FormPacket();
}





cPacket_Metadata::cPacket_Metadata()
	: m_UniqueID( 0 )
	, m_Type( 0 )
	, m_MetaData( 0 )
{
	m_PacketID = E_METADATA;
	m_EMetaData = cPawn::NORMAL;
	FormPacket();
}





cPacket_Metadata::cPacket_Metadata(const cPacket_Metadata & a_Other) 
	: m_EMetaData( a_Other.m_EMetaData )
	, m_UniqueID( a_Other.m_UniqueID )
	, m_Type( a_Other.m_Type )
	, m_MetaData( NULL )
{
	m_PacketID = E_METADATA;
	FormPacket();
}





cPacket_Metadata::~cPacket_Metadata()
{
	delete [] m_MetaData;
}





void cPacket_Metadata::FormPacket()
{
	delete [] m_MetaData;
	m_MetaData = new char[3];
	m_MetaDataSize = 3;
	// m_UniqueID = GetUniqueID();
	m_MetaData[0] = 0x00;

	m_MetaData[2] = 0x7f;
	switch(m_EMetaData)
	{
		case cPawn::NORMAL:
			m_MetaData[1] = 0x00;
			break;
		case cPawn::BURNING:
			m_MetaData[1] = 0x01;
			break;
		case cPawn::CROUCHED:
			m_MetaData[1] = 0x02;
			break;
		case cPawn::RIDING:
			m_MetaData[1] = 0x04;
			break;
		case cPawn::SPRINTING:
			m_MetaData[1] = 0x08;
			break;
		case cPawn::EATING:
		case cPawn::BLOCKING:
			m_MetaData[1] = 0x10;
			break;
		default:
			m_MetaData[1] = 0x00;
			break;
	}
}





void cPacket_Metadata::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_UniqueID);
	AppendData   (a_Data, m_MetaData, m_MetaDataSize);
}




