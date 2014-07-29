
// PrefabPiecePool.h

// Declares the cPrefabPiecePool class that represents a cPiecePool descendant that uses cPrefab instances as the pieces





#pragma once

#include "PieceGenerator.h"
#include "Prefab.h"





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
	More pieces can be added to the instance afterwards by calling AddPieceDefs() and AddStartingPieceDefs(). */
	cPrefabPiecePool(
		const cPrefab::sDef * a_PieceDefs,         size_t a_NumPieceDefs,
		const cPrefab::sDef * a_StartingPieceDefs, size_t a_NumStartingPieceDefs
	);
	
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
	May be called multiple times with different PieceDefs, will add all such pieces. */
	void AddStartingPieceDefs(const cPrefab::sDef * a_StartingPieceDefs, size_t a_NumStartingPieceDefs);
	
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


	/** Adds the prefab to the m_PiecesByConnector map for all its connectors. */
	void AddToPerConnectorMap(cPrefab * a_Prefab);
	
	// cPiecePool overrides:
	virtual cPieces GetPiecesWithConnector(int a_ConnectorType) override;
	virtual cPieces GetStartingPieces(void) override;
	virtual int GetPieceWeight(const cPlacedPiece & a_PlacedPiece, const cPiece::cConnector & a_ExistingConnector, const cPiece & a_NewPiece) override;
	virtual int GetStartingPieceWeight(const cPiece & a_NewPiece) override;
	virtual void PiecePlaced(const cPiece & a_Piece) override;
	virtual void Reset(void) override;
} ;




