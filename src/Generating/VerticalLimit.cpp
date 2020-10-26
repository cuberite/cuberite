
// VerticalLimit.cpp

#include "Globals.h"
#include "VerticalLimit.h"





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

/** Parses a string containing a range in which both values are optional ("<MinHeight>|<MaxHeight>") into Min, Max.
Returns true if successful, false on failure.
If a_LogWarnings is true, outputs failure reasons to console.
The range is returned in a_Min and a_Max.
If no value is in the string, both values are left unchanged.
If only the minimum is in the string, it is assigned to both a_Min and a_Max. */
namespace VerticalLimit
{
	static bool ParseRange(const AString & a_Params, int & a_Min, int & a_Max, bool a_LogWarnings)
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
			a_Max = a_Min;
			return true;
		}
		if (!StringToInteger(params[1], a_Max))
		{
			CONDWARNING(a_LogWarnings, "Cannot parse maximum height from string \"%s\"!", params[1].c_str());
			return false;
		}
		if (a_Max < a_Min)
		{
			std::swap(a_Max, a_Min);
		}
		return true;
	}
}





////////////////////////////////////////////////////////////////////////////////
/** Limit that accepts any height. The default for all pieces. */
class cVerticalLimitNone:
	public cPiece::cVerticalLimit
{
public:
		virtual bool CanBeAtHeight(int a_BlockX, int a_BlockZ, int a_Height) override
		{
			// Any height is okay
			return true;
		}


		virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
		{
			// No parameters to read, no checks being done
			return true;
		}
};





////////////////////////////////////////////////////////////////////////////////
/** Limit that accepts heights above the specified minimum fixed height. */
class cVerticalLimitAbove:
	public cPiece::cVerticalLimit
{
public:
	virtual bool CanBeAtHeight(int a_BlockX, int a_BlockZ, int a_Height) override
	{
		return (a_Height >= m_MinHeight);
	}


	virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
	{
		// Parameters: "<MinHeight>", compulsory
		if (!StringToInteger(a_Params, m_MinHeight))
		{
			CONDWARNING(a_LogWarnings, "Cannot parse the minimum height from string \"%s\"!", a_Params.c_str());
			return false;
		}
		return true;
	}

protected:
	/** The minimum accepted height. */
	int m_MinHeight;
};





////////////////////////////////////////////////////////////////////////////////
/** Limit that accepts heights that are a specified number of blocks above terrain. */
class cVerticalLimitAboveTerrain:
	public cPiece::cVerticalLimit
{
public:
	virtual bool CanBeAtHeight(int a_BlockX, int a_BlockZ, int a_Height) override
	{
		auto terrainHeight = m_TerrainHeightGen->GetHeightAt(a_BlockX, a_BlockZ);
		int compareHeight = a_Height - terrainHeight;
		return (
			(compareHeight >= m_MinBlocksAbove) &&  // Above the minimum
			(compareHeight <= m_MaxBlocksAbove)     // and below the maximum
		);
	}


	virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
	{
		// Parameters: "<MinBlocksAbove>|<MaxBlocksAbove>", both optional
		m_MinBlocksAbove = 0;
		m_MaxBlocksAbove = 0;
		return VerticalLimit::ParseRange(a_Params, m_MinBlocksAbove, m_MaxBlocksAbove, a_LogWarnings);
	}


	virtual void AssignGens(int a_Seed, cBiomeGenPtr & a_BiomeGen, cTerrainHeightGenPtr & a_TerrainHeightGen, int a_SeaLevel) override
	{
		m_TerrainHeightGen = a_TerrainHeightGen;
	}

protected:
	/** The underlying height generator. */
	cTerrainHeightGenPtr m_TerrainHeightGen;

	/** How many blocks above the terrain level do we accept on minimum. */
	int m_MinBlocksAbove;

	/** How many blocks above the terrain level do we accept on maximum. */
	int m_MaxBlocksAbove;
};





