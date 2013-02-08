
// ChunkDesc.cpp

// Implements the cChunkDesc class representing the chunk description used while generating a chunk. This class is also exported to Lua for HOOK_CHUNK_GENERATING.

#include "Globals.h"
#include "ChunkDesc.h"
#include "../BlockArea.h"





cChunkDesc::cChunkDesc(int a_ChunkX, int a_ChunkZ) :
	m_ChunkX(a_ChunkX),
	m_ChunkZ(a_ChunkZ),
	m_bUseDefaultBiomes(true),
	m_bUseDefaultHeight(true),
	m_bUseDefaultComposition(true),
	m_bUseDefaultStructures(true),
	m_bUseDefaultFinish(true)
{
	memset(m_BlockTypes, 0, sizeof(cChunkDef::BlockTypes));
	memset(m_BlockMeta,  0, sizeof(cChunkDef::BlockNibbles));
	memset(m_BiomeMap,   0, sizeof(cChunkDef::BiomeMap));
	memset(m_HeightMap,  0, sizeof(cChunkDef::HeightMap));
}





cChunkDesc::~cChunkDesc()
{
	// Nothing needed yet
}





void cChunkDesc::FillBlocks(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	const NIBBLETYPE CompressedMeta = a_BlockMeta | (a_BlockMeta << 4);
	memset(m_BlockTypes, a_BlockType,    sizeof(cChunkDef::BlockTypes));
	memset(m_BlockMeta,  CompressedMeta, sizeof(cChunkDef::BlockNibbles));
}





void cChunkDesc::SetBlockTypeMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	int Index = cChunkDef::MakeIndex(a_RelX, a_RelY, a_RelZ);
	cChunkDef::SetBlock(m_BlockTypes, Index, a_BlockType);
	cChunkDef::SetNibble(m_BlockMeta, Index, a_BlockMeta);
}





void cChunkDesc::GetBlockTypeMeta(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE & a_BlockType, NIBBLETYPE & a_BlockMeta)
{
	int Index = cChunkDef::MakeIndex(a_RelX, a_RelY, a_RelZ);
	a_BlockType = cChunkDef::GetBlock(m_BlockTypes, Index);
	a_BlockMeta = cChunkDef::GetNibble(m_BlockMeta, Index);
}





void cChunkDesc::SetBlockType(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType)
{
	cChunkDef::SetBlock(m_BlockTypes, a_RelX, a_RelY, a_RelZ, a_BlockType);
}





BLOCKTYPE cChunkDesc::GetBlockType(int a_RelX, int a_RelY, int a_RelZ)
{
	return cChunkDef::GetBlock(m_BlockTypes, a_RelX, a_RelY, a_RelZ);
}





NIBBLETYPE cChunkDesc::GetBlockMeta(int a_RelX, int a_RelY, int a_RelZ)
{
	return cChunkDef::GetNibble(m_BlockMeta, a_RelX, a_RelY, a_RelZ);
}





void cChunkDesc::SetBlockMeta(int a_RelX, int a_RelY, int a_RelZ, NIBBLETYPE a_BlockMeta)
{
	cChunkDef::SetNibble(m_BlockMeta, a_RelX, a_RelY, a_RelZ, a_BlockMeta);
}





void cChunkDesc::SetBiome(int a_RelX, int a_RelZ, int a_BiomeID)
{
	cChunkDef::SetBiome(m_BiomeMap, a_RelX, a_RelZ, (EMCSBiome)a_BiomeID);
}




EMCSBiome cChunkDesc::GetBiome(int a_RelX, int a_RelZ)
{
	return cChunkDef::GetBiome(m_BiomeMap, a_RelX, a_RelZ);
}





void cChunkDesc::SetHeight(int a_RelX, int a_RelZ, int a_Height)
{
	cChunkDef::SetHeight(m_HeightMap, a_RelX, a_RelZ, a_Height);
}





int cChunkDesc::GetHeight(int a_RelX, int a_RelZ)
{
	return cChunkDef::GetHeight(m_HeightMap, a_RelX, a_RelZ);
}





void cChunkDesc::SetUseDefaultBiomes(bool a_bUseDefaultBiomes)
{
	m_bUseDefaultBiomes = a_bUseDefaultBiomes;
}





bool cChunkDesc::IsUsingDefaultBiomes(void) const
{
	return m_bUseDefaultBiomes;
}





