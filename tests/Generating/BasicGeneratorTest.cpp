#include "Globals.h"
#include "Generating/ChunkGenerator.h"
#include "Generating/ChunkDesc.h"
#include "../TestHelpers.h"
#include "IniFile.h"





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
static void testGenerateOverworld()
{
	LOG("Testing Overworld generator...");

	// Create a default Overworld generator:
	cIniFile ini;
	ini.AddValue("General", "Dimension", "Overworld");
	ini.AddValueI("Seed", "Seed", 1);
	ini.AddValue("Generator", "Finishers", "");  // Use no finishers, so that we don't have to check too many blocktypes
	auto gen = cChunkGenerator::CreateFromIniFile(ini);
	TEST_NOTEQUAL(gen, nullptr);

	for (int chunkX = 0; chunkX < 50; ++chunkX)
	{
		// Generate a chunk:
		cChunkDesc chd({chunkX, 0});
		gen->Generate(chunkX, 0, chd);
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
- netherrack, lava or soulsand anywhere in the middle
- valid heightmap
Multiple chunks are tested. */
static void testGenerateNether()
{
	LOG("Testing Nether generator...");

	// Create a default Nether generator:
	cIniFile ini;
	ini.AddValue("General", "Dimension", "Nether");
	ini.AddValueI("Seed", "Seed", 1);
	auto gen = cChunkGenerator::CreateFromIniFile(ini);
	TEST_NOTEQUAL(gen, nullptr);

	for (int chunkX = 0; chunkX < 50; ++chunkX)
	{
		// Generate a chunk:
		cChunkDesc chd({chunkX, 0});
		gen->Generate(chunkX, 0, chd);
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
				TEST_EQUAL(y, prevHeight);  // Same height across the entire chunk
				auto blockType = chd.GetBlockType(x, y, z);
				TEST_EQUAL_MSG(blockType, E_BLOCK_BEDROCK,
					Printf("Bedrock ceiling at {%d, %d, %d}: %d", x, y, z, blockType)
				);
			}
		}

		// Check that the blocks on the top are valid Overworld blocks:
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





IMPLEMENT_TEST_MAIN("BasicGeneratorTest",
	testGenerateOverworld();
	testGenerateNether();
)
