#pragma once

#include "Vector2b.h"
#include "VectorTypes.h"

#ifdef USE_GCC_VECTORS
class Vector2i
{                                
public:

        Vector2i() {vec = (int2){0,0};}
        Vector2i(int a_x, int a_z) {vec = (int2){a_x,a_z};}

        inline void Set(int a_x, int a_z) { vec = (int2){a_x,a_z};}
        inline float Length() const { return sqrtf( (float)SqrLength()); }
        inline int SqrLength() const 
        {
        	int2 square = vec * vec;
        	return vec[0] + vec[1];
        }

        inline bool Equals( const Vector2i & v ) const
        {
		    int2 compare = vec == v.vec;
		    return ( compare[0] && compare[1]); 
        }
        inline bool Equals( const Vector2i * v ) const 
        {
		    int2 compare = vec == v->vec;
		    return ( compare[0] && compare[1]);
		}

        void operator += ( const Vector2i& a_V ) { vec += a_V.vec; }
        void operator += ( Vector2i* a_V ) { vec += a_V->vec; }
        void operator -= ( const Vector2i& a_V ) { vec -= a_V.vec;}
        void operator -= ( Vector2i* a_V ) { vec -= a_V->vec; }
        void operator *= ( const Vector2i& a_V ) { vec *= a_V.vec; }

        friend Vector2i operator + ( const Vector2i& v1, const Vector2i& v2 ) 
        { 
        return Vector2i(v1.vec + v2.vec); 
        }
        friend Vector2i operator + ( const Vector2i& v1, Vector2i* v2 ) { return Vector2i( v1.vec + v2->vec); }

        friend Vector2i operator - ( const Vector2i& v1, const Vector2i& v2 ) { return Vector2i( v1.vec - v2.vec ); }
        friend Vector2i operator - ( const Vector2i& v1, Vector2i* v2 ) { return Vector2i( v1.vec - v2->vec); }
        friend Vector2i operator - ( const Vector2i* v1, Vector2i& v2 ) { return Vector2i( v1->vec - v2.vec ); }

        friend Vector2i operator * ( const Vector2i& v1, const Vector2i& v2 ) { return Vector2i( v1.vec * v2.vec ); }

        friend Vector2b operator < ( const Vector2i& v1, const Vector2i& v2 )
        { 
		    int2 comp = v1.vec < v2.vec;
		    return Vector2b(comp);
        }
        
        friend Vector2b operator > ( const Vector2i& v1, const Vector2i& v2 )
        { 
		    return v2 > v1;
        }
        
        
private:
	int2 vec;
    Vector2i(int2 a_vec) {vec = a_vec;}
};
#else
class Vector2i
{                                
public:

        Vector2i() : x(0), z(0){}
        Vector2i(int a_x, int a_z) : x(a_x), z(a_z) {}

        inline void Set(int a_x, int a_z) { x = a_x, z = a_z; }
        inline float Length() const { return sqrtf( (float)( x * x + z * z) ); }
        inline int SqrLength() const { return x * x + z * z; }

        inline bool Equals( const Vector2i & v ) const { return (x == v.x && z == v.z ); }
        inline bool Equals( const Vector2i * v ) const { return (x == v->x && z == v->z ); }

        void operator += ( const Vector2i& a_V ) { x += a_V.x; z += a_V.z; }
        void operator += ( Vector2i* a_V ) { x += a_V->x; z += a_V->z; }
        void operator -= ( const Vector2i& a_V ) { x -= a_V.x; z -= a_V.z; }
        void operator -= ( Vector2i* a_V ) { x -= a_V->x; z -= a_V->z; }
		void operator *= ( const Vector2i& a_V ) { x *= a_V.x; z *= a_V.z;}

        friend Vector2i operator + ( const Vector2i& v1, const Vector2i& v2 ) { return Vector2i( v1.x + v2.x, v1.z + v2.z ); }
        friend Vector2i operator + ( const Vector2i& v1, Vector2i* v2 ) { return Vector2i( v1.x + v2->x,  v1.z + v2->z ); }

        friend Vector2i operator - ( const Vector2i& v1, const Vector2i& v2 ) { return Vector2i( v1.x - v2.x,  v1.z - v2.z ); }
        friend Vector2i operator - ( const Vector2i& v1, Vector2i* v2 ) { return Vector2i( v1.x - v2->x, v1.z - v2->z ); }
        friend Vector2i operator - ( const Vector2i* v1, Vector2i& v2 ) { return Vector2i( v1->x - v2.x, v1->z - v2.z ); }

        friend Vector2i operator * ( const Vector2i& v1, const Vector2i& v2 ) { return Vector2i( v1.x * v2.x, v1.z * v2.z ); }

        friend Vector2b operator < ( const Vector2i& v1, const Vector2i& v2 )
        { 
		    bool x = v1.x<v2.x;
		    bool z = v1.z<v2.z;
		    return Vector2b(x,z);
        }
        
        friend Vector2b operator > ( const Vector2i& v1, const Vector2i& v2 )
        { 
		    return v2 > v1;
        }
private:
        int x, z;
};
#endif
