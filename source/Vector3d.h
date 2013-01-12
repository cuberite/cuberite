#pragma once

#include <math.h>

class Vector3f;
class Vector3d	// tolua_export
{				// tolua_export
public:			// tolua_export
	// convert from float
	Vector3d(const Vector3f & v );	// tolua_export
	Vector3d(const Vector3f * v );	// tolua_export

	Vector3d() : x(0), y(0), z(0) {}																									// tolua_export
	Vector3d(double a_x, double a_y, double a_z) : x(a_x), y(a_y), z(a_z) {}															// tolua_export

	inline void Set(double a_x, double a_y, double a_z) { x = a_x, y = a_y, z = a_z; }													// tolua_export
	inline void Normalize() { double l = 1.0f / Length(); x *= l; y *= l; z *= l; }														// tolua_export
	inline Vector3d NormalizeCopy() { double l = 1.0f / Length(); return Vector3d( x * l, y * l, z * l ); }								// tolua_export
	inline void NormalizeCopy(Vector3d & a_V) { double l = 1.0f / Length(); a_V.Set(x*l, y*l, z*l ); }									// tolua_export
	inline double Length() const { return (double)sqrt( x * x + y * y + z * z ); }														// tolua_export
	inline double SqrLength() const { return x * x + y * y + z * z; }																	// tolua_export
	inline double Dot( const Vector3d & a_V ) const { return x * a_V.x + y * a_V.y + z * a_V.z; }										// tolua_export
	inline Vector3d Cross( const Vector3d & v ) const { return Vector3d( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x ); }	// tolua_export

	inline bool Equals( const Vector3d & v ) const { return (x == v.x && y == v.y && z == v.z ); }										// tolua_export

	void operator += ( const Vector3d& a_V ) { x += a_V.x; y += a_V.y; z += a_V.z; }
	void operator += ( Vector3d* a_V ) { x += a_V->x; y += a_V->y; z += a_V->z; }
	void operator -= ( const Vector3d& a_V ) { x -= a_V.x; y -= a_V.y; z -= a_V.z; }
	void operator -= ( Vector3d* a_V ) { x -= a_V->x; y -= a_V->y; z -= a_V->z; }
	void operator *= ( double a_f ) { x *= a_f; y *= a_f; z *= a_f; }

	Vector3d operator + ( const Vector3d& v2 ) const { return Vector3d( x + v2.x, y + v2.y, z + v2.z ); }									// tolua_export
	Vector3d operator + ( const Vector3d* v2 ) const { return Vector3d( x + v2->x, y + v2->y, z + v2->z ); }								// tolua_export
	Vector3d operator - ( const Vector3d& v2 ) const { return Vector3d( x - v2.x, y - v2.y, z - v2.z ); }									// tolua_export
	Vector3d operator - ( const Vector3d* v2 ) const { return Vector3d( x - v2->x, y - v2->y, z - v2->z ); }								// tolua_export
	Vector3d operator * ( const double f ) const { return Vector3d( x * f, y * f, z * f ); }												// tolua_export
	Vector3d operator * ( const Vector3d& v2 ) const { return Vector3d( x * v2.x, y * v2.y, z * v2.z ); }									// tolua_export

	double x, y, z;	// tolua_export

};// tolua_export
