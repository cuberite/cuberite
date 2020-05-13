
// OctavedNoise.h

// Implements the cOctavedNoise class template representing a noise generator that layers several octaves of another noise





#pragma once





template <typename N>
class cOctavedNoise
{
public:
	cOctavedNoise(int a_Seed = 0):
		m_Seed(a_Seed)
	{
	}


	/** Sets a new seed for the generators. Relays the seed to all underlying octaves. */
	void SetSeed(int a_Seed)
	{
		m_Seed = a_Seed;
		for (auto oct: m_Octaves)
		{
			oct->SetSeed(a_Seed);
		}
	}


	/** Adds a new octave to the list of octaves that compose this noise. */
	void AddOctave(NOISE_DATATYPE a_Frequency, NOISE_DATATYPE a_Amplitude)
	{
		m_Octaves.emplace_back(m_Seed, a_Frequency, a_Amplitude);
	}


	/** Fills a 2D array with the values of the noise. */
	void Generate2D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y]
		int a_SizeX, int a_SizeY,                        ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE * a_Workspace = nullptr           ///< Workspace that this function can use and trash.
	) const
	{
		// Check that state is alright:
		if (m_Octaves.empty())
		{
			ASSERT(!"cOctavedNoise: No octaves to generate!");
			return;
		}

		// Allocate the workspace on the heap, if it wasn't given:
		std::unique_ptr<NOISE_DATATYPE[]> workspaceHeap;
		if (a_Workspace == nullptr)
		{
			workspaceHeap.reset(new NOISE_DATATYPE[ToUnsigned(a_SizeX * a_SizeY)]);
			a_Workspace = workspaceHeap.get();
		}

		// Generate the first octave directly into array:
		int ArrayCount = a_SizeX * a_SizeY;
		{
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
		}

		// Add each octave:
		for (auto itr = m_Octaves.cbegin() + 1, end = m_Octaves.cend(); itr != end; ++itr)
		{
			// Generate the noise for the octave:
			itr->m_Noise.Generate2D(
				a_Workspace, a_SizeX, a_SizeY,
				a_StartX * itr->m_Frequency, a_EndX * itr->m_Frequency,
				a_StartY * itr->m_Frequency, a_EndY * itr->m_Frequency
			);
			// Add it into the output:
			NOISE_DATATYPE Amplitude = itr->m_Amplitude;
			for (int i = 0; i < ArrayCount; i++)
			{
				a_Array[i] += a_Workspace[i] * Amplitude;
			}
		}  // for itr - m_Octaves[]
	}


	/** Fills a 3D array with the values of the noise. */
	void Generate3D(
		NOISE_DATATYPE * a_Array,                        ///< Array to generate into [x + a_SizeX * y + a_SizeX * a_SizeY * z]
		int a_SizeX, int a_SizeY, int a_SizeZ,           ///< Count of the array, in each direction
		NOISE_DATATYPE a_StartX, NOISE_DATATYPE a_EndX,  ///< Noise-space coords of the array in the X direction
		NOISE_DATATYPE a_StartY, NOISE_DATATYPE a_EndY,  ///< Noise-space coords of the array in the Y direction
		NOISE_DATATYPE a_StartZ, NOISE_DATATYPE a_EndZ,  ///< Noise-space coords of the array in the Z direction
		NOISE_DATATYPE * a_Workspace = nullptr           ///< Workspace that this function can use and trash, same size as a_Array
	) const
	{
		// Check that state is alright:
		if (m_Octaves.empty())
		{
			ASSERT(!"cOctavedNoise: No octaves to generate!");
			return;
		}

		// Allocate the workspace on the heap, if it wasn't given:
		std::unique_ptr<NOISE_DATATYPE[]> workspaceHeap;
		if (a_Workspace == nullptr)
		{
			workspaceHeap.reset(new NOISE_DATATYPE[ToUnsigned(a_SizeX * a_SizeY * a_SizeZ)]);
			a_Workspace = workspaceHeap.get();
		}

		// Generate the first octave directly into array:
		int ArrayCount = a_SizeX * a_SizeY * a_SizeZ;
		{
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
		}

		// Add each octave:
		for (auto itr = m_Octaves.cbegin() + 1, end = m_Octaves.cend(); itr != end; ++itr)
		{
			// Generate the noise for the octave:
			itr->m_Noise.Generate3D(
				a_Workspace, a_SizeX, a_SizeY, a_SizeZ,
				a_StartX * itr->m_Frequency, a_EndX * itr->m_Frequency,
				a_StartY * itr->m_Frequency, a_EndY * itr->m_Frequency,
				a_StartZ * itr->m_Frequency, a_EndZ * itr->m_Frequency
			);
			// Add it into the output:
			NOISE_DATATYPE Amplitude = itr->m_Amplitude;
			for (int i = 0; i < ArrayCount; i++)
			{
				a_Array[i] += a_Workspace[i] * Amplitude;
			}
		}  // for itr - m_Octaves[]
	}

protected:
	/** Stores information and state for one octave of the noise. */
	class cOctave
	{
	public:
		N m_Noise;

		/** Coord multiplier. */
		NOISE_DATATYPE m_Frequency;

		/** Value multiplier. */
		NOISE_DATATYPE m_Amplitude;

		cOctave(int a_Seed, NOISE_DATATYPE a_Frequency, NOISE_DATATYPE a_Amplitude) :
			m_Noise(a_Seed),
			m_Frequency(a_Frequency),
			m_Amplitude(a_Amplitude)
		{
		}
	} ;
	typedef std::vector<cOctave> cOctaves;


	/** The seed used by the underlying generators. */
	int m_Seed;

	/** The octaves that compose this noise. */
	cOctaves m_Octaves;
};




