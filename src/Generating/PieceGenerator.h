
// PieceGenerator.h

// Declares the cBFSPieceGenerator class and cDFSPieceGenerator class
// representing base classes for generating structures composed of individual "pieces"

/*
Each uses a slightly different approach to generating:
	- DFS extends pieces one by one until it hits the configured depth (or can't connect another piece anymore),
		then starts looking at adjacent connectors (like depth-first search).
	- BFS keeps a pool of currently-open connectors, chooses one at random and tries to place a piece on it,
		thus possibly extending the pool of open connectors (like breadth-first search).
*/





#pragma once

#include "ComposableGenerator.h"
#include "../Defines.h"
#include "../Cuboid.h"
#include "../Noise/Noise.h"





/** Represents a single piece. Can have multiple connectors of different types where other pieces can connect. */
class cPiece
{
public:
	// Force a virtual destructor in all descendants
	virtual ~cPiece() {}
	
	struct cConnector
	{
		/** Position relative to the piece */
		Vector3i m_Pos;
		
		/** Type of the connector. Any arbitrary number; the generator connects only connectors of opposite
		(negative) types. */
		int m_Type;
		
		/** Direction in which the connector is facing.
		Will be matched by the opposite direction for the connecting connector. */
		eBlockFace m_Direction;
		
		cConnector(int a_X, int a_Y, int a_Z, int a_Type, eBlockFace a_Direction);
		cConnector(const Vector3i & a_Pos, int a_Type, eBlockFace a_Direction);
	};

	typedef std::vector<cConnector> cConnectors;


	/** Base class (interface) for strategies for placing the starting pieces vertically.
	Descendants can override the GetVerticalPlacement() method to provide custom placement decisions. */
	class cVerticalStrategy
	{
	public:
		// Force a virtual destructor in descendants
		virtual ~cVerticalStrategy() {}

		/** Returns the Y coord of the piece */
		virtual int GetVerticalPlacement(int a_BlockX, int a_BlockZ) = 0;

		/** Initializes the strategy's parameters from the string representation.
		a_Params is the string containing only the parameters (substring after the first pipe character in the strategy description string).
		If a_LogWarnings is true, logs any problems to the console.
		Returns true if successful, false if the string parsing failed.
		Used when loading the strategy from a file. */
		virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) = 0;

