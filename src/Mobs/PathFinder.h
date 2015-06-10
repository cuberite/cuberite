#pragma once
class cPathFinder;
class cPath;
class cChunk;

class cPathFinder
{
public:
	cPathFinder(double a_BoundingBoxWidth, double a_BoundingBoxHeight);
	Vector3d GetWayPoint(cChunk & a_Chunk, const Vector3d & a_CurrentLocation, Vector3d & a_Destination);
private:
	bool EnsureProperDestination(cChunk & a_Chunk, Vector3d & a_Destination);
	std::unique_ptr<cPath> m_Path;
	int m_BoundingBoxWidth;
	int m_BoundingBoxHeight;
};
