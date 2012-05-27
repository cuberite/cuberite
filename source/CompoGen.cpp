
// CompoGen.cpp

/* Implements the various terrain composition generators:
	- cCompoGenSameBlock
	- cCompoGenDebugBiomes
	- cCompoGenClassic
*/

#include "Globals.h"
#include "CompoGen.h"
#include "BlockID.h"





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompoGenSameBlock:

void cCompoGenSameBlock::ComposeTerrain(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
	cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
	const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
	cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
	cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
)
{
	memset(a_BlockTypes, E_BLOCK_AIR, sizeof(a_BlockTypes));
	memset(a_BlockMeta, 0, sizeof(a_BlockMeta));
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Start;
			if (m_IsBedrocked)
			{
				a_BlockTypes[cChunkDef::MakeIndex(x, 0, z)] = E_BLOCK_BEDROCK;
				Start = 1;
			}
			else
			{
				Start = 0;
			}
			for (int y = a_HeightMap[x + cChunkDef::Width * z]; y >= Start; y--)
			{
				a_BlockTypes[cChunkDef::MakeIndex(x, y, z)] = m_BlockType;
			}  // for y
		}  // for z
	}  // for x
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompoGenDebugBiomes:

void cCompoGenDebugBiomes::ComposeTerrain(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
	cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
	const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
	cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
	cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
)
{
	static BLOCKTYPE Blocks[] =
	{
		E_BLOCK_STONE,
		E_BLOCK_COBBLESTONE, 
		E_BLOCK_LOG,
		E_BLOCK_PLANKS,
		E_BLOCK_SANDSTONE,
		E_BLOCK_WHITE_CLOTH,
		E_BLOCK_COAL_ORE,
		E_BLOCK_IRON_ORE,
		E_BLOCK_GOLD_ORE,
		E_BLOCK_DIAMOND_ORE,
		E_BLOCK_LAPIS_ORE,
		E_BLOCK_REDSTONE_ORE,
		E_BLOCK_IRON_BLOCK,
		E_BLOCK_GOLD_BLOCK,
		E_BLOCK_DIAMOND_BLOCK,
		E_BLOCK_LAPIS_BLOCK,
		E_BLOCK_BRICK,
		E_BLOCK_MOSSY_COBBLESTONE,
		E_BLOCK_OBSIDIAN,
		E_BLOCK_NETHERRACK,
		E_BLOCK_SOULSAND,
		E_BLOCK_NETHER_BRICK,
		E_BLOCK_BEDROCK,
	} ;
	
	memset(a_BlockTypes, E_BLOCK_AIR, sizeof(a_BlockTypes));
	memset(a_BlockMeta, 0, sizeof(a_BlockMeta));

	cChunkDef::BiomeMap BiomeMap;
	m_BiomeGen->GenBiomes(a_ChunkX, a_ChunkZ, BiomeMap);
	
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			BLOCKTYPE BlockType = Blocks[cChunkDef::GetBiome(BiomeMap, x, z) % ARRAYCOUNT(Blocks)];
			for (int y = a_HeightMap[x + cChunkDef::Width * z]; y >= 0; y--)
			{
				cChunkDef::SetBlock(a_BlockTypes, x, y, z, BlockType);
			}  // for y
		}  // for z
	}  // for x
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCompoGenClassic:

cCompoGenClassic::cCompoGenClassic(int a_SeaLevel, int a_BeachHeight, int a_BeachDepth) :
	m_SeaLevel(a_SeaLevel),
	m_BeachHeight(a_BeachHeight),
	m_BeachDepth(a_BeachDepth)
{
}





void cCompoGenClassic::ComposeTerrain(
	int a_ChunkX, int a_ChunkZ,
	cChunkDef::BlockTypes & a_BlockTypes,      // BlockTypes to be generated
	cChunkDef::BlockNibbles & a_BlockMeta,     // BlockMetas to be generated
	const cChunkDef::HeightMap & a_HeightMap,  // The height map to fit
	cEntityList & a_Entities,                  // Entitites may be generated along with the terrain
	cBlockEntityList & a_BlockEntities         // Block entitites may be generated (chests / furnaces / ...)
)
{
	/* The classic composition means:
		- 1 layer of grass, 3 of dirt and the rest stone, if the height > sealevel + beachheight
		- 3 sand and a 1 sandstone, rest stone if between sealevel and sealevel + beachheight
		- water from waterlevel to height, then 3 sand, 1 sandstone, the rest stone, if water depth < beachdepth
		- water from waterlevel, then 3 dirt, the rest stone otherwise
		- bedrock at the bottom
	*/

	memset(a_BlockTypes, E_BLOCK_AIR, sizeof(a_BlockTypes));
	memset(a_BlockMeta, 0, sizeof(a_BlockMeta));

	// The patterns to use for different situations, must be same length!
	static const BLOCKTYPE PatternGround[] = {E_BLOCK_GRASS, E_BLOCK_DIRT, E_BLOCK_DIRT, E_BLOCK_DIRT} ;
	static const BLOCKTYPE PatternBeach[]  = {E_BLOCK_SAND,  E_BLOCK_SAND, E_BLOCK_SAND, E_BLOCK_SANDSTONE} ;
	static const BLOCKTYPE PatternOcean[]  = {E_BLOCK_DIRT,  E_BLOCK_DIRT, E_BLOCK_DIRT, E_BLOCK_STONE} ;
	static int PatternLength = ARRAYCOUNT(PatternGround);
	ASSERT(ARRAYCOUNT(PatternGround) == ARRAYCOUNT(PatternBeach));
	ASSERT(ARRAYCOUNT(PatternGround) == ARRAYCOUNT(PatternOcean));
	
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			int Height = a_HeightMap[x + cChunkDef::Width * z];
			const BLOCKTYPE * Pattern;
			if (Height > m_SeaLevel + m_BeachHeight)
			{
				Pattern = PatternGround;
			}
			else if (Height > m_SeaLevel - m_BeachDepth)
			{
				Pattern = PatternBeach;
			}
			else
			{
				Pattern = PatternOcean;
			}
			
			// Fill water from sealevel down to height (if any):
			for (int y = m_SeaLevel; y >= Height; --y)
			{
				cChunkDef::SetBlock(a_BlockTypes, x, y, z, E_BLOCK_STATIONARY_WATER);
			}
			
			// Fill from height till the bottom:
			for (int y = Height; y >= 1; y--)
			{
				cChunkDef::SetBlock(a_BlockTypes, x, y, z, (Height - y < PatternLength) ? Pattern[Height - y] : E_BLOCK_STONE);
			}
			
			// The last layer is always bedrock:
			cChunkDef::SetBlock(a_BlockTypes, x, 0, z, E_BLOCK_BEDROCK);
		}  // for x
	}  // for z
}




