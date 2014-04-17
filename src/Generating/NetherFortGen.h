
// NetherFortGen.h

// Declares the cNetherFortGen class representing the nether fortress generator





#pragma once

#include "ComposableGenerator.h"
#include "PieceGenerator.h"





class cNetherFortGen :
	public cFinishGen,
	public cPiecePool
{
public:
	cNetherFortGen(int a_Seed, int a_GridSize, int a_MaxDepth);
	
	virtual ~cNetherFortGen();
	
protected:
	class cNetherFort;  // fwd: NetherFortGen.cpp
	typedef std::list<cNetherFort *> cNetherForts;
	
	
	/** The seed used for generating*/
	int m_Seed;
	
	/** The noise used for generating */
	cNoise m_Noise;
	
	/** Average spacing between the fortresses*/
	int m_GridSize;
	
	/** Maximum depth of the piece-generator tree */
	int m_MaxDepth;

	/** Cache of the most recently used systems. MoveToFront used. */
	cNetherForts m_Cache;
	
	/** All the pieces that are allowed for building.
	This is the list that's used for memory allocation and deallocation for the pieces. */
	cPieces m_AllPieces;
	
	/** The pieces that are used as starting pieces.
	This list is not shared and the pieces need deallocation. */
	cPieces m_StartingPieces;
	
	/** The pieces that have an "outer" connector.
	The pieces are copies out of m_AllPieces and shouldn't be ever delete-d. */
	cPieces m_OuterPieces;

	/** The pieces that have an "inner" connector.
	The pieces are copies out of m_AllPieces and shouldn't be ever delete-d. */
	cPieces m_InnerPieces;


	/** Clears everything from the cache.
	Also invalidates the forst returned by GetFortsForChunk(). */
	void ClearCache(void);
	
	/** Returns all forts that *may* intersect the given chunk.
	The returned forts live within m_Cache.They are valid until the next call
	to this function (which may delete some of the pointers).	*/
	void GetFortsForChunk(int a_ChunkX, int a_ChunkZ, cNetherForts & a_Forts);

	// cFinishGen overrides:
	virtual void GenFinish(cChunkDesc & a_ChunkDesc) override;
	
	// cPiecePool overrides:
	virtual cPieces GetPiecesWithConnector(int a_ConnectorType) override;
	virtual cPieces GetStartingPieces(void) override;
	virtual int GetPieceWeight(const cPlacedPiece & a_PlacedPiece, const cPiece::cConnector & a_ExistingConnector, const cPiece & a_NewPiece) override;
	virtual void PiecePlaced(const cPiece & a_Piece) override;
	virtual void Reset(void) override;
} ;