void cChunkDesc::SetUseDefaultHeight(bool a_bUseDefaultHeight)
{
	m_bUseDefaultHeight = a_bUseDefaultHeight;
}





bool cChunkDesc::IsUsingDefaultHeight(void) const
{
	return m_bUseDefaultHeight;
}





void cChunkDesc::SetUseDefaultComposition(bool a_bUseDefaultComposition)
{
	m_bUseDefaultComposition = a_bUseDefaultComposition;
}





bool cChunkDesc::IsUsingDefaultComposition(void) const
{
	return m_bUseDefaultComposition;
}





void cChunkDesc::SetUseDefaultStructures(bool a_bUseDefaultStructures)
{
	m_bUseDefaultStructures = a_bUseDefaultStructures;
}





bool cChunkDesc::IsUsingDefaultStructures(void) const
{
	return m_bUseDefaultStructures;
}





void cChunkDesc::SetUseDefaultFinish(bool a_bUseDefaultFinish)
{
	m_bUseDefaultFinish = a_bUseDefaultFinish;
}





bool cChunkDesc::IsUsingDefaultFinish(void) const
{
	return m_bUseDefaultFinish;
}




void cChunkDesc::WriteBlockArea(const cBlockArea & a_BlockArea, int a_RelX, int a_RelY, int a_RelZ)
{
	if (!a_BlockArea.HasBlockTypes() && !a_BlockArea.HasBlockMetas())
	{
		LOGWARNING("Request was made to write a block area without BlockTypes nor BlockMetas into cChunkDesc. Ignoring.");
		return;
	}
	int BAOffX = std::max(0, -a_RelX);  // Offset in BA where to start reading
	int CDOffX = std::max(0,  a_RelX);  // Offset in CD where to start writing
	int SizeX  = std::min(a_BlockArea.GetSizeX() - BAOffX, cChunkDef::Width - CDOffX);  // Number of slices to write
	int BAOffY = std::max(0, -a_RelY);  // Offset in BA where to start reading
	int CDOffY = std::max(0,  a_RelY);  // Offset in CD where to start writing
	int SizeY  = std::min(a_BlockArea.GetSizeY() - BAOffY, cChunkDef::Height - CDOffY);  // Number of layers to write
	int BAOffZ = std::max(0, -a_RelZ);  // Offset in BA where to start reading
	int CDOffZ = std::max(0,  a_RelZ);  // Offset in CD where to start writing
	int SizeZ  = std::min(a_BlockArea.GetSizeZ() - BAOffZ, cChunkDef::Width - CDOffZ);  // Number of slices to write
	
	if (a_BlockArea.HasBlockTypes())
	{
		for (int y = 0; y < SizeY; y++)
		{
			int BAY = BAOffY + y;
			int CDY = CDOffY + y;
			for (int z = 0; z < SizeZ; z++)
			{
				int BAZ = BAOffZ + z;
				int CDZ = CDOffZ + z;
				for (int x = 0; x < SizeX; x++)
				{
					int BAX = BAOffX + x;
					int CDX = BAOffX + x;
					cChunkDef::SetBlock(m_BlockTypes, CDX, CDY, CDZ, a_BlockArea.GetRelBlockType(BAX, BAY, BAZ));
				}  // for x
			}  // for z
		}  // for y
	}  // HasBlockTypes()
	
	if (a_BlockArea.HasBlockMetas())
	{
		for (int y = 0; y < SizeY; y++)
		{
			int BAY = BAOffY + y;
			int CDY = CDOffY + y;
			for (int z = 0; z < SizeZ; z++)
			{
				int BAZ = BAOffZ + z;
				int CDZ = CDOffZ + z;
				for (int x = 0; x < SizeX; x++)
				{
					int BAX = BAOffX + x;
					int CDX = BAOffX + x;
					cChunkDef::SetNibble(m_BlockMeta, CDX, CDY, CDZ, a_BlockArea.GetRelBlockMeta(BAX, BAY, BAZ));
				}  // for x
			}  // for z
		}  // for y
	}  // HasBlockMetas()
}





