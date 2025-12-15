
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Blocks/BlockCocoaPod.h"





class cItemDyeHandler final :
	public cItemHandler
{
	using Super = cItemHandler;

public:
	using Super::Super;


	static inline bool IsDye(const cItem & a_Item)
	{
		switch (a_Item.m_ItemType)
		{
			case Item::BlackDye:
			case Item::BlueDye:
			case Item::BrownDye:
			case Item::BoneMeal:
			case Item::CyanDye:
			case Item::GrayDye:
			case Item::GreenDye:
			case Item::LightBlueDye:
			case Item::LightGrayDye:
			case Item::LimeDye:
			case Item::MagentaDye:
			case Item::OrangeDye:
			case Item::PinkDye:
			case Item::PurpleDye:
			case Item::RedDye:
			case Item::WhiteDye:
			case Item::YellowDye:
				return true;
			default: return false;
		}
	}


	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) const override
	{
		if ((a_HeldItem.m_ItemType == Item::BoneMeal) && (a_ClickedBlockFace != BLOCK_FACE_NONE))
		{
			// Bonemeal (white dye) is used to fertilize plants:
			if (FertilizePlant(*a_World, a_ClickedBlockPos))
			{
				if (a_Player->IsGameModeSurvival())
				{
					a_Player->GetInventory().RemoveOneEquippedItem();
					return true;
				}
			}
		}
		else if ((a_HeldItem.m_ItemType == Item::CocoaBeans) && (a_ClickedBlockFace >= BLOCK_FACE_ZM) && (a_ClickedBlockFace <= BLOCK_FACE_XP))
		{
			// Players can't place blocks while in adventure mode.
			if (a_Player->IsGameModeAdventure())
			{
				return false;
			}

			// Cocoa (brown dye) can be planted on jungle logs:

			BlockState ClickedBlock;
			if (a_World->GetBlock(a_ClickedBlockPos, ClickedBlock))
			{
				return false;
			}

			// Check if the block that the player clicked is a jungle log.
			if (ClickedBlock.Type() != BlockType::JungleLog)
			{
				return false;
			}

			// Get the location from the new cocoa pod.
			auto CocoaPos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace, false);

			// Place the cocoa pod:
			if (a_World->GetBlock(CocoaPos) != Block::Air::Air())
			{
				return false;
			}

			if (a_Player->PlaceBlock(CocoaPos, Block::Cocoa::Cocoa(0, a_ClickedBlockFace)))
			{
				if (a_Player->IsGameModeSurvival())
				{
					a_Player->GetInventory().RemoveOneEquippedItem();
				}
				return true;
			}
		}
		return false;
	}





	/** Attempts to use the bonemeal on the plant at the specified (absolute) position.
	The effect of fertilization depends on the plant: https://minecraft.wiki/w/Bone_Meal#Fertilizer
		- grow a few stages
		- grow 1 stage with a chance
		- drop pickups without destroying the plant
		- grow more plants in the vicinity
	If fertilized succesfully, spawn appropriate particle effects, too.
	Returns true if the plant was fertilized successfully, false if not / not a plant.
	Note that successful fertilization doesn't mean successful growth - for blocks that have only a chance to grow,
	fertilization success is reported even in the case when the chance fails (bonemeal still needs to be consumed). */
	static bool FertilizePlant(cWorld & a_World, Vector3i a_BlockPos)
	{
		BlockState DestBlock;
		if (!a_World.GetBlock(a_BlockPos, DestBlock))
		{
			return false;
		}
		switch (DestBlock.Type())
		{
			case BlockType::Wheat:
			case BlockType::Carrots:
			case BlockType::Potatoes:
			case BlockType::MelonStem:
			case BlockType::AttachedMelonStem:
			case BlockType::PumpkinStem:
			case BlockType::AttachedPumpkinStem:
			{
				// Grow by 2 - 5 stages:
				auto NumStages = static_cast<char>(GetRandomProvider().RandInt(2, 5));
				if (a_World.GrowPlantAt(a_BlockPos, NumStages) <= 0)
				{
					return false;
				}
				a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_BlockPos, 0);
				return true;
			}  // case wheat, carrots, potatoes, melon stem, pumpkin stem

			case BlockType::Beetroots:
			{
				// Fix GH #4805.
				// Bonemeal should only advance growth, not spawn produce, and should not be consumed if plant at maturity:
				if (a_World.GrowPlantAt(a_BlockPos, 1) <= 0)
				{
					return false;
				}
				a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_BlockPos, 0);
				if (GetRandomProvider().RandBool(0.25))
				{
					// 75% chance of 1-stage growth, but we hit the 25%, rollback:
					a_World.GrowPlantAt(a_BlockPos, -1);
				}
				return true;
			}  // case beetroots

			case BlockType::AcaciaSapling:
			case BlockType::BirchSapling:
			case BlockType::JungleSapling:
			case BlockType::DarkOakSapling:
			case BlockType::OakSapling:
			case BlockType::SpruceSapling:
			{
				// 45% chance of growing to the next stage / full tree:
				if (GetRandomProvider().RandBool(0.45))
				{
					a_World.GrowPlantAt(a_BlockPos, 1);
				}
				a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_BlockPos, 0);
				return true;
			}  // case sapling
			case BlockType::LargeFern: a_World.SpawnItemPickups(cItem(Item::LargeFern), a_BlockPos); return true;
			case BlockType::Lilac:     a_World.SpawnItemPickups(cItem(Item::Lilac), a_BlockPos); return true;
			case BlockType::Peony:     a_World.SpawnItemPickups(cItem(Item::Peony), a_BlockPos); return true;
			case BlockType::RoseBush:  a_World.SpawnItemPickups(cItem(Item::RoseBush), a_BlockPos); return true;
			case BlockType::Sunflower: a_World.SpawnItemPickups(cItem(Item::Sunflower), a_BlockPos); return true;

			case BlockType::TallGrass:
			case BlockType::Cocoa:
			{
				// Always try to grow 1 stage:
				if (a_World.GrowPlantAt(a_BlockPos, 1) <= 0)
				{
					return false;
				}
				a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_BlockPos, 0);
				return true;
			}  // case tall grass

			case BlockType::RedMushroom:
			case BlockType::BrownMushroom:
			{
				// 40% chance of growing into a large mushroom:
				if (GetRandomProvider().RandBool(0.6))
				{
					return false;
				}
				if (a_World.GrowPlantAt(a_BlockPos, 1) <= 0)
				{
					return false;
				}
				a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_BlockPos, 0);
				return true;
			}  // case red or brown mushroom

			case BlockType::ShortGrass:
			{
				GrowPlantsAround(a_World, a_BlockPos);
				return true;
			}
			default: return false;

			// TODO: case BlockType::SWEET_BERRY_BUSH:
			// TODO: case BlockType::SEA_PICKLE:
			// TODO: case BlockType::KELP:
			// TODO: case BlockType::BAMBOO:
		}  // switch (blockType)
		return false;
	}





	/** Grows new plants around the specified block.
	Places up to 40 new plants, with the following probability:
		- 0 up to 8  big grass  (2-block tall grass)
		- 8 up tp 24 tall grass (1-block tall grass)
		- 0 up to 8  flowers    (biome dependent variants)
	The new plants are spawned within 7 taxicab distance of a_Position, on a grass block.
	Broadcasts a particle for each new spawned plant. */
	static void GrowPlantsAround(cWorld & a_World, const Vector3i a_Position)
	{
		auto & Random = GetRandomProvider();

		auto DoubleGrassCount = Random.RandInt(8U);
		auto GrassCount = Random.RandInt(8U, 24U);
		auto FlowerCount = Random.RandInt(8U);

		// Do a round-robin placement:
		while ((DoubleGrassCount > 0) || (GrassCount > 0) || (FlowerCount > 0))
		{
			// place the big grass:
			if (DoubleGrassCount != 0)
			{
				FindAdjacentGrassAnd<&GrowDoubleTallGrass>(a_World, a_Position);
				DoubleGrassCount--;
			}

			// place the tall grass:
			if (GrassCount != 0)
			{
				FindAdjacentGrassAnd<&GrowTallGrass>(a_World, a_Position);
				GrassCount--;
			}

			// place the flowers
			if (FlowerCount != 0)
			{
				FindAdjacentGrassAnd<&GrowFlower>(a_World, a_Position);
				FlowerCount--;
			}
		}
	}

	static void GrowDoubleTallGrass(cWorld & a_World, const Vector3i a_Position)
	{
		a_World.SetBlock(a_Position, Block::TallGrass::TallGrass(Block::TallGrass::Half::Lower));
		a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_Position, 0);

		const auto Above = a_Position.addedY(1);
		a_World.SetBlock(Above, Block::TallGrass::TallGrass(Block::TallGrass::Half::Upper));
		a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, Above, 0);
	}

	static void GrowTallGrass(cWorld & a_World, const Vector3i a_Position)
	{
		a_World.SetBlock(a_Position, Block::ShortGrass::ShortGrass());
		a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_Position, 0);
	}

	/** Grows a biome-dependent flower according to https://minecraft.wiki/w/Flower#Flower_biomes */
	static void GrowFlower(cWorld & a_World, const Vector3i a_Position)
	{
		using namespace Block;

		auto & Random = GetRandomProvider();
		switch (a_World.GetBiomeAt(a_Position.x, a_Position.z))
		{
			case biPlains:
			case biSunflowerPlains:
			{
				switch (Random.RandInt(9))
				{
					case 0: a_World.SetBlock(a_Position, Dandelion::Dandelion()); break;
					case 1: a_World.SetBlock(a_Position, Poppy::Poppy()); break;
					case 2: a_World.SetBlock(a_Position, Allium::Allium()); break;
					case 3: a_World.SetBlock(a_Position, AzureBluet::AzureBluet()); break;
					case 4: a_World.SetBlock(a_Position, RedTulip::RedTulip()); break;
					case 5: a_World.SetBlock(a_Position, PinkTulip::PinkTulip()); break;
					case 6: a_World.SetBlock(a_Position, WhiteTulip::WhiteTulip()); break;
					case 7: a_World.SetBlock(a_Position, OrangeTulip::OrangeTulip()); break;
					case 8: a_World.SetBlock(a_Position, OxeyeDaisy::OxeyeDaisy()); break;
					case 9: a_World.SetBlock(a_Position, Cornflower::Cornflower()); break;
				}
				break;
			}
			case biSwampland:
			case biSwamplandM:
				a_World.SetBlock(a_Position, BlueOrchid::BlueOrchid()); break;
			case biFlowerForest:
			{
				switch (Random.RandInt(10))
				{
					case 0:  a_World.SetBlock(a_Position, Dandelion::Dandelion()); break;
					case 1:  a_World.SetBlock(a_Position, Poppy::Poppy()); break;
					case 2:  a_World.SetBlock(a_Position, Allium::Allium()); break;
					case 3:  a_World.SetBlock(a_Position, AzureBluet::AzureBluet()); break;
					case 4:  a_World.SetBlock(a_Position, RedTulip::RedTulip()); break;
					case 5:  a_World.SetBlock(a_Position, PinkTulip::PinkTulip()); break;
					case 6:  a_World.SetBlock(a_Position, WhiteTulip::WhiteTulip()); break;
					case 7:  a_World.SetBlock(a_Position, OrangeTulip::OrangeTulip()); break;
					case 8:  a_World.SetBlock(a_Position, OxeyeDaisy::OxeyeDaisy()); break;
					case 9:  a_World.SetBlock(a_Position, Cornflower::Cornflower()); break;
					case 10: a_World.SetBlock(a_Position, LilyOfTheValley::LilyOfTheValley()); break;
				}
				break;
			}
			case biMesa:
			case biMesaBryce:
			case biMesaPlateau:
			case biMesaPlateauF:
			case biMesaPlateauM:
			case biMesaPlateauFM:
			case biMushroomIsland:
			case biMushroomShore:
			case biNether:
			case biEnd:
			{
				break;
			}
			default:
			{
				switch (Random.RandInt(1))
				{
					case 0: a_World.SetBlock(a_Position, Dandelion::Dandelion()); break;
					case 1: a_World.SetBlock(a_Position, AzureBluet::AzureBluet()); break;
				}
				break;
			}
		}
	}

	/** Walks adjacent grass blocks up to 7 taxicab distance away from a_Position and calls the Planter function on the first suitable one found.
	Does nothing if no position suitable for growing was found. */
	template <auto Planter>
	static void FindAdjacentGrassAnd(cWorld & a_World, const Vector3i a_Position)
	{
		auto & Random = GetRandomProvider();
		auto Position = a_Position;

		// Maximum 7 taxicab distance away from centre:
		for (
			int Tries = 0;
			Tries != 8;
			Tries++,

			// Get the adjacent block to visit this iteration:
			Position += Vector3i(
				Random.RandInt(-1, 1),
				Random.RandInt(-1, 1) * (Random.RandInt(2) / 2),  // Y offset, with discouragement to values that aren't zero
				Random.RandInt(-1, 1)
			)
		)
		{
			if (
				!cChunkDef::IsValidHeight(Position) ||
				(a_World.GetBlock(Position).Type() != BlockType::ShortGrass)  // Are we looking at grass?
			)
			{
				// Not grass or invalid height, restart random walk and bail:
				Position = a_Position;
				continue;
			}

			if (Planter == GrowDoubleTallGrass)
			{
				const auto TwoAbove = Position.addedY(2);
				if ((TwoAbove.y >= cChunkDef::Height) || (a_World.GetBlock(TwoAbove).Type() != BlockType::Air))
				{
					// Insufficient space for tall grass:
					continue;
				}
			}

			const auto PlantBase = Position.addedY(1);
			if ((PlantBase.y >= cChunkDef::Height) || (a_World.GetBlock(PlantBase).Type() != BlockType::Air))
			{
				// Insufficient space:
				continue;
			}

			Planter(a_World, PlantBase);
			return;
		}
	}
} ;




