
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Tracer.h"
#include "World.h"

#include "Vector3f.h"
#include "Vector3i.h"
#include "Vector3d.h"

#include "Entities/Entity.h"

#ifndef _WIN32
	#include <stdlib.h> // abs()
#endif





cTracer::cTracer(cWorld * a_World)
	: m_World(a_World)
{
	m_NormalTable[0].Set(-1, 0, 0);
	m_NormalTable[1].Set( 0, 0,-1);
	m_NormalTable[2].Set( 1, 0, 0);
	m_NormalTable[3].Set( 0, 0, 1);
	m_NormalTable[4].Set( 0, 1, 0);
	m_NormalTable[5].Set( 0,-1, 0);
}





cTracer::~cTracer()
{
}





float cTracer::SigNum( float a_Num )
{
	if (a_Num < 0.f) return -1.f;
	if (a_Num > 0.f) return 1.f;
	return 0.f;
}





void cTracer::SetValues(const Vector3f & a_Start, const Vector3f & a_Direction)
{
	// calculate the direction of the ray (linear algebra)
	dir = a_Direction;

	// decide which direction to start walking in
	step.x = (int) SigNum(dir.x);
	step.y = (int) SigNum(dir.y);
	step.z = (int) SigNum(dir.z);

	// normalize the direction vector
	if( dir.SqrLength() > 0.f ) dir.Normalize();

	// how far we must move in the ray direction before
	// we encounter a new voxel in x-direction
	// same but y-direction
	if (dir.x != 0.f)
	{
		tDelta.x = 1 / fabs(dir.x);
	}
	else
	{
		tDelta.x = 0;
	}
	if (dir.y != 0.f)
	{
		tDelta.y = 1 / fabs(dir.y);
	}
	else
	{
		tDelta.y = 0;
	}
	if (dir.z != 0.f)
	{
		tDelta.z = 1 / fabs(dir.z);
	}
	else
	{
		tDelta.z = 0;
	}

	// start voxel coordinates
	pos.x = (int)floorf(a_Start.x);
	pos.y = (int)floorf(a_Start.y);
	pos.z = (int)floorf(a_Start.z);

	// calculate distance to first intersection in the voxel we start from
	if (dir.x < 0)
	{
		tMax.x = ((float)pos.x - a_Start.x) / dir.x;
	}
	else
	{
		tMax.x = (((float)pos.x + 1) - a_Start.x) / dir.x;
	}

	if (dir.y < 0)
	{
		tMax.y = ((float)pos.y - a_Start.y) / dir.y;
	}
	else
	{
		tMax.y = (((float)pos.y + 1) - a_Start.y) / dir.y;
	}

	if (dir.z < 0)
	{
		tMax.z = ((float)pos.z - a_Start.z) / dir.z;
	}
	else
	{
		tMax.z = (((float)pos.z + 1) - a_Start.z) / dir.z;
	}
}





