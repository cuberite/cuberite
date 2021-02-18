
// FinishGenPassiveMobs.cpp

#include "Globals.h"

#include "FinishGenPassiveMobs.h"

#include "../../Mobs/Monster.h"
#include "../../MobSpawner.h"

#define DEF_ANIMAL_SPAWN_PERCENT    10
#define DEF_NO_ANIMALS              0

cFinishGenPassiveMobs::cFinishGenPassiveMobs(int a_Seed, cIniFile & a_IniFile, eDimension a_Dimension) :
	m_Noise(a_Seed)
{
	AString SectionName = "Animals";
	int DefaultAnimalSpawnChunkPercentage = DEF_ANIMAL_SPAWN_PERCENT;
	switch (a_Dimension)
	{
		case dimOverworld:
		{
			DefaultAnimalSpawnChunkPercentage = DEF_ANIMAL_SPAWN_PERCENT;
			break;
		}
		case dimNether:
		case dimEnd:  // No nether or end animals (currently)
		{
			DefaultAnimalSpawnChunkPercentage = DEF_NO_ANIMALS;
			break;
		}
		default:
		{
			ASSERT(!"Unhandled world dimension");
			DefaultAnimalSpawnChunkPercentage = DEF_NO_ANIMALS;
			break;
		}
	}  // switch (dimension)
	m_AnimalProbability = a_IniFile.GetValueSetI(SectionName, "AnimalSpawnChunkPercentage", DefaultAnimalSpawnChunkPercentage);
	if ((m_AnimalProbability < 0) || (m_AnimalProbability > 100))
	{
		LOGWARNING("[Animals]: AnimalSpawnChunkPercentage is invalid, using the default of \"%d\".", DefaultAnimalSpawnChunkPercentage);
		m_AnimalProbability = DefaultAnimalSpawnChunkPercentage;
	}
}





void cFinishGenPassiveMobs::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int chunkX = a_ChunkDesc.GetChunkX();
	int chunkZ = a_ChunkDesc.GetChunkZ();
	int ChanceRnd = (m_Noise.IntNoise2DInt(chunkX, chunkZ) / 7) % 100;
	if (ChanceRnd > m_AnimalProbability)
	{
		return;
	}

	eMonsterType RandomMob = GetRandomMob(a_ChunkDesc);
	if (RandomMob == mtInvalidType)
	{
		// No mobs here. Don't send an error, because if the biome was a desert it would return mtInvalidType as well.
		return;
	}

	// Try spawning a pack center 10 times, should get roughly the same probability
	for (int Tries = 0; Tries < 10; Tries++)
	{
		int PackCenterX = (m_Noise.IntNoise2DInt(chunkX + chunkZ, Tries) / 7) % cChunkDef::Width;
		int PackCenterZ = (m_Noise.IntNoise2DInt(chunkX, chunkZ + Tries) / 7) % cChunkDef::Width;
		if (TrySpawnAnimals(a_ChunkDesc, PackCenterX, a_ChunkDesc.GetHeight(PackCenterX, PackCenterZ), PackCenterZ, RandomMob))
		{
			for (int i = 0; i < 3; i++)
			{
				int OffsetX = (m_Noise.IntNoise2DInt(chunkX + chunkZ + i, Tries) / 7) % cChunkDef::Width;
				int OffsetZ = (m_Noise.IntNoise2DInt(chunkX, chunkZ + Tries + i) / 7) % cChunkDef::Width;
				TrySpawnAnimals(a_ChunkDesc, OffsetX, a_ChunkDesc.GetHeight(OffsetX, OffsetZ), OffsetZ, RandomMob);
			}
			return;

		}  // if pack center spawn successful
	}  // for tries
}





bool cFinishGenPassiveMobs::TrySpawnAnimals(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelY, int a_RelZ, eMonsterType AnimalToSpawn)
{
	if ((a_RelY >= cChunkDef::Height - 1) || (a_RelY <= 0))
	{
		return false;
	}

	BLOCKTYPE BlockAtHead    = a_ChunkDesc.GetBlockType(a_RelX, a_RelY + 1, a_RelZ);
	BLOCKTYPE BlockAtFeet    = a_ChunkDesc.GetBlockType(a_RelX, a_RelY, a_RelZ);
	BLOCKTYPE BlockUnderFeet = a_ChunkDesc.GetBlockType(a_RelX, a_RelY - 1, a_RelZ);

	// Check block below (opaque, grass, water), and above (air)
	if ((AnimalToSpawn == mtSquid) && (BlockAtFeet != E_BLOCK_WATER))
	{
		return false;
	}
	if (
		(AnimalToSpawn != mtSquid) &&
		(BlockAtHead != E_BLOCK_AIR) &&
		(BlockAtFeet != E_BLOCK_AIR) &&
		(!cBlockInfo::IsTransparent(BlockUnderFeet))
		)
	{
		return false;
	}
	if (
		(BlockUnderFeet != E_BLOCK_GRASS) &&
		((AnimalToSpawn == mtWolf) || (AnimalToSpawn == mtRabbit) || (AnimalToSpawn == mtCow) || (AnimalToSpawn == mtSheep) || (AnimalToSpawn == mtChicken) || (AnimalToSpawn == mtPig))
		)
	{
		return false;
	}
	if ((AnimalToSpawn == mtMooshroom) && (BlockUnderFeet != E_BLOCK_MYCELIUM))
	{
		return false;
	}

	double AnimalX = static_cast<double>(a_ChunkDesc.GetChunkX() * cChunkDef::Width + a_RelX + 0.5);
	double AnimalY = a_RelY;
	double AnimalZ = static_cast<double>(a_ChunkDesc.GetChunkZ() * cChunkDef::Width + a_RelZ + 0.5);

	auto NewMob = cMonster::NewMonsterFromType(AnimalToSpawn);
	NewMob->SetHealth(NewMob->GetMaxHealth());
	NewMob->SetPosition(AnimalX, AnimalY, AnimalZ);
	FLOGD("Spawning {0} #{1} at {2:.02f}", NewMob->GetClass(), NewMob->GetUniqueID(), NewMob->GetPosition());
	a_ChunkDesc.GetEntities().emplace_back(std::move(NewMob));

	return true;
}





eMonsterType cFinishGenPassiveMobs::GetRandomMob(cChunkDesc & a_ChunkDesc)
{
	std::vector<eMonsterType> ListOfSpawnables;
	int chunkX = a_ChunkDesc.GetChunkX();
	int chunkZ = a_ChunkDesc.GetChunkZ();
	int x = (m_Noise.IntNoise2DInt(chunkX, chunkZ + 10) / 7) % cChunkDef::Width;
	int z = (m_Noise.IntNoise2DInt(chunkX + chunkZ, chunkZ) / 7) % cChunkDef::Width;

	for (auto MobType : cMobSpawner::GetAllowedMobTypes(a_ChunkDesc.GetBiome(x, z)))
	{
		if (cMonster::FamilyFromType(MobType) == cMonster::eFamily::mfPassive)
		{
			ListOfSpawnables.push_back(MobType);
		}
	}

	if (ListOfSpawnables.empty())
	{
		return mtInvalidType;
	}

	auto RandMob = (static_cast<size_t>(m_Noise.IntNoise2DInt(chunkX - chunkZ + 2, chunkX + 5) / 7) % ListOfSpawnables.size());
	return ListOfSpawnables[RandMob];
}
