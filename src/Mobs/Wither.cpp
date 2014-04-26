
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Wither.h"
#include "../World.h"





cWither::cWither(void) :
	super("Wither", mtWither, "mob.wither.hurt", "mob.wither.death", 0.9, 4.0),
	m_WitherInvulnerableTicks(220)
{
	SetMaxHealth(300);
}





bool cWither::IsArmored(void) const
{
	return GetHealth() <= (GetMaxHealth() / 2);
}





bool cWither::Initialize(cWorld * a_World)
{
	// Set health before BroadcastSpawnEntity()
	SetHealth(GetMaxHealth() / 3);

	return super::Initialize(a_World);
}





bool cWither::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (a_TDI.DamageType == dtDrowning)
	{
		return false;
	}

	if (m_WitherInvulnerableTicks > 0)
	{
		return false;
	}

	if (IsArmored() && (a_TDI.DamageType == dtRangedAttack))
	{
		return false;
	}

	return super::DoTakeDamage(a_TDI);
}





void cWither::Tick(float a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (m_WitherInvulnerableTicks > 0)
	{
		unsigned int NewTicks = m_WitherInvulnerableTicks - 1;

		if (NewTicks == 0)
		{
			m_World->DoExplosionAt(7.0, GetPosX(), GetPosY(), GetPosZ(), false, esWitherBirth, this);
		}

		m_WitherInvulnerableTicks = NewTicks;

		if ((NewTicks % 10) == 0)
		{
			Heal(10);
		}
	}

	m_World->BroadcastEntityMetadata(*this);
}





void cWither::GetDrops(cItems & a_Drops, cEntity * a_Killer)
{
	AddRandomDropItem(a_Drops, 1, 1, E_ITEM_NETHER_STAR);
}




