
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Vector3d.h"
#include "Vector3f.h"





const double Vector3d::EPS = 0.000001;          ///< The max difference between two coords for which the coords are assumed equal
const double Vector3d::NO_INTERSECTION = 1e70;  ///< Return value of LineCoeffToPlane() if the line is parallel to the plane





Vector3d::Vector3d(const Vector3f & v) :
	x(v.x),
	y(v.y),
	z(v.z)
{
}





Vector3d::Vector3d(const Vector3f * v) :
	x(v->x),
	y(v->y),
	z(v->z)
{
}





double Vector3d::LineCoeffToXYPlane(const Vector3d & a_OtherEnd, double a_Z) const
{
	if (abs(z - a_OtherEnd.z) < EPS)
	{
		return NO_INTERSECTION;
	}
	return (a_Z - z) / (a_OtherEnd.z - z);
}





double Vector3d::LineCoeffToXZPlane(const Vector3d & a_OtherEnd, double a_Y) const
{
	if (abs(y - a_OtherEnd.y) < EPS)
	{
		return NO_INTERSECTION;
	}
	return (a_Y - y) / (a_OtherEnd.y - y);
}





double Vector3d::LineCoeffToYZPlane(const Vector3d & a_OtherEnd, double a_X) const
{
	if (abs(x - a_OtherEnd.x) < EPS)
	{
		return NO_INTERSECTION;
	}
	return (a_X - x) / (a_OtherEnd.x - x);
}




