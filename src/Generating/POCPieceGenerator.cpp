
// POCPieceGenerator.cpp

// Implements the cPOCPieceGenerator class representing a Proof-Of_Concept structure generator using the cPieceGenerator technique
// The generator generates a maze of rooms at {0, 50, 0}

#include "Globals.h"
#include "POCPieceGenerator.h"
#include "ChunkDesc.h"





/** POC pieces are simple boxes that have connectors in the middle of their walls.
Each wall has one connector, there are 3 connector types that get assigned semi-randomly.
The piece also knows how to imprint itself in a cChunkDesc, each piece has a different color glass
and each connector is uses a different color wool frame. */
class cPOCPiece :
	public cPiece
{
public:
	cPOCPiece(int a_SizeXZ, int a_Height) :
		m_SizeXZ(a_SizeXZ),
		m_Height(a_Height)
	{
		m_Connectors.push_back(cConnector(m_SizeXZ / 2, a_Height / 2, 0,            0,            BLOCK_FACE_ZM));
		m_Connectors.push_back(cConnector(m_SizeXZ / 2, a_Height / 2, m_SizeXZ - 1, 1,            BLOCK_FACE_ZP));
		m_Connectors.push_back(cConnector(0,            a_Height / 2, m_SizeXZ / 2, 2,            BLOCK_FACE_XM));
		m_Connectors.push_back(cConnector(m_SizeXZ - 1, a_Height - 1, m_SizeXZ / 2, m_SizeXZ % 3, BLOCK_FACE_XP));
	}
	
	
	/** Imprints the piece in the specified chunk. Assumes they intersect. */
	void ImprintInChunk(cChunkDesc & a_ChunkDesc, const Vector3i & a_Pos, int a_NumCCWRotations)
	{
		int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
		int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
		Vector3i Min = a_Pos;
		Min.Move(-BlockX, 0, -BlockZ);
		Vector3i Max = Min;
		Max.Move(m_SizeXZ - 1, m_Height - 1, m_SizeXZ - 1);
		ASSERT(Min.x < cChunkDef::Width);
		ASSERT(Min.z < cChunkDef::Width);
		ASSERT(Max.x >= 0);
		ASSERT(Max.z >= 0);
		if (Min.x >= 0)
		{
			// Draw the XM wall:
			a_ChunkDesc.FillRelCuboid(Min.x, Min.x, Min.y, Max.y, Min.z, Max.z, E_BLOCK_STAINED_GLASS, m_SizeXZ % 16);
		}
		if (Min.z >= 0)
		{
			// Draw the ZM wall:
			a_ChunkDesc.FillRelCuboid(Min.x, Max.x, Min.y, Max.y, Min.z, Min.z, E_BLOCK_STAINED_GLASS, m_SizeXZ % 16);
		}
		if (Max.x < cChunkDef::Width)
		{
			// Draw the XP wall:
			a_ChunkDesc.FillRelCuboid(Max.x, Max.x, Min.y, Max.y, Min.z, Max.z, E_BLOCK_STAINED_GLASS, m_SizeXZ % 16);
		}
		if (Max.z < cChunkDef::Width)
		{
			// Draw the ZP wall:
			a_ChunkDesc.FillRelCuboid(Min.x, Max.x, Min.y, Max.y, Max.z, Max.z, E_BLOCK_STAINED_GLASS, m_SizeXZ % 16);
		}
		
		// Draw all the connectors:
		for (cConnectors::const_iterator itr = m_Connectors.begin(), end = m_Connectors.end(); itr != end; ++itr)
		{
			cConnector Conn = cPiece::RotateMoveConnector(*itr, a_NumCCWRotations, a_Pos.x, a_Pos.y, a_Pos.z);
			Conn.m_Pos.Move(-BlockX, 0, -BlockZ);
			if (
				(Conn.m_Pos.x >= 0) && (Conn.m_Pos.x < cChunkDef::Width) &&
				(Conn.m_Pos.z >= 0) && (Conn.m_Pos.z < cChunkDef::Width)
			)
			{
				a_ChunkDesc.SetBlockTypeMeta(Conn.m_Pos.x, Conn.m_Pos.y, Conn.m_Pos.z, E_BLOCK_WOOL, itr->m_Type % 16);
			}
			
			/*
			// TODO: Frame the connectors
			switch (itr->m_Direction)
			{
				case BLOCK_FACE_XM:
				case BLOCK_FACE_XP:
				{
					// TODO
					break;
				}
				
				case BLOCK_FACE_ZM:
				case BLOCK_FACE_ZP:
				{
					// TODO
					break;
				}
			}
			*/
		}  // for itr - m_Connectors[]
	}
	
protected:
	int m_SizeXZ;
	int m_Height;
	cConnectors m_Connectors;
	
	// cPiece overrides:
	virtual cConnectors GetConnectors(void) const override
	{
		return m_Connectors;
	}
	
	virtual Vector3i GetSize(void) const override
	{
		return Vector3i(m_SizeXZ, m_Height, m_SizeXZ);
	}
	
	virtual cCuboid GetHitBox(void) const override
	{
		return cCuboid(0, 0, 0, m_SizeXZ - 1, m_Height - 1, m_SizeXZ - 1);
	}
	
	virtual bool CanRotateCCW(int a_NumRotations) const override
	{
		return true;
	}
};





