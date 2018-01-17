
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Noise.h"

#define FAST_FLOOR(x) (((x) < 0) ? ((static_cast<int>(x)) - 1) : (static_cast<int>(x)))





#if 0
/** cImprovedPerlin noise test suite:
- Generate a rather large 2D and 3D noise array and output it to a file
- Compare performance of cCubicNoise and cImprovedNoise, both in single-value and 3D-array usages */
static class cImprovedPerlinNoiseTest
{
public:
	cImprovedPerlinNoiseTest(void)
	{
		printf("Performing Improved Perlin Noise tests...\n");
		TestImage();
		TestSpeed();
		TestSpeedArr();
		printf("Improved Perlin Noise tests complete.\n");
	}


	/** Tests the noise by generating 2D and 3D images and dumping them to files. */
	void TestImage(void)
	{
		static const int SIZE_X = 256;
		static const int SIZE_Y = 256;
		static const int SIZE_Z = 16;

		cImprovedNoise noise(1);
		std::unique_ptr<NOISE_DATATYPE[]> arr(new NOISE_DATATYPE[SIZE_X * SIZE_Y * SIZE_Z]);
		noise.Generate3D(arr.get(), SIZE_X, SIZE_Y, SIZE_Z, 0, 14, 0, 14, 0, 14);
		Debug3DNoise(arr.get(), SIZE_X, SIZE_Y, SIZE_Z, "ImprovedPerlinNoiseTest3D", 128);
		noise.Generate2D(arr.get(), SIZE_X, SIZE_Y, 0, 14, 15, 28);
		Debug2DNoise(arr.get(), SIZE_X, SIZE_Y, "ImprovedPerlinNoiseTest2D", 128);
	}


	/** Tests the speeds of cImprovedPerlin and cCubicNoise when generating individual values. */
	void TestSpeed(void)
	{
		cImprovedNoise improvedNoise(1);
		cNoise         noise(1);
		cTimer timer;

		// Measure the improvedNoise:
		NOISE_DATATYPE sum = 0;
		long long start = timer.GetNowTime();
		for (int i = 0; i < 100000000; i++)
		{
			sum += improvedNoise.GetValueAt(i, 0, -i);
		}
		long long finish = timer.GetNowTime();
		printf("cImprovedNoise took %.2f seconds; total is %f.\n", static_cast<float>(finish - start) / 1000.0f, sum);

		// Measure the cubicNoise:
		sum = 0;
		start = timer.GetNowTime();
		for (int i = 0; i < 100000000; i++)
		{
			sum += noise.IntNoise3D(i, 0, -i);
		}
		finish = timer.GetNowTime();
		printf("cCubicNoise took %.2f seconds; total is %f.\n", static_cast<float>(finish - start) / 1000.0f, sum);
	}


	/** Tests the speeds of cImprovedPerlin and cCubicNoise when generating arrays. */
	void TestSpeedArr(void)
	{
		static const int SIZE_X = 256;
		static const int SIZE_Y = 256;
		static const int SIZE_Z = 16;

		std::unique_ptr<NOISE_DATATYPE[]> arr(new NOISE_DATATYPE[SIZE_X * SIZE_Y * SIZE_Z]);
		cTimer timer;
		cImprovedNoise improvedNoise(1);
		cCubicNoise    cubicNoise(1);

		// Measure the improvedNoise:
		long long start = timer.GetNowTime();
		for (int i = 0; i < 40; i++)
		{
			improvedNoise.Generate3D(arr.get(), SIZE_X, SIZE_Y, SIZE_Z, 0, 14, 0, 14, 0, 14);
		}
		long long finish = timer.GetNowTime();
		printf("cImprovedNoise(arr) took %.2f seconds.\n", static_cast<float>(finish - start) / 1000.0f);

		// Measure the cubicNoise:
		start = timer.GetNowTime();
		for (int i = 0; i < 40; i++)
		{
			cubicNoise.Generate3D(arr.get(), SIZE_X, SIZE_Y, SIZE_Z, 0, 14, 0, 14, 0, 14);
		}
		finish = timer.GetNowTime();
		printf("cCubicNoise(arr) took %.2f seconds.\n", static_cast<float>(finish - start) / 1000.0f);
	}
} g_Test;

#endif





////////////////////////////////////////////////////////////////////////////////
// Globals:

