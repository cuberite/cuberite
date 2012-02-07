
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_SoundEffect.h"





cPacket_SoundEffect::cPacket_SoundEffect( const cPacket_SoundEffect & a_Copy )
{
	m_PacketID  = E_SOUND_EFFECT;
	m_SoundID   = a_Copy.m_SoundID;
	m_PosX      = a_Copy.m_PosX;
	m_PosY      = a_Copy.m_PosY;
	m_PosZ      = a_Copy.m_PosZ;
	m_SoundData = a_Copy.m_SoundData;
}





void cPacket_SoundEffect::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_SoundID);
	AppendInteger(a_Data, m_PosX);
	AppendByte   (a_Data, m_PosY);
	AppendInteger(a_Data, m_PosZ);
	AppendInteger(a_Data, m_SoundData);
}




