
// InterpolNoise.h

// Implements the cInterpolNoise class template representing a noise that interpolates the values between integer coords from a single set of neighbors





#pragma once

#include "Noise.h"

#define FAST_FLOOR(x) (((x) < 0) ? ((static_cast<int>(x)) - 1) : (static_cast<int>(x)))





////////////////////////////////////////////////////////////////////////////////
// cInterpolCell2D:

template <typename T>
class cInterpolCell2D
{
public:
	cInterpolCell2D(
		const cNoise & a_Noise,    ///< Noise to use for generating the random values
		NOISE_DATATYPE * a_Array,  ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,  ///< Count of the array, in each direction
		const NOISE_DATATYPE * a_FracX,  ///< Pointer to the array that stores the X fractional values
		const NOISE_DATATYPE * a_FracY   ///< Pointer to the attay that stores the Y fractional values
	):
		m_Noise(a_Noise),
		m_WorkRnds(&m_Workspace1),
		m_CurFloorX(0),
		m_CurFloorY(0),
		m_Array(a_Array),
		m_SizeX(a_SizeX),
		m_SizeY(a_SizeY),
		m_FracX(a_FracX),
		m_FracY(a_FracY)
	{
	}


	/** Generates part of the output noise array using the current m_WorkRnds[] values */
	void Generate(
		int a_FromX, int a_ToX,
		int a_FromY, int a_ToY
	)
	{
		for (int y = a_FromY; y < a_ToY; y++)
		{
			NOISE_DATATYPE Interp[2];
			NOISE_DATATYPE FracY = T::coeff(m_FracY[y]);
			Interp[0] = Lerp((*m_WorkRnds)[0][0], (*m_WorkRnds)[0][1], FracY);
			Interp[1] = Lerp((*m_WorkRnds)[1][0], (*m_WorkRnds)[1][1], FracY);
			int idx = y * m_SizeX + a_FromX;
			for (int x = a_FromX; x < a_ToX; x++)
			{
				m_Array[idx++] = Lerp(Interp[0], Interp[1], T::coeff(m_FracX[x]));
			}  // for x
		}  // for y
	}


	/** Initializes m_WorkRnds[] with the specified values of the noise at the specified integral coords. */
	void InitWorkRnds(int a_FloorX, int a_FloorY)
	{
		m_CurFloorX = a_FloorX;
		m_CurFloorY = a_FloorY;
		(*m_WorkRnds)[0][0] = m_Noise.IntNoise2D(m_CurFloorX,     m_CurFloorY);
		(*m_WorkRnds)[0][1] = m_Noise.IntNoise2D(m_CurFloorX,     m_CurFloorY + 1);
		(*m_WorkRnds)[1][0] = m_Noise.IntNoise2D(m_CurFloorX + 1, m_CurFloorY);
		(*m_WorkRnds)[1][1] = m_Noise.IntNoise2D(m_CurFloorX + 1, m_CurFloorY + 1);
	}


	/** Updates m_WorkRnds[] for the new integral coords */
	void Move(int a_NewFloorX, int a_NewFloorY)
	{
		// Swap the doublebuffer:
		int OldFloorX = m_CurFloorX;
		int OldFloorY = m_CurFloorY;
		Workspace * OldWorkRnds = m_WorkRnds;
		m_WorkRnds = (m_WorkRnds == &m_Workspace1) ? &m_Workspace2 : &m_Workspace1;

		// Reuse as much of the old workspace as possible:
		// TODO: Try out if simply calculating all 4 elements each time is faster than this monster loop
		int DiffX = OldFloorX - a_NewFloorX;
		int DiffY = OldFloorY - a_NewFloorY;
		for (int x = 0; x < 2; x++)
		{
			int cx = a_NewFloorX + x;
			int OldX = x - DiffX;  // Where would this X be in the old grid?
			for (int y = 0; y < 2; y++)
			{
				int cy = a_NewFloorY + y;
				int OldY = y - DiffY;  // Where would this Y be in the old grid?
				if ((OldX >= 0) && (OldX < 2) && (OldY >= 0) && (OldY < 2))
				{
					(*m_WorkRnds)[x][y] = (*OldWorkRnds)[OldX][OldY];
				}
				else
				{
					(*m_WorkRnds)[x][y] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise2D(cx, cy));
				}
			}
		}
		m_CurFloorX = a_NewFloorX;
		m_CurFloorY = a_NewFloorY;
	}

