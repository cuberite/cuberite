
// PrefabPiecePool.cpp

// Implements the cPrefabPiecePool class that represents a cPiecePool descendant that uses cPrefab instances as the pieces

#include "Globals.h"
#include "PrefabPiecePool.h"
#include "VerticalStrategy.h"
#include "../Bindings/LuaState.h"
#include "../WorldStorage/SchematicFileSerializer.h"
#include "../StringCompression.h"





// Conditionally log a warning
#define CONDWARNING(ShouldLog, ...) \
	do { \
		if (ShouldLog) \
		{ \
			LOGWARNING(__VA_ARGS__); \
		} \
	} while (false)





/** Returns the map of string => eMergeStrategy used when translating cubeset file merge strategies. */
static std::map<AString, cBlockArea::eMergeStrategy> & GetMergeStrategyMap(void)
{
	static std::map<AString, cBlockArea::eMergeStrategy> msmap;
	if (msmap.empty())
	{
		// This is the first use, initialize the map:
		msmap["msOverwrite"]     = cBlockArea::msOverwrite;
		msmap["msFillAir"]       = cBlockArea::msFillAir;
		msmap["msImprint"]       = cBlockArea::msImprint;
		msmap["msLake"]          = cBlockArea::msLake;
		msmap["msSpongePrint"]   = cBlockArea::msSpongePrint;
		msmap["msDifference"]    = cBlockArea::msDifference;
		msmap["msSimpleCompare"] = cBlockArea::msSimpleCompare;
		msmap["msMask"]          = cBlockArea::msMask;
	}
	return msmap;
}





////////////////////////////////////////////////////////////////////////////////
// cPrefabPiecePool:

cPrefabPiecePool::cPrefabPiecePool(void)
{
}





cPrefabPiecePool::cPrefabPiecePool(
	const cPrefab::sDef * a_PieceDefs,         size_t a_NumPieceDefs,
	const cPrefab::sDef * a_StartingPieceDefs, size_t a_NumStartingPieceDefs,
	int a_DefaultStartingPieceHeight
)
{
	AddPieceDefs(a_PieceDefs, a_NumPieceDefs);
	if (a_StartingPieceDefs != nullptr)
	{
		AddStartingPieceDefs(a_StartingPieceDefs, a_NumStartingPieceDefs, a_DefaultStartingPieceHeight);
	}
}





cPrefabPiecePool::cPrefabPiecePool(const AString & a_FileName, bool a_LogWarnings)
{
	LoadFromFile(a_FileName, a_LogWarnings);
}





cPrefabPiecePool::~cPrefabPiecePool()
{
	Clear();
}





void cPrefabPiecePool::Clear(void)
{
	m_PiecesByConnector.clear();
	for (cPieces::iterator itr = m_AllPieces.begin(), end = m_AllPieces.end(); itr != end; ++itr)
	{
		delete *itr;
	}
	m_AllPieces.clear();
	for (cPieces::iterator itr = m_StartingPieces.begin(), end = m_StartingPieces.end(); itr != end; ++itr)
	{
		delete *itr;
	}
	m_StartingPieces.clear();
}





void cPrefabPiecePool::AddPieceDefs(const cPrefab::sDef * a_PieceDefs, size_t a_NumPieceDefs)
{
	ASSERT(a_PieceDefs != nullptr);
	for (size_t i = 0; i < a_NumPieceDefs; i++)
	{
		cPrefab * Prefab = new cPrefab(a_PieceDefs[i]);
		m_AllPieces.push_back(Prefab);
		AddToPerConnectorMap(Prefab);
	}
}





void cPrefabPiecePool::AddStartingPieceDefs(
	const cPrefab::sDef * a_StartingPieceDefs,
	size_t a_NumStartingPieceDefs,
	int a_DefaultPieceHeight
)
{
	ASSERT(a_StartingPieceDefs != nullptr);
	auto verticalStrategy = CreateVerticalStrategyFromString(Printf("Fixed|%d", a_DefaultPieceHeight), false);
	for (size_t i = 0; i < a_NumStartingPieceDefs; i++)
	{
		cPrefab * Prefab = new cPrefab(a_StartingPieceDefs[i]);
		if (a_DefaultPieceHeight >= 0)
		{
			Prefab->SetVerticalStrategy(verticalStrategy);
		}
		m_StartingPieces.push_back(Prefab);
	}
}





