#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAttackerMelee.h"
#include "../Monster.h"
#include "../../Entities/Pawn.h"

bool cBehaviorAttackerMelee::StrikeTarget(std::chrono::milliseconds a_Dt, cChunk & a_Chunk, int a_StrikeTickCnt)
{
	GetTarget()->TakeDamage(dtMobAttack, this, m_AttackDamage, 0);
	return true;
}
