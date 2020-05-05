#include "Globals.h"
#include "Generating/ChunkGenerator.h"
#include "Generating/ChunkDesc.h"
#include "../TestHelpers.h"
#include "IniFile.h"
#include "mbedTLS++/Sha1Checksum.h"





/** Checks that the chunk's heightmap corresponds to the chunk contents. */
static void verifyChunkDescHeightmap(const cChunkDesc & a_ChunkDesc)
{
	for (int x = 0; x < cChunkDef::Width; x++)
	{
		for (int z = 0; z < cChunkDef::Width; z++)
		{
			for (int y = cChunkDef::Height - 1; y > 0; y--)
			{
				BLOCKTYPE BlockType = a_ChunkDesc.GetBlockType(x, y, z);
				if (BlockType != E_BLOCK_AIR)
				{
					int Height = a_ChunkDesc.GetHeight(x, z);
					TEST_EQUAL_MSG(Height, y, Printf("Chunk height at <%d, %d>: exp %d, got %d", x, z, y, Height));
					break;
				}
			}  // for y
		}  // for z
	}  // for x
}





static AString chunkSHA1(const cChunkDesc & a_ChunkDesc)
{
	cSha1Checksum cs;
	cs.Update(a_ChunkDesc.GetBlockTypes(), cChunkDef::Width * cChunkDef::Width * cChunkDef::Height);
	cSha1Checksum::Checksum digest;
	cs.Finalize(digest);
	AString res;
	cSha1Checksum::DigestToJava(digest, res);
	return res;
}





