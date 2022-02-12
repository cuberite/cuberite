
// LineBlockTracer.cpp

// Implements the cLineBlockTracer class representing a cBlockTracer that traces along a straight line between two points

#include "Globals.h"
#include "LineBlockTracer.h"
#include "BlockInfo.h"
#include "World.h"
#include "Chunk.h"
#include "BoundingBox.h"





cLineBlockTracer::cLineBlockTracer(cWorld & a_World, cCallbacks & a_Callbacks) :
	Super(a_World, a_Callbacks),
	m_Start(),
	m_End(),
	m_Diff(),
	m_Dir(),
	m_Current(),
	m_CurrentFace(BLOCK_FACE_NONE)
{
}





bool cLineBlockTracer::Trace(cWorld & a_World, cBlockTracer::cCallbacks & a_Callbacks, const Vector3d a_Start, const Vector3d a_End)
{
	cLineBlockTracer Tracer(a_World, a_Callbacks);
	return Tracer.Trace(a_Start, a_End);
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

		virtual bool OnNextBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) override
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

		virtual bool OnNextBlock(Vector3i a_BlockPos, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta, eBlockFace a_EntryFace) override
		{
			if (!cBlockInfo::IsSolid(a_BlockType))
			{
				return false;
			}

			// We hit a solid block, calculate the exact hit coords and abort trace:
			m_HitBlockCoords = a_BlockPos;
			m_HitBlockFace = a_EntryFace;
			cBoundingBox bb(a_BlockPos, a_BlockPos + Vector3i(1, 1, 1));  // Bounding box of the block hit
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





bool cLineBlockTracer::Trace(const Vector3d a_Start, const Vector3d a_End)
{
	// Initialize the member veriables:
	m_Start = a_Start;
	m_End = a_End;
	m_Dir.x = (m_Start.x < m_End.x) ? 1 : -1;
	m_Dir.y = (m_Start.y < m_End.y) ? 1 : -1;
	m_Dir.z = (m_Start.z < m_End.z) ? 1 : -1;
	m_CurrentFace = BLOCK_FACE_NONE;

	// Check the start coords, adjust into the world:
	if (m_Start.y < 0)
	{
		if (m_End.y < 0)
		{
			// Nothing to trace
			m_Callbacks->OnNoMoreHits();
			return true;
		}
		FixStartBelowWorld();
		m_Callbacks->OnIntoWorld(m_Start);
	}
	else if (m_Start.y >= cChunkDef::Height)
	{
		if (m_End.y >= cChunkDef::Height)
		{
			m_Callbacks->OnNoMoreHits();
			return true;
		}
		FixStartAboveWorld();
		m_Callbacks->OnIntoWorld(m_Start);
	}

	m_Current = m_Start.Floor();

	m_Diff = m_End -  m_Start;

	// The actual trace is handled with ChunkMapCS locked by calling our ChunkCallback for the specified chunk
	int BlockX = FloorC(m_Start.x);
	int BlockZ = FloorC(m_Start.z);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(BlockX, BlockZ, ChunkX, ChunkZ);
	return m_World->DoWithChunk(ChunkX, ChunkZ, [this](cChunk & a_Chunk) { return ChunkCallback(&a_Chunk); });
}





void cLineBlockTracer::FixStartAboveWorld(void)
{
	// We must set the start Y to less than cChunkDef::Height so that it is considered inside the world later on
	// Therefore we use an EPS-offset from the height, as small as reasonably possible.
	const double Height = static_cast<double>(cChunkDef::Height) - 0.00001;
	CalcXZIntersection(Height, m_Start.x, m_Start.z);
	m_Start.y = Height;
}





void cLineBlockTracer::FixStartBelowWorld(void)
{
	CalcXZIntersection(0, m_Start.x, m_Start.z);
	m_Start.y = 0;
}





void cLineBlockTracer::CalcXZIntersection(double a_Y, double & a_IntersectX, double & a_IntersectZ)
{
	double Ratio = (m_Start.y - a_Y) / (m_Start.y - m_End.y);
	a_IntersectX = m_Start.x + (m_End.x - m_Start.x) * Ratio;
	a_IntersectZ = m_Start.z + (m_End.z - m_Start.z) * Ratio;
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
	if (std::abs(m_Diff.x) > EPS)
	{
		double DestX = (m_Dir.x > 0) ? (m_Current.x + 1) : m_Current.x;
		double CoeffX = (DestX - m_Start.x) / m_Diff.x;
		if (CoeffX <= 1)  // We need to include equality for the last block in the trace
		{
			Coeff = CoeffX;
			Direction = dirX;
		}
	}

	// If the next XZ wall hit is closer, use it instead:
	if (std::abs(m_Diff.y) > EPS)
	{
		double DestY = (m_Dir.y > 0) ? (m_Current.y + 1) : m_Current.y;
		double CoeffY = (DestY - m_Start.y) / m_Diff.y;
		if (CoeffY <= Coeff)  // We need to include equality for the last block in the trace
		{
			Coeff = CoeffY;
			Direction = dirY;
		}
	}

	// If the next XY wall hit is closer, use it instead:
	if (std::abs(m_Diff.z) > EPS)
	{
		double DestZ = (m_Dir.z > 0) ? (m_Current.z + 1) : m_Current.z;
		double CoeffZ = (DestZ - m_Start.z) / m_Diff.z;
		if (CoeffZ <= Coeff)  // We need to include equality for the last block in the trace
		{
			Direction = dirZ;
		}
	}

	// Based on the wall hit, adjust the current coords
	switch (Direction)
	{
		case dirX:    m_Current.x += m_Dir.x; m_CurrentFace = (m_Dir.x > 0) ? BLOCK_FACE_XM : BLOCK_FACE_XP; break;
		case dirY:    m_Current.y += m_Dir.y; m_CurrentFace = (m_Dir.y > 0) ? BLOCK_FACE_YM : BLOCK_FACE_YP; break;
		case dirZ:    m_Current.z += m_Dir.z; m_CurrentFace = (m_Dir.z > 0) ? BLOCK_FACE_ZM : BLOCK_FACE_ZP; break;
		case dirNONE: return false;
	}
	return true;
}





bool cLineBlockTracer::ChunkCallback(cChunk * a_Chunk)
{
	ASSERT((m_Current.y >= 0) && (m_Current.y < cChunkDef::Height));  // This should be provided by FixStartAboveWorld() / FixStartBelowWorld()

	// This is the actual line tracing loop.
	for (;;)
	{
		// Our caller (DoWithChunk callback) should never give nothing:
		ASSERT(a_Chunk != nullptr);

		// Move to next block
		if (!MoveToNextBlock())
		{
			// We've reached the end
			m_Callbacks->OnNoMoreHits();
			return true;
		}

		if ((m_Current.y < 0) || (m_Current.y >= cChunkDef::Height))
		{
			// We've gone out of the world, that's the end of this trace
			double IntersectX, IntersectZ;
			CalcXZIntersection(m_Current.y, IntersectX, IntersectZ);
			if (m_Callbacks->OnOutOfWorld({IntersectX, double(m_Current.y), IntersectZ}))
			{
				// The callback terminated the trace
				return false;
			}
			m_Callbacks->OnNoMoreHits();
			return true;
		}

		// Update the current chunk
		a_Chunk = a_Chunk->GetNeighborChunk(m_Current.x, m_Current.z);
		if (a_Chunk == nullptr)
		{
			m_Callbacks->OnNoChunk();
			return false;
		}

		// Report the current block through the callbacks:
		if (a_Chunk->IsValid())
		{
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			int RelX = m_Current.x - a_Chunk->GetPosX() * cChunkDef::Width;
			int RelZ = m_Current.z - a_Chunk->GetPosZ() * cChunkDef::Width;
			a_Chunk->GetBlockTypeMeta(RelX, m_Current.y, RelZ, BlockType, BlockMeta);
			if (m_Callbacks->OnNextBlock(m_Current, BlockType, BlockMeta, m_CurrentFace))
			{
				// The callback terminated the trace
				return false;
			}
		}
		else if (m_Callbacks->OnNextBlockNoData(m_Current, m_CurrentFace))
		{
			// The callback terminated the trace
			return false;
		}
	}
}



