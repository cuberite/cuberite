// SimplexNoise.h

// Declares and implements the simplex noise, using a template parameter for the underlying datatype

/*
Note:
This code has been adapted from the public domain code by Stefan Gustavson, available at
http://staffwww.itn.liu.se/~stegu/simplexnoise/simplexnoise.pdf
*/

#include <random>





template<typename Datatype>
class cSimplexNoise
{
public:
	cSimplexNoise(unsigned a_Seed)
	{
		// Based on the seed, initialize the permutation table, using a simple LCG and swapping

		// Initialize with sorted sequence:
		for (size_t i = 0; i < ARRAYCOUNT(m_Perm) / 2; i++)
		{
			m_Perm[i] = static_cast<int>(i);
		}

		// Use swaps to randomize:
		std::minstd_rand lcg(a_Seed);
		for (size_t i = 0; i < 2000; i++)
		{
			std::swap(m_Perm[lcg() % (ARRAYCOUNT(m_Perm) / 2)], m_Perm[lcg() % (ARRAYCOUNT(m_Perm) / 2)]);
		}

		// Copy to the upper half of the buffer (to avoid the need for modulo when accessing neighbors):
		for (size_t i = ARRAYCOUNT(m_Perm) / 2; i < ARRAYCOUNT(m_Perm); i++)
		{
			m_Perm[i] = m_Perm[i - ARRAYCOUNT(m_Perm) / 2];
		}

		// Copy to the "modulo 12" table to optimize away four modulo ops per value calculation:
		for (size_t i = 0; i < ARRAYCOUNT(m_Perm); i++)
		{
			m_PermMod12[i] = m_Perm[i] % 12;
		}
	}



	/** Returns a dot product of an int vector with a Datatype vector. */
	inline Datatype dot(const int * g, const Datatype x, const Datatype y, const Datatype z)
	{
		return g[0] * x + g[1] * y + g[2] * z;
	}



	/** Returns a dot product of two Datatype vectors. */
	inline Datatype dot(const Datatype * g, const Datatype x, const Datatype y, const Datatype z)
	{
		return g[0] * x + g[1] * y + g[2] * z;
	}



	/** Returns the floor of the specified value, already type-cast to proper int type. */
	inline int datafloor(const Datatype a_Val)
	{
		return (a_Val > 0) ? static_cast<int>(a_Val) : static_cast<int>(a_Val - 1);  // This is faster than std::floor()
	}


