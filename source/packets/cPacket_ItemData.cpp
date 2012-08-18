
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cPacket_ItemData.h"





int cPacket_ItemData::Parse(cByteBuffer & a_Buffer)
{
	int TotalBytes = 0;
	HANDLE_PACKET_READ(ReadBEShort, m_Dst.m_ItemType, TotalBytes);

	if (m_Dst.m_ItemType <= -1)
	{
		m_Dst.Empty();
		return TotalBytes;
	}

	HANDLE_PACKET_READ(ReadChar,    m_Dst.m_ItemCount,  TotalBytes);
	HANDLE_PACKET_READ(ReadBEShort, m_Dst.m_ItemDamage, TotalBytes);

	if (cItem::IsEnchantable(m_Dst.m_ItemType))
	{
		short EnchantNumBytes;
		HANDLE_PACKET_READ(ReadBEShort, EnchantNumBytes, TotalBytes);
		
		if (EnchantNumBytes > 0)
		{
			// TODO: Enchantment not implemented yet!
			a_Buffer.SkipRead(EnchantNumBytes);
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





void cPacket_ItemData::AppendItem(AString & a_Data, const cItem & a_Item)
{
	return AppendItem(a_Data, a_Item.m_ItemType, a_Item.m_ItemCount, a_Item.m_ItemDamage);
}





void cPacket_ItemData::AppendItem(AString & a_Data, short a_ItemType, char a_Quantity, short a_Damage)
{
	short ItemType = a_ItemType;
	ASSERT(ItemType >= -1);  // Check validity of packets in debug runtime
	if (ItemType <= 0)
	{
		// Fix, to make sure no invalid values are sent.
		ItemType = -1;
	}
	
	AppendShort(a_Data, ItemType);
	if (a_ItemType > -1)
	{
		AppendByte (a_Data, a_Quantity);
		AppendShort(a_Data, a_Damage);
		
		if (cItem::IsEnchantable(a_ItemType))
		{
			// TODO: Implement enchantments
			AppendShort(a_Data, (short) -1);
		}
	}
}




