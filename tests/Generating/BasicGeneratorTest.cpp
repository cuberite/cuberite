#include "Globals.h"
#include "Generating/ChunkGenerator.h"
#include "Generating/ChunkDesc.h"
#include "../TestHelpers.h"
#include "IniFile.h"
#include "mbedTLS++/Sha1Checksum.h"
#include "BlockInfo.h"





/** Checks that the chunk's heightmap corresponds to the chunk contents. */
static void verifyChunkDescHeightmap(const cChunkDesc & a_ChunkDesc)
{
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int y = cChunkDef::Height - 1; y > 0; y--)
			{
				auto Block = a_ChunkDesc.GetBlock({x, y, z});
				if (!IsBlockAir(Block))
				{
					int Height = a_ChunkDesc.GetHeight(x, z);
					TEST_EQUAL_MSG(Height, y, fmt::format(FMT_STRING("Chunk height at <{}, {}>: exp {}, got {}"), x, z, y, Height));
					break;
				}
			}  // for y
		}  // for z
	}  // for x
}





static AString chunkSHA1(const cChunkDesc & a_ChunkDesc)
{
	cSha1Checksum cs;
	cs.Update(reinterpret_cast<const Byte *>(a_ChunkDesc.GetBlocks()), cChunkDef::Width * cChunkDef::Width * cChunkDef::Height);
	cSha1Checksum::Checksum digest;
	cs.Finalize(digest);
	AString res;
	cSha1Checksum::DigestToJava(digest, res);
	return res;
}





/** Prints out the entire column from the chunk, one block type per line. */
static void printChunkColumn(const cChunkDesc & a_ChunkDesc, int a_X, int a_Z)
{
	auto PrevBlock = a_ChunkDesc.GetBlock({a_X, cChunkDef::Height - 1, a_Z});
	int count = 1;
	LOG("Column {%d, %d}:", a_X, a_Z);
	LOG("Yfrom\tYto\tcnt\ttype\ttypeStr");
	for (int y = cChunkDef::Height - 2; y >= 0; --y)
	{
		auto BlockType = a_ChunkDesc.GetBlock({a_X, y, a_Z});
		if (BlockType != PrevBlock)
		{
			FLOG("{}\t{}\t{}\t{}\t{}", y + 1, y + count, count, PrevBlock, PrevBlock);
			PrevBlock = BlockType;
			count = 1;
		}
		else
		{
			count += 1;
		}
	}
	FLOG("{}\t{}\t{}\t{}", 0, count, PrevBlock, PrevBlock);
}





/** Tests that the default Overworld generator generates a few chunks that have the Overworld look:
- bedrock at their bottom
- a valid overworld block at their height's top
- air at their top, unless the height at that point is equal to full chunk height.
- valid heightmap
Multiple chunks are tested. */
static void testGenerateOverworld(cChunkGenerator & aDefaultOverworldGen)
{
	LOG("Testing Overworld generator...");

	for (int chunkX = 0; chunkX < 50; ++chunkX)
	{
		// Generate a chunk:
		cChunkDesc chd({chunkX, 0});
		aDefaultOverworldGen.Generate(chd);
		verifyChunkDescHeightmap(chd);

		// Check that it has bedrock at the bottom:
		for (int x = 0; x < cChunkDef::Width; ++x)
		{
			for (int z = 0; z < cChunkDef::Width; ++z)
			{
				TEST_EQUAL_MSG(chd.GetBlock({x, 0, z}).Type(), BlockType::Bedrock, fmt::format(FMT_STRING("Bedrock floor at {{{}, {}, {}}}"), x, 0, z));
			}
		}

		// Check that the blocks on the top are valid Overworld blocks:
		static std::set<BlockType> validOverworldBlockTypes =
		{
			BlockType::Stone,
			BlockType::GrassBlock,
			BlockType::Water,
			BlockType::Lava,
			BlockType::Sand,
			BlockType::Sand,
			BlockType::Gravel,
			BlockType::AcaciaLeaves,
			BlockType::BirchLeaves,
			BlockType::DarkOakLeaves,
			BlockType::JungleLeaves,
			BlockType::OakLeaves,
			BlockType::SpruceLeaves,
		};

		for (int x = 0; x < cChunkDef::Width; ++x)
		{
			for (int z = 0; z < cChunkDef::Width; ++z)
			{
				auto y = chd.GetHeight(x, z);
				auto Block = chd.GetBlock({x, y, z});
				TEST_EQUAL_MSG(validOverworldBlockTypes.count(Block.Type()), 1,
					fmt::format(FMT_STRING("Block at {{{}, {}, {}}}: {}"), x, y, z, Block.Type())
				);
				if (y < cChunkDef::Height - 1)
				{
					TEST_EQUAL_MSG(chd.GetBlock({x, cChunkDef::Height - 1, z}), BlockType::Air,
						fmt::format(FMT_STRING("Air at {{{}, {}, {}}}"), x, cChunkDef::Height - 1, z)
					);
				}
			}
		}
	}
}





