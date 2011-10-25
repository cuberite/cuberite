#pragma once

#include <emmintrin.h>

class cNoise
{
public:
	cNoise( unsigned int a_Seed );
	~cNoise();

	float IntNoise( int a_X ) const;
	float IntNoise2D( int a_X, int a_Y ) const;
	__m128 SSE_IntNoise2D( int a_X1, int a_Y1, int a_X2, int a_Y2, int a_X3, int a_Y3, int a_X4, int a_Y4 ) const;
	float IntNoise3D( int a_X, int a_Y, int a_Z ) const;

	float LinearNoise1D( float a_X ) const;
	float CosineNoise1D( float a_X ) const;
	float CubicNoise1D( float a_X ) const;
	float SmoothNoise1D( int a_X ) const;

	float LinearNoise2D( float a_X, float a_Y ) const;
	float CosineNoise2D( float a_X, float a_Y ) const;
	float CubicNoise2D( float a_X, float a_Y ) const;
	float SSE_CubicNoise2D( float a_X, float a_Y ) const;

	float CosineNoise3D( float a_X, float a_Y, float a_Z ) const;
	float CubicNoise3D( float a_X, float a_Y, float a_Z ) const;

	void SetSeed( unsigned int a_Seed ) { m_Seed = a_Seed; }
private:
	float CubicInterpolate( float a_A, float a_B, float a_C, float a_D, float a_Pct ) const;
	__m128 CubicInterpolate4( const __m128 & a_A, const __m128 & a_B, const __m128 & a_C, const __m128 & a_D, float a_Pct ) const;
	float CosineInterpolate( float a_A, float a_B, float a_Pct ) const;
	float LinearInterpolate( float a_A, float a_B, float a_Pct ) const;

	unsigned int m_Seed;
};