	/** Returns a single noise value based on the 3D coords. */
	Datatype GetValueAt3D(const Datatype a_X, const Datatype a_Y, const Datatype a_Z)
	{
		// The gradients are the midpoints of the vertices of a cube.
		static const Datatype grad3[12][3] =
		{
			{1, 1, 0}, {-1,  1, 0}, {1, -1,  0}, {-1, -1,  0},
			{1, 0, 1}, {-1,  0, 1}, {1,  0, -1}, {-1,  0, -1},
			{0, 1, 1}, { 0, -1, 1}, {0,  1, -1}, { 0, -1, -1}
		};

		// Skew factors:
		static const Datatype F3 = static_cast<Datatype>(1.0 / 3.0);
		static const Datatype G3 = static_cast<Datatype>(1.0 / 6.0);

		// Noise contributions from the four corners:
		Datatype n0, n1, n2, n3;

		// Skew the input space to determine which simplex cell we're in
		Datatype s = (a_X + a_Y + a_Z) * F3;
		int i = datafloor(a_X + s);
		int j = datafloor(a_Y + s);
		int k = datafloor(a_Z + s);

		// Unskew back into the XYZ space to calculate the distances from cell origin:
		Datatype t = (i + j + k) * G3;
		Datatype X0 = i - t;
		Datatype Y0 = j - t;
		Datatype Z0 = k - t;
		Datatype x0 = a_X - X0;
		Datatype y0 = a_Y - Y0;
		Datatype z0 = a_Z - Z0;

		// For the 3D case, the simplex shape is a slightly irregular tetrahedron.
		// Determine which simplex we are in.
		int i1, j1, k1;  // Offsets for second corner of simplex in IJK coords
		int i2, j2, k2;  // Offsets for third  corner of simplex in IJK coords
		if (x0 >= y0)
		{
			if (y0 >= z0)
			{
				// X Y Z order
				i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
			}
			else if (x0 >= z0)
			{
				// X Z Y order
				i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1;
			}
			else
			{
				// Z X Y order
				i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1;
			}
		}
		else
		{
			if (y0 < z0)
			{
				// Z Y X order
				i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1;
			}
			else if (x0 < z0)
			{
				// Y Z X order
				i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1;
			}
			else
			{
				// Y X Z order
				i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
			}
		}

		// A step of (1, 0, 0) in IJK means a step of (1 - c,    -c,    -c) in XYZ,
		// a step of (0, 1, 0) in IJK means a step of (-c,    1 - c,    -c) in XYZ, and
		// a step of (0, 0, 1) in IJK means a step of (-c,       -c, 1 - c) in XYZ, where c = G3 = 1 / 6.
		Datatype x1 = x0 - i1 + G3;  // Offsets for second corner in XYZ coords
		Datatype y1 = y0 - j1 + G3;
		Datatype z1 = z0 - k1 + G3;
		Datatype x2 = x0 - i2 + static_cast<Datatype>(2) * G3;  // Offsets for third corner in XYZ coords
		Datatype y2 = y0 - j2 + static_cast<Datatype>(2) * G3;
		Datatype z2 = z0 - k2 + static_cast<Datatype>(2) * G3;
		Datatype x3 = x0 - static_cast<Datatype>(1) + static_cast<Datatype>(3) * G3;  // Offsets for last corner in XYZ coords
		Datatype y3 = y0 - static_cast<Datatype>(1) + static_cast<Datatype>(3) * G3;
		Datatype z3 = z0 - static_cast<Datatype>(1) + static_cast<Datatype>(3) * G3;

		// Work out the hashed gradient indices of the four simplex corners
		int ii = i & 255;
		int jj = j & 255;
		int kk = k & 255;
		int gi0 = m_PermMod12[ii + m_Perm[jj + m_Perm[kk]]];
		int gi1 = m_PermMod12[ii + i1 + m_Perm[jj + j1 + m_Perm[kk + k1]]];
		int gi2 = m_PermMod12[ii + i2 + m_Perm[jj + j2 + m_Perm[kk + k2]]];
		int gi3 = m_PermMod12[ii + 1 + m_Perm[jj + 1 + m_Perm[kk + 1]]];

		// Calculate the contribution from the four corners
		Datatype t0 = static_cast<Datatype>(0.6) - x0 * x0 - y0 * y0 - z0 * z0;
		if (t0 < 0)
		{
			n0 = 0.0;
		}
		else
		{
			t0 *= t0;
			n0 = t0 * t0 * dot(grad3[gi0], x0, y0, z0);
		}

		Datatype t1 = static_cast<Datatype>(0.6) - x1 * x1 - y1 * y1 - z1 * z1;
		if (t1 < 0)
		{
			n1 = 0.0;
		}
		else
		{
			t1 *= t1;
			n1 = t1 * t1 * dot(grad3[gi1], x1, y1, z1);
		}

		Datatype t2 = static_cast<Datatype>(0.6) - x2 * x2 - y2 * y2 - z2 * z2;
		if (t2 < 0)
		{
			n2 = 0.0;
		}
		else
		{
			t2 *= t2;
			n2 = t2 * t2 * dot(grad3[gi2], x2, y2, z2);
		}

		Datatype t3 = static_cast<Datatype>(0.6) - x3 * x3 - y3 * y3 - z3 * z3;
		if (t3 < 0)
		{
			n3 = 0.0;
		}
		else
		{
			t3 *= t3;
			n3 = t3 * t3 * dot(grad3[gi3], x3, y3, z3);
		}

		// Add contributions from each corner to get the final noise value.
		// The result is scaled to stay just inside [-1, 1]
		return static_cast<Datatype>(32) * (n0 + n1 + n2 + n3);
	}




	/** Generates the 3D version of the SImplex noise.
	a_Out is the 3D array into which the noise is output. Organized as [x + a_SizeX * y + a_SizeX * a_SizeY * z].
	a_SizeX, a_SizeY, a_SizeZ are the dimensions of the a_Out array.
	a_Start and a_End are the coords of the 3D array in the noise-space. */
	void Generate3D(
		Datatype * a_Out,
		int a_SizeX, int a_SizeY, int a_SizeZ,
		Datatype a_StartX, Datatype a_EndX,
		Datatype a_StartY, Datatype a_EndY,
		Datatype a_StartZ, Datatype a_EndZ
	)
	{
		Datatype * out = a_Out;
		for (int z = 0; z < a_SizeZ; ++z)
		{
			Datatype nz = a_StartZ + z * (a_EndZ - a_StartZ) / a_SizeZ;
			for (int y = 0; y < a_SizeY; ++y)
			{
				Datatype ny = a_StartY + y * (a_EndY - a_StartY) / a_SizeY;
				for (int x = 0; x < a_SizeX; ++x)
				{
					Datatype nx = a_StartX + x * (a_EndX - a_StartX) / a_SizeX;
					*out = GetValueAt3D(nx, ny, nz);
					++out;
				}  // for x
			}  // for y
		}  // for z
	}

protected:
	/** The permutation table, initialized by the seed. */
	int m_Perm[512];

	/** A copy of the permutation table, with each item modulo 12, to avoid 4 modulo operations per value calculation. */
	int m_PermMod12[512];
};