protected:
	typedef NOISE_DATATYPE Workspace[2][2];

	/** The noise used for generating the values at integral coords. */
	const cNoise & m_Noise;

	/** The current random values; points to either m_Workspace1 or m_Workspace2 (doublebuffering) */
	Workspace * m_WorkRnds;

	/** Buffer 1 for workspace doublebuffering, used in Move() */
	Workspace m_Workspace1;

	/** Buffer 2 for workspace doublebuffering, used in Move() */
	Workspace m_Workspace2;

	/** Coords of the currently calculated m_WorkRnds[]. */
	int m_CurFloorX, m_CurFloorY;

	/** The output array to generate into. */
	NOISE_DATATYPE * m_Array;

	/** Dimensions of the output array. */
	int m_SizeX, m_SizeY;

	/** Arrays holding the fractional values of the coords in each direction. */
	const NOISE_DATATYPE * m_FracX;
	const NOISE_DATATYPE * m_FracY;
} ;





////////////////////////////////////////////////////////////////////////////////
// cInterpolCell3D:

/** Holds a cache of the last calculated integral noise values and interpolates between them en masse.
Provides a massive optimization for cInterpolNoise.
Works by calculating multiple noise values (that have the same integral noise coords) at once. The underlying noise values
needn't be recalculated for these values, only the interpolation is done within the unit cube. */
template <typename T>
class cInterpolCell3D
{
public:
	cInterpolCell3D(
		const cNoise & a_Noise,                 ///< Noise to use for generating the random values
		NOISE_DATATYPE * a_Array,               ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY, int a_SizeZ,  ///< Count of the array, in each direction
		const NOISE_DATATYPE * a_FracX,         ///< Pointer to the array that stores the X fractional values
		const NOISE_DATATYPE * a_FracY,         ///< Pointer to the attay that stores the Y fractional values
		const NOISE_DATATYPE * a_FracZ          ///< Pointer to the array that stores the Z fractional values
	):
		m_Noise(a_Noise),
		m_WorkRnds(&m_Workspace1),
		m_CurFloorX(0),
		m_CurFloorY(0),
		m_CurFloorZ(0),
		m_Array(a_Array),
		m_SizeX(a_SizeX),
		m_SizeY(a_SizeY),
		m_SizeZ(a_SizeZ),
		m_FracX(a_FracX),
		m_FracY(a_FracY),
		m_FracZ(a_FracZ)
	{
	}


	/** Generates part of the output array using current m_WorkRnds[]. */
	void Generate(
		int a_FromX, int a_ToX,
		int a_FromY, int a_ToY,
		int a_FromZ, int a_ToZ
	)
	{
		for (int z = a_FromZ; z < a_ToZ; z++)
		{
			int idxZ = z * m_SizeX * m_SizeY;
			NOISE_DATATYPE Interp2[2][2];
			NOISE_DATATYPE FracZ = T::coeff(m_FracZ[z]);
			for (int x = 0; x < 2; x++)
			{
				for (int y = 0; y < 2; y++)
				{
					Interp2[x][y] = Lerp((*m_WorkRnds)[x][y][0], (*m_WorkRnds)[x][y][1], FracZ);
				}
			}
			for (int y = a_FromY; y < a_ToY; y++)
			{
				NOISE_DATATYPE Interp[2];
				NOISE_DATATYPE FracY = T::coeff(m_FracY[y]);
				Interp[0] = Lerp(Interp2[0][0], Interp2[0][1], FracY);
				Interp[1] = Lerp(Interp2[1][0], Interp2[1][1], FracY);
				int idx = idxZ + y * m_SizeX + a_FromX;
				for (int x = a_FromX; x < a_ToX; x++)
				{
					m_Array[idx++] = Lerp(Interp[0], Interp[1], T::coeff(m_FracX[x]));
				}  // for x
			}  // for y
		}  // for z
	}


