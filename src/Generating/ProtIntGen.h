
// ProtIntGen.h

// Declares the prototyping integer generators - cProtIntGen class and its descendants

/*
These classes generate 2D arrays of integers that have various interpretations. The main purpose of these
classes is to provide fast prototyping for cIntGen classes - unlike cIntGen classes, these are not
template-based and so they take care of the underlying sizes automatically. This makes them easier to chain
and re-chain, since the size parameters don't need to be adjusted after each such case. Their performance is,
however, slightly worse, which is why we use cIntGen classes in the final generator.

Because there is no SizeX / SizeZ template param, the generators would have to either alloc memory for each
underlying generator's values, or use a maximum-size buffer. We chose the latter, to avoid memory allocation
overhead; this however means that there's (an arbitrary) limit to the size of the generated data.
*/





#pragma once

// We need the biome group constants defined there:
#include "IntGen.h"





/** Maximum size of the generated area.
This value is used only if there isn't an override in place.
To adjust the actual buffer size, just do a "#define PROT_INT_BUFFER_SIZE 9000" before including this header.
Note, however, that you should use a consistent value throughout a single project. */
#ifndef PROT_INT_BUFFER_SIZE
	#define PROT_INT_BUFFER_SIZE 900
#endif





/** Interface that all the generator classes provide. */
class cProtIntGen
{
protected:
	/** Maximum size of the generated area.
	Adjust the constant if you need larger areas, these are just so that we can use fixed-size buffers. */
	static const int m_BufferSize = PROT_INT_BUFFER_SIZE;

public:

	/** Type of the generic interface used for storing links to the underlying generators. */
	using Underlying = std::shared_ptr<cProtIntGen>;


	/** Force a virtual destructor in all descendants.
	Descendants contain virtual functions and are referred to via pointer-to-base, so they need a virtual destructor. */
	virtual ~cProtIntGen() {}

	/** Generates the array of specified size into a_Values, based on given min coords. */
	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) = 0;
};





/** Provides additional cNoise member and its helper functions. */
class cProtIntGenWithNoise:
	public cProtIntGen
{
	using Super = cProtIntGen;

public:

	cProtIntGenWithNoise(int a_Seed):
		m_Noise(a_Seed)
	{
	}

protected:
	cNoise m_Noise;

	/** Chooses one of a_Val1 or a_Val2, based on m_Noise and the coordinates for querying the noise. */
	int chooseRandomOne(int a_RndX, int a_RndZ, int a_Val1, int a_Val2)
	{
		int rnd = m_Noise.IntNoise2DInt(a_RndX, a_RndZ) / 7;
		return ((rnd & 1) == 0) ? a_Val1 : a_Val2;
	}

	/** Chooses one of a_ValN, based on m_Noise and the coordinates for querying the noise. */
	int chooseRandomOne(int a_RndX, int a_RndZ, int a_Val1, int a_Val2, int a_Val3, int a_Val4)
	{
		int rnd = m_Noise.IntNoise2DInt(a_RndX, a_RndZ) / 7;
		switch (rnd % 4)
		{
			case 0:  return a_Val1;
			case 1:  return a_Val2;
			case 2:  return a_Val3;
			default: return a_Val4;
		}
	}
};





/** Generates a 2D array of random integers in the specified range [0 .. Range). */
class cProtIntGenChoice:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenChoice(int a_Seed, int a_Range):
		Super(a_Seed),
		m_Range(a_Range)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			int BaseZ = a_MinZ + static_cast<int>(z);
			for (size_t x = 0; x < a_SizeX; x++)
			{
				a_Values[x + a_SizeX * z] = (Super::m_Noise.IntNoise2DInt(a_MinX + static_cast<int>(x), BaseZ) / 7) % m_Range;
			}
		}  // for z
	}

protected:
	int m_Range;
};





/** Decides between the ocean and landmass biomes.
Has a threshold (in percent) of how much land, the larger the threshold, the more land.
Generates 0 for ocean, biome group ID for landmass. */
class cProtIntGenLandOcean:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenLandOcean(int a_Seed, int a_Threshold):
		Super(a_Seed),
		m_Threshold(a_Threshold)
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			int BaseZ = a_MinZ + static_cast<int>(z);
			for (size_t x = 0; x < a_SizeX; x++)
			{
				int rnd = (Super::m_Noise.IntNoise2DInt(a_MinX + static_cast<int>(x), BaseZ) / 7);
				a_Values[x + a_SizeX * z] = ((rnd % 100) < m_Threshold) ? ((rnd / 101) % bgLandOceanMax + 1) : 0;
			}
		}

		// If the centerpoint of the world is within the area, set it to bgTemperate, always:
		if ((a_MinX <= 0) && (a_MinZ <= 0) && (a_MinX + static_cast<int>(a_SizeX) > 0) && (a_MinZ + static_cast<int>(a_SizeZ) > 0))
		{
			a_Values[static_cast<size_t>(-a_MinX) - static_cast<size_t>(a_MinZ) * a_SizeX] = bgTemperate;
		}
	}

protected:
	int m_Threshold;
};