bool cPrefabPiecePool::LoadFromFile(const AString & a_FileName, bool a_LogWarnings)
{
	// Read the file into a string buffer, load from string:
	auto contents = cFile::ReadWholeFile(a_FileName);
	if (contents.empty())
	{
		CONDWARNING(a_LogWarnings, "Cannot read data from file %s", a_FileName.c_str());
		return false;
	}
	return LoadFromString(contents, a_FileName, a_LogWarnings);
}





bool cPrefabPiecePool::LoadFromString(const AString & a_Contents, const AString & a_FileName, bool a_LogWarnings)
{
	// If the contents start with GZip signature, ungzip and retry:
	if (a_Contents.substr(0, 3) == "\x1f\x8b\x08")
	{
		AString Uncompressed;
		auto res = UncompressStringGZIP(a_Contents.data(), a_Contents.size(), Uncompressed);
		if (res == Z_OK)
		{
			return LoadFromString(Uncompressed, a_FileName, a_LogWarnings);
		}
		else
		{
			CONDWARNING(a_LogWarnings, "Failed to decompress Gzip data in file %s: %d", a_FileName.c_str(), res);
			return false;
		}
	}

	// Search the first 8 KiB of the file for the format auto-detection string:
	auto Header = a_Contents.substr(0, 8192);
	if (Header.find("CubesetFormatVersion =") != AString::npos)
	{
		return LoadFromCubeset(a_Contents, a_FileName, a_LogWarnings);
	}
	CONDWARNING(a_LogWarnings, "Cannot load prefabs from file %s, unknown file format", a_FileName.c_str());
	return false;
}





bool cPrefabPiecePool::LoadFromCubeset(const AString & a_Contents, const AString & a_FileName, bool a_LogWarnings)
{
	// Load the file in the Lua interpreter:
	cLuaState Lua(Printf("LoadablePiecePool %s", a_FileName.c_str()));
	Lua.Create();
	cLuaState::cLock lock(Lua);
	if (!Lua.LoadString(a_Contents, a_FileName, a_LogWarnings))
	{
		// Reason for failure has already been logged in LoadFile()
		return false;
	}

	// Check the version:
	int Version = 0;
	if (!Lua.GetNamedGlobal("Cubeset.Metadata.CubesetFormatVersion", Version))
	{
		CONDWARNING(a_LogWarnings, "Cannot load cubeset %s, it doesn't contain version information.", a_FileName.c_str());
		return false;
	}

	// Load the data, using the correct version loader:
	if (Version == 1)
	{
		return LoadFromCubesetVer1(a_FileName, Lua, a_LogWarnings);
	}

	// Unknown version:
	CONDWARNING(a_LogWarnings, "Cannot load cubeset %s, version (%d) not supported.", a_FileName.c_str(), Version);
	return false;
}





void cPrefabPiecePool::AddToPerConnectorMap(cPrefab * a_Prefab)
{
	cPiece::cConnectors Connectors = (static_cast<const cPiece *>(a_Prefab))->GetConnectors();
	for (cPiece::cConnectors::const_iterator itr = Connectors.begin(), end = Connectors.end(); itr != end; ++itr)
	{
		m_PiecesByConnector[itr->m_Type].push_back(a_Prefab);
	}
}





