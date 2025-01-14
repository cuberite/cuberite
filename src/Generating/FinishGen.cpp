
// FinishGen.cpp

/* Implements the various finishing generators:
	- cFinishGenSnow
	- cFinishGenIce
	- cFinishGenSprinkleFoliage
*/

#include "Globals.h"

#include "IniFile.h"
#include "MobSpawner.h"
#include "BlockInfo.h"
#include "Chunk.h"
#include "World.h"

#include "Generating/FinishGen.h"

#include "Simulator/FluidSimulator.h"  // for cFluidSimulator::CanWashAway()
#include "Simulator/FireSimulator.h"

#include "Protocol/Palettes/Upgrade.h"
#include "Registries/BlockItemConverter.h"

#include "Blocks/BlockAir.h"
#include "Blocks/BlockBigFlower.h"
#include "Blocks/BlockFluid.h"
#include "Blocks/BlockLeaves.h"
#include "Blocks/BlockLog.h"




#define DEF_NETHER_WATER_SPRINGS    "0, 1; 255, 1"
#define DEF_NETHER_LAVA_SPRINGS     "0, 0; 30, 0; 31, 50; 120, 50; 127, 0"
#define DEF_OVERWORLD_WATER_SPRINGS "0, 0; 10, 10; 11, 75; 16, 83; 20, 83; 24, 78; 32, 62; 40, 40; 44, 15; 48, 7; 56, 2; 64, 1; 255, 0"
#define DEF_OVERWORLD_LAVA_SPRINGS  "0, 0; 10, 5; 11, 45; 48, 2; 64, 1; 255, 0"
#define DEF_END_WATER_SPRINGS       "0, 1; 255, 1"
#define DEF_END_LAVA_SPRINGS        "0, 1; 255, 1"
#define DEF_ANIMAL_SPAWN_PERCENT    10
#define DEF_NO_ANIMALS              0





static inline bool IsWater(BlockState a_Block)
{
	return (a_Block.Type() == BlockType::Water);
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenNetherClumpFoliage:

void cFinishGenNetherClumpFoliage::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();

	int Val1 = m_Noise.IntNoise2DInt(ChunkX ^ ChunkZ, ChunkZ + ChunkX);
	int Val2 = m_Noise.IntNoise2DInt(ChunkZ ^ ChunkX, ChunkZ - ChunkX);

	int PosX = Val1 % 16;
	int PosZ = Val2 % 16;

	for (int y = 1; y < cChunkDef::Height; y++)
	{
		if (a_ChunkDesc.GetBlock({PosX, y, PosZ}).Type() != BlockType::Air)
		{
			continue;
		}

		if (!cBlockInfo::IsSolid(a_ChunkDesc.GetBlock({PosX, y - 1, PosZ})))  // Only place on solid blocks
		{
			continue;
		}

		// Choose what block to use.
		NOISE_DATATYPE BlockType = m_Noise.IntNoise3D(static_cast<int>(ChunkX), y, static_cast<int>(ChunkZ));
		if (BlockType < -0.7)
		{
			TryPlaceClump(a_ChunkDesc, PosX, y, PosZ, Block::BrownMushroom::BrownMushroom());
		}
		else if (BlockType < 0)
		{
			TryPlaceClump(a_ChunkDesc, PosX, y, PosZ, Block::RedMushroom::RedMushroom());
		}
		else if (BlockType < 0.7)
		{
			TryPlaceClump(a_ChunkDesc, PosX, y, PosZ, Block::Fire::Fire());
		}
	}
}





void cFinishGenNetherClumpFoliage::TryPlaceClump(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelY, int a_RelZ, BlockState a_Block)
{
	bool IsFireBlock = a_Block.Type() == BlockType::Fire;

	int MinX = a_RelX - 4;
	if (MinX < 0)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MinX = 0;
	}

	int MaxX = a_RelX + 4;
	if (MaxX > cChunkDef::Width)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MaxX = cChunkDef::Width;
	}

	int MinZ = a_RelZ - 4;
	if (MinZ < 0)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MinZ = 0;
	}

	int MaxZ = a_RelZ + 4;
	if (MaxZ > cChunkDef::Width)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MaxZ = cChunkDef::Width;
	}

	int MinY = a_RelY - 2;
	if (MinY < 0)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MinY = 0;
	}

	int MaxY = a_RelY + 2;
	if (MaxY > cChunkDef::Height)  // Check if the coordinate is outside the chunk. If it it then adjust it.
	{
		MaxY = cChunkDef::Height;
	}

	for (int x = MinX; x < MaxX; x++)
	{
		int xx = a_ChunkDesc.GetChunkX() * cChunkDef::Width + x;
		for (int z = MinZ; z < MaxZ; z++)
		{
			int zz = a_ChunkDesc.GetChunkZ() * cChunkDef::Width + z;
			for (int y = MinY; y < MaxY; y++)
			{
				if (
					((x < 0) || (x >= cChunkDef::Width)) ||
					((y < 0) || (y >= cChunkDef::Height)) ||
					((z < 0) || (z >= cChunkDef::Width))
					)
				{
					continue;
				}

				if (a_ChunkDesc.GetBlock({x, y, z}).Type() != BlockType::Air)  // Don't replace non air blocks.
				{
					continue;
				}

				auto BlockBelow = a_ChunkDesc.GetBlock({x, y - 1, z});
				if (!cBlockInfo::FullyOccupiesVoxel(BlockBelow))  // Only place on solid blocks
				{
					continue;
				}

				if (IsFireBlock)  // don't place fire on non-forever burning blocks.
				{
					if (!cFireSimulator::DoesBurnForever(BlockBelow))
					{
						continue;
					}
				}

				NOISE_DATATYPE Val = m_Noise.IntNoise2D(xx, zz);
				if (Val < -0.5)
				{
					a_ChunkDesc.SetBlock({x, y, z}, a_Block);
				}
			}
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenClumpTopBlock

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
				TryPlaceFoliageClump(a_ChunkDesc, PosX, PosZ, Block.m_Block, cBlockBigFlowerHandler::IsBlockBigFlower(Block.m_Block));
				break;
			}
		}
	}
}





void cFinishGenClumpTopBlock::TryPlaceFoliageClump(cChunkDesc & a_ChunkDesc, int a_CenterX, int a_CenterZ, BlockState a_Block, bool a_IsDoubleTall)
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

		auto GroundBlockType = a_ChunkDesc.GetBlock({x, Top, z});
		if (
			(GroundBlockType.Type() == BlockType::GrassBlock) || (
				(GroundBlockType == BlockType::Mycelium) && ((a_Block.Type() == BlockType::RedMushroomBlock) || (a_Block.Type() == BlockType::BrownMushroomBlock))
			)
		)
		{
			a_ChunkDesc.SetBlock({x, Top + 1, z}, a_Block);
			if (a_IsDoubleTall)
			{
				using namespace Block;
				switch (a_Block.Type())
				{
					case BlockType::TallGrass: a_ChunkDesc.SetBlock({x, Top + 2, z}, TallGrass::TallGrass(TallGrass::Half::Upper)); break;
					case BlockType::LargeFern: a_ChunkDesc.SetBlock({x, Top + 2, z}, LargeFern::LargeFern(LargeFern::Half::Upper)); break;
					case BlockType::Lilac:     a_ChunkDesc.SetBlock({x, Top + 2, z}, Lilac::Lilac(Lilac::Half::Upper)); break;
					case BlockType::Peony:     a_ChunkDesc.SetBlock({x, Top + 2, z}, Peony::Peony(Peony::Half::Upper)); break;
					case BlockType::RoseBush:  a_ChunkDesc.SetBlock({x, Top + 2, z}, RoseBush::RoseBush(RoseBush::Half::Upper)); break;
					case BlockType::Sunflower: a_ChunkDesc.SetBlock({x, Top + 2, z}, Sunflower::Sunflower(Sunflower::Half::Upper)); break;
					default: break;
				}
				a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(Top + 2));
				// Check if the switch statement is ok
#ifdef NDEBUG
				ASSERT(a_ChunkDesc.GetBlock(x, Top + 1, z).Type() == a_ChunkDesc.GetBlock(x, Top + 2, z).Type());
#endif
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
			if (!StringToItem(BlockName, Block) || (Block.m_ItemType == Item::Air))
			{
				LOGWARNING("Block \"%s\" is invalid", BlockName.c_str());
				continue;
			}

			// Construct the FoliageInfo:
			a_Output[BiomeIndex].m_Blocks.emplace_back(BlockItemConverter::FromItem(Block.m_ItemType), 100);
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
		cFinishGenClumpTopBlock::ParseConfigurationString(a_IniFile.GetValueSet("Generator", a_ClumpPrefix + "-8", "MegaTaiga; MegaTaigaHills = deadbush"), Foliage);
	}

	return Foliage;
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenGlowStone:

