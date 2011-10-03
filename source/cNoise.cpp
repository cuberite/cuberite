#include "cNoise.h"
#include <math.h>

#define FAST_FLOOR( x ) ( (x) < 0 ? ((int)x)-1 : ((int)x) )

cNoise::cNoise( unsigned int a_Seed )
	: m_Seed( a_Seed )
{
}


cNoise::~cNoise()
{
}

/****************
 * Random value generator
 **/
float cNoise::IntNoise( int a_X ) const
{
	int x = ((a_X*m_Seed)<<13) ^ a_X;
    return ( 1.0f - ( (x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f); 
}

float cNoise::IntNoise2D( int a_X, int a_Y ) const
{
	int n = a_X + a_Y * 57 + m_Seed*57*57;
    n = (n<<13) ^ n;
    return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);  
}

float cNoise::IntNoise3D( int a_X, int a_Y, int a_Z ) const
{
	int n = a_X + a_Y * 57 + a_Z * 57*57 + m_Seed*57*57*57;
    n = (n<<13) ^ n;
    return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);  
}

/***************
 * Interpolated (and 1 smoothed) noise in 1-dimension
 **/
float cNoise::LinearNoise1D( float a_X ) const
{
	int BaseX = FAST_FLOOR( a_X );
	float FracX = (a_X) - BaseX;
	return LinearInterpolate( IntNoise( BaseX ), IntNoise( BaseX+1 ), FracX);
}

float cNoise::CosineNoise1D( float a_X ) const
{
	int BaseX = FAST_FLOOR( a_X );
	float FracX = (a_X) - BaseX;
	return CosineInterpolate( IntNoise( BaseX ), IntNoise( BaseX+1 ), FracX);
}

float cNoise::CubicNoise1D( float a_X ) const
{
	int BaseX = FAST_FLOOR( a_X );
	float FracX = (a_X) - BaseX;
	return CubicInterpolate( IntNoise( BaseX-1 ), IntNoise( BaseX ), IntNoise( BaseX+1 ), IntNoise( BaseX+2 ), FracX);
}

float cNoise::SmoothNoise1D( int a_X ) const
{
	return IntNoise(a_X)/2  +  IntNoise(a_X-1)/4  +  IntNoise(a_X+1)/4;
}

/******************
 * Interpolated (and 1 smoothed) noise in 2-dimensions
 **/
float cNoise::LinearNoise2D( float a_X, float a_Y ) const
{
	const int	BaseX = FAST_FLOOR( a_X );
	const int	BaseY = FAST_FLOOR( a_Y );

	const float	tl = IntNoise2D( BaseX,		BaseY );
	const float	tr = IntNoise2D( BaseX+1,	BaseY );
	const float	bl = IntNoise2D( BaseX,		BaseY+1 );
	const float	br = IntNoise2D( BaseX+1,		BaseY+1 );

	const float FracX = (a_X) - BaseX;
	const float interp1 = LinearInterpolate( tl, tr, FracX );
	const float interp2 = LinearInterpolate( bl, br, FracX );

	const float	FracY = (a_Y) - BaseY;
	return LinearInterpolate( interp1, interp2, FracY );
}

float cNoise::CosineNoise2D( float a_X, float a_Y ) const
{
	const int	BaseX = FAST_FLOOR( a_X );
	const int	BaseY = FAST_FLOOR( a_Y );

	const float	tl = IntNoise2D( BaseX,		BaseY );
	const float	tr = IntNoise2D( BaseX+1,	BaseY );
	const float	bl = IntNoise2D( BaseX,		BaseY+1 );
	const float	br = IntNoise2D( BaseX+1,		BaseY+1 );

	const float FracX = (a_X) - BaseX;
	const float interp1 = CosineInterpolate( tl, tr, FracX );
	const float interp2 = CosineInterpolate( bl, br, FracX );

	const float	FracY = (a_Y) - BaseY;
	return CosineInterpolate( interp1, interp2, FracY );
}

