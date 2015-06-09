
// Noise.h

// Declares the cOldNoise, cCubicNoise and cPerlinNoise classes for generating noise

#pragma once

#include <cmath>

/** The datatype used by all the noise generators. */
typedef float NOISE_DATATYPE;

#include "Noise/OctavedNoise.h"
#include "Noise/RidgedNoise.h"





class cOldNoise
{
public:
	cOldNoise(int a_Seed);
	cOldNoise(const cOldNoise & a_Noise);

	// The following functions, if not marked INLINE, are about 20 % slower
	inline NOISE_DATATYPE IntNoise1D(int a_X) const;
	inline NOISE_DATATYPE IntNoise2D(int a_X, int a_Y) const;
	inline NOISE_DATATYPE IntNoise3D(int a_X, int a_Y, int a_Z) const;

	// Return a float number in the specified range:
	inline NOISE_DATATYPE IntNoise2DInRange(int a_X, int a_Y, float a_Min, float a_Max) const
	{
		return a_Min + std::abs(IntNoise2D(a_X, a_Y)) * (a_Max - a_Min);
	}

	// Note: These functions have a mod8-irregular chance - each of the mod8 remainders has different chance of occurrence. Divide by 8 to rectify.
	inline int IntNoise1DInt(int a_X) const;
	inline int IntNoise2DInt(int a_X, int a_Y) const;
	inline int IntNoise3DInt(int a_X, int a_Y, int a_Z) const;

	NOISE_DATATYPE LinearNoise1D(NOISE_DATATYPE a_X) const;
	NOISE_DATATYPE CosineNoise1D(NOISE_DATATYPE a_X) const;
	NOISE_DATATYPE CubicOldNoise1D (NOISE_DATATYPE a_X) const;
	NOISE_DATATYPE SmoothNoise1D(int a_X) const;

	NOISE_DATATYPE CubicOldNoise2D (NOISE_DATATYPE a_X, NOISE_DATATYPE a_Y) const;

	NOISE_DATATYPE CubicOldNoise3D (NOISE_DATATYPE a_X, NOISE_DATATYPE a_Y, NOISE_DATATYPE a_Z) const;

	void SetSeed(int a_Seed) { m_Seed = a_Seed; }

	inline static NOISE_DATATYPE CubicInterpolate (NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_C, NOISE_DATATYPE a_D, NOISE_DATATYPE a_Pct);
	inline static NOISE_DATATYPE CosineInterpolate(NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_Pct);
	inline static NOISE_DATATYPE LinearInterpolate(NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_Pct);

private:
	int m_Seed;
} ;





class cOldCubicNoise
{
public:
	/** Maximum size of each dimension of the query arrays. */
	static const int MAX_SIZE = 512;
	
	
	/** Creates a new instance with the specified seed. */
	cOldCubicNoise(int a_Seed);
	
	
	/** Fills a 2D array with the values of the noise. */
	void Generate2D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,                        ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY   ///< Noise-space coords of the array in the Y direction
	) const;
	
	
	/** Fills a 3D array with the values of the noise. */
	void Generate3D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y + a_SizeX * a_SizeY * z]
		int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ   ///< Noise-space coords of the array in the Z direction
	) const;
	
protected:
	
	/** Noise used for integral random values. */
	cOldNoise m_Noise;


	/** Calculates the integral and fractional parts along one axis.
	a_Floor will receive the integral parts (array of a_Size ints).
	a_Frac will receive the fractional parts (array of a_Size floats).
	a_Same will receive the counts of items that have the same integral parts (array of up to a_Size ints).
	a_NumSame will receive the count of a_Same elements (total count of different integral parts). */
	void CalcFloorFrac(
		int a_Size,
		NOISE_DATATYPE a_Start, NOISE_DATATYPE a_End,
		int * a_Floor, NOISE_DATATYPE * a_Frac,
		int * a_Same, int & a_NumSame
	) const;
} ;





/** Improved noise, as described by Ken Perlin: http://mrl.nyu.edu/~perlin/paper445.pdf
Implementation adapted from Perlin's Java implementation: http://mrl.nyu.edu/~perlin/noise/ */
class cOldImprovedNoise
{
public:
	/** Constructs a new instance of the noise obbject.
	Note that this operation is quite expensive (the permutation array being constructed). */
	cOldImprovedNoise(int a_Seed);


