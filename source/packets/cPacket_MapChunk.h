
#pragma once

#include "cPacket.h"





class cPacket_MapChunk :
	public cPacket
{
public:
	cPacket_MapChunk()
#if (MINECRAFT_1_2_2 == 1 )
		: m_PosX( 0 )
		, m_PosZ( 0 )
		, m_bContiguous( false )
		, m_BitMap1( 0 )
		, m_BitMap2( 0 )
		, m_CompressedSize( 0 )
		, m_UnusedInt( 0 )
#else
		: m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_SizeX( 0 )
		, m_SizeY( 0 )
		, m_SizeZ( 0 )
		, m_CompressedSize( 0 )
#endif
		, m_CompressedData( 0 )
	{ m_PacketID = E_MAP_CHUNK; m_CompressedData = 0; }
	
	cPacket_MapChunk( const cPacket_MapChunk & a_Copy );
	cPacket_MapChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData);
	~cPacket_MapChunk();
	virtual cPacket* Clone() const { return new cPacket_MapChunk(*this); }

	virtual void Serialize(AString & a_Data) const override;

#if (MINECRAFT_1_2_2 == 1 )
	int m_PosX;
	int m_PosZ;
	bool m_bContiguous; // le fuck?
	short m_BitMap1;
	short m_BitMap2;
	int m_CompressedSize;
	int m_UnusedInt;
	static const unsigned int c_Size = 1 + 4 + 4 + 1 + 2 + 2 + 4 + 4;
#else
	int m_PosX; // In block coordinates
	short m_PosY;
	int m_PosZ;
	char m_SizeX; // Is actually size-1 (when size is 128, this is 127)
	char m_SizeY;
	char m_SizeZ;
	int m_CompressedSize;
	static const unsigned int c_Size = 1 + 4 + 2 + 4 + 1 + 1 + 1 + 4;
#endif


	char * m_CompressedData;
};




