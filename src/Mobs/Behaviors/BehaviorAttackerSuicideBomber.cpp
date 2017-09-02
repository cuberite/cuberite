#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAttackerSuicideBomber.h"
#include "../Monster.h"
#include "../../Entities/Pawn.h"
#include "../../Entities/Player.h"
#include "../../BlockID.h"




cBehaviorAttackerSuicideBomber::cBehaviorAttackerSuicideBomber() :
	m_bIsBlowing(false),
	m_bIsCharged(false),
	m_BurnedWithFlintAndSteel(false),
	m_ExplodingTimer(0)
{

}





void cBehaviorAttackerSuicideBomber::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (((GetTarget() == nullptr) || !TargetIsInStrikeRadius()) && !m_BurnedWithFlintAndSteel)
	{
		if (m_bIsBlowing)
		{
			m_ExplodingTimer = 0;
			m_bIsBlowing = false;
			m_Parent->GetWorld()->BroadcastEntityMetadata(*m_Parent);
		}
	}
	else
	{
		if (m_bIsBlowing)
		{
			m_ExplodingTimer += 1;
		}

		if ((m_ExplodingTimer == 30) && (m_Parent->GetHealth() > 0.0))  // only explode when not already dead
		{
			m_Parent->GetWorld()->DoExplosionAt((m_bIsCharged ? 5 : 3), m_Parent->GetPosX(), m_Parent->GetPosY(), m_Parent->GetPosZ(), false, esMonster, this);
			m_Parent->Destroy();  // Just in case we aren't killed by the explosion
		}
	}

	cBehaviorAttacker::Tick(a_Dt, a_Chunk);
}





bool cBehaviorAttackerSuicideBomber::StrikeTarget(int a_StrikeTickCnt)
{
	UNUSED(a_StrikeTickCnt);

	if (!m_bIsBlowing)
	{
		m_Parent->GetWorld()->BroadcastSoundEffect("entity.creeper.primed", m_Parent->GetPosX(), m_Parent->GetPosY(), m_Parent->GetPosZ(), 1.f, (0.75f + (static_cast<float>((m_Parent->GetUniqueID() * 23) % 32)) / 64));
		m_bIsBlowing = true;
		m_Parent->GetWorld()->BroadcastEntityMetadata(*m_Parent);

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
		m_Parent->GetWorld()->BroadcastSoundEffect("entity.creeper.primed", m_Parent->GetPosX(), m_Parent->GetPosY(), m_Parent->GetPosZ(), 1.f, (0.75f + (static_cast<float>((m_Parent->GetUniqueID() * 23) % 32)) / 64));
		m_bIsBlowing = true;
		m_Parent->GetWorld()->BroadcastEntityMetadata(*m_Parent);
		m_BurnedWithFlintAndSteel = true;
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
