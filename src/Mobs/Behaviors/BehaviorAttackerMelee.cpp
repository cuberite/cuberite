#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAttackerMelee.h"
#include "../Monster.h"
#include "../../Entities/Pawn.h"
#include "../../BlockID.h"

cBehaviorAttackerMelee::cBehaviorAttackerMelee(PostAttackFunction a_PostAttackFunction)
	: m_PostAttackFunction(a_PostAttackFunction)
{

}





bool cBehaviorAttackerMelee::DoStrike(int a_StrikeTickCnt)
{
	UNUSED(a_StrikeTickCnt);
	GetTarget()->TakeDamage(dtMobAttack, m_Parent, m_AttackDamage, 0);
	if (m_PostAttackFunction != nullptr)
	{
		m_PostAttackFunction(*this, *m_Parent, *GetTarget());
	}
	return true; // Finish the strike. It only takes 1 tick.
}