/** Zooms the underlying value array to twice the size. Uses random-neighbor for the pixels in-between.
This means that the zoome out image is randomly distorted. Applying zoom several times provides all
the distortion that the generators need. */
class cProtIntGenZoom:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenZoom(int a_Seed, Underlying a_UnderlyingGen):
		Super(a_Seed),
		m_UnderlyingGen(std::move(a_UnderlyingGen))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Get the coords for the lower generator:
		int lowerMinX = a_MinX >> 1;
		int lowerMinZ = a_MinZ >> 1;
		size_t lowerSizeX = a_SizeX / 2 + 2;
		size_t lowerSizeZ = a_SizeZ / 2 + 2;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		ASSERT(lowerSizeX > 0);
		ASSERT(lowerSizeZ > 0);

		// Generate the underlying data with half the resolution:
		int lowerData[m_BufferSize];
		m_UnderlyingGen->GetInts(lowerMinX, lowerMinZ, lowerSizeX, lowerSizeZ, lowerData);
		const size_t lowStepX = (lowerSizeX - 1) * 2;
		int cache[m_BufferSize];

		// Discreet-interpolate the values into twice the size:
		for (size_t z = 0; z < lowerSizeZ - 1; ++z)
		{
			size_t idx = (z * 2) * lowStepX;
			int PrevZ0 = lowerData[z * lowerSizeX];
			int PrevZ1 = lowerData[(z + 1) * lowerSizeX];

			for (size_t x = 0; x < lowerSizeX - 1; ++x)
			{
				int ValX1Z0 = lowerData[x + 1 + z * lowerSizeX];
				int ValX1Z1 = lowerData[x + 1 + (z + 1) * lowerSizeX];
				int RndX = (static_cast<int>(x) + lowerMinX) * 2;
				int RndZ = (static_cast<int>(z) + lowerMinZ) * 2;
				cache[idx] = PrevZ0;
				cache[idx + lowStepX] = Super::chooseRandomOne(RndX, RndZ + 1, PrevZ0, PrevZ1);
				cache[idx + 1]        = Super::chooseRandomOne(RndX, RndZ - 1, PrevZ0, ValX1Z0);
				cache[idx + 1 + lowStepX] = Super::chooseRandomOne(RndX, RndZ, PrevZ0, ValX1Z0, PrevZ1, ValX1Z1);
				idx += 2;
				PrevZ0 = ValX1Z0;
				PrevZ1 = ValX1Z1;
			}
		}

		// Copy from Cache into a_Values; take into account the even / odd offsets in a_Min:
		for (size_t z = 0; z < a_SizeZ; ++z)
		{
			memcpy(a_Values + z * a_SizeX, cache + (z + (a_MinZ & 1)) * lowStepX + (a_MinX & 1), a_SizeX * sizeof(int));
		}
	}

protected:
	Underlying m_UnderlyingGen;
};





/** Smoothes out some artifacts generated by the zooming - mostly single-pixel values.
Compares each pixel to its neighbors and if the neighbors are equal, changes the pixel to their value. */
class cProtIntGenSmooth:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenSmooth(int a_Seed, Underlying a_Underlying):
		Super(a_Seed),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying values:
		size_t lowerSizeX = a_SizeX + 2;
		size_t lowerSizeZ = a_SizeZ + 2;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		int lowerData[m_BufferSize];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, lowerSizeX, lowerSizeZ, lowerData);

		// Smooth - for each square check if the surroundings are the same, if so, expand them diagonally.
		// Also get rid of single-pixel irregularities (A-B-A):
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			int NoiseZ = a_MinZ + static_cast<int>(z);
			for (size_t x = 0; x < a_SizeX; x++)
			{
				int val   = lowerData[x + 1 + (z + 1) * lowerSizeX];
				int above = lowerData[x + 1 +  z      * lowerSizeX];
				int below = lowerData[x + 1 + (z + 2) * lowerSizeX];
				int left  = lowerData[x     + (z + 1) * lowerSizeX];
				int right = lowerData[x + 2 + (z + 1) * lowerSizeX];

				if ((left == right) && (above == below))
				{
					if (((Super::m_Noise.IntNoise2DInt(a_MinX + static_cast<int>(x), NoiseZ) / 7) % 2) == 0)
					{
						val = left;
					}
					else
					{
						val = above;
					}
				}
				else
				{
					if (left == right)
					{
						val = left;
					}

					if (above == below)
					{
						val = above;
					}
				}

				a_Values[x + z * a_SizeX] = val;
			}
		}
	}

protected:
	Underlying m_Underlying;
};





/** Averages the values of the underlying 2 * 2 neighbors. */
class cProtIntGenAvgValues:
	public cProtIntGen
{
	using Super = cProtIntGen;

public:

	cProtIntGenAvgValues(Underlying a_Underlying):
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying values:
		size_t lowerSizeX = a_SizeX + 1;
		size_t lowerSizeZ = a_SizeZ + 1;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		int lowerData[m_BufferSize];
		m_Underlying->GetInts(a_MinX, a_MinZ, lowerSizeX, lowerSizeZ, lowerData);

		// Average - add all 4 "neighbors" and divide by 4:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t x = 0; x < a_SizeX; x++)
			{
				size_t idxLower = x + lowerSizeX * z;
				a_Values[x + a_SizeX * z] = (
					lowerData[idxLower] + lowerData[idxLower + 1] +
					lowerData[idxLower + lowerSizeX] + lowerData[idxLower + lowerSizeX + 1]
				) / 4;
			}
		}
	}

protected:
	Underlying m_Underlying;
};





/** Averages the values of the underlying 4 * 4 neighbors. */
class cProtIntGenAvg4Values:
	public cProtIntGen
{
	using Super = cProtIntGen;

public:

	cProtIntGenAvg4Values(Underlying a_Underlying):
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying values:
		size_t lowerSizeX = a_SizeX + 4;
		size_t lowerSizeZ = a_SizeZ + 4;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		int lowerData[m_BufferSize];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, lowerSizeX, lowerSizeZ, lowerData);

		// Calculate the weighted average of all 16 "neighbors":
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t x = 0; x < a_SizeX; x++)
			{
				size_t idxLower1 = x + lowerSizeX * z;
				size_t idxLower2 = idxLower1 + lowerSizeX;
				size_t idxLower3 = idxLower1 + 2 * lowerSizeX;
				size_t idxLower4 = idxLower1 + 3 * lowerSizeX;
				a_Values[x + a_SizeX * z] = (
					1 * lowerData[idxLower1] + 2 * lowerData[idxLower1 + 1] + 2 * lowerData[idxLower1 + 2] + 1 * lowerData[idxLower1 + 3] +
					2 * lowerData[idxLower2] + 32 * lowerData[idxLower2 + 1] + 32 * lowerData[idxLower2 + 2] + 2 * lowerData[idxLower2 + 3] +
					2 * lowerData[idxLower3] + 32 * lowerData[idxLower3 + 1] + 32 * lowerData[idxLower3 + 2] + 2 * lowerData[idxLower3 + 3] +
					1 * lowerData[idxLower4] + 2 * lowerData[idxLower4 + 1] + 2 * lowerData[idxLower4 + 2] + 1 * lowerData[idxLower4 + 3]
				) / 148;
			}
		}
	}

