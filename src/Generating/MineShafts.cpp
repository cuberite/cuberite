
// MineShafts.cpp

// Implements the cStructGenMineShafts class representing the structure generator for abandoned mineshafts

/*
Algorithm:
The cStructGenMineShafts::cMineShaftSystem class is the main controller, which knows what mineshaft
classes there are and their random weights. It gets asked to produce a new class everytime a connection is to be made.
The cMineShaft class is a base class for each mineshaft structure.
Each cMineShaft descendant knows how large it is, how to imprint itself into the chunk data and where to connect to
other descendants. Its PivotPoint is always a walkable column. Its Direction determines in which direction the structure
is facing.

The generation starts with the central dirt room, from there corridors, crossings and staircases are added
in a depth-first processing. Each of the descendants will branch randomly, if not beyond the allowed recursion level
*/

#include "Globals.h"
#include "MineShafts.h"
#include "../Cuboid.h"
#include "../BlockEntities/ChestEntity.h"
#include "../BlockEntities/MobSpawnerEntity.h"





class cMineShaft abstract
{
public:
	enum eKind
	{
		mskDirtRoom,
		mskCorridor,
		mskCrossing,
		mskStaircase,
	} ;


	enum eDirection
	{
		dirXP,
		dirZP,
		dirXM,
		dirZM,
	} ;


	cStructGenMineShafts::cMineShaftSystem & m_ParentSystem;
	eKind   m_Kind;
	cCuboid m_BoundingBox;


	cMineShaft(cStructGenMineShafts::cMineShaftSystem & a_ParentSystem, eKind a_Kind) :
		m_ParentSystem(a_ParentSystem),
		m_Kind(a_Kind)
	{
	}

	cMineShaft(cStructGenMineShafts::cMineShaftSystem & a_ParentSystem, eKind a_Kind, const cCuboid & a_BoundingBox) :
		m_ParentSystem(a_ParentSystem),
		m_Kind(a_Kind),
		m_BoundingBox(a_BoundingBox)
	{
	}

	virtual ~cMineShaft() {}

	/** Returns true if this mineshaft intersects the specified cuboid */
	bool DoesIntersect(const cCuboid & a_Other)
	{
		return m_BoundingBox.DoesIntersect(a_Other);
	}

	/** If recursion level is not too large, appends more branches to the parent system,
	using exit points specific to this class. */
	virtual void AppendBranches(int a_RecursionLevel, cNoise & a_Noise) = 0;

	/** Imprints this shape into the specified chunk's data */
	virtual void ProcessChunk(cChunkDesc & a_ChunkDesc) = 0;
} ;

using cMineShafts = std::vector<cMineShaft *>;





class cMineShaftDirtRoom:
	public cMineShaft
{
	using Super = cMineShaft;

public:

	cMineShaftDirtRoom(cStructGenMineShafts::cMineShaftSystem & a_Parent, cNoise & a_Noise);

	// cMineShaft overrides:
	virtual void AppendBranches(int a_RecursionLevel, cNoise & a_Noise) override;
	virtual void ProcessChunk(cChunkDesc & a_ChunkDesc) override;
} ;





class cMineShaftCorridor:
	public cMineShaft
{
	using Super = cMineShaft;

public:

	/** Creates a new Corridor attached to the specified pivot point and direction.
	Checks all ParentSystem's objects and disallows intersecting. Initializes the new object to fit.
	May return nullptr if cannot fit. */
	static cMineShaft * CreateAndFit(
		cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
		int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
		cNoise & a_Noise
	);

protected:
	static const int MAX_SEGMENTS = 5;

	int        m_NumSegments;
	eDirection m_Direction;
	bool       m_HasFullBeam[MAX_SEGMENTS];  ///< If true, segment at that index has a full beam support (planks in the top center block)
	int        m_ChestPosition;              ///< If <0, no chest; otherwise an offset from m_BoundingBox's p1.x or p1.z, depenging on m_Direction
	int        m_SpawnerPosition;            ///< If <0, no spawner; otherwise an offset from m_BoundingBox's p1.x or p1.z, depenging on m_Direction
	bool       m_HasTracks;                  ///< If true, random tracks will be placed on the floor

	cMineShaftCorridor(
		cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
		const cCuboid & a_BoundingBox, int a_NumSegments, eDirection a_Direction,
		cNoise & a_Noise
	);

	// cMineShaft overrides:
	virtual void AppendBranches(int a_RecursionLevel, cNoise & a_Noise) override;
	virtual void ProcessChunk(cChunkDesc & a_ChunkDesc) override;

	/** Places a chest, if the corridor has one */
	void PlaceChest(cChunkDesc & a_ChunkDesc);

	/** If this corridor has tracks, places them randomly */
	void PlaceTracks(cChunkDesc & a_ChunkDesc);

	/** If this corridor has a spawner, places the spawner */
	void PlaceSpawner(cChunkDesc & a_ChunkDesc);

	/** Randomly places torches around the central beam block */
	void PlaceTorches(cChunkDesc & a_ChunkDesc);
} ;





class cMineShaftCrossing:
	public cMineShaft
{
	using Super = cMineShaft;

public:

	/** Creates a new Crossing attached to the specified pivot point and direction.
	Checks all ParentSystem's objects and disallows intersecting. Initializes the new object to fit.
	May return nullptr if cannot fit. */
	static cMineShaft * CreateAndFit(
		cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
		int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
		cNoise & a_Noise
	);

protected:
	cMineShaftCrossing(cStructGenMineShafts::cMineShaftSystem & a_ParentSystem, const cCuboid & a_BoundingBox);

	// cMineShaft overrides:
	virtual void AppendBranches(int a_RecursionLevel, cNoise & a_Noise) override;
	virtual void ProcessChunk(cChunkDesc & a_ChunkDesc) override;
} ;





class cMineShaftStaircase:
	public cMineShaft
{
	using Super = cMineShaft;

public:

	enum eSlope
	{
		sUp,
		sDown,
	} ;

	/** Creates a new Staircase attached to the specified pivot point and direction.
	Checks all ParentSystem's objects and disallows intersecting. Initializes the new object to fit.
	May return nullptr if cannot fit.
	*/
	static cMineShaft * CreateAndFit(
		cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
		int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
		cNoise & a_Noise
	);

protected:
	eDirection m_Direction;
	eSlope     m_Slope;


	cMineShaftStaircase(
		cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
		const cCuboid & a_BoundingBox,
		eDirection a_Direction,
		eSlope a_Slope
	);

	// cMineShaft overrides:
	virtual void AppendBranches(int a_RecursionLevel, cNoise & a_Noise) override;
	virtual void ProcessChunk(cChunkDesc & a_ChunkDesc) override;
} ;





