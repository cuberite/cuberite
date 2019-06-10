
#pragma once

#include "ItemHandler.h"
#include "ItemThrowable.h"





class cItemEyeOfEnderHandler :
	public cItemThrowableHandler
{
	typedef cItemThrowableHandler super;
public:
	cItemEyeOfEnderHandler(void) :
		super(E_ITEM_EYE_OF_ENDER, cProjectileEntity::pkSnowball, 30)
	{
	}

	virtual bool OnItemUse(
		cWorld * a_World, cPlayer * a_Player, cBlockPluginInterface & a_PluginInterface, const cItem & a_Item,
		int a_BlockX, int a_BlockY, int a_BlockZ, eBlockFace a_BlockFace
	) override
	{
		BLOCKTYPE FacingBlock;
		NIBBLETYPE FacingMeta;
		a_World->GetBlockTypeMeta(a_BlockX, a_BlockY, a_BlockZ, FacingBlock, FacingMeta);
		switch (FacingBlock)
		{
			case E_BLOCK_END_PORTAL_FRAME:
			{
				// Fill the portal frame. E_META_END_PORTAL_EYE is the bit for holding the eye of ender.
				if ((FacingMeta & E_META_END_PORTAL_FRAME_EYE) != E_META_END_PORTAL_FRAME_EYE)
				{
					a_World->SetBlock(a_BlockX, a_BlockY, a_BlockZ, E_BLOCK_END_PORTAL_FRAME, FacingMeta | E_META_END_PORTAL_FRAME_EYE);
					if (!a_Player->IsGameModeCreative())
					{
						a_Player->GetInventory().RemoveOneEquippedItem();
					}
				}
				break;
			}
			default:
			{
				// TODO: Create projectile for Eye Of Ender
				// return cItemThrowableHandler::OnItemUse(a_World, a_Player, a_PluginInterface, a_Item, a_BlockX, a_BlockY, a_BlockZ, a_BlockFace);
			}
		}

		return false;
	}

} ;





