#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include "Vector3f.h"

class Matrix4f
{
public:
	enum
	{
		TX=3,
		TY=7,
		TZ=11,
		D0=0, D1=5, D2=10, D3=15,
		SX=D0, SY=D1, SZ=D2,
		W=D3
	};
	Matrix4f() { Identity(); }
	float& operator [] ( int a_N ) { return cell[a_N]; }
	void Identity()
	{
		cell[1] = cell[2] = cell[TX] = cell[4] = cell[6] = cell[TY] =
		cell[8] = cell[9] = cell[TZ] = cell[12] = cell[13] = cell[14] = 0;
		cell[D0] = cell[D1] = cell[D2] = cell[W] = 1;
	}
	void Init( Vector3f a_Pos, float a_RX, float a_RY, float a_RZ )
	{
		Matrix4f t;
		t.RotateX( a_RZ );
		RotateY( a_RY );
		Concatenate( t );
		t.RotateZ( a_RX );
		Concatenate( t );
		Translate( a_Pos );
	}
	void RotateX( float a_RX )
	{
		float sx = (float)sin( a_RX * M_PI / 180 );
		float cx = (float)cos( a_RX * M_PI / 180 );
		Identity();
		cell[5] = cx, cell[6] = sx, cell[9] = -sx, cell[10] = cx;
	}
	void RotateY( float a_RY )
	{
		float sy = (float)sin( a_RY * M_PI / 180 );
		float cy = (float)cos( a_RY * M_PI / 180 );
		Identity ();
		cell[0] = cy, cell[2] = -sy, cell[8] = sy, cell[10] = cy;
	}
	void RotateZ( float a_RZ )
	{
		float sz = (float)sin( a_RZ * M_PI / 180 );
		float cz = (float)cos( a_RZ * M_PI / 180 );
		Identity ();
		cell[0] = cz, cell[1] = sz, cell[4] = -sz, cell[5] = cz;
	}
	void Translate( Vector3f a_Pos ) { cell[TX] += a_Pos.x; cell[TY] += a_Pos.y; cell[TZ] += a_Pos.z; }
	void SetTranslation( Vector3f a_Pos ) { cell[TX] = a_Pos.x; cell[TY] = a_Pos.y; cell[TZ] = a_Pos.z; }
	void Concatenate( const Matrix4f& m2 )
	{
		Matrix4f res;
		int c;
		for ( c = 0; c < 4; c++ ) for ( int r = 0; r < 4; r++ )
			res.cell[r * 4 + c] = cell[r * 4] * m2.cell[c] +
				  				  cell[r * 4 + 1] * m2.cell[c + 4] +
								  cell[r * 4 + 2] * m2.cell[c + 8] +
								  cell[r * 4 + 3] * m2.cell[c + 12];
		for ( c = 0; c < 16; c++ ) cell[c] = res.cell[c];
	}
	Vector3f Transform( const Vector3f& v ) const
	{
		float x  = cell[0] * v.x + cell[1] * v.y + cell[2] * v.z + cell[3];
		float y  = cell[4] * v.x + cell[5] * v.y + cell[6] * v.z + cell[7];
		float z  = cell[8] * v.x + cell[9] * v.y + cell[10] * v.z + cell[11];
		return Vector3f( x, y, z );
	}
	void Invert()
	{
		Matrix4f t;
		int h, i;
		float tx = -cell[3], ty = -cell[7], tz = -cell[11];
		for ( h = 0; h < 3; h++ ) for ( int v = 0; v < 3; v++ ) t.cell[h + v * 4] = cell[v + h * 4];
		for ( i = 0; i < 11; i++ ) cell[i] = t.cell[i];
		cell[3] = tx * cell[0] + ty * cell[1] + tz * cell[2];
		cell[7] = tx * cell[4] + ty * cell[5] + tz * cell[6];
		cell[11] = tx * cell[8] + ty * cell[9] + tz * cell[10];
	}
	Vector3f GetXColumn() { return Vector3f( cell[0], cell[1], cell[2] ); }
	Vector3f GetYColumn() { return Vector3f( cell[4], cell[5], cell[6] ); }
	Vector3f GetZColumn() { return Vector3f( cell[8], cell[9], cell[10] ); }
	void SetXColumn( const Vector3f & a_X )
	{
		cell[0] = a_X.x;
		cell[1] = a_X.y;
		cell[2] = a_X.z;
	}
	void SetYColumn( const Vector3f & a_Y )
	{
		cell[4] = a_Y.x;
		cell[5] = a_Y.y;
		cell[6] = a_Y.z;
	}
	void SetZColumn( const Vector3f & a_Z )
	{
		cell[8] = a_Z.x;
		cell[9] = a_Z.y;
		cell[10] = a_Z.z;
	}
	float cell[16];
};