class cStructGenMineShafts::cMineShaftSystem:
	public cGridStructGen::cStructure
{
	using Super = cGridStructGen::cStructure;

public:

	int         m_GridSize;            ///< Maximum offset of the dirtroom from grid center, * 2, in each direction
	int         m_MaxRecursion;        ///< Maximum recursion level (initialized from cStructGenMineShafts::m_MaxRecursion)
	int         m_ProbLevelCorridor;   ///< Probability level of a branch object being the corridor
	int         m_ProbLevelCrossing;   ///< Probability level of a branch object being the crossing, minus Corridor
	int         m_ProbLevelStaircase;  ///< Probability level of a branch object being the staircase, minus Crossing
	int         m_ChanceChest;         ///< Chance [0 .. 250] that a corridor has a chest in it
	int         m_ChanceSpawner;       ///< Chance [0 .. 250] that a corridor has a spawner in it
	int         m_ChanceTorch;         ///< Chance [0 .. 10k] for a torch appearing attached to a corridor's beam
	cMineShafts m_MineShafts;          ///< List of cMineShaft descendants that comprise this system
	cCuboid     m_BoundingBox;         ///< Bounding box into which all of the components need to fit


	/** Creates and generates the entire system */
	cMineShaftSystem(
		int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ,
		int a_GridSize, int a_MaxSystemSize, cNoise & a_Noise,
		int a_ProbLevelCorridor, int a_ProbLevelCrossing, int a_ProbLevelStaircase
	);

	~cMineShaftSystem();

	/** Creates new cMineShaft descendant connected at the specified point, heading the specified direction,
	if it fits, appends it to the list and calls its AppendBranches()
	*/
	void AppendBranch(
		int a_BlockX, int a_BlockY, int a_BlockZ,
		cMineShaft::eDirection a_Direction, cNoise & a_Noise,
		int a_RecursionLevel
	);

	/** Returns true if none of the objects in m_MineShafts intersect with the specified bounding box and the bounding box is valid */
	bool CanAppend(const cCuboid & a_BoundingBox);

	// cGridStructGen::cStructure overrides:
	virtual void DrawIntoChunk(cChunkDesc & a_Chunk);
} ;





////////////////////////////////////////////////////////////////////////////////
// cStructGenMineShafts::cMineShaftSystem:

cStructGenMineShafts::cMineShaftSystem::cMineShaftSystem(
	int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ,
	int a_GridSize, int a_MaxSystemSize, cNoise & a_Noise,
	int a_ProbLevelCorridor, int a_ProbLevelCrossing, int a_ProbLevelStaircase
) :
	Super(a_GridX, a_GridZ, a_OriginX, a_OriginZ),
	m_GridSize(a_GridSize),
	m_MaxRecursion(8),  // TODO: settable
	m_ProbLevelCorridor(a_ProbLevelCorridor),
	m_ProbLevelCrossing(a_ProbLevelCrossing),
	m_ProbLevelStaircase(a_ProbLevelStaircase + 1),
	m_ChanceChest(12),  // TODO: settable
	m_ChanceSpawner(12),  // TODO: settable
	m_ChanceTorch(1000)  // TODO: settable
{
	m_MineShafts.reserve(100);

	cMineShaft * Start = new cMineShaftDirtRoom(*this, a_Noise);
	m_MineShafts.push_back(Start);

	m_BoundingBox.Assign(
		{Start->m_BoundingBox.p1.x - a_MaxSystemSize / 2, 2,  Start->m_BoundingBox.p1.z - a_MaxSystemSize / 2},
		{Start->m_BoundingBox.p2.x + a_MaxSystemSize / 2, 50, Start->m_BoundingBox.p2.z + a_MaxSystemSize / 2}
	);

	Start->AppendBranches(0, a_Noise);

	for (cMineShafts::const_iterator itr = m_MineShafts.begin(), end = m_MineShafts.end(); itr != end; ++itr)
	{
		ASSERT((*itr)->m_BoundingBox.IsSorted());
	}  // for itr - m_MineShafts[]
}





cStructGenMineShafts::cMineShaftSystem::~cMineShaftSystem()
{
	for (cMineShafts::iterator itr = m_MineShafts.begin(), end = m_MineShafts.end(); itr != end; ++itr)
	{
		delete *itr;
	}  // for itr - m_MineShafts[]
	m_MineShafts.clear();
}





void cStructGenMineShafts::cMineShaftSystem::DrawIntoChunk(cChunkDesc & a_Chunk)
{
	for (cMineShafts::const_iterator itr = m_MineShafts.begin(), end = m_MineShafts.end(); itr != end; ++itr)
	{
		(*itr)->ProcessChunk(a_Chunk);
	}  // for itr - m_MineShafts[]
}





void cStructGenMineShafts::cMineShaftSystem::AppendBranch(
	int a_PivotX, int a_PivotY, int a_PivotZ,
	cMineShaft::eDirection a_Direction, cNoise & a_Noise,
	int a_RecursionLevel
)
{
	if (a_RecursionLevel > m_MaxRecursion)
	{
		return;
	}

	cMineShaft * Next = nullptr;
	int rnd = (a_Noise.IntNoise3DInt(a_PivotX, a_PivotY + a_RecursionLevel * 16, a_PivotZ) / 13) % m_ProbLevelStaircase;
	if (rnd < m_ProbLevelCorridor)
	{
		Next = cMineShaftCorridor::CreateAndFit(*this, a_PivotX, a_PivotY, a_PivotZ, a_Direction, a_Noise);
	}
	else if (rnd < m_ProbLevelCrossing)
	{
		Next = cMineShaftCrossing::CreateAndFit(*this, a_PivotX, a_PivotY, a_PivotZ, a_Direction, a_Noise);
	}
	else
	{
		Next = cMineShaftStaircase::CreateAndFit(*this, a_PivotX, a_PivotY, a_PivotZ, a_Direction, a_Noise);
	}
	if (Next == nullptr)
	{
		return;
	}
	m_MineShafts.push_back(Next);
	Next->AppendBranches(a_RecursionLevel + 1, a_Noise);
}





bool cStructGenMineShafts::cMineShaftSystem::CanAppend(const cCuboid & a_BoundingBox)
{
	if (!a_BoundingBox.IsCompletelyInside(m_BoundingBox))
	{
		// Too far away, or too low / too high
		return false;
	}

	// Check intersections:
	for (cMineShafts::const_iterator itr = m_MineShafts.begin(), end = m_MineShafts.end(); itr != end; ++itr)
	{
		if ((*itr)->DoesIntersect(a_BoundingBox))
		{
			return false;
		}
	}  // for itr - m_MineShafts[]
	return true;
}





////////////////////////////////////////////////////////////////////////////////
// cMineShaftDirtRoom:

cMineShaftDirtRoom::cMineShaftDirtRoom(cStructGenMineShafts::cMineShaftSystem & a_Parent, cNoise & a_Noise) :
	Super(a_Parent, mskDirtRoom)
{
	// Make the room of random size, min 10 x 4 x 10; max 18 x 12 x 18:
	int rnd = a_Noise.IntNoise3DInt(a_Parent.m_OriginX, 0, a_Parent.m_OriginZ) / 7;
	int OfsX = (rnd % a_Parent.m_GridSize) - a_Parent.m_GridSize / 2;
	rnd >>= 12;
	int OfsZ = (rnd % a_Parent.m_GridSize) - a_Parent.m_GridSize / 2;
	rnd = a_Noise.IntNoise3DInt(a_Parent.m_OriginX, 1000, a_Parent.m_OriginZ) / 11;
	m_BoundingBox.p1.x = a_Parent.m_OriginX + OfsX;
	m_BoundingBox.p2.x = m_BoundingBox.p1.x + 10 + (rnd % 8);
	rnd >>= 4;
	m_BoundingBox.p1.z = a_Parent.m_OriginZ + OfsZ;
	m_BoundingBox.p2.z = m_BoundingBox.p1.z + 10 + (rnd % 8);
	rnd >>= 4;
	m_BoundingBox.p1.y = 20;
	m_BoundingBox.p2.y = 24 + rnd % 8;
}