protected:
	Underlying m_Underlying;
};





/** Averages the values of the underlying 3 * 3 neighbors with custom weight. */
template <int WeightCenter, int WeightCardinal, int WeightDiagonal>
class cProtIntGenWeightAvg:
	public cProtIntGen
{
	using Super = cProtIntGen;

public:

	cProtIntGenWeightAvg(Underlying a_Underlying):
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying values:
		size_t lowerSizeX = a_SizeX + 3;
		size_t lowerSizeZ = a_SizeZ + 3;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		int lowerData[m_BufferSize];
		m_Underlying->GetInts(a_MinX, a_MinZ, lowerSizeX, lowerSizeZ, lowerData);

		// Calculate the weighted average the neighbors:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t x = 0; x < a_SizeX; x++)
			{
				size_t idxLower1 = x + lowerSizeX * z;
				size_t idxLower2 = idxLower1 + lowerSizeX;
				size_t idxLower3 = idxLower1 + 2 * lowerSizeX;
				a_Values[x + a_SizeX * z] = (
					WeightDiagonal * lowerData[idxLower1] + WeightCardinal * lowerData[idxLower1 + 1] + WeightDiagonal * lowerData[idxLower1 + 2] +
					WeightCardinal * lowerData[idxLower2] + WeightCenter   * lowerData[idxLower2 + 1] + WeightCardinal * lowerData[idxLower2 + 2] +
					WeightDiagonal * lowerData[idxLower3] + WeightCardinal * lowerData[idxLower3 + 1] + WeightDiagonal * lowerData[idxLower3 + 2]
				) / (4 * WeightDiagonal + 4 * WeightCardinal + WeightCenter);
			}
		}
	}

protected:
	Underlying m_Underlying;
};





/** Replaces random values of the underlying data with random integers in the specified range [Min .. Min + Range). */
class cProtIntGenRndChoice:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenRndChoice(int a_Seed, int a_ChancePct, int a_Min, int a_Range, Underlying a_Underlying):
		Super(a_Seed),
		m_ChancePct(a_ChancePct),
		m_Min(a_Min),
		m_Range(a_Range),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying values:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, a_Values);

		// Replace random values:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			int BaseZ = a_MinZ + static_cast<int>(z);
			for (size_t x = 0; x < a_SizeX; x++)
			{
				if (((Super::m_Noise.IntNoise2DInt(BaseZ, a_MinX + static_cast<int>(x)) / 13) % 101) < m_ChancePct)
				{
					a_Values[x + a_SizeX * z] = m_Min + (Super::m_Noise.IntNoise2DInt(a_MinX + static_cast<int>(x), BaseZ) / 7) % m_Range;
				}
			}  // for x
		}  // for z
	}

protected:
	int m_ChancePct;
	int m_Min;
	int m_Range;
	Underlying m_Underlying;
};





/** Adds a random value in range [-a_HalfRange, +a_HalfRange] to each of the underlying values. */
class cProtIntGenAddRnd:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenAddRnd(int a_Seed, int a_HalfRange, Underlying a_Underlying):
		Super(a_Seed),
		m_Range(a_HalfRange * 2 + 1),
		m_HalfRange(a_HalfRange),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying values:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, a_Values);

		// Add the random values:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			int NoiseZ = a_MinZ + static_cast<int>(z);
			for (size_t x = 0; x < a_SizeX; x++)
			{
				int noiseVal = ((Super::m_Noise.IntNoise2DInt(a_MinX + static_cast<int>(x), NoiseZ) / 7) % m_Range) - m_HalfRange;
				a_Values[x + z * a_SizeX] += noiseVal;
			}
		}
	}

protected:
	int m_Range;
	int m_HalfRange;
	Underlying m_Underlying;
};





/** Replaces random underlying values with the average of the neighbors. */
class cProtIntGenRndAvg:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenRndAvg(int a_Seed, int a_AvgChancePct, Underlying a_Underlying):
		Super(a_Seed),
		m_AvgChancePct(a_AvgChancePct),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying values:
		size_t lowerSizeX = a_SizeX + 2;
		size_t lowerSizeZ = a_SizeZ + 2;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		int lowerData[m_BufferSize];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, lowerSizeX, lowerSizeZ, lowerData);

		// Average random values:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			int NoiseZ = a_MinZ + static_cast<int>(z);
			for (size_t x = 0; x < a_SizeX; x++)
			{
				size_t idxLower = x + 1 + lowerSizeX * (z + 1);
				if (((Super::m_Noise.IntNoise2DInt(a_MinX + static_cast<int>(x), NoiseZ) / 7) % 100) > m_AvgChancePct)
				{
					// Average the 4 neighbors:
					a_Values[x + z * a_SizeX] = (
						lowerData[idxLower - 1] + lowerData[idxLower + 1] +
						lowerData[idxLower - lowerSizeX] + lowerData[idxLower + lowerSizeX]
					) / 4;
				}
				else
				{
					// Keep the underlying value:
					a_Values[x + z * a_SizeX] = lowerData[idxLower];
				}
			}
		}
	}

protected:
	int m_AvgChancePct;
	Underlying m_Underlying;
};





