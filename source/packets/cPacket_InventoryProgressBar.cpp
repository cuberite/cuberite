
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_InventoryProgressBar.h"





void cPacket_InventoryProgressBar::Serialize(AString & a_Data) const
{
	AppendByte (a_Data, m_PacketID);
	AppendByte (a_Data, m_WindowID);
	AppendShort(a_Data, m_ProgressBar);
	AppendShort(a_Data, m_Value);
}




