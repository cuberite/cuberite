
#pragma once

#include "cPacket.h"





class cPacket_RelativeEntityMove : public cPacket
{
public:
	cPacket_RelativeEntityMove() 
		: m_UniqueID( 0 )
		, m_MoveX( 0 )
		, m_MoveY( 0 )
		, m_MoveZ( 0 )
	{ m_PacketID = E_REL_ENT_MOVE;  }
	virtual cPacket* Clone() const { return new cPacket_RelativeEntityMove(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int  m_UniqueID;
	char m_MoveX;	// Pixels, devide by 32 for block
	char m_MoveY;
	char m_MoveZ;

	static const unsigned int c_Size = 1 + 4 + 1 + 1 + 1;
};




