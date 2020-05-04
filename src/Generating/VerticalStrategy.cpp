
// VerticalStrategy.cpp

// Implements the various classes descending from cPiece::cVerticalStrategy

#include "Globals.h"
#include "VerticalStrategy.h"
#include "../Noise/Noise.h"





// Constant that is added to random seed
static const int SEED_OFFSET = 135;





// Emit a warning if the first param is true
#define CONDWARNING(ShouldLog, Fmt, ...) \
	do { \
		if (ShouldLog) \
		{ \
			LOGWARNING(Fmt, __VA_ARGS__); \
		} \
	} while (false)





////////////////////////////////////////////////////////////////////////////////
// Globals:

/** Parses a string containing a range in which both values are optional ("<MinHeight>|<MaxHeight>") into Min, Range.
Returns true if successful, false on failure.
If a_LogWarnings is true, outputs failure reasons to console.
The range is returned in a_Min and a_Range, they are left unchanged if the range value is not present in the string. */
namespace VerticalStrategy
{
	static bool ParseRange(const AString & a_Params, int & a_Min, int & a_Range, bool a_LogWarnings)
	{
		auto params = StringSplitAndTrim(a_Params, "|");
		if (params.size() == 0)
		{
			// No params, generate directly on top:
			return true;
		}
		if (!StringToInteger(params[0], a_Min))
		{
			// Failed to parse the min rel height:
			CONDWARNING(a_LogWarnings, "Cannot parse minimum height from string \"%s\"!", params[0].c_str());
			return false;
		}
		if (params.size() == 1)
		{
			// Only one param was given, there's no range
			return true;
		}
		int maxHeight = a_Min;
		if (!StringToInteger(params[1], maxHeight))
		{
			CONDWARNING(a_LogWarnings, "Cannot parse maximum height from string \"%s\"!", params[1].c_str());
			return false;
		}
		if (maxHeight < a_Min)
		{
			std::swap(maxHeight, a_Min);
		}
		a_Range = maxHeight - a_Min + 1;
		return true;
	}
}





////////////////////////////////////////////////////////////////////////////////
/** A vertical strategy that places the piece at a predefined height. */
class cVerticalStrategyFixed:
	public cPiece::cVerticalStrategy
{
public:
	cVerticalStrategyFixed(void):
		m_Height(-1000)  // Default to "unassigned" height
	{
	}


	virtual int GetVerticalPlacement(int a_BlockX, int a_BlockZ) override
	{
		return m_Height;
	}


	virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
	{
		// Params: "<Height>", compulsory
		if (!StringToInteger(a_Params, m_Height))
		{
			CONDWARNING(a_LogWarnings, "Cannot parse the fixed height from string \"%s\"!", a_Params.c_str());
			return false;
		}
		return true;
	}

protected:
	/** Height at which the pieces are placed.
	Note that this height may be outside the world, so that only a part of the piece is generated. */
	int m_Height;
};





////////////////////////////////////////////////////////////////////////////////
/** A vertical strategy that places the piece in a random height between two heights. */
class cVerticalStrategyRange:
	public cPiece::cVerticalStrategy
{
public:
	cVerticalStrategyRange(void):
		m_Seed(0),
		m_Min(-1),  // Default to "unassigned" height
		m_Range(1)
	{
	}


	virtual int GetVerticalPlacement(int a_BlockX, int a_BlockZ) override
	{
		cNoise Noise(m_Seed);
		return m_Min + (Noise.IntNoise2DInt(a_BlockX, a_BlockZ) / 7) % m_Range;
	}


	virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
	{
		// Params: "<MinHeight>|<MaxHeight>", all compulsory
		auto params = StringSplitAndTrim(a_Params, "|");
		if (params.size() != 2)
		{
			CONDWARNING(a_LogWarnings, "Cannot parse the range parameters from string \"%s\"!", a_Params.c_str());
			return false;
		}
		int Max = 0;
		if (!StringToInteger(params[0], m_Min) || !StringToInteger(params[1], Max))
		{
			CONDWARNING(a_LogWarnings, "Cannot parse the minimum or maximum height from string \"%s\"!", a_Params.c_str());
			return false;
		}
		if (m_Min > Max)
		{
			std::swap(m_Min, Max);
		}
		m_Range = Max - m_Min + 1;
		return true;
	}


	virtual void AssignGens(int a_Seed, cBiomeGenPtr & a_BiomeGen, cTerrainHeightGenPtr & a_TerrainHeightGen, int a_SeaLevel) override
	{
		m_Seed = a_Seed + SEED_OFFSET;
	}

protected:
	/** Seed for the random generator. Received in AssignGens(). */
	int m_Seed;

	/** Range for the random generator. Received in InitializeFromString(). */
	int m_Min, m_Range;
};





