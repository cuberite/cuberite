
// ProbabDistrib.h

// Declares the cProbabDistrib class representing a discrete probability distribution curve and random generator

/*
Usage:
1, Create a cProbabDistrib instance
2, Initialize the distribution either programmatically, using the SetPoints() function, or using a definition string
3, Ask for random numbers in that probability distribution using the Random() function
*/





#pragma once





#include "FastRandom.h"





class cProbabDistrib
{
public:
	class cPoint
	{
	public:
		int m_Value;
		int m_Probability;

		cPoint(int a_Value, int a_Probability) :
			m_Value(a_Value),
			m_Probability(a_Probability)
		{
		}
	} ;

	typedef std::vector<cPoint> cPoints;


	cProbabDistrib(int a_MaxValue);

	/** Sets the distribution curve using an array of [value, probability] points, linearly interpolated. a_Points must not be empty. */
	void SetPoints(const cPoints & a_Points);

	/** Sets the distribution curve using a definition string; returns true on successful parse */
	bool SetDefString(const AString & a_DefString);

	/** Gets a random value from a_Rand, shapes it into the distribution curve and returns the value. */
	int Random(MTRand & a_Rand) const;

	/** Maps value in range [0, m_Sum] into the range [0, m_MaxValue] using the stored probability */
	int MapValue(int a_OrigValue) const;

	int GetSum(void) const { return m_Sum; }

protected:

	int m_MaxValue;

	/** Cumulative probability of the values, sorted, for fast bsearch lookup */
	cPoints m_Cumulative;

	/** Sum of all the probabilities across all values in the domain; -1 if not set */
	int m_Sum;
} ;




