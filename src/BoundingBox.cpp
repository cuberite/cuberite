// BoundingBox.cpp

// Implements the cBoundingBox class representing an axis-aligned bounding box with floatingpoint coords

#include "Globals.h"
#include "BoundingBox.h"
#include "Defines.h"





#ifdef SELF_TEST

/** A simple self-test that is executed on program start, used to verify bbox functionality */
static class SelfTest_BoundingBox
{
public:
	SelfTest_BoundingBox(void)
	{
		Vector3d Min(1, 1, 1);
		Vector3d Max(2, 2, 2);
		Vector3d LineDefs[] =
		{
			Vector3d(1.5,   4, 1.5), Vector3d(1.5,   3, 1.5),  // Should intersect at 2,    face 1 (YP)
			Vector3d(1.5,   0, 1.5), Vector3d(1.5,   4, 1.5),  // Should intersect at 0.25, face 0 (YM)
			Vector3d(0,     0,   0), Vector3d(2,     2,   2),  // Should intersect at 0.5,  face 0, 3 or 5 (anyM)
			Vector3d(0.999, 0, 1.5), Vector3d(0.999, 4, 1.5),  // Should not intersect
			Vector3d(1.999, 0, 1.5), Vector3d(1.999, 4, 1.5),  // Should intersect at 0.25, face 0 (YM)
			Vector3d(2.001, 0, 1.5), Vector3d(2.001, 4, 1.5),  // Should not intersect
		} ;
		bool Results[] = {true, true, true, false, true, false};
		double LineCoeffs[] = {2, 0.25, 0.5, 0, 0.25, 0};
		
		for (size_t i = 0; i < ARRAYCOUNT(LineDefs) / 2; i++)
		{
			double LineCoeff;
			eBlockFace Face;
			Vector3d Line1 = LineDefs[2 * i];
			Vector3d Line2 = LineDefs[2 * i + 1];
			bool res = cBoundingBox::CalcLineIntersection(Min, Max, Line1, Line2, LineCoeff, Face);
			if (res != Results[i])
			{
				fprintf(stderr, "LineIntersection({%.02f, %.02f, %.02f}, {%.02f, %.02f, %.02f}) -> %d, %.05f, %d\n",
					Line1.x, Line1.y, Line1.z,
					Line2.x, Line2.y, Line2.z,
					res ? 1 : 0, LineCoeff, Face
				);
				abort();
			}
			if (res)
			{
				if (LineCoeff != LineCoeffs[i])
				{
					fprintf(stderr, "LineIntersection({%.02f, %.02f, %.02f}, {%.02f, %.02f, %.02f}) -> %d, %.05f, %d\n",
						Line1.x, Line1.y, Line1.z,
						Line2.x, Line2.y, Line2.z,
						res ? 1 : 0, LineCoeff, Face
					);
					abort();
				}
			}
		}  // for i - LineDefs[]
		fprintf(stderr, "BoundingBox selftest complete.\n");
	}
} gTest;

#endif





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
		((a_Other.m_Min.x <= m_Max.x) && (a_Other.m_Max.x >= m_Min.x)) &&  // X coords intersect
		((a_Other.m_Min.y <= m_Max.y) && (a_Other.m_Max.y >= m_Min.y)) &&  // Y coords intersect
		((a_Other.m_Min.z <= m_Max.z) && (a_Other.m_Max.z >= m_Min.z))     // Z coords intersect
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





bool cBoundingBox::IsInside(const Vector3d & a_Min, const Vector3d & a_Max)
{
	// If both coords are inside this, then the entire a_Other is inside
	return (IsInside(a_Min) && IsInside(a_Max));
}





bool cBoundingBox::IsInside(const Vector3d & a_Min, const Vector3d & a_Max, const Vector3d & a_Point)
{
	return (
		((a_Point.x >= a_Min.x) && (a_Point.x <= a_Max.x)) &&
		((a_Point.y >= a_Min.y) && (a_Point.y <= a_Max.y)) &&
		((a_Point.z >= a_Min.z) && (a_Point.z <= a_Max.z))
	);
}





bool cBoundingBox::IsInside(const Vector3d & a_Min, const Vector3d & a_Max, double a_X, double a_Y, double a_Z)
{
	return (
		((a_X >= a_Min.x) && (a_X <= a_Max.x)) &&
		((a_Y >= a_Min.y) && (a_Y <= a_Max.y)) &&
		((a_Z >= a_Min.z) && (a_Z <= a_Max.z))
	);
}





bool cBoundingBox::CalcLineIntersection(const Vector3d & a_Line1, const Vector3d & a_Line2, double & a_LineCoeff, eBlockFace & a_Face)
{
	return CalcLineIntersection(m_Min, m_Max, a_Line1, a_Line2, a_LineCoeff, a_Face);
}





bool cBoundingBox::CalcLineIntersection(const Vector3d & a_Min, const Vector3d & a_Max, const Vector3d & a_Line1, const Vector3d & a_Line2, double & a_LineCoeff, eBlockFace & a_Face)
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




