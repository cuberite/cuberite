#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorBrave.h"
#include "BehaviorAttacker.h"
#include "../Monster.h"
#include "../../Entities/Entity.h"

void cBehaviorBrave::AttachToMonster(cMonster & a_Parent)
{
	m_Parent = &a_Parent;
	m_Parent->AttachDoTakeDamageBehavior(this);
}




void cBehaviorBrave::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	cBehaviorAttacker * AttackBehavior = m_Parent->GetBehaviorAttacker();
	if ((AttackBehavior != nullptr) && (a_TDI.Attacker != m_Parent) &&
		(a_TDI.Attacker != nullptr) && (a_TDI.Attacker->IsPawn())
	)
	{
			AttackBehavior->SetTarget(static_cast<cPawn*>(a_TDI.Attacker));
	}
}

