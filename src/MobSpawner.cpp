
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "MobSpawner.h"
#include "BlockInfo.h"
#include "Mobs/IncludeAllMonsters.h"
#include "World.h"
#include "Blocks/BlockLeaves.h"





cMobSpawner::cMobSpawner(cMonster::eFamily a_MonsterFamily, const std::set<eEntityType>& a_AllowedTypes) :
	m_MonsterFamily(a_MonsterFamily),
	m_NewPack(true),
	m_MobType(etInvalid)
{
	for (auto a_AllowedType : a_AllowedTypes)
	{
		if (cMonster::FamilyFromType(a_AllowedType) == a_MonsterFamily)
		{
			m_AllowedTypes.insert(a_AllowedType);
		}
	}
}





bool cMobSpawner::CheckPackCenter(BlockState a_Block)
{
	// Packs of non-water mobs can only be centered on an air block
	// Packs of water mobs can only be centered on a water block
	if (m_MonsterFamily == cMonster::mfWater)
	{
		return (a_Block.Type() == BlockType::Water);
	}
	else
	{
		return IsBlockAir(a_Block);
	}
}





eEntityType cMobSpawner::ChooseMobType(EMCSBiome a_Biome)
{
	std::vector<eEntityType> AllowedMobs;

	for (eEntityType MobType : GetAllowedMobTypes(a_Biome))
	{
		auto itr = m_AllowedTypes.find(MobType);
		if (itr != m_AllowedTypes.end())
		{
			AllowedMobs.push_back(MobType);
		}
	}

	// Pick a random mob from the options:
	if (AllowedMobs.empty())
	{
		return etInvalid;
	}

	return AllowedMobs[GetRandomProvider().RandInt(AllowedMobs.size() - 1)];
}