	/** Fills a 2D array with the values of the noise. */
	void Generate2D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,                        ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY   ///< Noise-space coords of the array in the Y direction
	) const;
	
	
	/** Fills a 3D array with the values of the noise. */
	void Generate3D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y + a_SizeX * a_SizeY * z]
		int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ   ///< Noise-space coords of the array in the Z direction
	) const;

	/** Returns the value at the specified integral coords. Used for raw speed measurement. */
	NOISE_DATATYPE GetValueAt(int a_X, int a_Y, int a_Z);

protected:

	/** The permutation table used by the noise function. Initialized using seed. */
	int m_Perm[512];


	/** Calculates the fade curve, 6 * t^5 - 15 * t^4 + 10 * t^3. */
	inline static NOISE_DATATYPE Fade(NOISE_DATATYPE a_T)
	{
		return a_T * a_T * a_T * (a_T * (a_T * 6 - 15) + 10);
	}

	/** Returns the gradient value based on the hash. */
	inline static NOISE_DATATYPE Grad(int a_Hash, NOISE_DATATYPE a_X, NOISE_DATATYPE a_Y, NOISE_DATATYPE a_Z)
	{
		int hash = a_Hash % 16;
		NOISE_DATATYPE u = (hash < 8) ? a_X : a_Y;
		NOISE_DATATYPE v = (hash < 4) ? a_Y : (((hash == 12) || (hash == 14)) ? a_X : a_Z);
		return (((hash & 1) == 0) ? u : -u) + (((hash & 2) == 0) ? v : -v);
	}
};





typedef cOctavedNoise<cOldCubicNoise> cOldPerlinNoise;
typedef cOctavedNoise<cRidgedNoise<cOldCubicNoise>> cOldRidgedMultiNoise;





////////////////////////////////////////////////////////////////////////////////
// Inline function definitions:
// These need to be in the header, otherwise linker error occur in MSVC

NOISE_DATATYPE cOldNoise::IntNoise1D(int a_X) const
{
	int x = ((a_X * m_Seed) << 13) ^ a_X;
	return (1 - (NOISE_DATATYPE)((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824);
	// returns a float number in the range of [-1, 1]
}





NOISE_DATATYPE cOldNoise::IntNoise2D(int a_X, int a_Y) const
{
	int n = a_X + a_Y * 57 + m_Seed * 57 * 57;
	n = (n << 13) ^ n;
	return (1 - (NOISE_DATATYPE)((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824);
	// returns a float number in the range of [-1, 1]
}





NOISE_DATATYPE cOldNoise::IntNoise3D(int a_X, int a_Y, int a_Z) const
{
	int n = a_X + a_Y * 57 + a_Z * 57 * 57 + m_Seed * 57 * 57 * 57;
	n = (n << 13) ^ n;
	return ((NOISE_DATATYPE)1 - (NOISE_DATATYPE)((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
	// returns a float number in the range of [-1, 1]
}





int cOldNoise::IntNoise1DInt(int a_X) const
{
	int x = ((a_X * m_Seed) << 13) ^ a_X;
	return ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff);
}





int cOldNoise::IntNoise2DInt(int a_X, int a_Y) const
{
	int n = a_X + a_Y * 57 + m_Seed * 57 * 57;
	n = (n << 13) ^ n;
	return ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff);
}





int cOldNoise::IntNoise3DInt(int a_X, int a_Y, int a_Z) const
{
	int n = a_X + a_Y * 57 + a_Z * 57 * 57 + m_Seed * 57 * 57 * 57;
	n = (n << 13) ^ n;
	return ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff);
}





NOISE_DATATYPE cOldNoise::CubicInterpolate(NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_C, NOISE_DATATYPE a_D, NOISE_DATATYPE a_Pct)
{
	NOISE_DATATYPE P = (a_D - a_C) - (a_A - a_B);
	NOISE_DATATYPE Q = (a_A - a_B) - P;
	NOISE_DATATYPE R = a_C - a_A;
	NOISE_DATATYPE S = a_B;

	return ((P * a_Pct + Q) * a_Pct + R) * a_Pct + S;
}





NOISE_DATATYPE cOldNoise::CosineInterpolate(NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_Pct)
{
	const NOISE_DATATYPE ft = a_Pct * (NOISE_DATATYPE)3.1415927;
	const NOISE_DATATYPE f = (NOISE_DATATYPE)((NOISE_DATATYPE)(1 - cos(ft)) * (NOISE_DATATYPE)0.5);
	return  a_A * (1 - f) + a_B * f;
}





NOISE_DATATYPE cOldNoise::LinearInterpolate(NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_Pct)
{
	return  a_A * (1 - a_Pct) + a_B * a_Pct;
}