class Matrix4d
{
public:
	enum
	{
		TX=3,
		TY=7,
		TZ=11,
		D0=0, D1=5, D2=10, D3=15,
		SX=D0, SY=D1, SZ=D2,
		W=D3
	};
	Matrix4d() { Identity(); }
	double& operator [] ( int a_N ) { return cell[a_N]; }
	void Identity()
	{
		cell[1] = cell[2] = cell[TX] = cell[4] = cell[6] = cell[TY] =
		cell[8] = cell[9] = cell[TZ] = cell[12] = cell[13] = cell[14] = 0;
		cell[D0] = cell[D1] = cell[D2] = cell[W] = 1;
	}
	void Init( Vector3f a_Pos, double a_RX, double a_RY, double a_RZ )
	{
		Matrix4d t;
		t.RotateX( a_RZ );
		RotateY( a_RY );
		Concatenate( t );
		t.RotateZ( a_RX );
		Concatenate( t );
		Translate( a_Pos );
	}
	void RotateX( double a_RX )
	{
		double sx = (double)sin( a_RX * M_PI / 180 );
		double cx = (double)cos( a_RX * M_PI / 180 );
		Identity();
		cell[5] = cx, cell[6] = sx, cell[9] = -sx, cell[10] = cx;
	}
	void RotateY( double a_RY )
	{
		double sy = (double)sin( a_RY * M_PI / 180 );
		double cy = (double)cos( a_RY * M_PI / 180 );
		Identity ();
		cell[0] = cy, cell[2] = -sy, cell[8] = sy, cell[10] = cy;
	}
	void RotateZ( double a_RZ )
	{
		double sz = (double)sin( a_RZ * M_PI / 180 );
		double cz = (double)cos( a_RZ * M_PI / 180 );
		Identity ();
		cell[0] = cz, cell[1] = sz, cell[4] = -sz, cell[5] = cz;
	}
	void Translate( Vector3d a_Pos ) { cell[TX] += a_Pos.x; cell[TY] += a_Pos.y; cell[TZ] += a_Pos.z; }
	void SetTranslation( Vector3d a_Pos ) { cell[TX] = a_Pos.x; cell[TY] = a_Pos.y; cell[TZ] = a_Pos.z; }
	void Concatenate( const Matrix4d & m2 )
	{
		Matrix4d res;
		int c;
		for ( c = 0; c < 4; c++ ) for ( int r = 0; r < 4; r++ )
			res.cell[r * 4 + c] = cell[r * 4] * m2.cell[c] +
				  				  cell[r * 4 + 1] * m2.cell[c + 4] +
								  cell[r * 4 + 2] * m2.cell[c + 8] +
								  cell[r * 4 + 3] * m2.cell[c + 12];
		for ( c = 0; c < 16; c++ ) cell[c] = res.cell[c];
	}
	Vector3d Transform( const Vector3d & v ) const
	{
		double x  = cell[0] * v.x + cell[1] * v.y + cell[2] * v.z + cell[3];
		double y  = cell[4] * v.x + cell[5] * v.y + cell[6] * v.z + cell[7];
		double z  = cell[8] * v.x + cell[9] * v.y + cell[10] * v.z + cell[11];
		return Vector3d( x, y, z );
	}
	void Invert()
	{
		Matrix4d t;
		int h, i;
		double tx = -cell[3], ty = -cell[7], tz = -cell[11];
		for ( h = 0; h < 3; h++ ) for ( int v = 0; v < 3; v++ ) t.cell[h + v * 4] = cell[v + h * 4];
		for ( i = 0; i < 11; i++ ) cell[i] = t.cell[i];
		cell[3] = tx * cell[0] + ty * cell[1] + tz * cell[2];
		cell[7] = tx * cell[4] + ty * cell[5] + tz * cell[6];
		cell[11] = tx * cell[8] + ty * cell[9] + tz * cell[10];
	}
	Vector3d GetXColumn() { return Vector3d( cell[0], cell[1], cell[2] ); }
	Vector3d GetYColumn() { return Vector3d( cell[4], cell[5], cell[6] ); }
	Vector3d GetZColumn() { return Vector3d( cell[8], cell[9], cell[10] ); }
	void SetXColumn( const Vector3d & a_X )
	{
		cell[0] = a_X.x;
		cell[1] = a_X.y;
		cell[2] = a_X.z;
	}
	void SetYColumn( const Vector3d & a_Y )
	{
		cell[4] = a_Y.x;
		cell[5] = a_Y.y;
		cell[6] = a_Y.z;
	}
	void SetZColumn( const Vector3d & a_Z )
	{
		cell[8] = a_Z.x;
		cell[9] = a_Z.y;
		cell[10] = a_Z.z;
	}
	double cell[16];
} ;




