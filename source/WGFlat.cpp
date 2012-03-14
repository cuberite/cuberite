
// WGFlat.cpp

// Implements the cWGFlat class representing the flat world generator

#include "Globals.h"
#include "WGFlat.h"
#include "../iniFile/iniFile.h"
#include "cWorld.h"





cWGFlat::cWGFlat(cWorld * a_World) :
	super(a_World)
{
	// Load the settings from the INI file:
	cIniFile INI(a_World->GetIniFileName());
	INI.ReadFile();
	m_Height = INI.GetValueI("flat", "height", 5);
	if (m_Height < 1)
	{
		m_Height = 1;
	}
	if (m_Height > 250)
	{
		m_Height = 250;
	}
}





void cWGFlat::GenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ, char * a_BlockData, cEntityList & a_Entities, cBlockEntityList & a_BlockEntities)
{
	int SliceSize = cChunkDef::Width * cChunkDef::Width;
	memset(a_BlockData, E_BLOCK_BEDROCK, SliceSize);
	switch (m_Height)
	{
		case 1:
		{
			// Just the bedrock layer
			break;
		}
		case 2:
		{
			// Bedrock + 1 dirt layer:
			memset(a_BlockData + SliceSize, E_BLOCK_GRASS, SliceSize);
			break;
		}
		case 3:
		{
			// Bedrock + 2 dirt layers:
			memset(a_BlockData + SliceSize,     E_BLOCK_DIRT,  SliceSize);
			memset(a_BlockData + 2 * SliceSize, E_BLOCK_GRASS, SliceSize);
			break;
		}
		case 4:
		{
			// Bedrock + 3 dirt layers:
			memset(a_BlockData + SliceSize,     E_BLOCK_DIRT,  2 * SliceSize);
			memset(a_BlockData + 3 * SliceSize, E_BLOCK_GRASS, SliceSize);
			break;
		}
		default:
		{
			// Bedrock + stone layers + 3 dirt layers:
			memset(a_BlockData + SliceSize, E_BLOCK_STONE, SliceSize * (m_Height - 4));
			memset(a_BlockData + SliceSize * (m_Height - 3), E_BLOCK_DIRT,  SliceSize * 2);
			memset(a_BlockData + SliceSize * (m_Height - 1), E_BLOCK_GRASS, SliceSize);
			break;
		}
	}
	memset(a_BlockData + SliceSize * m_Height, E_BLOCK_AIR, cChunkDef::NumBlocks - SliceSize * m_Height);
	
	SliceSize /= 2;  // Nibbles from now on
	char * Meta = a_BlockData + cChunkDef::NumBlocks;
	memset(Meta, 0, cChunkDef::NumBlocks / 2);
	
	char * SkyLight = Meta + cChunkDef::NumBlocks / 2;
	memset(SkyLight, 0, m_Height * SliceSize);
	memset(SkyLight + m_Height * SliceSize, 0xff, cChunkDef::NumBlocks / 2 - m_Height * SliceSize);
	
	char * BlockLight = SkyLight + cChunkDef::NumBlocks / 2;
	memset(BlockLight, 0, cChunkDef::NumBlocks / 2);
}





void cWGFlat::PostGenerateChunk(int a_ChunkX, int a_ChunkY, int a_ChunkZ)
{
	// Nothing needed yet, just don't call the parent
}




