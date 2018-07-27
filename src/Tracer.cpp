
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Tracer.h"
#include "World.h"

#include "Entities/Entity.h"

#ifndef _WIN32
	#include <stdlib.h>
#endif



const float FLOAT_EPSILON = 0.0001f;  // TODO: Stash this in some header where it can be reused


const std::array<const Vector3f, 6>& cTracer::m_NormalTable(void)
{
	static std::array<const Vector3f, 6>* table =
		new std::array<const Vector3f, 6>
	{
		{
		Vector3f(-1,  0,  0),  // 1: -x
		Vector3f( 0,  0, -1),  // 2: -z
		Vector3f( 1,  0,  0),  // 3: +x
		Vector3f( 0,  0,  1),  // 4: +z
		Vector3f( 0,  1,  0),  // 5: +y
		Vector3f( 0, -1,  0)   // 6: -y
		}
	};

	return *table;
};



cTracer::cTracer(cWorld * a_World):
	m_World(a_World)
{
}





cTracer::~cTracer()
{
}





int cTracer::SigNum(float a_Num)
{
	if (a_Num < 0.f)
	{
		return -1;
	}
	if (a_Num > 0.f)
	{
		return 1;
	}
	return 0;
}





void cTracer::SetValues(const Vector3f & a_Start, const Vector3f & a_Direction)
{
	// Since this method should only be called by Trace, zero length vectors should already have been taken care of
	ASSERT(a_Direction.HasNonZeroLength());

	// calculate the direction of the ray (linear algebra)
	m_Dir = a_Direction;

	// decide which direction to start walking in
	m_Step.x = SigNum(m_Dir.x);
	m_Step.y = SigNum(m_Dir.y);
	m_Step.z = SigNum(m_Dir.z);


	// normalize the direction vector
	m_Dir.Normalize();


	// how far we must move in the ray direction before
	// we encounter a new voxel in x-direction
	// same but y-direction
	if (m_Dir.x != 0.f)
	{
		m_tDelta.x = 1 / std::abs(m_Dir.x);
	}
	else
	{
		m_tDelta.x = 0;
	}
	if (m_Dir.y != 0.f)
	{
		m_tDelta.y = 1 / std::abs(m_Dir.y);
	}
	else
	{
		m_tDelta.y = 0;
	}
	if (m_Dir.z != 0.f)
	{
		m_tDelta.z = 1 / std::abs(m_Dir.z);
	}
	else
	{
		m_tDelta.z = 0;
	}


	// start voxel coordinates
	m_Pos.x = static_cast<int>(floorf(a_Start.x));
	m_Pos.y = static_cast<int>(floorf(a_Start.y));
	m_Pos.z = static_cast<int>(floorf(a_Start.z));

	// calculate distance to first intersection in the voxel we start from
	if (m_Dir.x < 0)
	{
		m_tMax.x = (static_cast<float>(m_Pos.x) - a_Start.x) / m_Dir.x;
	}
	else
	{
		m_tMax.x = (static_cast<float>(m_Pos.x + 1) - a_Start.x) / m_Dir.x;  // TODO: Possible division by zero
	}

	if (m_Dir.y < 0)
	{
		m_tMax.y = (static_cast<float>(m_Pos.y) - a_Start.y) / m_Dir.y;
	}
	else
	{
		m_tMax.y = (static_cast<float>(m_Pos.y + 1) - a_Start.y) / m_Dir.y;  // TODO: Possible division by zero
	}

	if (m_Dir.z < 0)
	{
		m_tMax.z = (static_cast<float>(m_Pos.z) - a_Start.z) / m_Dir.z;
	}
	else
	{
		m_tMax.z = (static_cast<float>(m_Pos.z + 1) - a_Start.z) / m_Dir.z;  // TODO: Possible division by zero
	}
}





