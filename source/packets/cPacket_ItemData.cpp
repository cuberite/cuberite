#include "cPacket_ItemData.h"

bool cPacket_ItemData::Parse(cSocket & a_Socket)
{
	m_Socket = a_Socket;

	if( !ReadShort(m_ItemID) ) return false;

	if( m_ItemID > -1 )
	{
		if( !ReadByte(m_ItemCount) ) return false;
		if( !ReadShort(m_ItemUses) ) return false;

		if(cItem::IsEnchantable((ENUM_ITEM_ID) m_ItemID))
		{
			if( !ReadShort(m_EnchantNums) ) return false;
			if( m_EnchantNums > -1 )
			{
				//TODO Not implemented yet!
			}
		}

	}
	else
	{
	    m_ItemCount = 0;
	    m_ItemUses = 0;
	}

	return true;
}

int cPacket_ItemData::GetSize(short a_ItemID)
{
	if(a_ItemID <= -1)
		return 2;
	if(cItem::IsEnchantable((ENUM_ITEM_ID) a_ItemID))
		return 7;
	return 5;
}


void cPacket_ItemData::AppendItem(char* a_Message, unsigned int &a_Iterator, cItem *a_Item)
{
	return AppendItem(a_Message, a_Iterator, (short) a_Item->m_ItemID, a_Item->m_ItemCount, a_Item->m_ItemHealth);
}

void cPacket_ItemData::AppendItem(char* a_Message, unsigned int &a_Iterator, short a_ItemID, char a_Quantity, short a_Damage)
{
	
	AppendShort	 ( (short) a_ItemID,	a_Message, a_Iterator );
	if(a_ItemID > -1)
	{
		AppendByte	 ( a_Quantity,			a_Message, a_Iterator );
		AppendShort	 ( a_Damage,			a_Message, a_Iterator );
		if(cItem::IsEnchantable((ENUM_ITEM_ID) a_ItemID))
			AppendShort ( (short) -1,		a_Message, a_Iterator );
	}
}