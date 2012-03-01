
#pragma once

#include "cPacket.h"





class cPacket_Login : public cPacket											//tolua_export
{																				//tolua_export
public:
	cPacket_Login()
		: m_ProtocolVersion( 0 )
#if (MINECRAFT_1_2_2 != 1)
		, m_MapSeed( 0 )
#endif
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

	// 1.2.2 http://wiki.vg/Pre-release_protocol#Login_Request_.280x01.29
	int m_ProtocolVersion;														//tolua_export
	AString m_Username;														//tolua_export
#if (MINECRAFT_1_2_2 != 1)
	long long m_MapSeed;
#endif
	AString m_LevelType;													//tolua_export
	int m_ServerMode;															//tolua_export
#if (MINECRAFT_1_2_2 == 1 )
	int m_Dimension;
#else
	char m_Dimension;
#endif
	char m_Difficulty;															//tolua_export
	unsigned char m_WorldHeight;												//tolua_export
	unsigned char m_MaxPlayers;													//tolua_export

#if (MINECRAFT_1_2_2 == 1 )
	static const unsigned int c_Size = 1 + 4 + 2 + 2 + 4 + 4 + 1 + 1 + 1;
#else
	static const unsigned int c_Size = 1 + 4 + 2 + 8 + 2 + 4 + 1 + 1 + 1 + 1; // Minimal size
#endif

	static const AString LEVEL_TYPE_DEFAULT;
	static const AString LEVEL_TYPE_SUPERFLAT;
};	//tolua_export