void cMineShaftDirtRoom::AppendBranches(int a_RecursionLevel, cNoise & a_Noise)
{
	int Height = m_BoundingBox.DifY() - 3;
	for (int x = m_BoundingBox.p1.x + 1; x < m_BoundingBox.p2.x; x += 4)
	{
		int rnd = a_Noise.IntNoise3DInt(x, a_RecursionLevel, m_BoundingBox.p1.z) / 7;
		m_ParentSystem.AppendBranch(x, m_BoundingBox.p1.y + (rnd % Height), m_BoundingBox.p1.z - 1, dirZM, a_Noise, a_RecursionLevel);
		rnd >>= 4;
		m_ParentSystem.AppendBranch(x, m_BoundingBox.p1.y + (rnd % Height), m_BoundingBox.p2.z + 1, dirZP, a_Noise, a_RecursionLevel);
	}

	for (int z = m_BoundingBox.p1.z + 1; z < m_BoundingBox.p2.z; z += 4)
	{
		int rnd = a_Noise.IntNoise3DInt(m_BoundingBox.p1.x, a_RecursionLevel, z) / 13;
		m_ParentSystem.AppendBranch(m_BoundingBox.p1.x - 1, m_BoundingBox.p1.y + (rnd % Height), z, dirXM, a_Noise, a_RecursionLevel);
		rnd >>= 4;
		m_ParentSystem.AppendBranch(m_BoundingBox.p2.x + 1, m_BoundingBox.p1.y + (rnd % Height), z, dirXP, a_Noise, a_RecursionLevel);
	}
}





void cMineShaftDirtRoom::ProcessChunk(cChunkDesc & a_ChunkDesc)
{
	int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	if (
		(m_BoundingBox.p1.x > BlockX + cChunkDef::Width) ||
		(m_BoundingBox.p1.z > BlockZ + cChunkDef::Width) ||
		(m_BoundingBox.p2.x < BlockX) ||
		(m_BoundingBox.p2.z < BlockZ)
	)
	{
		// Early bailout - cannot intersect this chunk
		return;
	}

	// Chunk-relative coords of the boundaries:
	int MinX = std::max(BlockX, m_BoundingBox.p1.x) - BlockX;
	int MaxX = std::min(BlockX + cChunkDef::Width, m_BoundingBox.p2.x + 1) - BlockX;
	int MinZ = std::max(BlockZ, m_BoundingBox.p1.z) - BlockZ;
	int MaxZ = std::min(BlockZ + cChunkDef::Width, m_BoundingBox.p2.z + 1) - BlockZ;

	// Carve the room out:
	for (int z = MinZ; z < MaxZ; z++)
	{
		for (int x = MinX; x < MaxX; x++)
		{
			for (int y = m_BoundingBox.p1.y + 1; y < m_BoundingBox.p2.y; y++)
			{
				a_ChunkDesc.SetBlockType(x, y, z, E_BLOCK_AIR);
			}
			if (a_ChunkDesc.GetBlockType(x, m_BoundingBox.p1.y, z) != E_BLOCK_AIR)
			{
				a_ChunkDesc.SetBlockType(x, m_BoundingBox.p1.y, z, E_BLOCK_DIRT);
			}
		}  // for x
	}  // for z
}





////////////////////////////////////////////////////////////////////////////////
// cMineShaftCorridor:

cMineShaftCorridor::cMineShaftCorridor(
	cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
	const cCuboid & a_BoundingBox, int a_NumSegments, eDirection a_Direction,
	cNoise & a_Noise
) :
	Super(a_ParentSystem, mskCorridor, a_BoundingBox),
	m_NumSegments(a_NumSegments),
	m_Direction(a_Direction),
	m_ChestPosition(-1),
	m_SpawnerPosition(-1)
{
	int rnd = a_Noise.IntNoise3DInt(a_BoundingBox.p1.x, a_BoundingBox.p1.y, a_BoundingBox.p1.z) / 7;
	for (int i = 0; i < a_NumSegments; i++)
	{
		m_HasFullBeam[i] = (rnd % 4) < 3;  // 75 % chance of full beam
		rnd >>= 2;
	}
	m_HasTracks = ((rnd % 4) < 2);  // 50 % chance of tracks

	rnd = a_Noise.IntNoise3DInt(a_BoundingBox.p1.z, a_BoundingBox.p1.x, a_BoundingBox.p1.y) / 7;
	int ChestCheck = rnd % 250;
	rnd >>= 8;
	int SpawnerCheck = rnd % 250;
	rnd >>= 8;
	if (ChestCheck < a_ParentSystem.m_ChanceChest)
	{
		m_ChestPosition = rnd % (a_NumSegments * 5);
	}
	if ((a_NumSegments < 4) && (SpawnerCheck < a_ParentSystem.m_ChanceSpawner))
	{
		m_SpawnerPosition = rnd % (a_NumSegments * 5);
	}
}





cMineShaft * cMineShaftCorridor::CreateAndFit(
	cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
	int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
	cNoise & a_Noise
)
{
	cCuboid BoundingBox(a_PivotX, a_PivotY - 1, a_PivotZ);
	BoundingBox.p2.y += 3;
	int rnd = a_Noise.IntNoise3DInt(a_PivotX, a_PivotY + static_cast<int>(a_ParentSystem.m_MineShafts.size()), a_PivotZ) / 7;
	int NumSegments = 2 + (rnd) % (MAX_SEGMENTS - 1);  // 2 .. MAX_SEGMENTS
	switch (a_Direction)
	{
		case dirXP: BoundingBox.p2.x += NumSegments * 5 - 1; BoundingBox.p1.z -= 1; BoundingBox.p2.z += 1; break;
		case dirXM: BoundingBox.p1.x -= NumSegments * 5 - 1; BoundingBox.p1.z -= 1; BoundingBox.p2.z += 1; break;
		case dirZP: BoundingBox.p2.z += NumSegments * 5 - 1; BoundingBox.p1.x -= 1; BoundingBox.p2.x += 1; break;
		case dirZM: BoundingBox.p1.z -= NumSegments * 5 - 1; BoundingBox.p1.x -= 1; BoundingBox.p2.x += 1; break;
	}
	if (!a_ParentSystem.CanAppend(BoundingBox))
	{
		return nullptr;
	}
	return new cMineShaftCorridor(a_ParentSystem, BoundingBox, NumSegments, a_Direction, a_Noise);
}