void Debug3DNoise(const NOISE_DATATYPE * a_Noise, size_t a_SizeX, size_t a_SizeY, size_t a_SizeZ, const AString & a_FileNameBase, NOISE_DATATYPE a_Coeff)
{
	const int BUF_SIZE = 512;
	ASSERT(a_SizeX <= BUF_SIZE);  // Just stretch it, if needed

	// Save in XY cuts:
	cFile f1;
	if (f1.Open(Printf("%s_XY (%zu).grab", a_FileNameBase.c_str(), a_SizeX), cFile::fmWrite))
	{
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t y = 0; y < a_SizeY; y++)
			{
				size_t idx = y * a_SizeX + z * a_SizeX * a_SizeY;
				unsigned char buf[BUF_SIZE];
				for (size_t x = 0; x < a_SizeX; x++)
				{
					buf[x] = static_cast<unsigned char>(Clamp(static_cast<int>(128 + a_Coeff * a_Noise[idx++]), 0, 255));
				}
				f1.Write(buf, a_SizeX);
			}  // for y
			unsigned char buf[BUF_SIZE];
			memset(buf, 0, a_SizeX);
			f1.Write(buf, a_SizeX);
		}  // for z
	}  // if (XY file open)

	cFile f2;
	if (f2.Open(Printf("%s_XZ (%zu).grab", a_FileNameBase.c_str(), a_SizeX), cFile::fmWrite))
	{
		for (size_t y = 0; y < a_SizeY; y++)
		{
			for (size_t z = 0; z < a_SizeZ; z++)
			{
				size_t idx = y * a_SizeX + z * a_SizeX * a_SizeY;
				unsigned char buf[BUF_SIZE];
				for (size_t x = 0; x < a_SizeX; x++)
				{
					buf[x] = static_cast<unsigned char>(Clamp(static_cast<int>(128 + a_Coeff * a_Noise[idx++]), 0, 255));
				}
				f2.Write(buf, a_SizeX);
			}  // for z
			unsigned char buf[BUF_SIZE];
			memset(buf, 0, a_SizeX);
			f2.Write(buf, a_SizeX);
		}  // for y
	}  // if (XZ file open)
}





void Debug2DNoise(const NOISE_DATATYPE * a_Noise, size_t a_SizeX, size_t a_SizeY, const AString & a_FileNameBase, NOISE_DATATYPE a_Coeff)
{
	const int BUF_SIZE = 512;
	ASSERT(a_SizeX <= BUF_SIZE);  // Just stretch it, if needed

	cFile f1;
	if (f1.Open(Printf("%s (%zu).grab", a_FileNameBase.c_str(), a_SizeX), cFile::fmWrite))
	{
		for (size_t y = 0; y < a_SizeY; y++)
		{
			size_t idx = y * a_SizeX;
			unsigned char buf[BUF_SIZE];
			for (size_t x = 0; x < a_SizeX; x++)
			{
				buf[x] = static_cast<unsigned char>(Clamp(static_cast<int>(128 + a_Coeff * a_Noise[idx++]), 0, 255));
			}
			f1.Write(buf, a_SizeX);
		}  // for y
	}  // if (file open)
}





////////////////////////////////////////////////////////////////////////////////
// cCubicCell2D:

class cCubicCell2D
{
public:
	cCubicCell2D(
		const cNoise & a_Noise,    ///< Noise to use for generating the random values
		NOISE_DATATYPE * a_Array,  ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,  ///< Count of the array, in each direction
		const NOISE_DATATYPE * a_FracX,  ///< Pointer to the array that stores the X fractional values
		const NOISE_DATATYPE * a_FracY   ///< Pointer to the attay that stores the Y fractional values
	);

	/** Uses current m_WorkRnds[] to generate part of the array */
	void Generate(
		int a_FromX, int a_ToX,
		int a_FromY, int a_ToY
	);

	/** Initializes m_WorkRnds[] with the specified Floor values */
	void InitWorkRnds(int a_FloorX, int a_FloorY);

	/** Updates m_WorkRnds[] for the new Floor values. */
	void Move(int a_NewFloorX, int a_NewFloorY);

protected:
	typedef NOISE_DATATYPE Workspace[4][4];

	const cNoise & m_Noise;

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
	const cNoise & a_Noise,    ///< Noise to use for generating the random values
	NOISE_DATATYPE * a_Array,  ///< Array to generate into [x + a_SizeX * y]
	int a_SizeX, int a_SizeY,  ///< Count of the array, in each direction
	const NOISE_DATATYPE * a_FracX,  ///< Pointer to the array that stores the X fractional values
	const NOISE_DATATYPE * a_FracY   ///< Pointer to the attay that stores the Y fractional values
) :
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





