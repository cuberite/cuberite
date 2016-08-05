#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "TraitCoward.h"
#include "../Monster.h"
#include "../../World.h"
#include "../../Entities/Player.h"
#include "../../Entities/Entity.h"

cTraitCoward::cTraitCoward(cMonster * a_Parent) :
	m_Parent(a_Parent),
	m_Attacker(nullptr)
{
	ASSERT(m_Parent != nullptr);
}





bool cTraitCoward::ActiveTick()
{
	if (m_Attacker == nullptr)
	{
		return false;
	}

	// TODO NOT SAFE
	if (m_Attacker->IsDestroyed() || (m_Attacker->GetPosition() - m_Parent->GetPosition()).Length() > m_Parent->GetSightDistance())
	{
		m_Attacker = nullptr;
		return false;
	}

	Vector3d newloc = m_Parent->GetPosition();
	newloc.x = (m_Attacker->GetPosition().x < newloc.x)? (newloc.x + m_Parent->GetSightDistance()): (newloc.x - m_Parent->GetSightDistance());
	newloc.z = (m_Attacker->GetPosition().z < newloc.z)? (newloc.z + m_Parent->GetSightDistance()): (newloc.z - m_Parent->GetSightDistance());
	m_Parent->MoveToPosition(newloc);
	return true;
}





void cTraitCoward::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if ((a_TDI.Attacker != m_Parent) && (a_TDI.Attacker != nullptr))
	{
		m_Attacker = a_TDI.Attacker;
	}
}

