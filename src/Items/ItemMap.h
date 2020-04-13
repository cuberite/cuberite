
#pragma once

#include "../Item.h"





class cItemMapHandler:
	public cItemHandler
{
	using Super = cItemHandler;

	static const unsigned int DEFAULT_RADIUS = 128;

public:

	cItemMapHandler():
		Super(E_ITEM_MAP)
	{
	}

	virtual void OnUpdate(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item)
	{
		cMap * Map = a_World->GetMapManager().GetMapData(static_cast<unsigned>(a_Item.m_ItemDamage));

		if (Map == nullptr)
		{
			return;
		}

		Map->UpdateRadius(*a_Player, DEFAULT_RADIUS);
		Map->UpdateClient(a_Player);
	}
} ;
