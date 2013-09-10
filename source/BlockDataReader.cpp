
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BlockDataReader.h"
#include "Chunk.h"



void cBlockTypeReader::read(const cChunk& a_Chunk, int a_BlockIdx)
{
	setValue(a_Chunk.GetBlock(a_BlockIdx));
}

void cBlockMetaReader::read(const cChunk& a_Chunk, int a_BlockIdx)
{
	setValue(a_Chunk.GetMeta(a_BlockIdx));
}

void cBlockLightReader::read(const cChunk& a_Chunk, int a_BlockIdx)
{
	setValue(a_Chunk.GetBlockLight(a_BlockIdx));
}

void cBlockSkyLightReader::read(const cChunk& a_Chunk, int a_BlockIdx)
{
	setValue(a_Chunk.GetSkyLight(a_BlockIdx));
}


void cBlockMultipleReader::addReader(cBlockDataReader& toAdd)
{
	m_Readers.insert(&toAdd);	
}

void cBlockMultipleReader::read(const cChunk& a_Chunk, int a_BlockIdx)
{
	for(std::set<cBlockDataReader*>::iterator it = m_Readers.begin(); it != m_Readers.end(); it++)
	{
		(*it)->read(a_Chunk,a_BlockIdx);
	}
}
