
// ItemMap.h





#pragma once

#include "../Entities/Entity.h"
#include "../Item.h"





class cItemMapHandler :
	public cItemHandler
{
	typedef cItemHandler super;

	static const unsigned int DEFAULT_RADIUS = 128;
	
public:
	cItemMapHandler() :
		super(E_ITEM_MAP)
	{
	}

	virtual void OnUpdate(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item)
	{
		cMap * Map = a_World->GetMapData(a_Item.m_ItemDamage);

		if (Map == NULL)
		{
			return;
		}

		// Map->AddTrackedPlayer(a_Player);

		Map->UpdateRadius(*a_Player, DEFAULT_RADIUS);
	}
} ;
