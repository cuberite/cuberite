#pragma once

// Note that this class is rather trivial and therefore it was inlined for performance.
// Note that this class has no .cpp file.


class cPath;
#include <vector>
using namespace std;
#include "PathFinder.h"

/** Typical usage:
cPath * myPath = ...;
Vector3d myPoint;
for(myPoint=myPath->getfirstPoint(); !myPath->isLastPoint(); myPoint=myPath->getNextPoint())
{
	Do something with myPoint here.
} */
class cPath
{
public:
	
	/** Returns the first point in the path. */
	inline Vector3d getFirstPoint()
	{
		m_CurrentPoint=m_PointCount - 1;
		return m_Points[m_CurrentPoint];
	}
	
	/** Returns the next point in the path. Assumes you called getFirstPoint at least once before. */
	inline Vector3d getnextPoint()
	{
		return m_Points[m_PointCount - 1 - (--m_CurrentPoint)];
	}
	
	/** Checks whether this is the last point or not. Never call getnextPoint when this is true. */
	inline bool isLastPoint()
	{
		return m_CurrentPoint == 0;
	}
	
	/** Get the point at a_index. Remark: Internally, the indexes are reversed. */
	inline Vector3d getPoint(int a_index)
	{
		return m_Points[m_PointCount - 1 - a_index];
	}
	
	/** Returns the total number of points this path has. */
	inline int getPointCount()
	{
		return m_PointCount;
	}
	
	/* inline int getPathLength(){return m_pathLength;} TODO */
	
	
private:
	// int m_pathLength;
	// inline void setPathLength(int len){m_pathLength=len;}
	
	int m_PointCount;
	int m_CurrentPoint;
	vector<Vector3d> m_Points;
	inline void addPoint(Vector3d a_Vector)
	{
		m_Points.push_back(a_Vector);
		++m_PointCount;
		printf("%d %d %d --- %d\n",a_Vector.x, a_Vector.y, a_Vector.z,m_PointCount);
	}
	
	
	friend class cPathFinder;
	
};
