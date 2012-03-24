
#pragma once

#include "cPacket.h"





class cPacket_MultiBlock : public cPacket
{
public:
	struct sBlockChange
	{
		sBlockChange()
			: Data( 0 )
		{}
		unsigned int Data;
// 		short Data;   // 4bits metadata ... 12bits block ID
// 		short Coords; // 8bits Y ... 4bits Z ... 4bits X
	};

	cPacket_MultiBlock()
		: m_ChunkX( 0 )
		, m_ChunkZ( 0 )
		, m_NumBlocks( 0 )
		, m_DataSize( 0 )
		, m_Data( NULL )
	{ m_PacketID = E_MULTI_BLOCK; }
	
	cPacket_MultiBlock( const cPacket_MultiBlock & a_Copy );
	~cPacket_MultiBlock();
	virtual cPacket* Clone() const { return new cPacket_MultiBlock(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int m_ChunkX;
	int m_ChunkZ;
	short m_NumBlocks;

	int m_DataSize; // Should be 4 * m_NumBlocks ??
	sBlockChange * m_Data;
};




