
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Cuboid.h"





////////////////////////////////////////////////////////////////////////////////
// cCuboid:

void cCuboid::Assign(Vector3i a_Point1, Vector3i a_Point2)
{
	p1 = a_Point1;
	p2 = a_Point2;
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





void cCuboid::Move(Vector3i a_Offset)
{
	p1.Move(a_Offset);
	p2.Move(a_Offset);
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





void cCuboid::Clamp(const cCuboid & a_Limits)
{
	ASSERT(IsSorted());
	ASSERT(a_Limits.IsSorted());

	p1.x = ::Clamp(p1.x, a_Limits.p1.x, a_Limits.p2.x);
	p1.y = ::Clamp(p1.y, a_Limits.p1.y, a_Limits.p2.y);
	p1.z = ::Clamp(p1.z, a_Limits.p1.z, a_Limits.p2.z);
	p2.x = ::Clamp(p2.x, a_Limits.p1.x, a_Limits.p2.x);
	p2.y = ::Clamp(p2.y, a_Limits.p1.y, a_Limits.p2.y);
	p2.z = ::Clamp(p2.z, a_Limits.p1.z, a_Limits.p2.z);
}





void cCuboid::ClampSize(Vector3i a_MaxSize)
{
	ASSERT(IsSorted());

	if (p2.x - p1.x > a_MaxSize.x)
	{
		p2.x = p1.x + a_MaxSize.x;
	}
	if (p2.y - p1.y > a_MaxSize.y)
	{
		p2.y = p1.y + a_MaxSize.y;
	}
	if (p2.z - p1.z > a_MaxSize.z)
	{
		p2.z = p1.z + a_MaxSize.z;
	}
}





void cCuboid::ClampX(int a_MinX, int a_MaxX)
{
	p1.x = ::Clamp(p1.x, a_MinX, a_MaxX);
	p2.x = ::Clamp(p2.x, a_MinX, a_MaxX);
}





void cCuboid::ClampY(int a_MinY, int a_MaxY)
{
	p1.y = ::Clamp(p1.y, a_MinY, a_MaxY);
	p2.y = ::Clamp(p2.y, a_MinY, a_MaxY);
}





void cCuboid::ClampZ(int a_MinZ, int a_MaxZ)
{
	p1.z = ::Clamp(p1.z, a_MinZ, a_MaxZ);
	p2.z = ::Clamp(p2.z, a_MinZ, a_MaxZ);
}





bool cCuboid::IsSorted(void) const
{
	return (
		(p1.x <= p2.x) &&
		(p1.y <= p2.y) &&
		(p1.z <= p2.z)
	);
}





void cCuboid::Engulf(Vector3i a_Point)
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




