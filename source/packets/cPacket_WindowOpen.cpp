
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_WindowOpen.h"





void cPacket_WindowOpen::Serialize(AString & a_Data) const
{
	AppendByte    (a_Data, m_PacketID);
	AppendByte    (a_Data, m_WindowID);
	AppendByte    (a_Data, m_InventoryType);
	AppendString16(a_Data, m_WindowTitle);
	AppendByte    (a_Data, m_NumSlots);
}




