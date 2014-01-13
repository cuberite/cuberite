#pragma once

class Vector2b
{                                
public:
	Vector2b(bool a_x, bool a_z) : x(a_x),z(a_z) {}
	
	bool ReduceOr() {return x || z;}
	
	friend Vector2b operator || ( const Vector2b& v1, const Vector2b& v2 ) { return Vector2b( v1.x || v2.x, v1.z || v2.z ); }
	
	friend Vector2b operator && ( const Vector2b& v1, const Vector2b& v2 ) { return Vector2b( v1.x && v2.x, v1.z && v2.z ); }
	
	bool x,z;
};
