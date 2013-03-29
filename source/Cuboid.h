
#pragma once

#include "Vector3i.h"
#include "Vector3d.h"





// tolua_begin
class cCuboid
{
public:
	// p1 is expected to have the smaller of the coords; Sort() swaps coords to match this
	Vector3i p1, p2;

	cCuboid(void) {}
	cCuboid(const cCuboid & a_Cuboid ) : p1(a_Cuboid.p1), p2(a_Cuboid.p2) {}
	cCuboid(const Vector3i & a_p1, const Vector3i & a_p2) : p1(a_p1), p2(a_p2) {}

	void Sort(void);
	
	int DifX(void) const { return p2.x - p1.x; }
	int DifY(void) const { return p2.y - p1.y; }
	int DifZ(void) const { return p2.z - p1.z; }

	bool IsInside(const Vector3i & v)	const
	{
		return (
			(v.x >= p1.x) && (v.x <= p2.x) &&
			(v.y >= p1.y) && (v.y <= p2.y) &&
			(v.z >= p1.z) && (v.z <= p2.z)
		);
	}

	bool IsInside( const Vector3d & v ) const
	{
		return (
			(v.x >= p1.x) && (v.x <= p2.x) &&
			(v.y >= p1.y) && (v.y <= p2.y) &&
			(v.z >= p1.z) && (v.z <= p2.z)
		);
	}

} ;
// tolua_end