bool cTracer::Trace( const Vector3f & a_Start, const Vector3f & a_Direction, int a_Distance, bool a_LineOfSight)
{
	if ((a_Start.y < 0) || (a_Start.y >= cChunkDef::Height))
	{
		LOGD("%s: Start Y is outside the world (%.2f), not tracing.", __FUNCTION__, a_Start.y);
		return false;
	}
	
	SetValues(a_Start, a_Direction);

	Vector3f End = a_Start + (dir * (float)a_Distance);
	
	if (End.y < 0)
	{
		float dist = -a_Start.y / dir.y;
		End = a_Start + (dir * dist);
	}

	// end voxel coordinates
	end1.x = (int)floorf(End.x);
	end1.y = (int)floorf(End.y);
	end1.z = (int)floorf(End.z);
	
	// check if first is occupied
	if (pos.Equals(end1))
	{
		return false;
	}

	bool reachedX = false, reachedY = false, reachedZ = false;

	int Iterations = 0;
	while (Iterations < a_Distance)
	{
		Iterations++;
		if ((tMax.x < tMax.y) && (tMax.x < tMax.z))
		{
			tMax.x += tDelta.x;
			pos.x += step.x;
		}
		else if (tMax.y < tMax.z)
		{
			tMax.y += tDelta.y;
			pos.y += step.y;
		}
		else
		{
			tMax.z += tDelta.z;
			pos.z += step.z;
		}

		if (step.x > 0.0f)
		{
			if (pos.x >= end1.x)
			{
				reachedX = true;
			}
		}
		else if (pos.x <= end1.x)
		{
			reachedX = true;
		}

		if (step.y > 0.0f)
		{
			if(pos.y >= end1.y)
			{
				reachedY = true;
			}
		}
		else if (pos.y <= end1.y)
		{
			reachedY = true;
		}

		if (step.z > 0.0f)
		{
			if (pos.z >= end1.z)
			{
				reachedZ = true;
			}
		}
		else if (pos.z <= end1.z)
		{
			reachedZ = true;
		}

		if (reachedX && reachedY && reachedZ)
		{
			return false;
		}
		
		BLOCKTYPE BlockID = m_World->GetBlock(pos.x, pos.y, pos.z);
		// Block is counted as a collision if we are not doing a line of sight and it is solid,
		// or if the block is not air and not water. That way mobs can still see underwater.
		if ((!a_LineOfSight && g_BlockIsSolid[BlockID]) || (a_LineOfSight && (BlockID != E_BLOCK_AIR) && !IsBlockWater(BlockID)))
		{
			BlockHitPosition = pos;
			int Normal = GetHitNormal(a_Start, End, pos );
			if(Normal > 0)
			{
				HitNormal = m_NormalTable[Normal-1];
			}
			return true;
		}
	}
	return false;
}