bool cTracer::Trace(const Vector3f & a_Start, const Vector3f & a_Direction, int a_Distance, bool a_LineOfSight)
{
	if (!a_Direction.HasNonZeroLength())
	{
		return false;
	}

	if ((a_Start.y < 0) || (a_Start.y >= cChunkDef::Height))
	{
		LOGD("%s: Start Y is outside the world (%.2f), not tracing.", __FUNCTION__, a_Start.y);
		return false;
	}

	SetValues(a_Start, a_Direction);

	Vector3f End = a_Start + (m_Dir * static_cast<float>(a_Distance));

	if (End.y < 0)
	{
		float dist = -a_Start.y / m_Dir.y;  // No division by 0 possible
		End = a_Start + (m_Dir * dist);
	}

	// end voxel coordinates
	m_End1.x = static_cast<int>(floorf(End.x));
	m_End1.y = static_cast<int>(floorf(End.y));
	m_End1.z = static_cast<int>(floorf(End.z));

	// check if first is occupied
	if (m_Pos.Equals(m_End1))
	{
		return false;
	}

	bool reachedX = false, reachedY = false, reachedZ = false;

	int Iterations = 0;
	while (Iterations < a_Distance)
	{
		Iterations++;
		if ((m_tMax.x < m_tMax.y) && (m_tMax.x < m_tMax.z))
		{
			m_tMax.x += m_tDelta.x;
			m_Pos.x += m_Step.x;
		}
		else if (m_tMax.y < m_tMax.z)
		{
			m_tMax.y += m_tDelta.y;
			m_Pos.y += m_Step.y;
		}
		else
		{
			m_tMax.z += m_tDelta.z;
			m_Pos.z += m_Step.z;
		}

		if (m_Step.x > 0.0f)
		{
			if (m_Pos.x >= m_End1.x)
			{
				reachedX = true;
			}
		}
		else if (m_Pos.x <= m_End1.x)
		{
			reachedX = true;
		}

		if (m_Step.y > 0.0f)
		{
			if (m_Pos.y >= m_End1.y)
			{
				reachedY = true;
			}
		}
		else if (m_Pos.y <= m_End1.y)
		{
			reachedY = true;
		}

		if (m_Step.z > 0.0f)
		{
			if (m_Pos.z >= m_End1.z)
			{
				reachedZ = true;
			}
		}
		else if (m_Pos.z <= m_End1.z)
		{
			reachedZ = true;
		}

		if (reachedX && reachedY && reachedZ)
		{
			return false;
		}

		if ((m_Pos.y < 0) || (m_Pos.y >= cChunkDef::Height))
		{
			return false;
		}
		BLOCKTYPE BlockID = m_World->GetBlock(m_Pos);
		// Block is counted as a collision if we are not doing a line of sight and it is solid,
		// or if the block is not air and not water. That way mobs can still see underwater.
		if ((!a_LineOfSight && cBlockInfo::IsSolid(BlockID)) || (a_LineOfSight && (BlockID != E_BLOCK_AIR) && !IsBlockWater(BlockID)))
		{
			BlockHitPosition = m_Pos;
			int Normal = GetHitNormal(a_Start, End, m_Pos);
			if (Normal > 0)
			{
				HitNormal = m_NormalTable()[static_cast<size_t>(Normal - 1)];
			}
			return true;
		}
	}
	return false;
}





// return 1 = hit, other is not hit
static int LinesCross(float x0, float y0, float x1, float y1, float x2, float y2, float x3, float y3)
{
	// float linx, liny;

	float d = (x1 - x0) * (y3 - y2) - (y1 - y0) * (x3 - x2);
	if (std::abs(d) < 0.001)
	{
		return 0;
	}

	float AB = ((y0 - y2) * (x3 - x2) - (x0 - x2) * (y3 - y2)) / d;
	if ((AB >= 0.0) && (AB <= 1.0))
	{
		float CD = ((y0 - y2) * (x1 - x0) - (x0 - x2) * (y1 - y0)) / d;
		if ((CD >= 0.0) && (CD <= 1.0))
		{
			// linx = x0 + AB * (x1 - x0);
			// liny = y0 + AB * (y1 - y0);
			return 1;
		}
	}
	return 0;
}





// intersect3D_SegmentPlane(): intersect a segment and a plane
//    Input:  a_Ray = a segment, and a_Plane = a plane = {Point V0; Vector n;}
//    Output: *I0 = the intersect point (when it exists)
//    Return: 0 = disjoint (no intersection)
//            1 = intersection in the unique point *I0
//            2 = the segment lies in the plane
int cTracer::intersect3D_SegmentPlane(const Vector3f & a_Origin, const Vector3f & a_End, const Vector3f & a_PlanePos, const Vector3f & a_PlaneNormal)
{
	Vector3f    u = a_End - a_Origin;       // a_Ray.P1 - S.P0;
	Vector3f    w = a_Origin - a_PlanePos;  // S.P0 - Pn.V0;

	float     D = a_PlaneNormal.Dot(u);      // dot(Pn.n, u);
	float     N = -(a_PlaneNormal.Dot(w));  // -dot(a_Plane.n, w);

	if (std::abs(D) < FLOAT_EPSILON)
	{
		// segment is parallel to plane
		if (N == 0.0)
		{
			// segment lies in plane
			return 2;
		}
		return 0;  // no intersection
	}

	// they are not parallel
	// compute intersect param
	float sI = N / D;
	if ((sI < 0) || (sI > 1))
	{
		return 0;  // no intersection
	}

	// Vector3f I (a_Ray->GetOrigin() + sI * u);// S.P0 + sI * u;  // compute segment intersect point
	RealHit = a_Origin + u * sI;
	return 1;
}





