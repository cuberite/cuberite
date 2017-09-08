#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAttackerSuicideBomber.h"
#include "../Monster.h"
#include "../../Entities/Pawn.h"
#include "../../Entities/Player.h"
#include "../../BlockID.h"




cBehaviorAttackerSuicideBomber::cBehaviorAttackerSuicideBomber(bool a_ShouldBroadcastEntityMetadata) :
	m_bIsBlowing(false),
	m_bIsCharged(false),
	m_BurnedWithFlintAndSteel(false),
	m_ShouldBroadcastEntityMetadata(a_ShouldBroadcastEntityMetadata)
{

}





void cBehaviorAttackerSuicideBomber::AttachToMonster(cMonster & a_Parent)
{
	cBehaviorAttacker::AttachToMonster(a_Parent);
	m_Parent->AttachRightClickBehavior(this);
}





bool cBehaviorAttackerSuicideBomber::DoStrike(int a_StrikeTickCnt)
{
	UNUSED(a_StrikeTickCnt);

	// phase 1: start blowing up
	if (a_StrikeTickCnt == 1)
	{
		ASSERT(!m_bIsBlowing);

		m_Parent->GetWorld()->BroadcastSoundEffect("entity.creeper.primed", m_Parent->GetPosX(), m_Parent->GetPosY(), m_Parent->GetPosZ(), 1.f, (0.75f + (static_cast<float>((m_Parent->GetUniqueID() * 23) % 32)) / 64));
		m_bIsBlowing = true;
		if (m_ShouldBroadcastEntityMetadata)
		{
			m_Parent->GetWorld()->BroadcastEntityMetadata(*m_Parent);
		}
		return false;
	}

	ASSERT(m_bIsBlowing);
	if (((GetTarget() == nullptr) || (!TargetIsInStrikeRadius())) && (!m_BurnedWithFlintAndSteel))
	{
		m_bIsBlowing = false;
		if (m_ShouldBroadcastEntityMetadata)
		{
			m_Parent->GetWorld()->BroadcastEntityMetadata(*m_Parent);
		}
		return true;
	}

	if (a_StrikeTickCnt == 30)
	{
		ASSERT(m_Parent->GetHealth() > 0.0);
		m_Parent->GetWorld()->DoExplosionAt((m_bIsCharged ? 5 : 3), m_Parent->GetPosX(), m_Parent->GetPosY(), m_Parent->GetPosZ(), false, esMonster, this);
		m_Parent->Destroy();  // Just in case we aren't killed by the explosion
		return true;
	}

	return false;
}





void cBehaviorAttackerSuicideBomber::OnRightClicked(cPlayer & a_Player)
{
	if ((a_Player.GetEquippedItem().m_ItemType == E_ITEM_FLINT_AND_STEEL))
	{
		if (!a_Player.IsGameModeCreative())
		{
			a_Player.UseEquippedItem();
		}
		if (!m_BurnedWithFlintAndSteel)
		{
			m_BurnedWithFlintAndSteel = true;
			Strike();
		}
	}
}


bool cBehaviorAttackerSuicideBomber::IsBlowing(void) const
{
	return m_bIsBlowing;
}





bool cBehaviorAttackerSuicideBomber::IsCharged(void) const
{
	return m_bIsCharged;
}





bool cBehaviorAttackerSuicideBomber::IsBurnedWithFlintAndSteel(void) const
{
	return m_BurnedWithFlintAndSteel;
}





void cBehaviorAttackerSuicideBomber::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (a_TDI.DamageType == dtLightning)
	{
		m_bIsCharged = true;
		m_Parent->GetWorld()->BroadcastEntityMetadata(*m_Parent);
	}
	cBehaviorAttacker::DoTakeDamage(a_TDI);
}
