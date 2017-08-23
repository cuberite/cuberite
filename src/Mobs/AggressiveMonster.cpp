
#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "AggressiveMonster.h"

#include "../World.h"
#include "../Entities/Player.h"
#include "../Tracer.h"
#include "Behaviors/BehaviorAggressive.h"
#include "Behaviors/BehaviorChaser.h"
#include "Behaviors/BehaviorWanderer.h"



cAggressiveMonster::cAggressiveMonster(const AString & a_ConfigName, eMonsterType a_MobType, const AString & a_SoundHurt, const AString & a_SoundDeath, double a_Width, double a_Height) :
	super(a_ConfigName, a_MobType, a_SoundHurt, a_SoundDeath, a_Width, a_Height), m_BehaviorAggressive(this)
{
	m_EMPersonality = AGGRESSIVE;
	ASSERT(GetBehaviorChaser() != nullptr);
}






void cAggressiveMonster::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	super::Tick(a_Dt, a_Chunk);

	cBehaviorChaser * BehaviorChaser = GetBehaviorChaser();
	cBehaviorWanderer * BehaviorWanderer = GetBehaviorWanderer();

	for (;;)
	{
		m_BehaviorAggressive.Tick();
		if (BehaviorChaser->ActiveTick())
		{
			break;
		}
		if ((BehaviorWanderer != nullptr) && BehaviorWanderer->ActiveTick(a_Dt, a_Chunk))
		{
			break;
		}

		ASSERT(!"Not a single Behavior took control, this is not normal. ");
		break;
	}

	BehaviorChaser->Tick();
}
