#pragma once

#include "VectorTypes.h"

#ifdef USE_GCC_VECTORS 
class Vector2b
{
friend class Vector2i;                                
public:
	Vector2b(bool a_x, bool a_z) 
	{
		int2 vec = (int2){(int)a_x,(int)a_z};
		//convert from 1 is true to -1 is true
		vec = (vec == (int2){1,1});
	}
	
	bool ReduceOr() {return vec[0] | vec[1];}
	
	friend Vector2b operator || ( const Vector2b& v1, const Vector2b& v2 ) { return Vector2b( v1.vec | v2.vec ); }
	
	friend Vector2b operator && ( const Vector2b& v1, const Vector2b& v2 ) { return Vector2b( v1.vec & v2.vec ); }
private:
	int2 vec;
	
	Vector2b(int2 a_vec) : vec(a_vec) {}
};
#else
class Vector2b
{
friend class Vector2i;                                
public:
	Vector2b(bool a_x, bool a_z) : x(a_x),z(a_z) {}
	
	bool ReduceOr() {return x || z;}
	
	friend Vector2b operator || ( const Vector2b& v1, const Vector2b& v2 ) { return Vector2b( v1.x || v2.x, v1.z || v2.z ); }
	
	friend Vector2b operator && ( const Vector2b& v1, const Vector2b& v2 ) { return Vector2b( v1.x && v2.x, v1.z && v2.z ); }
private:
	bool x,z;
};
#endif
