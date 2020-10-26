// BoundingBox.cpp

// Implements the cBoundingBox class representing an axis-aligned bounding box with floatingpoint coords

#include "Globals.h"
#include "BoundingBox.h"





cBoundingBox::cBoundingBox(double a_MinX, double a_MaxX, double a_MinY, double a_MaxY, double a_MinZ, double a_MaxZ) :
	m_Min(a_MinX, a_MinY, a_MinZ),
	m_Max(a_MaxX, a_MaxY, a_MaxZ)
{
}





cBoundingBox::cBoundingBox(Vector3d a_Min, Vector3d a_Max) :
	m_Min(a_Min),
	m_Max(a_Max)
{
}





cBoundingBox::cBoundingBox(Vector3d a_Pos, double a_Radius, double a_Height) :
	m_Min(a_Pos.x - a_Radius, a_Pos.y,            a_Pos.z - a_Radius),
	m_Max(a_Pos.x + a_Radius, a_Pos.y + a_Height, a_Pos.z + a_Radius)
{
}





cBoundingBox::cBoundingBox(Vector3d a_Pos, double a_Radius, double a_Height, double a_VerticalOffset) :
	m_Min(a_Pos.x - a_Radius, a_Pos.y + a_VerticalOffset, a_Pos.z - a_Radius),
	m_Max(a_Pos.x + a_Radius, a_Pos.y + a_VerticalOffset + a_Height, a_Pos.z + a_Radius)
{
}





cBoundingBox::cBoundingBox(Vector3d a_Pos, double a_CubeLength) :
	m_Min(a_Pos.x - a_CubeLength / 2, a_Pos.y - a_CubeLength / 2, a_Pos.z - a_CubeLength / 2),
	m_Max(a_Pos.x + a_CubeLength / 2, a_Pos.y + a_CubeLength / 2, a_Pos.z + a_CubeLength / 2)
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





void cBoundingBox::Move(Vector3d a_Off)
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
		((a_Other.m_Min.x <= m_Max.x) && (a_Other.m_Max.x >= m_Min.x)) &&  // X coords intersect
		((a_Other.m_Min.y <= m_Max.y) && (a_Other.m_Max.y >= m_Min.y)) &&  // Y coords intersect
		((a_Other.m_Min.z <= m_Max.z) && (a_Other.m_Max.z >= m_Min.z))     // Z coords intersect
	);
}





cBoundingBox cBoundingBox::Union(const cBoundingBox & a_Other)
{
	return cBoundingBox(
		std::min(m_Min.x, a_Other.m_Min.x),
		std::max(m_Max.x, a_Other.m_Max.x),
		std::min(m_Min.y, a_Other.m_Min.y),
		std::max(m_Max.y, a_Other.m_Max.y),
		std::min(m_Min.z, a_Other.m_Min.z),
		std::max(m_Max.z, a_Other.m_Max.z)
	);
}





bool cBoundingBox::IsInside(Vector3d a_Point)
{
	return IsInside(m_Min, m_Max, a_Point);
}





bool cBoundingBox::IsInside(double a_X, double a_Y, double a_Z)
{
	return IsInside(m_Min, m_Max, a_X, a_Y, a_Z);
}





bool cBoundingBox::IsInside(cBoundingBox & a_Other)
{
	// If both a_Other's coords are inside this, then the entire a_Other is inside
	return (IsInside(a_Other.m_Min) && IsInside(a_Other.m_Max));
}





bool cBoundingBox::IsInside(Vector3d a_Min, Vector3d a_Max)
{
	// If both coords are inside this, then the entire a_Other is inside
	return (IsInside(a_Min) && IsInside(a_Max));
}





bool cBoundingBox::IsInside(Vector3d a_Min, Vector3d a_Max, Vector3d a_Point)
{
	return (
		((a_Point.x >= a_Min.x) && (a_Point.x <= a_Max.x)) &&
		((a_Point.y >= a_Min.y) && (a_Point.y <= a_Max.y)) &&
		((a_Point.z >= a_Min.z) && (a_Point.z <= a_Max.z))
	);
}





