
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

	virtual cItems ConvertToPickups(BlockState a_Block, const cItem * a_Tool) const override
	{
		return cItem(Item::Cauldron);
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
		auto FillState = Block::Cauldron::Level(a_ChunkInterface.GetBlock(a_BlockPos));
		auto EquippedItem = a_Player.GetEquippedItem();

		switch (EquippedItem.m_ItemType)
		{
			case Item::Bucket:
			{
				if (FillState == 3)
				{
					a_ChunkInterface.FastSetBlock(a_BlockPos, Block::Cauldron::Cauldron(0));
					// Give new bucket, filled with fluid when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::WaterBucket));
					}
				}
				break;
			}
			case Item::WaterBucket:
			{
				if (FillState < 3)
				{
					a_ChunkInterface.FastSetBlock(a_BlockPos, Block::Cauldron::Cauldron(3));
					// Give empty bucket back when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
					}
					a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;
				}
				break;
			}
			case Item::GlassBottle:
			{
				if (FillState > 0)
				{
					a_ChunkInterface.FastSetBlock(a_BlockPos, Block::Cauldron::Cauldron(--FillState));
					// Give new potion when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Potion));
					}
					a_Player.GetStatistics().Custom[CustomStatistic::UseCauldron]++;
				}
				break;
			}
			case Item::Potion:
			{
				// Refill cauldron with water bottles.
				if ((FillState < 3) && (EquippedItem.m_ItemDamage == 0))
				{
					a_ChunkInterface.FastSetBlock(Vector3i(a_BlockPos), Block::Cauldron::Cauldron(++FillState));
					// Give back an empty bottle when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::GlassBottle));
					}
				}
				break;
			}
			case Item::LeatherBoots:
			case Item::LeatherHelmet:
			case Item::LeatherLeggings:
			case Item::LeatherChestplate:
			{
				// Resets any color to default:
				if ((FillState > 0) && ((EquippedItem.m_ItemColor.GetRed() != 255) || (EquippedItem.m_ItemColor.GetBlue() != 255) || (EquippedItem.m_ItemColor.GetGreen() != 255)))
				{
					a_ChunkInterface.FastSetBlock(a_BlockPos, Block::Cauldron::Cauldron(--FillState));
					auto NewItem = cItem(EquippedItem);
					NewItem.m_ItemColor.Clear();
					a_Player.ReplaceOneEquippedItemTossRest(NewItem);
				}
				break;
			}
			case Item::BlackShulkerBox:
			case Item::BlueShulkerBox:
			case Item::BrownShulkerBox:
			case Item::CyanShulkerBox:
			case Item::GrayShulkerBox:
			case Item::GreenShulkerBox:
			case Item::LightBlueShulkerBox:
			case Item::LightGrayShulkerBox:
			case Item::LimeShulkerBox:
			case Item::MagentaShulkerBox:
			case Item::OrangeShulkerBox:
			case Item::PinkShulkerBox:
			case Item::RedShulkerBox:
			case Item::YellowShulkerBox:
			{
				// Resets shulker box color.
				if (FillState == 0)
				{
					// The cauldron is empty:
					break;
				}

				// Proceed with normal cleaning:
				a_ChunkInterface.FastSetBlock(a_BlockPos, Block::Cauldron::Cauldron(--FillState));
				auto NewShulker = cItem(EquippedItem);
				NewShulker.m_ItemType = Item::ShulkerBox;
				a_Player.ReplaceOneEquippedItemTossRest(NewShulker);
				break;
			}
			default: break;
		}

		if (!EquippedItem.GetHandler().IsPlaceable())
		{
			// Item not placeable in the first place, our work is done:
			return true;
		}

		// This is a workaround for versions < 1.13, where rclking a cauldron with a block, places a block.
		// Using cauldrons with blocks was added in 1.13 as part of shulker cleaning.
		const auto ResendPosition = AddFaceDirection(a_BlockPos, a_BlockFace);
		a_Player.GetClientHandle()->SendBlockChange(
			ResendPosition,
			a_ChunkInterface.GetBlock(ResendPosition)
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

		auto FillState = Block::Cauldron::Level(a_Chunk.GetBlock(a_RelPos));
		if (FillState < 3)
		{
			a_Chunk.FastSetBlock(a_RelPos, Block::Cauldron::Cauldron(FillState + 1));
		}
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 21;
	}
} ;