/*
static void DebugPieces(const cPlacedPieces & a_Pieces)
{
	size_t idx = 0;
	for (cPlacedPieces::const_iterator itr = a_Pieces.begin(), end = a_Pieces.end(); itr != end; ++itr, ++idx)
	{
		const cCuboid & HitBox = (*itr)->GetHitBox();
		printf("  %u: %d rotations, {%d - %d, %d - %d}\n",
			idx, (*itr)->GetNumCCWRotations(),
			HitBox.p1.x, HitBox.p2.x, HitBox.p1.z, HitBox.p2.z
		);
	}  // for itr - a_Pieces[]
}
//*/





////////////////////////////////////////////////////////////////////////////////
// cPOCPieceGenerator:

cPOCPieceGenerator::cPOCPieceGenerator(int a_Seed) :
	m_Seed(a_Seed)
{
	// Prepare a vector of available pieces:
	m_AvailPieces.push_back(new cPOCPiece(5, 3));
	m_AvailPieces.push_back(new cPOCPiece(7, 5));
	m_AvailPieces.push_back(new cPOCPiece(9, 5));
	m_AvailPieces.push_back(new cPOCPiece(5, 7));
	
	// Generate the structure:
	cBFSPieceGenerator Gen(*this, a_Seed);
	Gen.PlacePieces(0, 50, 0, 6, m_Pieces);
	
	// DebugPieces(m_Pieces);
	
	// Get the smallest cuboid encompassing the entire generated structure:
	cCuboid Bounds(0, 50, 0, 0, 50, 0);
	for (cPlacedPieces::const_iterator itr = m_Pieces.begin(), end = m_Pieces.end(); itr != end; ++itr)
	{
		Vector3i MinCoords = (*itr)->GetCoords();
		Bounds.Engulf(MinCoords);
		Bounds.Engulf(MinCoords + (*itr)->GetPiece().GetSize());
	}  // for itr - m_Pieces[]
	m_Bounds = Bounds;
}





cPOCPieceGenerator::~cPOCPieceGenerator()
{
	cPieceGenerator::FreePieces(m_Pieces);
	for (cPieces::iterator itr = m_AvailPieces.begin(), end = m_AvailPieces.end(); itr != end; ++itr)
	{
		delete *itr;
	}
	m_AvailPieces.clear();
}





void cPOCPieceGenerator::GenFinish(cChunkDesc & a_ChunkDesc)
{
	int BlockX = a_ChunkDesc.GetChunkX() * cChunkDef::Width;
	int BlockZ = a_ChunkDesc.GetChunkZ() * cChunkDef::Width;
	if (
		(BlockX + 16 < m_Bounds.p1.x) || (BlockX > m_Bounds.p2.x) ||  // X coords out of bounds of the generated structure
		(BlockZ + 16 < m_Bounds.p1.z) || (BlockZ > m_Bounds.p2.z)     // Z coords out of bounds of the generated structure
	)
	{
		return;
	}
	
	// Imprint each piece in the chunk:
	for (cPlacedPieces::const_iterator itr = m_Pieces.begin(), end = m_Pieces.end(); itr != end; ++itr)
	{
		const Vector3i & Pos = (*itr)->GetCoords();
		Vector3i Size = (*itr)->GetPiece().GetSize();
		if (((*itr)->GetNumCCWRotations() % 2) == 1)
		{
			std::swap(Size.x, Size.z);
		}
		if (
			(Pos.x >= BlockX + 16) || (Pos.x + Size.x - 1 < BlockX) ||
			(Pos.z >= BlockZ + 16) || (Pos.z + Size.z - 1 < BlockZ)
		)
		{
			// This piece doesn't intersect the chunk
			continue;
		}
			
		((cPOCPiece &)(*itr)->GetPiece()).ImprintInChunk(a_ChunkDesc, Pos, (*itr)->GetNumCCWRotations());
	}  // for itr - m_Pieces[]
	a_ChunkDesc.UpdateHeightmap();
}





cPieces cPOCPieceGenerator::GetPiecesWithConnector(int a_ConnectorType)
{
	// Each piece has each connector
	return m_AvailPieces;
}





cPieces cPOCPieceGenerator::GetStartingPieces(void)
{
	// Any piece can be a starting piece
	return m_AvailPieces;
}





void cPOCPieceGenerator::PiecePlaced(const cPiece & a_Piece)
{
	UNUSED(a_Piece);
}





void cPOCPieceGenerator::Reset(void)
{
	// Nothing needed
}




