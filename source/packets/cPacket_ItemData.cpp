
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_ItemData.h"





int cPacket_ItemData::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEShort, m_ItemID, TotalBytes);

	if (m_ItemID <= -1)
	{
		m_ItemCount = 0;
		m_ItemUses = 0;
		return TotalBytes;
	}

	HANDLE_PACKET_READ(ReadChar,    m_ItemCount, TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort, m_ItemUses,  TotalBytes);

	if (cItem::IsEnchantable((ENUM_ITEM_ID) m_ItemID))
	{
		HANDLE_PACKET_READ(ReadBEShort, m_EnchantNums, TotalBytes);
		
		if ( m_EnchantNums > -1 )
		{
			// TODO: Enchantment not implemented yet!
		}
	}
	return TotalBytes;
}





int cPacket_ItemData::GetSize(short a_ItemID)
{
	if(a_ItemID <= -1)
		return 2;
	if(cItem::IsEnchantable((ENUM_ITEM_ID) a_ItemID))
		return 7;
	return 5;
}





void cPacket_ItemData::AppendItem(AString & a_Data, const cItem * a_Item)
{
	return AppendItem(a_Data, a_Item->m_ItemID, a_Item->m_ItemCount, a_Item->m_ItemHealth);
}





void cPacket_ItemData::AppendItem(AString & a_Data, short a_ItemID, char a_Quantity, short a_Damage)
{
	AppendShort(a_Data, (short) a_ItemID);
	if (a_ItemID > -1)
	{
		AppendByte (a_Data, a_Quantity);
		AppendShort(a_Data, a_Damage);
		
		if (cItem::IsEnchantable((ENUM_ITEM_ID) a_ItemID))
		{
			// TODO: Implement enchantments
			AppendShort(a_Data, (short) -1);
		}
	}
}