bool cBoundingBox::IsInside(Vector3d a_Min, Vector3d a_Max, double a_X, double a_Y, double a_Z)
{
	return (
		((a_X >= a_Min.x) && (a_X <= a_Max.x)) &&
		((a_Y >= a_Min.y) && (a_Y <= a_Max.y)) &&
		((a_Z >= a_Min.z) && (a_Z <= a_Max.z))
	);
}





bool cBoundingBox::CalcLineIntersection(Vector3d a_Line1, Vector3d a_Line2, double & a_LineCoeff, eBlockFace & a_Face) const
{
	return CalcLineIntersection(m_Min, m_Max, a_Line1, a_Line2, a_LineCoeff, a_Face);
}





bool cBoundingBox::CalcLineIntersection(Vector3d a_Min, Vector3d a_Max, Vector3d a_Line1, Vector3d a_Line2, double & a_LineCoeff, eBlockFace & a_Face)
{
	if (IsInside(a_Min, a_Max, a_Line1))
	{
		// The starting point is inside the bounding box.
		a_LineCoeff = 0;
		a_Face = BLOCK_FACE_NONE;  // No faces hit
		return true;
	}

	eBlockFace Face = BLOCK_FACE_NONE;
	double Coeff = Vector3d::NO_INTERSECTION;

	// Check each individual bbox face for intersection with the line, remember the one with the lowest coeff
	double c = a_Line1.LineCoeffToXYPlane(a_Line2, a_Min.z);
	if ((c >= 0) && (c < Coeff) && IsInside(a_Min, a_Max, a_Line1 + (a_Line2 - a_Line1) * c))
	{
		Face = (a_Line1.z > a_Line2.z) ? BLOCK_FACE_ZP : BLOCK_FACE_ZM;
		Coeff = c;
	}
	c = a_Line1.LineCoeffToXYPlane(a_Line2, a_Max.z);
	if ((c >= 0) && (c < Coeff) && IsInside(a_Min, a_Max, a_Line1 + (a_Line2 - a_Line1) * c))
	{
		Face = (a_Line1.z > a_Line2.z) ? BLOCK_FACE_ZP : BLOCK_FACE_ZM;
		Coeff = c;
	}
	c = a_Line1.LineCoeffToXZPlane(a_Line2, a_Min.y);
	if ((c >= 0) && (c < Coeff) && IsInside(a_Min, a_Max, a_Line1 + (a_Line2 - a_Line1) * c))
	{
		Face = (a_Line1.y > a_Line2.y) ? BLOCK_FACE_YP : BLOCK_FACE_YM;
		Coeff = c;
	}
	c = a_Line1.LineCoeffToXZPlane(a_Line2, a_Max.y);
	if ((c >= 0) && (c < Coeff) && IsInside(a_Min, a_Max, a_Line1 + (a_Line2 - a_Line1) * c))
	{
		Face = (a_Line1.y > a_Line2.y) ? BLOCK_FACE_YP : BLOCK_FACE_YM;
		Coeff = c;
	}
	c = a_Line1.LineCoeffToYZPlane(a_Line2, a_Min.x);
	if ((c >= 0) && (c < Coeff) && IsInside(a_Min, a_Max, a_Line1 + (a_Line2 - a_Line1) * c))
	{
		Face = (a_Line1.x > a_Line2.x) ? BLOCK_FACE_XP : BLOCK_FACE_XM;
		Coeff = c;
	}
	c = a_Line1.LineCoeffToYZPlane(a_Line2, a_Max.x);
	if ((c >= 0) && (c < Coeff) && IsInside(a_Min, a_Max, a_Line1 + (a_Line2 - a_Line1) * c))
	{
		Face = (a_Line1.x > a_Line2.x) ? BLOCK_FACE_XP : BLOCK_FACE_XM;
		Coeff = c;
	}

	if (Coeff >= Vector3d::NO_INTERSECTION)
	{
		// There has been no intersection
		return false;
	}

	a_LineCoeff = Coeff;
	a_Face = Face;
	return true;
}





bool cBoundingBox::Intersect(const cBoundingBox & a_Other, cBoundingBox & a_Intersection) const
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
	return (a_Intersection.m_Min.z < a_Intersection.m_Max.z);
}