void cChunkDesc::ReadBlockArea(cBlockArea & a_Dest, int a_MinRelX, int a_MaxRelX, int a_MinRelY, int a_MaxRelY, int a_MinRelZ, int a_MaxRelZ)
{
	// Normalize the coords:
	if (a_MinRelX > a_MaxRelX)
	{
		std::swap(a_MinRelX, a_MaxRelX);
	}
	if (a_MinRelY > a_MaxRelY)
	{
		std::swap(a_MinRelY, a_MaxRelY);
	}
	if (a_MinRelZ > a_MaxRelZ)
	{
		std::swap(a_MinRelZ, a_MaxRelZ);
	}

	// Include the Max coords:
	a_MaxRelX += 1;
	a_MaxRelY += 1;
	a_MaxRelZ += 1;
	
	// Check coords validity:
	if (a_MinRelX < 0)
	{
		LOGWARNING("%s: MinRelX less than zero, adjusting to zero", __FUNCTION__);
		a_MinRelX = 0;
	}
	else if (a_MinRelX >= cChunkDef::Width)
	{
		LOGWARNING("%s: MinRelX more than chunk width, adjusting to chunk width", __FUNCTION__);
		a_MinRelX = cChunkDef::Width - 1;
	}
	if (a_MaxRelX < 0)
	{
		LOGWARNING("%s: MaxRelX less than zero, adjusting to zero", __FUNCTION__);
		a_MaxRelX = 0;
	}
	else if (a_MinRelX >= cChunkDef::Width)
	{
		LOGWARNING("%s: MaxRelX more than chunk width, adjusting to chunk width", __FUNCTION__);
		a_MaxRelX = cChunkDef::Width - 1;
	}

	if (a_MinRelY < 0)
	{
		LOGWARNING("%s: MinRelY less than zero, adjusting to zero", __FUNCTION__);
		a_MinRelY = 0;
	}
	else if (a_MinRelY >= cChunkDef::Height)
	{
		LOGWARNING("%s: MinRelY more than chunk height, adjusting to chunk height", __FUNCTION__);
		a_MinRelY = cChunkDef::Height - 1;
	}
	if (a_MaxRelY < 0)
	{
		LOGWARNING("%s: MaxRelY less than zero, adjusting to zero", __FUNCTION__);
		a_MaxRelY = 0;
	}
	else if (a_MinRelY >= cChunkDef::Height)
	{
		LOGWARNING("%s: MaxRelY more than chunk height, adjusting to chunk height", __FUNCTION__);
		a_MaxRelY = cChunkDef::Height - 1;
	}
	
	if (a_MinRelZ < 0)
	{
		LOGWARNING("%s: MinRelZ less than zero, adjusting to zero", __FUNCTION__);
		a_MinRelZ = 0;
	}
	else if (a_MinRelZ >= cChunkDef::Width)
	{
		LOGWARNING("%s: MinRelZ more than chunk width, adjusting to chunk width", __FUNCTION__);
		a_MinRelZ = cChunkDef::Width - 1;
	}
	if (a_MaxRelZ < 0)
	{
		LOGWARNING("%s: MaxRelZ less than zero, adjusting to zero", __FUNCTION__);
		a_MaxRelZ = 0;
	}
	else if (a_MinRelZ >= cChunkDef::Width)
	{
		LOGWARNING("%s: MaxRelZ more than chunk width, adjusting to chunk width", __FUNCTION__);
		a_MaxRelZ = cChunkDef::Width - 1;
	}

	// Prepare the block area:
	int SizeX = a_MaxRelX - a_MinRelX;
	int SizeY = a_MaxRelY - a_MinRelY;
	int SizeZ = a_MaxRelZ - a_MinRelZ;
	a_Dest.Clear();
	a_Dest.m_OriginX = m_ChunkX * cChunkDef::Width + a_MinRelX;
	a_Dest.m_OriginY = a_MinRelY;
	a_Dest.m_OriginZ = m_ChunkZ * cChunkDef::Width + a_MinRelZ;
	a_Dest.SetSize(SizeX, SizeY, SizeZ, cBlockArea::baTypes | cBlockArea::baMetas);

	for (int y = 0; y < SizeY; y++)
	{
		int CDY = a_MinRelY + y;
		for (int z = 0; z < SizeZ; z++)
		{
			int CDZ = a_MinRelZ + z;
			for (int x = 0; x < SizeX; x++)
			{
				int CDX = a_MinRelX + x;
				BLOCKTYPE BlockType;
				NIBBLETYPE BlockMeta;
				GetBlockTypeMeta(CDX, CDY, CDZ, BlockType, BlockMeta);
				a_Dest.SetRelBlockTypeMeta(x, y, z, BlockType, BlockMeta);
			}  // for x
		}  // for z
	}  // for y
}





