
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cuboid.h"





/// Returns true if the two specified intervals have a non-empty union
static bool DoIntervalsIntersect(int a_Min1, int a_Max1, int a_Min2, int a_Max2)
{
	return (
		((a_Min1 >= a_Min2) && (a_Min1 <= a_Max2)) ||  // Start of first  interval is within the second interval
		((a_Max1 >= a_Min2) && (a_Max1 <= a_Max2)) ||  // End   of first  interval is within the second interval
		((a_Min2 >= a_Min2) && (a_Min2 <= a_Max2))     // Start of second interval is within the first interval
	);
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCuboid:

void cCuboid::Sort(void)
{
	if (p1.x > p2.x)
	{
		std::swap(p1.x, p2.x);
	}
	if (p1.y > p2.y)
	{
		std::swap(p1.y, p2.y);
	}
	if (p1.z > p2.z)
	{
		std::swap(p1.z, p2.z);
	}
}





bool cCuboid::DoesIntersect(const cCuboid & a_Other) const
{
	// In order for cuboids to intersect, each of their coord intervals need to intersect
	return (
		DoIntervalsIntersect(p1.x, p2.x, a_Other.p1.x, a_Other.p2.x) &&
		DoIntervalsIntersect(p1.y, p2.y, a_Other.p1.y, a_Other.p2.y) &&
		DoIntervalsIntersect(p1.z, p2.z, a_Other.p1.z, a_Other.p2.z)
	);
}





void cCuboid::Move(int a_OfsX, int a_OfsY, int a_OfsZ)
{
	p1.x += a_OfsX;
	p1.y += a_OfsY;
	p1.z += a_OfsZ;
	p2.x += a_OfsX;
	p2.y += a_OfsY;
	p2.z += a_OfsZ;
}