/** Replaces random underlying values with a random value in between the max and min of the neighbors. */
class cProtIntGenRndBetween:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenRndBetween(int a_Seed, int a_AvgChancePct, Underlying a_Underlying):
		Super(a_Seed),
		m_AvgChancePct(a_AvgChancePct),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying values:
		size_t lowerSizeX = a_SizeX + 2;
		size_t lowerSizeZ = a_SizeZ + 2;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		int lowerData[m_BufferSize];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, lowerSizeX, lowerSizeZ, lowerData);

		// Average random values:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			int NoiseZ = a_MinZ + static_cast<int>(z);
			for (size_t x = 0; x < a_SizeX; x++)
			{
				size_t idxLower = x + 1 + lowerSizeX * (z + 1);
				if (((Super::m_Noise.IntNoise2DInt(a_MinX + static_cast<int>(x), NoiseZ) / 7) % 100) > m_AvgChancePct)
				{
					// Chose a value in between the min and max neighbor:
					int min = std::min(std::min(lowerData[idxLower - 1], lowerData[idxLower + 1]), std::min(lowerData[idxLower - lowerSizeX], lowerData[idxLower + lowerSizeX]));
					int max = std::max(std::max(lowerData[idxLower - 1], lowerData[idxLower + 1]), std::max(lowerData[idxLower - lowerSizeX], lowerData[idxLower + lowerSizeX]));
					a_Values[x + z * a_SizeX] = min + ((Super::m_Noise.IntNoise2DInt(a_MinX + static_cast<int>(x), NoiseZ + 10) / 7) % (max - min + 1));
				}
				else
				{
					// Keep the underlying value:
					a_Values[x + z * a_SizeX] = lowerData[idxLower];
				}
			}
		}
	}

protected:
	int m_AvgChancePct;
	Underlying m_Underlying;
};





/** Converts land biomes at the edge of an ocean into the respective beach biome. */
class cProtIntGenBeaches:
	public cProtIntGen
{
	using Super = cProtIntGen;

public:

	cProtIntGenBeaches(Underlying a_Underlying):
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Map for biome -> its beach:
		static const int ToBeach[] =
		{
			/* biOcean            */ biOcean,
			/* biPlains           */ biBeach,
			/* biDesert           */ biBeach,
			/* biExtremeHills     */ biStoneBeach,
			/* biForest           */ biBeach,
			/* biTaiga            */ biColdBeach,
			/* biSwampland        */ biSwampland,
			/* biRiver            */ biRiver,
			/* biNether           */ biNether,
			/* biEnd              */ biEnd,
			/* biFrozenOcean      */ biColdBeach,
			/* biFrozenRiver      */ biColdBeach,
			/* biIcePlains        */ biColdBeach,
			/* biIceMountains     */ biColdBeach,
			/* biMushroomIsland   */ biMushroomShore,
			/* biMushroomShore    */ biMushroomShore,
			/* biBeach            */ biBeach,
			/* biDesertHills      */ biBeach,
			/* biForestHills      */ biBeach,
			/* biTaigaHills       */ biColdBeach,
			/* biExtremeHillsEdge */ biStoneBeach,
			/* biJungle           */ biBeach,
			/* biJungleHills      */ biBeach,
			/* biJungleEdge       */ biBeach,
			/* biDeepOcean        */ biOcean,
			/* biStoneBeach       */ biStoneBeach,
			/* biColdBeach        */ biColdBeach,
			/* biBirchForest      */ biBeach,
			/* biBirchForestHills */ biBeach,
			/* biRoofedForest     */ biBeach,
			/* biColdTaiga        */ biColdBeach,
			/* biColdTaigaHills   */ biColdBeach,
			/* biMegaTaiga        */ biStoneBeach,
			/* biMegaTaigaHills   */ biStoneBeach,
			/* biExtremeHillsPlus */ biStoneBeach,
			/* biSavanna          */ biBeach,
			/* biSavannaPlateau   */ biBeach,
			/* biMesa             */ biMesa,
			/* biMesaPlateauF     */ biMesa,
			/* biMesaPlateau      */ biMesa,
		};

		// Generate the underlying values:
		size_t lowerSizeX = a_SizeX + 2;
		size_t lowerSizeZ = a_SizeZ + 2;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		int lowerValues[m_BufferSize];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, lowerSizeX, lowerSizeZ, lowerValues);

		// Add beaches between ocean and biomes:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t x = 0; x < a_SizeX; x++)
			{
				int val   = lowerValues[x + 1 + (z + 1) * lowerSizeX];
				int above = lowerValues[x + 1 + z       * lowerSizeX];
				int below = lowerValues[x + 1 + (z + 2) * lowerSizeX];
				int left  = lowerValues[x     + (z + 1) * lowerSizeX];
				int right = lowerValues[x + 2 + (z + 1) * lowerSizeX];
				if (!IsBiomeOcean(val))
				{
					if (IsBiomeOcean(above) || IsBiomeOcean(below) || IsBiomeOcean(left) || IsBiomeOcean(right))
					{
						// First convert the value to a regular biome (drop the M flag), then modulo by our biome count:
						val = ToBeach[(val % 128) % ARRAYCOUNT(ToBeach)];
					}
				}
				a_Values[x + z * a_SizeX] = val;
			}
		}
	}

protected:
	Underlying m_Underlying;
};





/** Generates the underlying numbers and then randomly changes some ocean group pixels into random land
biome group pixels, based on the predefined chance. */
class cProtIntGenAddIslands:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	using Underlying = std::shared_ptr<cProtIntGen>;


	cProtIntGenAddIslands(int a_Seed, int a_Chance, Underlying a_Underlying):
		Super(a_Seed),
		m_Chance(a_Chance),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		m_Underlying->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, a_Values);
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t x = 0; x < a_SizeX; x++)
			{
				if (a_Values[x + z * a_SizeX] == bgOcean)
				{
					int rnd = Super::m_Noise.IntNoise2DInt(a_MinX + static_cast<int>(x), a_MinZ + static_cast<int>(z)) / 7;
					if (rnd % 1000 < m_Chance)
					{
						a_Values[x + z * a_SizeX] = (rnd / 1003) % bgLandOceanMax;
					}
				}
			}
		}
	}

