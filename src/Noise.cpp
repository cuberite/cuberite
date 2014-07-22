
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Noise.h"

#define FAST_FLOOR(x) (((x) < 0) ? (((int)x) - 1) : ((int)x))





////////////////////////////////////////////////////////////////////////////////
// Globals:

void Debug3DNoise(const NOISE_DATATYPE * a_Noise, int a_SizeX, int a_SizeY, int a_SizeZ, const AString & a_FileNameBase)
{
	const int BUF_SIZE = 512;
	ASSERT(a_SizeX <= BUF_SIZE);  // Just stretch it, if needed
	
	// Save in XY cuts:
	cFile f1;
	if (f1.Open(Printf("%s_XY (%d).grab", a_FileNameBase.c_str(), a_SizeX), cFile::fmWrite))
	{
		for (int z = 0; z < a_SizeZ; z++)
		{
			for (int y = 0; y < a_SizeY; y++)
			{
				int idx = y * a_SizeX + z * a_SizeX * a_SizeY;
				unsigned char buf[BUF_SIZE];
				for (int x = 0; x < a_SizeX; x++)
				{
					buf[x] = (unsigned char)(std::min(255, std::max(0, (int)(128 + 32 * a_Noise[idx++]))));
				}
				f1.Write(buf, a_SizeX);
			}  // for y
			unsigned char buf[BUF_SIZE];
			memset(buf, 0, a_SizeX);
			f1.Write(buf, a_SizeX);
		}  // for z
	}  // if (XY file open)

	cFile f2;
	if (f2.Open(Printf("%s_XZ (%d).grab", a_FileNameBase.c_str(), a_SizeX), cFile::fmWrite))
	{
		for (int y = 0; y < a_SizeY; y++)
		{
			for (int z = 0; z < a_SizeZ; z++)
			{
				int idx = y * a_SizeX + z * a_SizeX * a_SizeY;
				unsigned char buf[BUF_SIZE];
				for (int x = 0; x < a_SizeX; x++)
				{
					buf[x] = (unsigned char)(std::min(255, std::max(0, (int)(128 + 32 * a_Noise[idx++]))));
				}
				f2.Write(buf, a_SizeX);
			}  // for z
			unsigned char buf[BUF_SIZE];
			memset(buf, 0, a_SizeX);
			f2.Write(buf, a_SizeX);
		}  // for y
	}  // if (XZ file open)
}