int cTracer::GetHitNormal(const Vector3f & a_Start, const Vector3f & a_End, const Vector3i & a_BlockPos)
{
	Vector3i SmallBlockPos = a_BlockPos;
	BLOCKTYPE BlockID = static_cast<BLOCKTYPE>(m_World->GetBlock(a_BlockPos.x, a_BlockPos.y, a_BlockPos.z));

	if ((BlockID == E_BLOCK_AIR) || IsBlockWater(BlockID))
	{
		return 0;
	}

	Vector3f BlockPos;
	BlockPos = Vector3f(SmallBlockPos);

	Vector3f Look = (a_End - a_Start);
	Look.Normalize();

	float dot = Look.Dot(Vector3f(-1, 0, 0));  // first face normal is x -1
	if (dot < 0)
	{
		int Lines = LinesCross(a_Start.x, a_Start.y, a_End.x, a_End.y, BlockPos.x, BlockPos.y, BlockPos.x, BlockPos.y + 1);
		if (Lines == 1)
		{
			Lines = LinesCross(a_Start.x, a_Start.z, a_End.x, a_End.z, BlockPos.x, BlockPos.z, BlockPos.x, BlockPos.z + 1);
			if (Lines == 1)
			{
				intersect3D_SegmentPlane(a_Start, a_End, BlockPos, Vector3f(-1, 0, 0));
				return 1;
			}
		}
	}
	dot = Look.Dot(Vector3f(0, 0, -1));  // second face normal is z -1
	if (dot < 0)
	{
		int Lines = LinesCross(a_Start.z, a_Start.y, a_End.z, a_End.y, BlockPos.z, BlockPos.y, BlockPos.z, BlockPos.y + 1);
		if (Lines == 1)
		{
			Lines = LinesCross(a_Start.z, a_Start.x, a_End.z, a_End.x, BlockPos.z, BlockPos.x, BlockPos.z, BlockPos.x + 1);
			if (Lines == 1)
			{
				intersect3D_SegmentPlane(a_Start, a_End, BlockPos, Vector3f(0, 0, -1));
				return 2;
			}
		}
	}
	dot = Look.Dot(Vector3f(1, 0, 0));  // third face normal is x 1
	if (dot < 0)
	{
		int Lines = LinesCross(a_Start.x, a_Start.y, a_End.x, a_End.y, BlockPos.x + 1, BlockPos.y, BlockPos.x + 1, BlockPos.y + 1);
		if (Lines == 1)
		{
			Lines = LinesCross(a_Start.x, a_Start.z, a_End.x, a_End.z, BlockPos.x + 1, BlockPos.z, BlockPos.x + 1, BlockPos.z + 1);
			if (Lines == 1)
			{
				intersect3D_SegmentPlane(a_Start, a_End, BlockPos + Vector3f(1, 0, 0), Vector3f(1, 0, 0));
				return 3;
			}
		}
	}
	dot = Look.Dot(Vector3f(0, 0, 1));  // fourth face normal is z 1
	if (dot < 0)
	{
		int Lines = LinesCross(a_Start.z, a_Start.y, a_End.z, a_End.y, BlockPos.z + 1, BlockPos.y, BlockPos.z + 1, BlockPos.y + 1);
		if (Lines == 1)
		{
			Lines = LinesCross(a_Start.z, a_Start.x, a_End.z, a_End.x, BlockPos.z + 1, BlockPos.x, BlockPos.z + 1, BlockPos.x + 1);
			if (Lines == 1)
			{
				intersect3D_SegmentPlane(a_Start, a_End, BlockPos + Vector3f(0, 0, 1), Vector3f(0, 0, 1));
				return 4;
			}
		}
	}
	dot = Look.Dot(Vector3f(0, 1, 0));  // fifth face normal is y 1
	if (dot < 0)
	{
		int Lines = LinesCross(a_Start.y, a_Start.x, a_End.y, a_End.x, BlockPos.y + 1, BlockPos.x, BlockPos.y + 1, BlockPos.x + 1);
		if (Lines == 1)
		{
			Lines = LinesCross(a_Start.y, a_Start.z, a_End.y, a_End.z, BlockPos.y + 1, BlockPos.z, BlockPos.y + 1, BlockPos.z + 1);
			if (Lines == 1)
			{
				intersect3D_SegmentPlane(a_Start, a_End, BlockPos + Vector3f(0, 1, 0), Vector3f(0, 1, 0));
				return 5;
			}
		}
	}
	dot = Look.Dot(Vector3f(0, -1, 0));  // sixth face normal is y -1
	if (dot < 0)
	{
		int Lines = LinesCross(a_Start.y, a_Start.x, a_End.y, a_End.x, BlockPos.y, BlockPos.x, BlockPos.y, BlockPos.x + 1);
		if (Lines == 1)
		{
			Lines = LinesCross(a_Start.y, a_Start.z, a_End.y, a_End.z, BlockPos.y, BlockPos.z, BlockPos.y, BlockPos.z + 1);
			if (Lines == 1)
			{
				intersect3D_SegmentPlane(a_Start, a_End, BlockPos, Vector3f(0, -1, 0));
				return 6;
			}
		}
	}
	return 0;
}
