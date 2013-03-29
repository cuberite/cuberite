
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
	cCuboid(int a_X1, int a_Y1, int a_Z1) : p1(a_X1, a_Y1, a_Z1), p2(a_X1, a_Y1, a_Z1) {}
	cCuboid(int a_X1, int a_Y1, int a_Z1, int a_X2, int a_Y2, int a_Z2) : p1(a_X1, a_Y1, a_Z1), p2(a_X2, a_Y2, a_Z2) {}

	void Sort(void);
	
	int DifX(void) const { return p2.x - p1.x; }
	int DifY(void) const { return p2.y - p1.y; }
	int DifZ(void) const { return p2.z - p1.z; }
	
	/// Returns true if the cuboids have at least one voxel in common. Both coords are considered inclusive.
	bool DoesIntersect(const cCuboid & a_Other) const;

	bool IsInside(const Vector3i & v)	const
	{
		return (
			(v.x >= p1.x) && (v.x <= p2.x) &&
			(v.y >= p1.y) && (v.y <= p2.y) &&
			(v.z >= p1.z) && (v.z <= p2.z)
		);
	}

	bool IsInside(int a_X, int a_Y, int a_Z)	const
	{
		return (
			(a_X >= p1.x) && (a_X <= p2.x) &&
			(a_Y >= p1.y) && (a_Y <= p2.y) &&
			(a_Z >= p1.z) && (a_Z <= p2.z)
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
	
	/// Moves the cuboid by the specified offsets in each direction
	void Move(int a_OfsX, int a_OfsY, int a_OfsZ);

	/// Returns true if the coords are properly sorted (lesser in p1, greater in p2)
	bool IsSorted(void) const;
} ;
// tolua_end