bool cMobSpawner::CanSpawnHere(cChunk * a_Chunk, Vector3i a_RelPos, eEntityType a_MobType, EMCSBiome a_Biome, bool a_DisableSolidBelowCheck)
{
	if ((a_RelPos.y >= cChunkDef::Height - 1) || (a_RelPos.y <= 0))
	{
		return false;
	}

	if (cChunkDef::IsValidHeight(a_RelPos.addedY(-1)) && (a_Chunk->GetBlock(a_RelPos.addedY(-1)).Type() == BlockType::Bedrock))
	{
		return false;   // Make sure mobs do not spawn on bedrock.
	}

	auto & Random = GetRandomProvider();
	auto TargetBlock = a_Chunk->GetBlock(a_RelPos);

	auto BlockLight = a_Chunk->GetBlockLight(a_RelPos);
	auto SkyLight = a_Chunk->GetSkyLight(a_RelPos);
	auto BlockAbove = a_Chunk->GetBlock(a_RelPos.addedY(1));
	auto BlockBelow = a_Chunk->GetBlock(a_RelPos.addedY(-1));

	SkyLight = a_Chunk->GetTimeAlteredLight(SkyLight);

	switch (a_MobType)
	{
		case etBat:
		{
			return
			(
				(a_RelPos.y <= 63) &&
				(BlockLight <= 4) &&
				(SkyLight <= 4) &&
				IsBlockAir(TargetBlock) &&
				(!cBlockInfo::IsTransparent(BlockAbove))
			);
		}

		case etBlaze:
		{
			return
			(
				IsBlockAir(TargetBlock) &&
				IsBlockAir(BlockAbove) &&
				((!cBlockInfo::IsTransparent(BlockBelow)) || (a_DisableSolidBelowCheck)) &&
				(Random.RandBool())
			);
		}

		case etCaveSpider:
		{
			return
			(
				IsBlockAir(TargetBlock) &&
				((!cBlockInfo::IsTransparent(BlockBelow)) || (a_DisableSolidBelowCheck)) &&
				(SkyLight <= 7) &&
				(BlockLight <= 7) &&
				(Random.RandBool())
			);
		}

		case etChicken:
		case etCow:
		case etPig:
		case etHorse:
		case etRabbit:
		case etSheep:
		{
			return
			(
				IsBlockAir(TargetBlock) &&
				IsBlockAir(BlockAbove) &&
				(BlockBelow.Type() == BlockType::GrassBlock) &&
				(SkyLight >= 9)
			);
		}

		case etCreeper:
		case etSkeleton:
		case etZombie:
		{
			return
			(
				IsBlockAir(TargetBlock) &&
				IsBlockAir(BlockAbove) &&
				((!cBlockInfo::IsTransparent(BlockBelow)) || (a_DisableSolidBelowCheck)) &&
				(SkyLight <= 7) &&
				(BlockLight <= 7) &&
				(Random.RandBool())
			);
		}

		case etEnderman:
		{
			if (a_RelPos.y < 250)
			{
				auto BlockTop = a_Chunk->GetBlock(a_RelPos.addedY(2));
				if (IsBlockAir(BlockTop))
				{
					BlockTop = a_Chunk->GetBlock(a_RelPos.addedY(3));
					return
					(
						IsBlockAir(TargetBlock) &&
						IsBlockAir(BlockAbove) &&
						IsBlockAir(BlockTop) &&
						((!cBlockInfo::IsTransparent(BlockBelow)) || (a_DisableSolidBelowCheck)) &&
						(SkyLight <= 7) &&
						(BlockLight <= 7)
					);
				}
			}
			break;
		}

		case etGhast:
		{
			return
			(
				IsBlockAir(TargetBlock) &&
				IsBlockAir(BlockAbove) &&
				(Random.RandBool(0.01))
			);
		}

		case etGuardian:
		{
			return
			(
				(TargetBlock.Type() == BlockType::Water) &&
				(BlockBelow.Type() == BlockType::Water) &&
				(a_RelPos.y >= 45) &&
				(a_RelPos.y <= 62)
			);
		}

		case etMagmaCube:
		case etSlime:
		{
			const int AMOUNT_MOON_PHASES = 8;
			auto maxLight = Random.RandInt(0, 7);
			auto moonPhaseNumber = static_cast<int>(std::floor(a_Chunk->GetWorld()->GetWorldAge().count() / 24000)) % AMOUNT_MOON_PHASES;
			auto moonThreshold = static_cast<float>(std::abs(moonPhaseNumber - (AMOUNT_MOON_PHASES / 2)) / (AMOUNT_MOON_PHASES / 2));
			return
			(
				IsBlockAir(TargetBlock) &&
				IsBlockAir(BlockAbove) &&
				(
					(!cBlockInfo::IsTransparent(BlockBelow)) ||
					(a_DisableSolidBelowCheck)) &&
				(
					(
						(a_RelPos.y <= 40) &&
						a_Chunk->IsSlimeChunk()
					) ||
					(
						(a_Biome == biSwampland) &&
						(a_RelPos.y >= 50) &&
						(a_RelPos.y <= 70) &&
						(SkyLight <= maxLight) &&
						(BlockLight <= maxLight) &&
						(Random.RandBool(moonThreshold)) &&
						(Random.RandBool(0.5))
					)
				)
			);
		}

		case etMooshroom:
		{
			return
				(
					IsBlockAir(TargetBlock) &&
					IsBlockAir(BlockAbove) &&
					(BlockBelow.Type() == BlockType::Mycelium) &&
				(
					(a_Biome == biMushroomShore) ||
					(a_Biome == biMushroomIsland)
				)
			);
		}

		case etOcelot:
		{
			return (
				IsBlockAir(TargetBlock) &&
				IsBlockAir(BlockAbove) &&
				(
					(BlockBelow.Type() == BlockType::GrassBlock) || cBlockLeavesHandler::IsBlockLeaves(BlockBelow)
				) &&
				(a_RelPos.y >= 62) &&
				(Random.RandBool(2.0 / 3.0))
			);
		}

		case etSpider:
		{
			bool CanSpawn = true;
			bool HasFloor = false;
			for (int x = 0; x < 2; ++x)
			{
				for (int z = 0; z < 2; ++z)
				{
					CanSpawn = a_Chunk->UnboundedRelGetBlock(a_RelPos.addedXZ(x, z), TargetBlock);
					CanSpawn = CanSpawn && IsBlockAir(TargetBlock);
					if (!CanSpawn)
					{
						return false;
					}
					HasFloor = (
						HasFloor ||
						(
							a_Chunk->UnboundedRelGetBlock(a_RelPos + Vector3i(x, -1, z), TargetBlock) &&
							!cBlockInfo::IsTransparent(TargetBlock)
						)
					);
				}
			}
			return CanSpawn && HasFloor && (SkyLight <= 7) && (BlockLight <= 7);
		}

		case etSquid:
		{
			return (
				(TargetBlock.Type() == BlockType::Water) &&
				(a_RelPos.y >= 45) &&
				(a_RelPos.y <= 62)
			);
		}

		case etWitherSkeleton:
		{
			return (
				IsBlockAir(TargetBlock) &&
				IsBlockAir(BlockAbove) &&
				((!cBlockInfo::IsTransparent(BlockBelow)) || (a_DisableSolidBelowCheck)) &&
				(SkyLight <= 7) &&
				(BlockLight <= 7) &&
				(Random.RandBool(0.6))
			);
		}

		case etWolf:
		{
			return (
				(TargetBlock.Type() == BlockType::GrassBlock) &&
				IsBlockAir(BlockAbove) &&
				(
					(a_Biome == biColdTaiga) ||
					(a_Biome == biColdTaigaHills) ||
					(a_Biome == biColdTaigaM) ||
					(a_Biome == biForest) ||
					(a_Biome == biTaiga) ||
					(a_Biome == biTaigaHills) ||
					(a_Biome == biTaigaM) ||
					(a_Biome == biMegaTaiga) ||
					(a_Biome == biMegaTaigaHills)
				)
			);
		}

		case etZombifiedPiglin:
		{
			return (
				IsBlockAir(TargetBlock) &&
				IsBlockAir(BlockAbove) &&
				((!cBlockInfo::IsTransparent(BlockBelow)) || (a_DisableSolidBelowCheck))
			);
		}

		default:
		{
			LOGD("MG TODO: Write spawning rule for mob type %d", a_MobType);
			return false;
		}
	}
	return false;
}





