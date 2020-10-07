
#pragma once

#include "BlockHandler.h"
#include "../Entities/Player.h"
#include "../ClientHandle.h"




class cBlockCauldronHandler final :
	public cBlockHandler
{
	using Super = cBlockHandler;

public:

	using Super::Super;

private:

	virtual cItems ConvertToPickups(NIBBLETYPE a_BlockMeta, const cEntity * a_Digger, const cItem * a_Tool) const override
	{
		return cItem(E_ITEM_CAULDRON, 1, 0);
	}





	virtual bool OnUse(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cPlayer & a_Player,
		const Vector3i a_BlockPos,
		eBlockFace a_BlockFace,
		const Vector3i a_CursorPos
	) const override
	{
		NIBBLETYPE Meta = a_ChunkInterface.GetBlockMeta(a_BlockPos);
		auto EquippedItem = a_Player.GetEquippedItem();

		switch (EquippedItem.m_ItemType)
		{
			case E_ITEM_BUCKET:
			{
				if (Meta == 3)
				{
					a_ChunkInterface.SetBlockMeta(a_BlockPos, 0);
					// Give new bucket, filled with fluid when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(E_ITEM_WATER_BUCKET));
					}
				}
				break;
			}
			case E_ITEM_WATER_BUCKET:
			{
				if (Meta < 3)
				{
					a_ChunkInterface.SetBlockMeta(a_BlockPos, 3);
					// Give empty bucket back when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(E_ITEM_BUCKET));
					}
					a_Player.GetStatManager().AddValue(Statistic::FillCauldron);
				}
				break;
			}
			case E_ITEM_GLASS_BOTTLE:
			{
				if (Meta > 0)
				{
					a_ChunkInterface.SetBlockMeta(a_BlockPos, --Meta);
					// Give new potion when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(E_ITEM_POTION));
					}
					a_Player.GetStatManager().AddValue(Statistic::UseCauldron);
				}
				break;
			}
			case E_ITEM_POTION:
			{
				// Refill cauldron with water bottles.
				if ((Meta < 3) && (EquippedItem.m_ItemDamage == 0))
				{
					a_ChunkInterface.SetBlockMeta(Vector3i(a_BlockPos), ++Meta);
					// Give back an empty bottle when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(E_ITEM_GLASS_BOTTLE));
					}
				}
				break;
			}
			case E_ITEM_LEATHER_BOOTS:
			case E_ITEM_LEATHER_CAP:
			case E_ITEM_LEATHER_PANTS:
			case E_ITEM_LEATHER_TUNIC:
			{
				// Resets any color to default:
				if ((Meta > 0) && ((EquippedItem.m_ItemColor.GetRed() != 255) || (EquippedItem.m_ItemColor.GetBlue() != 255) || (EquippedItem.m_ItemColor.GetGreen() != 255)))
				{
					a_ChunkInterface.SetBlockMeta(a_BlockPos, --Meta);
					auto NewItem = cItem(EquippedItem);
					NewItem.m_ItemColor.Clear();
					a_Player.ReplaceOneEquippedItemTossRest(NewItem);
				}
				break;
			}
			case E_BLOCK_BLACK_SHULKER_BOX:
			case E_BLOCK_BLUE_SHULKER_BOX:
			case E_BLOCK_BROWN_SHULKER_BOX:
			case E_BLOCK_CYAN_SHULKER_BOX:
			case E_BLOCK_GRAY_SHULKER_BOX:
			case E_BLOCK_GREEN_SHULKER_BOX:
			case E_BLOCK_LIGHT_BLUE_SHULKER_BOX:
			case E_BLOCK_LIGHT_GRAY_SHULKER_BOX:
			case E_BLOCK_LIME_SHULKER_BOX:
			case E_BLOCK_MAGENTA_SHULKER_BOX:
			case E_BLOCK_ORANGE_SHULKER_BOX:
			case E_BLOCK_PINK_SHULKER_BOX:
			case E_BLOCK_RED_SHULKER_BOX:
			case E_BLOCK_YELLOW_SHULKER_BOX:
			{
				// Resets shulker box color.

				// TODO: When there is an actual default shulker box add the appropriate changes here! - 19.09.2020 - 12xx12
				if (Meta == 0)
				{
					// The cauldron is empty:
					break;
				}

				// Proceed with normal cleaning:
				a_ChunkInterface.SetBlockMeta(a_BlockPos, --Meta);
				auto NewShulker = cItem(EquippedItem);
				NewShulker.m_ItemType = E_BLOCK_PURPLE_SHULKER_BOX;
				a_Player.ReplaceOneEquippedItemTossRest(NewShulker);

				break;
			}
		}

		if (!ItemHandler(EquippedItem.m_ItemType)->IsPlaceable())
		{
			// Item not placeable in the first place, our work is done:
			return true;
		}

		// This is a workaround for versions < 1.13, where rclking a cauldron with a block, places a block.
		// Using cauldrons with blocks was added in 1.13 as part of shulker cleaning.
		const auto ResendPosition = AddFaceDirection(a_BlockPos, a_BlockFace);
		a_Player.GetClientHandle()->SendBlockChange(
			ResendPosition.x, ResendPosition.y, ResendPosition.z,
			a_ChunkInterface.GetBlock(ResendPosition), a_ChunkInterface.GetBlockMeta(ResendPosition)
		);

		return true;
	}





	virtual bool IsUseable() const override
	{
		return true;
	}





	virtual void OnUpdate(
		cChunkInterface & a_ChunkInterface,
		cWorldInterface & a_WorldInterface,
		cBlockPluginInterface & a_PluginInterface,
		cChunk & a_Chunk,
		const Vector3i a_RelPos
	) const override
	{
		auto WorldPos = a_Chunk.RelativeToAbsolute(a_RelPos);
		if (!a_WorldInterface.IsWeatherWetAtXYZ(WorldPos.addedY(1)))
		{
			// It's not raining at our current location or we do not have a direct view of the sky
			return;
		}

		auto Meta = a_Chunk.GetMeta(a_RelPos);
		if (Meta < 3)
		{
			a_Chunk.SetMeta(a_RelPos, Meta + 1);
		}
	}





	virtual ColourID GetMapBaseColourID(NIBBLETYPE a_Meta) const override
	{
		UNUSED(a_Meta);
		return 21;
	}
} ;