void cFinishGenGlowStone::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();

	// Change the number of attempts to create a vein depending on the maximum height of the chunk. A standard Nether could have 5 veins at most.
	int NumGlowStone = m_Noise.IntNoise2DInt(ChunkX, ChunkZ) % a_ChunkDesc.GetMaxHeight() / 23;

	for (int i = 1; i <= NumGlowStone; i++)
	{
		// The maximum size for a string of glowstone can get 3 - 5 blocks long
		int Size = 3 + m_Noise.IntNoise3DInt(ChunkX, i, ChunkZ) % 3;

		// Generate X / Z coordinates.
		int X = Size + (m_Noise.IntNoise2DInt(i, Size) % (cChunkDef::Width - Size * 2));
		int Z = Size + (m_Noise.IntNoise2DInt(X, i)    % (cChunkDef::Width - Size * 2));

		int Height = a_ChunkDesc.GetHeight(X, Z);
		for (int y = Height; y > Size; y--)
		{
			if (!cBlockInfo::IsSolid(a_ChunkDesc.GetBlock({X, y, Z})))
			{
				// Current block isn't solid, bail out
				continue;
			}

			if (a_ChunkDesc.GetBlock({X, y - 1, Z}) != Block::Air::Air())
			{
				// The block below isn't air, bail out
				continue;
			}

			if ((m_Noise.IntNoise3DInt(X, y, Z) % 100) < 95)
			{
				// Have a 5% chance of creating the glowstone
				continue;
			}

			TryPlaceGlowstone(a_ChunkDesc, X, y, Z, Size, 5 + m_Noise.IntNoise3DInt(X, y, Z) % 7);
			break;
		}
	}
}





void cFinishGenGlowStone::TryPlaceGlowstone(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelY, int a_RelZ, int a_Size, int a_NumStrings)
{
	// The starting point of every glowstone string
	Vector3i StartPoint = Vector3i(a_RelX, a_RelY, a_RelZ);

	// Array with possible directions for a string of glowstone to go to.
	const Vector3i AvailableDirections[] =
	{
		{ -1,  0,  0 }, { 1, 0, 0 },
		{  0, -1,  0 },  // Don't let the glowstone go up
		{  0,  0, -1 }, { 0, 0, 1 },

		// Diagonal direction. Only X or Z with Y.
		// If all were changed the glowstone string looks awkward
		{ 0, -1,  1 }, {  1, -1, 0 },
		{ 0, -1, -1 }, { -1, -1, 0 },

	};

	for (int i = 1; i <= a_NumStrings; i++)
	{
		// The current position of the string that is being generated
		Vector3i CurrentPos = Vector3i(StartPoint);

		// A vector where the previous direction of a glowstone string is stored.
		// This is used to make the strings change direction when going one block further
		Vector3i PreviousDirection = Vector3i();

		for (int j = 0; j < a_Size; j++)
		{
			Vector3i Direction = AvailableDirections[static_cast<size_t>(m_Noise.IntNoise3DInt(CurrentPos.x, CurrentPos.y * i, CurrentPos.z)) % ARRAYCOUNT(AvailableDirections)];
			int Attempts = 2;  // multiply by 1 would make no difference, so multiply by 2 instead

			while (Direction.Equals(PreviousDirection))
			{
				// To make the glowstone branches look better we want to make the direction change every time.
				Direction = AvailableDirections[static_cast<size_t>(m_Noise.IntNoise3DInt(CurrentPos.x, CurrentPos.y * i * Attempts, CurrentPos.z)) % ARRAYCOUNT(AvailableDirections)];
				Attempts++;
			}

			// Update the previous direction variable
			PreviousDirection = Direction;

			// Update the position of the glowstone string
			CurrentPos += Direction;
			if (cBlockInfo::IsSolid(a_ChunkDesc.GetBlock(CurrentPos)) && (a_ChunkDesc.GetBlock(CurrentPos).Type() != BlockType::Glowstone))
			{
				// The glowstone hit something solid, and it wasn't glowstone. Stop the string.
				break;
			}

			// Place a glowstone block.
			a_ChunkDesc.SetBlock(CurrentPos, Block::Glowstone::Glowstone());
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenTallGrass:

void cFinishGenTallGrass::GenFinish(cChunkDesc & a_ChunkDesc)
{
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		int xx = x + a_ChunkDesc.GetChunkX() * cChunkDef::Width;
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			int zz = z + a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
			int BiomeDensity = GetBiomeDensity(a_ChunkDesc.GetBiome(x, z));

			// Choose if we want to place long grass here. If not then bail out:
			if ((m_Noise.IntNoise2DInt(xx + m_Noise.IntNoise1DInt(xx), zz + m_Noise.IntNoise1DInt(zz)) / 7 % 100) > BiomeDensity)
			{
				continue;
			}

			// Get the top block + 1. This is the place where the grass would finaly be placed:
			int y = a_ChunkDesc.GetHeight(x, z) + 1;

			if (y >= cChunkDef::Height - 1)
			{
				continue;
			}

			// Walk below trees:
			auto BlockBelow = a_ChunkDesc.GetBlock({x, y - 1, z});
			bool failed = false;  // marker if the search for a valid position was successful

			while (
				cBlockLeavesHandler::IsBlockLeaves(BlockBelow) ||
				cBlockLogHandler::IsBlockLog(BlockBelow) ||
				IsBlockAir(BlockBelow)
			)
			{
				y--;
				if (!cChunkDef::IsValidHeight({x, y - 1, z}))
				{
					failed = true;
					break;
				}
				BlockBelow = a_ChunkDesc.GetBlock({x, y - 1, z});
			}

			if (failed)
			{
				continue;
			}

			// Check if long grass can be placed:
			if (
				(a_ChunkDesc.GetBlock({x, y, z}).Type() != BlockType::Air) ||
				((a_ChunkDesc.GetBlock({x, y - 1, z}).Type() != BlockType::GrassBlock) && (a_ChunkDesc.GetBlock({x, y - 1, z}).Type() != BlockType::Dirt))

				)
			{
				continue;
			}

			// Choose what long grass meta we should use:
			int GrassType = m_Noise.IntNoise2DInt(xx * 50, zz * 50) / 7 % 100;
			if ((GrassType < 60) && CanGrassGrow(a_ChunkDesc.GetBiome(x, z)))
			{
				a_ChunkDesc.SetBlock({x, y, z}, Block::ShortGrass::ShortGrass());
				a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(y));
			}
			else if ((GrassType < 90) && CanFernGrow(a_ChunkDesc.GetBiome(x, z)))
			{
				a_ChunkDesc.SetBlock({x, y, z}, Block::Fern::Fern());
				a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(y));
			}
			else if (!IsBiomeVeryCold(a_ChunkDesc.GetBiome(x, z)))
			{
				// If double long grass we have to choose what type we should use:
				if (IsBlockAir(a_ChunkDesc.GetBlock({x, y + 1, z})))
				{
					BlockType BlockToPlace;
					if (CanGrassGrow(a_ChunkDesc.GetBiome(x, z)))
					{
						BlockToPlace = (m_Noise.IntNoise2DInt(xx * 100, zz * 100) / 7 % 100) > 25 ? BlockType::TallGrass : BlockType::LargeFern;
					}
					else
					{
						BlockToPlace = BlockType::LargeFern;
					}

					switch (BlockToPlace)
					{
						case BlockType::TallGrass:
						{
							a_ChunkDesc.SetBlock({x, y, z},     Block::TallGrass::TallGrass(Block::TallGrass::Half::Lower));
							a_ChunkDesc.SetBlock({x, y + 1, z}, Block::TallGrass::TallGrass(Block::TallGrass::Half::Upper));
							a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(y + 1));
							break;
						}
						case BlockType::LargeFern:
						{
							if (!CanLargeFernGrow(a_ChunkDesc.GetBiome(x, z)))
							{
								break;
							}
							a_ChunkDesc.SetBlock({x, y, z},     Block::LargeFern::LargeFern(Block::LargeFern::Half::Lower));
							a_ChunkDesc.SetBlock({x, y + 1, z}, Block::LargeFern::LargeFern(Block::LargeFern::Half::Upper));

							a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(y + 1));
							break;
						}
						default: break;
					}
				}
			}
			else
			{
				a_ChunkDesc.SetBlock({x, y, z}, Block::ShortGrass::ShortGrass());
				a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(y));
			}
		}
	}
}





bool cFinishGenTallGrass::CanFernGrow(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biJungle:
		case biJungleEdge:
		case biJungleEdgeM:
		case biJungleHills:
		case biJungleM:
		{
			return true;
		}

		default:
		{
			return CanLargeFernGrow(a_Biome);
		}
	}
}