void cCubicCell2D::Generate(
	int a_FromX, int a_ToX,
	int a_FromY, int a_ToY
)
{
	for (int y = a_FromY; y < a_ToY; y++)
	{
		NOISE_DATATYPE Interp[4];
		NOISE_DATATYPE FracY = m_FracY[y];
		Interp[0] = cNoise::CubicInterpolate((*m_WorkRnds)[0][0], (*m_WorkRnds)[0][1], (*m_WorkRnds)[0][2], (*m_WorkRnds)[0][3], FracY);
		Interp[1] = cNoise::CubicInterpolate((*m_WorkRnds)[1][0], (*m_WorkRnds)[1][1], (*m_WorkRnds)[1][2], (*m_WorkRnds)[1][3], FracY);
		Interp[2] = cNoise::CubicInterpolate((*m_WorkRnds)[2][0], (*m_WorkRnds)[2][1], (*m_WorkRnds)[2][2], (*m_WorkRnds)[2][3], FracY);
		Interp[3] = cNoise::CubicInterpolate((*m_WorkRnds)[3][0], (*m_WorkRnds)[3][1], (*m_WorkRnds)[3][2], (*m_WorkRnds)[3][3], FracY);
		int idx = y * m_SizeX + a_FromX;
		for (int x = a_FromX; x < a_ToX; x++)
		{
			m_Array[idx++] = cNoise::CubicInterpolate(Interp[0], Interp[1], Interp[2], Interp[3], m_FracX[x]);
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
			(*m_WorkRnds)[x][y] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise2D(cx, cy));
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
				(*m_WorkRnds)[x][y] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise2D(cx, cy));
			}
		}
	}
	m_CurFloorX = a_NewFloorX;
	m_CurFloorY = a_NewFloorY;
}





////////////////////////////////////////////////////////////////////////////////
// cCubicCell3D:

class cCubicCell3D
{
public:
	cCubicCell3D(
		const cNoise & a_Noise,                 ///< Noise to use for generating the random values
		NOISE_DATATYPE * a_Array,               ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY, int a_SizeZ,  ///< Count of the array, in each direction
		const NOISE_DATATYPE * a_FracX,         ///< Pointer to the array that stores the X fractional values
		const NOISE_DATATYPE * a_FracY,         ///< Pointer to the attay that stores the Y fractional values
		const NOISE_DATATYPE * a_FracZ          ///< Pointer to the array that stores the Z fractional values
	);

	/** Uses current m_WorkRnds[] to generate part of the array */
	void Generate(
		int a_FromX, int a_ToX,
		int a_FromY, int a_ToY,
		int a_FromZ, int a_ToZ
	);

	/** Initializes m_WorkRnds[] with the specified Floor values */
	void InitWorkRnds(int a_FloorX, int a_FloorY, int a_FloorZ);

	/** Updates m_WorkRnds[] for the new Floor values. */
	void Move(int a_NewFloorX, int a_NewFloorY, int a_NewFloorZ);

protected:
	typedef NOISE_DATATYPE Workspace[4][4][4];

	const cNoise & m_Noise;

	Workspace * m_WorkRnds;  ///< The current random values; points to either m_Workspace1 or m_Workspace2 (doublebuffering)
	Workspace m_Workspace1;  ///< Buffer 1 for workspace doublebuffering, used in Move()
	Workspace m_Workspace2;  ///< Buffer 2 for workspace doublebuffering, used in Move()
	int m_CurFloorX;
	int m_CurFloorY;
	int m_CurFloorZ;

	NOISE_DATATYPE * m_Array;
	int m_SizeX, m_SizeY, m_SizeZ;
	const NOISE_DATATYPE * m_FracX;
	const NOISE_DATATYPE * m_FracY;
	const NOISE_DATATYPE * m_FracZ;
} ;





cCubicCell3D::cCubicCell3D(
	const cNoise & a_Noise,                 ///< Noise to use for generating the random values
	NOISE_DATATYPE * a_Array,               ///< Array to generate into [x + a_SizeX * y]
	int a_SizeX, int a_SizeY, int a_SizeZ,  ///< Count of the array, in each direction
	const NOISE_DATATYPE * a_FracX,         ///< Pointer to the array that stores the X fractional values
	const NOISE_DATATYPE * a_FracY,         ///< Pointer to the attay that stores the Y fractional values
	const NOISE_DATATYPE * a_FracZ          ///< Pointer to the array that stores the Z fractional values
) :
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