/** Prints out the entire column from the chunk, one block type per line. */
static void printChunkColumn(const cChunkDesc & a_ChunkDesc, int a_X, int a_Z)
{
	auto prevBlockType = a_ChunkDesc.GetBlockType(a_X, cChunkDef::Height - 1, a_Z);
	int count = 1;
	LOG("Column {%d, %d}:", a_X, a_Z);
	LOG("Yfrom\tYto\tcnt\ttype\ttypeStr");
	for (int y = cChunkDef::Height - 2; y >= 0; --y)
	{
		auto blockType = a_ChunkDesc.GetBlockType(a_X, y, a_Z);
		if (blockType != prevBlockType)
		{
			LOG("%d\t%d\t%d\t%d\t%s", y + 1, y + count, count, prevBlockType, ItemTypeToString(prevBlockType));
			prevBlockType = blockType;
			count = 1;
		}
		else
		{
			count += 1;
		}
	}
	LOG("%d\t%d\t%d\t%s", 0, count, prevBlockType, ItemTypeToString(prevBlockType));
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
				TEST_EQUAL_MSG(chd.GetBlockType(x, 0, z), E_BLOCK_BEDROCK, Printf("Bedrock floor at {%d, 0, %d}", x, z));
			}
		}

		// Check that the blocks on the top are valid Overworld blocks:
		static std::set<BLOCKTYPE> validOverworldBlockTypes =
		{
			E_BLOCK_STONE,
			E_BLOCK_GRASS,
			E_BLOCK_WATER,
			E_BLOCK_STATIONARY_WATER,
			E_BLOCK_LAVA,
			E_BLOCK_STATIONARY_LAVA,
			E_BLOCK_SAND,
			E_BLOCK_GRAVEL,
			E_BLOCK_LEAVES,
			E_BLOCK_NEW_LEAVES,
		};
		for (int x = 0; x < cChunkDef::Width; ++x)
		{
			for (int z = 0; z < cChunkDef::Width; ++z)
			{
				auto y = chd.GetHeight(x, z);
				auto blockType = chd.GetBlockType(x, y, z);
				TEST_EQUAL_MSG(validOverworldBlockTypes.count(blockType), 1,
					Printf("Block at {%d, %d, %d}: %d", x, y, z, blockType)
				);
				if (y < cChunkDef::Height - 1)
				{
					TEST_EQUAL_MSG(chd.GetBlockType(x, cChunkDef::Height - 1, z), E_BLOCK_AIR,
						Printf("Air at {%d, %d, %d}", x, cChunkDef::Height - 1, z)
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
				TEST_EQUAL_MSG(chd.GetBiome(x, z), biNether, Printf("Nether biome at {%d, %d}", x, z));
			}
		}

		// Check that it has bedrock at the bottom and height:
		int prevHeight = chd.GetHeight(0, 0);
		for (int x = 0; x < cChunkDef::Width; ++x)
		{
			for (int z = 0; z < cChunkDef::Width; ++z)
			{
				TEST_EQUAL_MSG(chd.GetBlockType(x, 0, z), E_BLOCK_BEDROCK, Printf("Bedrock floor at {%d, 0, %d}", x, z));
				auto y = chd.GetHeight(x, z);
				auto topBlockType = chd.GetBlockType(x, y, z);
				// Skip the mushrooms generated on the top bedrock layer:
				if (
					(topBlockType == E_BLOCK_BROWN_MUSHROOM) ||
					(topBlockType == E_BLOCK_RED_MUSHROOM)
				)
				{
					y -= 1;
				}
				TEST_EQUAL_MSG(y, prevHeight, Printf("Failed: Same height across the entire chunk, at {%d, %d}: exp %d, got %d; top block: %d",
					x, z, prevHeight, y, chd.GetBlockType(x, y, z)
				));
				auto blockType = chd.GetBlockType(x, y, z);
				TEST_EQUAL_MSG(blockType, E_BLOCK_BEDROCK,
					Printf("Bedrock ceiling at {%d, %d, %d}: %d", x, y, z, blockType)
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
					switch (chd.GetBlockType(x, y, z))
					{
						case E_BLOCK_NETHERRACK:
						case E_BLOCK_NETHER_QUARTZ_ORE:
						case E_BLOCK_LAVA:
						case E_BLOCK_STATIONARY_LAVA:
						case E_BLOCK_SOULSAND:
						{
							hasSuitableBlockType = true;
							break;
						}
					}
				}  // for y
				if (!hasSuitableBlockType)
				{
					printChunkColumn(chd, x, z);
					TEST_FAIL(Printf("!hasSuitableBlockType at column {%d, %d} of chunk [%d, 0]", x, z, chunkX));
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
			Printf("%s chunk %s SHA1: expected %s, got %s", aDimension, coords.mCoords.ToString(), coords.mChecksum, checksum)
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
		{0,    0, "-380dace6af9e653a2c68a51779cf5b8ff521cde1"},
		{1,    0, "-651dfec5a64b7adccf6bf2845396e27f53c6c4c0"},
		{1,    1, "-621454452edeb0ac369fea520fee3d80a5ecae49"},
		{8, 1024, "5ed38ba7ffee6b29f774ad24820ad3ca1ff058bf"},
	};
	checkChunkChecksums(aDefaultOverworldGenerator, overworldChecksums, "Overworld");

	// Test the default Nether generator:
	std::vector<CoordsWithChecksum> netherChecksums =
	{
		{ 0,    0, "-487001a1ada9cdd7c8b557d3ff7081881f57c660"},
		{ 1,    0, "a074ac7a1f2fbf4173324e5edd792197d6a29c2"},
		{ 1,    1, "5867c5121f2a259ebc2aa53ecafed93dd3d6de95"},
		{17,    0, "-300191cee5b30592f7b61cd22ea08669eba3f569"},
		{ 8, 1024, "69bbda09be981f5e3adc53d0a49995aff43f1290"},
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
	auto defaultNetherGen = cChunkGenerator::CreateFromIniFile(iniNether);
	TEST_NOTEQUAL(defaultNetherGen, nullptr);

	// Run the tests on the generators:
	testGenerateOverworld(*defaultOverworldGen);
	testGenerateNether(*defaultNetherGen);
	testRepeatability(*defaultOverworldGen, *defaultNetherGen);
)
