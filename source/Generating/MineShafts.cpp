
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





static const int NEIGHBORHOOD_SIZE = 1;





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
	
	
	eKind   m_Kind;
	cCuboid m_BoundingBox;
	cStructGenMineShafts::cMineShaftSystem & m_ParentSystem;


	cMineShaft(cStructGenMineShafts::cMineShaftSystem & a_ParentSystem, eKind a_Kind) :
		m_ParentSystem(a_ParentSystem),
		m_Kind(a_Kind)
	{
	}
	
	/// Returns true if this mineshaft intersects the specified cuboid
	bool DoesIntersect(const cCuboid & a_Other)
	{
		return m_BoundingBox.DoesIntersect(a_Other);
	}
	
	/** If recursion level is not too large, appends more branches to the parent system,
	using exit points specific to this class.
	*/
	virtual void AppendBranches(int a_RecursionLevel, cNoise & a_Noise) = 0;
	
	/// Imprints this shape into the specified chunk's data
	virtual void ProcessChunk(cChunkDesc & a_ChunkDesc) = 0;
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
	
protected:
	static const int MAX_SEGMENTS = 5;
	
	int        m_NumSegments;
	eDirection m_Direction;
	bool       m_HasFullBeam[MAX_SEGMENTS];  // If true, segment at that index has a full beam support (planks in the top center block)
	
	cMineShaftCorridor(
		cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
		const cCuboid & a_BoundingBox, int a_NumSegments, eDirection a_Direction,
		cNoise & a_Noise
	);
	
	// cMineShaft overrides:
	virtual void AppendBranches(int a_RecursionLevel, cNoise & a_Noise) override;
	virtual void ProcessChunk(cChunkDesc & a_ChunkDesc) override;
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
	
protected:
	// TODO
	
	// cMineShaft overrides:
	virtual void AppendBranches(int a_RecursionLevel, cNoise & a_Noise) override;
	virtual void ProcessChunk(cChunkDesc & a_ChunkDesc) override;
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
	
protected:
	// TODO

	// cMineShaft overrides:
	virtual void AppendBranches(int a_RecursionLevel, cNoise & a_Noise) override;
	virtual void ProcessChunk(cChunkDesc & a_ChunkDesc) override;
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
	cMineShaftSystem(
		int a_BlockX, int a_BlockZ, int a_MaxSystemSize, cNoise & a_Noise,
		int a_ChanceCorridor, int a_ChanceCrossing, int a_ChanceStaircase
	);

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
	
	/// Returns true if any of the objects in m_MineShafts intersects with the specified bounding box
	bool DoIntersect(const cCuboid & a_BoundingBox);
} ;





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cStructGenMineShafts::cMineShaftSystem:

