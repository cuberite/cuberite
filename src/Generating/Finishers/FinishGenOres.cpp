
// FinishGenOres.cpp

#include "Globals.h"

#include "FinishGenOres.h"

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
			ore.m_BlockType, ore.m_BlockMeta,
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
			// OreType,              OreMeta, MaxHeight, NumNests, NestSize
			{E_BLOCK_COAL_ORE,       0,       127,       20,       16},
			{E_BLOCK_IRON_ORE,       0,        64,       20,        8},
			{E_BLOCK_GOLD_ORE,       0,        32,        2,        8},
			{E_BLOCK_REDSTONE_ORE,   0,        16,        8,        7},
			{E_BLOCK_DIAMOND_ORE,    0,        15,        1,        7},
			{E_BLOCK_LAPIS_ORE,      0,        30,        1,        6},
			{E_BLOCK_EMERALD_ORE,    0,        32,       11,        1},
			{E_BLOCK_SILVERFISH_EGG, 0,        64,        7,        9},
		};
	return res;
}





const cFinishGenOres::OreInfos & cFinishGenOres::DefaultNetherOres(void)
{
	static OreInfos res
		{
			// OreType,                 OreMeta, MaxHeight, NumNests, NestSize
			{E_BLOCK_NETHER_QUARTZ_ORE, 0,       127,       20,       8},
		};
	return res;
}





const cFinishGenOres::OreInfos & cFinishGenOres::DefaultNaturalPatches(void)
{
	static OreInfos res
		{
			// OreType,      OreMeta,               MaxHeight, NumNests, NestSize
			{E_BLOCK_DIRT,   0,                     127,       20,       32},
			{E_BLOCK_GRAVEL, 0,                     127,       10,       32},
			{E_BLOCK_STONE,  E_META_STONE_GRANITE,  127,       20,       32},
			{E_BLOCK_STONE,  E_META_STONE_DIORITE,  127,       20,       32},
			{E_BLOCK_STONE,  E_META_STONE_ANDESITE, 127,       20,       32},
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
			LOGWARNING("Cannot parse ore information from string, not enough OreInfo members (exp 5, got %d). Offending item: \"%s\".", static_cast<unsigned>(parts.size()), ore.c_str());
			continue;
		}
		auto oreType = BlockStringToType(parts[0]);
		if (oreType < 0)
		{
			LOGWARNING("Cannot parse ore information from string, invalid OreType: \"%s\".", parts[0].c_str());
			continue;
		}
		NIBBLETYPE oreMeta;
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
		res.emplace_back(oreType, oreMeta, maxHeight, numNests, nestSize);
	}  // for i - split[]
	return res;
}





AString cFinishGenOres::OreInfosToString(const cFinishGenOres::OreInfos & a_OreInfos)
{
	AString res;
	for (const auto & ore: a_OreInfos)
	{
		if (!res.empty())
		{
			res.append(" | ");
		}
		AppendPrintf(res, "%s:%d:%d:%d:%d",
			ItemTypeToString(ore.m_BlockType).c_str(), ore.m_BlockMeta,
			ore.m_MaxHeight, ore.m_NumNests, ore.m_NestSize
		);
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
	BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta,
	int a_MaxHeight, int a_NumNests, int a_NestSize,
	int a_Seq
)
{
	// This function generates several "nests" of ore, each nest consisting of number of ore blocks relatively adjacent to each other.
	// It does so by making a random XYZ walk and adding ore along the way in cuboids of different (random) sizes
	// Only "terraformable" blocks get replaced with ore, all other blocks stay (so the nest can actually be smaller than specified).

	// If there is an attempt to generate Emerald ores in a chunk with no mountains biome abort
	// There are just four points sampled to avoid searching all 16 * 16 blocks:
	if (a_OreType == E_BLOCK_EMERALD_ORE)
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
	if (a_OreType == E_BLOCK_GOLD_ORE)
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

	if (a_OreType == E_BLOCK_SILVERFISH_EGG)
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
	auto & blockTypes = a_ChunkDesc.GetBlockTypes();
	auto & blockMetas = a_ChunkDesc.GetBlockMetasUncompressed();
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
					if (!cChunkDef::IsValidHeight(BlockY))
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

						int Index = cChunkDef::MakeIndexNoCheck(BlockX, BlockY, BlockZ);
						auto blockType = blockTypes[Index];
						if ((blockType == E_BLOCK_STONE) || (blockType == E_BLOCK_NETHERRACK))
						{
							blockTypes[Index] = a_OreType;
							blockMetas[Index] = a_OreMeta;
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
	BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta,
	int a_MaxHeight, int a_NumNests, int a_NestSize,
	int a_Seq
)
{
	// This function generates several "pockets" of the specified ore
	// Each chunk can contain only pockets that are generated for that chunk, or for its XM / ZM neighbors.

	// Generate for the 3 neighbors in the XP / ZP direction as well, so that pockets crossing the boundaries are accounted for as well:
	int chunkZ = a_ChunkDesc.GetChunkZ();
	int chunkX = a_ChunkDesc.GetChunkX();
	imprintChunkOrePockets(chunkX - 1, chunkZ - 1, a_ChunkDesc, a_OreType, a_OreMeta, a_MaxHeight, a_NumNests, a_NestSize, a_Seq);
	imprintChunkOrePockets(chunkX - 1, chunkZ,     a_ChunkDesc, a_OreType, a_OreMeta, a_MaxHeight, a_NumNests, a_NestSize, a_Seq);
	imprintChunkOrePockets(chunkX,     chunkZ - 1, a_ChunkDesc, a_OreType, a_OreMeta, a_MaxHeight, a_NumNests, a_NestSize, a_Seq);
	imprintChunkOrePockets(chunkX,     chunkZ,     a_ChunkDesc, a_OreType, a_OreMeta, a_MaxHeight, a_NumNests, a_NestSize, a_Seq);
}





void cFinishGenOrePockets::imprintChunkOrePockets(
	int a_ChunkX, int a_ChunkZ,
	cChunkDesc & a_ChunkDesc,
	BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta,
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
		imprintPocket(
			a_ChunkDesc,
			baseX, baseY, baseZ,
			a_NestSize, i + 200 * a_Seq,
			a_OreType, a_OreMeta
		);
	}  // for i - NumNests
}





void cFinishGenOrePockets::imprintPocket(
	cChunkDesc & a_ChunkDesc,
	int a_MinPocketX, int a_PocketY, int a_MinPocketZ,
	int a_NestSize, int a_Seq,
	BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta
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
		imprintSphere(a_ChunkDesc, sphereX, sphereY, sphereZ, radius, a_OreType, a_OreMeta);
	}  // for i
}





void cFinishGenOrePockets::imprintSphere(
	cChunkDesc & a_ChunkDesc,
	double a_SphereX, double a_SphereY, double a_SphereZ, double a_Radius,
	BLOCKTYPE a_OreType, NIBBLETYPE a_OreMeta
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
				auto blockType = a_ChunkDesc.GetBlockType(bX, y, bZ);
				if ((blockType == E_BLOCK_STONE) || (blockType == E_BLOCK_NETHERRACK))
				{
					a_ChunkDesc.SetBlockTypeMeta(bX, y, bZ, a_OreType, a_OreMeta);
				}
			}  // for x
		}  // for z
	}  // for y
}
