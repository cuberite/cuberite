
#pragma once

#include "cPacket.h"
#include "../ChunkDef.h"





class cPacket_MapChunk :
	public cPacket
{
public:
	cPacket_MapChunk()
		: m_PosX( 0 )
		, m_PosZ( 0 )
	{
		m_PacketID = E_MAP_CHUNK;
	}
	
	cPacket_MapChunk(int a_ChunkX, int a_ChunkZ, const AString & a_SerializedData);
	virtual cPacket* Clone() const { return new cPacket_MapChunk(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int     m_PosX;
	int     m_PosZ;
	AString m_SerializedData;
};