		/** Called when the piece pool is assigned to a generator,
		so that the strategies may bind to the underlying subgenerators. */
		virtual void AssignGens(int a_Seed, cBiomeGenPtr & a_BiomeGen, cTerrainHeightGenPtr & a_TerrainHeightGen, int a_SeaLevel) {}
	};

	typedef SharedPtr<cVerticalStrategy> cVerticalStrategyPtr;


	/** Base class (interface) for the vertical limit of piece placement.
	Each placed piece can have a limit, represented by this class, that gets queried for validity of the placement. */
	class cVerticalLimit
	{
	public:
		virtual ~cVerticalLimit() {}

		/** Called to inquire whether the specified piece can be placed at the specified height.
		a_BlockX, a_BlockZ is the column of the connector that is being queried.
		a_Height is the requested height of the piece's lowest block. */
		virtual bool CanBeAtHeight(int a_BlockX, int a_BlockZ, int a_Height) = 0;

		/** Initializes the limit's parameters from the string representation.
		a_Params is the string containing only the parameters (substring after the first pipe character in the limit description string).
		Returns true if successful, false if the string parsing failed.
		If a_LogWarnings is true, any error while parsing the string is output to the server console.
		Used when loading the limit from a file. */
		virtual bool InitializeFromString(const AString & a_Params, bool a_LogWarnings) = 0;

		/** Called when the piece pool is assigned to a generator,
		so that the limits may bind to the underlying subgenerators. */
		virtual void AssignGens(int a_Seed, cBiomeGenPtr & a_BiomeGen, cTerrainHeightGenPtr & a_TerrainHeightGen, int a_SeaLevel) {}
	};

	typedef SharedPtr<cVerticalLimit> cVerticalLimitPtr;


	/** The strategy used for vertical placement of this piece when it is used as a starting piece. */
	cVerticalStrategyPtr m_VerticalStrategy;

	/** The checker that verifies each placement's vertical position. */
	cVerticalLimitPtr m_VerticalLimit;


	/** Returns all of the available connectors that the piece has.
	Each connector has a (relative) position in the piece, and a type associated with it. */
	virtual cConnectors GetConnectors(void) const = 0;
	
	/** Returns the dimensions of this piece.
	The dimensions cover the entire piece, there is no block that the piece generates outside of this size. */
	virtual Vector3i GetSize(void) const = 0;
	
	/** Returns the "hitbox" of this piece.
	A hitbox is what is compared and must not intersect other pieces' hitboxes when generating. */
	virtual cCuboid GetHitBox(void) const = 0;
	
	/** Returns true if the piece can be rotated CCW the specific number of 90-degree turns. */
	virtual bool CanRotateCCW(int a_NumRotations) const = 0;

	/** Returns the height, based on m_VerticalStrategy, for this piece when used as the starting piece.
	If there's no strategy assigned to this piece, returns -1. */
	int GetStartingPieceHeight(int a_BlockX, int a_BlockZ)
	{
		if (m_VerticalStrategy != nullptr)
		{
			return m_VerticalStrategy->GetVerticalPlacement(a_BlockX, a_BlockZ);
		}
		return -1;
	}
	
	void SetVerticalStrategy(cVerticalStrategyPtr a_VerticalStrategy)
	{
		m_VerticalStrategy = a_VerticalStrategy;
	}

	cVerticalStrategyPtr GetVerticalStrategy(void) const
	{
		return m_VerticalStrategy;
	}

	cVerticalLimitPtr GetVerticalLimit(void) const
	{
		return m_VerticalLimit;
	}

	/** Sets the vertical strategy based on the description in the string.
	If a_LogWarnings is true, logs the parsing problems into the server console.
	Returns true if successful, false if strategy parsing failed (no strategy assigned). */
	bool SetVerticalStrategyFromString(const AString & a_StrategyDesc, bool a_LogWarnings);

	/** Sets the vertical limit based on the description string.
	Returns true if successful, false if limit parsing failed (no limit assigned).
	If a_LogWarnings is true, any problem is reported into the server console. */
	bool SetVerticalLimitFromString(const AString & a_LimitDesc, bool a_LogWarnings);

	/** Returns a copy of the a_Pos after rotating the piece the specified number of CCW rotations. */
	Vector3i RotatePos(const Vector3i & a_Pos, int a_NumCCWRotations) const;

	/** Returns a copy of the connector that is rotated and then moved by the specified amounts. */
	cConnector RotateMoveConnector(const cConnector & a_Connector, int a_NumCCWRotations, int a_MoveX, int a_MoveY, int a_MoveZ) const;
	
	/** Returns the hitbox after the specified number of rotations and moved so that a_MyConnector is placed at a_ToConnectorPos. */
	cCuboid RotateHitBoxToConnector(const cConnector & a_MyConnector, const Vector3i & a_ToConnectorPos, int a_NumCCWRotations) const;
	
	/** Returns the hitbox after the specified number of CCW rotations and moved by the specified amounts. */
	cCuboid RotateMoveHitBox(int a_NumCCWRotations, int a_MoveX, int a_MoveY, int a_MoveZ) const;
};

typedef std::vector<cPiece *> cPieces;





// fwd:
class cPlacedPiece;





/** This class is an interface that provides pieces for the generator. It can keep track of what pieces were
placed and adjust the returned piece vectors. */
class cPiecePool
{
public:
	// Force a virtual destructor in all descendants:
	virtual ~cPiecePool() {}
	
	/** Returns a list of pieces that contain the specified connector type.
	The cPiece pointers returned are managed by the pool and the caller doesn't free them. */
	virtual cPieces GetPiecesWithConnector(int a_ConnectorType) = 0;
	
