/**
 * Copyright 2014 MCServer Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// ProbabDistrib.h

// Declares the cProbabDistrib class representing a discrete probability distribution curve and random generator

/*
Usage:
1, Create a cProbabDistrib instance
2, Initialize the distribution either programmatically, using the SetPoints() function, or using a definition string
3, Ask for random numbers in that probability distribution using the Random() function
*/





#pragma once





// fwd:
class MTRand;





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
	
	/// Sets the distribution curve using an array of [value, probability] points, linearly interpolated. a_Points must not be empty.
	void SetPoints(const cPoints & a_Points);
	
	/// Sets the distribution curve using a definition string; returns true on successful parse
	bool SetDefString(const AString & a_DefString);
	
	/// Gets a random value from a_Rand, shapes it into the distribution curve and returns the value.
	int Random(MTRand & a_Rand) const;
	
	/// Maps value in range [0, m_Sum] into the range [0, m_MaxValue] using the stored probability
	int MapValue(int a_OrigValue) const;
	
	int GetSum(void) const { return m_Sum; }
	
protected:

	int     m_MaxValue;
	cPoints m_Cumulative;  ///< Cumulative probability of the values, sorted, for fast bsearch lookup
	int     m_Sum;  ///< Sum of all the probabilities across all values in the domain; -1 if not set
} ;




