
// ChunkDesc.cpp

// Implements the cChunkDesc class representing the chunk description used while generating a chunk. This class is also exported to Lua for HOOK_CHUNK_GENERATING.

#include "Globals.h"
#include "ChunkDesc.h"





cChunkDesc::cChunkDesc(void) :
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





void cChunkDesc::SetBlock(int a_RelX, int a_RelY, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta)
{
	int Index = cChunkDef::MakeIndex(a_RelX, a_RelY, a_RelZ);
	cChunkDef::SetBlock(m_BlockTypes, Index, a_BlockType);
	cChunkDef::SetNibble(m_BlockMeta, Index, a_BlockMeta);
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