	/** Returns the pieces that should be used as the starting point.
	Multiple starting points are supported, one of the returned piece will be chosen. */
	virtual cPieces GetStartingPieces(void) = 0;
	
	/** Returns the relative weight with which the a_NewPiece is to be selected for placing under a_PlacedPiece through a_ExistingConnector.
	a_ExistingConnector is the original connector, before any movement or rotation is applied to it.
	This allows the pool to tweak the piece's chances, based on the previous pieces in the tree and the connector used.
	The higher the number returned, the higher the chance the piece will be chosen. 0 means the piece will never be chosen. */
	virtual int GetPieceWeight(
		const cPlacedPiece & a_PlacedPiece,
		const cPiece::cConnector & a_ExistingConnector,
		const cPiece & a_NewPiece
	)
	{
		return 1;
	}
	
	/** Returns the relative weight with which the a_NewPiece is to be selected for placing as the first piece.
	This allows the pool to tweak the piece's chances.
	The higher the number returned, the higher the chance the piece will be chosen. 0 means the piece will not be chosen.
	If all pieces return 0, a random piece is chosen, with all equal chances. */
	virtual int GetStartingPieceWeight(const cPiece & a_NewPiece)
	{
		return 1;
	}

	/** Called after a piece is placed, to notify the pool that it has been used.
	The pool may adjust the pieces it will return the next time. */
	virtual void PiecePlaced(const cPiece & a_Piece) = 0;
	
	/** Called when the pool has finished the current structure and should reset any piece-counters it has
	for a new structure. */
	virtual void Reset(void) = 0;
};





/** Represents a single piece that has been placed to specific coords in the world. */
class cPlacedPiece
{
public:
	cPlacedPiece(const cPlacedPiece * a_Parent, const cPiece & a_Piece, const Vector3i & a_Coords, int a_NumCCWRotations);
	
	const cPlacedPiece * GetParent           (void) const { return m_Parent; }
	const cPiece &       GetPiece            (void) const { return *m_Piece; }
	const Vector3i &     GetCoords           (void) const { return m_Coords; }
	int                  GetNumCCWRotations  (void) const { return m_NumCCWRotations; }
	const cCuboid &      GetHitBox           (void) const { return m_HitBox; }
	int                  GetDepth            (void) const { return m_Depth; }
	bool                 HasBeenMovedToGround(void) const { return m_HasBeenMovedToGround; }
	
	/** Returns the coords as a modifiable object. */
	Vector3i & GetCoords(void) { return m_Coords; }
	
	/** Returns the connector at the specified index, rotated in the actual placement.
	Undefined behavior if a_Index is out of range. */
	cPiece::cConnector GetRotatedConnector(size_t a_Index) const;
	
	/** Returns a copy of the specified connector, modified to account for the translation and rotation for
	this placement. */
	cPiece::cConnector GetRotatedConnector(const cPiece::cConnector & a_Connector) const;
	
	/** Moves the placed piece Y-wise by the specified offset.
	Sets m_HasBeenMovedToGround to true, too.
	Used eg. by village houses. */
	void MoveToGroundBy(int a_OffsetY);
	
protected:
	const cPlacedPiece * m_Parent;
	const cPiece * m_Piece;
	Vector3i m_Coords;
	int m_NumCCWRotations;
	cCuboid m_HitBox;  // Hitbox of the placed piece, in world coords
	int m_Depth;       // Depth in the generated piece tree
	
	/** Set to true once the piece has been moved Y-wise.
	Used eg. by village houses. */
	bool m_HasBeenMovedToGround;
};

typedef std::vector<cPlacedPiece *> cPlacedPieces;





class cPieceGenerator
{
public:
	cPieceGenerator(cPiecePool & a_PiecePool, int a_Seed);
	
	/** Cleans up all the memory used by the placed pieces.
	Call this utility function instead of freeing the items on your own. */
	static void FreePieces(cPlacedPieces & a_PlacedPieces);
	
protected:
	/** The type used for storing a connection from one piece to another, while building the piece tree. */
	struct cConnection
	{
		cPiece * m_Piece;                  // The piece being connected
		cPiece::cConnector m_Connector;    // The piece's connector being used (relative non-rotated coords)
		int m_NumCCWRotations;             // Number of rotations necessary to match the two connectors
		int m_Weight;                      // Relative chance that this connection will be chosen
		