float cNoise::CubicNoise2D( float a_X, float a_Y ) const
{
	const int	BaseX = FAST_FLOOR( a_X );
	const int	BaseY = FAST_FLOOR( a_Y );
	
	const float points[4][4] = { 
		
		IntNoise2D( BaseX-1, BaseY-1 ), IntNoise2D( BaseX, BaseY-1 ),	IntNoise2D( BaseX+1, BaseY-1 ), IntNoise2D( BaseX+2, BaseY-1 ),
		IntNoise2D( BaseX-1, BaseY ),	IntNoise2D( BaseX, BaseY ),		IntNoise2D( BaseX+1, BaseY ),	IntNoise2D( BaseX+2, BaseY ),
		IntNoise2D( BaseX-1, BaseY+1 ), IntNoise2D( BaseX, BaseY+1 ),	IntNoise2D( BaseX+1, BaseY+1 ), IntNoise2D( BaseX+2, BaseY+1 ),
		IntNoise2D( BaseX-1, BaseY+2 ), IntNoise2D( BaseX, BaseY+2 ),	IntNoise2D( BaseX+1, BaseY+2 ), IntNoise2D( BaseX+2, BaseY+2 ),

	};

	const float	FracX = (a_X) - BaseX;
	const float interp1 = CubicInterpolate( points[0][0], points[0][1], points[0][2], points[0][3], FracX );
	const float interp2 = CubicInterpolate( points[1][0], points[1][1], points[1][2], points[1][3], FracX );
	const float interp3 = CubicInterpolate( points[2][0], points[2][1], points[2][2], points[2][3], FracX );
	const float interp4 = CubicInterpolate( points[3][0], points[3][1], points[3][2], points[3][3], FracX );


	const float	FracY = (a_Y) - BaseY;
	return CubicInterpolate( interp1, interp2, interp3, interp4, FracY );
}

/******************
 * Interpolated (and 1 smoothed) noise in 3-dimensions
 **/
float cNoise::CosineNoise3D( float a_X, float a_Y, float a_Z ) const
{
	const int	BaseX = FAST_FLOOR( a_X );
	const int	BaseY = FAST_FLOOR( a_Y );
	const int	BaseZ = FAST_FLOOR( a_Z );

	const float	ftl = IntNoise3D( BaseX,	BaseY,		BaseZ );
	const float	ftr = IntNoise3D( BaseX+1,	BaseY,		BaseZ  );
	const float	fbl = IntNoise3D( BaseX,	BaseY+1,	BaseZ  );
	const float	fbr = IntNoise3D( BaseX+1,	BaseY+1,	BaseZ  );

	const float	btl = IntNoise3D( BaseX,	BaseY,		BaseZ+1 );
	const float	btr = IntNoise3D( BaseX+1,	BaseY,		BaseZ+1 );
	const float	bbl = IntNoise3D( BaseX,	BaseY+1,	BaseZ+1 );
	const float	bbr = IntNoise3D( BaseX+1,	BaseY+1,	BaseZ+1 );

	const float FracX = (a_X) - BaseX;
	const float finterp1 = CosineInterpolate( ftl, ftr, FracX );
	const float finterp2 = CosineInterpolate( fbl, fbr, FracX );
	const float binterp1 = CosineInterpolate( btl, btr, FracX );
	const float binterp2 = CosineInterpolate( bbl, bbr, FracX );

	const float	FracY = (a_Y) - BaseY;
	const float interp1 = CosineInterpolate( finterp1, finterp2, FracY );
	const float interp2 = CosineInterpolate( binterp1, binterp2, FracY );

	const float	FracZ = (a_Z) - BaseZ;
	return CosineInterpolate( interp1, interp2, FracZ );
}

