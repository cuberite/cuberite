
// Noise.h

// Declares the cNoise, cCubicNoise and cPerlinNoise classes for generating noise

#pragma once

// Some settings
#define NOISE_DATATYPE float





#ifdef _MSC_VER
	#define INLINE __forceinline
#else
	#define INLINE inline
#endif





class cNoise
{
public:
	cNoise(int a_Seed);
	cNoise(const cNoise & a_Noise);

	// The following functions, if not marked INLINE, are about 20 % slower
	INLINE NOISE_DATATYPE IntNoise1D(int a_X) const;
	INLINE NOISE_DATATYPE IntNoise2D(int a_X, int a_Y) const;
	INLINE NOISE_DATATYPE IntNoise3D(int a_X, int a_Y, int a_Z) const;

	// Note: These functions have a mod8-irregular chance - each of the mod8 remainders has different chance of occurrence. Divide by 8 to rectify.
	INLINE int IntNoise1DInt(int a_X) const;
	INLINE int IntNoise2DInt(int a_X, int a_Y) const;
	INLINE int IntNoise3DInt(int a_X, int a_Y, int a_Z) const;

	NOISE_DATATYPE LinearNoise1D(NOISE_DATATYPE a_X) const;
	NOISE_DATATYPE CosineNoise1D(NOISE_DATATYPE a_X) const;
	NOISE_DATATYPE CubicNoise1D (NOISE_DATATYPE a_X) const;
	NOISE_DATATYPE SmoothNoise1D(int a_X) const;

	NOISE_DATATYPE CubicNoise2D (NOISE_DATATYPE a_X, NOISE_DATATYPE a_Y) const;

	NOISE_DATATYPE CubicNoise3D (NOISE_DATATYPE a_X, NOISE_DATATYPE a_Y, NOISE_DATATYPE a_Z) const;

	void SetSeed(int a_Seed) { m_Seed = a_Seed; }

	INLINE static NOISE_DATATYPE CubicInterpolate (NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_C, NOISE_DATATYPE a_D, NOISE_DATATYPE a_Pct);
	INLINE static NOISE_DATATYPE CosineInterpolate(NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_Pct);
	INLINE static NOISE_DATATYPE LinearInterpolate(NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_Pct);

private:
	int m_Seed;
} ;





class cCubicNoise
{
public:
	static const int MAX_SIZE = 512;  ///< Maximum size of each dimension of the query arrays.
	
	
	cCubicNoise(int a_Seed);
	
	
	void Generate1D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into
		int a_SizeX,                                     ///< Count of the array
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX   ///< Noise-space coords of the array
	) const;
	
	
	void Generate2D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,                        ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY   ///< Noise-space coords of the array in the Y direction
	) const;
	
	
	void Generate3D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y + a_SizeX * a_SizeY * z]
		int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ   ///< Noise-space coords of the array in the Z direction
	) const;
	
protected:
	typedef NOISE_DATATYPE Workspace1D[4];
	typedef NOISE_DATATYPE Workspace2D[4][4];
	
	cNoise m_Noise;  // Used for integral rnd values

	#ifdef _DEBUG
		// Statistics on the noise-space coords:
		static int m_NumSingleX;
		static int m_NumSingleXY;
		static int m_NumSingleY;
		static int m_NumCalls;
	#endif  // _DEBUG
	
	/// Calculates the integral and fractional parts along one axis.
	void CalcFloorFrac(
		int a_Size,
		NOISE_DATATYPE a_Start, NOISE_DATATYPE a_End,
		int * a_Floor, NOISE_DATATYPE * a_Frac,
		int * a_Same, int & a_NumSame
	) const;
	
	void UpdateWorkRnds2DX(
		Workspace2D & a_WorkRnds,
		Workspace1D & a_Interps,
		int a_LastFloorX, int a_NewFloorX,
		int a_FloorY,
		NOISE_DATATYPE a_FractionY
	) const;
} ;





class cPerlinNoise
{
public:
	cPerlinNoise(void);
	cPerlinNoise(int a_Seed);
	
	
	void SetSeed(int a_Seed);
	
	void AddOctave(NOISE_DATATYPE a_Frequency, NOISE_DATATYPE a_Amplitude);
	
	void Generate1D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into
		int a_SizeX,                                     ///< Count of the array
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	) const;
	
	
	void Generate2D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,                        ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	) const;
	
	
	void Generate3D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y + a_SizeX * a_SizeY * z]
		int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ,  ///< Noise-space coords of the array in the Z direction
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	) const;
	
protected:
	class cOctave
	{
	public:
		cCubicNoise m_Noise;
		
		NOISE_DATATYPE m_Frequency;  // Coord multiplier
		NOISE_DATATYPE m_Amplitude;  // Value multiplier
		
		cOctave(int a_Seed, NOISE_DATATYPE a_Frequency, NOISE_DATATYPE a_Amplitude) :
			m_Noise(a_Seed),
			m_Frequency(a_Frequency),
			m_Amplitude(a_Amplitude)
		{
		}
	} ;
	
	typedef std::vector<cOctave> cOctaves;
	
	int      m_Seed;
	cOctaves m_Octaves;
} ;