		cConnection(cPiece & a_Piece, cPiece::cConnector & a_Connector, int a_NumCCWRotations, int a_Weight);
	};
	typedef std::vector<cConnection> cConnections;
	
	/** The type used for storing a pool of connectors that will be attempted to expand by another piece. */
	struct cFreeConnector
	{
		cPlacedPiece * m_Piece;
		cPiece::cConnector m_Connector;
		
		cFreeConnector(cPlacedPiece * a_Piece, const cPiece::cConnector & a_Connector);
	};
	typedef std::vector<cFreeConnector> cFreeConnectors;


	cPiecePool & m_PiecePool;
	cNoise m_Noise;
	int m_Seed;

	
	/** Selects a starting piece and places it, including its height and rotation.
	Also puts the piece's connectors in a_OutConnectors. */
	cPlacedPiece * PlaceStartingPiece(int a_BlockX, int a_BlockZ, cFreeConnectors & a_OutConnectors);
	
	/** Tries to place a new piece at the specified (placed) connector. Returns true if successful. */
	bool TryPlacePieceAtConnector(
		const cPlacedPiece & a_ParentPiece,      // The existing piece to a new piece should be placed
		const cPiece::cConnector & a_Connector,  // The existing connector (world-coords) to which a new piece should be placed
		cPlacedPieces & a_OutPieces,             // Already placed pieces, to be checked for intersections
		cFreeConnectors & a_OutConnectors        // List of free connectors to which the new connectors will be placed
	);

	/** Checks if the specified piece would fit with the already-placed pieces, using the specified connector
	and number of CCW rotations.
	a_ExistingConnector is in world-coords and is already rotated properly
	a_ToPos is the world-coords position on which the new connector should be placed (1 block away from a_ExistingConnector, in its Direction)
	a_NewConnector is in the original (non-rotated) coords.
	Returns true if the piece fits, false if not. */
	bool CheckConnection(
		const cPiece::cConnector & a_ExistingConnector,  // The existing connector
		const Vector3i & a_ToPos,                        // The position on which the new connector should be placed
		const cPiece & a_Piece,                          // The new piece
		const cPiece::cConnector & a_NewConnector,       // The connector of the new piece
		int a_NumCCWRotations,                           // Number of rotations for the new piece to align the connector
		const cPlacedPieces & a_OutPieces                // All the already-placed pieces to check
	);
	
	/** DEBUG: Outputs all the connectors in the pool into stdout.
	a_NumProcessed signals the number of connectors from the pool that should be considered processed (not listed). */
	void DebugConnectorPool(const cPieceGenerator::cFreeConnectors & a_ConnectorPool, size_t a_NumProcessed);
} ;





class cBFSPieceGenerator :
	public cPieceGenerator
{
	typedef cPieceGenerator super;

public:
	cBFSPieceGenerator(cPiecePool & a_PiecePool, int a_Seed);

	/** Generates a placement for pieces at the specified coords.
	The Y coord is generated automatically based on the starting piece that is chosen.
	Caller must free each individual cPlacedPiece in a_OutPieces using cPieceGenerator::FreePieces(). */
	void PlacePieces(int a_BlockX, int a_BlockZ, int a_MaxDepth, cPlacedPieces & a_OutPieces);
};





class cDFSPieceGenerator :
	public cPieceGenerator
{
public:
	cDFSPieceGenerator(cPiecePool & a_PiecePool, int a_Seed);
	
	/** Generates a placement for pieces at the specified coords.
	The Y coord is generated automatically based on the starting piece that is chosen.
	Caller must free each individual cPlacedPiece in a_OutPieces using cPieceGenerator::FreePieces(). */
	void PlacePieces(int a_BlockX, int a_BlockZ, cPlacedPieces & a_OutPieces);
};




