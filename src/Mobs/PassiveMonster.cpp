
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "PassiveMonster.h"
#include "../World.h"
#include "../Entities/Player.h"




cPassiveMonster::cPassiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height) :
	super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_Width, a_Height)
{
	m_EMPersonality = PASSIVE;
}





bool cPassiveMonster::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if (!super::DoTakeDamage(a_TDI))
	{
		return false;
	}
	if ((a_TDI.Attacker != this) && (a_TDI.Attacker != nullptr))
	{
		m_EMState = ESCAPING;
	}
	return true;
}





void cPassiveMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	if (m_EMState == ESCAPING)
	{
		CheckEventLostPlayer();
	}
	cItems FollowedItems;
	GetFollowedItems(FollowedItems);
	if (FollowedItems.Size() <= 0)
	{
		return;
	}
	cPlayer * a_Closest_Player = m_World->FindClosestPlayer(GetPosition(), static_cast<float>(m_SightDistance));
	if (a_Closest_Player != nullptr)
	{
		cItem EquippedItem = a_Closest_Player->GetEquippedItem();
		if (FollowedItems.ContainsType(EquippedItem))
		{
			Vector3d PlayerPos = a_Closest_Player->GetPosition();
			MoveToPosition(PlayerPos);
		}
	}
}





