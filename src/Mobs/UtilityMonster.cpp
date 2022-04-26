
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "UtilityMonster.h"
#include "LineBlockTracer.h"
#include "../World.h"





cUtilityMonster::cUtilityMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, const AString & a_SoundAmbient, float a_Width, float a_Height) :
	Super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_SoundAmbient, a_Width, a_Height)
{
	m_EMPersonality = PASSIVE;
}





void cUtilityMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	Super::Tick(a_Dt, a_Chunk);
	if (!IsTicking())
	{
		// The base class tick destroyed us
		return;
	}

	// Set or clear m_Target:
	if (m_EMState == CHASING)
	{
		CheckEventSeeMob(a_Chunk);
	}
	else
	{
		CheckEventLoseMob(a_Dt);
	}
}





// Checks to see if there are hostile mobs nearby
void cUtilityMonster::CheckEventSeeMob(cChunk & a_Chunk)
{
	if (GetTarget() != nullptr)
	{
		return;
	}

	cMonster * TargetMonster = nullptr;
	double ClosestDistance = m_SightDistance * m_SightDistance;  // Squared
	const auto MyHeadPosition = GetPosition().addedY(GetHeight());

	// Enumerate all monsters within sight:
	m_World->ForEachEntityInBox(
		cBoundingBox(MyHeadPosition, m_SightDistance * 2),
		[&](cEntity & a_Entity)
	{
		if (!a_Entity.IsMob())
		{
			return false;
		}

		auto Monster = static_cast<cMonster *>(&a_Entity);

		if (Monster->GetMobFamily() != mfHostile)
		{
			return false;
		}

		// Snow golems will never attack ghasts
		if (Monster->GetMobType() == mtGhast)
		{
			return false;
		}

		const auto TargetHeadPosition = Monster->GetPosition().addedY(Monster->GetHeight());
		const auto TargetDistance = (TargetHeadPosition - MyHeadPosition).SqrLength();

		if (
			(TargetDistance < ClosestDistance) &&
			cLineBlockTracer::LineOfSightTrace(*GetWorld(), MyHeadPosition, TargetHeadPosition, cLineBlockTracer::losAirWater)
		)
		{
			TargetMonster = Monster;
			ClosestDistance = TargetDistance;
		}

		return false;
	});

	// Target it if suitable monster found:
	if (TargetMonster != nullptr)
	{
		EventSeeMob(TargetMonster, a_Chunk);
	}
}
