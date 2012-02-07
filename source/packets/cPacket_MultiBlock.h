
#pragma once

#include "cPacket.h"





class cPacket_MultiBlock : public cPacket
{
public:
	cPacket_MultiBlock()
		: m_ChunkX( 0 )
		, m_ChunkZ( 0 )
		, m_NumBlocks( 0 )
		, m_BlockCoordinates( 0 )
		, m_BlockTypes( 0 )
		, m_BlockMetas( 0 )
	{ m_PacketID = E_MULTI_BLOCK; }
	cPacket_MultiBlock( const cPacket_MultiBlock & a_Copy );
	~cPacket_MultiBlock();
	virtual cPacket* Clone() const { return new cPacket_MultiBlock(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int m_ChunkX;
	int m_ChunkZ;
	short m_NumBlocks;

	static const unsigned int c_Size = 1 + 4 + 4 + 2; // Minimum size (when NumBlocks == 0)

	unsigned short* m_BlockCoordinates; // x<<12 | z<<8 | y
	char* m_BlockTypes;
	char* m_BlockMetas;
};




