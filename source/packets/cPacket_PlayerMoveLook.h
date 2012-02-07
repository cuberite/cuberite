
#pragma once

#include "cPacket.h"





class cPlayer;





class cPacket_PlayerMoveLook : public cPacket
{
public:
	cPacket_PlayerMoveLook()
		: m_PosX( 0.0 )
		, m_PosY( 0.0 )
		, m_Stance( 0.0 )
		, m_PosZ( 0.0 )
		, m_Rotation( 0.f )
		, m_Pitch( 0.f )
		, m_bFlying( false )
	{ m_PacketID = E_PLAYERMOVELOOK;  }
	cPacket_PlayerMoveLook( cPlayer* a_Player );
	virtual cPacket* Clone() const { return new cPacket_PlayerMoveLook(*this); }

	virtual int Parse(const char * a_Data, int a_Size) override;
	virtual void Serialize(AString & a_Data) const override;

	double m_PosX;
	double m_PosY;
	double m_Stance;
	double m_PosZ;
	float  m_Rotation;
	float  m_Pitch;
	bool   m_bFlying; // Yeah.. wtf
	
	static const unsigned int c_Size = 42;
};