void cMineShaftCorridor::AppendBranches(int a_RecursionLevel, cNoise & a_Noise)
{
	int Outerrnd = a_Noise.IntNoise3DInt(m_BoundingBox.p1.x, m_BoundingBox.p1.y + a_RecursionLevel, m_BoundingBox.p1.z) / 7;
	// Prefer the same height, but allow for up to one block height displacement:
	int OuterHeight = m_BoundingBox.p1.y + ((Outerrnd % 4) + ((Outerrnd >> 3) % 3)) / 2;
	switch (m_Direction)
	{
		case dirXM:
		{
			m_ParentSystem.AppendBranch(m_BoundingBox.p1.x - 1, OuterHeight, m_BoundingBox.p1.z + 1, dirXM, a_Noise, a_RecursionLevel);
			for (int i = m_NumSegments; i >= 0; i--)
			{
				int rnd = a_Noise.IntNoise3DInt(m_BoundingBox.p1.x + i + 10, m_BoundingBox.p1.y + a_RecursionLevel, m_BoundingBox.p1.z) / 11;
				int Height = m_BoundingBox.p1.y + ((rnd % 4) + ((rnd >> 3) % 3)) / 2;
				rnd >>= 6;
				int Ofs = 1 + rnd % (m_NumSegments * 5 - 2);
				m_ParentSystem.AppendBranch(m_BoundingBox.p1.x + Ofs, Height, m_BoundingBox.p1.z - 1, dirZM, a_Noise, a_RecursionLevel);
				m_ParentSystem.AppendBranch(m_BoundingBox.p1.x + Ofs, Height, m_BoundingBox.p2.z + 1, dirZP, a_Noise, a_RecursionLevel);
			}
			break;
		}

		case dirXP:
		{
			m_ParentSystem.AppendBranch(m_BoundingBox.p2.x + 1, OuterHeight, m_BoundingBox.p1.z + 1, dirXP, a_Noise, a_RecursionLevel);
			for (int i = m_NumSegments; i >= 0; i--)
			{
				int rnd = a_Noise.IntNoise3DInt(m_BoundingBox.p1.x + i + 10, m_BoundingBox.p1.y + a_RecursionLevel, m_BoundingBox.p1.z) / 11;
				int Height = m_BoundingBox.p1.y + ((rnd % 4) + ((rnd >> 3) % 3)) / 2;
				rnd >>= 6;
				int Ofs = 1 + rnd % (m_NumSegments * 5 - 2);
				m_ParentSystem.AppendBranch(m_BoundingBox.p1.x + Ofs, Height, m_BoundingBox.p1.z - 1, dirZM, a_Noise, a_RecursionLevel);
				m_ParentSystem.AppendBranch(m_BoundingBox.p1.x + Ofs, Height, m_BoundingBox.p2.z + 1, dirZP, a_Noise, a_RecursionLevel);
			}
			break;
		}

		case dirZM:
		{
			m_ParentSystem.AppendBranch(m_BoundingBox.p1.x + 1, OuterHeight, m_BoundingBox.p1.z - 1, dirZM, a_Noise, a_RecursionLevel);
			for (int i = m_NumSegments; i >= 0; i--)
			{
				int rnd = a_Noise.IntNoise3DInt(m_BoundingBox.p1.x + i + 10, m_BoundingBox.p1.y + a_RecursionLevel, m_BoundingBox.p1.z) / 11;
				int Height = m_BoundingBox.p1.y + ((rnd % 4) + ((rnd >> 3) % 3)) / 2;
				rnd >>= 6;
				int Ofs = 1 + rnd % (m_NumSegments * 5 - 2);
				m_ParentSystem.AppendBranch(m_BoundingBox.p1.x - 1, Height, m_BoundingBox.p1.z + Ofs, dirXM, a_Noise, a_RecursionLevel);
				m_ParentSystem.AppendBranch(m_BoundingBox.p2.x + 1, Height, m_BoundingBox.p1.z + Ofs, dirXP, a_Noise, a_RecursionLevel);
			}
			break;
		}

		case dirZP:
		{
			m_ParentSystem.AppendBranch(m_BoundingBox.p1.x + 1, OuterHeight, m_BoundingBox.p2.z + 1, dirZP, a_Noise, a_RecursionLevel);
			for (int i = m_NumSegments; i >= 0; i--)
			{
				int rnd = a_Noise.IntNoise3DInt(m_BoundingBox.p1.x + i + 10, m_BoundingBox.p1.y + a_RecursionLevel, m_BoundingBox.p1.z) / 11;
				int Height = m_BoundingBox.p1.y + ((rnd % 4) + ((rnd >> 3) % 3)) / 2;
				rnd >>= 6;
				int Ofs = 1 + rnd % (m_NumSegments * 5 - 2);
				m_ParentSystem.AppendBranch(m_BoundingBox.p1.x - 1, Height, m_BoundingBox.p1.z + Ofs, dirXM, a_Noise, a_RecursionLevel);
				m_ParentSystem.AppendBranch(m_BoundingBox.p2.x + 1, Height, m_BoundingBox.p1.z + Ofs, dirXP, a_Noise, a_RecursionLevel);
			}
			break;
		}
	}  // switch (m_Direction)
}





void cMineShaftCorridor::ProcessChunk(cChunkDesc & a_ChunkDesc)
{
	int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	cCuboid RelBoundingBox(m_BoundingBox);
	RelBoundingBox.Move({-BlockX, 0, -BlockZ});
	RelBoundingBox.p1.y += 1;
	RelBoundingBox.p2.y -= 1;
	cCuboid Top(RelBoundingBox);
	Top.p2.y += 1;
	Top.p1.y = Top.p2.y;
	a_ChunkDesc.FillRelCuboid(RelBoundingBox, E_BLOCK_AIR, 0);
	a_ChunkDesc.RandomFillRelCuboid(Top, E_BLOCK_AIR, 0, (BlockX ^ (BlockZ + BlockX)), 8000);
	if (m_SpawnerPosition >= 0)
	{
		// Cobwebs around the spider spawner
		a_ChunkDesc.RandomFillRelCuboid(RelBoundingBox, E_BLOCK_COBWEB, 0, (BlockX ^ (BlockZ + BlockZ)), 8000);
		a_ChunkDesc.RandomFillRelCuboid(Top,            E_BLOCK_COBWEB, 0, (BlockX ^ (BlockZ + BlockX)), 5000);
	}
	a_ChunkDesc.RandomFillRelCuboid(Top, E_BLOCK_COBWEB, 0, (BlockX ^ (BlockZ + BlockX + 10)), 500);
	RelBoundingBox.p1.y = m_BoundingBox.p1.y;
	RelBoundingBox.p2.y = m_BoundingBox.p1.y;
	a_ChunkDesc.FloorRelCuboid(RelBoundingBox, E_BLOCK_PLANKS, 0);
	switch (m_Direction)
	{
		case dirXM:
		case dirXP:
		{
			int y1 = m_BoundingBox.p1.y + 1;
			int y2 = m_BoundingBox.p1.y + 2;
			int y3 = m_BoundingBox.p1.y + 3;
			int z1 = m_BoundingBox.p1.z - BlockZ;
			int z2 = m_BoundingBox.p2.z - BlockZ;
			for (int i = 0; i < m_NumSegments; i++)
			{
				int x = m_BoundingBox.p1.x + i * 5 + 2 - BlockX;
				if ((x < 0) || (x >= cChunkDef::Width))
				{
					continue;
				}
				if ((z1 >= 0) && (z1 < cChunkDef::Width))
				{
					a_ChunkDesc.SetBlockTypeMeta(x, y1, z1, E_BLOCK_FENCE, 0);
					a_ChunkDesc.SetBlockTypeMeta(x, y2, z1, E_BLOCK_FENCE, 0);
					a_ChunkDesc.SetBlockTypeMeta(x, y3, z1, E_BLOCK_PLANKS, 0);
				}
				if ((z2 >= 0) && (z2 < cChunkDef::Width))
				{
					a_ChunkDesc.SetBlockTypeMeta(x, y1, z2, E_BLOCK_FENCE, 0);
					a_ChunkDesc.SetBlockTypeMeta(x, y2, z2, E_BLOCK_FENCE, 0);
					a_ChunkDesc.SetBlockTypeMeta(x, y3, z2, E_BLOCK_PLANKS, 0);
				}
				if ((z1 >= -1) && (z1 < cChunkDef::Width - 1) && m_HasFullBeam[i])
				{
					a_ChunkDesc.SetBlockTypeMeta(x, y3, z1 + 1, E_BLOCK_PLANKS, 0);
				}
			}  // for i - NumSegments
			break;
		}

		case dirZM:
		case dirZP:
		{
			int y1 = m_BoundingBox.p1.y + 1;
			int y2 = m_BoundingBox.p1.y + 2;
			int y3 = m_BoundingBox.p1.y + 3;
			int x1 = m_BoundingBox.p1.x - BlockX;
			int x2 = m_BoundingBox.p2.x - BlockX;
			for (int i = 0; i < m_NumSegments; i++)
			{
				int z = m_BoundingBox.p1.z + i * 5 + 2 - BlockZ;
				if ((z < 0) || (z >= cChunkDef::Width))
				{
					continue;
				}
				if ((x1 >= 0) && (x1 < cChunkDef::Width))
				{
					a_ChunkDesc.SetBlockTypeMeta(x1, y1, z, E_BLOCK_FENCE, 0);
					a_ChunkDesc.SetBlockTypeMeta(x1, y2, z, E_BLOCK_FENCE, 0);
					a_ChunkDesc.SetBlockTypeMeta(x1, y3, z, E_BLOCK_PLANKS, 0);
				}
				if ((x2 >= 0) && (x2 < cChunkDef::Width))
				{
					a_ChunkDesc.SetBlockTypeMeta(x2, y1, z, E_BLOCK_FENCE, 0);
					a_ChunkDesc.SetBlockTypeMeta(x2, y2, z, E_BLOCK_FENCE, 0);
					a_ChunkDesc.SetBlockTypeMeta(x2, y3, z, E_BLOCK_PLANKS, 0);
				}
				if ((x1 >= -1) && (x1 < cChunkDef::Width - 1) && m_HasFullBeam[i])
				{
					a_ChunkDesc.SetBlockTypeMeta(x1 + 1, y3, z, E_BLOCK_PLANKS, 0);
				}
			}  // for i - NumSegments
			break;
		}  // case dirZ?
	}  // for i

	PlaceChest(a_ChunkDesc);
	PlaceTracks(a_ChunkDesc);
	PlaceSpawner(a_ChunkDesc);  // (must be after Tracks!)
	PlaceTorches(a_ChunkDesc);
}