protected:
	/** Chance of each ocean pixel being converted, in permille. */
	int m_Chance;

	Underlying m_Underlying;
};





/** A filter that adds an edge biome group between two biome groups that need an edge between them. */
class cProtIntGenBiomeGroupEdges:
	public cProtIntGen
{
	using Super = cProtIntGen;

public:

	cProtIntGenBiomeGroupEdges(Underlying a_Underlying):
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values)
	{
		// Generate the underlying biome groups:
		size_t lowerSizeX = a_SizeX + 2;
		size_t lowerSizeZ = a_SizeZ + 2;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		int lowerValues[m_BufferSize];
		m_Underlying->GetInts(a_MinX, a_MinZ, lowerSizeX, lowerSizeZ, lowerValues);

		// Change the biomes on incompatible edges into an edge biome:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t x = 0; x < a_SizeX; x++)
			{
				int val   = lowerValues[x + 1 + (z + 1) * lowerSizeX];
				int Above = lowerValues[x + 1 + z       * lowerSizeX];
				int Below = lowerValues[x + 1 + (z + 2) * lowerSizeX];
				int Left  = lowerValues[x     + (z + 1) * lowerSizeX];
				int Right = lowerValues[x + 2 + (z + 1) * lowerSizeX];
				switch (val)
				{
					// Desert should neighbor only oceans, desert and temperates; change to temperate when another:
					case bgDesert:
					{
						if (
							!isDesertCompatible(Above) ||
							!isDesertCompatible(Below) ||
							!isDesertCompatible(Left) ||
							!isDesertCompatible(Right)
						)
						{
							val = bgTemperate;
						}
						break;
					}  // case bgDesert

					// Ice should not neighbor deserts; change to temperate:
					case bgIce:
					{
						if (
							(Above == bgDesert) ||
							(Below == bgDesert) ||
							(Left  == bgDesert) ||
							(Right == bgDesert)
						)
						{
							val = bgTemperate;
						}
						break;
					}  // case bgIce
				}
				a_Values[x + z * a_SizeX] = val;
			}  // for x
		}  // for z
	}

protected:
	Underlying m_Underlying;


	inline bool isDesertCompatible(int a_BiomeGroup)
	{
		switch (a_BiomeGroup)
		{
			case bgOcean:
			case bgDesert:
			case bgTemperate:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}
};





/** Turns biome group indices into real biomes.
For each pixel, takes its biome group and chooses a random biome from that group; replaces the value with
that biome. */
class cProtIntGenBiomes:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenBiomes(int a_Seed, Underlying a_Underlying):
		Super(a_Seed),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Define the per-biome-group biomes:
		static const int oceanBiomes[] =
		{
			biOcean,  // biDeepOcean,
		};

		// Same as oceanBiomes, there are no rare oceanic biomes (mushroom islands are handled separately)
		static const int rareOceanBiomes[] =
		{
			biOcean,
		};

		static const int desertBiomes[] =
		{
			biDesert, biDesert, biDesert, biDesert, biDesert, biDesert, biSavanna, biSavanna, biPlains,
		};

		static const int rareDesertBiomes[] =
		{
			biMesaPlateau, biMesaPlateauF,
		};

		static const int temperateBiomes[] =
		{
			biForest, biForest, biRoofedForest, biExtremeHills, biPlains, biBirchForest, biSwampland,
		};

		static const int rareTemperateBiomes[] =
		{
			biJungle,  // Jungle is not strictly temperate, but let's piggyback it here
		};

		static const int mountainBiomes[] =
		{
			biExtremeHills, biForest, biTaiga, biPlains,
		};

		static const int rareMountainBiomes[] =
		{
			biMegaTaiga,
		};

		static const int iceBiomes[] =
		{
			biIcePlains, biIcePlains, biIcePlains, biIcePlains, biColdTaiga,
		};

		// Same as iceBiomes, there's no rare ice biome
		static const int rareIceBiomes[] =
		{
			biIcePlains, biIcePlains, biIcePlains, biIcePlains, biColdTaiga,
		};

		static const cBiomesInGroups biomesInGroups[] =
		{
			/* bgOcean */     { static_cast<int>(ARRAYCOUNT(oceanBiomes)),     oceanBiomes},
			/* bgDesert */    { static_cast<int>(ARRAYCOUNT(desertBiomes)),    desertBiomes},
			/* bgTemperate */ { static_cast<int>(ARRAYCOUNT(temperateBiomes)), temperateBiomes},
			/* bgMountains */ { static_cast<int>(ARRAYCOUNT(mountainBiomes)),  mountainBiomes},
			/* bgIce */       { static_cast<int>(ARRAYCOUNT(iceBiomes)),       iceBiomes},
		};

		static const cBiomesInGroups rareBiomesInGroups[] =
		{
			/* bgOcean */     { static_cast<int>(ARRAYCOUNT(rareOceanBiomes)),     rareOceanBiomes},
			/* bgDesert */    { static_cast<int>(ARRAYCOUNT(rareDesertBiomes)),    rareDesertBiomes},
			/* bgTemperate */ { static_cast<int>(ARRAYCOUNT(rareTemperateBiomes)), rareTemperateBiomes},
			/* bgMountains */ { static_cast<int>(ARRAYCOUNT(rareMountainBiomes)),  rareMountainBiomes},
			/* bgIce */       { static_cast<int>(ARRAYCOUNT(rareIceBiomes)),       rareIceBiomes},
		};

		// Generate the underlying values, representing biome groups:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, a_Values);

		// Overwrite each biome group with a random biome from that group:
		// Take care of the bgfRare flag
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			size_t IdxZ = z * a_SizeX;
			for (size_t x = 0; x < a_SizeX; x++)
			{
				int val = a_Values[x + IdxZ];
				const cBiomesInGroups & Biomes = (val > bgfRare) ?
					rareBiomesInGroups[(val & (bgfRare - 1)) % ARRAYCOUNT(rareBiomesInGroups)] :
					biomesInGroups[static_cast<size_t>(val) % ARRAYCOUNT(biomesInGroups)];
				int rnd = (Super::m_Noise.IntNoise2DInt(static_cast<int>(x) + a_MinX, static_cast<int>(z) + a_MinZ) / 7);
				a_Values[x + IdxZ] = Biomes.Biomes[rnd % Biomes.Count];
			}
		}
	}

