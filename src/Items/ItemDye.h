
#pragma once

#include "ItemHandler.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "../Blocks/BlockCocoaPod.h"





class cItemDyeHandler :
	public cItemHandler
{
	using Super = cItemHandler;

public:
	cItemDyeHandler(int a_ItemType):
		Super(a_ItemType)
	{
	}





	virtual bool OnItemUse(
		cWorld * a_World,
		cPlayer * a_Player,
		cBlockPluginInterface & a_PluginInterface,
		const cItem & a_HeldItem,
		const Vector3i a_ClickedBlockPos,
		eBlockFace a_ClickedBlockFace
	) override
	{
		if ((a_HeldItem.m_ItemDamage == E_META_DYE_WHITE) && (a_ClickedBlockFace != BLOCK_FACE_NONE))
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
		else if ((a_HeldItem.m_ItemDamage == E_META_DYE_BROWN) && (a_ClickedBlockFace >= BLOCK_FACE_ZM) && (a_ClickedBlockFace <= BLOCK_FACE_XP))
		{
			// Players can't place blocks while in adventure mode.
			if (a_Player->IsGameModeAdventure())
			{
				return false;
			}

			// Cocoa (brown dye) can be planted on jungle logs:
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			a_World->GetBlockTypeMeta(a_ClickedBlockPos, BlockType, BlockMeta);

			// Check if the block that the player clicked is a jungle log.
			if ((BlockType != E_BLOCK_LOG) || ((BlockMeta & 0x03) != E_META_LOG_JUNGLE))
			{
				return false;
			}

			// Get the location from the new cocoa pod.
			auto CocoaPos = AddFaceDirection(a_ClickedBlockPos, a_ClickedBlockFace, false);
			BlockMeta = cBlockCocoaPodHandler::BlockFaceToMeta(a_ClickedBlockFace);

			// Place the cocoa pod:
			if (a_World->GetBlock(CocoaPos) != E_BLOCK_AIR)
			{
				return false;
			}
			if (a_Player->PlaceBlock(CocoaPos, E_BLOCK_COCOA_POD, BlockMeta))
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
	The effect of fertilization depends on the plant: https://minecraft.gamepedia.com/Bone_Meal#Fertilizer
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
		BLOCKTYPE BlockType;
		NIBBLETYPE BlockMeta;
		if (!a_World.GetBlockTypeMeta(a_BlockPos, BlockType, BlockMeta))
		{
			return false;
		}
		switch (BlockType)
		{
			case E_BLOCK_WHEAT:
			case E_BLOCK_CARROTS:
			case E_BLOCK_POTATOES:
			case E_BLOCK_MELON_STEM:
			case E_BLOCK_PUMPKIN_STEM:
			{
				// Grow by 2 - 5 stages:
				auto NumStages = GetRandomProvider().RandInt(2, 5);
				if (a_World.GrowPlantAt(a_BlockPos, NumStages) <= 0)
				{
					return false;
				}
				a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_BlockPos, 0);
				return true;
			}  // case wheat, carrots, potatoes, melon stem, pumpkin stem

			case E_BLOCK_BEETROOTS:
			{
				if (a_World.GrowPlantAt(a_BlockPos, 1) <= 0)
				{
					// Fix GH #4805 (bonemeal should only advance growth, not spawn produce):
					return false;
				}

				a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_BlockPos, 0);

				// 75% chance of 1-stage growth:
				if (!GetRandomProvider().RandBool(0.75))
				{
					// Hit the 25%, rollback:
					a_World.GrowPlantAt(a_BlockPos, -1);
				}

				return true;
			}  // case beetroots

			case E_BLOCK_SAPLING:
			{
				// 45% chance of growing to the next stage / full tree:
				if (GetRandomProvider().RandBool(0.45))
				{
					a_World.GrowPlantAt(a_BlockPos, 1);
				}
				a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_BlockPos, 0);
				return true;
			}  // case sapling

			case E_BLOCK_BIG_FLOWER:
			{
				// Drop the corresponding flower item without destroying the block:
				cItems Pickups;
				switch (BlockMeta)
				{
					case E_META_BIG_FLOWER_SUNFLOWER: Pickups.Add(E_BLOCK_BIG_FLOWER, 1, E_META_BIG_FLOWER_SUNFLOWER); break;
					case E_META_BIG_FLOWER_LILAC:     Pickups.Add(E_BLOCK_BIG_FLOWER, 1, E_META_BIG_FLOWER_LILAC);     break;
					case E_META_BIG_FLOWER_ROSE_BUSH: Pickups.Add(E_BLOCK_BIG_FLOWER, 1, E_META_BIG_FLOWER_ROSE_BUSH); break;
					case E_META_BIG_FLOWER_PEONY:     Pickups.Add(E_BLOCK_BIG_FLOWER, 1, E_META_BIG_FLOWER_PEONY);     break;
				}
				// TODO: Should we call any hook for this?
				a_World.SpawnItemPickups(Pickups, a_BlockPos);
				return true;
			}  // big flower

			case E_BLOCK_TALL_GRASS:
			case E_BLOCK_COCOA_POD:
			{
				// Always try to grow 1 stage:
				if (a_World.GrowPlantAt(a_BlockPos, 1) <= 0)
				{
					return false;
				}
				a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_BlockPos, 0);
				return true;
			}  // case tall grass

			case E_BLOCK_RED_MUSHROOM:
			case E_BLOCK_BROWN_MUSHROOM:
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

			case E_BLOCK_GRASS:
			{
				GrowPlantsAround(a_World, a_BlockPos);
				return true;
			}

			// TODO: case E_BLOCK_SWEET_BERRY_BUSH:
			// TODO: case E_BLOCK_SEA_PICKLE:
			// TODO: case E_BLOCK_KELP:
			// TODO: case E_BLOCK_BAMBOO:
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
		a_World.SetBlock(a_Position, E_BLOCK_BIG_FLOWER, E_META_BIG_FLOWER_DOUBLE_TALL_GRASS);
		a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_Position, 0);

		const auto Above = a_Position.addedY(1);
		a_World.SetBlock(Above, E_BLOCK_BIG_FLOWER, E_META_BIG_FLOWER_DOUBLE_TALL_GRASS | E_META_BIG_FLOWER_TOP);
		a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, Above, 0);
	}

	static void GrowTallGrass(cWorld & a_World, const Vector3i a_Position)
	{
		a_World.SetBlock(a_Position, E_BLOCK_TALL_GRASS, E_META_TALL_GRASS_GRASS);
		a_World.BroadcastSoundParticleEffect(EffectID::PARTICLE_HAPPY_VILLAGER, a_Position, 0);
	}

	/** Grows a biome-dependent flower according to https://minecraft.gamepedia.com/Flower#Flower_biomes */
	static void GrowFlower(cWorld & a_World, const Vector3i a_Position)
	{
		auto & Random = GetRandomProvider();
		switch (a_World.GetBiomeAt(a_Position.x, a_Position.z))
		{
			case biPlains:
			case biSunflowerPlains:
			{
				switch (Random.RandInt(8))
				{
					case 0: a_World.SetBlock(a_Position, E_BLOCK_DANDELION, 0); break;
					case 1: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_POPPY); break;
					case 2: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_ALLIUM); break;
					case 3: a_World.SetBlock(a_Position, E_BLOCK_RED_ROSE, 0); break;  // was renamed to Azure Bluet later
					case 4: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_RED_TULIP); break;
					case 5: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_PINK_TULIP); break;
					case 6: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_WHITE_TULIP); break;
					case 7: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_ORANGE_TULIP); break;
					case 8: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_OXEYE_DAISY); break;
					// TODO: Add cornflower
				}
				break;
			}
			case biSwampland:
			case biSwamplandM:
			{
				a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_BLUE_ORCHID);
				break;
			}
			case biFlowerForest:
			{
				switch (Random.RandInt(8))
				{
					case 0: a_World.SetBlock(a_Position, E_BLOCK_DANDELION, 0); break;
					case 1: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_POPPY); break;
					case 2: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_ALLIUM); break;
					case 3: a_World.SetBlock(a_Position, E_BLOCK_RED_ROSE, 0); break;  // was renamed to Azure Bluet later
					case 4: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_RED_TULIP); break;
					case 5: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_PINK_TULIP); break;
					case 6: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_WHITE_TULIP); break;
					case 7: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_ORANGE_TULIP); break;
					case 8: a_World.SetBlock(a_Position, E_BLOCK_FLOWER, E_META_FLOWER_OXEYE_DAISY); break;
					// TODO: Add cornflower, lily of the valley
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
					case 0: a_World.SetBlock(a_Position, E_BLOCK_DANDELION, 0); break;
					case 1: a_World.SetBlock(a_Position, E_BLOCK_RED_ROSE, 0); break;
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
				!cChunkDef::IsValidHeight(Position.y) ||
				(a_World.GetBlock(Position) != E_BLOCK_GRASS)  // Are we looking at grass?
			)
			{
				// Not grass or invalid height, restart random walk and bail:
				Position = a_Position;
				continue;
			}

			if (Planter == GrowDoubleTallGrass)
			{
				const auto TwoAbove = Position.addedY(2);
				if ((TwoAbove.y >= cChunkDef::Height) || (a_World.GetBlock(TwoAbove) != E_BLOCK_AIR))
				{
					// Insufficient space for tall grass:
					continue;
				}
			}

			const auto PlantBase = Position.addedY(1);
			if ((PlantBase.y >= cChunkDef::Height) || (a_World.GetBlock(PlantBase) != E_BLOCK_AIR))
			{
				// Insufficient space:
				continue;
			}

			Planter(a_World, PlantBase);
			return;
		}
	}
} ;




