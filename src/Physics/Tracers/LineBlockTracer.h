
// LineBlockTracer.h

// Declares the LineBlockTracer namespace representing a tracer that visits every block along a straight line between two points





#pragma once

#include "BlockTracer.h"





class cChunk;
class cWorld;





namespace LineBlockTracer
{
	/* Bit flags used for LineOfSightTrace's Sight parameter. */
	enum LineOfSight
	{
		Air = 1,    // Can see through air.
		Water = 2,  // Can see through water.
		Lava = 4,   // Can see through lava.

		// Common combinations:
		AirWaterLava = Air | Water | Lava,
		AirWater = Air | Water,
	};

	/** Traces one line between Start and End; returns true if the entire line was traced (until OnNoMoreHits()) */
	bool Trace(const cChunk & a_Chunk, BlockTracerCallbacks & a_Callbacks, Vector3d a_Start, Vector3d a_End);

	/** Traces one line between Start and End; returns true if the entire line was traced (until OnNoMoreHits()) */
	bool Trace(cWorld & a_World, BlockTracerCallbacks & a_Callbacks, const Vector3d a_Start, const Vector3d a_End);

	/** Returns true if the two positions are within line of sight (not obscured by blocks).
	a_Sight specifies which blocks are considered transparent for the trace, is an OR-combination of eLineOfSight constants. */
	bool LineOfSightTrace(cWorld & a_World, const Vector3d & a_Start, const Vector3d & a_End, LineOfSight a_Sight);

	/** Traces until the first solid block is hit (or until end, whichever comes first.
	If a solid block was hit, returns true and fills a_HitCoords, a_HitBlockCoords and a_HitBlockFace.
	If a_End is encountered without hitting any solid block, returns false and doesn't touch a_HitCoords, a_HitBlockCoords nor a_HitBlockFace.
	a_HitCoords is the exact coords of the hit,
	a_HitBlockCoords are the coords of the solid block that was hit,
	a_HitBlockFace is the face of the solid block that was hit. */
	bool FirstSolidHitTrace(
		cWorld & a_World,
		const Vector3d & a_Start, const Vector3d & a_End,
		Vector3d & a_HitCoords,
		Vector3i & a_HitBlockCoords,
		eBlockFace & a_HitBlockFace
	);
}