/** Tests that the default Nether generator generates a chunk that has the Nether look:
- bedrock at the bottom
- bedrock at the height's top
- at least one Nether-native block in each column
- valid heightmap
Multiple chunks are tested. */
static void testGenerateNether(cChunkGenerator & aDefaultNetherGen)
{
	LOG("Testing Nether generator...");

	for (int chunkX = 0; chunkX < 50; ++chunkX)
	{
		// Generate a chunk:
		cChunkDesc chd({chunkX, 0});
		aDefaultNetherGen.Generate(chd);
		verifyChunkDescHeightmap(chd);

		// Check that the biome is Nether everywhere:
		for (int x = 0; x < cChunkDef::Width; ++x)
		{
			for (int z = 0; z < cChunkDef::Width; ++z)
			{
				TEST_EQUAL_MSG(chd.GetBiome(x, z), biNether, fmt::format(FMT_STRING("Nether biome at <{}, {}>"), x, z));
			}
		}

		// Check that it has bedrock at the bottom and height:
		int prevHeight = chd.GetHeight(0, 0);
		for (int x = 0; x < cChunkDef::Width; ++x)
		{
			for (int z = 0; z < cChunkDef::Width; ++z)
			{
				TEST_EQUAL_MSG(chd.GetBlock({x, 0, z}), BlockType::Bedrock, fmt::format(FMT_STRING("Bedrock floor at {{{}, {}}}"), x, z));
				auto y = chd.GetHeight(x, z);
				auto TopBlock = chd.GetBlock({x, y, z});
				// Skip the mushrooms generated on the top bedrock layer:
				if (
					(TopBlock.Type() == BlockType::BrownMushroomBlock) ||
					(TopBlock.Type() == BlockType::RedMushroomBlock)
				)
				{
					y -= 1;
				}
				TEST_EQUAL_MSG(y, prevHeight, fmt::format(FMT_STRING("Failed: Same height across the entire chunk, at {{{}, {}}}: exp {}, got {}; top block: {}"),
					x, z, prevHeight, y, chd.GetBlock({x, y, z}).Type()
				));
				auto Block = chd.GetBlock({x, y, z});
				TEST_EQUAL_MSG(Block.Type(), BlockType::Bedrock,
					fmt::format(FMT_STRING("Bedrock ceiling at {{{}, {}, {}}}: {}"), x, y, z, Block.Type())
				);
			}
		}

		// Check that each column contains at least one Nether-native block:
		for (int x = 0; x < cChunkDef::Width; ++x)
		{
			for (int z = 0; z < cChunkDef::Width; ++z)
			{
				bool hasSuitableBlockType = false;
				for (int y = chd.GetHeight(x, z); y > 0; --y)
				{
					switch (chd.GetBlock({x, y, z}).Type())
					{
						case BlockType::Netherrack:
						case BlockType::NetherQuartzOre:
						case BlockType::Lava:
						case BlockType::SoulSand:
						case BlockType::Bedrock:
						case BlockType::Air:
						case BlockType::CaveAir:
						{
							hasSuitableBlockType = true;
							break;
						}
						default:
						{
							hasSuitableBlockType = false;
							break;
						}
					}
				}  // for y
				if (!hasSuitableBlockType)
				{
					printChunkColumn(chd, x, z);
					TEST_FAIL(fmt::format(FMT_STRING("!hasSuitableBlockType at column <{}, {}> of chunk [{}, 0]"), x, z, chunkX));
				}
			}
		}
	}
}





