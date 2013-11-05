#pragma once

#include "Vector3i.h"
#include "Vector3f.h"


class cWorld;
class cTracer		// tolua_export
{					// tolua_export
public:				// tolua_export
	Vector3f DotPos;				
	Vector3f BoxOffset;
	cTracer( cWorld* a_World);		// tolua_export
	~cTracer();						// tolua_export

	/// Determines if a collision occures along a line.
	int Trace( const Vector3f & a_Start, const Vector3f & a_Direction, int a_Distance );		// tolua_export

	/// Contains the position of the block that caused the collision
	Vector3f BlockHitPosition;		// tolua_export

	/// Contains which face was hit
	Vector3f HitNormal;			// tolua_export

	/// Contains the exact position where a collision occured. (BlockHitPosition + Offset on block)
	Vector3f RealHit;				// tolua_export
private:

	/// Preps Tracer object for call of Trace function. Only used internally
	void SetValues( const Vector3f & a_Start, const Vector3f & a_Direction );

	/// Calculates where on the block a collision occured, if it does occur
	int intersect3D_SegmentPlane( const Vector3f & a_Origin, const Vector3f & a_End, const Vector3f & a_PlanePos, const Vector3f & a_PlaneNormal );
	
	/// Determines which face on the block a collision occured, if it does occur
	int GetHitNormal( const Vector3f & start, const Vector3f & end, const Vector3i &  a_BlockPos);

	float SigNum( float a_Num );
	cWorld* m_World;

	Vector3f m_NormalTable[6];

	Vector3f dir;
	Vector3f tDelta;
	Vector3i pos;
	Vector3i end1;
	Vector3i step;
	Vector3f tMax;
}; // tolua_export
