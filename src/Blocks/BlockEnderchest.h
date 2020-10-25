
#pragma once

#include "Mixins.h"




class cBlockEnderchestHandler final :
	public cYawRotator<cBlockEntityHandler, 0x07, 0x03, 0x04, 0x02, 0x05>
{
	using Super = cYawRotator<cBlockEntityHandler, 0x07, 0x03, 0x04, 0x02, 0x05>;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
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
				return cItem(E_BLOCK_ENDER_CHEST);
			}

			return cItem(E_BLOCK_OBSIDIAN, 8);
		}

		return {};
	}

};




