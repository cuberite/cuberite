
// StepSound.h

// Handle the sounds from a block: place, break, step





#pragma once

#include "Defines.h"
#include "StringUtils.h"





// tolua_begin
class cBlockSounds
{
public:

	cBlockSounds(const AString & a_PlaceSound, const AString & a_BreakSound, const AString & a_StepSound, float a_Volume1 = 1.0F, float a_Volume2 = 1.0F) :
		m_PlaceSound(a_PlaceSound),
		m_BreakSound(a_BreakSound),
		m_StepSound(a_StepSound),
		m_Volume1(a_Volume1),
		m_Volume2(a_Volume2)
	{}

	cBlockSounds(const AString & a_SoundName, float a_Volume1 = 1.0F, float a_Volume2 = 1.0F) :
		m_Volume1(a_Volume1),
		m_Volume2(a_Volume2)
	{
		SetBySoundName(a_SoundName);
	}

	cBlockSounds() :
		m_Volume1(1.0F),
		m_Volume2(1.0F)
	{
		SetBySoundName("stone");
	}

	void SetBySoundName(const AString & a_SoundName)
	{
		m_PlaceSound = Printf("dig.%s", a_SoundName.c_str());
		m_BreakSound = m_PlaceSound;
		m_StepSound = Printf("step.%s", a_SoundName.c_str());
	}

	AString m_PlaceSound, m_BreakSound, m_StepSound;
	float m_Volume1, m_Volume2;

	// tolua_end

} ;  // tolua_export



