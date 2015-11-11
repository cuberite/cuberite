
// PrefabPiecePool.h

// Declares the cPrefabPiecePool class that represents a cPiecePool descendant that uses cPrefab instances as the pieces





#pragma once

#include <unordered_set>
#include "PieceGenerator.h"
#include "Prefab.h"





// fwd:
class cLuaState;





class cPrefabPiecePool :
	public cPiecePool
{
public:
	/** Creates an empty instance. Prefabs can be added by calling AddPieceDefs() and AddStartingPieceDefs(). */
	cPrefabPiecePool(void);
	
	/** Creates a piece pool with prefabs from the specified definitions.
	If both a_PieceDefs and a_StartingPieceDefs are given, only the a_StartingPieceDefs are used as starting
	pieces for the pool, and they do not participate in the generation any further.
	If only a_PieceDefs is given, any such piece can be chosen as a starting piece, and all the pieces are used
	for generating.
	More pieces can be added to the instance afterwards by calling AddPieceDefs() and AddStartingPieceDefs().
	If a_DefaultStartingPieceHeight is non-negative, it is applied to each starting piece as its fixed
	height (for the GetStartingPieceHeight() call). */
	cPrefabPiecePool(
		const cPrefab::sDef * a_PieceDefs,         size_t a_NumPieceDefs,
		const cPrefab::sDef * a_StartingPieceDefs, size_t a_NumStartingPieceDefs,
		int a_DefaultStartingPieceHeight = -1
	);
	
	/** Creates a pool and loads the contents of the specified file into it.
	If a_LogWarnings is true, logs a warning to console when loading fails. */
	cPrefabPiecePool(const AString & a_FileName, bool a_LogWarnings);

	/** Destroys the pool, freeing all pieces. */
	~cPrefabPiecePool();
	
	/** Removes and frees all pieces from this pool. */
	void Clear(void);
	
	/** Adds pieces from the specified definitions into m_AllPieces. Also adds the pieces into
	the m_PiecesByConnector map.
	May be called multiple times with different PieceDefs, will add all such pieces. */
	void AddPieceDefs(const cPrefab::sDef * a_PieceDefs, size_t a_NumPieceDefs);
	
	/** Adds pieces from the specified definitions into m_StartingPieces. Doesn't add them to
	the m_PiecesByConnector map.
	May be called multiple times with different PieceDefs, will add all such pieces.
	If a_DefaultPieceHeight is non-negative, it is applied to each piece as its fixed
	height (for the GetStartingPieceHeight() call). */
	void AddStartingPieceDefs(
		const cPrefab::sDef * a_StartingPieceDefs,
		size_t a_NumStartingPieceDefs,
		int a_DefaultPieceHeight = -1
	);
	
	/** Loads the pieces from the specified file. Returns true if successful, false on error.
	If a_LogWarnings is true, logs a warning to console when loading fails. */
	bool LoadFromFile(const AString & a_FileName, bool a_LogWarnings);

	/** Loads the pieces from the specified string.
	Returns true if successful, false on error.
	a_FileName is used only logging.
	If a_LogWarnings is true, logs a warning to console when loading fails. */
	bool LoadFromString(const AString & a_Contents, const AString & a_FileName, bool a_LogWarnings);

	/** Loads the pieces from the specified string containing Cubeset file data.
	Returns true if successful, false on error.
	a_FileName is used only logging.
	If a_LogWarnings is true, logs a warning to console when loading fails. */
	bool LoadFromCubeset(const AString & a_Contents, const AString & a_FileName, bool a_LogWarnings);

	/** Returns the number of regular (non-starting) pieces. */
	size_t GetAllPiecesCount(void) const { return m_AllPieces.size(); }

	/** Returns the number of starting pieces. */
	size_t GetStartingPiecesCount(void) const { return m_StartingPieces.size(); }

	// Metadata accessors:
	const AString & GetIntendedUse(void) const { return m_IntendedUse; }
	int GetMinDensity(void) const { return m_MinDensity; }
	int GetMaxDensity(void) const { return m_MaxDensity; }
	BLOCKTYPE  GetVillageRoadBlockType     (void) const { return m_VillageRoadBlockType; }
	NIBBLETYPE GetVillageRoadBlockMeta     (void) const { return m_VillageRoadBlockMeta; }
	BLOCKTYPE  GetVillageWaterRoadBlockType(void) const { return m_VillageWaterRoadBlockType; }
	NIBBLETYPE GetVillageWaterRoadBlockMeta(void) const { return m_VillageWaterRoadBlockMeta; }

	/** Returns true if a_Biome is among the accepted biomes in the m_AcceptedBiomes metadata member. */
	bool IsBiomeAllowed(EMCSBiome a_Biome) const { return (m_AllowedBiomes.find(a_Biome) != m_AllowedBiomes.end()); }

	/** Returns the specified value from the metadata map.
	Returns an empty string if no such value had been read. */
	AString GetMetadata(const AString & a_ParamName) const;

	const AStringMap & GetAllMetadata(void) const { return m_Metadata; }

	/** Called when the piece pool is assigned to a generator,
	so that the individual starting pieces' vertical strategies may bind to the underlying subgenerators. */
	void AssignGens(int a_Seed, cBiomeGenPtr & a_BiomeGen, cTerrainHeightGenPtr & a_HeightGen, int a_SeaLevel);

	// cPiecePool overrides:
	virtual cPieces GetPiecesWithConnector(int a_ConnectorType) override;
	virtual cPieces GetStartingPieces(void) override;
	virtual int GetPieceWeight(const cPlacedPiece & a_PlacedPiece, const cPiece::cConnector & a_ExistingConnector, const cPiece & a_NewPiece) override;
	virtual int GetStartingPieceWeight(const cPiece & a_NewPiece) override;
	virtual void PiecePlaced(const cPiece & a_Piece) override;
	virtual void Reset(void) override;

protected:

	/** The type used to map a connector type to the list of pieces with that connector */
	typedef std::map<int, cPieces> cPiecesMap;


	/** All the pieces that are allowed for building.
	This is the list that's used for memory allocation and deallocation for the pieces. */
	cPieces m_AllPieces;
	
	/** The pieces that are used as starting pieces.
	This list is not shared and the pieces need deallocation. */
	cPieces m_StartingPieces;
	
	/** The map that has all pieces by their connector types
	The pieces are copies out of m_AllPieces and shouldn't be ever delete-d. */
	cPiecesMap m_PiecesByConnector;

	/** The intended use of this piece pool, as specified by the pool's metadata. */
	AString m_IntendedUse;

	/** The minimum density, as read from the metadata. */
	int m_MinDensity;

	/** The maximum density, as read from the metadata. */
	int m_MaxDensity;

	/** The block type to use for the village roads. */
	BLOCKTYPE m_VillageRoadBlockType;

	/** The block meta to use for the village roads. */
	NIBBLETYPE m_VillageRoadBlockMeta;

	/** The block type used for the village roads if the road is on water. */
	BLOCKTYPE m_VillageWaterRoadBlockType;
	
	/** The block meta used for the village roads if the road is on water. */
	NIBBLETYPE m_VillageWaterRoadBlockMeta;

	/** A set of allowed  biomes for the pool. The pool will only be used within the specified biomes. */
	std::unordered_set<EMCSBiome, BiomeHasher> m_AllowedBiomes;

	/** A dictionary of pool-wide metadata, as read from the cubeset file. */
	AStringMap m_Metadata;


	/** Adds the prefab to the m_PiecesByConnector map for all its connectors. */
	void AddToPerConnectorMap(cPrefab * a_Prefab);

	/** Loads the pieces from the cubeset file parsed into the specified Lua state.
	Returns true on success, false on error.
	a_FileName is used only logging.
	If a_LogWarnings is true, logs a warning to console when loading fails. */
	bool LoadFromCubesetVer1(const AString & a_FileName, cLuaState & a_LuaState, bool a_LogWarnings);

	/** Loads a single piece from the cubeset file parsed into the specified Lua state.
	The piece's definition table is expected to be at the top of the Lua stack.
	Returns true on success, false on error.
	a_PieceIndex is the index of the piece, in the Pieces table. It is used for logging only.
	If a_LogWarnings is true, logs a warning to console when loading fails. */
	bool LoadCubesetPieceVer1(const AString & a_FileName, cLuaState & a_LuaState, int a_PieceIndex, bool a_LogWarnings);

	/** Loads a single piece's prefab from the cubeset file parsed into the specified Lua state.
	The piece's definition table is expected to be at the top of the Lua stack.
	Returns the prefab on success, nullptr on failure.
	a_PieceName is the identification of the piece, used for logging only.
	If a_LogWarnings is true, logs a warning to console when loading fails. */
	UniquePtr<cPrefab> LoadPrefabFromCubesetVer1(
		const AString & a_FileName,
		cLuaState & a_LuaState,
		const AString & a_PieceName,
		bool a_LogWarnings
	);
	
	/** Reads a single piece's connectors from the cubeset file parsed into the specified Lua state.
	The piece's definition table is expected to be at the top of the Lua stack.
	Returns true on success, false on failure.
	The connectors are added into the a_Prefab object.
	No Connectors table is considered a failure, empty Connectors table is considered a success.
	If any of the connectors are malformed, it is considered a failure, although the rest of the connectors will still load.
	a_PieceName is the identification of the piece, used for logging only.
	If a_LogWarnings is true, logs a warning to console when loading fails. */
	bool ReadConnectorsCubesetVer1(
		const AString & a_FileName,
		cLuaState & a_LuaState,
		const AString & a_PieceName,
		cPrefab * a_Prefab,
		bool a_LogWarnings
	);

	/** Reads a single piece's metadata from the cubeset file parsed into the specified Lua state.
	The piece's definition table is expected to be at the top of the Lua stack.
	Returns true on success, false on failure.
	The metadata is applied into the a_Prefab object.
	a_PieceName is the identification of the piece, used for logging only.
	If a_LogWarnings is true, logs a warning to console when loading fails. */
	bool ReadPieceMetadataCubesetVer1(
		const AString & a_FileName,
		cLuaState & a_LuaState,
		const AString & a_PieceName,
		cPrefab * a_Prefab,
		bool a_LogWarnings
	);

	/** Reads the metadata for the entire pool from the cubeset file, stores it in the m_Metadata map.
	Returns true on success, false on failure.
	If a_LogWarnings is true, logs a warning to console when loading fails. */
	bool ReadPoolMetadataCubesetVer1(
		const AString & a_FileName,
		cLuaState & a_LuaState,
		bool a_LogWarnings
	);

	/** Applies the base known metadata from the m_Metadata map into this pool.
	If a_LogWarnings is true, logs a warning to console when loading fails. */
	void ApplyBaseMetadataCubesetVer1(
		const AString & a_FileName,
		bool a_LogWarnings
	);
} ;