protected:

	struct cBiomesInGroups
	{
		const int Count;
		const int * Biomes;
	};


	/** The underlying int generator */
	Underlying m_Underlying;
};





/** Randomly replaces pixels of one value to another value, using the given chance. */
class cProtIntGenReplaceRandomly:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	using Underlying = std::shared_ptr<cProtIntGen>;


	cProtIntGenReplaceRandomly(int a_Seed, int a_From, int a_To, int a_Chance, Underlying a_Underlying):
		Super(a_Seed),
		m_From(a_From),
		m_To(a_To),
		m_Chance(a_Chance),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying values:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, a_Values);

		// Replace some of the values:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			size_t idxZ = z * a_SizeX;
			for (size_t x = 0; x < a_SizeX; x++)
			{
				size_t idx = x + idxZ;
				if (a_Values[idx] == m_From)
				{
					int rnd = Super::m_Noise.IntNoise2DInt(static_cast<int>(x) + a_MinX, static_cast<int>(z) + a_MinZ) / 7;
					if (rnd % 1000 < m_Chance)
					{
						a_Values[idx] = m_To;
					}
				}
			}
		}  // for z
	}


protected:
	/** The original value to be replaced. */
	int m_From;

	/** The destination value to which to replace. */
	int m_To;

	/** Chance, in permille, of replacing the value. */
	int m_Chance;

	Underlying m_Underlying;
};





/** Mixer that joins together finalized biomes and rivers.
It first checks for oceans, if there is an ocean in the Biomes, it keeps the ocean.
If there's no ocean, it checks Rivers for a river, if there is a river, it uses the Biomes to select either
regular river or frozen river, based on the biome. */
class cProtIntGenMixRivers:
	public cProtIntGen
{
	using Super = cProtIntGen;

public:

	cProtIntGenMixRivers(Underlying a_Biomes, Underlying a_Rivers):
		m_Biomes(std::move(a_Biomes)),
		m_Rivers(std::move(a_Rivers))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying data:
		ASSERT(a_SizeX * a_SizeZ <= m_BufferSize);
		m_Biomes->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, a_Values);
		int riverData[m_BufferSize];
		m_Rivers->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, riverData);

		// Mix the values:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			size_t idxZ = z * a_SizeX;
			for (size_t x = 0; x < a_SizeX; x++)
			{
				size_t idx = x + idxZ;
				if (IsBiomeOcean(a_Values[idx]))
				{
					// Oceans are kept without any changes
					continue;
				}
				if (riverData[idx] != biRiver)
				{
					// There's no river, keep the current value
					continue;
				}

				// There's a river, change the output to a river or a frozen river, based on the original biome:
				if (IsBiomeVeryCold(static_cast<EMCSBiome>(a_Values[idx])))
				{
					a_Values[idx] = biFrozenRiver;
				}
				else
				{
					a_Values[idx] = biRiver;
				}
			}  // for x
		}  // for z
	}

protected:
	Underlying m_Biomes;
	Underlying m_Rivers;
};





/** Generates a river based on the underlying data.
This is basically an edge detector over the underlying data. The rivers are the edges where the underlying
data changes from one pixel to its neighbor. */
class cProtIntGenRiver:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenRiver(int a_Seed, Underlying a_Underlying):
		Super(a_Seed),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying data:
		size_t lowerSizeX = a_SizeX + 2;
		size_t lowerSizeZ = a_SizeZ + 2;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		int lowerValues[m_BufferSize];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, lowerSizeX, lowerSizeZ, lowerValues);

		// Detect the edges:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t x = 0; x < a_SizeX; x++)
			{
				int Above = lowerValues[x + 1 + z       * lowerSizeX];
				int Below = lowerValues[x + 1 + (z + 2) * lowerSizeX];
				int Left  = lowerValues[x +     (z + 1) * lowerSizeX];
				int Right = lowerValues[x + 2 + (z + 1) * lowerSizeX];
				int val   = lowerValues[x + 1 + (z + 1) * lowerSizeX];

				if ((val == Above) && (val == Below) && (val == Left) && (val == Right))
				{
					val = 0;
				}
				else
				{
					val = biRiver;
				}
				a_Values[x + z * a_SizeX] = val;
			}  // for x
		}  // for z
	}

protected:
	Underlying m_Underlying;
};





/** Turns some of the oceans into the specified biome. Used for mushroom and deep ocean.
The biome is only placed if at least 3 of its neighbors are ocean and only with the specified chance. */
class cProtIntGenAddToOcean:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenAddToOcean(int a_Seed, int a_Chance, int a_ToValue, Underlying a_Underlying):
		Super(a_Seed),
		m_Chance(a_Chance),
		m_ToValue(a_ToValue),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying data:
		size_t lowerSizeX = a_SizeX + 2;
		size_t lowerSizeZ = a_SizeZ + 2;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		int lowerValues[m_BufferSize];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, lowerSizeX, lowerSizeZ, lowerValues);

		// Add the mushroom islands:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t x = 0; x < a_SizeX; x++)
			{
				int val = lowerValues[x + 1 + (z + 1) * lowerSizeX];
				if (!IsBiomeOcean(val))
				{
					a_Values[x + z * a_SizeX] = val;
					continue;
				}

				// Count the ocean neighbors:
				int Above = lowerValues[x + 1 + z       * lowerSizeX];
				int Below = lowerValues[x + 1 + (z + 2) * lowerSizeX];
				int Left  = lowerValues[x +     (z + 1) * lowerSizeX];
				int Right = lowerValues[x + 2 + (z + 1) * lowerSizeX];
				int NumOceanNeighbors = 0;
				if (IsBiomeOcean(Above))
				{
					NumOceanNeighbors += 1;
				}
				if (IsBiomeOcean(Below))
				{
					NumOceanNeighbors += 1;
				}
				if (IsBiomeOcean(Left))
				{
					NumOceanNeighbors += 1;
				}
				if (IsBiomeOcean(Right))
				{
					NumOceanNeighbors += 1;
				}

				// If at least 3 ocean neighbors and the chance is right, change:
				if (
					(NumOceanNeighbors >= 3) &&
					((Super::m_Noise.IntNoise2DInt(static_cast<int>(x) + a_MinX, static_cast<int>(z) + a_MinZ) / 7) % 1000 < m_Chance)
				)
				{
					a_Values[x + z * a_SizeX] = m_ToValue;
				}
				else
				{
					a_Values[x + z * a_SizeX] = val;
				}
			}  // for x
		}  // for z
	}

