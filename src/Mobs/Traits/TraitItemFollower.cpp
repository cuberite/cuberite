#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "TraitItemFollower.h"
#include "../Monster.h"
#include "../../World.h"
#include "../../Entities/Player.h"

iTraitItemFollower::~iTraitItemFollower()
{

}

cTraitItemFollower::cTraitItemFollower(iTraitItemFollower * a_ParentInterface) :
	m_ParentInterface(a_ParentInterface)
{
	m_Parent = dynamic_cast<cMonster *>(m_ParentInterface);
	ASSERT(m_Parent != nullptr);
}





bool cTraitItemFollower::ActiveTick()
{
	cWorld * World = m_Parent->GetWorld();
	cItems FollowedItems;
	m_ParentInterface->GetFollowedItems(FollowedItems);
	if (FollowedItems.Size() > 0)
	{
		cPlayer * a_Closest_Player = World->FindClosestPlayer(m_Parent->GetPosition(), static_cast<float>(m_Parent->GetSightDistance()));
		if (a_Closest_Player != nullptr)
		{
			cItem EquippedItem = a_Closest_Player->GetEquippedItem();
			if (FollowedItems.ContainsType(EquippedItem))
			{
				Vector3d PlayerPos = a_Closest_Player->GetPosition();
				m_Parent->MoveToPosition(PlayerPos);
				return true;  // We took control of the monster, prevent other traits from doing so
			}
		}
	}
	return false;
}