////////////////////////////////////////////////////////////////////////////////
/** Limit that accepts heights that are a specified number of blocks above terrain and sealevel, whichever is higher. */
class cVerticalLimitAboveTerrainAndOcean:
	public cPiece::cVerticalLimit
{
public:
	virtual bool CanBeAtHeight(int a_BlockX, int a_BlockZ, int a_Height) override
	{
		ASSERT(m_TerrainHeightGen != nullptr);
		int terrainHeight = m_TerrainHeightGen->GetHeightAt(a_BlockX, a_BlockZ);
		int compareHeight = a_Height - std::max(terrainHeight, m_SeaLevel);
		return (
			(compareHeight >= m_MinBlocksAbove) &&  // Above the minimum
			(compareHeight <= m_MaxBlocksAbove)     // and below the maximum
		);
	}


	virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
	{
		// Parameters: "<MinBlocksAbove>|<MaxBlocksAbove>", both optional
		m_MinBlocksAbove = 0;
		m_MaxBlocksAbove = 0;
		return VerticalLimit::ParseRange(a_Params, m_MinBlocksAbove, m_MaxBlocksAbove, a_LogWarnings);
	}


	virtual void AssignGens(int a_Seed, cBiomeGenPtr & a_BiomeGen, cTerrainHeightGenPtr & a_TerrainHeightGen, int a_SeaLevel) override
	{
		m_TerrainHeightGen = a_TerrainHeightGen;
		m_SeaLevel = a_SeaLevel;
	}

protected:
	/** The underlying height generator. */
	cTerrainHeightGenPtr m_TerrainHeightGen;

	/** The sealevel for the current world. */
	int m_SeaLevel;

	/** How many blocks above the terrain level / ocean do we accept on minimum. */
	int m_MinBlocksAbove;

	/** How many blocks above the terrain level / ocean do we accept on maximum. */
	int m_MaxBlocksAbove;
};





////////////////////////////////////////////////////////////////////////////////
/** Limit that accepts heights below the specified fixed height.
NOTE that the query height is the BOTTOM of the piece. */
class cVerticalLimitBelow:
	public cPiece::cVerticalLimit
{
public:
	virtual bool CanBeAtHeight(int a_BlockX, int a_BlockZ, int a_Height) override
	{
		return (a_Height <= m_MaxHeight);
	}


	virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
	{
		// Parameters: "<MaxHeight>"
		if (!StringToInteger(a_Params, m_MaxHeight))
		{
			CONDWARNING(a_LogWarnings, "Cannot parse the maximum height from string \"%s\"!", a_Params.c_str());
			return false;
		}
		return true;
	}

protected:
	/** The maximum accepted height. */
	int m_MaxHeight;
};





////////////////////////////////////////////////////////////////////////////////
/** Limit that accepts heights that are within a specified range below terrain.
NOTE that the query height is the BOTTOM of the piece. */
class cVerticalLimitBelowTerrain:
	public cPiece::cVerticalLimit
{
public:
	virtual bool CanBeAtHeight(int a_BlockX, int a_BlockZ, int a_Height) override
	{
		auto terrainHeight = m_TerrainHeightGen->GetHeightAt(a_BlockX, a_BlockZ);
		auto compareHeight = terrainHeight - a_Height;
		return (
			(compareHeight >= m_MinBlocksBelow) &&
			(compareHeight <= m_MaxBlocksBelow)
		);
	}


	virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
	{
		// Parameters: "<MinBlocksBelow>|<MaxBlocksBelow>", both optional
		m_MinBlocksBelow = 0;
		m_MaxBlocksBelow = 0;
		return VerticalLimit::ParseRange(a_Params, m_MinBlocksBelow, m_MaxBlocksBelow, a_LogWarnings);
	}


	virtual void AssignGens(int a_Seed, cBiomeGenPtr & a_BiomeGen, cTerrainHeightGenPtr & a_TerrainHeightGen, int a_SeaLevel) override
	{
		m_TerrainHeightGen = a_TerrainHeightGen;
	}

protected:
	/** The underlying height generator. */
	cTerrainHeightGenPtr m_TerrainHeightGen;

	/** How many blocks below the terrain level do we accept on minimum. */
	int m_MinBlocksBelow;

	/** How many blocks below the terrain level do we accept on maximum. */
	int m_MaxBlocksBelow;
};





