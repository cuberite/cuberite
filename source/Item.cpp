
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Item.h"
#include <json/json.h>





cItem cItem::CopyOne(void) const
{
	cItem res(*this);
	res.m_ItemCount = 1;
	return res;
}





bool cItem::IsStackableWith(const cItem & a_OtherStack)
{
	if (a_OtherStack.m_ItemType != m_ItemType)
	{
		return false;
	}
	
	// TODO: match enchantments etc.
	return true;
}





void cItem::GetJson( Json::Value & a_OutValue ) const
{
	a_OutValue["ID"] = m_ItemType;
	if( m_ItemType > 0 )
	{
		a_OutValue["Count"] = m_ItemCount;
		a_OutValue["Health"] = m_ItemDamage;
	}
}

void cItem::FromJson( const Json::Value & a_Value )
{
	m_ItemType = (ENUM_ITEM_ID)a_Value.get("ID", -1 ).asInt();
	if( m_ItemType > 0 )
	{
		m_ItemCount = (char)a_Value.get("Count", -1 ).asInt();
		m_ItemDamage = (short)a_Value.get("Health", -1 ).asInt();
	}
}





bool cItem::IsEnchantable(short item)
{
	if ((item >= 256) && (item <= 259))
		return true;
	if ((item >= 267) && (item <= 279))
		return true;
	if ((item >= 283) && (item <= 286))	
		return true;
	if ((item >= 290) && (item <= 294))
		return true;
	if ((item >= 298) && (item <= 317))
		return true;
	if ((item >= 290) && (item <= 294))
		return true;

	if ((item == 346) || (item == 359) || (item == 261))
		return true;

	return false;
}