bool cPrefabPiecePool::LoadFromCubesetVer1(const AString & a_FileName, cLuaState & a_LuaState, bool a_LogWarnings)
{
	// Load the metadata and apply the known ones:
	ReadPoolMetadataCubesetVer1(a_FileName, a_LuaState, a_LogWarnings);
	ApplyBaseMetadataCubesetVer1(a_FileName, a_LogWarnings);

	// Push the Cubeset.Pieces global value on the stack:
	auto pieces = a_LuaState.WalkToNamedGlobal("Cubeset.Pieces");
	if (!pieces.IsValid() || !lua_istable(a_LuaState, -1))
	{
		CONDWARNING(a_LogWarnings, "The cubeset file %s doesn't contain any pieces", a_FileName.c_str());
		return false;
	}

	// Iterate over all items in the Cubeset.Pieces value:
	int idx = 1;
	bool res = true;
	while (true)
	{
		lua_pushinteger(a_LuaState, idx);  // stk: [Pieces] [idx]
		lua_gettable(a_LuaState, -2);      // stk: [Pieces] [PieceItem]
		if (!lua_istable(a_LuaState, -1))
		{
			// The PieceItem is not present, we've iterated over all items
			lua_pop(a_LuaState, 1);  // stk: [Pieces]
			break;
		}
		if (!LoadCubesetPieceVer1(a_FileName, a_LuaState, idx, a_LogWarnings))
		{
			res = false;
		}
		lua_pop(a_LuaState, 1);  // stk: [Pieces]
		idx += 1;
	}
	return res;
}





bool cPrefabPiecePool::LoadCubesetPieceVer1(const AString & a_FileName, cLuaState & a_LuaState, int a_PieceIndex, bool a_LogWarnings)
{
	ASSERT(lua_istable(a_LuaState, -1));

	// The piece name is optional, but useful for debugging messages:
	AString PieceName;
	if (!a_LuaState.GetNamedValue("OriginData.ExportName", PieceName))
	{
		Printf(PieceName, "Piece #%d", a_PieceIndex);
	}

	// Read the hitbox dimensions:
	cCuboid Hitbox;
	if (
		!a_LuaState.GetNamedValue("Hitbox.MinX", Hitbox.p1.x) ||
		!a_LuaState.GetNamedValue("Hitbox.MinY", Hitbox.p1.y) ||
		!a_LuaState.GetNamedValue("Hitbox.MinZ", Hitbox.p1.z) ||
		!a_LuaState.GetNamedValue("Hitbox.MaxX", Hitbox.p2.x) ||
		!a_LuaState.GetNamedValue("Hitbox.MaxY", Hitbox.p2.y) ||
		!a_LuaState.GetNamedValue("Hitbox.MaxZ", Hitbox.p2.z)
	)
	{
		CONDWARNING(a_LogWarnings, "Cannot load piece %s from file %s, it's missing hitbox information", PieceName.c_str(), a_FileName.c_str());
		return false;
	}

	// Load the prefab data:
	auto prefab = LoadPrefabFromCubesetVer1(a_FileName, a_LuaState, PieceName, a_LogWarnings);
	if (prefab == nullptr)
	{
		return false;
	}
	prefab->SetHitBox(Hitbox);

	// Read the connectors
	if (!ReadConnectorsCubesetVer1(a_FileName, a_LuaState, PieceName, prefab.get(), a_LogWarnings))
	{
		return false;
	}

	// Read the allowed rotations. It is an optional metadata value, default to 0:
	int AllowedRotations = 0;
	a_LuaState.GetNamedValue("Metadata.AllowedRotations", AllowedRotations);
	prefab->SetAllowedRotations(AllowedRotations);

	// Read the relevant metadata for the piece:
	if (!ReadPieceMetadataCubesetVer1(a_FileName, a_LuaState, PieceName, prefab.get(), a_LogWarnings))
	{
		return false;
	}

	// If the piece is a starting piece, check that it has a vertical strategy:
	int IsStartingPiece = 0;
	a_LuaState.GetNamedValue("Metadata.IsStarting", IsStartingPiece);
	if (IsStartingPiece != 0)
	{
		if (prefab->GetVerticalStrategy() == nullptr)
		{
			CONDWARNING(a_LogWarnings, "Starting prefab %s in file %s doesn't have its VerticalStrategy set. Setting to Fixed|150.",
				PieceName.c_str(), a_FileName.c_str()
			);
			VERIFY(prefab->SetVerticalStrategyFromString("Fixed|150", false));
		}
	}

	// Add the prefab into the list of pieces:
	if (IsStartingPiece != 0)
	{
		m_StartingPieces.push_back(prefab.release());
	}
	else
	{
		auto p = prefab.release();
		m_AllPieces.push_back(p);
		AddToPerConnectorMap(p);
	}

	return true;
}





