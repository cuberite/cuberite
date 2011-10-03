#pragma once

class cWorld;
class Vector3i;
class Vector3f;
class cTracer		//tolua_export
{					//tolua_export
public:				//tolua_export
	Vector3f* DotPos;				
	Vector3f* BoxOffset;
	cTracer( cWorld* a_World);		//tolua_export
	~cTracer();						//tolua_export
	int Trace( const Vector3f & a_Start, const Vector3f & a_Direction, int a_Distance );		//tolua_export
	void SetValues( const Vector3f & a_Start, const Vector3f & a_Direction );					//tolua_export
	Vector3f* BlockHitPosition;		//tolua_export
	Vector3f* HitNormal;			//tolua_export
	Vector3f* RealHit;				//tolua_export
private:
	int intersect3D_SegmentPlane( const Vector3f & a_Origin, const Vector3f & a_End, const Vector3f & a_PlanePos, const Vector3f & a_PlaneNormal );
	int GetHitNormal( const Vector3f & start, const Vector3f & end, const Vector3i &  a_BlockPos);
	float SigNum( float a_Num );
	cWorld* m_World;

	Vector3f* m_NormalTable[6];

	Vector3f* dir;
	Vector3f* tDelta;
	Vector3i* pos;
	Vector3i* end1;
	Vector3i* step;
	Vector3f* tMax;
}; //tolua_export