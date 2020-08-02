

#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Blocks/BlockBanner.h"





class cItemBannerHandler: public cItemHandler
{
	using Super = cItemHandler;

  public:

	cItemBannerHandler(int a_ItemType): Super(a_ItemType)
	{
	}





	virtual bool IsPlaceable(void) override
	{
		return true;
	}





	virtual bool GetBlocksToPlace(
		cWorld & a_World,
		cPlayer & a_Player,
		const cItem & a_EquippedItem,
		const Vector3i a_PlacedBlockPos,
		eBlockFace a_ClickedBlockFace,
		const Vector3i a_CursorPos,
		sSetBlockVector & a_BlocksToPlace
	) override
	{
		/*
			may either be placed on top of a block or on the side (may overlap)
			may have 16 colors
			has no collisions
			hitbox is one block tall
			can be placed in item frame - just shows the item model
			water and lava flow around banners (water goes through the bottom part if placed onto a wall)
			lava can spawn fire blocks next to the banner but doesn't burn the banner
			cannot be moved by pistons
			If renamed retains its name on adding/removing pattern
			keeps its name on building/breaking

			Banners can be copied using blank banner
			may be crated together with shield to display the banner

			can be used for 1.5 items as fuel

		 	sounds:
		 		Description                            |sound             | Volume | Pitch |dist
		 		Breaking the block                      block.wood.break    1.0     0.8     16
				Falling on the block with fall damage   block.wood.fall	    0.5     0.75    16
				Mining the block                        block.wood.hit      0.25    0.5     16
				Placing the block                       block.wood.place    1.0     0.8     16
				Walking on the block                    block.wood.step	    0.15    1.0     16

			Block data:
		 		Standing:
		 			direction
				Wall:
					direction
		*/

		if (a_ClickedBlockFace == BLOCK_FACE_BOTTOM)
		{
			return false;
		}
		// Placing on the floor
		else if (a_ClickedBlockFace == BLOCK_FACE_TOP)
		{
			/*
			NIBBLETYPE BlockMeta = cBlockBannerHandler::YawToMetaData(a_Player.GetYaw());
			a_BlocksToPlace.emplace_back(a_PlacedBlockPos, E_BLOCK_STANDING_BANNER, BlockMeta);
			*/
		}
		// placing on the sides
		else
		{
			/*
			NIBBLETYPE BlockMeta = cBlockBannerHandler::YawToMetaData(a_Player.GetYaw());
			a_BlocksToPlace.emplace_back(a_PlacedBlockPos, E_BLOCK_WALL_BANNER, BlockMeta);
			*/
		}
		return true;
	}
};