protected:
	/** Chance, in permille, of changing the biome. */
	int m_Chance;

	/** The value to change the ocean into. */
	int m_ToValue;

	Underlying m_Underlying;
};





/** Changes random pixels of the underlying data to the specified value. */
class cProtIntGenSetRandomly:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenSetRandomly(int a_Seed, int a_Chance, int a_ToValue, Underlying a_Underlying):
		Super(a_Seed),
		m_Chance(a_Chance),
		m_ToValue(a_ToValue),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying data:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, a_Values);

		// Change random pixels to bgOcean:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t x = 0; x < a_SizeX; x++)
			{
				int rnd = Super::m_Noise.IntNoise2DInt(static_cast<int>(x) + a_MinX, static_cast<int>(z) + a_MinZ) / 7;
				if (rnd % 1000 < m_Chance)
				{
					a_Values[x + z * a_SizeX] = m_ToValue;
				}
			}
		}
	}

protected:
	/** Chance, in permille, of changing each pixel. */
	int m_Chance;

	/** The value to which to set the pixel. */
	int m_ToValue;

	Underlying m_Underlying;
};





/** Adds a "rare" flag to random biome groups, based on the given chance. */
class cProtIntGenRareBiomeGroups:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenRareBiomeGroups(int a_Seed, int a_Chance, Underlying a_Underlying):
		Super(a_Seed),
		m_Chance(a_Chance),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying data:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, a_Values);

		// Change some of the biome groups into rare biome groups:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t x = 0; x < a_SizeX; x++)
			{
				int rnd = Super::m_Noise.IntNoise2DInt(static_cast<int>(x) + a_MinX, static_cast<int>(z) + a_MinZ) / 7;
				if (rnd % 1000 < m_Chance)
				{
					size_t idx = x + a_SizeX * z;
					a_Values[idx] = a_Values[idx] | bgfRare;
				}
			}
		}
	}

protected:
	/** Chance, in permille, of changing each pixel into the rare biome group. */
	int m_Chance;

	/** The underlying generator. */
	Underlying m_Underlying;
};





/** Changes biomes in the parent data into an alternate versions (usually "hill" variants), in such places
that have their alterations set. */
class cProtIntGenAlternateBiomes:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenAlternateBiomes(int a_Seed, Underlying a_Alterations, Underlying a_BaseBiomes):
		Super(a_Seed),
		m_Alterations(std::move(a_Alterations)),
		m_BaseBiomes(std::move(a_BaseBiomes))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the base biomes and the alterations:
		m_BaseBiomes->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, a_Values);
		int alterations[m_BufferSize];
		m_Alterations->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, alterations);

		// Change the biomes into their alternate versions:
		size_t len = a_SizeX * a_SizeZ;
		for (size_t idx = 0; idx < len; ++idx)
		{
			if (alterations[idx] == 0)
			{
				// No change
				continue;
			}

			// Change to alternate biomes:
			int val = a_Values[idx];
			switch (val)
			{
				case biBirchForest:   val = biBirchForestHills; break;
				case biDesert:        val = biDesertHills;      break;
				case biExtremeHills:  val = biExtremeHillsPlus; break;
				case biForest:        val = biForestHills;      break;
				case biIcePlains:     val = biIceMountains;     break;
				case biJungle:        val = biJungleHills;      break;
				case biMegaTaiga:     val = biMegaTaigaHills;   break;
				case biMesaPlateau:   val = biMesa;             break;
				case biMesaPlateauF:  val = biMesa;             break;
				case biMesaPlateauM:  val = biMesa;             break;
				case biMesaPlateauFM: val = biMesa;             break;
				case biPlains:        val = biForest;           break;
				case biRoofedForest:  val = biPlains;           break;
				case biSavanna:       val = biSavannaPlateau;   break;
				case biTaiga:         val = biTaigaHills;       break;
			}
			a_Values[idx] = val;
		}  // for idx - a_Values[]
	}

protected:
	Underlying m_Alterations;
	Underlying m_BaseBiomes;
};





