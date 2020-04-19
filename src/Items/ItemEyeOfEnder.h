
#pragma once

#include "ItemHandler.h"
#include "ItemThrowable.h"





class cItemEyeOfEnderHandler:
	public cItemThrowableHandler
{
	using Super = cItemThrowableHandler;

public:

	cItemEyeOfEnderHandler():
		Super(E_ITEM_EYE_OF_ENDER, cProjectileEntity::pkSnowball, 30)
	{
	}





	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		// Try to fill an End Portal Frame block:
		if (a_ClickedBlockFace != BLOCK_FACE_NONE)
		{
			BLOCKTYPE FacingBlock;
			NIBBLETYPE FacingMeta;
			a_World->GetBlockTypeMeta(a_ClickedBlockPos, FacingBlock, FacingMeta);
			if (FacingBlock == E_BLOCK_END_PORTAL_FRAME)
			{
				// Fill the portal frame. E_META_END_PORTAL_EYE is the bit for holding the eye of ender.
				if ((FacingMeta & E_META_END_PORTAL_FRAME_EYE) != E_META_END_PORTAL_FRAME_EYE)
				{
					a_World->SetBlock(a_ClickedBlockPos, E_BLOCK_END_PORTAL_FRAME, FacingMeta | E_META_END_PORTAL_FRAME_EYE);
					if (!a_Player->IsGameModeCreative())
					{
						a_Player->GetInventory().RemoveOneEquippedItem();
					}
					return true;
				}
			}
			return false;
		}

		// TODO: Create projectile for Eye Of Ender
		// return Super::OnItemUse(a_World, a_Player, a_PluginInterface, a_Item, a_ClickedBlockPos, a_ClickedBlockFace);

		return false;
	}

} ;





