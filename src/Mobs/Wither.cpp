
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "Wither.h"

#include "../World.h"
#include "../Entities/Player.h"





cWither::cWither(void) :
	super("Wither", mtWither, "entity.wither.hurt", "entity.wither.death", 0.9, 4.0),
	m_WitherInvulnerableTicks(220)
{
	SetMaxHealth(300);
	SetHealth(GetMaxHealth() / 3);
}





bool cWither::IsArmored(void) const
{
	return GetHealth() <= (GetMaxHealth() / 2);
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





void cWither::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

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





void cWither::KilledBy(TakeDamageInfo & a_TDI)
{
	super::KilledBy(a_TDI);

	class cPlayerCallback : public cPlayerListCallback
	{
		Vector3f m_Pos;

		virtual bool Item(cPlayer * a_Player)
		{
			// TODO 2014-05-21 xdot: Vanilla minecraft uses an AABB check instead of a radius one
			double Dist = (a_Player->GetPosition() - m_Pos).Length();
			if (Dist < 50.0)
			{
				// If player is close, award achievement
				a_Player->AwardAchievement(achKillWither);
			}
			return false;
		}

	public:
		cPlayerCallback(const Vector3f & a_Pos) : m_Pos(a_Pos) {}

	} PlayerCallback(GetPosition());

	m_World->ForEachPlayer(PlayerCallback);
}




