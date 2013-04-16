
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Noise.h"





#if NOISE_USE_SSE
	#include <smmintrin.h> //_mm_mul_epi32
#endif

#define FAST_FLOOR(x) (((x) < 0) ? (((int)x) - 1) : ((int)x))





NOISE_DATATYPE CubicInterpolate(NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_C, NOISE_DATATYPE a_D, NOISE_DATATYPE a_Pct)
{
	NOISE_DATATYPE P = (a_D - a_C) - (a_A - a_B);
	NOISE_DATATYPE Q = (a_A - a_B) - P;
	NOISE_DATATYPE R = a_C - a_A;
	NOISE_DATATYPE S = a_B;

	return ((P * a_Pct + Q) * a_Pct + R) * a_Pct + S;
}





class cCubicCell2D
{
public:
	cCubicCell2D(
		cNoise & a_Noise,          ///< Noise to use for generating the random values
		NOISE_DATATYPE * a_Array,  ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,  ///< Count of the array, in each direction
		const NOISE_DATATYPE * a_FracX,  ///< Pointer to the array that stores the X fractional values
		const NOISE_DATATYPE * a_FracY   ///< Pointer to the attay that stores the Y fractional values
	);
	
	/// Uses current m_WorkRnds[] to generate part of the array
	void Generate(
		int a_FromX, int a_ToX,
		int a_FromY, int a_ToY
	);
	
	/// Initializes m_WorkRnds[] with the specified Floor values
	void InitWorkRnds(int a_FloorX, int a_FloorY);
	
	/// Updates m_WorkRnds[] for the new Floor values.
	void Move(int a_NewFloorX, int a_NewFloorY);

protected:
	typedef NOISE_DATATYPE Workspace[4][4];
	
	cNoise & m_Noise;
	
	Workspace * m_WorkRnds;  ///< The current random values; points to either m_Workspace1 or m_Workspace2 (doublebuffering)
	Workspace m_Workspace1;  ///< Buffer 1 for workspace doublebuffering, used in Move()
	Workspace m_Workspace2;  ///< Buffer 2 for workspace doublebuffering, used in Move()
	int m_CurFloorX;
	int m_CurFloorY;
	
	NOISE_DATATYPE * m_Array;
	int m_SizeX, m_SizeY;
	const NOISE_DATATYPE * m_FracX;
	const NOISE_DATATYPE * m_FracY;
} ;





cCubicCell2D::cCubicCell2D(
	cNoise & a_Noise,          ///< Noise to use for generating the random values
	NOISE_DATATYPE * a_Array,  ///< Array to generate into [x + a_SizeX * y]
	int a_SizeX, int a_SizeY,  ///< Count of the array, in each direction
	const NOISE_DATATYPE * a_FracX,  ///< Pointer to the array that stores the X fractional values
	const NOISE_DATATYPE * a_FracY   ///< Pointer to the attay that stores the Y fractional values
) :
	m_Noise(a_Noise),
	m_WorkRnds(&m_Workspace1),
	m_Array(a_Array),
	m_SizeX(a_SizeX),
	m_SizeY(a_SizeY),
	m_FracX(a_FracX),
	m_FracY(a_FracY)
{
}





void cCubicCell2D::Generate(
	int a_FromX, int a_ToX,
	int a_FromY, int a_ToY
)
{
	for (int y = a_FromY; y < a_ToY; y++)
	{
		NOISE_DATATYPE Interp[4];
		NOISE_DATATYPE FracY = m_FracY[y];
		Interp[0] = CubicInterpolate((*m_WorkRnds)[0][0], (*m_WorkRnds)[0][1], (*m_WorkRnds)[0][2], (*m_WorkRnds)[0][3], FracY);
		Interp[1] = CubicInterpolate((*m_WorkRnds)[1][0], (*m_WorkRnds)[1][1], (*m_WorkRnds)[1][2], (*m_WorkRnds)[1][3], FracY);
		Interp[2] = CubicInterpolate((*m_WorkRnds)[2][0], (*m_WorkRnds)[2][1], (*m_WorkRnds)[2][2], (*m_WorkRnds)[2][3], FracY);
		Interp[3] = CubicInterpolate((*m_WorkRnds)[3][0], (*m_WorkRnds)[3][1], (*m_WorkRnds)[3][2], (*m_WorkRnds)[3][3], FracY);
		int idx = y * m_SizeX + a_FromX;
		for (int x = a_FromX; x < a_ToX; x++)
		{
			m_Array[idx++] = CubicInterpolate(Interp[0], Interp[1], Interp[2], Interp[3], m_FracX[x]);
		}  // for x
	}  // for y
}





