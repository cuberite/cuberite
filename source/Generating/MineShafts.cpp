
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





static const int NEIGHBORHOOD_SIZE = 3;





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
	
	
	cMineShaft(cStructGenMineShafts::cMineShaftSystem & a_ParentSystem, eKind a_Kind) :
		m_ParentSystem(a_ParentSystem),
		m_Kind(a_Kind)
	{
	}
	
	/// Returns true if this mineshaft intersects the specified cuboid
	bool DoesIntersect(const cCuboid & a_Other);
	
	/** If recursion level is not too large, appends more branches to the parent system,
	using exit points specific to this class.
	*/
	virtual void AppendBranches(int a_RecursionLevel, cNoise & a_Noise) = 0;
	
	/// Imprints this shape into the specified chunk's data
	virtual void ProcessChunk(cChunkDesc & a_ChunkDesc) = 0;
	
protected:
	eKind   m_Kind;
	cCuboid m_BoundingBox;
	
	int   m_PivotX, m_PivotZ;
	
	cStructGenMineShafts::cMineShaftSystem & m_ParentSystem;
} ;

typedef std::vector<cMineShaft *> cMineShafts;





class cMineShaftDirtRoom :
	public cMineShaft
{
	typedef cMineShaft super;
	
public:
	cMineShaftDirtRoom(cStructGenMineShafts::cMineShaftSystem & a_Parent, cNoise & a_Noise);
	
	// cMineShaft overrides:
	virtual void AppendBranches(int a_RecursionLevel, cNoise & a_Noise) override;
	virtual void ProcessChunk(cChunkDesc & a_ChunkDesc) override;
} ;





class cMineShaftCorridor :
	public cMineShaft
{
	typedef cMineShaft super;
	
public:
	/** Creates a new Corridor attached to the specified pivot point and direction.
	Checks all ParentSystem's objects and disallows intersecting. Initializes the new object to fit.
	May return NULL if cannot fit.
	*/
	static cMineShaft * CreateAndFit(
		cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
		int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
		cNoise & a_Noise
	);
	
	// TODO
} ;





class cMineShaftCrossing :
	public cMineShaft
{
	typedef cMineShaft super;
	
public:
	/** Creates a new Crossing attached to the specified pivot point and direction.
	Checks all ParentSystem's objects and disallows intersecting. Initializes the new object to fit.
	May return NULL if cannot fit.
	*/
	static cMineShaft * CreateAndFit(
		cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
		int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
		cNoise & a_Noise
	);
	
	// TODO
} ;





class cMineShaftStaircase :
	public cMineShaft
{
	typedef cMineShaft super;
	
public:
	/** Creates a new Staircase attached to the specified pivot point and direction.
	Checks all ParentSystem's objects and disallows intersecting. Initializes the new object to fit.
	May return NULL if cannot fit.
	*/
	static cMineShaft * CreateAndFit(
		cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
		int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
		cNoise & a_Noise
	);
	
	// TODO
} ;





class cStructGenMineShafts::cMineShaftSystem
{
public:
	int         m_BlockX, m_BlockZ;  ///< The pivot point on which the system is generated
	int         m_MaxSystemSize;     ///< Maximum size of a system (initialized from cStructGenMineShafts::m_MaxSystemSize)
	int         m_MaxRecursion;      ///< Maximum recursion level (initialized from cStructGenMineShafts::m_MaxRecursion)
	int         m_ChanceCorridor;    ///< Chance (out of 1000) of the next branch object being the corridor
	int         m_ChanceCrossing;    ///< Chance (out of 1000) of the next branch object being the crossing
	int         m_ChanceStaircase;   ///< Chance (out of 1000) of the next branch object being the staircase
	cMineShafts m_MineShafts;        ///< List of cMineShaft descendants that comprise this system

	/// Creates and generates the entire system
	cMineShaftSystem(int a_BlockX, int a_BlockZ, int a_MaxSystemSize, cNoise & a_Noise);

	~cMineShaftSystem();

	/// Carves the system into the chunk data
	void ProcessChunk(cChunkDesc & a_Chunk);
	
	/** Creates new cMineShaft descendant connected at the specified point, heading the specified direction,
	if it fits, appends it to the list and calls its AppendBranches()
	*/
	void AppendBranch(
		int a_BlockX, int a_BlockY, int a_BlockZ,
		cMineShaft::eDirection a_Direction, cNoise & a_Noise,
		int a_RecursionLevel
	);
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenMineShafts::cMineShaftSystem:

cStructGenMineShafts::cMineShaftSystem::cMineShaftSystem(int a_BlockX, int a_BlockZ, int a_MaxSystemSize, cNoise & a_Noise) :
	m_BlockX(a_BlockX),
	m_BlockZ(a_BlockZ),
	m_MaxSystemSize(a_MaxSystemSize)
{
	m_MineShafts.reserve(100);
	
	cMineShaft * Start = new cMineShaftDirtRoom(*this, a_Noise);
	m_MineShafts.push_back(Start);
	Start->AppendBranches(0, a_Noise);
}





cStructGenMineShafts::cMineShaftSystem::~cMineShaftSystem()
{
	for (cMineShafts::iterator itr = m_MineShafts.begin(), end = m_MineShafts.end(); itr != end; ++itr)
	{
		delete *itr;
	}  // for itr - m_MineShafts[]
	m_MineShafts.clear();
}





void cStructGenMineShafts::cMineShaftSystem::ProcessChunk(cChunkDesc & a_Chunk)
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
	
