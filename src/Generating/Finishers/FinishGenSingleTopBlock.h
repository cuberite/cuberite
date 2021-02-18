
// FinishGenSingleTopBlock.h

#pragma once

#include "../ComposableGenerator.h"
#include "../../BlockInfo.h"

/** This class adds a single top block in random positions in the specified biome on top of specified allowed blocks.
Used for:
- Lilypads finisher
- DeadBushes finisher */
class cFinishGenSingleTopBlock :
	public cFinishGen
{
public:
	typedef std::vector<BLOCKTYPE> BlockList;
	bool m_IsAllowedBelow[256];

	typedef std::vector<EMCSBiome> BiomeList;
	bool m_IsBiomeAllowed[256];


	cFinishGenSingleTopBlock(
		int a_Seed, BLOCKTYPE a_BlockType, BiomeList a_Biomes, int a_Amount,
		BlockList a_AllowedBelow
	) :
		m_Noise(a_Seed),
		m_BlockType(a_BlockType),
		m_Amount(a_Amount)
	{
		// Initialize all the block types.
		for (size_t idx = 0; idx < ARRAYCOUNT(m_IsAllowedBelow); ++idx)
		{
			m_IsAllowedBelow[idx] = false;
		}

		// Load the allowed blocks into m_IsAllowedBelow
		for (BlockList::iterator itr = a_AllowedBelow.begin(); itr != a_AllowedBelow.end(); ++itr)
		{
			m_IsAllowedBelow[*itr] = true;
		}

		// Initialize all the biome types.
		for (size_t idx = 0; idx < ARRAYCOUNT(m_IsBiomeAllowed); ++idx)
		{
			m_IsBiomeAllowed[idx] = false;
		}

		// Load the allowed biomes into m_IsBiomeAllowed
		for (BiomeList::iterator itr = a_Biomes.begin(); itr != a_Biomes.end(); ++itr)
		{
			m_IsBiomeAllowed[*itr] = true;
		}
	}

protected:
	cNoise m_Noise;
	BLOCKTYPE m_BlockType;

	/** Relative amount of blocks to try adding. 1 = one block per 256 biome columns. */
	int m_Amount;


	int GetNumToGen(const cChunkDef::BiomeMap & a_BiomeMap);

	/** Returns true if the given biome is a biome that is allowed. */
	inline bool IsAllowedBiome(EMCSBiome a_Biome)
	{
		return m_IsBiomeAllowed[a_Biome];
	}

	/** Returns true if the given blocktype may be below m_BlockType */
	inline bool IsAllowedBlockBelow(BLOCKTYPE a_BlockBelow)
	{
		return m_IsAllowedBelow[a_BlockBelow];
	}


	// cFinishGen override:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
} ;