void cCubicCell2D::InitWorkRnds(int a_FloorX, int a_FloorY)
{
	m_CurFloorX = a_FloorX;
	m_CurFloorY = a_FloorY;
	for (int x = 0; x < 4; x++)
	{
		int cx = a_FloorX + x - 1;
		for (int y = 0; y < 4; y++)
		{
			int cy = a_FloorY + y - 1;
			(*m_WorkRnds)[x][y] = (NOISE_DATATYPE)m_Noise.IntNoise2D(cx, cy);
		}
	}
}





void cCubicCell2D::Move(int a_NewFloorX, int a_NewFloorY)
{
	// Swap the doublebuffer:
	int OldFloorX = m_CurFloorX;
	int OldFloorY = m_CurFloorY;
	Workspace * OldWorkRnds = m_WorkRnds;
	m_WorkRnds = (m_WorkRnds == &m_Workspace1) ? &m_Workspace2 : &m_Workspace1;
	
	// Reuse as much of the old workspace as possible:
	int DiffX = OldFloorX - a_NewFloorX;
	int DiffY = OldFloorY - a_NewFloorY;
	for (int x = 0; x < 4; x++)
	{
		int cx = a_NewFloorX + x - 1;
		int OldX = x - DiffX;  // Where would this X be in the old grid?
		for (int y = 0; y < 4; y++)
		{
			int cy = a_NewFloorY + y - 1;
			int OldY = y - DiffY;  // Where would this Y be in the old grid?
			if ((OldX >= 0) && (OldX < 4) && (OldY >= 0) && (OldY < 4))
			{
				(*m_WorkRnds)[x][y] = (*OldWorkRnds)[OldX][OldY];
			}
			else
			{
				(*m_WorkRnds)[x][y] = (NOISE_DATATYPE)m_Noise.IntNoise2D(cx, cy);
			}
		}
	}
	m_CurFloorX = a_NewFloorX;
	m_CurFloorY = a_NewFloorY;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cNoise:

cNoise::cNoise(unsigned int a_Seed) :
	m_Seed(a_Seed)
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





void IntArrayLinearInterpolate2D(
	int * a_Array, 
	int a_SizeX, int a_SizeY,  // Dimensions of the array
	int a_AnchorStepX, int a_AnchorStepY  // Distances between the anchor points in each direction
)
{
	// First interpolate columns where the anchor points are:
	int LastYCell = a_SizeY - a_AnchorStepY;
	for (int y = 0; y < LastYCell; y += a_AnchorStepY)
	{
		int Idx = a_SizeX * y;
		for (int x = 0; x < a_SizeX; x += a_AnchorStepX)
		{
			int StartValue = a_Array[Idx];
			int EndValue   = a_Array[Idx + a_SizeX * a_AnchorStepY];
			int Diff = EndValue - StartValue;
			for (int CellY = 1; CellY < a_AnchorStepY; CellY++)
			{
				a_Array[Idx + a_SizeX * CellY] = StartValue + CellY * Diff / a_AnchorStepY;
			}  // for CellY
			Idx += a_AnchorStepX;
		}  // for x
	}  // for y
	
	// Now interpolate in rows, each row has values in the anchor columns
	int LastXCell = a_SizeX - a_AnchorStepX;
	for (int y = 0; y < a_SizeY; y++)
	{
		int Idx = a_SizeX * y;
		for (int x = 0; x < LastXCell; x += a_AnchorStepX)
		{
			int StartValue = a_Array[Idx];
			int EndValue   = a_Array[Idx + a_AnchorStepX];
			int Diff = EndValue - StartValue;
			for (int CellX = 1; CellX < a_AnchorStepX; CellX++)
			{
				a_Array[Idx + CellX] = StartValue + CellX * Diff / a_AnchorStepX;
			}  // for CellY
			Idx += a_AnchorStepX;
		}
	}
}





#if NOISE_USE_INLINE
	#include "Noise.inc"
#endif





///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cCubicNoise:

cCubicNoise::cCubicNoise(int a_Seed) :
	m_Noise(a_Seed)
{
}





void cCubicNoise::Generate2D(
	NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
	int a_SizeX, int a_SizeY,                        ///< Size of the array (num doubles), in each direction
	NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
	NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
	NOISE_DATATYPE * a_Workspace                     ///< Workspace that this function can use and trash
)
{
	ASSERT(a_SizeX < MAX_SIZE);
	ASSERT(a_SizeY < MAX_SIZE);
	ASSERT(a_StartX < a_EndX);
	ASSERT(a_StartY < a_EndY);
	
	// Calculate the integral and fractional parts of each coord:
	int FloorX[MAX_SIZE];
	int FloorY[MAX_SIZE];
	NOISE_DATATYPE FracX[MAX_SIZE];
	NOISE_DATATYPE FracY[MAX_SIZE];
	int SameX[MAX_SIZE];
	int SameY[MAX_SIZE];
	int NumSameX, NumSameY;
	CalcFloorFrac(a_SizeX, a_StartX, a_EndX, FloorX, FracX, SameX, NumSameX);
	CalcFloorFrac(a_SizeY, a_StartY, a_EndY, FloorY, FracY, SameY, NumSameY);
	
	cCubicCell2D Cell(m_Noise, a_Array, a_SizeX, a_SizeY, FracX, FracY);
	
	Cell.InitWorkRnds(FloorX[0], FloorY[0]);
	
	// Calculate query values using Cell:
	int FromY = 0;
	for (int y = 0; y < NumSameY; y++)
	{
		int ToY = FromY + SameY[y];
		int FromX = 0;
		int CurFloorY = FloorY[FromY];
		for (int x = 0; x < NumSameX; x++)
		{
			int ToX = FromX + SameX[x];
			Cell.Generate(FromX, ToX, FromY, ToY);
			Cell.Move(FloorX[ToX], CurFloorY);
			FromX = ToX;
		}
		Cell.Move(FloorX[0], FloorY[ToY]);
		FromY = ToY;
	}
}





void cCubicNoise::CalcFloorFrac(
	int a_Size,
	NOISE_DATATYPE a_Start, NOISE_DATATYPE a_End,
	int * a_Floor, NOISE_DATATYPE * a_Frac,
	int * a_Same, int & a_NumSame
)
{
	NOISE_DATATYPE val = a_Start;
	NOISE_DATATYPE dif = (a_End - a_Start) / a_Size;
	for (int i = 0; i < a_Size; i++)
	{
		a_Floor[i] = FAST_FLOOR(val);
		a_Frac[i] = val - a_Floor[i];
		val += dif;
	}
	
	// Mark up the same floor values into a_Same / a_NumSame:
	int CurFloor = a_Floor[0];
	int LastSame = 0;
	a_NumSame = 0;
	for (int i = 1; i < a_Size; i++)
	{
		if (a_Floor[i] != CurFloor)
		{
			a_Same[a_NumSame] = i - LastSame;
			LastSame = i;
			a_NumSame += 1;
			CurFloor = a_Floor[i];
		}
	}  // for i - a_Floor[]
	if (LastSame < a_Size)
	{
		a_Same[a_NumSame] = a_Size - LastSame;
		a_NumSame += 1;
	}
}




