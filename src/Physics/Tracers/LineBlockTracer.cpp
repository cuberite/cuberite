
// LineBlockTracer.cpp

// Implements the cLineBlockTracer class representing a cBlockTracer that traces along a straight line between two points

#include "Globals.h"
#include "LineBlockTracer.h"
#include "BlockInfo.h"
#include "World.h"
#include "Chunk.h"
#include "BoundingBox.h"





static Vector3d CalcXZIntersection(double a_Y, const Vector3d a_Start, const Vector3d a_End)
{
	const double Ratio = (a_Start.y - a_Y) / (a_Start.y - a_End.y);
	return { a_Start.x + (a_End.x - a_Start.x) * Ratio, a_Y, a_Start.z + (a_End.z - a_Start.z) * Ratio };
}





static Vector3d FixStartAboveWorld(const Vector3d a_Start, const Vector3d a_End)
{
	// We must set the start Y to less than cChunkDef::Height so that it is considered inside the world later on.
	// Therefore we use an EPS-offset from the height, as small as reasonably possible.
	const double Height = static_cast<double>(cChunkDef::Height) - 0.00001;
	return CalcXZIntersection(Height, a_Start, a_End);
}





static Vector3d FixStartBelowWorld(const Vector3d a_Start, const Vector3d a_End)
{
	return CalcXZIntersection(0, a_Start, a_End);
}





static eBlockFace MoveToNextBlock(Vector3i & a_CurrentBlock, const Vector3i a_Adjustment, const Vector3d a_Direction, const Vector3d a_Start, const Vector3i a_StepOffset, const eBlockFace a_StepXFace, const eBlockFace a_StepYFace, const eBlockFace a_StepZFace)
{
	const auto Coeff = (Vector3d(a_CurrentBlock + a_Adjustment) - a_Start) / a_Direction;

	if (Coeff.x <= Coeff.y)
	{
		if (Coeff.x <= Coeff.z)
		{
			a_CurrentBlock.x += a_StepOffset.x;
			return a_StepXFace;
		}
	}
	else if (Coeff.y <= Coeff.z)
	{
		a_CurrentBlock.y += a_StepOffset.y;
		return a_StepYFace;
	}

	a_CurrentBlock.z += a_StepOffset.z;
	return a_StepZFace;
}





