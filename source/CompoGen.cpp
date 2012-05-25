
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
	memset(a_BlockTypes, E_BLOCK_AIR, sizeof(a_BlockTypes));
	memset(a_BlockMeta, 0, sizeof(a_BlockMeta));

	cChunkDef::BiomeMap BiomeMap;
	m_BiomeGen->GenBiomes(a_ChunkX, a_ChunkZ, BiomeMap);
	
	for (int z = 0; z < cChunkDef::Width; z++)
	{
		for (int x = 0; x < cChunkDef::Width; x++)
		{
			BLOCKTYPE BlockType = (BLOCKTYPE)cChunkDef::GetBiome(BiomeMap, x, z);
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




