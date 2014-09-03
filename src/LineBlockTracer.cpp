
// LineBlockTracer.cpp

// Implements the cLineBlockTracer class representing a cBlockTracer that traces along a straight line between two points

#include "Globals.h"
#include "LineBlockTracer.h"
#include "Vector3.h"
#include "World.h"
#include "Chunk.h"






cLineBlockTracer::cLineBlockTracer(cWorld & a_World, cCallbacks & a_Callbacks) :
	super(a_World, a_Callbacks)
{
}





bool cLineBlockTracer::Trace(cWorld & a_World, cBlockTracer::cCallbacks & a_Callbacks, const Vector3d & a_Start, const Vector3d & a_End)
{
	cLineBlockTracer Tracer(a_World, a_Callbacks);
	return Tracer.Trace(a_Start.x, a_Start.y, a_Start.z, a_End.x, a_End.y, a_End.z);
}





bool cLineBlockTracer::Trace(cWorld & a_World, cBlockTracer::cCallbacks &a_Callbacks, double a_StartX, double a_StartY, double a_StartZ, double a_EndX, double a_EndY, double a_EndZ)
{
	cLineBlockTracer Tracer(a_World, a_Callbacks);
	return Tracer.Trace(a_StartX, a_StartY, a_StartZ, a_EndX, a_EndY, a_EndZ);
}





bool cLineBlockTracer::Trace(double a_StartX, double a_StartY, double a_StartZ, double a_EndX, double a_EndY, double a_EndZ)
{
	// Initialize the member veriables:
	m_StartX = a_StartX;
	m_StartY = a_StartY;
	m_StartZ = a_StartZ;
	m_EndX = a_EndX;
	m_EndY = a_EndY;
	m_EndZ = a_EndZ;
	m_DirX = (m_StartX < m_EndX) ? 1 : -1;
	m_DirY = (m_StartY < m_EndY) ? 1 : -1;
	m_DirZ = (m_StartZ < m_EndZ) ? 1 : -1;
	m_CurrentFace = BLOCK_FACE_NONE;
	
	// Check the start coords, adjust into the world:
	if (m_StartY < 0)
	{
		if (m_EndY < 0)
		{
			// Nothing to trace
			m_Callbacks->OnNoMoreHits();
			return true;
		}
		FixStartBelowWorld();
		m_Callbacks->OnIntoWorld(m_StartX, m_StartY, m_StartZ);
	}
	else if (m_StartY >= cChunkDef::Height)
	{
		if (m_EndY >= cChunkDef::Height)
		{
			m_Callbacks->OnNoMoreHits();
			return true;
		}
		FixStartAboveWorld();
		m_Callbacks->OnIntoWorld(m_StartX, m_StartY, m_StartZ);
	}

	m_CurrentX = (int)floor(m_StartX);
	m_CurrentY = (int)floor(m_StartY);
	m_CurrentZ = (int)floor(m_StartZ);
	
	m_DiffX = m_EndX - m_StartX;
	m_DiffY = m_EndY - m_StartY;
	m_DiffZ = m_EndZ - m_StartZ;
	
	// The actual trace is handled with ChunkMapCS locked by calling our Item() for the specified chunk
	int BlockX = (int)floor(m_StartX);
	int BlockZ = (int)floor(m_StartZ);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(BlockX, BlockZ, ChunkX, ChunkZ);
	return m_World->DoWithChunk(ChunkX, ChunkZ, *this);
}





void cLineBlockTracer::FixStartAboveWorld(void)
{
	// We must set the start Y to less than cChunkDef::Height so that it is considered inside the world later on
	// Therefore we use an EPS-offset from the height, as small as reasonably possible.
	const double Height = (double)cChunkDef::Height - 0.00001;
	CalcXZIntersection(Height, m_StartX, m_StartZ);
	m_StartY = Height;
}





void cLineBlockTracer::FixStartBelowWorld(void)
{
	CalcXZIntersection(0, m_StartX, m_StartZ);
	m_StartY = 0;
}





void cLineBlockTracer::CalcXZIntersection(double a_Y, double & a_IntersectX, double & a_IntersectZ)
{
	double Ratio = (m_StartY - a_Y) / (m_StartY - m_EndY);
	a_IntersectX = m_StartX + (m_EndX - m_StartX) * Ratio;
	a_IntersectZ = m_StartZ + (m_EndZ - m_StartZ) * Ratio;
}





