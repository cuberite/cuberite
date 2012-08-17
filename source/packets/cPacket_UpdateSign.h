
#pragma once

#include "cPacket.h"





class cPacket_UpdateSign : public cPacket
{
public:
	cPacket_UpdateSign() 
		: m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
	{ m_PacketID = E_UPDATE_SIGN; }
	virtual cPacket* Clone() const { return new cPacket_UpdateSign( *this ); }

	virtual int Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	int     m_PosX;
	short   m_PosY;
	int     m_PosZ;
	AString m_Line1;
	AString m_Line2;
	AString m_Line3;
	AString m_Line4;

	static const unsigned int c_Size = 1 + 4 + 2 + 4 + 2 + 2 + 2 + 2; // minimum size
};




