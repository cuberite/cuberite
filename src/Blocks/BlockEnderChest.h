
#pragma once

#include "Mixins/Mixins.h"




class cBlockEnderChestHandler final :
	public cBlockEntityHandler
{
	using Super = cBlockEntityHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		// Only drop something when mined with a pickaxe:
		if (
			(a_Tool != nullptr) &&
			ItemCategory::IsPickaxe(a_Tool->m_ItemType)
		)
		{
			// Only drop self when mined with a silk-touch pickaxe:
			if (a_Tool->m_Enchantments.GetLevel(cEnchantments::enchSilkTouch) > 0)
			{
				return cItem(Item::EnderChest);
			}

			return cItem(Item::Obsidian, 8);
		}

		return {};
	}

};