	/** Initializes m_WorkRnds[] with the specified Floor values. */
	void InitWorkRnds(int a_FloorX, int a_FloorY, int a_FloorZ)
	{
		m_CurFloorX = a_FloorX;
		m_CurFloorY = a_FloorY;
		m_CurFloorZ = a_FloorZ;
		(*m_WorkRnds)[0][0][0] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise3D(m_CurFloorX,     m_CurFloorY,     m_CurFloorZ));
		(*m_WorkRnds)[0][0][1] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise3D(m_CurFloorX,     m_CurFloorY,     m_CurFloorZ + 1));
		(*m_WorkRnds)[0][1][0] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise3D(m_CurFloorX,     m_CurFloorY + 1, m_CurFloorZ));
		(*m_WorkRnds)[0][1][1] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise3D(m_CurFloorX,     m_CurFloorY + 1, m_CurFloorZ + 1));
		(*m_WorkRnds)[1][0][0] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise3D(m_CurFloorX + 1, m_CurFloorY,     m_CurFloorZ));
		(*m_WorkRnds)[1][0][1] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise3D(m_CurFloorX + 1, m_CurFloorY,     m_CurFloorZ + 1));
		(*m_WorkRnds)[1][1][0] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise3D(m_CurFloorX + 1, m_CurFloorY + 1, m_CurFloorZ));
		(*m_WorkRnds)[1][1][1] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise3D(m_CurFloorX + 1, m_CurFloorY + 1, m_CurFloorZ + 1));
	}


	/** Updates m_WorkRnds[] for the new Floor values. */
	void Move(int a_NewFloorX, int a_NewFloorY, int a_NewFloorZ)
	{
		// Swap the doublebuffer:
		int OldFloorX = m_CurFloorX;
		int OldFloorY = m_CurFloorY;
		int OldFloorZ = m_CurFloorZ;
		Workspace * OldWorkRnds = m_WorkRnds;
		m_WorkRnds = (m_WorkRnds == &m_Workspace1) ? &m_Workspace2 : &m_Workspace1;

		// Reuse as much of the old workspace as possible:
		// TODO: Try out if simply calculating all 8 elements each time is faster than this monster loop
		int DiffX = OldFloorX - a_NewFloorX;
		int DiffY = OldFloorY - a_NewFloorY;
		int DiffZ = OldFloorZ - a_NewFloorZ;
		for (int x = 0; x < 2; x++)
		{
			int cx = a_NewFloorX + x;
			int OldX = x - DiffX;  // Where would this X be in the old grid?
			for (int y = 0; y < 2; y++)
			{
				int cy = a_NewFloorY + y;
				int OldY = y - DiffY;  // Where would this Y be in the old grid?
				for (int z = 0; z < 2; z++)
				{
					int cz = a_NewFloorZ + z;
					int OldZ = z - DiffZ;
					if ((OldX >= 0) && (OldX < 2) && (OldY >= 0) && (OldY < 2) && (OldZ >= 0) && (OldZ < 2))
					{
						(*m_WorkRnds)[x][y][z] = (*OldWorkRnds)[OldX][OldY][OldZ];
					}
					else
					{
						(*m_WorkRnds)[x][y][z] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise3D(cx, cy, cz));
					}
				}  // for z
			}  // for y
		}  // for x
		m_CurFloorX = a_NewFloorX;
		m_CurFloorY = a_NewFloorY;
		m_CurFloorZ = a_NewFloorZ;
	}

protected:
	typedef NOISE_DATATYPE Workspace[2][2][2];

	/** The noise used for generating the values at integral coords. */
	const cNoise & m_Noise;

	/** The current random values; points to either m_Workspace1 or m_Workspace2 (doublebuffering) */
	Workspace * m_WorkRnds;

	/** Buffer 1 for workspace doublebuffering, used in Move() */
	Workspace m_Workspace1;

	/** Buffer 2 for workspace doublebuffering, used in Move() */
	Workspace m_Workspace2;

	/** The integral coords of the currently calculated WorkRnds[] */
	int m_CurFloorX, m_CurFloorY, m_CurFloorZ;

	/** The output array where the noise is calculated. */
	NOISE_DATATYPE * m_Array;

	/** Dimensions of the output array. */
	int m_SizeX, m_SizeY, m_SizeZ;

	/** Arrays holding the fractional values of the coords in each direction. */
	const NOISE_DATATYPE * m_FracX;
	const NOISE_DATATYPE * m_FracY;
	const NOISE_DATATYPE * m_FracZ;
} ;





////////////////////////////////////////////////////////////////////////////////
// cInterpolNoise:

template <typename T>
class cInterpolNoise
{
	/** Maximum size, for each direction, of the generated array. */
	static const int MAX_SIZE = 256;

public:
	cInterpolNoise(int a_Seed):
		m_Noise(a_Seed)
	{
	}


