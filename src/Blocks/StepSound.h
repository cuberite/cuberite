
// StepSound.h

// Handle the sounds from a block: place, break, step





#pragma once

#include "Defines.h"





class cStepSound
{
public:

	cStepSound(const AString & a_PlaceSound, const AString & a_BreakSound, const AString & a_StepSound, float a_Volume1 = 1.0F, float a_Volume2 = 1.0F) :
		m_PlaceSound(a_PlaceSound),
		m_BreakSound(a_BreakSound),
		m_StepSound(a_StepSound),
		m_SoundName(""),
		m_Volume1(a_Volume1),
		m_Volume2(a_Volume2)
	{}

	cStepSound(const AString & a_SoundName, float a_Volume1 = 1.0F, float a_Volume2 = 1.0F) :
		m_PlaceSound(""),
		m_BreakSound(""),
		m_StepSound(""),
		m_SoundName(a_SoundName),
		m_Volume1(a_Volume1),
		m_Volume2(a_Volume2)
	{}

	AString GetPlaceSound() const
	{
		if (m_PlaceSound.empty())
		{
			return Printf("dig.%s", m_SoundName.c_str());
		}
		return m_PlaceSound;
	}

	AString GetBreakSound() const
	{
		if (m_BreakSound.empty())
		{
			return Printf("dig.%s", m_SoundName.c_str());
		}
		return m_BreakSound;
	}

	AString GetStepSound() const
	{
		if (m_StepSound.empty())
		{
			return Printf("step.%s", m_SoundName.c_str());
		}
		return m_StepSound;
	}

	int GetVolume1() const
	{
		return m_Volume1;
	}

	int GetVolume2() const
	{
		return m_Volume2;
	}

protected:
	AString m_PlaceSound, m_BreakSound, m_StepSound;

	AString m_SoundName;

	float m_Volume1, m_Volume2;
} ;