std::unique_ptr<cPrefab> cPrefabPiecePool::LoadPrefabFromCubesetVer1(
	const AString & a_FileName,
	cLuaState & a_LuaState,
	const AString & a_PieceName,
	bool a_LogWarnings
)
{
	// First try loading a referenced schematic file, if any:
	AString SchematicFileName;
	if (a_LuaState.GetNamedValue("SchematicFileName", SchematicFileName))
	{
		auto PathEnd = a_FileName.find_last_of("/\\");  // Find the last path separator
		if (PathEnd != AString::npos)
		{
			SchematicFileName = a_FileName.substr(0, PathEnd) + SchematicFileName;
		}
		cBlockArea area;
		if (!cSchematicFileSerializer::LoadFromSchematicFile(area, SchematicFileName))
		{
			CONDWARNING(a_LogWarnings, "Cannot load schematic file \"%s\" for piece %s in cubeset %s.",
				SchematicFileName.c_str(), a_PieceName.c_str(), a_FileName.c_str()
			);
			return nullptr;
		}
		return std::make_unique<cPrefab>(area);
	}  // if (SchematicFileName)

	// There's no referenced schematic file, load from BlockDefinitions / BlockData.
	// Get references to the data and the table.concat function:
	cLuaState::cRef TableConcat, BlockDefinitions, BlockData;
	if (
		!a_LuaState.GetNamedGlobal("table.concat", TableConcat) ||
		!a_LuaState.GetNamedValue("BlockDefinitions", BlockDefinitions) ||
		!a_LuaState.GetNamedValue("BlockData", BlockData)
	)
	{
		CONDWARNING(a_LogWarnings, "Cannot parse block data for piece %s in cubeset %s", a_PieceName.c_str(), a_FileName.c_str());
		return nullptr;
	}

	// Call table.concat() on the BlockDefinitions:
	AString BlockDefStr;
	if (!a_LuaState.Call(TableConcat, BlockDefinitions, "\n", cLuaState::Return, BlockDefStr))
	{
		CONDWARNING(a_LogWarnings, "Cannot concat block definitions for piece %s in cubeset %s", a_PieceName.c_str(), a_FileName.c_str());
		return nullptr;
	}

	// Call table.concat() on the BlockData:
	AString BlockDataStr;
	if (!a_LuaState.Call(TableConcat, BlockData, "", cLuaState::Return, BlockDataStr))
	{
		CONDWARNING(a_LogWarnings, "Cannot concat block data for piece %s in cubeset %s", a_PieceName.c_str(), a_FileName.c_str());
		return nullptr;
	}

	// Read the size:
	int SizeX = 0, SizeY = 0, SizeZ = 0;
	if (
		!a_LuaState.GetNamedValue("Size.x", SizeX) ||
		!a_LuaState.GetNamedValue("Size.y", SizeY) ||
		!a_LuaState.GetNamedValue("Size.z", SizeZ)
	)
	{
		CONDWARNING(a_LogWarnings, "Cannot load piece %s from file %s, its size information is missing", a_PieceName.c_str(), a_FileName.c_str());
		return nullptr;
	}

	// Check that the size matches the data length:
	if (static_cast<size_t>(SizeX * SizeY * SizeZ) != BlockDataStr.size())
	{
		CONDWARNING(a_LogWarnings, "Cannot create piece %s from file %s, its size (%d) doesn't match the blockdata length (%u)",
			a_PieceName.c_str(), a_FileName.c_str(),
			SizeX * SizeY * SizeZ, static_cast<unsigned>(BlockDataStr.size())
		);
		return nullptr;
	}

	return std::make_unique<cPrefab>(BlockDefStr, BlockDataStr, SizeX, SizeY, SizeZ);
}





