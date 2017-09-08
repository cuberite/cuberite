#pragma once

#include "Behavior.h"

/** Makes the mob tamable. Once tamed, the mob will have an owner and will follow it around.
The mob can optionally be granted sitting ability via the CanSit flag, and the ability to defend
its owner via the IsProtector flag.

If the mob is not sitting and the owner strays too far, the mob will teleport to the owner.

This base class behavior does not permit in-game taming (except when used by horses).
The derived classes determine how the mob should be tamed and they should be used instead.

Connections to other behaviors:
 - Suppresses cBehaviorBreeder until the mob has an owner.
 - If IsProtecter is set to true, the mob should also have a cBehaviorAttacker

Special connections:
 - relies on cMonster::NeverDespawn (TODO) and ensures the mob never despawns if it has an owner.
 - mobTodo cMonster::NeverDespawn + cWorld changes
 - Horse taming is horse-specific, so the horse taming logic is in the horse class, and
it will call cBehaviorBrave::SetOwner once tamed.

*/
class cBehaviorBrave : cBehavior
{
public:
	void AttachToMonster(cMonster & a_Parent);
	void DoTakeDamage(TakeDamageInfo & a_TDI) override;

private:
	cMonster * m_Parent;  // Our Parent
};
