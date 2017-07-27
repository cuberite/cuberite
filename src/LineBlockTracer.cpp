
// LineBlockTracer.cpp

// Implements the cLineBlockTracer class representing a cBlockTracer that traces along a straight line between two points

#include "Globals.h"
#include "LineBlockTracer.h"
#include "World.h"
#include "Chunk.h"
#include "BoundingBox.h"






cLineBlockTracer::cLineBlockTracer(cWorld & a_World, cCallbacks & a_Callbacks) :
	super(a_World, a_Callbacks),
	m_StartX(0.0),
	m_StartY(0.0),
	m_StartZ(0.0),
	m_EndX(0.0),
	m_EndY(0.0),
	m_EndZ(0.0),
	m_DiffX(0.0),
	m_DiffY(0.0),
	m_DiffZ(0.0),
	m_DirX(0),
	m_DirY(0),
	m_DirZ(0),
	m_CurrentX(0),
	m_CurrentY(0),
	m_CurrentZ(0),
	m_CurrentFace(BLOCK_FACE_NONE)
{
}





bool cLineBlockTracer::Trace(cWorld & a_World, cBlockTracer::cCallbacks & a_Callbacks, const Vector3d & a_Start, const Vector3d & a_End)
{
	cLineBlockTracer Tracer(a_World, a_Callbacks);
	return Tracer.Trace(a_Start.x, a_Start.y, a_Start.z, a_End.x, a_End.y, a_End.z);
}





bool cLineBlockTracer::LineOfSightTrace(cWorld & a_World, const Vector3d & a_Start, const Vector3d & a_End, int a_Sight)
{
	static class LineOfSightCallbacks:
		public cLineBlockTracer::cCallbacks
	{
		bool m_IsAirOpaque;
		bool m_IsWaterOpaque;
		bool m_IsLavaOpaque;
	public:
		LineOfSightCallbacks(bool a_IsAirOpaque, bool a_IsWaterOpaque, bool a_IsLavaOpaque):
			m_IsAirOpaque(a_IsAirOpaque),
			m_IsWaterOpaque(a_IsWaterOpaque),
			m_IsLavaOpaque(a_IsLavaOpaque)
		{}

		virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) override
		{
			switch (a_BlockType)
			{
				case E_BLOCK_AIR:              return m_IsAirOpaque;
				case E_BLOCK_LAVA:             return m_IsLavaOpaque;
				case E_BLOCK_STATIONARY_LAVA:  return m_IsLavaOpaque;
				case E_BLOCK_STATIONARY_WATER: return m_IsWaterOpaque;
				case E_BLOCK_WATER:            return m_IsWaterOpaque;
				default: return true;
			}
		}
	} callbacks(
		(a_Sight & losAir) == 0,
		(a_Sight & losWater) == 0,
		(a_Sight & losLava) == 0
	);
	return Trace(a_World, callbacks, a_Start, a_End);
}





