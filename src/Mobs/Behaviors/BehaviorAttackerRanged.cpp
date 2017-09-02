#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorAttackerRanged.h"
#include "../Monster.h"
#include "../../Entities/Pawn.h"
#include "../../BlockID.h"
#include "../../Entities/ArrowEntity.h"

bool cBehaviorAttackerRanged::DoStrike(int a_StrikeTickCnt)
{
	UNUSED(a_StrikeTickCnt);
	auto & Random = GetRandomProvider();
	if ((GetTarget() != nullptr) && (m_AttackCoolDownTicksLeft == 0))
	{
		Vector3d Inaccuracy = Vector3d(Random.RandReal<double>(-0.25, 0.25), Random.RandReal<double>(-0.25, 0.25), Random.RandReal<double>(-0.25, 0.25));
		Vector3d Speed = (GetTarget()->GetPosition() + Inaccuracy - m_Parent->GetPosition()) * 5;
		Speed.y += Random.RandInt(-1, 1);

		auto Arrow = cpp14::make_unique<cArrowEntity>(m_Parent, m_Parent->GetPosX(), m_Parent->GetPosY() + 1, m_Parent->GetPosZ(), Speed);
		auto ArrowPtr = Arrow.get();
		ArrowPtr->Initialize(std::move(Arrow), *m_Parent->GetWorld());
	}
	return true; // Finish the strike. It only takes 1 tick.
}
