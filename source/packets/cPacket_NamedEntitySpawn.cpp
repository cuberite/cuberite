
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_NamedEntitySpawn.h"





void cPacket_NamedEntitySpawn::Serialize(AString & a_Data) const
{
	short CurrentItem = m_CurrentItem;
	assert(CurrentItem > 0);
	if (CurrentItem <= 0)
	{
		CurrentItem = 0;
		// Fix, to make sure no invalid values are sent.
		// WARNING: HERE ITS 0, BUT IN EQUIP PACKET ITS -1 !!
	}

	AppendByte	  (a_Data, m_PacketID);
	AppendInteger (a_Data, m_UniqueID);
	AppendString16(a_Data, m_PlayerName);
	AppendInteger (a_Data, m_PosX);
	AppendInteger (a_Data, m_PosY);
	AppendInteger (a_Data, m_PosZ);
	AppendByte    (a_Data, m_Rotation);
	AppendByte    (a_Data, m_Pitch);
	AppendShort   (a_Data, CurrentItem);
}




