
// RidgedNoise.h

// Implements the cRidgedNoise template class that generates ridged noise based on another noise provider.





#pragma once





template <typename N>
class cRidgedNoise
{
public:
	/** Creates a new instance with the seed set to 0. */
	cRidgedNoise(void):
		m_Noise(0)
	{
	}


	/** Creates a new instance with the specified seed. */
	cRidgedNoise(int a_Seed):
		m_Noise(a_Seed)
	{
	}


	/** Sets the seed for the underlying noise. */
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
		int ArrayCount = a_SizeX * a_SizeY;
		m_Noise.Generate2D(
			a_Array, a_SizeX, a_SizeY,
			a_StartX, a_EndX,
			a_StartY, a_EndY
		);
		for (int i = 0; i < ArrayCount; i++)
		{
			a_Array[i] = std::abs(a_Array[i]);
		}
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
		int ArrayCount = a_SizeX * a_SizeY * a_SizeZ;
		m_Noise.Generate2D(
			a_Array, a_SizeX, a_SizeY, a_SizeZ,
			a_StartX, a_EndX,
			a_StartY, a_EndY,
			a_StartZ, a_EndZ
		);
		for (int i = 0; i < ArrayCount; i++)
		{
			a_Array[i] = std::abs(a_Array[i]);
		}
	}

protected:
	N m_Noise;
} ;





