#pragma once

#include <math.h>

class Vector3f;



// tolua_begin

class Vector3d
{
public:
	// convert from float
	Vector3d(const Vector3f & v);
	Vector3d(const Vector3f * v);

	Vector3d() : x(0), y(0), z(0) {}
	Vector3d(double a_x, double a_y, double a_z) : x(a_x), y(a_y), z(a_z) {}

	inline void Set(double a_x, double a_y, double a_z) { x = a_x, y = a_y, z = a_z; }
	inline void Normalize() { double l = 1.0f / Length(); x *= l; y *= l; z *= l; }
	inline Vector3d NormalizeCopy() { double l = 1.0f / Length(); return Vector3d( x * l, y * l, z * l ); }
	inline void NormalizeCopy(Vector3d & a_V) { double l = 1.0f / Length(); a_V.Set(x*l, y*l, z*l ); }
	inline double Length() const { return (double)sqrt( x * x + y * y + z * z ); }
	inline double SqrLength() const { return x * x + y * y + z * z; }
	inline double Dot( const Vector3d & a_V ) const { return x * a_V.x + y * a_V.y + z * a_V.z; }
	inline Vector3d Cross( const Vector3d & v ) const { return Vector3d( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x ); }
	
	/** Returns the coefficient for the (a_OtherEnd - this) line to reach the specified Z coord
	The result satisfies the following equation:
	(*this + Result * (a_OtherEnd - *this)).z = a_Z
	If the line is too close to being parallel, this function returns NO_INTERSECTION
	*/
	double LineCoeffToXYPlane(const Vector3d & a_OtherEnd, double a_Z) const;

	/** Returns the coefficient for the (a_OtherEnd - this) line to reach the specified Y coord
	The result satisfies the following equation:
	(*this + Result * (a_OtherEnd - *this)).y = a_Y
	If the line is too close to being parallel, this function returns NO_INTERSECTION
	*/
	double LineCoeffToXZPlane(const Vector3d & a_OtherEnd, double a_Y) const;

	/** Returns the coefficient for the (a_OtherEnd - this) line to reach the specified X coord
	The result satisfies the following equation:
	(*this + Result * (a_OtherEnd - *this)).x = a_X
	If the line is too close to being parallel, this function returns NO_INTERSECTION
	*/
	double LineCoeffToYZPlane(const Vector3d & a_OtherEnd, double a_X) const;

	inline bool Equals(const Vector3d & v) const { return ((x == v.x) && (y == v.y) && (z == v.z)); }
	
	// tolua_end

	void operator += ( const Vector3d& a_V ) { x += a_V.x; y += a_V.y; z += a_V.z; }
	void operator += ( Vector3d* a_V ) { x += a_V->x; y += a_V->y; z += a_V->z; }
	void operator -= ( const Vector3d& a_V ) { x -= a_V.x; y -= a_V.y; z -= a_V.z; }
	void operator -= ( Vector3d* a_V ) { x -= a_V->x; y -= a_V->y; z -= a_V->z; }
	void operator *= ( double a_f ) { x *= a_f; y *= a_f; z *= a_f; }

	// tolua_begin
	
	Vector3d operator + (const Vector3d & v2) const { return Vector3d(x + v2.x, y + v2.y, z + v2.z ); }
	Vector3d operator + (const Vector3d * v2) const { return Vector3d(x + v2->x, y + v2->y, z + v2->z ); }
	Vector3d operator - (const Vector3d & v2) const { return Vector3d(x - v2.x, y - v2.y, z - v2.z ); }
	Vector3d operator - (const Vector3d * v2) const { return Vector3d(x - v2->x, y - v2->y, z - v2->z ); }
	Vector3d operator * (const double f)      const { return Vector3d(x * f, y * f, z * f ); }
	Vector3d operator * (const Vector3d & v2) const { return Vector3d(x * v2.x, y * v2.y, z * v2.z ); }
	Vector3d operator / (const double f)      const { return Vector3d(x / f, y / f, z / f ); }

	double x, y, z;

	static const double EPS;              ///< The max difference between two coords for which the coords are assumed equal
	static const double NO_INTERSECTION;  ///< Return value of LineCoeffToPlane() if the line is parallel to the plane
} ;

// tolua_end




