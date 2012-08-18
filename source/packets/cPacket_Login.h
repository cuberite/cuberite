
#pragma once

#include "cPacket.h"





class cPacket_Login : public cPacket
{
public:
	cPacket_Login()
		: m_ProtocolVersion( 0 )
		, m_ServerMode( 0 )
		, m_Dimension( 0 )
		, m_Difficulty( 0 )
		, m_WorldHeight( 0 )
		, m_MaxPlayers( 0 )
		, m_LevelType( LEVEL_TYPE_DEFAULT )
	{ m_PacketID = E_LOGIN;  }
	virtual cPacket* Clone() const { return new cPacket_Login(*this); }

	virtual int Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	int           m_ProtocolVersion;
	AString       m_Username;
	AString       m_LevelType;
	int           m_ServerMode;
	int           m_Dimension;
	char          m_Difficulty;
	unsigned char m_WorldHeight;
	unsigned char m_MaxPlayers;

	static const char * LEVEL_TYPE_DEFAULT;
	static const char * LEVEL_TYPE_SUPERFLAT;
};




