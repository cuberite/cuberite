
// BoundingBox.h

// Declares the cBoundingBox class representing an axis-aligned bounding box with floatingpoint coords




#pragma once

#include "Defines.h"





// tolua_begin

/** Represents two sets of coords, minimum and maximum for each direction.
All the coords within those limits (inclusive the edges) are considered "inside" the box.
For intersection purposes, though, if the intersection is "sharp" in any coord (i. e. zero volume),
the boxes are considered non-intersecting. */
class cBoundingBox
{
public:
	cBoundingBox(double a_MinX, double a_MaxX, double a_MinY, double a_MaxY, double a_MinZ, double a_MaxZ);
	cBoundingBox(Vector3d a_Min, Vector3d a_Max);
	cBoundingBox(Vector3d a_Pos, double a_Radius, double a_Height);
	/** Constructor that allows to define a bounding box given a center point (a_Pos), a horizontal radius (a_Radius),
	a height starting from given center point (a_Height) and a vertical offset (a_VerticalOffset) to adjust the vertical starting point.
	For example: cBoundingBox([0, 0, 0], 6, 6, -3) would create a bounding cube from (-3, -3, -3) to (3, 3, 3). */
	cBoundingBox(Vector3d a_Pos, double a_Radius, double a_Height, double a_VerticalOffset);
	cBoundingBox(Vector3d a_Pos, double a_CubeLength);

	#ifdef TOLUA_EXPOSITION  // tolua isn't aware of implicitly generated copy constructors
		cBoundingBox(const cBoundingBox & a_Orig);
	#endif

	/** Moves the entire boundingbox by the specified offset */
	void Move(double a_OffX, double a_OffY, double a_OffZ);

	/** Moves the entire boundingbox by the specified offset */
	void Move(Vector3d a_Off);

	/** Expands the bounding box by the specified amount in each direction (so the box becomes larger by 2 * Expand in each direction) */
	void Expand(double a_ExpandX, double a_ExpandY, double a_ExpandZ);

	/** Returns true if the two bounding boxes intersect */
	bool DoesIntersect(const cBoundingBox & a_Other);

	/** Returns the union of the two bounding boxes */
	cBoundingBox Union(const cBoundingBox & a_Other);

	/** Returns true if the point is inside the bounding box */
	bool IsInside(Vector3d a_Point);

	/** Returns true if the point is inside the bounding box */
	bool IsInside(double a_X, double a_Y, double a_Z);

	/** Returns true if a_Other is inside this bounding box */
	bool IsInside(cBoundingBox & a_Other);

	/** Returns true if a boundingbox specified by a_Min and a_Max is inside this bounding box */
	bool IsInside(Vector3d a_Min, Vector3d a_Max);

	/** Returns true if the specified point is inside the bounding box specified by its min / max corners */
	static bool IsInside(Vector3d a_Min, Vector3d a_Max, Vector3d a_Point);

	/** Returns true if the specified point is inside the bounding box specified by its min / max corners */
	static bool IsInside(Vector3d a_Min, Vector3d a_Max, double a_X, double a_Y, double a_Z);

	// tolua_end

	/** Returns true if this bounding box is intersected by the line specified by its two points
	Also calculates the distance along the line in which the intersection occurs, and the face hit (BLOCK_FACE_ constants)
	Only forward collisions (a_LineCoeff >= 0) are returned.
	Exported to Lua manually, because ToLua++ would generate needless input params (a_LineCoeff, a_Face). */
	bool CalcLineIntersection(Vector3d a_LinePoint1, Vector3d a_LinePoint2, double & a_LineCoeff, eBlockFace & a_Face) const;

	/** Returns true if the specified bounding box is intersected by the line specified by its two points
	Also calculates the distance along the line in which the intersection occurs, and the face hit (BLOCK_FACE_ constants)
	Only forward collisions (a_LineCoeff >= 0) are returned.
	Exported to Lua manually, because ToLua++ would generate needless input params (a_LineCoeff, a_Face). */
	static bool CalcLineIntersection(Vector3d a_Min, Vector3d a_Max, Vector3d a_LinePoint1, Vector3d a_LinePoint2, double & a_LineCoeff, eBlockFace & a_Face);

	/** Calculates the intersection of the two bounding boxes; returns true if nonempty.
	Exported manually, because ToLua++ would generate needless input params (a_Intersection). */
	bool Intersect(const cBoundingBox & a_Other, cBoundingBox & a_Intersection) const;

	// tolua_begin

	double GetMinX(void) const { return m_Min.x; }
	double GetMinY(void) const { return m_Min.y; }
	double GetMinZ(void) const { return m_Min.z; }

	double GetMaxX(void) const { return m_Max.x; }
	double GetMaxY(void) const { return m_Max.y; }
	double GetMaxZ(void) const { return m_Max.z; }

	Vector3d GetMin(void) const { return m_Min; }
	Vector3d GetMax(void) const { return m_Max; }

	// tolua_end

protected:
	Vector3d m_Min;
	Vector3d m_Max;

} ;  // tolua_export