	/** Sets a new seed for the generators. Relays the seed to the underlying noise. */
	void SetSeed(int a_Seed)
	{
		m_Noise.SetSeed(a_Seed);
	}


	/** Fills a 2D array with the values of the noise. */
	void Generate2D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,                        ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY   ///< Noise-space coords of the array in the Y direction
	) const
	{
		ASSERT(a_SizeX > 0);
		ASSERT(a_SizeY > 0);
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

		cInterpolCell2D<T> Cell(m_Noise, a_Array, a_SizeX, a_SizeY, FracX, FracY);

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
			}  // for x
			Cell.Move(FloorX[0], FloorY[ToY]);
			FromY = ToY;
		}  // for y
	}


	/** Fills a 3D array with the values of the noise. */
	void Generate3D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y + a_SizeX * a_SizeY * z]
		int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ   ///< Noise-space coords of the array in the Z direction
	) const
	{
		// Check params:
		ASSERT(a_SizeX > 1);
		ASSERT(a_SizeY > 1);

		ASSERT(a_SizeX < MAX_SIZE);
		ASSERT(a_SizeY < MAX_SIZE);
		ASSERT(a_SizeZ < MAX_SIZE);
		ASSERT(a_StartX < a_EndX);
		ASSERT(a_StartY < a_EndY);
		ASSERT(a_StartZ < a_EndZ);

		// Calculate the integral and fractional parts of each coord:
		int FloorX[MAX_SIZE];
		int FloorY[MAX_SIZE];
		int FloorZ[MAX_SIZE];
		NOISE_DATATYPE FracX[MAX_SIZE];
		NOISE_DATATYPE FracY[MAX_SIZE];
		NOISE_DATATYPE FracZ[MAX_SIZE];
		int SameX[MAX_SIZE];
		int SameY[MAX_SIZE];
		int SameZ[MAX_SIZE];
		int NumSameX, NumSameY, NumSameZ;
		CalcFloorFrac(a_SizeX, a_StartX, a_EndX, FloorX, FracX, SameX, NumSameX);
		CalcFloorFrac(a_SizeY, a_StartY, a_EndY, FloorY, FracY, SameY, NumSameY);
		CalcFloorFrac(a_SizeZ, a_StartZ, a_EndZ, FloorZ, FracZ, SameZ, NumSameZ);

		cInterpolCell3D<T> Cell(
			m_Noise, a_Array,
			a_SizeX, a_SizeY, a_SizeZ,
			FracX, FracY, FracZ
		);

		Cell.InitWorkRnds(FloorX[0], FloorY[0], FloorZ[0]);

		// Calculate query values using Cell:
		int FromZ = 0;
		for (int z = 0; z < NumSameZ; z++)
		{
			int ToZ = FromZ + SameZ[z];
			int CurFloorZ = FloorZ[FromZ];
			int FromY = 0;
			for (int y = 0; y < NumSameY; y++)
			{
				int ToY = FromY + SameY[y];
				int CurFloorY = FloorY[FromY];
				int FromX = 0;
				for (int x = 0; x < NumSameX; x++)
				{
					int ToX = FromX + SameX[x];
					Cell.Generate(FromX, ToX, FromY, ToY, FromZ, ToZ);
					Cell.Move(FloorX[ToX], CurFloorY, CurFloorZ);
					FromX = ToX;
				}
				Cell.Move(FloorX[0], FloorY[ToY], CurFloorZ);
				FromY = ToY;
			}  // for y
			Cell.Move(FloorX[0], FloorY[0], FloorZ[ToZ]);
			FromZ = ToZ;
		}  // for z
	}

protected:

	/** The noise used for the underlying value generation. */
	cNoise m_Noise;


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
	) const
	{
		ASSERT(a_Size > 0);

		// Calculate the floor and frac values:
		NOISE_DATATYPE val = a_Start;
		NOISE_DATATYPE dif = (a_End - a_Start) / (a_Size - 1);
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
};





/** A fifth-degree curve for interpolating.
Implemented as a functor for better chance of inlining. */
struct Interp5Deg
{
	static NOISE_DATATYPE coeff(NOISE_DATATYPE a_Val)
	{
		return a_Val * a_Val * a_Val * (a_Val * (a_Val * 6 - 15) + 10);
	}
};

typedef cInterpolNoise<Interp5Deg> cInterp5DegNoise;