bool cPrefabPiecePool::ReadConnectorsCubesetVer1(
	const AString & a_FileName,
	cLuaState & a_LuaState,
	const AString & a_PieceName,
	cPrefab * a_Prefab,
	bool a_LogWarnings
)
{
	// Get the Connectors subtable:
	auto conns = a_LuaState.WalkToValue("Connectors");
	if (!conns.IsValid())
	{
		CONDWARNING(a_LogWarnings, "Cannot load piece %s from file %s, it has no connectors definition.", a_PieceName.c_str(), a_FileName.c_str());
		return false;
	}

	// Iterate over all items in the Connectors table:
	int idx = 1;
	bool res = true;
	while (true)
	{
		lua_pushinteger(a_LuaState, idx);  // stk: [Connectors] [idx]
		lua_gettable(a_LuaState, -2);      // stk: [Connectors] [conn]
		if (!lua_istable(a_LuaState, -1))
		{
			// The connector is not present, we've iterated over all items
			lua_pop(a_LuaState, 1);  // stk: [Connectors]
			break;
		}
		int Type = 0, RelX = 0, RelY = 0, RelZ = 0;
		AString DirectionStr;
		cPiece::cConnector::eDirection Direction = cPiece::cConnector::dirYM;
		if (
			!a_LuaState.GetNamedValue("Type",      Type) ||
			!a_LuaState.GetNamedValue("RelX",      RelX) ||
			!a_LuaState.GetNamedValue("RelY",      RelY) ||
			!a_LuaState.GetNamedValue("RelZ",      RelZ) ||
			!a_LuaState.GetNamedValue("Direction", DirectionStr) ||
			!cPiece::cConnector::StringToDirection(DirectionStr, Direction)
		)
		{
			if (a_LogWarnings)
			{
				FLOGWARNING("Piece {0} in file {1} has a malformed Connector at index {2} ({3}, type {4}, direction {5}). Skipping the connector.",
					a_PieceName, a_FileName, idx, Vector3i{RelX, RelY, RelZ}, Type, DirectionStr
				);
			}
			res = false;
			lua_pop(a_LuaState, 1);  // stk: [Connectors]
			idx += 1;
			continue;
		}
		a_Prefab->AddConnector(RelX, RelY, RelZ, Direction, Type);
		lua_pop(a_LuaState, 1);  // stk: [Connectors]
		idx += 1;
	}
	return res;
}





