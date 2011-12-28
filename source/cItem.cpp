#include "cItem.h"
#include <json/json.h>

void cItem::GetJson( Json::Value & a_OutValue )
{
	a_OutValue["ID"] = m_ItemID;
	if( m_ItemID > 0 )
	{
		a_OutValue["Count"] = m_ItemCount;
		a_OutValue["Health"] = m_ItemHealth;
	}
}

void cItem::FromJson( const Json::Value & a_Value )
{
	m_ItemID = (ENUM_ITEM_ID)a_Value.get("ID", -1 ).asInt();
	if( m_ItemID > 0 )
	{
		m_ItemCount = (char)a_Value.get("Count", -1 ).asInt();
		m_ItemHealth = (short)a_Value.get("Health", -1 ).asInt();
	}
}

bool cItem::IsEnchantable(ENUM_ITEM_ID item)
{
	if(item >= 256 && item <= 259)
		return true;
	if(item >= 267 && item <= 279)
		return true;
	if(item >= 283 && item <= 286)	
		return true;
	if(item >= 290 && item <= 294)
		return true;
	if(item >= 298 && item <= 317)
		return true;
	if(item >= 290 && item <= 294)
		return true;

	if(item == 346 || item == 359 || item == 261)
		return true;


	return false;
}