////////////////////////////////////////////////////////////////////////////////
/** A vertical strategy that places the piece in a specified range relative to the top of the terrain. */
class cVerticalStrategyTerrainTop:
	public cPiece::cVerticalStrategy
{
public:

	virtual int GetVerticalPlacement(int a_BlockX, int a_BlockZ) override
	{
		ASSERT(m_HeightGen != nullptr);

		int ChunkX, ChunkZ;
		cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
		cChunkDef::HeightMap HeightMap;
		m_HeightGen->GenHeightMap({ChunkX, ChunkZ}, HeightMap);
		cNoise noise(m_Seed);
		int rel = m_MinRelHeight + (noise.IntNoise2DInt(a_BlockX, a_BlockZ) / 7) % m_RelHeightRange + 1;
		return cChunkDef::GetHeight(HeightMap, a_BlockX - ChunkX * cChunkDef::Width, a_BlockZ - ChunkZ * cChunkDef::Width) + rel;
	}


	virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
	{
		// Params: "<MinRelativeHeight>|<MaxRelativeHeight>", all optional
		m_MinRelHeight = 0;
		m_RelHeightRange = 1;
		return VerticalStrategy::ParseRange(a_Params, m_MinRelHeight, m_RelHeightRange, a_LogWarnings);
	}


	virtual void AssignGens(int a_Seed, cBiomeGenPtr & a_BiomeGen, cTerrainHeightGenPtr & a_HeightGen, int a_SeaLevel) override
	{
		m_Seed = a_Seed + SEED_OFFSET;
		m_HeightGen = a_HeightGen;
	}

protected:
	/** Seed for the random generator. */
	int m_Seed;

	/** Height generator from which the top of the terrain is read. */
	cTerrainHeightGenPtr m_HeightGen;

	/** Minimum relative height at which the prefab is placed. */
	int m_MinRelHeight;

	/** Range of the relative heights at which the prefab can be placed above the minimum. */
	int m_RelHeightRange;
};





////////////////////////////////////////////////////////////////////////////////
/** A vertical strategy that places the piece within a range on top of the terrain or ocean, whichever's higher. */
class cVerticalStrategyTerrainOrOceanTop:
	public cPiece::cVerticalStrategy
{
public:

	virtual int GetVerticalPlacement(int a_BlockX, int a_BlockZ) override
	{
		ASSERT(m_HeightGen != nullptr);

		int ChunkX, ChunkZ;
		cChunkDef::BlockToChunk(a_BlockX, a_BlockZ, ChunkX, ChunkZ);
		cChunkDef::HeightMap HeightMap;
		m_HeightGen->GenHeightMap({ChunkX, ChunkZ}, HeightMap);
		int terrainHeight = static_cast<int>(cChunkDef::GetHeight(HeightMap, a_BlockX - ChunkX * cChunkDef::Width, a_BlockZ - ChunkZ * cChunkDef::Width));
		terrainHeight = std::max(1 + terrainHeight, m_SeaLevel);
		cNoise noise(m_Seed);
		int rel = m_MinRelHeight + (noise.IntNoise2DInt(a_BlockX, a_BlockZ) / 7) % m_RelHeightRange + 1;
		return terrainHeight + rel;
	}


	virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
	{
		// Params: "<MinRelativeHeight>|<MaxRelativeHeight>", all optional
		m_MinRelHeight = 0;
		m_RelHeightRange = 1;
		return VerticalStrategy::ParseRange(a_Params, m_MinRelHeight, m_RelHeightRange, a_LogWarnings);
	}


	virtual void AssignGens(int a_Seed, cBiomeGenPtr & a_BiomeGen, cTerrainHeightGenPtr & a_HeightGen, int a_SeaLevel) override
	{
		m_Seed = a_Seed + SEED_OFFSET;
		m_HeightGen = a_HeightGen;
		m_SeaLevel = a_SeaLevel;
	}

protected:
	/** Seed for the random generator. */
	int m_Seed;

	/** Height generator from which the top of the terrain is read. */
	cTerrainHeightGenPtr m_HeightGen;

	/** The sea level used by the world. */
	int m_SeaLevel;

	/** Minimum relative height at which the prefab is placed. */
	int m_MinRelHeight;

	/** Range of the relative heights at which the prefab can be placed above the minimum. */
	int m_RelHeightRange;
};





////////////////////////////////////////////////////////////////////////////////
// CreateVerticalStrategyFromString:

cPiece::cVerticalStrategyPtr CreateVerticalStrategyFromString(const AString & a_StrategyDesc, bool a_LogWarnings)
{
	// Break apart the strategy class, the first parameter before the first pipe char:
	auto idxPipe = a_StrategyDesc.find('|');
	if (idxPipe == AString::npos)
	{
		idxPipe = a_StrategyDesc.length();
	}
	AString StrategyClass = a_StrategyDesc.substr(0, idxPipe);

	// Create a strategy class based on the class string:
	cPiece::cVerticalStrategyPtr Strategy;
	if (NoCaseCompare(StrategyClass, "Fixed") == 0)
	{
		Strategy = std::make_shared<cVerticalStrategyFixed>();
	}
	else if (NoCaseCompare(StrategyClass, "Range") == 0)
	{
		Strategy = std::make_shared<cVerticalStrategyRange>();
	}
	else if (NoCaseCompare(StrategyClass, "TerrainTop") == 0)
	{
		Strategy = std::make_shared<cVerticalStrategyTerrainTop>();
	}
	else if (NoCaseCompare(StrategyClass, "TerrainOrOceanTop") == 0)
	{
		Strategy = std::make_shared<cVerticalStrategyTerrainOrOceanTop>();
	}
	else
	{
		return nullptr;
	}

	// Initialize the strategy's parameters:
	AString Params;
	if (idxPipe < a_StrategyDesc.length())
	{
		Params = a_StrategyDesc.substr(idxPipe + 1);
	}
	if (!Strategy->InitializeFromString(Params, a_LogWarnings))
	{
		return nullptr;
	}

	return Strategy;
}