std::set<eEntityType> cMobSpawner::GetAllowedMobTypes(EMCSBiome a_Biome)
{
	std::set<eEntityType> ListOfSpawnables;
	// Check biomes first to get a list of animals
	switch (a_Biome)
	{
		// Mooshroom only - no other mobs on mushroom islands
		case biMushroomIsland:
		case biMushroomShore:
		{
			ListOfSpawnables.insert(etMooshroom);
			return ListOfSpawnables;
		}

		// Add Squid in ocean and river biomes
		case biOcean:
		case biFrozenOcean:
		case biFrozenRiver:
		case biRiver:
		case biDeepOcean:
		{
			ListOfSpawnables.insert(etGuardian);
			break;
		}

		// Add ocelots in jungle biomes
		case biJungle:
		case biJungleHills:
		case biJungleEdge:
		case biJungleM:
		case biJungleEdgeM:
		{
			ListOfSpawnables.insert(etOcelot);
			break;
		}

		// Add horses in plains-like biomes
		case biPlains:
		case biSunflowerPlains:
		case biSavanna:
		case biSavannaPlateau:
		case biSavannaM:
		case biSavannaPlateauM:
		{
			ListOfSpawnables.insert(etHorse);
			break;
		}

		// Add wolves in forest biomes
		case biForest:
		{
			ListOfSpawnables.insert(etWolf);
			break;
		}

		// Add wolves and rabbits in all taiga biomes
		case biColdTaiga:
		case biColdTaigaM:
		case biColdTaigaHills:
		case biTaiga:
		case biTaigaHills:
		case biTaigaM:
		case biMegaTaiga:
		case biMegaTaigaHills:
		{
			ListOfSpawnables.insert(etWolf);
			ListOfSpawnables.insert(etRabbit);
			break;
		}

		// Add rabbits in desert and flower forest biomes
		case biDesert:
		case biDesertHills:
		case biDesertM:
		case biFlowerForest:
		{
			ListOfSpawnables.insert(etRabbit);
			break;
		}

		// Nothing special about this biome
		default:
		{
			break;
		}
	}

	// Overworld
	if (
		(a_Biome != biDesertHills) &&
		(a_Biome != biDesert) &&
		(a_Biome != biDesertM) &&
		(a_Biome != biBeach) &&
		(a_Biome != biOcean) &&
		(a_Biome != biDeepOcean))
	{
		ListOfSpawnables.insert(etSheep);
		ListOfSpawnables.insert(etPig);
		ListOfSpawnables.insert(etCow);
		ListOfSpawnables.insert(etChicken);
		ListOfSpawnables.insert(etEnderman);
		ListOfSpawnables.insert(etSlime);
	}

	ListOfSpawnables.insert(etBat);
	ListOfSpawnables.insert(etSpider);
	ListOfSpawnables.insert(etZombie);
	ListOfSpawnables.insert(etSkeleton);
	ListOfSpawnables.insert(etCreeper);
	ListOfSpawnables.insert(etSquid);

	// Nether
	ListOfSpawnables.insert(etBlaze);
	ListOfSpawnables.insert(etGhast);
	ListOfSpawnables.insert(etMagmaCube);
	ListOfSpawnables.insert(etWitherSkeleton);
	ListOfSpawnables.insert(etZombifiedPiglin);

	return ListOfSpawnables;
}





cMonster * cMobSpawner::TryToSpawnHere(cChunk * a_Chunk, Vector3i a_RelPos, EMCSBiome a_Biome, int & a_MaxPackSize)
{
	// If too close to any player, don't spawn anything
	auto AbsPos = a_Chunk->RelativeToAbsolute(a_RelPos);
	static const double RangeLimit = 24;
	if (
		a_Chunk->GetWorld()->DoWithNearestPlayer(
			AbsPos,
			RangeLimit,
			[](cPlayer & a_Player)
			{
				return true;
			}
		)
	)
	{
		return nullptr;
	}

	if (m_NewPack)
	{
		m_MobType = ChooseMobType(a_Biome);
		if (m_MobType == etInvalid)
		{
			return nullptr;
		}
		if (m_MobType == etWitherSkeleton)
		{
			a_MaxPackSize = 5;
		}
		else if (m_MobType == etWolf)
		{
			a_MaxPackSize = 8;
		}
		else if (m_MobType == etGhast)
		{
			a_MaxPackSize = 1;
		}
		m_NewPack = false;
	}

	if ((m_AllowedTypes.find(m_MobType) != m_AllowedTypes.end()) && CanSpawnHere(a_Chunk, a_RelPos, m_MobType, a_Biome))
	{
		auto NewMob = cMonster::NewMonsterFromType(m_MobType);
		auto NewMobPtr = NewMob.get();
		if (NewMob)
		{
			m_Spawned.push_back(std::move(NewMob));
		}
		return NewMobPtr;
	}

	return nullptr;
}





void cMobSpawner::NewPack()
{
	m_NewPack = true;
}





bool cMobSpawner::CanSpawnAnything(void)
{
	return !m_AllowedTypes.empty();
}