/** Storage for checksums with chunk coords. */
struct CoordsWithChecksum
{
	cChunkCoords mCoords;
	AString mChecksum;
	CoordsWithChecksum(int aChunkX, int aChunkZ, const AString & aChecksum):
		mCoords(aChunkX, aChunkZ),
		mChecksum(aChecksum)
	{}
};





/** Checks that the specified generator generates chunk that match the specified checksums. */
static void checkChunkChecksums(
	cChunkGenerator & aGenerator,
	const std::vector<CoordsWithChecksum> & aCoordsWithChecksum,
	const AString & aDimension
)
{
	LOG("Testing the repeatability of the %s generator", aDimension);
	for (const auto & coords: aCoordsWithChecksum)
	{
		cChunkDesc chd(coords.mCoords);
		aGenerator.Generate(chd);
		/*
		cFile f(Printf("Repeatability_%s-%02d-%02d.raw", aDimension, coords.mCoords.m_ChunkX, coords.mCoords.m_ChunkZ), cFile::fmWrite);
		f.Write(chd.GetBlockTypes(), sizeof(chd.GetBlockTypes()));
		f.Close();
		*/
		auto checksum = chunkSHA1(chd);

		TEST_EQUAL_MSG(checksum, coords.mChecksum,
			fmt::format(FMT_STRING("{} chunk {} SHA1: expected {}, got {}"), aDimension, coords.mCoords.ToString(), coords.mChecksum, checksum)
		);
	}
}





/** Checks that the generated chunks look the same across all builds on all platforms.
This is done by SHA1-ing the blocks for known chunks and comparing against known values.
If the generator defaults change, this test will likely break, just update the SHA1s. */
static void testRepeatability(cChunkGenerator & aDefaultOverworldGenerator, cChunkGenerator & aDefaultNetherGenerator)
{

	// Test the default Overworld generator:
	std::vector<CoordsWithChecksum> overworldChecksums =
	{
		{0,    0, "6130c010a4fc58bd3cf1c00d7e6dcedb218bf26a"},
		{1,    0, "-2735a213482432cbcc44b8ddaf8ed4a2faa22d1d"},
		{1,    1, "-73e93cf821096840d60f96c71b86827191256753"},
		{8, 1024, "-1bb30162a2cd33d3e3ab92b9af514f8fadc11b0"},
	};
	checkChunkChecksums(aDefaultOverworldGenerator, overworldChecksums, "Overworld");

	// Test the default Nether generator:
	std::vector<CoordsWithChecksum> netherChecksums =
	{
		{ 0,    0, "445a149c2b2dc12cbb2e2d8cdf432b44b9136ffc"},
		{ 1,    0, "-305a31b8424ca6d95bb22f7a39b860b2feab2f"},
		{ 1,    1, "-67ecbf4e488daf69c960ff52923140325df66e55"},
		{17,    0, "270d3705328fd7fdcfcc6af45312a67c570fcea"},
		{ 8, 1024, "3b915b890d06fbc5c76eb4d560b1264c8b9ee39b"},
	};
	checkChunkChecksums(aDefaultNetherGenerator, netherChecksums, "Nether");
}





IMPLEMENT_TEST_MAIN("BasicGeneratorTest",

	// Create a default Overworld generator:
	cIniFile iniOverworld;
	iniOverworld.AddValue("General", "Dimension", "Overworld");
	iniOverworld.AddValueI("Seed", "Seed", 1);
	iniOverworld.AddValue("Generator", "Finishers", "");  // Use no finishers, so that we don't have to check too many blocktypes
	auto defaultOverworldGen = cChunkGenerator::CreateFromIniFile(iniOverworld);
	TEST_NOTEQUAL(defaultOverworldGen, nullptr);

	// Create a default Nether generator:
	cIniFile iniNether;
	iniNether.AddValue("General", "Dimension", "Nether");
	iniNether.AddValueI("Seed", "Seed", 1);
	iniNether.AddValue("Generator", "Finishers", "");  // Use no finishers, so that we don't have to check too many blocktypes
	auto defaultNetherGen = cChunkGenerator::CreateFromIniFile(iniNether);
	TEST_NOTEQUAL(defaultNetherGen, nullptr);

	// Run the tests on the generators:
	testGenerateOverworld(*defaultOverworldGen);
	testGenerateNether(*defaultNetherGen);
	testRepeatability(*defaultOverworldGen, *defaultNetherGen);
)