void cMineShaftCorridor::PlaceChest(cChunkDesc & a_ChunkDesc)
{
	static const cLootProbab LootProbab[] =
	{
		// Item,                          MinAmount, MaxAmount, Weight
		{ cItem(E_ITEM_IRON),             1,         5,         10 },
		{ cItem(E_ITEM_GOLD),             1,         3,          5 },
		{ cItem(E_ITEM_REDSTONE_DUST),    4,         9,          5 },
		{ cItem(E_ITEM_DIAMOND),          1,         2,          3 },
		{ cItem(E_ITEM_DYE, 1, 4),        4,         9,          5 },  // lapis lazuli dye
		{ cItem(E_ITEM_COAL),             3,         8,         10 },
		{ cItem(E_ITEM_BREAD),            1,         3,         15 },
		{ cItem(E_ITEM_IRON_PICKAXE),     1,         1,          1 },
		{ cItem(E_BLOCK_MINECART_TRACKS), 4,         8,          1 },
		{ cItem(E_ITEM_MELON_SEEDS),      2,         4,         10 },
		{ cItem(E_ITEM_PUMPKIN_SEEDS),    2,         4,         10 },
	} ;

	if (m_ChestPosition < 0)
	{
		return;
	}

	int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	int x, z;
	NIBBLETYPE Meta = 0;
	[&]
	{
		switch (m_Direction)
		{
			case dirXM:
			case dirXP:
			{
				x = m_BoundingBox.p1.x + m_ChestPosition - BlockX;
				z = m_BoundingBox.p1.z - BlockZ;
				Meta = E_META_CHEST_FACING_ZP;
				return;
			}

			case dirZM:
			case dirZP:
			{
				x = m_BoundingBox.p1.x - BlockX;
				z = m_BoundingBox.p1.z + m_ChestPosition - BlockZ;
				Meta = E_META_CHEST_FACING_XP;
				return;
			}
		}  // switch (Dir)
		UNREACHABLE("Unsupported corridor direction");
	}();

	if (
		(x >= 0) && (x < cChunkDef::Width) &&
		(z >= 0) && (z < cChunkDef::Width)
	)
	{
		a_ChunkDesc.SetBlockTypeMeta(x, m_BoundingBox.p1.y + 1, z, E_BLOCK_CHEST, Meta);
		cChestEntity * ChestEntity = static_cast<cChestEntity *>(a_ChunkDesc.GetBlockEntity(x, m_BoundingBox.p1.y + 1, z));
		ASSERT((ChestEntity != nullptr) && (ChestEntity->GetBlockType() == E_BLOCK_CHEST));
		cNoise Noise(a_ChunkDesc.GetChunkX() ^ a_ChunkDesc.GetChunkZ());
		int NumSlots = 3 + ((Noise.IntNoise3DInt(x, m_BoundingBox.p1.y, z) / 11) % 4);
		int Seed = Noise.IntNoise2DInt(x, z);
		ChestEntity->GetContents().GenerateRandomLootWithBooks(LootProbab, ARRAYCOUNT(LootProbab), NumSlots, Seed);
	}
}





void cMineShaftCorridor::PlaceTracks(cChunkDesc & a_ChunkDesc)
{
	if (!m_HasTracks)
	{
		return;
	}
	cCuboid Box(m_BoundingBox);
	Box.Move({-a_ChunkDesc.GetChunkX() * cChunkDef::Width, 1, -a_ChunkDesc.GetChunkZ() * cChunkDef::Width});
	Box.p2.y = Box.p1.y;
	Box.p1.x += 1;
	Box.p2.x -= 1;
	Box.p1.z += 1;
	Box.p2.z -= 1;
	NIBBLETYPE Meta = 0;
	switch (m_Direction)
	{
		case dirXM:
		case dirXP:
		{
			Meta = E_META_TRACKS_X;
			break;
		}

		case dirZM:
		case dirZP:
		{
			Meta = E_META_TRACKS_Z;
			break;
		}
	}  // switch (direction)
	a_ChunkDesc.RandomFillRelCuboid(Box, E_BLOCK_MINECART_TRACKS, Meta, a_ChunkDesc.GetChunkX() + a_ChunkDesc.GetChunkZ(), 6000);
}





void cMineShaftCorridor::PlaceSpawner(cChunkDesc & a_ChunkDesc)
{
	if (m_SpawnerPosition < 0)
	{
		// No spawner in this corridor
		return;
	}
	int SpawnerRelX = m_BoundingBox.p1.x + 1 - a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int SpawnerRelZ = m_BoundingBox.p1.z + 1 - a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	switch (m_Direction)
	{
		case dirXM:
		case dirXP:
		{
			SpawnerRelX += m_SpawnerPosition - 1;
			break;
		}
		case dirZM:
		case dirZP:
		{
			SpawnerRelZ += m_SpawnerPosition - 1;
			break;
		}
	}
	if (
		(SpawnerRelX >= 0) && (SpawnerRelX < cChunkDef::Width) &&
		(SpawnerRelZ >= 0) && (SpawnerRelZ < cChunkDef::Width)
	)
	{
		a_ChunkDesc.SetBlockTypeMeta(SpawnerRelX, m_BoundingBox.p1.y + 1, SpawnerRelZ, E_BLOCK_MOB_SPAWNER, 0);
		cMobSpawnerEntity * MobSpawner = static_cast<cMobSpawnerEntity *>(a_ChunkDesc.GetBlockEntity(SpawnerRelX, m_BoundingBox.p1.y + 1, SpawnerRelZ));
		ASSERT((MobSpawner != nullptr) && (MobSpawner->GetBlockType() == E_BLOCK_MOB_SPAWNER));
		MobSpawner->SetEntity(mtCaveSpider);
	}
}





