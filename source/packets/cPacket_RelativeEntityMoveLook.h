#pragma once

#include "cPacket.h"


class cPacket_RelativeEntityMoveLook : public cPacket
{
public:
	cPacket_RelativeEntityMoveLook() 
		: m_UniqueID( 0 )
		, m_MoveX( 0 )
		, m_MoveY( 0 )
		, m_MoveZ( 0 )
		, m_Yaw( 0 )
		, m_Pitch( 0 )
	{ m_PacketID = E_REL_ENT_MOVE_LOOK;  }
	virtual cPacket* Clone() const { return new cPacket_RelativeEntityMoveLook(*this); }

	bool Send( cSocket & a_Socket );

	int m_UniqueID;
	char m_MoveX;	// Pixels, divide by 32 for block
	char m_MoveY;
	char m_MoveZ;
	char m_Yaw;
	char m_Pitch;

	static const unsigned int c_Size = 1 + 4 + 1 + 1 + 1 + 1 + 1;
};
