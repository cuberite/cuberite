
// BoundingBox.h

// Declares the cBoundingBox class representing an axis-aligned bounding box with floatingpoint coords




#pragma once

#include "Vector3.h"
#include "Defines.h"





// tolua_begin

/** Represents two sets of coords, minimum and maximum for each direction.
All the coords within those limits (inclusive the edges) are considered "inside" the box.
For intersection purposes, though, if the intersection is "sharp" in any coord (i. e. zero volume),
the boxes are considered non-intersecting.
*/
class cBoundingBox
{
public:
	cBoundingBox(double a_MinX, double a_MaxX, double a_MinY, double a_MaxY, double a_MinZ, double a_MaxZ);
	cBoundingBox(const Vector3d & a_Min, const Vector3d & a_Max);
	cBoundingBox(const Vector3d & a_Pos, double a_Radius, double a_Height);
	cBoundingBox(const cBoundingBox & a_Orig);
	
	/// Moves the entire boundingbox by the specified offset
	void Move(double a_OffX, double a_OffY, double a_OffZ);
	
	/// Moves the entire boundingbox by the specified offset
	void Move(const Vector3d & a_Off);
	
	/// Expands the bounding box by the specified amount in each direction (so the box becomes larger by 2 * Expand in each direction)
	void Expand(double a_ExpandX, double a_ExpandY, double a_ExpandZ);
	
	/// Returns true if the two bounding boxes intersect
	bool DoesIntersect(const cBoundingBox & a_Other);
	
	/// Returns the union of the two bounding boxes
	cBoundingBox Union(const cBoundingBox & a_Other);
	
	/// Returns true if the point is inside the bounding box
	bool IsInside(const Vector3d & a_Point);
	
	/// Returns true if the point is inside the bounding box
	bool IsInside(double a_X, double a_Y, double a_Z);
	
	/// Returns true if a_Other is inside this bounding box
	bool IsInside(cBoundingBox & a_Other);
	
	/// Returns true if a boundingbox specified by a_Min and a_Max is inside this bounding box
	bool IsInside(const Vector3d & a_Min, const Vector3d & a_Max);
	
	/// Returns true if the specified point is inside the bounding box specified by its min/max corners
	static bool IsInside(const Vector3d & a_Min, const Vector3d & a_Max, const Vector3d & a_Point);
	
	/// Returns true if the specified point is inside the bounding box specified by its min/max corners
	static bool IsInside(const Vector3d & a_Min, const Vector3d & a_Max, double a_X, double a_Y, double a_Z);
	
	/** Returns true if this bounding box is intersected by the line specified by its two points
	Also calculates the distance along the line in which the intersection occurs (0 .. 1)
	Only forward collisions (a_LineCoeff >= 0) are returned.
	*/
	bool CalcLineIntersection(const Vector3d & a_Line1, const Vector3d & a_Line2, double & a_LineCoeff, eBlockFace & a_Face);
	
	/** Returns true if the specified bounding box is intersected by the line specified by its two points
	Also calculates the distance along the line in which the intersection occurs (0 .. 1) and the face hit (BLOCK_FACE_ constants)
	Only forward collisions (a_LineCoeff >= 0) are returned.
	*/
	static bool CalcLineIntersection(const Vector3d & a_Min, const Vector3d & a_Max, const Vector3d & a_Line1, const Vector3d & a_Line2, double & a_LineCoeff, eBlockFace & a_Face);
	
	// tolua_end
	
	/// Calculates the intersection of the two bounding boxes; returns true if nonempty
	bool Intersect(const cBoundingBox & a_Other, cBoundingBox & a_Intersection);
	
protected:
	Vector3d m_Min;
	Vector3d m_Max;
	
} ;  // tolua_export




