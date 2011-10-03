#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cPacket_UpdateSign : public cPacket
{
public:
	cPacket_UpdateSign() 
		: m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
	{ m_PacketID = E_UPDATE_SIGN; }
	virtual cPacket* Clone() const { return new cPacket_UpdateSign( *this ); }

	bool Parse( cSocket & a_Socket );
	bool Send( cSocket & a_Socket );

	int m_PosX;
	short m_PosY;
	int m_PosZ;
	std::string m_Line1;
	std::string m_Line2;
	std::string m_Line3;
	std::string m_Line4;

	static const unsigned int c_Size = 1 + 4 + 2 + 4 + 2 + 2 + 2 + 2; // minimum size
};
