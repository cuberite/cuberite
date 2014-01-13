#pragma once

class Vector2i
{                                
public:            
        Vector2i( const Vector2d & v ) : x( (int)v.x ), z( (int)v.z ){}

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
        void operator *= ( int a_f ) { x *= a_f; z *= a_f; }

        friend Vector2i operator + ( const Vector2i& v1, const Vector2i& v2 ) { return Vector2i( v1.x + v2.x, v1.z + v2.z ); }
        friend Vector2i operator + ( const Vector2i& v1, Vector2i* v2 ) { return Vector2i( v1.x + v2->x,  v1.z + v2->z ); }
        friend Vector2i operator - ( const Vector2i& v1, const Vector2i& v2 ) { return Vector2i( v1.x - v2.x,  v1.z - v2.z ); }
        friend Vector2i operator - ( const Vector2i& v1, Vector2i* v2 ) { return Vector2i( v1.x - v2->x, v1.z - v2->z ); }
        friend Vector2i operator - ( const Vector2i* v1, Vector2i& v2 ) { return Vector2i( v1->x - v2.x, v1->z - v2.z ); }
        friend Vector2i operator * ( const Vector2i& v, const int f ) { return Vector2i( v.x * f, v.z * f ); }
        friend Vector2i operator * ( const Vector2i& v1, const Vector2i& v2 ) { return Vector2i( v1.x * v2.x, v1.z * v2.z ); }
        friend Vector2i operator * ( const int f, const Vector2i& v ) { return Vector2i( v.x * f, v.z * f ); }
        friend bool operator < ( const Vector2i& v1, const Vector2i& v2 ) { return (v1.x<v2.x)||(v1.x==v2.x && v1.z<v2.z); }

        int x, z;
};
