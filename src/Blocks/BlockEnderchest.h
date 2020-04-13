
#pragma once

#include "BlockEntity.h"
#include "Mixins.h"




class cBlockEnderchestHandler :
	public cYawRotator<cBlockEntityHandler>
{
	using Super = cYawRotator<cBlockEntityHandler>;

public:

	cBlockEnderchestHandler(BLOCKTYPE a_BlockType):
		Super(a_BlockType)
	{
	}





	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, cBlockEntity * a_BlockEntity, const cEntity * a_Digger, const cItem * a_Tool) override
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
				return cItem(E_BLOCK_ENDER_CHEST, 1, 0);
			}
			else
			{
				return cItem(E_BLOCK_OBSIDIAN, 8, 0);
			}
		}
		return {};
	}

};




