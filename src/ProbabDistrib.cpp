
// ProbabDistrib.cpp

// Implements the cProbabDistrib class representing a discrete probability distribution curve and random generator

#include "Globals.h"
#include "ProbabDistrib.h"
#include "FastRandom.h"






cProbabDistrib::cProbabDistrib(int a_MaxValue) :
	m_MaxValue(a_MaxValue),
	m_Sum(-1)
{
}






void cProbabDistrib::SetPoints(const cProbabDistrib::cPoints & a_Points)
{
	ASSERT(!a_Points.empty());
	m_Sum = 0;
	m_Cumulative.clear();
	m_Cumulative.reserve(a_Points.size() + 1);
	int ProbSum = 0;
	int LastProb = 0;
	int LastValue = -1;
	if (a_Points[0].m_Value != 0)
	{
		m_Cumulative.push_back(cPoint(0, 0));  // Always push in the [0, 0] point for easier search algorithm bounds
		LastValue = 0;
	}
	for (cPoints::const_iterator itr = a_Points.begin(), end = a_Points.end(); itr != end; ++itr)
	{
		if (itr->m_Value == LastValue)
		{
			continue;
		}
		
		// Add the current trapezoid to the sum:
		ProbSum += (LastProb + itr->m_Probability) * (itr->m_Value - LastValue) / 2;
		LastProb = itr->m_Probability;
		LastValue = itr->m_Value;
		m_Cumulative.push_back(cPoint(itr->m_Value, ProbSum));
	}  // for itr - a_Points[]
	if (LastValue != m_MaxValue)
	{
		m_Cumulative.push_back(cPoint(m_MaxValue, 0));  // Always push in the last point for easier search algorithm bounds
	}
	m_Sum = ProbSum;
}





bool cProbabDistrib::SetDefString(const AString & a_DefString)
{
	AStringVector Points = StringSplitAndTrim(a_DefString, ";");
	if (Points.empty())
	{
		return false;
	}
	cPoints Pts;
	for (AStringVector::const_iterator itr = Points.begin(), end = Points.end(); itr != end; ++itr)
	{
		AStringVector Split = StringSplitAndTrim(*itr, ",");
		if (Split.size() != 2)
		{
			// Bad format
			return false;
		}
		int Value = atoi(Split[0].c_str());
		int Prob  = atoi(Split[1].c_str());
		if (
			((Value == 0) && (Split[0] != "0")) ||
			((Prob  == 0) && (Split[1] != "0"))
		)
		{
			// Number parse error
			return false;
		}
		Pts.push_back(cPoint(Value, Prob));
	}  // for itr - Points[]
	
	SetPoints(Pts);
	return true;
}





int cProbabDistrib::Random(MTRand & a_Rand) const
{
	int v = a_Rand.randInt(m_Sum);
	return MapValue(v);
}





int cProbabDistrib::MapValue(int a_OrigValue) const
{
	ASSERT(a_OrigValue >= 0);
	ASSERT(a_OrigValue < m_Sum);
	
	// Binary search through m_Cumulative for placement:
	size_t Lo = 0;
	size_t Hi = m_Cumulative.size() - 1;
	while (Hi - Lo > 1)
	{
		size_t Mid = (Lo + Hi) / 2;
		int MidProbab = m_Cumulative[Mid].m_Probability;
		if (MidProbab < a_OrigValue)
		{
			Lo = Mid;
		}
		else
		{
			Hi = Mid;
		}
	}
	ASSERT(Hi - Lo == 1);
	
	// Linearly interpolate between Lo and Hi:
	int ProbDif  = m_Cumulative[Hi].m_Probability - m_Cumulative[Lo].m_Probability;
	ProbDif = (ProbDif != 0) ? ProbDif : 1;
	int ValueDif = m_Cumulative[Hi].m_Value - m_Cumulative[Lo].m_Value;
	return m_Cumulative[Lo].m_Value + (a_OrigValue - m_Cumulative[Lo].m_Probability) * ValueDif / ProbDif;
}