bool cFinishGenTallGrass::CanLargeFernGrow(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biColdTaiga:
		case biColdTaigaHills:
		case biColdTaigaM:

		case biTaiga:
		case biTaigaHills:
		case biTaigaM:

		case biMegaSpruceTaiga:
		case biMegaSpruceTaigaHills:
		case biMegaTaiga:
		case biMegaTaigaHills:
		{
			return true;
		}

		default:
		{
			return false;
		}
	}
}





int cFinishGenTallGrass::GetBiomeDensity(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biSavanna:
		case biSavannaM:
		case biSavannaPlateau:
		case biSavannaPlateauM:
		case biPlains:
		{
			return 70;
		}

		case biExtremeHillsEdge:
		case biExtremeHillsPlus:
		case biExtremeHills:
		case biExtremeHillsPlusM:
		case biExtremeHillsM:
		case biIceMountains:
		{
			return 3;
		}

		default:
		{
			return 20;
		}
	}
}





bool cFinishGenTallGrass::CanGrassGrow(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biMegaTaiga:
		case biMegaTaigaHills:
		{
			return false;
		}
		default:
		{
			return true;
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenVines

bool cFinishGenVines::IsJungleVariant(EMCSBiome a_Biome)
{
	switch (a_Biome)
	{
		case biJungle:
		case biJungleEdge:
		case biJungleEdgeM:
		case biJungleHills:
		case biJungleM:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}





void cFinishGenVines::GenFinish(cChunkDesc & a_ChunkDesc)
{
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		int xx = x + a_ChunkDesc.GetChunkX() * cChunkDef::Width;
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			int zz = z + a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
			if (!IsJungleVariant(a_ChunkDesc.GetBiome(x, z)))
			{
				// Current biome isn't a jungle
				continue;
			}

			if ((m_Noise.IntNoise2DInt(xx, zz) % 101) < 50)
			{
				continue;
			}

			int Height = a_ChunkDesc.GetHeight(x, z);
			for (int y = Height; y > m_Level; y--)
			{
				if (a_ChunkDesc.GetBlock({x, y, z}).Type() != BlockType::Air)
				{
					// Can't place vines in non-air blocks
					continue;
				}

				if ((m_Noise.IntNoise3DInt(xx, y, zz) % 101) < 50)
				{
					continue;
				}

				std::vector<unsigned char> Places;
				if ((x + 1 < cChunkDef::Width) && cBlockInfo::FullyOccupiesVoxel(a_ChunkDesc.GetBlock({x + 1, y, z})))
				{
					Places.push_back(8);
				}

				if ((x - 1 > 0) && cBlockInfo::FullyOccupiesVoxel(a_ChunkDesc.GetBlock({x - 1, y, z})))
				{
					Places.push_back(2);
				}

				if ((z + 1 < cChunkDef::Width) && cBlockInfo::FullyOccupiesVoxel(a_ChunkDesc.GetBlock({x, y, z + 1})))
				{
					Places.push_back(1);
				}

				if ((z - 1 > 0) && cBlockInfo::FullyOccupiesVoxel(a_ChunkDesc.GetBlock({x, y, z - 1})))
				{
					Places.push_back(4);
				}

				if (Places.size() == 0)
				{
					continue;
				}

				auto Dir = Places[static_cast<size_t>(m_Noise.IntNoise3DInt(xx, y, zz)) % Places.size()];
				switch (Dir)
				{
					case 1:  a_ChunkDesc.SetBlock({x, y, z}, Block::Vine::Vine(false, false, true, false, false)); break;
					case 2:  a_ChunkDesc.SetBlock({x, y, z}, Block::Vine::Vine(false, false, false, false, true)); break;
					case 4:  a_ChunkDesc.SetBlock({x, y, z}, Block::Vine::Vine(false, true, false, false, false)); break;
					case 8:  a_ChunkDesc.SetBlock({x, y, z}, Block::Vine::Vine(true, false, false, false, false)); break;
					default: a_ChunkDesc.SetBlock({x, y, z}, Block::Vine::Vine());
				}
			}
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenSprinkleFoliage:

bool cFinishGenSprinkleFoliage::TryAddCactus(cChunkDesc & a_ChunkDesc, int a_RelX, HEIGHTTYPE & a_RelY, int a_RelZ)
{
	if (!IsDesertVariant(a_ChunkDesc.GetBiome(a_RelX, a_RelZ)))
	{
		return false;
	}

	int CactusHeight = 1 + (m_Noise.IntNoise2DInt(a_RelX, a_RelZ) % m_MaxCactusHeight);

	// We'll be doing comparison with blocks above, so the coords should be 1 block away from chunk top
	if (a_RelY + CactusHeight >= cChunkDef::Height - 1)
	{
		CactusHeight = cChunkDef::Height - a_RelY - 1;
	}

	// We'll be doing comparison to neighbors, so require the coords to be 1 block away from the chunk edges:
	if (
		(a_RelX < 1) || (a_RelX >= cChunkDef::Width - 1) ||
		(a_RelZ < 1) || (a_RelZ >= cChunkDef::Width - 1)
	)
	{
		return false;
	}

	for (int i = 0; i < CactusHeight; i++)
	{
		const bool cactusExists = i != 0;

		const int y = a_RelY + 1;
		if (
			cBlockInfo::IsSolid(a_ChunkDesc.GetBlock({a_RelX + 1, y, a_RelZ})) 	 ||
			cBlockInfo::IsSolid(a_ChunkDesc.GetBlock({a_RelX - 1, y, a_RelZ})) 	 ||
			cBlockInfo::IsSolid(a_ChunkDesc.GetBlock({a_RelX, 	 y, a_RelZ + 1})) ||
			cBlockInfo::IsSolid(a_ChunkDesc.GetBlock({a_RelX, 	 y, a_RelZ - 1}))
		)
		{
			return cactusExists;
		}

		// All conditions are met, we can place a cactus here
		a_ChunkDesc.SetBlock({a_RelX, ++a_RelY, a_RelZ}, Block::Cactus::Cactus());
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
// cFinishGenSprinkleFoliage:

bool cFinishGenSprinkleFoliage::TryAddSugarcane(cChunkDesc & a_ChunkDesc, int a_RelX, HEIGHTTYPE & a_RelY, int a_RelZ)
{
	int SugarcaneHeight = 1 + (m_Noise.IntNoise2DInt(a_RelX, a_RelZ) % m_MaxSugarcaneHeight);

	// Only allow dirt, grass, sand and sugarcane below sugarcane:
	switch (a_ChunkDesc.GetBlock({a_RelX, a_RelY, a_RelZ}).Type())
	{
		case BlockType::Dirt:
		case BlockType::ShortGrass:
		case BlockType::Sand:
		case BlockType::SugarCane:
		{
			break;
		}
		default:
		{
			return false;
		}
	}

	// We'll be doing comparison with blocks above, so the coords should be 1 block away from chunk top
	if (a_RelY + SugarcaneHeight >= cChunkDef::Height - 1)
	{
		SugarcaneHeight = cChunkDef::Height - a_RelY - 1;
	}

	// We'll be doing comparison to neighbors, so require the coords to be 1 block away from the chunk edges:
	if (
		(a_RelX < 1) || (a_RelX >= cChunkDef::Width  - 1) ||
		(a_RelZ < 1) || (a_RelZ >= cChunkDef::Width  - 1)
	)
	{
		return false;
	}

	// Water is required next to the block below the sugarcane (if the block below isn't sugarcane already)
	if (
		!IsWater(a_ChunkDesc.GetBlock({a_RelX - 1, a_RelY, a_RelZ})) &&
		!IsWater(a_ChunkDesc.GetBlock({a_RelX + 1, a_RelY, a_RelZ})) &&
		!IsWater(a_ChunkDesc.GetBlock({a_RelX,     a_RelY, a_RelZ - 1})) &&
		!IsWater(a_ChunkDesc.GetBlock({a_RelX,     a_RelY, a_RelZ + 1})) &&
		a_ChunkDesc.GetBlock({a_RelX, a_RelY, a_RelZ}).Type() != BlockType::SugarCane
	)
	{
		return false;
	}

	for (int i = 0; i < SugarcaneHeight; i++)
	{
		// All conditions met, place a sugarcane here
		a_ChunkDesc.SetBlock({a_RelX, ++a_RelY, a_RelZ}, Block::SugarCane::SugarCane());
	}

	return true;
}





void cFinishGenSprinkleFoliage::GenFinish(cChunkDesc & a_ChunkDesc)
{
	// Generate small foliage (1-block):

	// TODO: Update heightmap with 1-block-tall foliage
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width + z;
		const float zz = static_cast<float>(BlockZ);
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width + x;
			if (((m_Noise.IntNoise2DInt(BlockX, BlockZ) / 8) % 128) < 124)
			{
				continue;
			}
			HEIGHTTYPE Top = a_ChunkDesc.GetHeight(x, z);
			if (Top > 250)
			{
				// Nothing grows above Y=250
				continue;
			}
			if (a_ChunkDesc.GetBlock({x, Top + 1, z}).Type() != BlockType::Air)
			{
				// Space already taken by something else, don't grow here
				// WEIRD, since we're using heightmap, so there should NOT be anything above it
				continue;
			}

			const float xx = static_cast<float>(BlockX);
			float val1 = m_Noise.CubicNoise2D(xx * 0.1f,  zz * 0.1f);
			float val2 = m_Noise.CubicNoise2D(xx * 0.01f, zz * 0.01f);
			switch (a_ChunkDesc.GetBlock({x, Top, z}).Type())
			{
				case BlockType::GrassBlock:
				{
					if (TryAddSugarcane(a_ChunkDesc, x, Top, z))
					{
						// Checks and block placing are handled in the TryAddSugarcane method
					}
					else if ((val1 > 0.5) && (val2 < -0.5))
					{
						a_ChunkDesc.SetBlock({x, ++Top, z}, Block::Pumpkin::Pumpkin());
					}
					break;
				}  // case BlockType::GrassBlock

				case BlockType::Sand:
				{
					if (val1 + val2 > 0.5f)
					{
						if (!TryAddCactus(a_ChunkDesc, x, Top, z))
						{
							TryAddSugarcane(a_ChunkDesc, x, Top, z);
						}
					}
					else
					{
						TryAddSugarcane(a_ChunkDesc, x, Top, z);
					}
					break;
				}
				default: break;
			}  // switch (TopBlock)
			a_ChunkDesc.SetHeight(x, z, Top);
		}  // for x
	}  // for z
}





bool cFinishGenSprinkleFoliage::IsDesertVariant(EMCSBiome a_Biome)
{
	return
	(
		(a_Biome == biDesertHills) ||
		(a_Biome == biDesert) ||
		(a_Biome == biDesertM)
	);
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenSoulsandRims

void cFinishGenSoulsandRims::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int ChunkZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	HEIGHTTYPE MaxHeight = a_ChunkDesc.GetMaxHeight();

	for (int x = 0; x < cChunkDef::Width; x++)
	{
		int xx = ChunkX + x;
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			int zz = ChunkZ + z;

			// Place soulsand rims when netherrack gets thin
			for (int y = 2; y < MaxHeight - 2; y++)
			{
				// The current block is air. Let's bail ut.
				auto Block = a_ChunkDesc.GetBlock({x, y, z});
				if (Block.Type() != BlockType::Netherrack)
				{
					continue;
				}

				if (
					((a_ChunkDesc.GetBlock({x, y + 1, z}).Type() != BlockType::Air) &&
					( a_ChunkDesc.GetBlock({x, y + 2, z}).Type() != BlockType::Air)) ||
					((a_ChunkDesc.GetBlock({x, y - 1, z}).Type() != BlockType::Air) &&
					( a_ChunkDesc.GetBlock({x, y - 2, z}).Type() != BlockType::Air))
				)
				{
					continue;
				}

				NOISE_DATATYPE NoiseX = (static_cast<NOISE_DATATYPE>(xx)) / 32;
				NOISE_DATATYPE NoiseY = (static_cast<NOISE_DATATYPE>(zz)) / 32;
				NOISE_DATATYPE CompBlock = m_Noise.CubicNoise3D(NoiseX, static_cast<float>(y) / 4, NoiseY);
				if (CompBlock < 0)
				{
					a_ChunkDesc.SetBlock({x, y, z}, Block::SoulSand::SoulSand());
				}
			}
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenSnow:

void cFinishGenSnow::GenFinish(cChunkDesc & a_ChunkDesc)
{
	// Add a snow block in snowy biomes onto blocks that can be snowed over
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			HEIGHTTYPE Height = a_ChunkDesc.GetHeight(x, z);
			if (GetSnowStartHeight(a_ChunkDesc.GetBiome(x, z)) > Height)
			{
				// Height isn't high enough for snow to start forming.
				continue;
			}

			if (!cBlockInfo::IsSnowable(a_ChunkDesc.GetBlock({x, Height, z})) || (Height >= cChunkDef::Height - 1))
			{
				// The top block can't be snown over.
				continue;
			}

			a_ChunkDesc.SetBlock({x, Height + 1, z}, Block::Snow::Snow());
			a_ChunkDesc.SetHeight(x, z, Height + 1);
		}  // for x
	}  // for z
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenIce:

void cFinishGenIce::GenFinish(cChunkDesc & a_ChunkDesc)
{
	// Turn surface water into ice in icy biomes
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Height = a_ChunkDesc.GetHeight(x, z);
			if (GetSnowStartHeight(a_ChunkDesc.GetBiome(x, z)) > Height)
			{
				// Height isn't high enough for snow to start forming.
				continue;
			}

			if (a_ChunkDesc.GetBlock({x, Height, z}).Type() == BlockType::Water)
			{
				// The block isn't a water block.
				continue;
			}

			if (cBlockFluidHandler::GetFalloff(a_ChunkDesc.GetBlock({x, Height, z})) != 0)
			{
				// The water block isn't a source block.
				continue;
			}

			a_ChunkDesc.SetBlock({x, Height, z}, Block::Ice::Ice());
		}  // for x
	}  // for z
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenSingleTopBlock:

int cFinishGenSingleTopBlock::GetNumToGen(const cChunkDef::BiomeMap & a_BiomeMap)
{
	int res = 0;
	for (const auto & Biome : a_BiomeMap)
	{
		if (IsAllowedBiome(Biome))
		{
			res++;
		}
	}
	return m_Amount * res / 256;
}





void cFinishGenSingleTopBlock::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int NumToGen = GetNumToGen(a_ChunkDesc.GetBiomeMap());
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();
	for (int i = 0; i < NumToGen; i++)
	{
		int x = (m_Noise.IntNoise3DInt(ChunkX + ChunkZ, ChunkZ, i) / 13) % cChunkDef::Width;
		int z = (m_Noise.IntNoise3DInt(ChunkX - ChunkZ, i, ChunkZ) / 11) % cChunkDef::Width;

		// Place the block at {x, z} if possible:
		EMCSBiome Biome = a_ChunkDesc.GetBiome(x, z);
		if (!IsAllowedBiome(Biome))
		{
			// Incorrect biome
			continue;
		}

		HEIGHTTYPE Height = a_ChunkDesc.GetHeight(x, z);
		if (Height >= cChunkDef::Height - 1)
		{
			// Too high up
			continue;
		}
		if (a_ChunkDesc.GetBlock({x, Height + 1, z}).Type() != BlockType::Air)
		{
			// Not an empty block
			continue;
		}

		auto BlockBelow = a_ChunkDesc.GetBlock({x, Height, z});
		if (!IsAllowedBlockBelow(BlockBelow))
		{
			continue;
		}

		a_ChunkDesc.SetBlock({x, Height + 1, z}, m_Block);
		a_ChunkDesc.SetHeight(x, z, Height + 1);
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenBottomLava:

void cFinishGenBottomLava::GenFinish(cChunkDesc & a_ChunkDesc)
{
	auto BlockTypes = a_ChunkDesc.GetBlocks();
	for (int y = m_Level; y > 0; y--)
	{
		for (int z = 0; z < cChunkDef::Width; z++) for (int x = 0; x < cChunkDef::Width; x++)
		{
			const auto Index = cChunkDef::MakeIndex(x, y, z);
			if (IsBlockAir(BlockTypes[Index]))
			{
				BlockTypes[Index] = Block::Lava::Lava();
			}
		}  // for x, for z
	}  // for y
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenPreSimulator:

cFinishGenPreSimulator::cFinishGenPreSimulator(bool a_PreSimulateFallingBlocks, bool a_PreSimulateWater, bool a_PreSimulateLava) :
	m_PreSimulateFallingBlocks(a_PreSimulateFallingBlocks),
	m_PreSimulateWater(a_PreSimulateWater),
	m_PreSimulateLava(a_PreSimulateLava)
{
	// Nothing needed yet
}





void cFinishGenPreSimulator::GenFinish(cChunkDesc & a_ChunkDesc)
{
	if (m_PreSimulateFallingBlocks)
	{
		CollapseSandGravel(a_ChunkDesc);
	}

	if (m_PreSimulateWater)
	{
		StationarizeFluid(a_ChunkDesc.GetBlocks(), a_ChunkDesc.GetHeightMap(), BlockType::Water);
	}

	if (m_PreSimulateLava)
	{
		StationarizeFluid(a_ChunkDesc.GetBlocks(), a_ChunkDesc.GetHeightMap(), BlockType::Lava);
	}
	// TODO: other operations
}





void cFinishGenPreSimulator::CollapseSandGravel(cChunkDesc & a_ChunkDesc)
{
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int LastY = -1;
			int HeightY = 0;
			for (int y = 0; y < cChunkDef::Height; y++)
			{
				auto Block = a_ChunkDesc.GetBlock({x, y, z});
				switch (Block.Type())
				{
					default:
					{
						// Set the last block onto which stuff can fall to this height:
						LastY = y;
						HeightY = y;
						break;
					}
					case BlockType::Air:
					{
						// Do nothing
						break;
					}
					case BlockType::Fire:
					case BlockType::Water:
					case BlockType::Lava:
					{
						// Do nothing, only remember this height as potentially highest
						HeightY = y;
						break;
					}
					case BlockType::Sand:
					case BlockType::Gravel:
					{
						if (LastY < y - 1)
						{
							a_ChunkDesc.SetBlock({x, LastY + 1, z}, Block);
							a_ChunkDesc.SetBlock({x, y, z}, Block::Air::Air());
						}
						LastY++;
						if (LastY > HeightY)
						{
							HeightY = LastY;
						}
						break;
					}
				}  // switch (GetBlock)
			}  // for y
			a_ChunkDesc.SetHeight(x, z, static_cast<HEIGHTTYPE>(HeightY));
		}  // for x
	}  // for z
}





void cFinishGenPreSimulator::StationarizeFluid(
	cChunkDef::BlockStates a_BlockTypes,    // Block types to read and change
	cChunkDef::HeightMap & a_HeightMap,      // Height map to read
	BlockState a_Block
)
{
	// Turn fluid in the middle to stationary, unless it has air or washable block next to it:
	for (int z = 1; z < cChunkDef::Width - 1; z++)
	{
		for (int x = 1; x < cChunkDef::Width - 1; x++)
		{
			for (int y = cChunkDef::GetHeight(a_HeightMap, x, z); y >= 0; y--)
			{
				auto Block = cChunkDef::GetBlock(a_BlockTypes, {x, y, z});
				if (Block.Type() != a_Block.Type())
				{
					continue;
				}
				static const std::array<Vector3i, 5> Coords =
				{
					Vector3i( 1,  0,  0),
					Vector3i(-1,  0,  0),
					Vector3i( 0,  0,  1),
					Vector3i( 0,  0, -1),
					Vector3i( 0, -1,  0)
				} ;
				BlockState BlockToSet = a_Block;
				for (const auto & Offset : Coords)
				{
					if ((y == 0) && (Offset.y < 0))
					{
						continue;
					}
					auto Neighbor = cChunkDef::GetBlock(a_BlockTypes, x + Offset.x, y + Offset.y, z + Offset.z);
					if (IsBlockAir(Neighbor) || cFluidSimulator::CanWashAway(Neighbor))
					{
						// There is an air / washable neighbor, simulate this block
						switch (Block.Type())
						{
							case BlockType::Water: BlockToSet = Block::Water::Water(0); break;
							case BlockType::Lava:  BlockToSet = Block::Lava::Lava(0); break;
							default: continue;
						}
						break;
					}
				}  // for i - Coords[]
				cChunkDef::SetBlock(a_BlockTypes, x, y, z, BlockToSet);
			}  // for y
		}  // for x
	}  // for z

	// Turn fluid at the chunk edges into non-stationary fluid:
	for (int y = 0; y < cChunkDef::Height; y++)
	{
		for (int i = 0; i < cChunkDef::Width; i++)  // i stands for both x and z here
		{

			if ((cBlockFluidHandler::GetFalloff(cChunkDef::GetBlock(a_BlockTypes, 0, y, i)) == 0))
			{
				cChunkDef::SetBlock(a_BlockTypes, 0, y, i, cBlockFluidHandler::SetFalloff(a_Block, 1));
			}
			if (cBlockFluidHandler::GetFalloff(cChunkDef::GetBlock(a_BlockTypes, i, y, 0)) == 0)
			{
				cChunkDef::SetBlock(a_BlockTypes, i, y, 0, cBlockFluidHandler::SetFalloff(a_Block, 1));
			}
			if (cBlockFluidHandler::GetFalloff(cChunkDef::GetBlock(a_BlockTypes, cChunkDef::Width - 1, y, i)) == 0)
			{
				cChunkDef::SetBlock(a_BlockTypes, cChunkDef::Width - 1, y, i, cBlockFluidHandler::SetFalloff(a_Block, 1));
			}
			if (cBlockFluidHandler::GetFalloff(cChunkDef::GetBlock(a_BlockTypes, i, y, cChunkDef::Width - 1)) == 0)
			{
				cChunkDef::SetBlock(a_BlockTypes, i, y, cChunkDef::Width - 1, cBlockFluidHandler::SetFalloff(a_Block, 1));
			}
		}
	}
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenFluidSprings:

cFinishGenFluidSprings::cFinishGenFluidSprings(int a_Seed, BlockType a_Fluid, cIniFile & a_IniFile, eDimension a_Dimension) :
	m_Noise(a_Seed + static_cast<int>(a_Fluid) * 100),  // Need to take fluid into account, otherwise water and lava springs generate next to each other
	m_HeightDistribution(cChunkDef::Height - 1),
	m_Fluid(a_Fluid)
{
	bool IsWater = (a_Fluid == BlockType::Water);
	AString SectionName = IsWater ? "WaterSprings" : "LavaSprings";
	AString DefaultHeightDistribution;

	int DefaultChance = 0;
	switch (a_Dimension)
	{
		case dimNether:
		{
			DefaultHeightDistribution = IsWater ? DEF_NETHER_WATER_SPRINGS : DEF_NETHER_LAVA_SPRINGS;
			DefaultChance = IsWater ? 0 : 15;
			break;
		}
		case dimOverworld:
		{
			DefaultHeightDistribution = IsWater ? DEF_OVERWORLD_WATER_SPRINGS : DEF_OVERWORLD_LAVA_SPRINGS;
			DefaultChance = IsWater ? 24 : 9;
			break;
		}
		case dimEnd:
		{
			DefaultHeightDistribution = IsWater ? DEF_END_WATER_SPRINGS : DEF_END_LAVA_SPRINGS;
			DefaultChance = 0;
			break;
		}
		default:
		{
			ASSERT(!"Unhandled world dimension");
			break;
		}
	}  // switch (dimension)
	AString HeightDistribution = a_IniFile.GetValueSet(SectionName, "HeightDistribution", DefaultHeightDistribution);
	if (!m_HeightDistribution.SetDefString(HeightDistribution) || (m_HeightDistribution.GetSum() <= 0))
	{
		LOGWARNING("[%sSprings]: HeightDistribution is invalid, using the default of \"%s\".",
			(a_Fluid == BlockType::Water) ? "Water" : "Lava",
			DefaultHeightDistribution.c_str()
		);
		m_HeightDistribution.SetDefString(DefaultHeightDistribution);
	}
	m_Chance = a_IniFile.GetValueSetI(SectionName, "Chance", DefaultChance);
}





void cFinishGenFluidSprings::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int ChanceRnd = (m_Noise.IntNoise3DInt(128 * a_ChunkDesc.GetChunkX(), 512, 256 * a_ChunkDesc.GetChunkZ()) / 13) % 100;
	if (ChanceRnd > m_Chance)
	{
		// Not in this chunk
		return;
	}

	// Get the height at which to try:
	int Height = m_Noise.IntNoise3DInt(128 * a_ChunkDesc.GetChunkX(), 1024, 256 * a_ChunkDesc.GetChunkZ()) / 11;
	Height %= m_HeightDistribution.GetSum();
	Height = m_HeightDistribution.MapValue(Height);

	// Try adding the spring at the height, if unsuccessful, move lower:
	for (int y = Height; y > 1; y--)
	{
		// TODO: randomize the order in which the coords are being checked
		for (int z = 1; z < cChunkDef::Width - 1; z++)
		{
			for (int x = 1; x < cChunkDef::Width - 1; x++)
			{
				switch (a_ChunkDesc.GetBlock({x, y, z}).Type())
				{
					case BlockType::Netherrack:
					case BlockType::Stone:
					{
						if (TryPlaceSpring(a_ChunkDesc, {x, y, z}))
						{
							// Succeeded, bail out
							return;
						}
						break;
					}
					default: break;
				}
			}  // for x
		}  // for y
	}  // for y
}





bool cFinishGenFluidSprings::TryPlaceSpring(cChunkDesc & a_ChunkDesc, Vector3i a_Pos)
{
	// In order to place a spring, it needs exactly one of the XZ neighbors or a below neighbor to be air
	// Also, its neighbor on top of it must be non-air
	if (cBlockAirHandler::IsBlockAir(a_ChunkDesc.GetBlock(a_Pos.addedY(1))))
	{
		return false;
	}

	std::array<Vector3i, 5> Coords =
	{
		Vector3i(-1,  0,  0),
		Vector3i( 1,  0,  0),
		Vector3i( 0, -1,  0),
		Vector3i( 0,  0, -1),
		Vector3i( 0,  0,  1),
	} ;

	int NumAirNeighbors = 0;
	for (const auto & Offset : Coords)
	{
		switch (a_ChunkDesc.GetBlock(a_Pos + Offset).Type())
		{
			case BlockType::Air:
			case BlockType::CaveAir:
			case BlockType::VoidAir:
			{
				NumAirNeighbors += 1;
				if (NumAirNeighbors > 1)
				{
					return false;
				}
				break;
			}
			default: break;
		}
	}
	if (NumAirNeighbors == 0)
	{
		return false;
	}

	// Has exactly one air neighbor, place a spring:
	a_ChunkDesc.SetBlock(a_Pos, m_Fluid);
	return true;
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenPassiveMobs:

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
		if (TrySpawnAnimals(a_ChunkDesc, {PackCenterX, a_ChunkDesc.GetHeight(PackCenterX, PackCenterZ), PackCenterZ}, RandomMob))
		{
			for (int i = 0; i < 3; i++)
			{
				int OffsetX = (m_Noise.IntNoise2DInt(chunkX + chunkZ + i, Tries) / 7) % cChunkDef::Width;
				int OffsetZ = (m_Noise.IntNoise2DInt(chunkX, chunkZ + Tries + i) / 7) % cChunkDef::Width;
				TrySpawnAnimals(a_ChunkDesc, {OffsetX, a_ChunkDesc.GetHeight(OffsetX, OffsetZ), OffsetZ}, RandomMob);
			}
			return;

		}  // if pack center spawn successful
	}  // for tries
}





bool cFinishGenPassiveMobs::TrySpawnAnimals(cChunkDesc & a_ChunkDesc, Vector3i a_RelPos, eMonsterType AnimalToSpawn)
{
	if (!cChunkDef::IsValidRelPos(a_RelPos))
	{
		return false;
	}

	auto BlockAtHead    = a_ChunkDesc.GetBlock(a_RelPos.addedY(1));
	auto BlockAtFeet    = a_ChunkDesc.GetBlock(a_RelPos);
	auto BlockUnderFeet = a_ChunkDesc.GetBlock(a_RelPos.addedY(-1));

	// Check block below (opaque, grass, water), and above (air)
	if ((AnimalToSpawn == mtSquid) && (BlockAtFeet.Type() != BlockType::Water))
	{
		return false;
	}
	if (
		(AnimalToSpawn != mtSquid) &&
		(!cBlockAirHandler::IsBlockAir(BlockAtHead)) &&
		(!cBlockAirHandler::IsBlockAir(BlockAtFeet)) &&
		(!cBlockInfo::IsTransparent(BlockUnderFeet))
	)
	{
		return false;
	}
	if (
		(BlockUnderFeet.Type() != BlockType::GrassBlock) &&
		((AnimalToSpawn == mtWolf) || (AnimalToSpawn == mtRabbit) || (AnimalToSpawn == mtCow) || (AnimalToSpawn == mtSheep) || (AnimalToSpawn == mtChicken) || (AnimalToSpawn == mtPig))
	)
	{
		return false;
	}
	if ((AnimalToSpawn == mtMooshroom) && (BlockUnderFeet.Type() != BlockType::Mycelium))
	{
		return false;
	}

	auto AnimalPos = Vector3d(cChunkDef::RelativeToAbsolute(a_RelPos, a_ChunkDesc.GetChunkCoords())).addedXZ(0.5, 0.5);

	auto NewMob = cMonster::NewMonsterFromType(AnimalToSpawn);
	NewMob->SetHealth(NewMob->GetMaxHealth());
	NewMob->SetPosition(AnimalPos);
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





////////////////////////////////////////////////////////////////////////////////
// cFinishGenOres:

void cFinishGenOres::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int seq = 1;

	// Generate the ores from the ore list.
	for (const auto & ore: m_OreInfos)
	{
		GenerateOre(
			a_ChunkDesc,
			ore.m_OreBlock,
			ore.m_MaxHeight, ore.m_NumNests, ore.m_NestSize,
			seq
		);
		seq++;
	}
}





const cFinishGenOres::OreInfos & cFinishGenOres::DefaultOverworldOres(void)
{
	static OreInfos res
	{
		// OreType,              MaxHeight, NumNests, NestSize
		{BlockType::CoalOre,      127,       20,       16},
		{BlockType::IronOre,       64,       20,        8},
		{BlockType::GoldOre,       32,        2,        8},
		{BlockType::RedstoneOre,   16,        8,        7},
		{BlockType::DiamondOre,    15,        1,        7},
		{BlockType::LapisOre,      30,        1,        6},
		{BlockType::EmeraldOre,    32,       11,        1},
		{BlockType::InfestedStone, 64,        7,        9},
	};
	return res;
}





const cFinishGenOres::OreInfos & cFinishGenOres::DefaultNetherOres(void)
{
	static OreInfos res
	{
		// OreType,                  MaxHeight, NumNests, NestSize
		{BlockType::NetherQuartzOre, 127,       20,       8},
	};
	return res;
}





const cFinishGenOres::OreInfos & cFinishGenOres::DefaultNaturalPatches(void)
{
	static OreInfos res
	{
		// OreType,           MaxHeight, NumNests, NestSize
		{BlockType::Dirt,     127,       20,       32},
		{BlockType::Gravel,   127,       10,       32},
		{BlockType::Granite,  127,       20,       32},
		{BlockType::Diorite,  127,       20,       32},
		{BlockType::Andesite, 127,       20,       32},
	};
	return res;
}





cFinishGenOres::OreInfos cFinishGenOres::OreInfosFromString(const AString & a_OreInfosString)
{
	// The string is expected to be formatted as "<OreInfo1> | <OreInfo2> | <OreInfo3> | ..."
	// Each OreInfo is expected to be formatted as "<OreType> : <OreMeta> : <MaxHeight> : <NumNests> : <NestSize>"

	OreInfos res;
	auto ores = StringSplitAndTrim(a_OreInfosString, "|");
	for (const auto & ore: ores)
	{
		auto parts = StringSplitAndTrim(ore, ":");
		if (parts.size() != 5)
		{
			LOGWARNING("Cannot parse ore information from string, not enough OreInfo members (exp 5, got %d). Offending item: \"%s\".",
				static_cast<unsigned>(parts.size()), ore.c_str()
			);
			continue;
		}
		auto oreType = BlockStringToType(parts[0]);
		if (oreType < 0)
		{
			LOGWARNING("Cannot parse ore information from string, invalid OreType: \"%s\".", parts[0].c_str());
			continue;
		}
		unsigned char oreMeta;
		int maxHeight, numNests, nestSize;
		if (
			!StringToInteger(parts[1], oreMeta) ||
			!StringToInteger(parts[2], maxHeight) ||
			!StringToInteger(parts[3], numNests) ||
			!StringToInteger(parts[4], nestSize)
		)
		{
			LOGWARNING("Cannot parse ore information from string, invalid number in OreInfo \"%s\".", ore.c_str());
			continue;
		}
		res.emplace_back(PaletteUpgrade::FromBlock(static_cast<unsigned char>(oreType), static_cast<unsigned char>(oreMeta)).Type(), maxHeight, numNests, nestSize);
	}  // for i - split[]
	return res;
}





AString cFinishGenOres::OreInfosToString(const cFinishGenOres::OreInfos & a_OreInfos)
{
	AString res;
	for (const auto & Ore: a_OreInfos)
	{
		if (!res.empty())
		{
			res.append(" | ");
		}

		auto NumericBlock = PaletteUpgrade::ToBlock(Ore.m_OreBlock);
		res = fmt::format(FMT_STRING("%s:%d:%d:%d:%d"), NamespaceSerializer::From(Ore.m_OreBlock), NumericBlock.second, Ore.m_MaxHeight, Ore.m_NumNests, Ore.m_NestSize);
	}  // for ore - a_OreInfos[]
	return res;
}





void cFinishGenOres::SetSeed(int a_Seed)
{
	m_Noise.SetSeed(a_Seed);
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenOreNests:

void cFinishGenOreNests::GenerateOre(
	cChunkDesc & a_ChunkDesc,
	BlockState a_OreBlock,
	int a_MaxHeight, int a_NumNests, int a_NestSize,
	int a_Seq
)
{
	// This function generates several "nests" of ore, each nest consisting of number of ore blocks relatively adjacent to each other.
	// It does so by making a random XYZ walk and adding ore along the way in cuboids of different (random) sizes
	// Only "terraformable" blocks get replaced with ore, all other blocks stay (so the nest can actually be smaller than specified).

	// If there is an attempt to generate Emerald ores in a chunk with no mountains biome abort
	// There are just four points sampled to avoid searching all 16 * 16 blocks:
	if (a_OreBlock.Type() == BlockType::EmeraldOre)
	{
		const auto BiomeSampleOne =   a_ChunkDesc.GetBiome( 4,  4);
		const auto BiomeSampleTwo =   a_ChunkDesc.GetBiome( 4, 12);
		const auto BiomeSampleThree = a_ChunkDesc.GetBiome(12,  4);
		const auto BiomeSampleFour =  a_ChunkDesc.GetBiome(12, 12);

		if (
			!IsBiomeMountain(BiomeSampleOne) &&
			!IsBiomeMountain(BiomeSampleTwo) &&
			!IsBiomeMountain(BiomeSampleThree) &&
			!IsBiomeMountain(BiomeSampleFour)
		)
		{
			return;
		}
	}

	// Gold ores are generated more often in Mesa-Type-Biomes:
	// https://minecraft.gamepedia.com/Gold_Ore
	if (a_OreBlock.Type() == BlockType::GoldOre)
	{
		const auto BiomeSampleOne =   a_ChunkDesc.GetBiome( 4,  4);
		const auto BiomeSampleTwo =   a_ChunkDesc.GetBiome( 4, 12);
		const auto BiomeSampleThree = a_ChunkDesc.GetBiome(12,  4);
		const auto BiomeSampleFour =  a_ChunkDesc.GetBiome(12, 12);

		if (
			IsBiomeMesa(BiomeSampleOne) ||
			IsBiomeMesa(BiomeSampleTwo) ||
			IsBiomeMesa(BiomeSampleThree) ||
			IsBiomeMesa(BiomeSampleFour)
		)
		{
			a_MaxHeight = 76;
			a_NumNests = 22;  // 2 times default + 20 times mesa bonus
		}
	}

	if (a_OreBlock.Type() == BlockType::InfestedStone)
	{
		const auto BiomeSampleOne =   a_ChunkDesc.GetBiome( 4,  4);
		const auto BiomeSampleTwo =   a_ChunkDesc.GetBiome( 4, 12);
		const auto BiomeSampleThree = a_ChunkDesc.GetBiome(12,  4);
		const auto BiomeSampleFour =  a_ChunkDesc.GetBiome(12, 12);

		if (
			!IsBiomeMountain(BiomeSampleOne) &&
			!IsBiomeMountain(BiomeSampleTwo) &&
			!IsBiomeMountain(BiomeSampleThree) &&
			!IsBiomeMountain(BiomeSampleFour)
		)
		{
			return;
		}
	}

	auto chunkX = a_ChunkDesc.GetChunkX();
	auto chunkZ = a_ChunkDesc.GetChunkZ();
	auto Blocks = a_ChunkDesc.GetBlocks();
	for (int i = 0; i < a_NumNests; i++)
	{
		int nestRnd = m_Noise.IntNoise3DInt(chunkX + i, a_Seq, chunkZ + 64 * i) / 8;
		int BaseX = nestRnd % cChunkDef::Width;
		nestRnd /= cChunkDef::Width;
		int BaseZ = nestRnd % cChunkDef::Width;
		nestRnd /= cChunkDef::Width;
		int BaseY = nestRnd % a_MaxHeight;
		nestRnd /= a_MaxHeight;
		int NestSize = a_NestSize + (nestRnd % (std::max(a_NestSize, 4) / 4));  // The actual nest size may be up to 1 / 4 larger
		int Num = 0;
		while (Num < NestSize)
		{
			// Put a cuboid around [BaseX, BaseY, BaseZ]
			int rnd = m_Noise.IntNoise3DInt(chunkX + 64 * i, 2 * a_Seq + Num, chunkZ + 32 * i) / 8;
			int xsize = rnd % 2;
			int ysize = (rnd / 4) % 2;
			int zsize = (rnd / 16) % 2;
			rnd >>= 8;
			for (int x = xsize; x >= 0; --x)
			{
				int BlockX = BaseX + x;
				if (!cChunkDef::IsValidWidth(BlockX))
				{
					Num++;  // So that the cycle finishes even if the base coords wander away from the chunk
					continue;
				}
				for (int y = ysize; y >= 0; --y)
				{
					int BlockY = BaseY + y;
					if (!cChunkDef::IsValidHeight({BlockX, BlockY, BaseZ}))
					{
						Num++;  // So that the cycle finishes even if the base coords wander away from the chunk
						continue;
					}
					for (int z = zsize; z >= 0; --z)
					{
						int BlockZ = BaseZ + z;
						if (!cChunkDef::IsValidWidth(BlockZ))
						{
							Num++;  // So that the cycle finishes even if the base coords wander away from the chunk
							continue;
						}

						const auto Index = cChunkDef::MakeIndex(BlockX, BlockY, BlockZ);
						auto Block = Blocks[Index];
						if ((Block.Type() == BlockType::Stone) || (Block.Type() == BlockType::Netherrack))
						{
							// TODO: Check if this actually creates the ore
							Blocks[Index] = a_OreBlock;
						}
						Num++;
					}  // for z
				}  // for y
			}  // for x

			// Move the base to a neighbor voxel
			switch (rnd % 4)
			{
				case 0: BaseX--; break;
				case 1: BaseX++; break;
			}
			switch ((rnd >> 3) % 4)
			{
				case 0: BaseY--; break;
				case 1: BaseY++; break;
			}
			switch ((rnd >> 6) % 4)
			{
				case 0: BaseZ--; break;
				case 1: BaseZ++; break;
			}
		}  // while (Num < NumBlocks)
	}  // for i - NumNests
}





////////////////////////////////////////////////////////////////////////////////
// cFinishGenOrePockets:

void cFinishGenOrePockets::Initialize(cIniFile & a_IniFile, const AString & a_GenName)
{
	// Read the OreInfos configuration:
	auto valueName = a_GenName + "Blocks";
	auto pocketCfg = a_IniFile.GetValue("Generator", valueName, "");
	if (pocketCfg.empty())
	{
		// There's no config currently stored in the INI file. Store the defaults as the config:
		a_IniFile.SetValue("Generator", valueName, OreInfosToString(m_OreInfos));
	}
	else
	{
		m_OreInfos = OreInfosFromString(pocketCfg);
	}

	// Read the optional seed configuration (but do not store the default):
	valueName = a_GenName + "Seed";
	SetSeed(a_IniFile.GetValueI("Generator", valueName, m_Noise.GetSeed()));
}





void cFinishGenOrePockets::GenerateOre(
	cChunkDesc & a_ChunkDesc,
	BlockState a_OreBlock,
	int a_MaxHeight, int a_NumNests, int a_NestSize,
	int a_Seq
)
{
	// This function generates several "pockets" of the specified ore
	// Each chunk can contain only pockets that are generated for that chunk, or for its XM / ZM neighbors.

	// Generate for the 3 neighbors in the XP / ZP direction as well, so that pockets crossing the boundaries are accounted for as well:
	int chunkZ = a_ChunkDesc.GetChunkZ();
	int chunkX = a_ChunkDesc.GetChunkX();
	ImprintChunkOrePockets(chunkX - 1, chunkZ - 1, a_ChunkDesc, a_OreBlock, a_MaxHeight, a_NumNests, a_NestSize, a_Seq);
	ImprintChunkOrePockets(chunkX - 1, chunkZ,     a_ChunkDesc, a_OreBlock, a_MaxHeight, a_NumNests, a_NestSize, a_Seq);
	ImprintChunkOrePockets(chunkX,     chunkZ - 1, a_ChunkDesc, a_OreBlock, a_MaxHeight, a_NumNests, a_NestSize, a_Seq);
	ImprintChunkOrePockets(chunkX,     chunkZ,     a_ChunkDesc, a_OreBlock, a_MaxHeight, a_NumNests, a_NestSize, a_Seq);
}





void cFinishGenOrePockets::ImprintChunkOrePockets(
	int a_ChunkX, int a_ChunkZ,
	cChunkDesc & a_ChunkDesc,
	BlockState a_OreBlock,
	int a_MaxHeight, int a_NumNests, int a_NestSize,
	int a_Seq
)
{
	// Pick a starting coord for each nest:
	int baseBlockX = a_ChunkX * cChunkDef::Width;
	int baseBlockZ = a_ChunkZ * cChunkDef::Width;
	for (int i = 0; i < a_NumNests; i++)
	{
		int nestRnd = m_Noise.IntNoise3DInt(a_ChunkX + i, a_Seq, a_ChunkZ + 64 * i) / 7;
		int baseX = (nestRnd % cChunkDef::Width) + baseBlockX;
		nestRnd /= cChunkDef::Width;
		int baseZ = (nestRnd % cChunkDef::Width) + baseBlockZ;
		nestRnd /= cChunkDef::Width;
		int baseY = nestRnd % a_MaxHeight;
		nestRnd /= a_MaxHeight;
		ImprintPocket(
			a_ChunkDesc,
			baseX, baseY, baseZ,
			a_NestSize, i + 200 * a_Seq,
			a_OreBlock
		);
	}  // for i - NumNests
}





void cFinishGenOrePockets::ImprintPocket(
	cChunkDesc & a_ChunkDesc,
	int a_MinPocketX, int a_PocketY, int a_MinPocketZ,
	int a_NestSize, int a_Seq,
	BlockState a_OreBlock
)
{
	// A line segment in a random direction is chosen. Then, several spheres are formed along this line segment,
	// with their diameters diminishing towards the line ends (one half of a sinusoid)

	double x1 = static_cast<double>(a_MinPocketX) + 0.5;
	double y1 = static_cast<double>(a_PocketY)    + 0.5;
	double z1 = static_cast<double>(a_MinPocketZ) + 0.5;
	int rnd = m_Noise.IntNoise2DInt(a_MinPocketX + 7 * a_Seq, a_MinPocketZ + a_PocketY * 11) / 7;
	double angle = static_cast<double>(rnd % 256) / (256.0 * M_PI / 2.0);  // range [0 .. pi / 2]
	rnd /= 256;
	double length = static_cast<double>(a_NestSize) / 4.0;
	double x2 = x1 + sin(angle) * length;  // Always larger than x1
	double z2 = z1 + cos(angle) * length;  // Always larger than z1
	double y2 = y1 + static_cast<double>((rnd % 3) - 1);  // Up to 1 block away from y1

	// Iterate over the line segment in a total of a_NestSize steps:
	double stepX = (x2 - x1) / static_cast<double>(a_NestSize);
	double stepY = (y2 - y1) / static_cast<double>(a_NestSize);
	double stepZ = (z2 - z1) / static_cast<double>(a_NestSize);
	double stepR = M_PI / static_cast<double>(a_NestSize);
	double size = static_cast<double>(a_NestSize) / 16.0;
	for (int i = 0; i < a_NestSize; ++i)
	{
		double iDbl = static_cast<double>(i);
		double sphereX = x1 + stepX * iDbl;
		double sphereY = y1 + stepY * iDbl;
		double sphereZ = z1 + stepZ * iDbl;
		double radius = (sin(stepR * iDbl) + 1.0) * size + 1.0;
		ImprintSphere(a_ChunkDesc, sphereX, sphereY, sphereZ, radius, a_OreBlock);
	}  // for i
}





void cFinishGenOrePockets::ImprintSphere(
	cChunkDesc & a_ChunkDesc,
	double a_SphereX, double a_SphereY, double a_SphereZ, double a_Radius,
	BlockState a_OreBlock
)
{
	// Get the sphere's bounding box, unioned with the chunk's bounding box (possibly empty):
	int baseX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int baseZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	int minX = std::max(FloorC(a_SphereX - a_Radius), baseX);
	int minY = std::max(FloorC(a_SphereY - a_Radius), 0);
	int minZ = std::max(FloorC(a_SphereZ - a_Radius), baseZ);
	int maxX = std::min(CeilC(a_SphereX + a_Radius), baseX + cChunkDef::Width - 1);
	int maxY = std::min(CeilC(a_SphereY + a_Radius), cChunkDef::Height - 1);
	int maxZ = std::min(CeilC(a_SphereZ + a_Radius), baseZ + cChunkDef::Width - 1);

	/*
	// DEBUG:
	int blockX = FloorC(a_SphereX);
	int blockY = FloorC(a_SphereY);
	int blockZ = FloorC(a_SphereZ);
	if (
		(blockX >= baseX) && (blockX < baseX + cChunkDef::Width) &&
		(blockY >= 0) && (blockY < cChunkDef::Height) &&
		(blockZ >= baseZ) && (blockZ < baseZ + cChunkDef::Width)
	)
	{
		// FLOGD("Imprinting a sphere center at {0}", Vector3i{blockX, blockY, blockZ});
		a_ChunkDesc.SetBlockTypeMeta(blockX - baseX, blockY, blockZ - baseZ, a_OreType, a_OreMeta);
	}
	return;
	//*/

	// Imprint the parts of the sphere intersecting the chunk:
	double radiusSq = a_Radius * a_Radius / 4.0;
	for (int y = minY; y <= maxY; y++)
	{
		double relY = static_cast<double>(y) + 0.5 - a_SphereY;
		double relYSq = relY * relY;
		if (relYSq > radiusSq)
		{
			// outside the sphere, bail out
			continue;
		}
		for (int z = minZ; z <= maxZ; z++)
		{
			double relZ = static_cast<double>(z) + 0.5 - a_SphereZ;
			double relZSq = relZ * relZ;
			if (relZSq + relYSq > radiusSq)
			{
				// outside the sphere, bail out
				continue;
			}
			for (int x = minX; x <= maxX; x++)
			{
				double relX = static_cast<double>(x) + 0.5 - a_SphereX;
				double relXSq = relX * relX;
				if (relZSq + relYSq + relXSq > radiusSq)
				{
					// outside the sphere, bail out
					continue;
				}
				int bX = x - baseX;
				int bZ = z - baseZ;
				auto Block = a_ChunkDesc.GetBlock({bX, y, bZ});
				if ((Block.Type() == BlockType::Stone) || (Block.Type() == BlockType::Netherrack))
				{
					a_ChunkDesc.SetBlock({bX, y, bZ}, a_OreBlock);
				}
			}  // for x
		}  // for z
	}  // for y
}





cFinishGenForestRocks::cFinishGenForestRocks(int a_Seed, cIniFile & a_IniFile) : m_Noise(a_Seed)
{
}





void cFinishGenForestRocks::GenFinish(cChunkDesc & a_ChunkDesc)
{
	// Choose random position in chunk and place boulder around it
	auto Pos = Vector3i(
		m_Noise.IntNoise2DInt(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ()) % cChunkDef::Width,
		0,
		m_Noise.IntNoise2DInt(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ()) % cChunkDef::Width
		);
	Pos.y = a_ChunkDesc.GetHeight(Pos.x, Pos.z) % cChunkDef::Height;

	auto Biome = a_ChunkDesc.GetBiome(Pos.x, Pos.z);
	if ((Biome != biMegaTaiga) && (Biome != biMegaTaigaHills))
	{
		return;
	}

	// Determines the size of the boulder
	const int TwoLimit = 70;
	const int ThreeLimit = 90;

	auto RadiusChance = m_Noise.IntNoise2DInt(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ()) % 100;
	int Radius = 1;
	if (RadiusChance > TwoLimit && RadiusChance <= ThreeLimit)
	{
		Radius = 2;
	}
	else if (RadiusChance > ThreeLimit)
	{
		Radius = 3;
	}

	Pos.x = Clamp(Pos.x, Radius, cChunkDef::Width - Radius - 1);
	Pos.z = Clamp(Pos.z, Radius, cChunkDef::Width - Radius - 1);

	auto StartBlock = a_ChunkDesc.GetBlock(Pos);
	while (!((StartBlock.Type() == BlockType::Dirt) || (StartBlock.Type() == BlockType::GrassBlock)))
	{
		Pos.y -= 1;
		if (!cChunkDef::IsValidRelPos(Pos.addedY(-Radius)))
		{
			return;
		}
		StartBlock = a_ChunkDesc.GetBlock(Pos);
	}


	Pos.y -= Radius - 1;
	// Pos.y = Clamp(Pos.y - m_Noise.IntNoise2DInt(a_ChunkDesc.GetChunkX(), a_ChunkDesc.GetChunkZ()) % Radius + 1, 0, cChunkDef::Height);

	for (int x = -Radius; x <= Radius; x++)
	{
		for (int y = -Radius; y <= Radius; y++)
		{
			for (int z = -Radius; z <= Radius; z++)
			{
				if (!cChunkDef::IsValidRelPos({ Pos.x + x, Pos.y + y, Pos.z + z }))
				{
					continue;
				}

				if (Vector3d(x, y, z).SqrLength() > Radius * Radius + 1)
				{
					continue;
				}

				a_ChunkDesc.SetBlock({Pos.x + x, Pos.y + y, Pos.z + z}, Block::MossyCobblestone::MossyCobblestone());
			}
		}
	}
}
