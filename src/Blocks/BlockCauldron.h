
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
		auto EquippedItem = a_Player.GetEquippedItem();

		if (m_BlockType == BlockType::Cauldron)
		{
			switch (EquippedItem.m_ItemType)
			{
				case Item::WaterBucket:
				{
					a_ChunkInterface.FastSetBlock(a_BlockPos, Block::WaterCauldron::WaterCauldron(3));
					// Give empty bucket back when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
					}
					a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;

					break;
				}
				case Item::Potion:
				{
					// Refill cauldron with water bottles.
					if (EquippedItem.m_ItemDamage == 0)
					{
						a_ChunkInterface.FastSetBlock(Vector3i(a_BlockPos), Block::WaterCauldron::WaterCauldron(1));
						// Give back an empty bottle when the gamemode is not creative:
						if (!a_Player.IsGameModeCreative())
						{
							a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::GlassBottle));
						}
					}
					break;
				}
				case Item::LavaBucket:
				{
					a_ChunkInterface.FastSetBlock(a_BlockPos, Block::LavaCauldron::LavaCauldron());
					// Give empty bucket back when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
					}
					a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;

					break;
				}
				case Item::PowderSnowBucket:
				{
					a_ChunkInterface.FastSetBlock(a_BlockPos, Block::PowderSnowCauldron::PowderSnowCauldron(3));
					// Give empty bucket back when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
					}
					a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;

					break;
				}
			}
		}
		else if (m_BlockType == BlockType::WaterCauldron)
		{
			auto FillState = Block::WaterCauldron::Level(a_ChunkInterface.GetBlock(a_BlockPos));
			switch (EquippedItem.m_ItemType)
			{
				case Item::PowderSnowBucket:
				{
					a_ChunkInterface.FastSetBlock(a_BlockPos, Block::PowderSnowCauldron::PowderSnowCauldron(3));
					// Give empty bucket back when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
					}
					a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;
					break;
				}
				case Item::LavaBucket:
				{
					a_ChunkInterface.FastSetBlock(a_BlockPos, Block::LavaCauldron::LavaCauldron());
					// Give empty bucket back when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
					}
					a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;

					break;
				}
				case Item::Bucket:
				{
					if (FillState == 3)
					{
						a_ChunkInterface.FastSetBlock(a_BlockPos, Block::Cauldron::Cauldron());
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
					if (FillState == 3)
					{
						a_ChunkInterface.FastSetBlock(a_BlockPos, Block::WaterCauldron::WaterCauldron());
						// Give new bucket, filled with fluid when the gamemode is not creative:
						if (!a_Player.IsGameModeCreative())
						{
							a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
						}
					}
					break;
				}
				case Item::GlassBottle:
				{
					if (FillState > 1)
					{
						a_ChunkInterface.FastSetBlock(a_BlockPos, Block::WaterCauldron::WaterCauldron(--FillState));
					}
					else
					{
						a_ChunkInterface.FastSetBlock(a_BlockPos, Block::Cauldron::Cauldron());
					}
					// Give new potion when the gamemode is not creative:
					if (!a_Player.IsGameModeCreative())
					{
						a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Potion));
					}
					a_Player.GetStatistics().Custom[CustomStatistic::UseCauldron]++;
					break;
				}
				case Item::Potion:
				{
					// Refill cauldron with water bottles.
					if ((FillState < 3) && (EquippedItem.m_ItemDamage == 0))
					{
						a_ChunkInterface.FastSetBlock(Vector3i(a_BlockPos), Block::WaterCauldron::WaterCauldron(++FillState));
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
						FillState--;
						if (FillState > 0)
						{
							a_ChunkInterface.FastSetBlock(a_BlockPos, Block::WaterCauldron::WaterCauldron(FillState));
						}
						else
						{
							a_ChunkInterface.FastSetBlock(a_BlockPos, Block::Cauldron::Cauldron());
						}
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
					FillState--;
					if (FillState > 0)
					{
						a_ChunkInterface.FastSetBlock(a_BlockPos, Block::WaterCauldron::WaterCauldron(FillState));
					}
					else
					{
						a_ChunkInterface.FastSetBlock(a_BlockPos, Block::Cauldron::Cauldron());
					}
					auto NewShulker = cItem(EquippedItem);
					NewShulker.m_ItemType = Item::ShulkerBox;
					a_Player.ReplaceOneEquippedItemTossRest(NewShulker);
					break;
				}
				break;
			}
		}
		else if (m_BlockType == BlockType::LavaCauldron)
		{
			if (EquippedItem.m_ItemType == Item::Bucket)
			{
				a_ChunkInterface.FastSetBlock(a_BlockPos, Block::Cauldron::Cauldron());
				// Give back an empty bottle when the gamemode is not creative:
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::LavaBucket));
				}
			}
			if (EquippedItem.m_ItemType == Item::WaterBucket)
			{
				a_ChunkInterface.FastSetBlock(a_BlockPos, Block::WaterCauldron::WaterCauldron(3));
				// Give empty bucket back when the gamemode is not creative:
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
				}
				a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;
			}
			if (EquippedItem.m_ItemType == Item::PowderSnowBucket)
			{
				a_ChunkInterface.FastSetBlock(a_BlockPos, Block::PowderSnowCauldron::PowderSnowCauldron(3));
				// Give empty bucket back when the gamemode is not creative:
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
				}
				a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;
			}
		}
		else if (m_BlockType == BlockType::PowderSnowCauldron)
		{
			if (EquippedItem.m_ItemType == Item::Bucket)
			{
				a_ChunkInterface.FastSetBlock(a_BlockPos, Block::Cauldron::Cauldron());
				// Give back an empty bottle when the gamemode is not creative:
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::PowderSnowBucket));
				}
			}
			if (EquippedItem.m_ItemType == Item::WaterBucket)
			{
				a_ChunkInterface.FastSetBlock(a_BlockPos, Block::WaterCauldron::WaterCauldron(3));
				// Give empty bucket back when the gamemode is not creative:
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
				}
				a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;
			}
			if (EquippedItem.m_ItemType == Item::LavaBucket)
			{
				a_ChunkInterface.FastSetBlock(a_BlockPos, Block::LavaCauldron::LavaCauldron());
				// Give empty bucket back when the gamemode is not creative:
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
				}
				a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;
			}
		}
		/* TODO: either create a seperate header file fore water and lava or handle them here
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
				a_ChunkInterface.FastSetBlock(a_BlockPos, Block::WaterCauldron::WaterCauldron(3));
				// Give empty bucket back when the gamemode is not creative:
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
				}
				a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;
				break;
			}
			case Item::LavaBucket:
			{
				a_ChunkInterface.FastSetBlock(a_BlockPos, Block::LavaCauldron::LavaCauldron());
				// Give empty bucket back when the gamemode is not creative:
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
				}
				a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;
				break;
			}
			case Item::PowderSnowBucket:
			{
				a_ChunkInterface.FastSetBlock(a_BlockPos, Block::PowderSnowCauldron::PowderSnowCauldron(3));
				// Give empty bucket back when the gamemode is not creative:
				if (!a_Player.IsGameModeCreative())
				{
					a_Player.ReplaceOneEquippedItemTossRest(cItem(Item::Bucket));
				}
				a_Player.GetStatistics().Custom[CustomStatistic::FillCauldron]++;
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
		*/

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
		/*
		if (!a_WorldInterface.IsWeatherWetAtXYZ(WorldPos.addedY(1)))
		{
			// It's not raining at our current location or we do not have a direct view of the sky
			return;
		}
		*/

		BlockState Block = a_Chunk.GetBlock(a_RelPos);
		if (((Block.Type() == BlockType::WaterCauldron) || (Block.Type() == BlockType::Cauldron)) && a_WorldInterface.IsWeatherWetAtXYZ(WorldPos.addedY(1)))
		{
			auto FillState = Block::WaterCauldron::Level(a_Chunk.GetBlock(a_RelPos));
			if (FillState < 3)
			{
				a_Chunk.FastSetBlock(a_RelPos, Block::WaterCauldron::WaterCauldron(FillState + 1));
			}
		}
		/*
		TODO: check if its snowing
		else if (((Block.Type() == BlockType::PowderSnowCauldron) || (Block.Type() == BlockType::Cauldron)) && a_WorldInterface.i)
		{
			auto FillState = Block::PowderSnowCauldron::Level(a_Chunk.GetBlock(a_RelPos));
			if (FillState < 3)
			{
				a_Chunk.FastSetBlock(a_RelPos, Block::PowderSnowCauldron::PowderSnowCauldron(FillState + 1));
			}
		}
		*/
	}





	virtual ColourID GetMapBaseColourID() const override
	{
		return 21;
	}
} ;