bool LineBlockTracer::Trace(const cChunk & a_Chunk, BlockTracerCallbacks & a_Callbacks, Vector3d a_Start, Vector3d a_End)
{
	// Clamp the start coords into the world by advancing them along the line:
	if (a_Start.y < 0)
	{
		if (a_End.y < 0)
		{
			// Nothing to trace:
			a_Callbacks.OnNoMoreHits();
			return true;
		}

		a_Start = FixStartBelowWorld(a_Start, a_End);
		a_Callbacks.OnIntoWorld(a_Start);
	}
	else if (a_Start.y >= cChunkDef::Height)
	{
		if (a_End.y >= cChunkDef::Height)
		{
			a_Callbacks.OnNoMoreHits();
			return true;
		}

		a_Start = FixStartAboveWorld(a_Start, a_End);
		a_Callbacks.OnIntoWorld(a_Start);
	}

	const auto EndPosition = a_End.Floor();
	const auto Direction = a_End - a_Start;
	const bool XPositive = a_Start.x <= a_End.x;
	const bool YPositive = a_Start.y <= a_End.y;
	const bool ZPositive = a_Start.z <= a_End.z;
	const auto StepXFace = XPositive ? BLOCK_FACE_XM : BLOCK_FACE_XP;
	const auto StepYFace = YPositive ? BLOCK_FACE_YM : BLOCK_FACE_YP;
	const auto StepZFace = ZPositive ? BLOCK_FACE_ZM : BLOCK_FACE_ZP;
	const Vector3i Adjustment(XPositive ? 1 : 0, YPositive ? 1 : 0, ZPositive ? 1 : 0);
	const Vector3i StepOffset(XPositive ? 1 : -1, YPositive ? 1 : -1, ZPositive ? 1 : -1);

	auto Position = a_Start.Floor();
	auto Chunk = const_cast<cChunk &>(a_Chunk).GetNeighborChunk(Position.x, Position.z);

	// We should always start in a valid chunk:
	ASSERT(Chunk != nullptr);

	// This is guaranteed by FixStartAboveWorld() / FixStartBelowWorld():
	ASSERT(cChunkDef::IsValidHeight(Position));

	// This is the actual line tracing loop.
	for (;;)
	{
		if (Position == EndPosition)
		{
			// We've reached the end
			a_Callbacks.OnNoMoreHits();
			return true;
		}

		// The face of the next block the line just entered.
		const auto CurrentFace = MoveToNextBlock(Position, Adjustment, Direction, a_Start, StepOffset, StepXFace, StepYFace, StepZFace);

		if (!cChunkDef::IsValidHeight(Position))
		{
			// We've gone out of the world, that's the end of this trace.
			if (a_Callbacks.OnOutOfWorld(CalcXZIntersection(static_cast<double>(Position.y), a_Start, a_End)))
			{
				// The callback terminated the trace
				return false;
			}
			a_Callbacks.OnNoMoreHits();
			return true;
		}

		// Update the current chunk:
		Chunk = Chunk->GetNeighborChunk(Position.x, Position.z);
		if (Chunk == nullptr)
		{
			a_Callbacks.OnNoChunk();
			return false;
		}

		// Report the current block through the callbacks:
		if (Chunk->IsValid())
		{
			BLOCKTYPE BlockType;
			NIBBLETYPE BlockMeta;
			int RelX = Position.x - Chunk->GetPosX() * cChunkDef::Width;
			int RelZ = Position.z - Chunk->GetPosZ() * cChunkDef::Width;
			Chunk->GetBlockTypeMeta(RelX, Position.y, RelZ, BlockType, BlockMeta);
			if (a_Callbacks.OnNextBlock(Position, BlockType, BlockMeta, CurrentFace))
			{
				// The callback terminated the trace.
				return false;
			}
		}
		else if (a_Callbacks.OnNextBlockNoData(Position, CurrentFace))
		{
			// The callback terminated the trace.
			return false;
		}
	}
}





bool LineBlockTracer::Trace(cWorld & a_World, BlockTracerCallbacks & a_Callbacks, const Vector3d a_Start, const Vector3d a_End)
{
	int BlockX = FloorC(a_Start.x);
	int BlockZ = FloorC(a_Start.z);
	int ChunkX, ChunkZ;
	cChunkDef::BlockToChunk(BlockX, BlockZ, ChunkX, ChunkZ);
	return a_World.DoWithChunk(ChunkX, ChunkZ, [&a_Callbacks, a_Start, a_End](cChunk & a_Chunk) { return Trace(a_Chunk, a_Callbacks, a_Start, a_End); });
}





bool LineBlockTracer::LineOfSightTrace(cWorld & a_World, const Vector3d & a_Start, const Vector3d & a_End, LineOfSight a_Sight)
{
	static class LineOfSightCallbacks:
		public BlockTracerCallbacks
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
		(a_Sight & LineOfSight::Air) == 0,
		(a_Sight & LineOfSight::Water) == 0,
		(a_Sight & LineOfSight::Lava) == 0
	);
	return Trace(a_World, callbacks, a_Start, a_End);
}





bool LineBlockTracer::FirstSolidHitTrace(
	cWorld & a_World,
	const Vector3d & a_Start, const Vector3d & a_End,
	Vector3d & a_HitCoords,
	Vector3i & a_HitBlockCoords, eBlockFace & a_HitBlockFace
)
{
	class cSolidHitCallbacks:
		public BlockTracerCallbacks
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
