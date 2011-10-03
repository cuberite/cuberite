#pragma once

#include "Defines.h"
#include "BlockID.h"

class cItem																								//tolua_export
{																										//tolua_export
public:
	cItem( ENUM_ITEM_ID a_ItemID = E_ITEM_EMPTY, char a_ItemCount = 0, short a_ItemHealth = 0 )			//tolua_export
		: m_ItemID		( a_ItemID )
		, m_ItemCount	( a_ItemCount )
		, m_ItemHealth	( a_ItemHealth )
	{																									//tolua_export
		if(!isValidItem( m_ItemID ) ) m_ItemID = E_ITEM_EMPTY;
	}																									//tolua_export
	void Empty()																						//tolua_export
	{																									//tolua_export
		m_ItemID = E_ITEM_EMPTY;
		m_ItemCount = 0;
		m_ItemHealth = 0;
	}																									//tolua_export
	bool IsEmpty()																						//tolua_export
	{																									//tolua_export
		return (m_ItemID <= 0 || m_ItemCount <= 0);
	}																									//tolua_export
	bool Equals( cItem & a_Item )																		//tolua_export
	{																									//tolua_export
		return ( (m_ItemID == a_Item.m_ItemID) && (m_ItemHealth == a_Item.m_ItemHealth) );
	}																									//tolua_export
	ENUM_ITEM_ID	m_ItemID;																			//tolua_export
	char			m_ItemCount;																		//tolua_export
	short			m_ItemHealth;																		//tolua_export
}; //tolua_export