void cMineShaftCorridor::PlaceTorches(cChunkDesc & a_ChunkDesc)
{
	cNoise Noise(m_BoundingBox.p1.x);
	switch (m_Direction)
	{
		case dirXM:
		case dirXP:
		{
			int z = m_BoundingBox.p1.z + 1 - a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
			if ((z < 0) || (z >= cChunkDef::Width))
			{
				return;
			}
			int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
			for (int i = 0; i < m_NumSegments; i++)
			{
				if (!m_HasFullBeam[i])
				{
					continue;
				}
				int x = m_BoundingBox.p1.x + i * 5 + 1 - BlockX;
				if ((x >= 0) && (x < cChunkDef::Width))
				{
					if (((Noise.IntNoise2DInt(x, z) / 7) % 10000) < m_ParentSystem.m_ChanceTorch)
					{
						a_ChunkDesc.SetBlockTypeMeta(x, m_BoundingBox.p2.y, z, E_BLOCK_TORCH, E_META_TORCH_XP);
					}
				}
				x += 2;
				if ((x >= 0) && (x < cChunkDef::Width))
				{
					if (((Noise.IntNoise2DInt(x, z) / 7) % 10000) < m_ParentSystem.m_ChanceTorch)
					{
						a_ChunkDesc.SetBlockTypeMeta(x, m_BoundingBox.p2.y, z, E_BLOCK_TORCH, E_META_TORCH_XM);
					}
				}
			}  // for i
			break;
		}

		case dirZM:
		case dirZP:
		{
			int x = m_BoundingBox.p1.x + 1 - a_ChunkDesc.GetChunkX() * cChunkDef::Width;
			if ((x < 0) || (x >= cChunkDef::Width))
			{
				return;
			}
			int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
			for (int i = 0; i < m_NumSegments; i++)
			{
				if (!m_HasFullBeam[i])
				{
					continue;
				}
				int z = m_BoundingBox.p1.z + i * 5 + 1 - BlockZ;
				if ((z >= 0) && (z < cChunkDef::Width))
				{
					if (((Noise.IntNoise2DInt(x, z) / 7) % 10000) < m_ParentSystem.m_ChanceTorch)
					{
						a_ChunkDesc.SetBlockTypeMeta(x, m_BoundingBox.p2.y, z, E_BLOCK_TORCH, E_META_TORCH_ZP);
					}
				}
				z += 2;
				if ((z >= 0) && (z < cChunkDef::Width))
				{
					if (((Noise.IntNoise2DInt(x, z) / 7) % 10000) < m_ParentSystem.m_ChanceTorch)
					{
						a_ChunkDesc.SetBlockTypeMeta(x, m_BoundingBox.p2.y, z, E_BLOCK_TORCH, E_META_TORCH_ZM);
					}
				}
			}  // for i
			break;
		}
	}  // switch (direction)
}





////////////////////////////////////////////////////////////////////////////////
// cMineShaftCrossing:

cMineShaftCrossing::cMineShaftCrossing(cStructGenMineShafts::cMineShaftSystem & a_ParentSystem, const cCuboid & a_BoundingBox) :
	Super(a_ParentSystem, mskCrossing, a_BoundingBox)
{
}





cMineShaft * cMineShaftCrossing::CreateAndFit(
	cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
	int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
	cNoise & a_Noise
)
{
	cCuboid BoundingBox(a_PivotX, a_PivotY - 1, a_PivotZ);
	int rnd = a_Noise.IntNoise3DInt(a_PivotX, a_PivotY + static_cast<int>(a_ParentSystem.m_MineShafts.size()), a_PivotZ) / 7;
	BoundingBox.p2.y += 3;
	if ((rnd % 4) < 2)
	{
		// 2-level crossing:
		BoundingBox.p2.y += 4;
		rnd >>= 2;
		if ((rnd % 4) < 2)
		{
			// This is the higher level:
			BoundingBox.p1.y -= 4;
			BoundingBox.p2.y -= 4;
		}
	}
	switch (a_Direction)
	{
		case dirXP: BoundingBox.p2.x += 4; BoundingBox.p1.z -= 2; BoundingBox.p2.z += 2; break;
		case dirXM: BoundingBox.p1.x -= 4; BoundingBox.p1.z -= 2; BoundingBox.p2.z += 2; break;
		case dirZP: BoundingBox.p2.z += 4; BoundingBox.p1.x -= 2; BoundingBox.p2.x += 2; break;
		case dirZM: BoundingBox.p1.z -= 4; BoundingBox.p1.x -= 2; BoundingBox.p2.x += 2; break;
	}
	if (!a_ParentSystem.CanAppend(BoundingBox))
	{
		return nullptr;
	}
	return new cMineShaftCrossing(a_ParentSystem, BoundingBox);
}





void cMineShaftCrossing::AppendBranches(int a_RecursionLevel, cNoise & a_Noise)
{
	struct
	{
		int x, y, z;
		eDirection dir;
	} Exits[] =
	{
		// Bottom level:
		{-1, 1,  2, dirXM},
		{ 2, 1, -1, dirZM},
		{ 5, 1,  2, dirXP},
		{ 2, 1,  5, dirZP},
		// Top level:
		{-1, 5,  2, dirXM},
		{ 2, 5, -1, dirZM},
		{ 5, 5,  2, dirXP},
		{ 2, 5,  5, dirZP},
	} ;
	for (size_t i = 0; i < ARRAYCOUNT(Exits); i++)
	{
		if (m_BoundingBox.p1.y + Exits[i].y >= m_BoundingBox.p2.y)
		{
			// This exit is not available (two-level exit on a one-level crossing)
			continue;
		}

		int Height = m_BoundingBox.p1.y + Exits[i].y;
		m_ParentSystem.AppendBranch(m_BoundingBox.p1.x + Exits[i].x, Height, m_BoundingBox.p1.z + Exits[i].z, Exits[i].dir, a_Noise, a_RecursionLevel);
	}  // for i
}





