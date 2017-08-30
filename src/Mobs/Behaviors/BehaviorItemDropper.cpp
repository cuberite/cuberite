#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorItemDropper.h"
#include "../Monster.h"
#include "../../World.h"


void cBehaviorItemDropper::AttachToMonster(cMonster & a_Parent)
{
	m_Parent = &a_Parent;
	m_EggDropTimer = 0;
	m_Parent->AttachPostTickBehavior(this);
}





void cBehaviorItemDropper::PostTick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	if (!m_Parent->IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	if (m_Parent->IsBaby())
	{
		return;  // Babies don't lay eggs
	}

	if ((m_EggDropTimer == 6000) && GetRandomProvider().RandBool())
	{
		cItems Drops;
		m_EggDropTimer = 0;
		Drops.push_back(cItem(E_ITEM_EGG, 1));
		m_Parent->GetWorld()->SpawnItemPickups(Drops, m_Parent->GetPosX(), m_Parent->GetPosY(), m_Parent->GetPosZ(), 10);
	}
	else if (m_EggDropTimer == 12000)
	{
		cItems Drops;
		m_EggDropTimer = 0;
		Drops.push_back(cItem(E_ITEM_EGG, 1));
		m_Parent->GetWorld()->SpawnItemPickups(Drops, m_Parent->GetPosX(), m_Parent->GetPosY(), m_Parent->GetPosZ(), 10);
	}
	else
	{
		m_EggDropTimer++;
	}
}
