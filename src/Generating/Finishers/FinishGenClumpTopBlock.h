// FinishGenClumpTopBlock.h

#pragma once

#include "../ComposableGenerator.h"
#include "../../Noise/Noise.h"

class cFinishGenClumpTopBlock :
	public cFinishGen
{
public:
	// Contains the meta, type and weight for a clump block
	struct FoliageInfo
	{
		BLOCKTYPE m_BlockType;
		NIBBLETYPE m_BlockMeta;
		int m_Weight;

		FoliageInfo(BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, int a_Weight) :
			m_BlockType(a_BlockType),
			m_BlockMeta(a_BlockMeta),
			m_Weight(a_Weight)
		{}
	};

	// Contains the minimum and maximum amount of clumps for a biome and it's blocks.
	struct BiomeInfo
	{
		int m_MinNumClumpsPerChunk;
		int m_MaxNumClumpsPerChunk;
		std::vector<FoliageInfo> m_Blocks;

		BiomeInfo() :
			m_MinNumClumpsPerChunk(0),
			m_MaxNumClumpsPerChunk(2),
			m_Blocks()
		{}

		BiomeInfo(int a_MinNumClumpsPerChunk, int a_MaxNumClumpsPerChunk, std::vector<FoliageInfo> a_Blocks) :
			m_MinNumClumpsPerChunk(a_MinNumClumpsPerChunk),
			m_MaxNumClumpsPerChunk(a_MaxNumClumpsPerChunk),
			m_Blocks(std::move(a_Blocks))
		{}
	};


	cFinishGenClumpTopBlock(int a_Seed, std::vector<BiomeInfo> a_BlockList) :
		m_Noise(a_Seed),
		m_FlowersPerBiome()
	{
		std::swap(a_BlockList, m_FlowersPerBiome);
	}

	/** Parses a string and puts a vector with a length of biMaxVariantBiome in a_Output.
	The format of the string is "<Biomes separated with a comma>;<Blocks separated with a comma>". This can also be repeated with a | */
	static void ParseConfigurationString(const AString & a_String, std::vector<BiomeInfo> & a_Output);

	/** Parses an inifile in search for all clumps */
	static std::vector<BiomeInfo> ParseIniFile(cIniFile & a_IniFile, const AString & a_ClumpPrefix);
protected:

	cNoise m_Noise;
	std::vector<BiomeInfo> m_FlowersPerBiome;

	/** The maximum number of foliage per clump */
	const int MAX_NUM_FOLIAGE = 8;

	/** The minimum number of foliage per clump */
	const int MIN_NUM_FOLIAGE = 4;

	/** The maximum range a foliage can be placed from the center of the clump */
	const int RANGE_FROM_CENTER = 5;

	void TryPlaceFoliageClump(cChunkDesc & a_ChunkDesc, int a_RelX, int a_RelZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, bool a_IsDoubleTall);
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
};
