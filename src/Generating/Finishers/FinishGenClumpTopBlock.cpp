// FinishGenClumpTopBlock.cpp

#include "FinishGenClumpTopBlock.h"





void cFinishGenClumpTopBlock::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();

	int NoiseVal = m_Noise.IntNoise2DInt(ChunkX, ChunkZ);
	EMCSBiome Biome = a_ChunkDesc.GetBiome(cChunkDef::Width / 2, cChunkDef::Width / 2);
	BiomeInfo info = m_FlowersPerBiome[static_cast<size_t>(Biome)];

	const auto & PossibleBlocks = info.m_Blocks;
	if (PossibleBlocks.empty())
	{
		// No need to go any further. This biome can't generate any blocks.
		return;
	}

	int NumClumps = info.m_MaxNumClumpsPerChunk - info.m_MinNumClumpsPerChunk;
	if (NumClumps == 0)
	{
		NumClumps = 1;
	}

	NumClumps = NoiseVal % NumClumps + info.m_MinNumClumpsPerChunk;
	for (int i = 0; i < NumClumps; i++)
	{
		int Val1 = m_Noise.IntNoise2DInt(ChunkX * ChunkZ * i, ChunkZ + ChunkX + i);
		int Val2 = m_Noise.IntNoise2DInt(ChunkZ * ChunkX + i, ChunkZ - ChunkX * i);
		int BlockVal = m_Noise.IntNoise2DInt(Val1, Val2);

		int PosX = Val1 % (cChunkDef::Width - RANGE_FROM_CENTER * 2) + RANGE_FROM_CENTER;
		int PosZ = Val2 % (cChunkDef::Width - RANGE_FROM_CENTER * 2) + RANGE_FROM_CENTER;

		int TotalWeight = 0;
		for (const auto & Block : PossibleBlocks)
		{
			TotalWeight += Block.m_Weight;
		}

		// Prevent division by 0
		TotalWeight = (TotalWeight != 0) ? TotalWeight : 1;
		int Weight = BlockVal % TotalWeight;
		for (const auto & Block : PossibleBlocks)
		{
			Weight -= Block.m_Weight;
			if (Weight < 0)
			{
				TryPlaceFoliageClump(a_ChunkDesc, PosX, PosZ, Block.m_BlockType, Block.m_BlockMeta, Block.m_BlockType == E_BLOCK_BIG_FLOWER);
				break;
			}
		}
	}
}





void cFinishGenClumpTopBlock::TryPlaceFoliageClump(cChunkDesc & a_ChunkDesc, int a_CenterX, int a_CenterZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, bool a_IsDoubleTall)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();

	int NumBlocks = m_Noise.IntNoise2DInt(a_CenterX + ChunkX * 16, a_CenterZ + ChunkZ * 16) % (MAX_NUM_FOLIAGE - MIN_NUM_FOLIAGE) + MIN_NUM_FOLIAGE + 1;
	for (int i = 1; i < NumBlocks; i++)
	{
		int Rnd = m_Noise.IntNoise2DInt(ChunkX + ChunkZ + i, ChunkX - ChunkZ - i) / 59;
		int x = a_CenterX + (((Rnd % 59) % RANGE_FROM_CENTER * 2) - RANGE_FROM_CENTER);
		int z = a_CenterZ + (((Rnd / 97) % RANGE_FROM_CENTER * 2) - RANGE_FROM_CENTER);
		int Top = a_ChunkDesc.GetHeight(x, z);

		// Doesn't place if the blocks can't be placed. Checked value also depends on a_IsDoubleTall
		if (Top + 1 + (a_IsDoubleTall ? 1 : 0) >= cChunkDef::Height)
		{
			continue;
		}

		auto GroundBlockType = a_ChunkDesc.GetBlockType(x, Top, z);
		if (
			(GroundBlockType == E_BLOCK_GRASS) || (
				(GroundBlockType == E_BLOCK_MYCELIUM) && ((a_BlockType == E_BLOCK_RED_MUSHROOM) || (a_BlockType == E_BLOCK_BROWN_MUSHROOM))
			)
			)
		{
			a_ChunkDesc.SetBlockTypeMeta(x, Top + 1, z, a_BlockType, a_BlockMeta);
			if (a_IsDoubleTall)
			{
				a_ChunkDesc.SetBlockTypeMeta(x, Top + 2, z, E_BLOCK_BIG_FLOWER, E_META_BIG_FLOWER_TOP);
				a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(Top + 2));
			}
			else
			{
				a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(Top + 1));
			}
		}
	}

}





