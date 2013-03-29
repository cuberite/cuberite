
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cuboid.h"





void cCuboid::Sort()
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




