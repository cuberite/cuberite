/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

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
		cMap * Map = a_World->GetMapManager().GetMapData(a_Item.m_ItemDamage);

		if (Map == NULL)
		{
			return;
		}

		Map->UpdateRadius(*a_Player, DEFAULT_RADIUS);

		Map->UpdateClient(a_Player);
	}
} ;
