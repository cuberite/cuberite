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