void Debug2DNoise(const NOISE_DATATYPE * a_Noise, int a_SizeX, int a_SizeY, const AString & a_FileNameBase)
{
	const int BUF_SIZE = 512;
	ASSERT(a_SizeX <= BUF_SIZE);  // Just stretch it, if needed
	
	cFile f1;
	if (f1.Open(Printf("%s (%d).grab", a_FileNameBase.c_str(), a_SizeX), cFile::fmWrite))
	{
		for (int y = 0; y < a_SizeY; y++)
		{
			int idx = y * a_SizeX;
			unsigned char buf[BUF_SIZE];
			for (int x = 0; x < a_SizeX; x++)
			{
				buf[x] = (unsigned char)(std::min(255, std::max(0, (int)(128 + 32 * a_Noise[idx++]))));
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
	
	/// Uses current m_WorkRnds[] to generate part of the array
	void Generate(
		int a_FromX, int a_ToX,
		int a_FromY, int a_ToY,
		int a_FromZ, int a_ToZ
	);
	
	/// Initializes m_WorkRnds[] with the specified Floor values
	void InitWorkRnds(int a_FloorX, int a_FloorY, int a_FloorZ);
	
	/// Updates m_WorkRnds[] for the new Floor values.
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
				(*m_WorkRnds)[x][y][z] = (NOISE_DATATYPE)m_Noise.IntNoise3D(cx, cy, cz);
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
					(*m_WorkRnds)[x][y][z] = (NOISE_DATATYPE)m_Noise.IntNoise3D(cx, cy, cz);
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
	const NOISE_DATATYPE x1interp1 = CubicInterpolate( points1[0][0], points1[0][1], points1[0][2], points1[0][3], FracX);
	const NOISE_DATATYPE x1interp2 = CubicInterpolate( points1[1][0], points1[1][1], points1[1][2], points1[1][3], FracX);
	const NOISE_DATATYPE x1interp3 = CubicInterpolate( points1[2][0], points1[2][1], points1[2][2], points1[2][3], FracX);
	const NOISE_DATATYPE x1interp4 = CubicInterpolate( points1[3][0], points1[3][1], points1[3][2], points1[3][3], FracX);

	const NOISE_DATATYPE points2[4][4] =
	{
		{ IntNoise3D(BaseX - 1, BaseY - 1, BaseZ), IntNoise3D(BaseX, BaseY - 1, BaseZ), IntNoise3D(BaseX + 1, BaseY - 1, BaseZ), IntNoise3D(BaseX + 2, BaseY - 1, BaseZ), },
		{ IntNoise3D(BaseX - 1, BaseY,     BaseZ), IntNoise3D(BaseX, BaseY,     BaseZ), IntNoise3D(BaseX + 1, BaseY,     BaseZ), IntNoise3D(BaseX + 2, BaseY,     BaseZ), },
		{ IntNoise3D(BaseX - 1, BaseY + 1, BaseZ), IntNoise3D(BaseX, BaseY + 1, BaseZ), IntNoise3D(BaseX + 1, BaseY + 1, BaseZ), IntNoise3D(BaseX + 2, BaseY + 1, BaseZ), },
		{ IntNoise3D(BaseX - 1, BaseY + 2, BaseZ), IntNoise3D(BaseX, BaseY + 2, BaseZ), IntNoise3D(BaseX + 1, BaseY + 2, BaseZ), IntNoise3D(BaseX + 2, BaseY + 2, BaseZ), },
	};

	const NOISE_DATATYPE x2interp1 = CubicInterpolate( points2[0][0], points2[0][1], points2[0][2], points2[0][3], FracX);
	const NOISE_DATATYPE x2interp2 = CubicInterpolate( points2[1][0], points2[1][1], points2[1][2], points2[1][3], FracX);
	const NOISE_DATATYPE x2interp3 = CubicInterpolate( points2[2][0], points2[2][1], points2[2][2], points2[2][3], FracX);
	const NOISE_DATATYPE x2interp4 = CubicInterpolate( points2[3][0], points2[3][1], points2[3][2], points2[3][3], FracX);

	const NOISE_DATATYPE points3[4][4] =
	{
		{ IntNoise3D( BaseX-1, BaseY-1, BaseZ+1), IntNoise3D( BaseX, BaseY-1, BaseZ+1), IntNoise3D( BaseX+1, BaseY-1, BaseZ+1), IntNoise3D( BaseX+2, BaseY-1, BaseZ + 1), },
		{ IntNoise3D( BaseX-1, BaseY,	  BaseZ+1), IntNoise3D( BaseX, BaseY,   BaseZ+1), IntNoise3D( BaseX+1, BaseY,   BaseZ+1), IntNoise3D( BaseX+2, BaseY,   BaseZ + 1), },
		{ IntNoise3D( BaseX-1, BaseY+1, BaseZ+1), IntNoise3D( BaseX, BaseY+1, BaseZ+1), IntNoise3D( BaseX+1, BaseY+1, BaseZ+1), IntNoise3D( BaseX+2, BaseY+1, BaseZ + 1), },
		{ IntNoise3D( BaseX-1, BaseY+2, BaseZ+1), IntNoise3D( BaseX, BaseY+2, BaseZ+1), IntNoise3D( BaseX+1, BaseY+2, BaseZ+1), IntNoise3D( BaseX+2, BaseY+2, BaseZ + 1), },
	};

	const NOISE_DATATYPE x3interp1 = CubicInterpolate( points3[0][0], points3[0][1], points3[0][2], points3[0][3], FracX);
	const NOISE_DATATYPE x3interp2 = CubicInterpolate( points3[1][0], points3[1][1], points3[1][2], points3[1][3], FracX);
	const NOISE_DATATYPE x3interp3 = CubicInterpolate( points3[2][0], points3[2][1], points3[2][2], points3[2][3], FracX);
	const NOISE_DATATYPE x3interp4 = CubicInterpolate( points3[3][0], points3[3][1], points3[3][2], points3[3][3], FracX);

	const NOISE_DATATYPE points4[4][4] =
	{
		{ IntNoise3D( BaseX-1, BaseY-1, BaseZ+2), IntNoise3D( BaseX, BaseY-1, BaseZ+2), IntNoise3D( BaseX+1, BaseY-1, BaseZ+2), IntNoise3D( BaseX+2, BaseY-1, BaseZ+2), },
		{ IntNoise3D( BaseX-1, BaseY,	  BaseZ+2), IntNoise3D( BaseX, BaseY,   BaseZ+2), IntNoise3D( BaseX+1, BaseY,   BaseZ+2), IntNoise3D( BaseX+2, BaseY,   BaseZ+2), },
		{ IntNoise3D( BaseX-1, BaseY+1, BaseZ+2), IntNoise3D( BaseX, BaseY+1, BaseZ+2), IntNoise3D( BaseX+1, BaseY+1, BaseZ+2), IntNoise3D( BaseX+2, BaseY+1, BaseZ+2), },
		{ IntNoise3D( BaseX-1, BaseY+2, BaseZ+2), IntNoise3D( BaseX, BaseY+2, BaseZ+2), IntNoise3D( BaseX+1, BaseY+2, BaseZ+2), IntNoise3D( BaseX+2, BaseY+2, BaseZ+2), },
	};

	const NOISE_DATATYPE x4interp1 = CubicInterpolate( points4[0][0], points4[0][1], points4[0][2], points4[0][3], FracX);
	const NOISE_DATATYPE x4interp2 = CubicInterpolate( points4[1][0], points4[1][1], points4[1][2], points4[1][3], FracX);
	const NOISE_DATATYPE x4interp3 = CubicInterpolate( points4[2][0], points4[2][1], points4[2][2], points4[2][3], FracX);
	const NOISE_DATATYPE x4interp4 = CubicInterpolate( points4[3][0], points4[3][1], points4[3][2], points4[3][3], FracX);

	const NOISE_DATATYPE FracY = (a_Y) - BaseY;
	const NOISE_DATATYPE yinterp1 = CubicInterpolate( x1interp1, x1interp2, x1interp3, x1interp4, FracY);
	const NOISE_DATATYPE yinterp2 = CubicInterpolate( x2interp1, x2interp2, x2interp3, x2interp4, FracY);
	const NOISE_DATATYPE yinterp3 = CubicInterpolate( x3interp1, x3interp2, x3interp3, x3interp4, FracY);
	const NOISE_DATATYPE yinterp4 = CubicInterpolate( x4interp1, x4interp2, x4interp3, x4interp4, FracY);

	const NOISE_DATATYPE FracZ = (a_Z) - BaseZ;
	return CubicInterpolate( yinterp1, yinterp2, yinterp3, yinterp4, FracZ);
}





////////////////////////////////////////////////////////////////////////////////
// cCubicNoise:

#ifdef _DEBUG
	int cCubicNoise::m_NumSingleX = 0;
	int cCubicNoise::m_NumSingleXY = 0;
	int cCubicNoise::m_NumSingleY = 0;
	int cCubicNoise::m_NumCalls = 0;
#endif  // _DEBUG

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
	
	#ifdef _DEBUG
		// Statistics on the noise-space coords:
		if (NumSameX == 1)
		{
			m_NumSingleX++;
			if (NumSameY == 1)
			{
				m_NumSingleXY++;
			}
		}
		if (NumSameY == 1)
		{
			m_NumSingleY++;
		}
		m_NumCalls++;
	#endif  // _DEBUG
	
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
// cPerlinNoise:

cPerlinNoise::cPerlinNoise(void) :
	m_Seed(0)
{
}





cPerlinNoise::cPerlinNoise(int a_Seed) :
	m_Seed(a_Seed)
{
}





void cPerlinNoise::SetSeed(int a_Seed)
{
	m_Seed = a_Seed;
}





void cPerlinNoise::AddOctave(float a_Frequency, float a_Amplitude)
{
	m_Octaves.push_back(cOctave(m_Seed * ((int)m_Octaves.size() + 4) * 4 + 1024, a_Frequency, a_Amplitude));
}





void cPerlinNoise::Generate2D(
	NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
	int a_SizeX, int a_SizeY,                        ///< Count of the array, in each direction
	NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
	NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
	NOISE_DATATYPE * a_Workspace                     ///< Workspace that this function can use and trash
) const
{
	if (m_Octaves.empty())
	{
		// No work to be done
		ASSERT(!"Perlin: No octaves to generate!");
		return;
	}
	
	bool ShouldFreeWorkspace = (a_Workspace == NULL);
	int ArrayCount = a_SizeX * a_SizeY;
	if (ShouldFreeWorkspace)
	{
		a_Workspace = new NOISE_DATATYPE[ArrayCount];
	}
	
	// Generate the first octave directly into array:
	const cOctave & FirstOctave = m_Octaves.front();
	
	FirstOctave.m_Noise.Generate2D(
		a_Workspace, a_SizeX, a_SizeY,
		a_StartX * FirstOctave.m_Frequency, a_EndX * FirstOctave.m_Frequency,
		a_StartY * FirstOctave.m_Frequency, a_EndY * FirstOctave.m_Frequency
	);
	NOISE_DATATYPE Amplitude = FirstOctave.m_Amplitude;
	for (int i = 0; i < ArrayCount; i++)
	{
		a_Array[i] = a_Workspace[i] * Amplitude;
	}
	
	// Add each octave:
	for (cOctaves::const_iterator itr = m_Octaves.begin() + 1, end = m_Octaves.end(); itr != end; ++itr)
	{
		// Generate cubic noise for the octave:
		itr->m_Noise.Generate2D(
			a_Workspace, a_SizeX, a_SizeY,
			a_StartX * itr->m_Frequency, a_EndX * itr->m_Frequency,
			a_StartY * itr->m_Frequency, a_EndY * itr->m_Frequency
		);
		// Add the cubic noise into the output:
		NOISE_DATATYPE Amplitude = itr->m_Amplitude;
		for (int i = 0; i < ArrayCount; i++)
		{
			a_Array[i] += a_Workspace[i] * Amplitude;
		}
	}
	
	if (ShouldFreeWorkspace)
	{
		delete[] a_Workspace;
		a_Workspace = NULL;
	}
}





void cPerlinNoise::Generate3D(
	NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y + a_SizeX * a_SizeY * z]
	int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Count of the array, in each direction
	NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
	NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
	NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ,  ///< Noise-space coords of the array in the Z direction
	NOISE_DATATYPE * a_Workspace                     ///< Workspace that this function can use and trash
) const
{
	if (m_Octaves.empty())
	{
		// No work to be done
		ASSERT(!"Perlin: No octaves to generate!");
		return;
	}
	
	bool ShouldFreeWorkspace = (a_Workspace == NULL);
	int ArrayCount = a_SizeX * a_SizeY * a_SizeZ;
	if (ShouldFreeWorkspace)
	{
		a_Workspace = new NOISE_DATATYPE[ArrayCount];
	}
	
	// Generate the first octave directly into array:
	const cOctave & FirstOctave = m_Octaves.front();

	FirstOctave.m_Noise.Generate3D(
		a_Workspace, a_SizeX, a_SizeY, a_SizeZ,
		a_StartX * FirstOctave.m_Frequency, a_EndX * FirstOctave.m_Frequency,
		a_StartY * FirstOctave.m_Frequency, a_EndY * FirstOctave.m_Frequency,
		a_StartZ * FirstOctave.m_Frequency, a_EndZ * FirstOctave.m_Frequency
	);
	NOISE_DATATYPE Amplitude = FirstOctave.m_Amplitude;
	for (int i = 0; i < ArrayCount; i++)
	{
		a_Array[i] = a_Workspace[i] * Amplitude;
	}
	
	// Add each octave:
	for (cOctaves::const_iterator itr = m_Octaves.begin() + 1, end = m_Octaves.end(); itr != end; ++itr)
	{
		// Generate cubic noise for the octave:
		itr->m_Noise.Generate3D(
			a_Workspace, a_SizeX, a_SizeY, a_SizeZ,
			a_StartX * itr->m_Frequency, a_EndX * itr->m_Frequency,
			a_StartY * itr->m_Frequency, a_EndY * itr->m_Frequency,
			a_StartZ * itr->m_Frequency, a_EndZ * itr->m_Frequency
		);
		// Add the cubic noise into the output:
		NOISE_DATATYPE Amplitude = itr->m_Amplitude;
		for (int i = 0; i < ArrayCount; i++)
		{
			a_Array[i] += a_Workspace[i] * Amplitude;
		}
	}
	
	if (ShouldFreeWorkspace)
	{
		delete[] a_Workspace;
		a_Workspace = NULL;
	}
}





////////////////////////////////////////////////////////////////////////////////
// cRidgedMultiNoise:

cRidgedMultiNoise::cRidgedMultiNoise(void) :
	m_Seed(0)
{
}





cRidgedMultiNoise::cRidgedMultiNoise(int a_Seed) :
	m_Seed(a_Seed)
{
}





void cRidgedMultiNoise::SetSeed(int a_Seed)
{
	m_Seed = a_Seed;
}





void cRidgedMultiNoise::AddOctave(float a_Frequency, float a_Amplitude)
{
	m_Octaves.push_back(cOctave(m_Seed * ((int)m_Octaves.size() + 4) * 4 + 1024, a_Frequency, a_Amplitude));
}





void cRidgedMultiNoise::Generate2D(
	NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
	int a_SizeX, int a_SizeY,                        ///< Count of the array, in each direction
	NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
	NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
	NOISE_DATATYPE * a_Workspace                     ///< Workspace that this function can use and trash
) const
{
	if (m_Octaves.empty())
	{
		// No work to be done
		ASSERT(!"RidgedMulti: No octaves to generate!");
		return;
	}
	
	bool ShouldFreeWorkspace = (a_Workspace == NULL);
	int ArrayCount = a_SizeX * a_SizeY;
	if (ShouldFreeWorkspace)
	{
		a_Workspace = new NOISE_DATATYPE[ArrayCount];
	}
	
	// Generate the first octave directly into array:
	const cOctave & FirstOctave = m_Octaves.front();
	
	FirstOctave.m_Noise.Generate2D(
		a_Workspace, a_SizeX, a_SizeY,
		a_StartX * FirstOctave.m_Frequency, a_EndX * FirstOctave.m_Frequency,
		a_StartY * FirstOctave.m_Frequency, a_EndY * FirstOctave.m_Frequency
	);
	NOISE_DATATYPE Amplitude = FirstOctave.m_Amplitude;
	for (int i = 0; i < ArrayCount; i++)
	{
		a_Array[i] = fabs(a_Workspace[i] * Amplitude);
	}
	
	// Add each octave:
	for (cOctaves::const_iterator itr = m_Octaves.begin() + 1, end = m_Octaves.end(); itr != end; ++itr)
	{
		// Generate cubic noise for the octave:
		itr->m_Noise.Generate2D(
			a_Workspace, a_SizeX, a_SizeY,
			a_StartX * itr->m_Frequency, a_EndX * itr->m_Frequency,
			a_StartY * itr->m_Frequency, a_EndY * itr->m_Frequency
		);
		// Add the cubic noise into the output:
		NOISE_DATATYPE Amplitude = itr->m_Amplitude;
		for (int i = 0; i < ArrayCount; i++)
		{
			a_Array[i] += fabs(a_Workspace[i] * Amplitude);
		}
	}
	
	if (ShouldFreeWorkspace)
	{
		delete[] a_Workspace;
		a_Workspace = NULL;
	}
}





void cRidgedMultiNoise::Generate3D(
	NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y + a_SizeX * a_SizeY * z]
	int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Count of the array, in each direction
	NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
	NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
	NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ,  ///< Noise-space coords of the array in the Z direction
	NOISE_DATATYPE * a_Workspace                     ///< Workspace that this function can use and trash
) const
{
	if (m_Octaves.empty())
	{
		// No work to be done
		ASSERT(!"RidgedMulti: No octaves to generate!");
		return;
	}
	
	bool ShouldFreeWorkspace = (a_Workspace == NULL);
	int ArrayCount = a_SizeX * a_SizeY * a_SizeZ;
	if (ShouldFreeWorkspace)
	{
		a_Workspace = new NOISE_DATATYPE[ArrayCount];
	}
	
	// Generate the first octave directly into array:
	const cOctave & FirstOctave = m_Octaves.front();

	FirstOctave.m_Noise.Generate3D(
		a_Workspace, a_SizeX, a_SizeY, a_SizeZ,
		a_StartX * FirstOctave.m_Frequency, a_EndX * FirstOctave.m_Frequency,
		a_StartY * FirstOctave.m_Frequency, a_EndY * FirstOctave.m_Frequency,
		a_StartZ * FirstOctave.m_Frequency, a_EndZ * FirstOctave.m_Frequency
	);
	NOISE_DATATYPE Amplitude = FirstOctave.m_Amplitude;
	for (int i = 0; i < ArrayCount; i++)
	{
		a_Array[i] = a_Workspace[i] * Amplitude;
	}
	
	// Add each octave:
	for (cOctaves::const_iterator itr = m_Octaves.begin() + 1, end = m_Octaves.end(); itr != end; ++itr)
	{
		// Generate cubic noise for the octave:
		itr->m_Noise.Generate3D(
			a_Workspace, a_SizeX, a_SizeY, a_SizeZ,
			a_StartX * itr->m_Frequency, a_EndX * itr->m_Frequency,
			a_StartY * itr->m_Frequency, a_EndY * itr->m_Frequency,
			a_StartZ * itr->m_Frequency, a_EndZ * itr->m_Frequency
		);
		// Add the cubic noise into the output:
		NOISE_DATATYPE Amplitude = itr->m_Amplitude;
		for (int i = 0; i < ArrayCount; i++)
		{
			a_Array[i] += a_Workspace[i] * Amplitude;
		}
	}
	
	if (ShouldFreeWorkspace)
	{
		delete[] a_Workspace;
		a_Workspace = NULL;
	}
}




