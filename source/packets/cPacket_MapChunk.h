#pragma once

#include "cPacket.h"
#include "PacketID.h"

class cChunk;
class cPacket_MapChunk : public cPacket
{
public:
	cPacket_MapChunk()
		: m_PosX( 0 )
		, m_PosY( 0 )
		, m_PosZ( 0 )
		, m_SizeX( 0 )
		, m_SizeY( 0 )
		, m_SizeZ( 0 )
		, m_CompressedSize( 0 )
		, m_CompressedData( 0 )
	{ m_PacketID = E_MAP_CHUNK; m_CompressedData = 0; }
	cPacket_MapChunk(cChunk* a_Chunk);
	cPacket_MapChunk( const cPacket_MapChunk & a_Copy );
	~cPacket_MapChunk();
	virtual cPacket* Clone() const { return new cPacket_MapChunk(*this); }

	bool Send(cSocket & a_Socket);

	int m_PosX; // In block coordinates
	short m_PosY;
	int m_PosZ;
	char m_SizeX; // Is actually size-1 (when size is 128, this is 127)
	char m_SizeY;
	char m_SizeZ;
	int m_CompressedSize;
	static const unsigned int c_Size = 1 + 4 + 2 + 4 + 1 + 1 + 1 + 4;

	char* m_CompressedData;
};