
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_CollectItem.h"





void cPacket_CollectItem::Serialize(AString & a_Data) const
{
	AppendByte   (a_Data, m_PacketID);
	AppendInteger(a_Data, m_CollectedID);
	AppendInteger(a_Data, m_CollectorID);
}