void cCubicCell3D::Generate(
	int a_FromX, int a_ToX,
	int a_FromY, int a_ToY,
	int a_FromZ, int a_ToZ
)
{
	for (int z = a_FromZ; z < a_ToZ; z++)
	{
		int idxZ = z * m_SizeX * m_SizeY;
		NOISE_DATATYPE Interp2[4][4];
		NOISE_DATATYPE FracZ = m_FracZ[z];
		for (int x = 0; x < 4; x++)
		{
			for (int y = 0; y < 4; y++)
			{
				Interp2[x][y] = cNoise::CubicInterpolate((*m_WorkRnds)[x][y][0], (*m_WorkRnds)[x][y][1], (*m_WorkRnds)[x][y][2], (*m_WorkRnds)[x][y][3], FracZ);
			}
		}
		for (int y = a_FromY; y < a_ToY; y++)
		{
			NOISE_DATATYPE Interp[4];
			NOISE_DATATYPE FracY = m_FracY[y];
			Interp[0] = cNoise::CubicInterpolate(Interp2[0][0], Interp2[0][1], Interp2[0][2], Interp2[0][3], FracY);
			Interp[1] = cNoise::CubicInterpolate(Interp2[1][0], Interp2[1][1], Interp2[1][2], Interp2[1][3], FracY);
			Interp[2] = cNoise::CubicInterpolate(Interp2[2][0], Interp2[2][1], Interp2[2][2], Interp2[2][3], FracY);
			Interp[3] = cNoise::CubicInterpolate(Interp2[3][0], Interp2[3][1], Interp2[3][2], Interp2[3][3], FracY);
			int idx = idxZ + y * m_SizeX + a_FromX;
			for (int x = a_FromX; x < a_ToX; x++)
			{
				m_Array[idx++] = cNoise::CubicInterpolate(Interp[0], Interp[1], Interp[2], Interp[3], m_FracX[x]);
			}  // for x
		}  // for y
	}  // for z
}





void cCubicCell3D::InitWorkRnds(int a_FloorX, int a_FloorY, int a_FloorZ)
{
	m_CurFloorX = a_FloorX;
	m_CurFloorY = a_FloorY;
	m_CurFloorZ = a_FloorZ;
	for (int x = 0; x < 4; x++)
	{
		int cx = a_FloorX + x - 1;
		for (int y = 0; y < 4; y++)
		{
			int cy = a_FloorY + y - 1;
			for (int z = 0; z < 4; z++)
			{
				int cz = a_FloorZ + z - 1;
				(*m_WorkRnds)[x][y][z] = static_cast<NOISE_DATATYPE>(m_Noise.IntNoise3D(cx, cy, cz));
			}
		}
	}
}





