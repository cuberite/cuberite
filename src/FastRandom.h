
// FastRandom.h

// Declares the cFastRandom class representing a fast random number generator

/*
The cFastRandom alias should be avoided in favor of the result of a call to GetRandomProvider().
The MTRand generator used is faster, has a better range and provides higher quality randomness.
Note that MTRand is relatively costly to construct and so instances should be long lived,
prefer calls to GetRandomProvider over creating new instances.
*/





#pragma once





namespace Detail
{
	/** Returns a low quality seed. */
	UInt32 GetRandomSeed();

	/** Aliases true_type if Char is any variant of char ignoring signed-ness. */
	template <class Char>
	using IsChar = typename std::is_same<typename std::make_signed<Char>::type, signed char>::type;

	template <class IntType>
	struct cUniformImpl :
		public std::conditional<
			IsChar<IntType>::value,
			typename std::conditional<  // Match signed-ness of IntType
				std::is_signed<IntType>::value,
				std::uniform_int_distribution<short>,
				std::uniform_int_distribution<unsigned short>
			>::type,
			std::uniform_int_distribution<IntType>
		>
	{
	};

	/** uniform_int_distribution<char> is undefined so this aliases a valid type. */
	template <class IntType>
	using cUniform = typename cUniformImpl<IntType>::type;
}





/** Class to wrap any random engine to provide a more convenient interface. */
template <class RandomEngine>
class cRandomWrapper
{
public:
	/** Initialize with a low quality seed. */
	cRandomWrapper():
		m_Engine(Detail::GetRandomSeed())
	{
	}


	/** Initialize with a SeedSequence. */
	template <class SeedSeq>
	cRandomWrapper(SeedSeq & a_SeedSeq):
		m_Engine(a_SeedSeq)
	{
	}



	/** Return a random IntType in the range [a_Min, a_Max]. */
	template <class IntType = int>
	IntType RandInt(IntType a_Min, IntType a_Max)
	{
		ASSERT(
			(a_Max >= a_Min) &&
			(a_Max <= std::numeric_limits<IntType>::max()) &&
			(a_Min >= std::numeric_limits<IntType>::min())
		);
		Detail::cUniform<IntType> dist(
			static_cast<IntType>(a_Min),
			static_cast<IntType>(a_Max)
		);
		return static_cast<IntType>(dist(m_Engine));
	}





	/** Return a random IntType in the range [0, a_Max]. */
	template <class IntType = int>
	IntType RandInt(IntType a_Max)
	{
		ASSERT((a_Max >= 0) && (a_Max <= std::numeric_limits<IntType>::max()));
		Detail::cUniform<IntType> dist(IntType(0), static_cast<IntType>(a_Max));
		return static_cast<IntType>(dist(m_Engine));
	}





	/** Return a random IntType in the range [0, std::numeric_limits<IntType>::max()]. */
	template <class IntType = int>
	IntType RandInt()
	{
		Detail::cUniform<IntType> dist(IntType(0), std::numeric_limits<IntType>::max());
		return static_cast<IntType>(dist(m_Engine));
	}





	/** Return a random RealType in the range [a_Min, a_Max). */
	template <class RealType = float>
	RealType RandReal(RealType a_Min, RealType a_Max)
	{
		std::uniform_real_distribution<RealType> dist(a_Min, a_Max);
		return dist(m_Engine);
	}





	/** Return a random RealType in the range [0, a_Max). */
	template <class RealType = float>
	RealType RandReal(RealType a_Max)
	{
		std::uniform_real_distribution<RealType> dist(RealType(0), a_Max);
		return dist(m_Engine);
	}





	/** Return a random RealType in the range [0, 1). */
	template <class RealType = float>
	RealType RandReal()
	{
		std::uniform_real_distribution<RealType> dist;
		return dist(m_Engine);
	}





	/** Return a random bool with the given probability of being true. */
	bool RandBool(double a_TrueProbability = 0.5)
	{
		std::bernoulli_distribution dist(a_TrueProbability);
		return dist(m_Engine);
	}




	/** Returns a reference to the underlying random engine. */
	RandomEngine & Engine()
	{
		return m_Engine;
	}

private:

	RandomEngine m_Engine;
};





/** Utility to seed a random engine with maximal entropy from random_device. */
struct cRandomDeviceSeeder
{
	using result_type = std::random_device::result_type;

	template <class Itr>
	void generate(Itr first, Itr last)
	{
		std::random_device rd;
		std::uniform_int_distribution<result_type> dist;
		for (; first != last; ++first)
		{
			*first = dist(rd);
		}
	}
};





using cFastRandom = cRandomWrapper<std::minstd_rand>;
using MTRand = cRandomWrapper<std::mt19937>;

/** Returns the current thread's random number source. */
MTRand & GetRandomProvider();
