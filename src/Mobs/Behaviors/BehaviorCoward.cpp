#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorCoward.h"
#include "../Monster.h"
#include "../../World.h"
#include "../../Entities/Player.h"
#include "../../Entities/Entity.h"

cBehaviorCoward::cBehaviorCoward() :
	m_Attacker(nullptr)
{
}




void cBehaviorCoward::AttachToMonster(cMonster & a_Parent)
{
	m_Parent = &a_Parent;
	m_Parent->AttachTickBehavior(this);
	m_Parent->AttachDoTakeDamageBehavior(this);
}





bool cBehaviorCoward::IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	return (m_Attacker != nullptr); //mobTodo probably not so safe pointer (and cChaser m_Target too)
}





bool cBehaviorCoward::ControlStarting(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	m_OldDontCare = m_Parent->GetPathFinder().getDontCare();
	m_Parent->GetPathFinder().setDontCare(true); // We don't care we're we are going when
	// wandering. If a path is not found, the pathfinder just modifies our destination.
	m_Parent->SetRelativeWalkSpeed(m_Parent->GetRelativeWalkSpeed() * 3);
	return true;
}


bool cBehaviorCoward::ControlEnding(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	m_Parent->SetRelativeWalkSpeed(m_Parent->GetRelativeWalkSpeed() / 3);
	m_Parent->GetPathFinder().setDontCare(m_OldDontCare);
	return true;
}





void cBehaviorCoward::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	if (m_Attacker == nullptr)
	{
		return;
	}

	// TODO NOT SAFE
	if (m_Attacker->IsDestroyed() || (m_Attacker->GetPosition() - m_Parent->GetPosition()).Length() > m_Parent->GetSightDistance())
	{
		// We lost the attacker
		m_Attacker = nullptr;
		return;
	}

	Vector3d newloc = m_Parent->GetPosition();
	newloc.x = (m_Attacker->GetPosition().x < newloc.x)? (newloc.x + m_Parent->GetSightDistance()): (newloc.x - m_Parent->GetSightDistance());
	newloc.z = (m_Attacker->GetPosition().z < newloc.z)? (newloc.z + m_Parent->GetSightDistance()): (newloc.z - m_Parent->GetSightDistance());
	m_Parent->MoveToPosition(newloc);
}





void cBehaviorCoward::DoTakeDamage(TakeDamageInfo & a_TDI)
{
	if ((a_TDI.Attacker != m_Parent) && (a_TDI.Attacker != nullptr))
	{
		m_Attacker = a_TDI.Attacker;
	}
}

