#pragma once

// Some settings
#define NOISE_USE_INLINE 1
#define NOISE_USE_SSE 0

#define NOISE_DATATYPE double





// Do not touch
#if NOISE_USE_INLINE
	#ifdef _MSC_VER
		#define __NOISE_INLINE__ __forceinline
	#else
		#define __NOISE_INLINE__ inline
	#endif  // _MSC_VER
#else
	#define __NOISE_INLINE__
#endif

#if NOISE_USE_SSE
	#include <emmintrin.h>
#endif





class cNoise
{
public:
	cNoise( unsigned int a_Seed );

#if NOISE_USE_SSE
	__m128 SSE_IntNoise2D( int a_X1, int a_Y1, int a_X2, int a_Y2, int a_X3, int a_Y3, int a_X4, int a_Y4 ) const;
#endif

	__NOISE_INLINE__ float IntNoise( int a_X ) const;
	__NOISE_INLINE__ float IntNoise2D( int a_X, int a_Y ) const;
	__NOISE_INLINE__ float IntNoise3D( int a_X, int a_Y, int a_Z ) const;

	// Note: These functions have a mod8-irregular chance - each of the mod8 remainders has different chance of occurrence. Divide by 8 to rectify.
	__NOISE_INLINE__ int IntNoise1DInt( int a_X ) const;
	__NOISE_INLINE__ int IntNoise2DInt( int a_X, int a_Y ) const;
	__NOISE_INLINE__ int IntNoise3DInt( int a_X, int a_Y, int a_Z ) const;

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

	__NOISE_INLINE__ static float CubicInterpolate (float a_A, float a_B, float a_C, float a_D, float a_Pct);
	__NOISE_INLINE__ static float CosineInterpolate(float a_A, float a_B, float a_Pct);
	__NOISE_INLINE__ static float LinearInterpolate(float a_A, float a_B, float a_Pct);

private:

#if NOISE_USE_SSE
	__m128 CubicInterpolate4( const __m128 & a_A, const __m128 & a_B, const __m128 & a_C, const __m128 & a_D, float a_Pct ) const;
#endif

	unsigned int m_Seed;
};





/// Linearly interpolates values in the array between the anchor points
extern void IntArrayLinearInterpolate2D(
	int * a_Array, 
	int a_SizeX, int a_SizeY,  // Dimensions of the array
	int a_AnchorStepX, int a_AnchorStepY  // Distances between the anchor points in each direction
);





/// Linearly interpolates values in the array between the anchor points; universal data type
template<typename TYPE> void ArrayLinearInterpolate2D(
	TYPE * a_Array, 
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
			TYPE StartValue = a_Array[Idx];
			TYPE EndValue   = a_Array[Idx + a_SizeX * a_AnchorStepY];
			TYPE Diff = EndValue - StartValue;
			for (int CellY = 1; CellY < a_AnchorStepY; CellY++)
			{
				a_Array[Idx + a_SizeX * CellY] = StartValue + Diff * CellY / a_AnchorStepY;
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
			TYPE StartValue = a_Array[Idx];
			TYPE EndValue   = a_Array[Idx + a_AnchorStepX];
			TYPE Diff = EndValue - StartValue;
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





class cCubicNoise
{
public:
	static const int MAX_SIZE = 512;  ///< Maximum size of each dimension of the query arrays.
	
	
	cCubicNoise(int a_Seed);
	
	
	void Generate1D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into
		int a_SizeX,                                     ///< Count of the array
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	);
	
	
	void Generate2D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,                        ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	);
	
	
	void Generate3D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y + a_SizeX * a_SizeY * z]
		int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ,  ///< Noise-space coords of the array in the Z direction
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	);
	
protected:
	typedef NOISE_DATATYPE Workspace1D[4];
	typedef NOISE_DATATYPE Workspace2D[4][4];
	
	cNoise m_Noise;  // Used for integral rnd values
	
	/// Calculates the integral and fractional parts along one axis.
	void CalcFloorFrac(
		int a_Size,
		NOISE_DATATYPE a_Start, NOISE_DATATYPE a_End,
		int * a_Floor, NOISE_DATATYPE * a_Frac,
		int * a_Same, int & a_NumSame
	);
	
	void UpdateWorkRnds2DX(
		Workspace2D & a_WorkRnds,
		Workspace1D & a_Interps,
		int a_LastFloorX, int a_NewFloorX,
		int a_FloorY,
		NOISE_DATATYPE a_FractionY
	);
} ;





class cPerlinNoise
{
public:
	void Generate1D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into
		int a_SizeX,                                     ///< Count of the array
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	);
	
	
	void Generate2D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,                        ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	);
	
	
	void Generate3D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y + a_SizeX * a_SizeY * z]
		int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ,  ///< Noise-space coords of the array in the Z direction
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	);
} ;