bool cLineBlockTracer::FirstSolidHitTrace(
	cWorld & a_World,
	const Vector3d & a_Start, const Vector3d & a_End,
	Vector3d & a_HitCoords,
	Vector3i & a_HitBlockCoords, eBlockFace & a_HitBlockFace
)
{
	class cSolidHitCallbacks:
		public cCallbacks
	{
	public:
		cSolidHitCallbacks(const Vector3d & a_CBStart, const Vector3d & a_CBEnd, Vector3d & a_CBHitCoords, Vector3i & a_CBHitBlockCoords, eBlockFace & a_CBHitBlockFace):
			m_Start(a_CBStart),
			m_End(a_CBEnd),
			m_HitCoords(a_CBHitCoords),
			m_HitBlockCoords(a_CBHitBlockCoords),
			m_HitBlockFace(a_CBHitBlockFace)
		{
		}

		virtual bool OnNextBlock(int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) override
		{
			if (!cBlockInfo::IsSolid(a_BlockType))
			{
				return false;
			}

			// We hit a solid block, calculate the exact hit coords and abort trace:
			m_HitBlockCoords.Set(a_BlockX, a_BlockY, a_BlockZ);
			m_HitBlockFace = a_EntryFace;
			cBoundingBox bb(a_BlockX, a_BlockX + 1, a_BlockY, a_BlockY + 1, a_BlockZ, a_BlockZ + 1);  // Bounding box of the block hit
			double LineCoeff = 0;  // Used to calculate where along the line an intersection with the bounding box occurs
			eBlockFace Face;  // Face hit
			if (!bb.CalcLineIntersection(m_Start, m_End, LineCoeff, Face))
			{
				// Math rounding errors have caused the calculation to miss the block completely, assume immediate hit
				LineCoeff = 0;
			}
			m_HitCoords = m_Start + (m_End - m_Start) * LineCoeff;  // Point where projectile goes into the hit block
			return true;
		}

	protected:
		const Vector3d & m_Start;
		const Vector3d & m_End;
		Vector3d & m_HitCoords;
		Vector3i & m_HitBlockCoords;
		eBlockFace & m_HitBlockFace;
	} callbacks(a_Start, a_End, a_HitCoords, a_HitBlockCoords, a_HitBlockFace);
	return !Trace(a_World, callbacks, a_Start, a_End);
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

	m_CurrentX = FloorC(m_StartX);
	m_CurrentY = FloorC(m_StartY);
	m_CurrentZ = FloorC(m_StartZ);

	m_DiffX = m_EndX - m_StartX;
	m_DiffY = m_EndY - m_StartY;
	m_DiffZ = m_EndZ - m_StartZ;

	// The actual trace is handled with ChunkMapCS locked by calling our ChunkCallback for the specified chunk
	int BlockX = FloorC(m_StartX);
	int BlockZ = FloorC(m_StartZ);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(BlockX, BlockZ, ChunkX, ChunkZ);
	return m_World->DoWithChunk(ChunkX, ChunkZ, [this](cChunk & a_Chunk) { return ChunkCallback(&a_Chunk); });
}





void cLineBlockTracer::FixStartAboveWorld(void)
{
	// We must set the start Y to less than cChunkDef::Height so that it is considered inside the world later on
	// Therefore we use an EPS-offset from the height, as small as reasonably possible.
	const double Height = static_cast<double>(cChunkDef::Height) - 0.00001;
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
	enum
	{
		dirNONE,
		dirX,
		dirY,
		dirZ,
	} Direction = dirNONE;

	// Calculate the next YZ wall hit:
	double Coeff = 1;
	if (std::abs(m_DiffX) > EPS)
	{
		double DestX = (m_DirX > 0) ? (m_CurrentX + 1) : m_CurrentX;
		double CoeffX = (DestX - m_StartX) / m_DiffX;
		if (CoeffX <= 1)  // We need to include equality for the last block in the trace
		{
			Coeff = CoeffX;
			Direction = dirX;
		}
	}

	// If the next XZ wall hit is closer, use it instead:
	if (std::abs(m_DiffY) > EPS)
	{
		double DestY = (m_DirY > 0) ? (m_CurrentY + 1) : m_CurrentY;
		double CoeffY = (DestY - m_StartY) / m_DiffY;
		if (CoeffY <= Coeff)  // We need to include equality for the last block in the trace
		{
			Coeff = CoeffY;
			Direction = dirY;
		}
	}

	// If the next XY wall hit is closer, use it instead:
	if (std::abs(m_DiffZ) > EPS)
	{
		double DestZ = (m_DirZ > 0) ? (m_CurrentZ + 1) : m_CurrentZ;
		double CoeffZ = (DestZ - m_StartZ) / m_DiffZ;
		if (CoeffZ <= Coeff)  // We need to include equality for the last block in the trace
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





bool cLineBlockTracer::ChunkCallback(cChunk * a_Chunk)
{
	ASSERT((m_CurrentY >= 0) && (m_CurrentY < cChunkDef::Height));  // This should be provided by FixStartAboveWorld() / FixStartBelowWorld()

	// This is the actual line tracing loop.
	for (;;)
	{
		// Report the current block through the callbacks:
		if (a_Chunk == nullptr)
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
		a_Chunk = a_Chunk->GetNeighborChunk(m_CurrentX, m_CurrentZ);
		if (a_Chunk == nullptr)
		{
			m_Callbacks->OnNoChunk();
			return false;
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
		else if (m_Callbacks->OnNextBlockNoData(m_CurrentX, m_CurrentY, m_CurrentZ, m_CurrentFace))
		{
			// The callback terminated the trace
			return false;
		}
	}
}