void cCubicCell3D::Move(int a_NewFloorX, int a_NewFloorY, int a_NewFloorZ)
{
	// Swap the doublebuffer:
	int OldFloorX = m_CurFloorX;
	int OldFloorY = m_CurFloorY;
	int OldFloorZ = m_CurFloorZ;
	Workspace * OldWorkRnds = m_WorkRnds;
	m_WorkRnds = (m_WorkRnds == &m_Workspace1) ? &m_Workspace2 : &m_Workspace1;

	// Reuse as much of the old workspace as possible:
	int DiffX = OldFloorX - a_NewFloorX;
	int DiffY = OldFloorY - a_NewFloorY;
	int DiffZ = OldFloorZ - a_NewFloorZ;
	for (int x = 0; x < 4; x++)
	{
		int cx = a_NewFloorX + x - 1;
		int OldX = x - DiffX;  // Where would this X be in the old grid?
		for (int y = 0; y < 4; y++)
		{
			int cy = a_NewFloorY + y - 1;
			int OldY = y - DiffY;  // Where would this Y be in the old grid?
			for (int z = 0; z < 4; z++)
			{
				int cz = a_NewFloorZ + z - 1;
				int OldZ = z - DiffZ;
				if ((OldX >= 0) && (OldX < 4) && (OldY >= 0) && (OldY < 4) && (OldZ >= 0) && (OldZ < 4))
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





////////////////////////////////////////////////////////////////////////////////
// cNoise:

cNoise::cNoise(int a_Seed) :
	m_Seed(a_Seed)
{
}





cNoise::cNoise(const cNoise & a_Noise) :
	m_Seed(a_Noise.m_Seed)
{
}





NOISE_DATATYPE cNoise::LinearNoise1D(NOISE_DATATYPE a_X) const
{
	int BaseX = FAST_FLOOR(a_X);
	NOISE_DATATYPE FracX = a_X - BaseX;
	return LinearInterpolate(IntNoise1D(BaseX), IntNoise1D(BaseX + 1), FracX);
}





NOISE_DATATYPE cNoise::CosineNoise1D(NOISE_DATATYPE a_X) const
{
	int BaseX = FAST_FLOOR(a_X);
	NOISE_DATATYPE FracX = a_X - BaseX;
	return CosineInterpolate(IntNoise1D(BaseX), IntNoise1D(BaseX + 1), FracX);
}





NOISE_DATATYPE cNoise::CubicNoise1D(NOISE_DATATYPE a_X) const
{
	int BaseX = FAST_FLOOR(a_X);
	NOISE_DATATYPE FracX = a_X - BaseX;
	return CubicInterpolate(IntNoise1D(BaseX - 1), IntNoise1D(BaseX), IntNoise1D(BaseX + 1), IntNoise1D(BaseX + 2), FracX);
}





NOISE_DATATYPE cNoise::SmoothNoise1D(int a_X) const
{
	return IntNoise1D(a_X) / 2 + IntNoise1D(a_X - 1) / 4 + IntNoise1D(a_X + 1) / 4;
}





NOISE_DATATYPE cNoise::CubicNoise2D(NOISE_DATATYPE a_X, NOISE_DATATYPE a_Y) const
{
	const int BaseX = FAST_FLOOR(a_X);
	const int BaseY = FAST_FLOOR(a_Y);

	const NOISE_DATATYPE points[4][4] =
	{
		{ IntNoise2D(BaseX - 1, BaseY - 1), IntNoise2D(BaseX, BaseY - 1), IntNoise2D(BaseX + 1, BaseY - 1), IntNoise2D(BaseX + 2, BaseY - 1), },
		{ IntNoise2D(BaseX - 1, BaseY),     IntNoise2D(BaseX, BaseY),     IntNoise2D(BaseX + 1, BaseY),     IntNoise2D(BaseX + 2, BaseY),     },
		{ IntNoise2D(BaseX - 1, BaseY + 1), IntNoise2D(BaseX, BaseY + 1), IntNoise2D(BaseX + 1, BaseY + 1), IntNoise2D(BaseX + 2, BaseY + 1), },
		{ IntNoise2D(BaseX - 1, BaseY + 2), IntNoise2D(BaseX, BaseY + 2), IntNoise2D(BaseX + 1, BaseY + 2), IntNoise2D(BaseX + 2, BaseY + 2), },
	};

	const NOISE_DATATYPE FracX = a_X - BaseX;
	const NOISE_DATATYPE interp1 = CubicInterpolate(points[0][0], points[0][1], points[0][2], points[0][3], FracX);
	const NOISE_DATATYPE interp2 = CubicInterpolate(points[1][0], points[1][1], points[1][2], points[1][3], FracX);
	const NOISE_DATATYPE interp3 = CubicInterpolate(points[2][0], points[2][1], points[2][2], points[2][3], FracX);
	const NOISE_DATATYPE interp4 = CubicInterpolate(points[3][0], points[3][1], points[3][2], points[3][3], FracX);


	const NOISE_DATATYPE FracY = a_Y - BaseY;
	return CubicInterpolate(interp1, interp2, interp3, interp4, FracY);
}





NOISE_DATATYPE cNoise::CubicNoise3D(NOISE_DATATYPE a_X, NOISE_DATATYPE a_Y, NOISE_DATATYPE a_Z) const
{
	const int BaseX = FAST_FLOOR(a_X);
	const int BaseY = FAST_FLOOR(a_Y);
	const int BaseZ = FAST_FLOOR(a_Z);

	const NOISE_DATATYPE points1[4][4] =
	{
		{ IntNoise3D(BaseX - 1, BaseY - 1, BaseZ - 1), IntNoise3D(BaseX, BaseY - 1, BaseZ - 1), IntNoise3D(BaseX + 1, BaseY - 1, BaseZ - 1), IntNoise3D(BaseX + 2, BaseY - 1, BaseZ - 1), },
		{ IntNoise3D(BaseX - 1, BaseY,     BaseZ - 1), IntNoise3D(BaseX, BaseY,     BaseZ - 1), IntNoise3D(BaseX + 1, BaseY,     BaseZ - 1), IntNoise3D(BaseX + 2, BaseY,     BaseZ - 1), },
		{ IntNoise3D(BaseX - 1, BaseY + 1, BaseZ - 1), IntNoise3D(BaseX, BaseY + 1, BaseZ - 1), IntNoise3D(BaseX + 1, BaseY + 1, BaseZ - 1), IntNoise3D(BaseX + 2, BaseY + 1, BaseZ - 1), },
		{ IntNoise3D(BaseX - 1, BaseY + 2, BaseZ - 1), IntNoise3D(BaseX, BaseY + 2, BaseZ - 1), IntNoise3D(BaseX + 1, BaseY + 2, BaseZ - 1), IntNoise3D(BaseX + 2, BaseY + 2, BaseZ - 1), },
	};

	const NOISE_DATATYPE FracX = (a_X) - BaseX;
	const NOISE_DATATYPE x1interp1 = CubicInterpolate(points1[0][0], points1[0][1], points1[0][2], points1[0][3], FracX);
	const NOISE_DATATYPE x1interp2 = CubicInterpolate(points1[1][0], points1[1][1], points1[1][2], points1[1][3], FracX);
	const NOISE_DATATYPE x1interp3 = CubicInterpolate(points1[2][0], points1[2][1], points1[2][2], points1[2][3], FracX);
	const NOISE_DATATYPE x1interp4 = CubicInterpolate(points1[3][0], points1[3][1], points1[3][2], points1[3][3], FracX);

	const NOISE_DATATYPE points2[4][4] =
	{
		{ IntNoise3D(BaseX - 1, BaseY - 1, BaseZ), IntNoise3D(BaseX, BaseY - 1, BaseZ), IntNoise3D(BaseX + 1, BaseY - 1, BaseZ), IntNoise3D(BaseX + 2, BaseY - 1, BaseZ), },
		{ IntNoise3D(BaseX - 1, BaseY,     BaseZ), IntNoise3D(BaseX, BaseY,     BaseZ), IntNoise3D(BaseX + 1, BaseY,     BaseZ), IntNoise3D(BaseX + 2, BaseY,     BaseZ), },
		{ IntNoise3D(BaseX - 1, BaseY + 1, BaseZ), IntNoise3D(BaseX, BaseY + 1, BaseZ), IntNoise3D(BaseX + 1, BaseY + 1, BaseZ), IntNoise3D(BaseX + 2, BaseY + 1, BaseZ), },
		{ IntNoise3D(BaseX - 1, BaseY + 2, BaseZ), IntNoise3D(BaseX, BaseY + 2, BaseZ), IntNoise3D(BaseX + 1, BaseY + 2, BaseZ), IntNoise3D(BaseX + 2, BaseY + 2, BaseZ), },
	};

	const NOISE_DATATYPE x2interp1 = CubicInterpolate(points2[0][0], points2[0][1], points2[0][2], points2[0][3], FracX);
	const NOISE_DATATYPE x2interp2 = CubicInterpolate(points2[1][0], points2[1][1], points2[1][2], points2[1][3], FracX);
	const NOISE_DATATYPE x2interp3 = CubicInterpolate(points2[2][0], points2[2][1], points2[2][2], points2[2][3], FracX);
	const NOISE_DATATYPE x2interp4 = CubicInterpolate(points2[3][0], points2[3][1], points2[3][2], points2[3][3], FracX);

	const NOISE_DATATYPE points3[4][4] =
	{
		{ IntNoise3D(BaseX - 1, BaseY - 1, BaseZ + 1), IntNoise3D(BaseX, BaseY - 1, BaseZ + 1), IntNoise3D(BaseX + 1, BaseY - 1, BaseZ + 1), IntNoise3D(BaseX + 2, BaseY - 1, BaseZ + 1), },
		{ IntNoise3D(BaseX - 1, BaseY,	   BaseZ + 1), IntNoise3D(BaseX, BaseY,     BaseZ + 1), IntNoise3D(BaseX + 1, BaseY,     BaseZ + 1), IntNoise3D(BaseX + 2, BaseY,     BaseZ + 1), },
		{ IntNoise3D(BaseX - 1, BaseY + 1, BaseZ + 1), IntNoise3D(BaseX, BaseY + 1, BaseZ + 1), IntNoise3D(BaseX + 1, BaseY + 1, BaseZ + 1), IntNoise3D(BaseX + 2, BaseY + 1, BaseZ + 1), },
		{ IntNoise3D(BaseX - 1, BaseY + 2, BaseZ + 1), IntNoise3D(BaseX, BaseY + 2, BaseZ + 1), IntNoise3D(BaseX + 1, BaseY + 2, BaseZ + 1), IntNoise3D(BaseX + 2, BaseY + 2, BaseZ + 1), },
	};

	const NOISE_DATATYPE x3interp1 = CubicInterpolate(points3[0][0], points3[0][1], points3[0][2], points3[0][3], FracX);
	const NOISE_DATATYPE x3interp2 = CubicInterpolate(points3[1][0], points3[1][1], points3[1][2], points3[1][3], FracX);
	const NOISE_DATATYPE x3interp3 = CubicInterpolate(points3[2][0], points3[2][1], points3[2][2], points3[2][3], FracX);
	const NOISE_DATATYPE x3interp4 = CubicInterpolate(points3[3][0], points3[3][1], points3[3][2], points3[3][3], FracX);

	const NOISE_DATATYPE points4[4][4] =
	{
		{ IntNoise3D(BaseX - 1, BaseY - 1, BaseZ + 2), IntNoise3D(BaseX, BaseY - 1, BaseZ + 2), IntNoise3D(BaseX + 1, BaseY - 1, BaseZ + 2), IntNoise3D(BaseX + 2, BaseY - 1, BaseZ + 2), },
		{ IntNoise3D(BaseX - 1, BaseY,	   BaseZ + 2), IntNoise3D(BaseX, BaseY,     BaseZ + 2), IntNoise3D(BaseX + 1, BaseY,     BaseZ + 2), IntNoise3D(BaseX + 2, BaseY,     BaseZ + 2), },
		{ IntNoise3D(BaseX - 1, BaseY + 1, BaseZ + 2), IntNoise3D(BaseX, BaseY + 1, BaseZ + 2), IntNoise3D(BaseX + 1, BaseY + 1, BaseZ + 2), IntNoise3D(BaseX + 2, BaseY + 1, BaseZ + 2), },
		{ IntNoise3D(BaseX - 1, BaseY + 2, BaseZ + 2), IntNoise3D(BaseX, BaseY + 2, BaseZ + 2), IntNoise3D(BaseX + 1, BaseY + 2, BaseZ + 2), IntNoise3D(BaseX + 2, BaseY + 2, BaseZ + 2), },
	};

	const NOISE_DATATYPE x4interp1 = CubicInterpolate(points4[0][0], points4[0][1], points4[0][2], points4[0][3], FracX);
	const NOISE_DATATYPE x4interp2 = CubicInterpolate(points4[1][0], points4[1][1], points4[1][2], points4[1][3], FracX);
	const NOISE_DATATYPE x4interp3 = CubicInterpolate(points4[2][0], points4[2][1], points4[2][2], points4[2][3], FracX);
	const NOISE_DATATYPE x4interp4 = CubicInterpolate(points4[3][0], points4[3][1], points4[3][2], points4[3][3], FracX);

	const NOISE_DATATYPE FracY = (a_Y) - BaseY;
	const NOISE_DATATYPE yinterp1 = CubicInterpolate(x1interp1, x1interp2, x1interp3, x1interp4, FracY);
	const NOISE_DATATYPE yinterp2 = CubicInterpolate(x2interp1, x2interp2, x2interp3, x2interp4, FracY);
	const NOISE_DATATYPE yinterp3 = CubicInterpolate(x3interp1, x3interp2, x3interp3, x3interp4, FracY);
	const NOISE_DATATYPE yinterp4 = CubicInterpolate(x4interp1, x4interp2, x4interp3, x4interp4, FracY);

	const NOISE_DATATYPE FracZ = (a_Z) - BaseZ;
	return CubicInterpolate(yinterp1, yinterp2, yinterp3, yinterp4, FracZ);
}





////////////////////////////////////////////////////////////////////////////////
// cCubicNoise:

cCubicNoise::cCubicNoise(int a_Seed) :
	m_Noise(a_Seed)
{
}





void cCubicNoise::Generate2D(
	NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
	int a_SizeX, int a_SizeY,                        ///< Size of the array (num doubles), in each direction
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





void cCubicNoise::Generate3D(
	NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
	int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Size of the array (num doubles), in each direction
	NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
	NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
	NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ   ///< Noise-space coords of the array in the Y direction
) const
{
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

	cCubicCell3D Cell(
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





void cCubicNoise::CalcFloorFrac(
	int a_Size,
	NOISE_DATATYPE a_Start, NOISE_DATATYPE a_End,
	int * a_Floor, NOISE_DATATYPE * a_Frac,
	int * a_Same, int & a_NumSame
) const
{
	ASSERT(a_Size > 0);

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





////////////////////////////////////////////////////////////////////////////////
// cImprovedNoise:

cImprovedNoise::cImprovedNoise(int a_Seed)
{
	// Initialize the permutations with identity:
	for (int i = 0; i < 256; i++)
	{
		m_Perm[i] = i;
	}

	// Randomize the permutation table - swap each element with a random other element:
	cNoise noise(a_Seed);
	for (int i = 0; i < 256; i++)
	{
		int rnd = (noise.IntNoise1DInt(i) / 7) % 256;
		std::swap(m_Perm[i], m_Perm[rnd]);
	}

	// Copy the lower 256 entries into upper 256 entries:
	for (int i = 0; i < 256; i++)
	{
		m_Perm[i + 256] = m_Perm[i];
	}
}





void cImprovedNoise::Generate2D(
	NOISE_DATATYPE * a_Array,
	int a_SizeX, int a_SizeY,
	NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,
	NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY
) const
{
	size_t idx = 0;
	for (int y = 0; y < a_SizeY; y++)
	{
		NOISE_DATATYPE ratioY = static_cast<NOISE_DATATYPE>(y) / (a_SizeY - 1);
		NOISE_DATATYPE noiseY = Lerp(a_StartY, a_EndY, ratioY);
		int noiseYInt = FAST_FLOOR(noiseY);
		int yCoord = noiseYInt & 255;
		NOISE_DATATYPE noiseYFrac = noiseY - noiseYInt;
		NOISE_DATATYPE fadeY = Fade(noiseYFrac);
		for (int x = 0; x < a_SizeX; x++)
		{
			NOISE_DATATYPE ratioX = static_cast<NOISE_DATATYPE>(x) / (a_SizeX - 1);
			NOISE_DATATYPE noiseX = Lerp(a_StartX, a_EndX, ratioX);
			int noiseXInt = FAST_FLOOR(noiseX);
			int xCoord = noiseXInt & 255;
			NOISE_DATATYPE noiseXFrac = noiseX - noiseXInt;
			NOISE_DATATYPE fadeX = Fade(noiseXFrac);

			// Hash the coordinates:
			int A  = m_Perm[xCoord] + yCoord;
			int AA = m_Perm[A];
			int AB = m_Perm[A + 1];
			int B  = m_Perm[xCoord + 1] + yCoord;
			int BA = m_Perm[B];
			int BB = m_Perm[B + 1];

			// Lerp the gradients:
			a_Array[idx++] = Lerp(
				Lerp(Grad(m_Perm[AA], noiseXFrac, noiseYFrac,     0), Grad(m_Perm[BA], noiseXFrac - 1, noiseYFrac,     0), fadeX),
				Lerp(Grad(m_Perm[AB], noiseXFrac, noiseYFrac - 1, 0), Grad(m_Perm[BB], noiseXFrac - 1, noiseYFrac - 1, 0), fadeX),
				fadeY
			);
		}  // for x
	}  // for y
}





void cImprovedNoise::Generate3D(
	NOISE_DATATYPE * a_Array,
	int a_SizeX, int a_SizeY, int a_SizeZ,
	NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,
	NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,
	NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ
) const
{
	size_t idx = 0;
	for (int z = 0; z < a_SizeZ; z++)
	{
		NOISE_DATATYPE ratioZ = static_cast<NOISE_DATATYPE>(z) / (a_SizeZ - 1);
		NOISE_DATATYPE noiseZ = Lerp(a_StartZ, a_EndZ, ratioZ);
		int noiseZInt = FAST_FLOOR(noiseZ);
		int zCoord = noiseZInt & 255;
		NOISE_DATATYPE noiseZFrac = noiseZ - noiseZInt;
		NOISE_DATATYPE fadeZ = Fade(noiseZFrac);
		for (int y = 0; y < a_SizeY; y++)
		{
			NOISE_DATATYPE ratioY = static_cast<NOISE_DATATYPE>(y) / (a_SizeY - 1);
			NOISE_DATATYPE noiseY = Lerp(a_StartY, a_EndY, ratioY);
			int noiseYInt = FAST_FLOOR(noiseY);
			int yCoord = noiseYInt & 255;
			NOISE_DATATYPE noiseYFrac = noiseY - noiseYInt;
			NOISE_DATATYPE fadeY = Fade(noiseYFrac);
			for (int x = 0; x < a_SizeX; x++)
			{
				NOISE_DATATYPE ratioX = static_cast<NOISE_DATATYPE>(x) / (a_SizeX - 1);
				NOISE_DATATYPE noiseX = Lerp(a_StartX, a_EndX, ratioX);
				int noiseXInt = FAST_FLOOR(noiseX);
				int xCoord = noiseXInt & 255;
				NOISE_DATATYPE noiseXFrac = noiseX - noiseXInt;
				NOISE_DATATYPE fadeX = Fade(noiseXFrac);

				// Hash the coordinates:
				int A  = m_Perm[xCoord] + yCoord;
				int AA = m_Perm[A] + zCoord;
				int AB = m_Perm[A + 1] + zCoord;
				int B  = m_Perm[xCoord + 1] + yCoord;
				int BA = m_Perm[B] + zCoord;
				int BB = m_Perm[B + 1] + zCoord;

				// Lerp the gradients:
				// TODO: This may be optimized by swapping the coords and recalculating most lerps only "once every x"
				a_Array[idx++] = Lerp(
					Lerp(
						Lerp(Grad(m_Perm[AA], noiseXFrac, noiseYFrac,     noiseZFrac), Grad(m_Perm[BA], noiseXFrac - 1, noiseYFrac,     noiseZFrac), fadeX),
						Lerp(Grad(m_Perm[AB], noiseXFrac, noiseYFrac - 1, noiseZFrac), Grad(m_Perm[BB], noiseXFrac - 1, noiseYFrac - 1, noiseZFrac), fadeX),
						fadeY
					),
					Lerp(
						Lerp(Grad(m_Perm[AA + 1], noiseXFrac, noiseYFrac,     noiseZFrac - 1), Grad(m_Perm[BA + 1], noiseXFrac - 1, noiseYFrac,     noiseZFrac - 1), fadeX),
						Lerp(Grad(m_Perm[AB + 1], noiseXFrac, noiseYFrac - 1, noiseZFrac - 1), Grad(m_Perm[BB + 1], noiseXFrac - 1, noiseYFrac - 1, noiseZFrac - 1), fadeX),
						fadeY
					),
					fadeZ
				);
			}  // for x
		}  // for y
	}  // for z
}





NOISE_DATATYPE cImprovedNoise::GetValueAt(int a_X, int a_Y, int a_Z)
{
	// Hash the coordinates:
	a_X = a_X & 255;
	a_Y = a_Y & 255;
	a_Z = a_Z & 255;
	int A  = m_Perm[a_X] + a_Y;
	int AA = m_Perm[A] + a_Z;

	return Grad(m_Perm[AA], 1, 1, 1);
}