cStructGenMineShafts::cMineShaftSystem::cMineShaftSystem(
	int a_BlockX, int a_BlockZ, int a_MaxSystemSize, cNoise & a_Noise,
	int a_ChanceCorridor, int a_ChanceCrossing, int a_ChanceStaircase
) :
	m_BlockX(a_BlockX),
	m_BlockZ(a_BlockZ),
	m_MaxSystemSize(a_MaxSystemSize),
	m_MaxRecursion(8),  // TODO: settable
	m_ChanceCorridor(a_ChanceCorridor),
	m_ChanceCrossing(a_ChanceCrossing),
	m_ChanceStaircase(a_ChanceStaircase)
{
	m_MineShafts.reserve(100);
	
	cMineShaft * Start = new cMineShaftDirtRoom(*this, a_Noise);
	m_MineShafts.push_back(Start);
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





bool cStructGenMineShafts::cMineShaftSystem::DoIntersect(const cCuboid & a_BoundingBox)
{
	for (cMineShafts::const_iterator itr = m_MineShafts.begin(), end = m_MineShafts.end(); itr != end; ++itr)
	{
		if ((*itr)->DoesIntersect(a_BoundingBox))
		{
			return true;
		}
	}  // for itr - m_MineShafts[]
	return false;
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

cMineShaftCorridor::cMineShaftCorridor(
	cStructGenMineShafts::cMineShaftSystem & a_ParentSystem,
	const cCuboid & a_BoundingBox, int a_NumSegments, eDirection a_Direction,
	cNoise & a_Noise
) :
	super(a_ParentSystem, mskCorridor),
	m_NumSegments(a_NumSegments),
	m_Direction(a_Direction)
{
	m_BoundingBox = a_BoundingBox;
	int rnd = a_Noise.IntNoise3DInt(a_BoundingBox.p1.x, a_BoundingBox.p1.y, a_BoundingBox.p1.z) / 7;
	for (int i = 0; i < a_NumSegments; i++)
	{
		m_HasFullBeam[i] = (rnd % 4) < 3;  // 75 % chance of full beam
		rnd >>= 2;
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
	int rnd = a_Noise.IntNoise3DInt(a_PivotX, a_PivotY + a_ParentSystem.m_MineShafts.size(), a_PivotZ) / 7;
	int NumSegments = 2 + (rnd) % (MAX_SEGMENTS - 1);  // 2 .. MAX_SEGMENTS
	switch (a_Direction)
	{
		case dirXP: BoundingBox.p2.x += NumSegments * 5; BoundingBox.p1.z -= 1; BoundingBox.p2.z += 1; break;
		case dirXM: BoundingBox.p1.x -= NumSegments * 5; BoundingBox.p1.z -= 1; BoundingBox.p2.z += 1; break;
		case dirZP: BoundingBox.p2.z += NumSegments * 5; BoundingBox.p1.x -= 1; BoundingBox.p2.x += 1; break;
		case dirZM: BoundingBox.p1.z -= NumSegments * 5; BoundingBox.p1.x -= 1; BoundingBox.p2.x += 1; break;
	}
	if (a_ParentSystem.DoIntersect(BoundingBox))
	{
		return NULL;
	}
	return new cMineShaftCorridor(a_ParentSystem, BoundingBox, NumSegments, a_Direction, a_Noise);
}





void cMineShaftCorridor::AppendBranches(int a_RecursionLevel, cNoise & a_Noise)
{
	int rnd = a_Noise.IntNoise3DInt(m_BoundingBox.p1.x, m_BoundingBox.p1.y + a_RecursionLevel, m_BoundingBox.p1.z) / 7;
	// Prefer the same height, but allow for up to one block height displacement:
	int Height = m_BoundingBox.p1.y + ((rnd % 4) + ((rnd >> 3) % 3)) / 2;
	switch (m_Direction)
	{
		case dirXM:
		{
			m_ParentSystem.AppendBranch(m_BoundingBox.p1.x - 1, Height, m_BoundingBox.p1.z + 1, dirXM, a_Noise, a_RecursionLevel);
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
			m_ParentSystem.AppendBranch(m_BoundingBox.p2.x + 1, Height, m_BoundingBox.p1.z + 1, dirXP, a_Noise, a_RecursionLevel);
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
			m_ParentSystem.AppendBranch(m_BoundingBox.p1.x + 1, Height, m_BoundingBox.p1.z - 1, dirZM, a_Noise, a_RecursionLevel);
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
			m_ParentSystem.AppendBranch(m_BoundingBox.p1.x + 1, Height, m_BoundingBox.p2.z + 1, dirZP, a_Noise, a_RecursionLevel);
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
	RelBoundingBox.Move(-BlockX, 0, -BlockZ);
	RelBoundingBox.p1.y += 1;
	a_ChunkDesc.FillRelCuboid(RelBoundingBox, E_BLOCK_AIR, 0);
	RelBoundingBox.p1.y -= 1;
	RelBoundingBox.p2.y = RelBoundingBox.p1.y;
	a_ChunkDesc.ReplaceRelCuboid(RelBoundingBox, E_BLOCK_AIR, 0, E_BLOCK_PLANKS, 0);
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
				int x = m_BoundingBox.p1.x + i * 5 + 3 - BlockX;
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
				if ((z1 > -1) && (z1 < cChunkDef::Width - 1) && m_HasFullBeam[i])
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
				int z = m_BoundingBox.p1.z + i * 5 + 3 - BlockZ;
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
				if ((x1 > -1) && (x1 < cChunkDef::Width - 1) && m_HasFullBeam[i])
				{
					a_ChunkDesc.SetBlockTypeMeta(x1 + 1, y3, z, E_BLOCK_PLANKS, 0);
				}
			}  // for i - NumSegments
			break;
		}  // case dirZ?
	}  // for i
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

cStructGenMineShafts::cStructGenMineShafts(
	int a_Seed, int a_GridSize, int a_MaxSystemSize,
	int a_ChanceCorridor, int a_ChanceCrossing, int a_ChanceStaircase
) :
	m_Noise(a_Seed),
	m_GridSize(a_GridSize),
	m_MaxSystemSize(a_MaxSystemSize),
	m_ChanceCorridor(a_ChanceCorridor),
	m_ChanceCrossing(a_ChanceCorridor + a_ChanceCrossing),
	m_ChanceStaircase(a_ChanceCorridor + a_ChanceCrossing + a_ChanceStaircase)
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
				a_MineShafts.push_back(new cMineShaftSystem(RealX, RealZ, m_MaxSystemSize, m_Noise, m_ChanceCorridor, m_ChanceCrossing, m_ChanceStaircase));
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




