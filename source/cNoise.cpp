
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "cNoise.h"
#include <math.h>

#if NOISE_USE_SSE
#include <smmintrin.h> //_mm_mul_epi32
#endif

#define FAST_FLOOR( x ) ( (x) < 0 ? ((int)x)-1 : ((int)x) )





cNoise::cNoise( unsigned int a_Seed )
	: m_Seed( a_Seed )
{
}


cNoise::~cNoise()
{
}

#if NOISE_USE_SSE
/****************
 * SSE Random value generator
 **/
__m128 cNoise::SSE_IntNoise2D( int a_X1, int a_Y1, int a_X2, int a_Y2, int a_X3, int a_Y3, int a_X4, int a_Y4 ) const
{
	const __m128i X4 = _mm_set_epi32(a_X4, a_X3, a_X2, a_X1);
	const __m128i Y4 = _mm_set_epi32(a_Y4, a_Y3, a_Y2, a_Y1);

	const __m128 One4 = _mm_set_ps1( 1.f );
	const __m128i YScale4 = _mm_set1_epi32( 57 );

	const __m128i i15731 = _mm_set1_epi32( 15731 );
	const __m128i i789221 = _mm_set1_epi32( 789221 );
	const __m128i i1376312589 = _mm_set1_epi32(1376312589);
	const __m128i MaskValue4 = _mm_set1_epi32(0x7fffffff);
	const __m128 f1073741824 = _mm_set_ps1( 1073741824.0f );

	const __m128i Seed4 = _mm_mullo_epi32( _mm_mullo_epi32( _mm_set1_epi32( m_Seed ), YScale4 ), YScale4 );

	const __m128i ScaledY4 = _mm_mullo_epi32( Y4, YScale4 );
	const __m128i n4 = _mm_add_epi32( _mm_add_epi32( X4, ScaledY4 ), Seed4 );
	const __m128i nn4 = _mm_slli_epi32( n4, 13 );
	const __m128i nnn4 = _mm_xor_si128( nn4, n4 );

	const __m128i StepA4 = _mm_mullo_epi32( nnn4, nnn4 );
	const __m128i StepAA4 = _mm_add_epi32( _mm_mullo_epi32( StepA4, i15731 ), i789221 );
	const __m128i StepB4 = _mm_add_epi32( _mm_mullo_epi32( nnn4, StepAA4 ), i1376312589 );
	const __m128i StepC4 = _mm_and_si128( StepB4, MaskValue4 );
	const __m128 StepD4 = _mm_div_ps( _mm_cvtepi32_ps( StepC4 ), f1073741824 );
	const __m128 Result4 = _mm_sub_ps( One4, StepD4 );

	return Result4;
}
#endif


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
	
	const float points[4][4] =
	{
		IntNoise2D( BaseX-1, BaseY-1 ), IntNoise2D( BaseX, BaseY-1 ), IntNoise2D( BaseX+1, BaseY-1 ), IntNoise2D( BaseX+2, BaseY-1 ),
		IntNoise2D( BaseX-1, BaseY ),   IntNoise2D( BaseX, BaseY ),   IntNoise2D( BaseX+1, BaseY ),   IntNoise2D( BaseX+2, BaseY ),
		IntNoise2D( BaseX-1, BaseY+1 ), IntNoise2D( BaseX, BaseY+1 ), IntNoise2D( BaseX+1, BaseY+1 ), IntNoise2D( BaseX+2, BaseY+1 ),
		IntNoise2D( BaseX-1, BaseY+2 ), IntNoise2D( BaseX, BaseY+2 ), IntNoise2D( BaseX+1, BaseY+2 ), IntNoise2D( BaseX+2, BaseY+2 ),
	};

	const float	FracX = (a_X) - BaseX;
	const float interp1 = CubicInterpolate( points[0][0], points[0][1], points[0][2], points[0][3], FracX );
	const float interp2 = CubicInterpolate( points[1][0], points[1][1], points[1][2], points[1][3], FracX );
	const float interp3 = CubicInterpolate( points[2][0], points[2][1], points[2][2], points[2][3], FracX );
	const float interp4 = CubicInterpolate( points[3][0], points[3][1], points[3][2], points[3][3], FracX );


	const float	FracY = (a_Y) - BaseY;
	return CubicInterpolate( interp1, interp2, interp3, interp4, FracY );
}





#if NOISE_USE_SSE
float cNoise::SSE_CubicNoise2D( float a_X, float a_Y ) const
{
	const int	BaseX = FAST_FLOOR( a_X );
	const int	BaseY = FAST_FLOOR( a_Y );

	__m128 points4[4] = {
		SSE_IntNoise2D( BaseX-1, BaseY-1,	BaseX-1, BaseY, BaseX-1, BaseY+1,	BaseX-1, BaseY+2 ),
		SSE_IntNoise2D( BaseX,	 BaseY-1,	BaseX, BaseY,	BaseX, BaseY+1,		BaseX, BaseY+2 ),
		SSE_IntNoise2D( BaseX+1, BaseY-1,	BaseX+1, BaseY, BaseX+1, BaseY+1,	BaseX+1, BaseY+2 ),
		SSE_IntNoise2D( BaseX+2, BaseY-1,	BaseX+2, BaseY,	BaseX+2, BaseY+1,	BaseX+2, BaseY+2 ),
	};

	const float	FracX = (a_X) - BaseX;
	union { __m128 p4; float p[4]; } 
	AllInterp = { CubicInterpolate4( points4[0], points4[1], points4[2], points4[3], FracX ) };

	const float	FracY = (a_Y) - BaseY;
	return CubicInterpolate( AllInterp.p[0], AllInterp.p[1], AllInterp.p[2], AllInterp.p[3], FracY );
}
#endif

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

#if NOISE_USE_SSE
__m128 cNoise::CubicInterpolate4( const __m128 & a_A, const __m128 & a_B, const __m128 & a_C, const __m128 & a_D, float a_Pct ) const
{
	const __m128 P = _mm_sub_ps( _mm_sub_ps( a_D, a_C ), _mm_sub_ps( a_A, a_B ) );
	const __m128 Q = _mm_sub_ps( _mm_sub_ps( a_A, a_B ), P );
	const __m128 R = _mm_sub_ps( a_C, a_A );

	const __m128 Pct = _mm_set_ps1( a_Pct );
	const __m128 Pct2 = _mm_mul_ps( Pct, Pct );
	const __m128 Pct3 = _mm_mul_ps( Pct2, Pct );

	return _mm_add_ps( _mm_add_ps( _mm_add_ps( _mm_mul_ps(P, Pct3), _mm_mul_ps( Q, Pct2 ) ), _mm_mul_ps( R, Pct ) ), a_B );
}
#endif

#if NOISE_USE_INLINE
# include "cNoise.inc"
#endif