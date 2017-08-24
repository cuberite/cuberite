#include "Globals.h"  // NOTE: MSVC stupidness requires this to be the same across all modules

#include "BehaviorItemFollower.h"
#include "../Monster.h"
#include "../../World.h"
#include "../../Entities/Player.h"


void cBehaviorItemFollower::AttachToMonster(cMonster & a_Parent)
{
	m_Parent = &a_Parent;
	m_Parent->AttachTickBehavior(this);
}





bool cBehaviorItemFollower::IsControlDesired(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	cItems FollowedItems;
	m_Parent->GetFollowedItems(FollowedItems);
	if (FollowedItems.Size() > 0)
	{
		cPlayer * a_Closest_Player = m_Parent->GetNearestPlayer();
		if (a_Closest_Player != nullptr)
		{
			cItem EquippedItem = a_Closest_Player->GetEquippedItem();
			if (FollowedItems.ContainsType(EquippedItem))
			{
				return true;  // We take control of the monster. Now it'll call our tick
			}
		}
	}
	return false;
}





void cBehaviorItemFollower::Tick(std::chrono::milliseconds a_Dt, cChunk & a_Chunk)
{
	UNUSED(a_Dt);
	UNUSED(a_Chunk);
	cItems FollowedItems;
	m_Parent->GetFollowedItems(FollowedItems);
	if (FollowedItems.Size() > 0)
	{
		cPlayer * a_Closest_Player = m_Parent->GetNearestPlayer();
		if (a_Closest_Player != nullptr)
		{
			cItem EquippedItem = a_Closest_Player->GetEquippedItem();
			if (FollowedItems.ContainsType(EquippedItem))
			{
				Vector3d PlayerPos = a_Closest_Player->GetPosition();
				m_Parent->MoveToPosition(PlayerPos);
			}
		}
	}
}
