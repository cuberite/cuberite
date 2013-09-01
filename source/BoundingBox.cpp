
// BoundingBox.cpp

// Implements the cBoundingBox class representing an axis-aligned bounding box with floatingpoint coords

#include "Globals.h"
#include "BoundingBox.h"





cBoundingBox::cBoundingBox(double a_MinX, double a_MaxX, double a_MinY, double a_MaxY, double a_MinZ, double a_MaxZ) :
	m_Min(a_MinX, a_MinY, a_MinZ),
	m_Max(a_MaxX, a_MaxY, a_MaxZ)
{
}





cBoundingBox::cBoundingBox(const Vector3d & a_Min, const Vector3d & a_Max) :
	m_Min(a_Min),
	m_Max(a_Max)
{
}





cBoundingBox::cBoundingBox(const Vector3d & a_Pos, double a_Radius, double a_Height) :
	m_Min(a_Pos.x - a_Radius, a_Pos.y,            a_Pos.z - a_Radius),
	m_Max(a_Pos.x + a_Radius, a_Pos.y + a_Height, a_Pos.z + a_Radius)
{
}





cBoundingBox::cBoundingBox(const cBoundingBox & a_Orig) :
	m_Min(a_Orig.m_Min),
	m_Max(a_Orig.m_Max)
{
}





void cBoundingBox::Move(double a_OffX, double a_OffY, double a_OffZ)
{
	m_Min.x += a_OffX;
	m_Min.y += a_OffY;
	m_Min.z += a_OffZ;
	m_Max.x += a_OffX;
	m_Max.y += a_OffY;
	m_Max.z += a_OffZ;
}





void cBoundingBox::Move(const Vector3d & a_Off)
{
	m_Min.x += a_Off.x;
	m_Min.y += a_Off.y;
	m_Min.z += a_Off.z;
	m_Max.x += a_Off.x;
	m_Max.y += a_Off.y;
	m_Max.z += a_Off.z;
}





void cBoundingBox::Expand(double a_ExpandX, double a_ExpandY, double a_ExpandZ)
{
	m_Min.x -= a_ExpandX;
	m_Min.y -= a_ExpandY;
	m_Min.z -= a_ExpandZ;
	m_Max.x += a_ExpandX;
	m_Max.y += a_ExpandY;
	m_Max.z += a_ExpandZ;
}





bool cBoundingBox::DoesIntersect(const cBoundingBox & a_Other)
{
	return (
		((a_Other.m_Min.x < m_Max.x) && (a_Other.m_Max.x > m_Min.x)) &&  // X coords intersect
		((a_Other.m_Min.y < m_Max.y) && (a_Other.m_Max.y > m_Min.y)) &&  // Y coords intersect
		((a_Other.m_Min.z < m_Max.z) && (a_Other.m_Max.z > m_Min.z))     // Z coords intersect
	);
}





cBoundingBox cBoundingBox::Union(const cBoundingBox & a_Other)
{
	return cBoundingBox(
		std::min(m_Min.x, a_Other.m_Min.x),
		std::min(m_Min.y, a_Other.m_Min.y),
		std::min(m_Min.z, a_Other.m_Min.z),
		std::max(m_Max.x, a_Other.m_Max.x),
		std::max(m_Max.y, a_Other.m_Max.y),
		std::max(m_Max.z, a_Other.m_Max.z)
	);
}





bool cBoundingBox::IsInside(const Vector3d & a_Point)
{
	return (
		((a_Point.x >= m_Min.x) && (a_Point.x < m_Max.x)) &&
		((a_Point.y >= m_Min.y) && (a_Point.y < m_Max.y)) &&
		((a_Point.z >= m_Min.z) && (a_Point.z < m_Max.z))
	);
}





bool cBoundingBox::IsInside(double a_X, double a_Y,double a_Z)
{
	return (
		((a_X >= m_Min.x) && (a_X < m_Max.x)) &&
		((a_Y >= m_Min.y) && (a_Y < m_Max.y)) &&
		((a_Z >= m_Min.z) && (a_Z < m_Max.z))
	);
}





bool cBoundingBox::IsInside(cBoundingBox & a_Other)
{
	// If both a_Other's coords are inside this, then the entire a_Other is inside
	return (IsInside(a_Other.m_Min) && IsInside(a_Other.m_Max));
}





bool cBoundingBox::IsInside(const Vector3d & a_Min, const Vector3d & a_Max)
{
	// If both coords are inside this, then the entire a_Other is inside
	return (IsInside(a_Min) && IsInside(a_Max));
}





bool cBoundingBox::Intersect(const cBoundingBox & a_Other, cBoundingBox & a_Intersection)
{
	a_Intersection.m_Min.x = std::max(m_Min.x, a_Other.m_Min.x);
	a_Intersection.m_Max.x = std::min(m_Max.x, a_Other.m_Max.x);
	if (a_Intersection.m_Min.x >= a_Intersection.m_Max.x)
	{
		return false;
	}
	a_Intersection.m_Min.y = std::max(m_Min.y, a_Other.m_Min.y);
	a_Intersection.m_Max.y = std::min(m_Max.y, a_Other.m_Max.y);
	if (a_Intersection.m_Min.y >= a_Intersection.m_Max.y)
	{
		return false;
	}
	a_Intersection.m_Min.z = std::max(m_Min.z, a_Other.m_Min.z);
	a_Intersection.m_Max.z = std::min(m_Max.z, a_Other.m_Max.z);
	if (a_Intersection.m_Min.z >= a_Intersection.m_Max.z)
	{
		return false;
	}
	return true;
}