void cFinishGenClumpTopBlock::ParseConfigurationString(const AString & a_RawClumpInfo, std::vector<BiomeInfo> & a_Output)
{
	// Initialize the vector for all biomes.
	for (int i = static_cast<int>(a_Output.size()); i <= static_cast<int>(biMaxVariantBiome); i++)
	{
		// Push empty BiomeInfo structure to be later directly accessed by index:
		a_Output.emplace_back();
	}

	AStringVector ClumpInfo = StringSplitAndTrim(a_RawClumpInfo, "=");

	// Information about a clump is divided in 2 parts. The biomes they can be in and the blocks that can be placed.
	if (ClumpInfo.size() != 2)
	{
		LOGWARNING("OverworldClumpFoliage: Data missing for \"%s\". Please divide biome and blocks with a semi colon", a_RawClumpInfo.c_str());
		return;
	}

	AStringVector Biomes = StringSplitAndTrim(ClumpInfo[0], ";");
	AStringVector Blocks = StringSplitAndTrim(ClumpInfo[1], ";");

	for (const auto & RawBiomeInfo : Biomes)
	{
		const AStringVector BiomeInfo = StringSplitAndTrim(RawBiomeInfo, ",");
		const AString & BiomeName = BiomeInfo[0];
		const EMCSBiome Biome = StringToBiome(BiomeName);
		if (Biome == biInvalidBiome)
		{
			LOGWARNING("Biome \"%s\" is invalid.", BiomeName.c_str());
			continue;
		}

		// The index of Biome in the output vector.
		const size_t BiomeIndex = static_cast<size_t>(Biome);

		if (BiomeInfo.size() == 2)
		{
			// Only the minimum amount of clumps per chunk is changed.
			int MinNumClump = 1;
			if (!StringToInteger(BiomeInfo[1], MinNumClump))
			{
				LOGWARNING("OverworldClumpFoliage: Invalid data in \"%s\". Second parameter is either not existing or a number", RawBiomeInfo.c_str());
				continue;
			}
			a_Output[BiomeIndex].m_MinNumClumpsPerChunk = MinNumClump;

			// In case the minimum number is higher than the current maximum value we change the max to the minimum value.
			a_Output[BiomeIndex].m_MaxNumClumpsPerChunk = std::max(MinNumClump, a_Output[BiomeIndex].m_MaxNumClumpsPerChunk);
		}
		else if (BiomeInfo.size() == 3)
		{
			// Both the minimum and maximum amount of clumps per chunk is changed.
			int MinNumClumps = 0, MaxNumClumps = 1;
			if (!StringToInteger(BiomeInfo[1], MinNumClumps) || !StringToInteger(BiomeInfo[2], MaxNumClumps))
			{
				LOGWARNING("Invalid data in \"%s\". Second parameter is either not existing or a number", RawBiomeInfo.c_str());
				continue;
			}

			a_Output[BiomeIndex].m_MaxNumClumpsPerChunk = MaxNumClumps + 1;
			a_Output[BiomeIndex].m_MinNumClumpsPerChunk = MinNumClumps;
		}

		// TODO: Make the weight configurable.
		for (const auto & BlockName : Blocks)
		{
			cItem Block;
			if (!StringToItem(BlockName, Block) || !IsValidBlock(Block.m_ItemType))
			{
				LOGWARNING("Block \"%s\" is invalid", BlockName.c_str());
				continue;
			}

			// Construct the FoliageInfo:
			a_Output[BiomeIndex].m_Blocks.emplace_back(
				static_cast<BLOCKTYPE>(Block.m_ItemType), static_cast<NIBBLETYPE>(Block.m_ItemDamage), 100
			);
		}
	}
}





std::vector<cFinishGenClumpTopBlock::BiomeInfo> cFinishGenClumpTopBlock::ParseIniFile(cIniFile & a_IniFile, const AString & a_ClumpPrefix)
{
	std::vector<cFinishGenClumpTopBlock::BiomeInfo> Foliage;
	int NumGeneratorValues = a_IniFile.GetNumValues("Generator");
	int GeneratorKeyId = a_IniFile.FindKey("Generator");
	for (int i = 0; i < NumGeneratorValues; i++)
	{
		AString ValueName = a_IniFile.GetValueName("Generator", i);
		if (ValueName.substr(0, a_ClumpPrefix.size()) == a_ClumpPrefix)
		{
			AString RawClump = a_IniFile.GetValue(GeneratorKeyId, i);
			cFinishGenClumpTopBlock::ParseConfigurationString(RawClump, Foliage);
		}
	}

	if (Foliage.empty())
	{
		cFinishGenClumpTopBlock::ParseConfigurationString(a_IniFile.GetValueSet("Generator", a_ClumpPrefix + "-1", "Forest, -2, 2; ForestHills, -3, 2; FlowerForest = yellowflower; redflower; lilac; rosebush"), Foliage);
		cFinishGenClumpTopBlock::ParseConfigurationString(a_IniFile.GetValueSet("Generator", a_ClumpPrefix + "-2", "Plains, -2, 1; SunflowerPlains = yellowflower; redflower; azurebluet; redtulip; orangetulip; whitetulip; pinktulip; oxeyedaisy"), Foliage);
		cFinishGenClumpTopBlock::ParseConfigurationString(a_IniFile.GetValueSet("Generator", a_ClumpPrefix + "-3", "SunflowerPlains, 1, 2 = sunflower"), Foliage);
		cFinishGenClumpTopBlock::ParseConfigurationString(a_IniFile.GetValueSet("Generator", a_ClumpPrefix + "-4", "FlowerForest, 2, 5 = allium; redtulip; orangetulip; whitetulip; pinktulip; oxeyedaisy"), Foliage);
		cFinishGenClumpTopBlock::ParseConfigurationString(a_IniFile.GetValueSet("Generator", a_ClumpPrefix + "-5", "Swampland; SwamplandM = brownmushroom; redmushroom; blueorchid"), Foliage);
		cFinishGenClumpTopBlock::ParseConfigurationString(a_IniFile.GetValueSet("Generator", a_ClumpPrefix + "-6", "MushroomIsland; MushroomShore; MegaTaiga; MegaTaigaHills; MegaSpruceTaiga; MegaSpruceTaigaHills = brownmushroom; redmushroom"), Foliage);
		cFinishGenClumpTopBlock::ParseConfigurationString(a_IniFile.GetValueSet("Generator", a_ClumpPrefix + "-7", "RoofedForest, 1, 5; RoofedForestM, 1, 5 = rosebush; peony; lilac; grass"), Foliage);
	}

	return Foliage;
}
