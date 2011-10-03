#pragma once

#include <math.h>

class Vector3i;
class Vector3d;
class Vector3f	//tolua_export
{				//tolua_export
public:			//tolua_export
	Vector3f( const Vector3d & v );	//tolua_export
	Vector3f( const Vector3d * v );	//tolua_export
	Vector3f( const Vector3i & v );	//tolua_export
	Vector3f( const Vector3i * v );	//tolua_export


	Vector3f() : x(0), y(0), z(0) {}																			//tolua_export
	Vector3f(float a_x, float a_y, float a_z) : x(a_x), y(a_y), z(a_z) {}										//tolua_export

	inline void Set(float a_x, float a_y, float a_z) { x = a_x, y = a_y, z = a_z; }								//tolua_export
	inline void Normalize() { float l = 1.0f / Length(); x *= l; y *= l; z *= l; }								//tolua_export
	inline Vector3f NormalizeCopy() const { float l = 1.0f / Length(); return Vector3f( x * l, y * l, z * l ); }//tolua_export
	inline void NormalizeCopy(Vector3f & a_V) const { float l = 1.0f / Length(); a_V.Set(x*l, y*l, z*l ); }		//tolua_export
	inline float Length() const { return (float)sqrtf( x * x + y * y + z * z ); }								//tolua_export
	inline float SqrLength() const { return x * x + y * y + z * z; }											//tolua_export
	inline float Dot( const Vector3f & a_V ) const { return x * a_V.x + y * a_V.y + z * a_V.z; }				//tolua_export
	inline Vector3f Cross( const Vector3f & v ) const { return Vector3f( y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x ); }	//tolua_export

	inline bool Equals( const Vector3f & v ) const { return (x == v.x && y == v.y && z == v.z ); }										//tolua_export

	void operator += ( const Vector3f& a_V ) { x += a_V.x; y += a_V.y; z += a_V.z; }
	void operator += ( Vector3f* a_V ) { x += a_V->x; y += a_V->y; z += a_V->z; }
	void operator -= ( const Vector3f& a_V ) { x -= a_V.x; y -= a_V.y; z -= a_V.z; }
	void operator -= ( Vector3f* a_V ) { x -= a_V->x; y -= a_V->y; z -= a_V->z; }
	void operator *= ( float a_f ) { x *= a_f; y *= a_f; z *= a_f; }
	void operator *= ( Vector3f* a_V ) { x *= a_V->x; y *= a_V->y; z *= a_V->z; }
	void operator *= ( const Vector3f& a_V ) { x *= a_V.x; y *= a_V.y; z *= a_V.z; }

	Vector3f operator + ( const Vector3f& v2 ) const { return Vector3f( x + v2.x, y + v2.y, z + v2.z ); }									//tolua_export
	Vector3f operator + ( const Vector3f* v2 ) const { return Vector3f( x + v2->x, y + v2->y, z + v2->z ); }								//tolua_export
	Vector3f operator - ( const Vector3f& v2 ) const { return Vector3f( x - v2.x, y - v2.y, z - v2.z ); }									//tolua_export
	Vector3f operator - ( const Vector3f* v2 ) const { return Vector3f( x - v2->x, y - v2->y, z - v2->z ); }								//tolua_export
	Vector3f operator * ( const float f ) const { return Vector3f( x * f, y * f, z * f ); }												//tolua_export
	Vector3f operator * ( const Vector3f& v2 ) const { return Vector3f( x * v2.x, y * v2.y, z * v2.z ); }									//tolua_export

	float x, y, z;	//tolua_export

};//tolua_export