void cMineShaftCrossing::ProcessChunk(cChunkDesc & a_ChunkDesc)
{
	int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	cCuboid box(m_BoundingBox);
	box.Move({-BlockX, 0, -BlockZ});
	if ((box.p2.x < 0) || (box.p2.z < 0) || (box.p1.x >= cChunkDef::Width) || (box.p1.z > cChunkDef::Width))
	{
		// Does not intersect this chunk
		return;
	}
	int Floor = box.p1.y + 1;
	int Ceil = box.p2.y;

	// The supports:
	a_ChunkDesc.FillRelCuboid(box.p1.x + 1, box.p1.x + 1, Floor, Ceil, box.p1.z + 1, box.p1.z + 1, E_BLOCK_PLANKS, 0);
	a_ChunkDesc.FillRelCuboid(box.p2.x - 1, box.p2.x - 1, Floor, Ceil, box.p1.z + 1, box.p1.z + 1, E_BLOCK_PLANKS, 0);
	a_ChunkDesc.FillRelCuboid(box.p1.x + 1, box.p1.x + 1, Floor, Ceil, box.p2.z - 1, box.p2.z - 1, E_BLOCK_PLANKS, 0);
	a_ChunkDesc.FillRelCuboid(box.p2.x - 1, box.p2.x - 1, Floor, Ceil, box.p2.z - 1, box.p2.z - 1, E_BLOCK_PLANKS, 0);

	// The air in between:
	a_ChunkDesc.FillRelCuboid(box.p1.x + 2, box.p1.x + 2, Floor, Ceil, box.p1.z + 1, box.p2.z - 1, E_BLOCK_AIR, 0);
	a_ChunkDesc.FillRelCuboid(box.p1.x + 1, box.p2.x - 1, Floor, Ceil, box.p1.z + 2, box.p1.z + 2, E_BLOCK_AIR, 0);

	// The air on the edges:
	int Mid = Floor + 2;
	a_ChunkDesc.FillRelCuboid(box.p1.x,     box.p1.x,     Floor, Mid, box.p1.z + 1, box.p2.z - 1, E_BLOCK_AIR, 0);
	a_ChunkDesc.FillRelCuboid(box.p2.x,     box.p2.x,     Floor, Mid, box.p1.z + 1, box.p2.z - 1, E_BLOCK_AIR, 0);
	a_ChunkDesc.FillRelCuboid(box.p1.x + 1, box.p2.x - 1, Floor, Mid, box.p1.z,     box.p1.z,     E_BLOCK_AIR, 0);
	a_ChunkDesc.FillRelCuboid(box.p1.x + 1, box.p2.x - 1, Floor, Mid, box.p2.z,     box.p2.z,     E_BLOCK_AIR, 0);
	Mid += 2;
	if (Mid < Ceil)
	{
		a_ChunkDesc.FillRelCuboid(box.p1.x,     box.p1.x,     Mid, Ceil, box.p1.z + 1, box.p2.z - 1, E_BLOCK_AIR, 0);
		a_ChunkDesc.FillRelCuboid(box.p2.x,     box.p2.x,     Mid, Ceil, box.p1.z + 1, box.p2.z - 1, E_BLOCK_AIR, 0);
		a_ChunkDesc.FillRelCuboid(box.p1.x + 1, box.p2.x - 1, Mid, Ceil, box.p1.z,     box.p1.z,     E_BLOCK_AIR, 0);
		a_ChunkDesc.FillRelCuboid(box.p1.x + 1, box.p2.x - 1, Mid, Ceil, box.p2.z,     box.p2.z,     E_BLOCK_AIR, 0);
	}

	// The floor, if needed:
	box.p2.y = box.p1.y;
	a_ChunkDesc.FloorRelCuboid(box, E_BLOCK_PLANKS, 0);
}





////////////////////////////////////////////////////////////////////////////////
// cMineShaftStaircase:

cMineShaftStaircase::cMineShaftStaircase(
	cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
	const cCuboid & a_BoundingBox,
	eDirection a_Direction,
	eSlope a_Slope
) :
	Super(a_ParentSystem, mskStaircase, a_BoundingBox),
	m_Direction(a_Direction),
	m_Slope(a_Slope)
{
}





cMineShaft * cMineShaftStaircase::CreateAndFit(
	cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
	int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
	cNoise & a_Noise
)
{
	int rnd = a_Noise.IntNoise3DInt(a_PivotX, a_PivotY + static_cast<int>(a_ParentSystem.m_MineShafts.size()), a_PivotZ) / 7;
	cCuboid Box;
	switch (a_Direction)
	{
		case dirXM:
		{
			Box.Assign({a_PivotX - 7, a_PivotY - 1, a_PivotZ - 1}, {a_PivotX, a_PivotY + 6, a_PivotZ + 1});
			break;
		}
		case dirXP:
		{
			Box.Assign({a_PivotX, a_PivotY - 1, a_PivotZ - 1}, {a_PivotX + 7, a_PivotY + 6, a_PivotZ + 1});
			break;
		}
		case dirZM:
		{
			Box.Assign({a_PivotX - 1, a_PivotY - 1, a_PivotZ - 7}, {a_PivotX + 1, a_PivotY + 6, a_PivotZ});
			break;
		}
		case dirZP:
		{
			Box.Assign({a_PivotX - 1, a_PivotY - 1, a_PivotZ}, {a_PivotX + 1, a_PivotY + 6, a_PivotZ + 7});
			break;
		}
	}
	eSlope Slope = sUp;
	if ((rnd % 4) < 2)  // 50 %
	{
		Slope = sDown;
		Box.Move({0, -4, 0});
	}
	if (!a_ParentSystem.CanAppend(Box))
	{
		return nullptr;
	}
	return new cMineShaftStaircase(a_ParentSystem, Box, a_Direction, Slope);
}





void cMineShaftStaircase::AppendBranches(int a_RecursionLevel, cNoise & a_Noise)
{
	int Height = m_BoundingBox.p1.y + ((m_Slope == sDown) ? 1 : 5);
	switch (m_Direction)
	{
		case dirXM: m_ParentSystem.AppendBranch(m_BoundingBox.p1.x - 1, Height, m_BoundingBox.p1.z + 1, dirXM, a_Noise, a_RecursionLevel); break;
		case dirXP: m_ParentSystem.AppendBranch(m_BoundingBox.p2.x + 1, Height, m_BoundingBox.p1.z + 1, dirXP, a_Noise, a_RecursionLevel); break;
		case dirZM: m_ParentSystem.AppendBranch(m_BoundingBox.p1.x + 1, Height, m_BoundingBox.p1.z - 1, dirZM, a_Noise, a_RecursionLevel); break;
		case dirZP: m_ParentSystem.AppendBranch(m_BoundingBox.p1.x + 1, Height, m_BoundingBox.p2.z + 1, dirZP, a_Noise, a_RecursionLevel); break;
	}
}