bool cLineBlockTracer::MoveToNextBlock(void)
{
	// Find out which of the current block's walls gets hit by the path:
	static const double EPS = 0.00001;
	double Coeff = 1;
	enum eDirection
	{
		dirNONE,
		dirX,
		dirY,
		dirZ,
	} Direction = dirNONE;
	if (abs(m_DiffX) > EPS)
	{
		double DestX = (m_DirX > 0) ? (m_CurrentX + 1) : m_CurrentX;
		Coeff = (DestX - m_StartX) / m_DiffX;
		if (Coeff <= 1)
		{
			Direction = dirX;
		}
	}
	if (abs(m_DiffY) > EPS)
	{
		double DestY = (m_DirY > 0) ? (m_CurrentY + 1) : m_CurrentY;
		double CoeffY = (DestY - m_StartY) / m_DiffY;
		if (CoeffY < Coeff)
		{
			Coeff = CoeffY;
			Direction = dirY;
		}
	}
	if (abs(m_DiffZ) > EPS)
	{
		double DestZ = (m_DirZ > 0) ? (m_CurrentZ + 1) : m_CurrentZ;
		double CoeffZ = (DestZ - m_StartZ) / m_DiffZ;
		if (CoeffZ < Coeff)
		{
			Direction = dirZ;
		}
	}
	
	// Based on the wall hit, adjust the current coords
	switch (Direction)
	{
		case dirX:    m_CurrentX += m_DirX; m_CurrentFace = (m_DirX > 0) ? BLOCK_FACE_XM : BLOCK_FACE_XP; break;
		case dirY:    m_CurrentY += m_DirY; m_CurrentFace = (m_DirY > 0) ? BLOCK_FACE_YM : BLOCK_FACE_YP; break;
		case dirZ:    m_CurrentZ += m_DirZ; m_CurrentFace = (m_DirZ > 0) ? BLOCK_FACE_ZM : BLOCK_FACE_ZP; break;
		case dirNONE: return false;
	}
	return true;
}





bool cLineBlockTracer::Item(cChunk * a_Chunk)
{
	ASSERT((m_CurrentY >= 0) && (m_CurrentY < cChunkDef::Height));  // This should be provided by FixStartAboveWorld() / FixStartBelowWorld()
	
	// This is the actual line tracing loop.
	for (;;)
	{
		// Report the current block through the callbacks:
		if (a_Chunk == NULL)
		{
			m_Callbacks->OnNoChunk();
			return false;
		}

		// Move to next block
		if (!MoveToNextBlock())
		{
			// We've reached the end
			m_Callbacks->OnNoMoreHits();
			return true;
		}

		if ((m_CurrentY < 0) || (m_CurrentY >= cChunkDef::Height))
		{
			// We've gone out of the world, that's the end of this trace
			double IntersectX, IntersectZ;
			CalcXZIntersection(m_CurrentY, IntersectX, IntersectZ);
			if (m_Callbacks->OnOutOfWorld(IntersectX, m_CurrentY, IntersectZ))
			{
				// The callback terminated the trace
				return false;
			}
			m_Callbacks->OnNoMoreHits();
			return true;
		}

		// Update the current chunk
		if (a_Chunk != NULL)
		{
			a_Chunk = a_Chunk->GetNeighborChunk(m_CurrentX, m_CurrentZ);
		}

		if (a_Chunk->IsValid())
		{
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			int RelX = m_CurrentX - a_Chunk->GetPosX() * cChunkDef::Width;
			int RelZ = m_CurrentZ - a_Chunk->GetPosZ() * cChunkDef::Width;
			a_Chunk->GetBlockTypeMeta(RelX, m_CurrentY, RelZ, BlockType, BlockMeta);
			if (m_Callbacks->OnNextBlock(m_CurrentX, m_CurrentY, m_CurrentZ, BlockType, BlockMeta, m_CurrentFace))
			{
				// The callback terminated the trace
				return false;
			}
		}
		else
		{
			if (m_Callbacks->OnNextBlockNoData(m_CurrentX, m_CurrentY, m_CurrentZ, m_CurrentFace))
			{
				// The callback terminated the trace
				return false;
			}
		}
	}
}




