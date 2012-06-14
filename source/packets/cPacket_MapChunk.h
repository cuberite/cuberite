
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
		, m_bContiguous( false )
		, m_BitMap1( 0 )
		, m_BitMap2( 0 )
		, m_CompressedSize( 0 )
		, m_UnusedInt( 0 )
		, m_CompressedData( NULL )
	{ m_PacketID = E_MAP_CHUNK; }
	
	cPacket_MapChunk( const cPacket_MapChunk & a_Copy );
	cPacket_MapChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, const BLOCKTYPE * a_BlockData, const unsigned char * a_BiomeData);
	~cPacket_MapChunk();
	virtual cPacket* Clone() const { return new cPacket_MapChunk(*this); }

	virtual void Serialize(AString & a_Data) const override;

	int m_PosX;
	int m_PosZ;
	bool m_bContiguous;
	short m_BitMap1;
	short m_BitMap2;
	int m_CompressedSize;
	int m_UnusedInt;
	char * m_CompressedData;
};