class cRidgedMultiNoise
{
public:
	cRidgedMultiNoise(void);
	cRidgedMultiNoise(int a_Seed);
	
	
	void SetSeed(int a_Seed);
	
	void AddOctave(NOISE_DATATYPE a_Frequency, NOISE_DATATYPE a_Amplitude);
	
	void Generate1D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into
		int a_SizeX,                                     ///< Count of the array
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	) const;
	
	
	void Generate2D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,                        ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	) const;
	
	
	void Generate3D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y + a_SizeX * a_SizeY * z]
		int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ,  ///< Noise-space coords of the array in the Z direction
		NOISE_DATATYPE * a_Workspace = NULL              ///< Workspace that this function can use and trash
	) const;
	
protected:
	class cOctave
	{
	public:
		cCubicNoise m_Noise;
		
		NOISE_DATATYPE m_Frequency;  // Coord multiplier
		NOISE_DATATYPE m_Amplitude;  // Value multiplier
		
		cOctave(int a_Seed, NOISE_DATATYPE a_Frequency, NOISE_DATATYPE a_Amplitude) :
			m_Noise(a_Seed),
			m_Frequency(a_Frequency),
			m_Amplitude(a_Amplitude)
		{
		}
	} ;
	
	typedef std::vector<cOctave> cOctaves;
	
	int      m_Seed;
	cOctaves m_Octaves;
} ;





////////////////////////////////////////////////////////////////////////////////
// Inline function definitions:
// These need to be in the header, otherwise linker error occur in MSVC

NOISE_DATATYPE cNoise::IntNoise1D(int a_X) const
{
	int x = ((a_X * m_Seed) << 13) ^ a_X;
	return (1 - (NOISE_DATATYPE)((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824);
	// returns a float number in the range of [-1, 1]
}





NOISE_DATATYPE cNoise::IntNoise2D(int a_X, int a_Y) const
{
	int n = a_X + a_Y * 57 + m_Seed * 57 * 57;
	n = (n << 13) ^ n;
	return (1 - (NOISE_DATATYPE)((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824);
	// returns a float number in the range of [-1, 1]
}





NOISE_DATATYPE cNoise::IntNoise3D(int a_X, int a_Y, int a_Z) const
{
	int n = a_X + a_Y * 57 + a_Z * 57 * 57 + m_Seed * 57 * 57 * 57;
	n = (n << 13) ^ n;
	return ((NOISE_DATATYPE)1 - (NOISE_DATATYPE)((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
	// returns a float number in the range of [-1, 1]
}





int cNoise::IntNoise1DInt(int a_X) const
{
	int x = ((a_X * m_Seed) << 13) ^ a_X;
	return ((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff);
}





int cNoise::IntNoise2DInt(int a_X, int a_Y) const
{
	int n = a_X + a_Y * 57 + m_Seed * 57 * 57;
	n = (n << 13) ^ n;
	return ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff);
}





int cNoise::IntNoise3DInt(int a_X, int a_Y, int a_Z) const
{
	int n = a_X + a_Y * 57 + a_Z * 57 * 57 + m_Seed * 57 * 57 * 57;
	n = (n << 13) ^ n;
	return ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff);
}





NOISE_DATATYPE cNoise::CubicInterpolate(NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_C, NOISE_DATATYPE a_D, NOISE_DATATYPE a_Pct)
{
	NOISE_DATATYPE P = (a_D - a_C) - (a_A - a_B);
	NOISE_DATATYPE Q = (a_A - a_B) - P;
	NOISE_DATATYPE R = a_C - a_A;
	NOISE_DATATYPE S = a_B;

	return ((P * a_Pct + Q) * a_Pct + R) * a_Pct + S;
}





NOISE_DATATYPE cNoise::CosineInterpolate(NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_Pct)
{
	const NOISE_DATATYPE ft = a_Pct * (NOISE_DATATYPE)3.1415927;
	const NOISE_DATATYPE f = (NOISE_DATATYPE)((NOISE_DATATYPE)(1 - cos(ft)) * (NOISE_DATATYPE)0.5);
	return  a_A * (1 - f) + a_B * f;
}





NOISE_DATATYPE cNoise::LinearInterpolate(NOISE_DATATYPE a_A, NOISE_DATATYPE a_B, NOISE_DATATYPE a_Pct)
{
	return  a_A * (1 - a_Pct) + a_B * a_Pct;
}





////////////////////////////////////////////////////////////////////////////////
// Global functions:

extern void Debug2DNoise(const NOISE_DATATYPE * a_Noise, int a_SizeX, int a_SizeY, const AString & a_FileNameBase);
extern void Debug3DNoise(const NOISE_DATATYPE * a_Noise, int a_SizeX, int a_SizeY, int a_SizeZ, const AString & a_FileNameBase);