	cMineShaft * Next = NULL;
	int rnd = (a_Noise.IntNoise3DInt(a_PivotX, a_PivotY + a_RecursionLevel * 16, a_PivotZ) / 13) % 1000;
	if (rnd < m_ChanceCorridor)
	{
		Next = cMineShaftCorridor::CreateAndFit(*this, a_PivotX, a_PivotY, a_PivotZ, a_Direction, a_Noise);
	}
	else if (rnd < m_ChanceCrossing)
	{
		Next = cMineShaftCrossing::CreateAndFit(*this, a_PivotX, a_PivotY, a_PivotZ, a_Direction, a_Noise);
	}
	else if (rnd < m_ChanceStaircase)
	{
		Next = cMineShaftStaircase::CreateAndFit(*this, a_PivotX, a_PivotY, a_PivotZ, a_Direction, a_Noise);
	}
	if (Next == NULL)
	{
		return;
	}
	m_MineShafts.push_back(Next);
	Next->AppendBranches(a_RecursionLevel + 1, a_Noise);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMineShaftDirtRoom:

cMineShaftDirtRoom::cMineShaftDirtRoom(cStructGenMineShafts::cMineShaftSystem & a_Parent, cNoise & a_Noise) :
	super(a_Parent, mskDirtRoom)
{
	// Make the room of random size, min 10 x 4 x 10; max 18 x 12 x 18:
	int rnd = a_Noise.IntNoise3DInt(a_Parent.m_BlockX, 0, a_Parent.m_BlockZ) / 7;
	m_BoundingBox.p1.x = a_Parent.m_BlockX;
	m_BoundingBox.p2.x = a_Parent.m_BlockX + 10 + (rnd % 8);
	rnd >>= 4;
	m_BoundingBox.p1.z = a_Parent.m_BlockZ;
	m_BoundingBox.p2.z = a_Parent.m_BlockZ + 10 + (rnd % 8);
	rnd >>= 4;
	m_BoundingBox.p1.y = 50;
	m_BoundingBox.p2.y = 54 + rnd % 8;
}





void cMineShaftDirtRoom::AppendBranches(int a_RecursionLevel, cNoise & a_Noise)
{
	int Height = m_BoundingBox.DifY() - 3;
	for (int x = m_BoundingBox.p1.x + 1; x < m_BoundingBox.p2.x; x += 4)
	{
		int rnd = a_Noise.IntNoise3DInt(x, a_RecursionLevel, m_BoundingBox.p1.z) / 7;
		m_ParentSystem.AppendBranch(x, m_BoundingBox.p1.y + (rnd % Height), m_BoundingBox.p1.z, dirZM, a_Noise, a_RecursionLevel);
		rnd >>= 4;
		m_ParentSystem.AppendBranch(x, m_BoundingBox.p1.y + (rnd % Height), m_BoundingBox.p2.z, dirZP, a_Noise, a_RecursionLevel);
	}
	
	for (int z = m_BoundingBox.p1.z + 1; z < m_BoundingBox.p2.z; z += 4)
	{
		int rnd = a_Noise.IntNoise3DInt(m_BoundingBox.p1.x, a_RecursionLevel, z) / 13;
		m_ParentSystem.AppendBranch(m_BoundingBox.p1.x, m_BoundingBox.p1.y + (rnd % Height), z, dirXM, a_Noise, a_RecursionLevel);
		rnd >>= 4;
		m_ParentSystem.AppendBranch(m_BoundingBox.p2.x, m_BoundingBox.p1.y + (rnd % Height), z, dirXP, a_Noise, a_RecursionLevel);
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
	
	// DEBUG: To find the rooms easily, add a glass column to the top of the world:
	if (
		(m_BoundingBox.p1.x >= BlockX) && (m_BoundingBox.p1.x < BlockX + cChunkDef::Width) &&
		(m_BoundingBox.p1.z >= BlockZ) && (m_BoundingBox.p1.z < BlockZ + cChunkDef::Width)
	)
	{
		int Height = a_ChunkDesc.GetHeight(BlockX - m_BoundingBox.p1.x, BlockZ - m_BoundingBox.p1.z);
		for (int y = m_BoundingBox.p2.y; y < 256; y++)
		{
			a_ChunkDesc.SetBlockType(BlockX - m_BoundingBox.p1.x, y, BlockZ - m_BoundingBox.p1.z, (y < Height) ? E_BLOCK_AIR : E_BLOCK_GLASS);
		}
	}
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMineShaftCorridor:

cMineShaft * cMineShaftCorridor::CreateAndFit(
	cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
	int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
	cNoise & a_Noise
)
{
	// TODO
	return NULL;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMineShaftCrossing:

cMineShaft * cMineShaftCrossing::CreateAndFit(
	cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
	int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
	cNoise & a_Noise
)
{
	// TODO
	return NULL;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cMineShaftStaircase:

cMineShaft * cMineShaftStaircase::CreateAndFit(
	cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
	int a_PivotX, int a_PivotY, int a_PivotZ, eDirection a_Direction,
	cNoise & a_Noise
)
{
	// TODO
	return NULL;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenMineShafts:

cStructGenMineShafts::cStructGenMineShafts(int a_Seed, int a_GridSize, int a_MaxSystemSize) :
	m_Noise(a_Seed),
	m_GridSize(a_GridSize),
	m_MaxSystemSize(a_MaxSystemSize)
{
}





cStructGenMineShafts::~cStructGenMineShafts()
{
	ClearCache();
}





void cStructGenMineShafts::ClearCache(void)
{
	for (cMineShaftSystems::const_iterator itr = m_Cache.begin(), end = m_Cache.end(); itr != end; ++itr)
	{
		delete *itr;
	}  // for itr - m_Cache[]
	m_Cache.clear();
}





void cStructGenMineShafts::GetMineShaftSystemsForChunk(
	int a_ChunkX, int a_ChunkZ,
	cStructGenMineShafts::cMineShaftSystems & a_MineShafts
)
{
	int BaseX = a_ChunkX * cChunkDef::Width / m_GridSize;
	int BaseZ = a_ChunkZ * cChunkDef::Width / m_GridSize;
	if (BaseX < 0)
	{
		--BaseX;
	}
	if (BaseZ < 0)
	{
		--BaseZ;
	}
	BaseX -= NEIGHBORHOOD_SIZE / 2;
	BaseZ -= NEIGHBORHOOD_SIZE / 2;

	// Walk the cache, move each cave system that we want into a_Caves:
	int StartX = BaseX * m_GridSize;
	int EndX = (BaseX + NEIGHBORHOOD_SIZE + 1) * m_GridSize;
	int StartZ = BaseZ * m_GridSize;
	int EndZ = (BaseZ + NEIGHBORHOOD_SIZE + 1) * m_GridSize;
	for (cMineShaftSystems::iterator itr = m_Cache.begin(), end = m_Cache.end(); itr != end;)
	{
		if (
			((*itr)->m_BlockX >= StartX) && ((*itr)->m_BlockX < EndX) &&
			((*itr)->m_BlockZ >= StartZ) && ((*itr)->m_BlockZ < EndZ)
		)
		{
			// want
			a_MineShafts.push_back(*itr);
			itr = m_Cache.erase(itr);
		}
		else
		{
			// don't want
			++itr;
		}
	}  // for itr - m_Cache[]
	
	for (int x = 0; x < NEIGHBORHOOD_SIZE; x++)
	{
		int RealX = (BaseX + x) * m_GridSize;
		for (int z = 0; z < NEIGHBORHOOD_SIZE; z++)
		{
			int RealZ = (BaseZ + z) * m_GridSize;
			bool Found = false;
			for (cMineShaftSystems::const_iterator itr = a_MineShafts.begin(), end = a_MineShafts.end(); itr != end; ++itr)
			{
				if (((*itr)->m_BlockX == RealX) && ((*itr)->m_BlockZ == RealZ))
				{
					Found = true;
					break;
				}
			}  // for itr - a_Mineshafts
			if (!Found)
			{
				a_MineShafts.push_back(new cMineShaftSystem(RealX, RealZ, m_MaxSystemSize, m_Noise));
			}
		}  // for z
	}  // for x
	
	// Copy a_MineShafts into m_Cache to the beginning:
	cMineShaftSystems MineShaftsCopy(a_MineShafts);
	m_Cache.splice(m_Cache.begin(), MineShaftsCopy, MineShaftsCopy.begin(), MineShaftsCopy.end());
	
	// Trim the cache if it's too long:
	if (m_Cache.size() > 100)
	{
		cMineShaftSystems::iterator itr = m_Cache.begin();
		std::advance(itr, 100);
		for (cMineShaftSystems::iterator end = m_Cache.end(); itr != end; ++itr)
		{
			delete *itr;
		}
		itr = m_Cache.begin();
		std::advance(itr, 100);
		m_Cache.erase(itr, m_Cache.end());
	}
}






void cStructGenMineShafts::GenStructures(cChunkDesc & a_ChunkDesc)
{
	int ChunkX = a_ChunkDesc.GetChunkX();
	int ChunkZ = a_ChunkDesc.GetChunkZ();
	cMineShaftSystems MineShafts;
	GetMineShaftSystemsForChunk(ChunkX, ChunkZ, MineShafts);
	for (cMineShaftSystems::const_iterator itr = MineShafts.begin(); itr != MineShafts.end(); ++itr)
	{
		(*itr)->ProcessChunk(a_ChunkDesc);
	}  // for itr - MineShafts[]
}