void cMineShaftStaircase::ProcessChunk(cChunkDesc & a_ChunkDesc)
{
	int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	cCuboid RelB(m_BoundingBox);
	RelB.Move({-BlockX, 0, -BlockZ});
	if (
		(RelB.p1.x >= cChunkDef::Width) ||
		(RelB.p1.z >= cChunkDef::Width) ||
		(RelB.p2.x < 0) ||
		(RelB.p2.z < 0)
	)
	{
		// No intersection between this staircase and this chunk
		return;
	}

	int SFloor = RelB.p1.y + ((m_Slope == sDown) ? 5 : 1);
	int DFloor = RelB.p1.y + ((m_Slope == sDown) ? 1 : 5);
	int Add = (m_Slope == sDown) ? -1 : 1;
	int InitAdd = (m_Slope == sDown) ? -1 : 0;
	cCuboid Box;
	switch (m_Direction)
	{
		case dirXM:
		{
			a_ChunkDesc.FillRelCuboid (RelB.p2.x - 1, RelB.p2.x,     SFloor,     SFloor + 2, RelB.p1.z, RelB.p2.z, E_BLOCK_AIR, 0);
			a_ChunkDesc.FillRelCuboid (RelB.p1.x,     RelB.p1.x + 1, DFloor,     DFloor + 2, RelB.p1.z, RelB.p2.z, E_BLOCK_AIR, 0);
			a_ChunkDesc.FloorRelCuboid(RelB.p2.x - 1, RelB.p2.x,     SFloor - 1, SFloor - 1, RelB.p1.z, RelB.p2.z, E_BLOCK_PLANKS, 0);
			a_ChunkDesc.FloorRelCuboid(RelB.p1.x,     RelB.p1.x + 1, DFloor - 1, DFloor - 1, RelB.p1.z, RelB.p2.z, E_BLOCK_PLANKS, 0);
			Box.Assign({RelB.p2.x - 2, SFloor + InitAdd, RelB.p1.z}, {RelB.p2.x - 2, SFloor + 3 + InitAdd, RelB.p2.z});
			for (int i = 0; i < 4; i++)
			{
				a_ChunkDesc.FillRelCuboid(Box, E_BLOCK_AIR, 0);
				a_ChunkDesc.FloorRelCuboid(Box.p1.x, Box.p2.x, Box.p1.y - 1, Box.p1.y - 1, Box.p1.z, Box.p2.z, E_BLOCK_PLANKS, 0);
				Box.Move({-1, Add, 0});
			}
			break;
		}

		case dirXP:
		{
			a_ChunkDesc.FillRelCuboid (RelB.p1.x,     RelB.p1.x + 1, SFloor,     SFloor + 2, RelB.p1.z, RelB.p2.z, E_BLOCK_AIR, 0);
			a_ChunkDesc.FillRelCuboid (RelB.p2.x - 1, RelB.p2.x,     DFloor,     DFloor + 2, RelB.p1.z, RelB.p2.z, E_BLOCK_AIR, 0);
			a_ChunkDesc.FloorRelCuboid(RelB.p1.x,     RelB.p1.x + 1, SFloor - 1, SFloor - 1, RelB.p1.z, RelB.p2.z, E_BLOCK_PLANKS, 0);
			a_ChunkDesc.FloorRelCuboid(RelB.p2.x - 1, RelB.p2.x,     DFloor - 1, DFloor - 1, RelB.p1.z, RelB.p2.z, E_BLOCK_PLANKS, 0);
			Box.Assign({RelB.p1.x + 2, SFloor + InitAdd, RelB.p1.z}, {RelB.p1.x + 2, SFloor + 3 + InitAdd, RelB.p2.z});
			for (int i = 0; i < 4; i++)
			{
				a_ChunkDesc.FillRelCuboid(Box, E_BLOCK_AIR, 0);
				a_ChunkDesc.FloorRelCuboid(Box.p1.x, Box.p2.x, Box.p1.y - 1, Box.p1.y - 1, Box.p1.z, Box.p2.z, E_BLOCK_PLANKS, 0);
				Box.Move({1, Add, 0});
			}
			break;
		}

		case dirZM:
		{
			a_ChunkDesc.FillRelCuboid (RelB.p1.x, RelB.p2.x, SFloor,     SFloor + 2, RelB.p2.z - 1, RelB.p2.z,      E_BLOCK_AIR, 0);
			a_ChunkDesc.FillRelCuboid (RelB.p1.x, RelB.p2.x, DFloor,     DFloor + 2, RelB.p1.z,     RelB.p1.z + 1,  E_BLOCK_AIR, 0);
			a_ChunkDesc.FloorRelCuboid(RelB.p1.x, RelB.p2.x, SFloor - 1, SFloor - 1, RelB.p2.z - 1, RelB.p2.z,      E_BLOCK_PLANKS, 0);
			a_ChunkDesc.FloorRelCuboid(RelB.p1.x, RelB.p2.x, DFloor - 1, DFloor - 1, RelB.p1.z,     RelB.p1.z + 1,  E_BLOCK_PLANKS, 0);
			Box.Assign({RelB.p1.x, SFloor + InitAdd, RelB.p2.z - 2}, {RelB.p2.x, SFloor + 3 + InitAdd, RelB.p2.z - 2});
			for (int i = 0; i < 4; i++)
			{
				a_ChunkDesc.FillRelCuboid(Box, E_BLOCK_AIR, 0);
				a_ChunkDesc.FloorRelCuboid(Box.p1.x, Box.p2.x, Box.p1.y - 1, Box.p1.y - 1, Box.p1.z, Box.p2.z, E_BLOCK_PLANKS, 0);
				Box.Move({0, Add, -1});
			}
			break;
		}

		case dirZP:
		{
			a_ChunkDesc.FillRelCuboid (RelB.p1.x, RelB.p2.x, SFloor,     SFloor + 2, RelB.p1.z,     RelB.p1.z + 1,  E_BLOCK_AIR, 0);
			a_ChunkDesc.FillRelCuboid (RelB.p1.x, RelB.p2.x, DFloor,     DFloor + 2, RelB.p2.z - 1, RelB.p2.z,      E_BLOCK_AIR, 0);
			a_ChunkDesc.FloorRelCuboid(RelB.p1.x, RelB.p2.x, SFloor - 1, SFloor - 1, RelB.p1.z,     RelB.p1.z + 1,  E_BLOCK_PLANKS, 0);
			a_ChunkDesc.FloorRelCuboid(RelB.p1.x, RelB.p2.x, DFloor - 1, DFloor - 1, RelB.p2.z - 1, RelB.p2.z,      E_BLOCK_PLANKS, 0);
			Box.Assign({RelB.p1.x, SFloor + InitAdd, RelB.p1.z + 2}, {RelB.p2.x, SFloor + 3 + InitAdd, RelB.p1.z + 2});
			for (int i = 0; i < 4; i++)
			{
				a_ChunkDesc.FillRelCuboid(Box, E_BLOCK_AIR, 0);
				a_ChunkDesc.FloorRelCuboid(Box.p1.x, Box.p2.x, Box.p1.y - 1, Box.p1.y - 1, Box.p1.z, Box.p2.z, E_BLOCK_PLANKS, 0);
				Box.Move({0, Add, 1});
			}
			break;
		}

	}  // switch (m_Direction)
}





////////////////////////////////////////////////////////////////////////////////
// cStructGenMineShafts:

cStructGenMineShafts::cStructGenMineShafts(
	int a_Seed, int a_GridSize, int a_MaxOffset, int a_MaxSystemSize,
	int a_ChanceCorridor, int a_ChanceCrossing, int a_ChanceStaircase
) :
	Super(a_Seed, a_GridSize, a_GridSize, a_MaxOffset, a_MaxOffset, a_MaxSystemSize, a_MaxSystemSize, 100),
	m_GridSize(a_GridSize),
	m_MaxSystemSize(a_MaxSystemSize),
	m_ProbLevelCorridor(std::max(0, a_ChanceCorridor)),
	m_ProbLevelCrossing(std::max(0, a_ChanceCorridor + a_ChanceCrossing)),
	m_ProbLevelStaircase(std::max(0, a_ChanceCorridor + a_ChanceCrossing + a_ChanceStaircase))
{
}





cGridStructGen::cStructurePtr cStructGenMineShafts::CreateStructure(int a_GridX, int a_GridZ, int a_OriginX, int a_OriginZ)
{
	return cStructurePtr(new cMineShaftSystem(a_GridX, a_GridZ, a_OriginX, a_OriginZ, m_GridSize, m_MaxSystemSize, m_Noise, m_ProbLevelCorridor, m_ProbLevelCrossing, m_ProbLevelStaircase));
}




