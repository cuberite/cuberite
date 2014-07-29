
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cuboid.h"





/// Returns true if the two specified intervals have a non-empty union
static bool DoIntervalsIntersect(int a_Min1, int a_Max1, int a_Min2, int a_Max2)
{
	return (
		((a_Min1 >= a_Min2) && (a_Min1 <= a_Max2)) ||  // Start of first  interval is within the second interval
		((a_Max1 >= a_Min2) && (a_Max1 <= a_Max2)) ||  // End   of first  interval is within the second interval
		((a_Min2 >= a_Min1) && (a_Min2 <= a_Max1))     // Start of second interval is within the first interval
	);
}





////////////////////////////////////////////////////////////////////////////////
// cCuboid:

cCuboid & cCuboid::operator=(cCuboid a_Other)
{
	std::swap(p1, a_Other.p1);
	std::swap(p2, a_Other.p2);
	return *this;
}





void cCuboid::Assign(int a_X1, int a_Y1, int a_Z1, int a_X2, int a_Y2, int a_Z2)
{
	p1.x = a_X1;
	p1.y = a_Y1;
	p1.z = a_Z1;
	p2.x = a_X2;
	p2.y = a_Y2;
	p2.z = a_Z2;
}





void cCuboid::Assign(const cCuboid & a_SrcCuboid)
{
	p1.x = a_SrcCuboid.p1.x;
	p1.y = a_SrcCuboid.p1.y;
	p1.z = a_SrcCuboid.p1.z;
	p2.x = a_SrcCuboid.p2.x;
	p2.y = a_SrcCuboid.p2.y;
	p2.z = a_SrcCuboid.p2.z;
}





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





int cCuboid::GetVolume(void) const
{
	int DifX = abs(p2.x - p1.x) + 1;
	int DifY = abs(p2.y - p1.y) + 1;
	int DifZ = abs(p2.z - p1.z) + 1;
	return DifX * DifY * DifZ;
}





bool cCuboid::DoesIntersect(const cCuboid & a_Other) const
{
	ASSERT(IsSorted());
	ASSERT(a_Other.IsSorted());
	
	// In order for cuboids to intersect, each of their coord intervals need to intersect
	return (
		DoIntervalsIntersect(p1.x, p2.x, a_Other.p1.x, a_Other.p2.x) &&
		DoIntervalsIntersect(p1.y, p2.y, a_Other.p1.y, a_Other.p2.y) &&
		DoIntervalsIntersect(p1.z, p2.z, a_Other.p1.z, a_Other.p2.z)
	);
}





bool cCuboid::IsCompletelyInside(const cCuboid & a_Outer) const
{
	ASSERT(IsSorted());
	ASSERT(a_Outer.IsSorted());
	
	return (
		(p1.x >= a_Outer.p1.x) &&
		(p2.x <= a_Outer.p2.x) &&
		(p1.y >= a_Outer.p1.y) &&
		(p2.y <= a_Outer.p2.y) &&
		(p1.z >= a_Outer.p1.z) &&
		(p2.z <= a_Outer.p2.z)
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






void cCuboid::Expand(int a_SubMinX, int a_AddMaxX, int a_SubMinY, int a_AddMaxY, int a_SubMinZ, int a_AddMaxZ)
{
	if (p1.x < p2.x)
	{
		p1.x -= a_SubMinX;
		p2.x += a_AddMaxX;
	}
	else
	{
		p2.x -= a_SubMinX;
		p1.x += a_AddMaxX;
	}
	
	if (p1.y < p2.y)
	{
		p1.y -= a_SubMinY;
		p2.y += a_AddMaxY;
	}
	else
	{
		p2.y -= a_SubMinY;
		p1.y += a_AddMaxY;
	}
	
	if (p1.z < p2.z)
	{
		p1.z -= a_SubMinZ;
		p2.z += a_AddMaxZ;
	}
	else
	{
		p2.z -= a_SubMinZ;
		p1.z += a_AddMaxZ;
	}
}





void cCuboid::ClampX(int a_MinX, int a_MaxX)
{
	p1.x = Clamp(p1.x, a_MinX, a_MaxX);
	p2.x = Clamp(p2.x, a_MinX, a_MaxX);
}





void cCuboid::ClampY(int a_MinY, int a_MaxY)
{
	p1.y = Clamp(p1.y, a_MinY, a_MaxY);
	p2.y = Clamp(p2.y, a_MinY, a_MaxY);
}





void cCuboid::ClampZ(int a_MinZ, int a_MaxZ)
{
	p1.z = Clamp(p1.z, a_MinZ, a_MaxZ);
	p2.z = Clamp(p2.z, a_MinZ, a_MaxZ);
}





bool cCuboid::IsSorted(void) const
{
	return (
		(p1.x <= p2.x) &&
		(p1.y <= p2.y) &&
		(p1.z <= p2.z)
	);
}





void cCuboid::Engulf(const Vector3i & a_Point)
{
	if (a_Point.x < p1.x)
	{
		p1.x = a_Point.x;
	}
	else if (a_Point.x > p2.x)
	{
		p2.x = a_Point.x;
	}

	if (a_Point.y < p1.y)
	{
		p1.y = a_Point.y;
	}
	else if (a_Point.y > p2.y)
	{
		p2.y = a_Point.y;
	}

	if (a_Point.z < p1.z)
	{
		p1.z = a_Point.z;
	}
	else if (a_Point.z > p2.z)
	{
		p2.z = a_Point.z;
	}
}




