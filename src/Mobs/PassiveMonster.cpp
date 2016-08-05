
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PassiveMonster.h"
#include "../World.h"
#include "../Entities/Player.h"
#include "BoundingBox.h"




cPassiveMonster::cPassiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height) :
	super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_Width, a_Height),
	m_TraitBreeder(this), m_TraitItemFollower(this), m_TraitCoward(this)
{
	m_EMPersonality = PASSIVE;
}





bool cPassiveMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!super::DoTakeDamage(a_TDI))
	{
		return false;
	}
	m_TraitCoward.DoTakeDamage(a_TDI);
	return true;
}





void cPassiveMonster::Destroyed()
{
	m_TraitBreeder.Destroyed();
	super::Destroyed();
}





cTraitBreeder & cPassiveMonster::GetTraitBreeder()
{
	return m_TraitBreeder;
}





const cTraitBreeder & cPassiveMonster::GetTraitBreeder() const
{
	return static_cast<const cTraitBreeder &>(m_TraitBreeder);
}


void cPassiveMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (m_EMState == ESCAPING)
	{
		CheckEventLostPlayer();
	}

	for (;;)
	{
		if (m_TraitCoward.ActiveTick())
		{
			break;
		}
		if (m_TraitBreeder.ActiveTick())
		{
			break;
		}
		if (m_TraitItemFollower.ActiveTick())
		{
			break;
		}
		if (super::m_TraitWanderer.ActiveTick(a_Dt, a_Chunk))
		{
			break;
		}

		ASSERT(!"Not a single trait took control, this is not normal. ");
		break;
	}

	m_TraitBreeder.Tick();
	// someOtherTrait.Tick();
	// someOtherTrait.Tick();
	// someOtherTrait.Tick();
	// someOtherTrait.Tick();
}





void cPassiveMonster::OnRightClicked(cPlayer & a_Player)
{
	super::OnRightClicked(a_Player);
	m_TraitBreeder.OnRightClicked(a_Player);
}