bool cPrefabPiecePool::ReadPieceMetadataCubesetVer1(
	const AString & a_FileName,
	cLuaState & a_LuaState,
	const AString & a_PieceName,
	cPrefab * a_Prefab,
	bool a_LogWarnings
)
{
	// Push the Metadata table on top of the Lua stack:
	auto md = a_LuaState.WalkToValue("Metadata");
	if (!md.IsValid())
	{
		return false;
	}

	// Get the values:
	int AddWeightIfSame = 0, DefaultWeight = 100, MoveToGround = 0;
	AString DepthWeight, MergeStrategy, VerticalLimit, VerticalStrategy;
	a_LuaState.GetNamedValue("AddWeightIfSame",  AddWeightIfSame);
	a_LuaState.GetNamedValue("DefaultWeight",    DefaultWeight);
	a_LuaState.GetNamedValue("DepthWeight",      DepthWeight);
	a_LuaState.GetNamedValue("MergeStrategy",    MergeStrategy);
	a_LuaState.GetNamedValue("MoveToGround",     MoveToGround);
	a_LuaState.GetNamedValue("VerticalLimit",    VerticalLimit);
	a_LuaState.GetNamedValue("VerticalStrategy", VerticalStrategy);

	// Apply the values:
	a_Prefab->SetAddWeightIfSame(AddWeightIfSame);
	a_Prefab->SetDefaultWeight(DefaultWeight);
	a_Prefab->ParseDepthWeight(DepthWeight.c_str());
	auto msmap = GetMergeStrategyMap();
	auto strategy = msmap.find(MergeStrategy);
	if (strategy == msmap.end())
	{
		CONDWARNING(a_LogWarnings, "Unknown merge strategy (\"%s\") specified for piece %s in file %s. Using msSpongePrint instead.",
			MergeStrategy.c_str(), a_PieceName.c_str(), a_FileName.c_str()
		);
		a_Prefab->SetMergeStrategy(cBlockArea::msSpongePrint);
	}
	else
	{
		a_Prefab->SetMergeStrategy(strategy->second);
	}
	a_Prefab->SetMoveToGround(MoveToGround != 0);

	AString ExpandFloorStrategyStr;
	if (!a_LuaState.GetNamedValue("ExpandFloorStrategy", ExpandFloorStrategyStr))
	{
		// Check the older variant for ExpandFloorStrategy, ShouldExpandFloor:
		int ShouldExpandFloor;
		if (a_LuaState.GetNamedValue("ShouldExpandFloor", ShouldExpandFloor))
		{
			LOG("Piece \"%s\" in file \"%s\" is using the old \"ShouldExpandFloor\" attribute. Use the new \"ExpandFloorStrategy\" attribute instead for more options.",
				a_PieceName.c_str(), a_FileName.c_str()
			);
			a_Prefab->SetExtendFloorStrategy((ShouldExpandFloor != 0) ? cPrefab::efsRepeatBottomTillNonAir : cPrefab::efsNone);
		}
	}
	else
	{
		auto lcExpandFloorStrategyStr = StrToLower(ExpandFloorStrategyStr);
		if (lcExpandFloorStrategyStr == "repeatbottomtillnonair")
		{
			a_Prefab->SetExtendFloorStrategy(cPrefab::efsRepeatBottomTillNonAir);
		}
		else if (lcExpandFloorStrategyStr == "repeatbottomtillsolid")
		{
			a_Prefab->SetExtendFloorStrategy(cPrefab::efsRepeatBottomTillSolid);
		}
		else
		{
			if (lcExpandFloorStrategyStr != "none")
			{
				LOGWARNING("Piece \"%s\" in file \"%s\" is using an unknown \"ExpandFloorStrategy\" attribute value: \"%s\"",
					a_PieceName.c_str(), a_FileName.c_str(), ExpandFloorStrategyStr.c_str()
				);
			}
			a_Prefab->SetExtendFloorStrategy(cPrefab::efsNone);
		}
	}
	if (!VerticalLimit.empty())
	{
		if (!a_Prefab->SetVerticalLimitFromString(VerticalLimit, a_LogWarnings))
		{
			CONDWARNING(a_LogWarnings, "Unknown VerticalLimit (\"%s\") specified for piece %s in file %s. Using no limit instead.",
				VerticalLimit.c_str(), a_PieceName.c_str(), a_FileName.c_str()
			);
		}
	}
	a_Prefab->SetVerticalStrategyFromString(VerticalStrategy, a_LogWarnings);

	return true;
}





void cPrefabPiecePool::ApplyBaseMetadataCubesetVer1(
	const AString & a_FileName,
	bool a_LogWarnings
)
{
	// Set the metadata values to defaults:
	m_MinDensity = 100;
	m_MaxDensity = 100;
	m_VillageRoadBlockType = E_BLOCK_GRAVEL;
	m_VillageRoadBlockMeta = 0;
	m_VillageWaterRoadBlockType = E_BLOCK_PLANKS;
	m_VillageWaterRoadBlockMeta = 0;

	// Read the metadata values:
	m_IntendedUse = GetMetadata("IntendedUse");
	GetStringMapInteger(m_Metadata, "MaxDensity",                m_MaxDensity);
	GetStringMapInteger(m_Metadata, "MinDensity",                m_MinDensity);
	GetStringMapInteger(m_Metadata, "VillageRoadBlockType",      m_VillageRoadBlockType);
	GetStringMapInteger(m_Metadata, "VillageRoadBlockMeta",      m_VillageRoadBlockMeta);
	GetStringMapInteger(m_Metadata, "VillageWaterRoadBlockType", m_VillageWaterRoadBlockType);
	GetStringMapInteger(m_Metadata, "VillageWaterRoadBlockMeta", m_VillageWaterRoadBlockMeta);

	// Read the allowed biomes:
	AString allowedBiomes = GetMetadata("AllowedBiomes");
	if (allowedBiomes.empty())
	{
		// All biomes are allowed:
		for (int b = biFirstBiome; b <= biMaxBiome; b++)
		{
			m_AllowedBiomes.insert(static_cast<EMCSBiome>(b));
		}
		for (int b = biFirstVariantBiome; b <= biMaxVariantBiome; b++)
		{
			m_AllowedBiomes.insert(static_cast<EMCSBiome>(b));
		}
	}
	else
	{
		auto biomes = StringSplitAndTrim(allowedBiomes, ",");
		for (const auto & biome: biomes)
		{
			EMCSBiome b = StringToBiome(biome);
			if (b == biInvalidBiome)
			{
				CONDWARNING(a_LogWarnings, "Invalid biome (\"%s\") specified in AllowedBiomes in cubeset file %s. Skipping the biome.",
					biome.c_str(), a_FileName.c_str()
				);
				continue;
			}
			m_AllowedBiomes.insert(b);
		}
	}
}