// return 1 = hit, other is not hit
int LinesCross(float x0,float y0,float x1,float y1,float x2,float y2,float x3,float y3)
{
	//float linx, liny;

	float d=(x1-x0)*(y3-y2)-(y1-y0)*(x3-x2);
	if (abs(d)<0.001) {return 0;}
	float AB=((y0-y2)*(x3-x2)-(x0-x2)*(y3-y2))/d;
	if (AB>=0.0 && AB<=1.0)
	{
		float CD=((y0-y2)*(x1-x0)-(x0-x2)*(y1-y0))/d;
		if (CD>=0.0 && CD<=1.0)
		{
			//linx=x0+AB*(x1-x0);
			//liny=y0+AB*(y1-y0);
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
int cTracer::intersect3D_SegmentPlane( const Vector3f & a_Origin, const Vector3f & a_End, const Vector3f & a_PlanePos, const Vector3f & a_PlaneNormal )
{
	Vector3f    u = a_End - a_Origin;//a_Ray.P1 - S.P0;
	Vector3f    w = a_Origin - a_PlanePos;//S.P0 - Pn.V0;

	float     D = a_PlaneNormal.Dot( u );//dot(Pn.n, u);
	float     N = -(a_PlaneNormal.Dot( w ) );//-dot(a_Plane.n, w);

	const float EPSILON = 0.0001f;
	if (fabs(D) < EPSILON) {          // segment is parallel to plane
		if (N == 0)                     // segment lies in plane
			return 2;
		return 0;                   // no intersection
	}
	// they are not parallel
	// compute intersect param
	float sI = N / D;
	if (sI < 0 || sI > 1)
		return 0;                       // no intersection

	//Vector3f I ( a_Ray->GetOrigin() + sI * u );//S.P0 + sI * u;                 // compute segment intersect point
	RealHit = a_Origin + u * sI;
	return 1;
}

int cTracer::GetHitNormal(const Vector3f & start, const Vector3f & end, const Vector3i & a_BlockPos)
{
	Vector3i SmallBlockPos = a_BlockPos;
	char BlockID = m_World->GetBlock( a_BlockPos.x, a_BlockPos.y, a_BlockPos.z );

	if( BlockID == E_BLOCK_AIR || IsBlockWater(BlockID))
		return 0;

	Vector3f BlockPos;
	BlockPos = Vector3f(SmallBlockPos);

	Vector3f Look = (end - start);
	Look.Normalize();

	float dot = Look.Dot( Vector3f(-1, 0, 0) ); // first face normal is x -1
	if(dot < 0)
	{
		int Lines = LinesCross( start.x, start.y, end.x, end.y, BlockPos.x, BlockPos.y, BlockPos.x, BlockPos.y + 1 );
		if(Lines == 1)
		{
			Lines = LinesCross( start.x, start.z, end.x, end.z, BlockPos.x, BlockPos.z, BlockPos.x, BlockPos.z + 1 );
			if(Lines == 1)
			{
				intersect3D_SegmentPlane( start, end, BlockPos, Vector3f(-1, 0, 0) );
				return 1;
			}
		}
	}
	dot = Look.Dot( Vector3f(0, 0, -1) ); // second face normal is z -1
	if(dot < 0)
	{
		int Lines = LinesCross( start.z, start.y, end.z, end.y, BlockPos.z, BlockPos.y, BlockPos.z, BlockPos.y + 1 );
		if(Lines == 1)
		{
			Lines = LinesCross( start.z, start.x, end.z, end.x, BlockPos.z, BlockPos.x, BlockPos.z, BlockPos.x + 1 );
			if(Lines == 1)
			{
				intersect3D_SegmentPlane( start, end, BlockPos, Vector3f(0, 0, -1) );
				return 2;
			}
		}
	}
	dot = Look.Dot( Vector3f(1, 0, 0) ); // third face normal is x 1
	if(dot < 0)
	{
		int Lines = LinesCross( start.x, start.y, end.x, end.y, BlockPos.x + 1, BlockPos.y, BlockPos.x + 1, BlockPos.y + 1 );
		if(Lines == 1)
		{
			Lines = LinesCross( start.x, start.z, end.x, end.z, BlockPos.x + 1, BlockPos.z, BlockPos.x + 1, BlockPos.z + 1 );
			if(Lines == 1)
			{
				intersect3D_SegmentPlane( start, end, BlockPos + Vector3f(1, 0, 0), Vector3f(1, 0, 0) );
				return 3;
			}
		}
	}
	dot = Look.Dot( Vector3f(0, 0, 1) ); // fourth face normal is z 1
	if(dot < 0)
	{
		int Lines = LinesCross( start.z, start.y, end.z, end.y, BlockPos.z + 1, BlockPos.y, BlockPos.z + 1, BlockPos.y + 1 );
		if(Lines == 1)
		{
			Lines = LinesCross( start.z, start.x, end.z, end.x, BlockPos.z + 1, BlockPos.x, BlockPos.z + 1, BlockPos.x + 1 );
			if(Lines == 1)
			{
				intersect3D_SegmentPlane( start, end, BlockPos + Vector3f(0, 0, 1), Vector3f(0, 0, 1) );
				return 4;
			}
		}
	}
	dot = Look.Dot( Vector3f(0, 1, 0) ); // fifth face normal is y 1
	if(dot < 0)
	{
		int Lines = LinesCross( start.y, start.x, end.y, end.x, BlockPos.y + 1, BlockPos.x, BlockPos.y + 1, BlockPos.x + 1 );
		if(Lines == 1)
		{
			Lines = LinesCross( start.y, start.z, end.y, end.z, BlockPos.y + 1, BlockPos.z, BlockPos.y + 1, BlockPos.z + 1 );
			if(Lines == 1)
			{
				intersect3D_SegmentPlane( start, end, BlockPos + Vector3f(0, 1, 0), Vector3f(0, 1, 0) );
				return 5;
			}
		}
	}
	dot = Look.Dot( Vector3f(0, -1, 0) ); // sixth face normal is y -1
	if(dot < 0)
	{
		int Lines = LinesCross( start.y, start.x, end.y, end.x, BlockPos.y, BlockPos.x, BlockPos.y, BlockPos.x + 1 );
		if(Lines == 1)
		{
			Lines = LinesCross( start.y, start.z, end.y, end.z, BlockPos.y, BlockPos.z, BlockPos.y, BlockPos.z + 1 );
			if(Lines == 1)
			{
				intersect3D_SegmentPlane( start, end, BlockPos, Vector3f(0, -1, 0) );
				return 6;
			}
		}
	}
	return 0;
}
