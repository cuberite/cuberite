
#pragma once

#include "cPacket.h"





class cPacket_Login : public cPacket											//tolua_export
{																				//tolua_export
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

	virtual int Parse(const char * a_Data, int a_Size) override;
	virtual void Serialize(AString & a_Data) const override;

	int m_ProtocolVersion;														//tolua_export
	AString m_Username;														//tolua_export
	AString m_LevelType;													//tolua_export
	int m_ServerMode;															//tolua_export
	int m_Dimension;
	char m_Difficulty;															//tolua_export
	unsigned char m_WorldHeight;												//tolua_export
	unsigned char m_MaxPlayers;													//tolua_export

	static const AString LEVEL_TYPE_DEFAULT;
	static const AString LEVEL_TYPE_SUPERFLAT;
};	//tolua_export




