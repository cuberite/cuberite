
// LootTables.h

#pragma once

#include "../LootTableProvider.h"

/*
This files contains all default loot tables
*/

class cTestLootTable: public cLootTable
{
	enum LootTable::eType m_Type = LootTable::eType::Chest;
	cLootTablePoolVector m_LootTablePools =
		{
			{
				-1, {1, 3},  // Rolls
				0 , {0, 0},  // Bonus rolls
				{
					{{}, E_ITEM_LEAD, 20},
					{{}, E_ITEM_GOLDEN_APPLE, 15},
					{{}, E_ITEM_13_DISC, 15},
					{{}, E_ITEM_CAT_DISC, 15},
					{{}, E_ITEM_NAME_TAG, 20},
					{{}, E_ITEM_CHAIN_CHESTPLATE, 10}
				}
			}
		};
};