////////////////////////////////////////////////////////////////////////////////
/** Limit that accepts heights that are a specified number of blocks below terrain or sealevel, whichever is higher. */
class cVerticalLimitBelowTerrainOrOcean:
	public cPiece::cVerticalLimit
{
public:
	virtual bool CanBeAtHeight(int a_BlockX, int a_BlockZ, int a_Height) override
	{
		int terrainHeight = m_TerrainHeightGen->GetHeightAt(a_BlockX, a_BlockZ);
		auto compareHeight = std::max(terrainHeight, m_SeaLevel) - a_Height;
		return (
			(compareHeight >= m_MinBlocksBelow) &&
			(compareHeight <= m_MaxBlocksBelow)
		);
	}


	virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) override
	{
		// Parameters: "<MinBlocksBelow>|<MaxBlocksBelow>", both optional
		m_MinBlocksBelow = 0;
		m_MaxBlocksBelow = 0;
		return VerticalLimit::ParseRange(a_Params, m_MinBlocksBelow, m_MaxBlocksBelow, a_LogWarnings);
	}


	virtual void AssignGens(int a_Seed, cBiomeGenPtr & a_BiomeGen, cTerrainHeightGenPtr & a_TerrainHeightGen, int a_SeaLevel) override
	{
		m_TerrainHeightGen = a_TerrainHeightGen;
		m_SeaLevel = a_SeaLevel;
	}

protected:
	/** The underlying height generator. */
	cTerrainHeightGenPtr m_TerrainHeightGen;

	/** The sealevel for the current world. */
	int m_SeaLevel;

	/** How many blocks below the terrain level do we accept on minimum. */
	int m_MinBlocksBelow;

	/** How many blocks below the terrain level do we accept on maximum. */
	int m_MaxBlocksBelow;
};





////////////////////////////////////////////////////////////////////////////////
// CreateVerticalLimitFromString:

cPiece::cVerticalLimitPtr CreateVerticalLimitFromString(const AString & a_LimitDesc, bool a_LogWarnings)
{
	// Break apart the limit class, the first parameter before the first pipe char:
	auto idxPipe = a_LimitDesc.find('|');
	if (idxPipe == AString::npos)
	{
		idxPipe = a_LimitDesc.length();
	}
	AString LimitClass = a_LimitDesc.substr(0, idxPipe);

	// Create a strategy class based on the class string:
	cPiece::cVerticalLimitPtr Limit;
	if ((LimitClass == "") || (NoCaseCompare(LimitClass, "None") == 0))
	{
		Limit = std::make_shared<cVerticalLimitNone>();
	}
	else if (NoCaseCompare(LimitClass, "Above") == 0)
	{
		Limit = std::make_shared<cVerticalLimitAbove>();
	}
	else if (NoCaseCompare(LimitClass, "AboveTerrain") == 0)
	{
		Limit = std::make_shared<cVerticalLimitAboveTerrain>();
	}
	else if (NoCaseCompare(LimitClass, "AboveTerrainAndOcean") == 0)
	{
		Limit = std::make_shared<cVerticalLimitAboveTerrainAndOcean>();
	}
	else if (NoCaseCompare(LimitClass, "Below") == 0)
	{
		Limit = std::make_shared<cVerticalLimitBelow>();
	}
	else if (NoCaseCompare(LimitClass, "BelowTerrain") == 0)
	{
		Limit = std::make_shared<cVerticalLimitBelowTerrain>();
	}
	else if (NoCaseCompare(LimitClass, "BelowTerrainOrOcean") == 0)
	{
		Limit = std::make_shared<cVerticalLimitBelowTerrainOrOcean>();
	}
	else
	{
		return nullptr;
	}

	// Initialize the limit's parameters:
	AString Params;
	if (idxPipe < a_LimitDesc.length())
	{
		Params = a_LimitDesc.substr(idxPipe + 1);
	}
	if (!Limit->InitializeFromString(Params, a_LogWarnings))
	{
		return nullptr;
	}

	return Limit;
}




