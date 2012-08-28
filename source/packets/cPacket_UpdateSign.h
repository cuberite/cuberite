
#pragma once

#include "cPacket.h"





class cPacket_UpdateSign : public cPacket
{
public:
	cPacket_UpdateSign() 
		: m_BlockX( 0 )
		, m_BlockY( 0 )
		, m_BlockZ( 0 )
	{
		m_PacketID = E_UPDATE_SIGN;
	}
	
	virtual cPacket * Clone() const { return new cPacket_UpdateSign( *this); }

	virtual int  Parse(cByteBuffer & a_Buffer) override;

	int     m_BlockX;
	short   m_BlockY;
	int     m_BlockZ;
	AString m_Line1;
	AString m_Line2;
	AString m_Line3;
	AString m_Line4;
};




