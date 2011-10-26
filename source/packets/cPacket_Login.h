#pragma once

#include "cPacket.h"
#include <string>
#include "PacketID.h"

class cPacket_Login : public cPacket											//tolua_export
{																				//tolua_export
public:
	cPacket_Login()
		: m_ProtocolVersion( 0 )
		, m_MapSeed( 0 )
		, m_ServerMode( 1 ) //0 for survival, 1 for creative
		, m_Dimension( 0 )
		, m_Difficulty( 0 )
		, m_WorldHeight( 0 )
		, m_MaxPlayers( 0 )
	{ m_PacketID = E_LOGIN;  }
	virtual cPacket* Clone() const { return new cPacket_Login(*this); }

	virtual bool Parse(cSocket & a_Socket);
	virtual bool Send(cSocket & a_Socket);

	int m_ProtocolVersion;														//tolua_export
	std::string m_Username;														//tolua_export
	long long m_MapSeed;														//tolua_export
	int m_ServerMode;															//tolua_export
	char m_Dimension;															//tolua_export
	char m_Difficulty;															//tolua_export
	unsigned char m_WorldHeight;												//tolua_export
	unsigned char m_MaxPlayers;													//tolua_export
	static const unsigned int c_Size = 1 + 4 + 2 + 8 + 4 + 1 + 1 + 1 + 1; // Minimal size
};	//tolua_export
