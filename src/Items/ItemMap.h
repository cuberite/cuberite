
#pragma once

#include "../Item.h"





class cItemMapHandler final:
	public cItemHandler
{
	using Super = cItemHandler;

	static const unsigned int DEFAULT_RADIUS = 128;

public:

	using Super::Super;

	virtual void OnUpdate(cWorld * a_World, cPlayer * a_Player, const cItem & a_Item) const override
	{
		// Get the map ID from the item's MapIdComponent
		if (!a_Item.HasComponent<DataComponents::MapIdComponent>())
		{
			return;
		}

		unsigned int MapID = a_Item.GetComponentOrDefault<DataComponents::MapIdComponent>().MapID;
		cMap * Map = a_World->GetMapManager().GetMapData(MapID);

		if (Map == nullptr)
		{
			return;
		}

		Map->UpdateRadius(*a_Player, DEFAULT_RADIUS);
		Map->UpdateClient(a_Player);
	}
} ;
