
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_MapChunk.h"
#include "../ChunkDef.h"





cPacket_MapChunk::cPacket_MapChunk(int a_ChunkX, int a_ChunkZ, const AString & a_SerializedData) :
	m_PosX(a_ChunkX),
	m_PosZ(a_ChunkZ),
	m_SerializedData(a_SerializedData)
{
	m_PacketID = E_MAP_CHUNK;
}





void cPacket_MapChunk::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);

	AppendInteger(a_Data, m_PosX);
	AppendInteger(a_Data, m_PosZ);
	a_Data.append(m_SerializedData);
}




