
#pragma once

#include "cPacket.h"





class cPacket_BlockPlace : public cPacket	//tolua_export
{											//tolua_export
public:
	cPacket_BlockPlace()
		: m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_Direction( 0 )
		, m_ItemType( 0 )
		, m_Count( 0 )
		, m_Uses( 0 )
	{ m_PacketID = E_BLOCK_PLACE;  }
	virtual cPacket* Clone() const { return new cPacket_BlockPlace(*this); }

	virtual int Parse(const char * a_Data, int a_Size) override;

	int  m_PosX;			//tolua_export
	char m_PosY;		//tolua_export
	int  m_PosZ;			//tolua_export
	char m_Direction;	//tolua_export

	short m_ItemType;	//tolua_export
	char  m_Count;		//tolua_export
	short m_Uses;		//tolua_export

	static const unsigned int c_Size = 1 + 4 + 1 + 4 + 1 + 2;// ( + 2 )
}; //tolua_export




