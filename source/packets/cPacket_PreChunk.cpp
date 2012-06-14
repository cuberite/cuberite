
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_PreChunk.h"





void cPacket_PreChunk::Serialize(AString & a_Data) const
{
	AppendByte	 (a_Data, m_PacketID);
	AppendInteger(a_Data, m_PosX);
	AppendInteger(a_Data, m_PosZ);
	AppendBool   (a_Data, m_bLoad);
}




