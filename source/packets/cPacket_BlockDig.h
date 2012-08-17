#pragma once

#include "cPacket.h"


class cPacket_BlockDig : public cPacket	//tolua_export
{										//tolua_export
public:
	cPacket_BlockDig()					//tolua_export
		: m_Status( 0 )
		, m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_Direction( 0 )
	{ m_PacketID = E_BLOCK_DIG;  }		//tolua_export
	virtual cPacket* Clone() const { return new cPacket_BlockDig(*this); }	//tolua_export

	virtual int Parse(cByteBuffer & a_Buffer) override;
	virtual void Serialize(AString & a_Data) const override;

	char m_Status;     // tolua_export
	int  m_PosX;       // tolua_export
	char m_PosY;       // tolua_export
	int  m_PosZ;       // tolua_export
	char m_Direction;  // tolua_export
	
	static const unsigned int c_Size = 12;
};	//tolua_export