float cNoise::CubicNoise3D( float a_X, float a_Y, float a_Z ) const
{
	const int	BaseX = FAST_FLOOR( a_X );
	const int	BaseY = FAST_FLOOR( a_Y );
	const int	BaseZ = FAST_FLOOR( a_Z );
	
	const float points1[4][4] = { 
		IntNoise3D( BaseX-1, BaseY-1, BaseZ-1 ), IntNoise3D( BaseX, BaseY-1, BaseZ-1 ),	IntNoise3D( BaseX+1, BaseY-1, BaseZ-1 ), IntNoise3D( BaseX+2, BaseY-1, BaseZ-1 ),
		IntNoise3D( BaseX-1, BaseY,	  BaseZ-1 ), IntNoise3D( BaseX, BaseY,   BaseZ-1 ),	IntNoise3D( BaseX+1, BaseY,   BaseZ-1 ), IntNoise3D( BaseX+2, BaseY,   BaseZ-1 ),
		IntNoise3D( BaseX-1, BaseY+1, BaseZ-1 ), IntNoise3D( BaseX, BaseY+1, BaseZ-1 ),	IntNoise3D( BaseX+1, BaseY+1, BaseZ-1 ), IntNoise3D( BaseX+2, BaseY+1, BaseZ-1 ),
		IntNoise3D( BaseX-1, BaseY+2, BaseZ-1 ), IntNoise3D( BaseX, BaseY+2, BaseZ-1 ),	IntNoise3D( BaseX+1, BaseY+2, BaseZ-1 ), IntNoise3D( BaseX+2, BaseY+2, BaseZ-1 ),
	};

	const float	FracX = (a_X) - BaseX;
	const float x1interp1 = CubicInterpolate( points1[0][0], points1[0][1], points1[0][2], points1[0][3], FracX );
	const float x1interp2 = CubicInterpolate( points1[1][0], points1[1][1], points1[1][2], points1[1][3], FracX );
	const float x1interp3 = CubicInterpolate( points1[2][0], points1[2][1], points1[2][2], points1[2][3], FracX );
	const float x1interp4 = CubicInterpolate( points1[3][0], points1[3][1], points1[3][2], points1[3][3], FracX );

	const float points2[4][4] = { 
		IntNoise3D( BaseX-1, BaseY-1, BaseZ ), IntNoise3D( BaseX, BaseY-1, BaseZ ),	IntNoise3D( BaseX+1, BaseY-1, BaseZ ), IntNoise3D( BaseX+2, BaseY-1, BaseZ ),
		IntNoise3D( BaseX-1, BaseY,	  BaseZ ), IntNoise3D( BaseX, BaseY,   BaseZ ),	IntNoise3D( BaseX+1, BaseY,   BaseZ ), IntNoise3D( BaseX+2, BaseY,   BaseZ ),
		IntNoise3D( BaseX-1, BaseY+1, BaseZ ), IntNoise3D( BaseX, BaseY+1, BaseZ ),	IntNoise3D( BaseX+1, BaseY+1, BaseZ ), IntNoise3D( BaseX+2, BaseY+1, BaseZ ),
		IntNoise3D( BaseX-1, BaseY+2, BaseZ ), IntNoise3D( BaseX, BaseY+2, BaseZ ),	IntNoise3D( BaseX+1, BaseY+2, BaseZ ), IntNoise3D( BaseX+2, BaseY+2, BaseZ ),
	};

	const float x2interp1 = CubicInterpolate( points2[0][0], points2[0][1], points2[0][2], points2[0][3], FracX );
	const float x2interp2 = CubicInterpolate( points2[1][0], points2[1][1], points2[1][2], points2[1][3], FracX );
	const float x2interp3 = CubicInterpolate( points2[2][0], points2[2][1], points2[2][2], points2[2][3], FracX );
	const float x2interp4 = CubicInterpolate( points2[3][0], points2[3][1], points2[3][2], points2[3][3], FracX );

	const float points3[4][4] = { 
		IntNoise3D( BaseX-1, BaseY-1, BaseZ+1 ), IntNoise3D( BaseX, BaseY-1, BaseZ+1 ),	IntNoise3D( BaseX+1, BaseY-1, BaseZ+1 ), IntNoise3D( BaseX+2, BaseY-1, BaseZ+1 ),
		IntNoise3D( BaseX-1, BaseY,	  BaseZ+1 ), IntNoise3D( BaseX, BaseY,   BaseZ+1 ),	IntNoise3D( BaseX+1, BaseY,   BaseZ+1 ), IntNoise3D( BaseX+2, BaseY,   BaseZ+1 ),
		IntNoise3D( BaseX-1, BaseY+1, BaseZ+1 ), IntNoise3D( BaseX, BaseY+1, BaseZ+1 ),	IntNoise3D( BaseX+1, BaseY+1, BaseZ+1 ), IntNoise3D( BaseX+2, BaseY+1, BaseZ+1 ),
		IntNoise3D( BaseX-1, BaseY+2, BaseZ+1 ), IntNoise3D( BaseX, BaseY+2, BaseZ+1 ),	IntNoise3D( BaseX+1, BaseY+2, BaseZ+1 ), IntNoise3D( BaseX+2, BaseY+2, BaseZ+1 ),
	};

	const float x3interp1 = CubicInterpolate( points3[0][0], points3[0][1], points3[0][2], points3[0][3], FracX );
	const float x3interp2 = CubicInterpolate( points3[1][0], points3[1][1], points3[1][2], points3[1][3], FracX );
	const float x3interp3 = CubicInterpolate( points3[2][0], points3[2][1], points3[2][2], points3[2][3], FracX );
	const float x3interp4 = CubicInterpolate( points3[3][0], points3[3][1], points3[3][2], points3[3][3], FracX );

	const float points4[4][4] = { 
		IntNoise3D( BaseX-1, BaseY-1, BaseZ+2 ), IntNoise3D( BaseX, BaseY-1, BaseZ+2 ),	IntNoise3D( BaseX+1, BaseY-1, BaseZ+2 ), IntNoise3D( BaseX+2, BaseY-1, BaseZ+2 ),
		IntNoise3D( BaseX-1, BaseY,	  BaseZ+2 ), IntNoise3D( BaseX, BaseY,   BaseZ+2 ),	IntNoise3D( BaseX+1, BaseY,   BaseZ+2 ), IntNoise3D( BaseX+2, BaseY,   BaseZ+2 ),
		IntNoise3D( BaseX-1, BaseY+1, BaseZ+2 ), IntNoise3D( BaseX, BaseY+1, BaseZ+2 ),	IntNoise3D( BaseX+1, BaseY+1, BaseZ+2 ), IntNoise3D( BaseX+2, BaseY+1, BaseZ+2 ),
		IntNoise3D( BaseX-1, BaseY+2, BaseZ+2 ), IntNoise3D( BaseX, BaseY+2, BaseZ+2 ),	IntNoise3D( BaseX+1, BaseY+2, BaseZ+2 ), IntNoise3D( BaseX+2, BaseY+2, BaseZ+2 ),
	};

	const float x4interp1 = CubicInterpolate( points4[0][0], points4[0][1], points4[0][2], points4[0][3], FracX );
	const float x4interp2 = CubicInterpolate( points4[1][0], points4[1][1], points4[1][2], points4[1][3], FracX );
	const float x4interp3 = CubicInterpolate( points4[2][0], points4[2][1], points4[2][2], points4[2][3], FracX );
	const float x4interp4 = CubicInterpolate( points4[3][0], points4[3][1], points4[3][2], points4[3][3], FracX );

	const float	FracY = (a_Y) - BaseY;
	const float yinterp1 = CubicInterpolate( x1interp1, x1interp2, x1interp3, x1interp4, FracY );
	const float yinterp2 = CubicInterpolate( x2interp1, x2interp2, x2interp3, x2interp4, FracY );
	const float yinterp3 = CubicInterpolate( x3interp1, x3interp2, x3interp3, x3interp4, FracY );
	const float yinterp4 = CubicInterpolate( x4interp1, x4interp2, x4interp3, x4interp4, FracY );

	const float	FracZ = (a_Z) - BaseZ;
	return CubicInterpolate( yinterp1, yinterp2, yinterp3, yinterp4, FracZ );
}

/******************
 * Private
 **/
float cNoise::CubicInterpolate( float a_A, float a_B, float a_C, float a_D, float a_Pct ) const
{
	float P = (a_D - a_C) - (a_A - a_B);
	float Q = (a_A - a_B) - P;
	float R = a_C - a_A;
	float S = a_B;

	return P*(a_Pct*a_Pct*a_Pct) + Q*(a_Pct*a_Pct) + R*a_Pct + S;
}

float cNoise::CosineInterpolate( float a_A, float a_B, float a_Pct ) const
{
	const float ft = a_Pct * 3.1415927f;
	const float f = (1.f - cosf(ft)) * 0.5f;
	return  a_A*(1-f) + a_B*f;
}

float cNoise::LinearInterpolate( float a_A, float a_B, float a_Pct ) const
{
	return  a_A*(1.f-a_Pct) + a_B*a_Pct;
}