/** Adds an edge between two specifically incompatible biomes, such as mesa and forest. */
class cProtIntGenBiomeEdges:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenBiomeEdges(int a_Seed, Underlying a_Underlying):
		Super(a_Seed),
		m_Underlying(std::move(a_Underlying))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying biomes:
		size_t lowerSizeX = a_SizeX + 2;
		size_t lowerSizeZ = a_SizeZ + 2;
		ASSERT(lowerSizeX * lowerSizeZ <= m_BufferSize);
		int lowerValues[m_BufferSize];
		m_Underlying->GetInts(a_MinX - 1, a_MinZ - 1, lowerSizeX, lowerSizeZ, lowerValues);

		// Convert incompatible edges into neutral biomes:
		for (size_t z = 0; z < a_SizeZ; z++)
		{
			for (size_t x = 0; x < a_SizeX; x++)
			{
				int biome = lowerValues[x + 1 + (z + 1) * lowerSizeX];
				int above = lowerValues[x + 1 + z *       lowerSizeX];
				int below = lowerValues[x + 1 + (z + 2) * lowerSizeX];
				int left  = lowerValues[x +     (z + 1) * lowerSizeX];
				int right = lowerValues[x + 2 + (z + 1) * lowerSizeX];

				switch (biome)
				{
					case biDesert:
					case biDesertM:
					case biDesertHills:
					{
						if (
							IsBiomeVeryCold(static_cast<EMCSBiome>(above)) ||
							IsBiomeVeryCold(static_cast<EMCSBiome>(below)) ||
							IsBiomeVeryCold(static_cast<EMCSBiome>(left))  ||
							IsBiomeVeryCold(static_cast<EMCSBiome>(right))
						)
						{
							biome = biPlains;
						}
						break;
					}  // case biDesert

					case biMesaPlateau:
					case biMesaPlateauF:
					case biMesaPlateauFM:
					case biMesaPlateauM:
					{
						if (
							!isMesaCompatible(above) ||
							!isMesaCompatible(below) ||
							!isMesaCompatible(left)  ||
							!isMesaCompatible(right)
						)
						{
							biome = biDesert;
						}
						break;
					}  // Mesa biomes

					case biJungle:
					case biJungleM:
					{
						if (
							!isJungleCompatible(above) ||
							!isJungleCompatible(below) ||
							!isJungleCompatible(left)  ||
							!isJungleCompatible(right)
						)
						{
							biome = (biome == biJungle) ? biJungleEdge : biJungleEdgeM;
						}
						break;
					}  // Jungle biomes

					case biSwampland:
					case biSwamplandM:
					{
						if (
							IsBiomeNoDownfall(static_cast<EMCSBiome>(above)) ||
							IsBiomeNoDownfall(static_cast<EMCSBiome>(below)) ||
							IsBiomeNoDownfall(static_cast<EMCSBiome>(left))  ||
							IsBiomeNoDownfall(static_cast<EMCSBiome>(right))
						)
						{
							biome = biPlains;
						}
						break;
					}  // Swampland biomes
				}  // switch (biome)

				a_Values[x + z * a_SizeX] = biome;
			}  // for x
		}  // for z
	}


protected:
	Underlying m_Underlying;


	bool isMesaCompatible(int a_Biome)
	{
		switch (a_Biome)
		{
			case biDesert:
			case biMesa:
			case biMesaBryce:
			case biMesaPlateau:
			case biMesaPlateauF:
			case biMesaPlateauFM:
			case biMesaPlateauM:
			case biOcean:
			case biDeepOcean:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}


	bool isJungleCompatible(int a_Biome)
	{
		switch (a_Biome)
		{
			case biJungle:
			case biJungleM:
			case biJungleEdge:
			case biJungleEdgeM:
			case biJungleHills:
			{
				return true;
			}
			default:
			{
				return false;
			}
		}
	}
};





/** Changes biomes in the parent data into their alternate versions ("M" variants), in such places that
have their alterations set. */
class cProtIntGenMBiomes:
	public cProtIntGenWithNoise
{
	using Super = cProtIntGenWithNoise;

public:

	cProtIntGenMBiomes(int a_Seed, Underlying a_Alteration, Underlying a_Underlying):
		Super(a_Seed),
		m_Underlying(std::move(a_Underlying)),
		m_Alteration(std::move(a_Alteration))
	{
	}


	virtual void GetInts(int a_MinX, int a_MinZ, size_t a_SizeX, size_t a_SizeZ, int *a_Values) override
	{
		// Generate the underlying biomes and the alterations:
		m_Underlying->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, a_Values);
		int alterations[m_BufferSize];
		m_Alteration->GetInts(a_MinX, a_MinZ, a_SizeX, a_SizeZ, alterations);

		// Wherever alterations are nonzero, change into alternate biome, if available:
		size_t len = a_SizeX * a_SizeZ;
		for (size_t idx = 0; idx < len; ++idx)
		{
			if (alterations[idx] == 0)
			{
				continue;
			}

			// Ice spikes biome was removed from here, because it was generated way too often
			switch (a_Values[idx])
			{
				case biPlains:               a_Values[idx] = biSunflowerPlains;      break;
				case biDesert:               a_Values[idx] = biDesertM;              break;
				case biExtremeHills:         a_Values[idx] = biExtremeHillsM;        break;
				case biForest:               a_Values[idx] = biFlowerForest;         break;
				case biTaiga:                a_Values[idx] = biTaigaM;               break;
				case biSwampland:            a_Values[idx] = biSwamplandM;           break;
				case biJungle:               a_Values[idx] = biJungleM;              break;
				case biJungleEdge:           a_Values[idx] = biJungleEdgeM;          break;
				case biBirchForest:          a_Values[idx] = biBirchForestM;         break;
				case biBirchForestHills:     a_Values[idx] = biBirchForestHillsM;    break;
				case biRoofedForest:         a_Values[idx] = biRoofedForestM;        break;
				case biColdTaiga:            a_Values[idx] = biColdTaigaM;           break;
				case biMegaSpruceTaiga:      a_Values[idx] = biMegaSpruceTaiga;      break;
				case biMegaSpruceTaigaHills: a_Values[idx] = biMegaSpruceTaigaHills; break;
				case biExtremeHillsPlus:     a_Values[idx] = biExtremeHillsPlusM;    break;
				case biSavanna:              a_Values[idx] = biSavannaM;             break;
				case biSavannaPlateau:       a_Values[idx] = biSavannaPlateauM;      break;
				case biMesa:                 a_Values[idx] = biMesaBryce;            break;
				case biMesaPlateauF:         a_Values[idx] = biMesaPlateauFM;        break;
				case biMesaPlateau:          a_Values[idx] = biMesaBryce;            break;
			}
		}  // for idx - a_Values[] / alterations[]
	}

protected:
	Underlying m_Underlying;
	Underlying m_Alteration;
};