bool cPrefabPiecePool::ReadPoolMetadataCubesetVer1(
	const AString & a_FileName,
	cLuaState & a_LuaState,
	bool a_LogWarnings
)
{
	// Push the Cubeset.Metadata table on top of the Lua stack:
	auto gp = a_LuaState.WalkToNamedGlobal("Cubeset.Metadata");
	if (!gp.IsValid())
	{
		return true;
	}

	// Iterate over elements in the table, put them into the m_GeneratorParams map:
	lua_pushnil(a_LuaState);  // Table is at index -2, starting key (nil) at index -1
	while (lua_next(a_LuaState, -2) != 0)
	{
		// Table at index -3, key at index -2, value at index -1
		AString key, val;
		a_LuaState.GetStackValues(-2, key, val);
		m_Metadata[key] = val;
		lua_pop(a_LuaState, 1);  // Table at index -2, key at index -1
	}
	return true;
}





AString cPrefabPiecePool::GetMetadata(const AString & a_ParamName) const
{
	auto itr = m_Metadata.find(a_ParamName);
	if (itr == m_Metadata.end())
	{
		return AString();
	}
	return itr->second;
}





void cPrefabPiecePool::AssignGens(int a_Seed, cBiomeGenPtr & a_BiomeGen, cTerrainHeightGenPtr & a_HeightGen, int a_SeaLevel)
{
	// Assign the generator linkage to all starting pieces' VerticalStrategies:
	for (auto & piece: m_StartingPieces)
	{
		auto verticalStrategy = piece->GetVerticalStrategy();
		if (verticalStrategy != nullptr)
		{
			verticalStrategy->AssignGens(a_Seed, a_BiomeGen, a_HeightGen, a_SeaLevel);
		}
	}  // for piece - m_StartingPieces[]

	// Assign the generator linkage to all pieces' VerticalLimits:
	for (auto & piece: m_AllPieces)
	{
		auto verticalLimit = piece->GetVerticalLimit();
		if (verticalLimit != nullptr)
		{
			verticalLimit->AssignGens(a_Seed, a_BiomeGen, a_HeightGen, a_SeaLevel);
		}
	}  // for piece - m_AllPieces[]
}





cPieces cPrefabPiecePool::GetPiecesWithConnector(int a_ConnectorType)
{
	return m_PiecesByConnector[a_ConnectorType];
}





cPieces cPrefabPiecePool::GetStartingPieces(void)
{
	if (m_StartingPieces.empty())
	{
		return m_AllPieces;
	}
	else
	{
		return m_StartingPieces;
	}
}





int cPrefabPiecePool::GetPieceWeight(const cPlacedPiece & a_PlacedPiece, const cPiece::cConnector & a_ExistingConnector, const cPiece & a_NewPiece)
{
	return (static_cast<const cPrefab &>(a_NewPiece)).GetPieceWeight(a_PlacedPiece, a_ExistingConnector);
}





int cPrefabPiecePool::GetStartingPieceWeight(const cPiece & a_NewPiece)
{
	return (static_cast<const cPrefab &>(a_NewPiece)).GetDefaultWeight();
}





void cPrefabPiecePool::PiecePlaced(const cPiece & a_Piece)
{
	// Do nothing
	UNUSED(a_Piece);
}





void cPrefabPiecePool::Reset(void)
{
	// Do nothing
}




